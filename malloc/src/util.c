#include "util.h"
#include "main.h"
#include <sys/mman.h>

static void assert_valid_mmap_addr(void* ptr)
{
	assert(ptr);
	assert((size_t)ptr % PAGE_SIZE == 0);
}
// Assert size is valid for mmap
static void assert_valid_mmap_size(size_t size)
{
	assert(size);
	assert(size % PAGE_SIZE == 0);
}

size_t nearest_multiple_of(size_t number, size_t multiple)
{
	size_t remainder = number % multiple;
	if (remainder == 0)
		return number;
	return number + multiple - remainder;
}

static void* ft_mmap_unsafe(void* addr, size_t len, int flags)
{
	g_debug.mmap_calls++;
	return mmap(addr, len, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED | flags, -1, 0);
}

// safe mmap
void* ft_mmap(void* addr, size_t length)
{
	if (addr)
		assert_valid_mmap_addr(addr);
	assert_valid_mmap_size(length);

	void* map = ft_mmap_unsafe(addr, length, 0);
	assert(map != MAP_FAILED);
	assert(map);
	return map;
}

static e_result ft_mmap_try_grow(void* addr, size_t length)
{

	(void)addr;
	(void)length;
	return FAIL; // TODO: enable

	void* map = ft_mmap_unsafe(addr, length, MAP_FIXED);
	if (map == MAP_FAILED)
		return FAIL;
	if (!map)
		return FAIL;
	return SUCCESS;
}

// attemps to grow the mmap, if it fails
// it will create a new map and copy the old map into it
// and free the old map
void* ft_mmap_grow(void* map, size_t curr_size, size_t new_size)
{
	assert_valid_mmap_addr(map);
	assert_valid_mmap_size(curr_size);
	assert_valid_mmap_size(new_size);
	assert(new_size > curr_size);

	if (ft_mmap_try_grow(map, new_size) == SUCCESS)
		return map;

	void* new_map = ft_mmap(NULL, new_size);
	ft_memcpy(new_map, map, curr_size);
	// ft_munmap(map, curr_size); // TODO: enable
	return new_map;
}

void ft_munmap(void* addr, size_t length)
{
	assert_valid_mmap_addr(addr);
	assert_valid_mmap_size(length);
	g_debug.mumap_calls++;
	assert(munmap(addr, length) == 0);
}
