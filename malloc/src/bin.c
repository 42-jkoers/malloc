#include "main.h"

// @param mmap: the mmap in which the bin is located
t_bin bin_construct(const t_mmaps* maps, t_mmap* mmap, size_t size)
{
	assert(mmap_remaining_size(mmap) >= size);

	t_bin bin = {
		.p = mmap->end,
		.mmap_offset = (size_t)(mmap - maps->mmaps),
		.size = size,
		.status = USED};
	mmap->end += size;
	mmap->uses++;

	return bin;
}

t_mmap* bin_get_mmap(t_mmaps* maps, const t_bin* bin)
{
	return &maps->mmaps[bin->mmap_offset];
}
