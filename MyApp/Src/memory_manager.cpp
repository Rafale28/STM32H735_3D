#include <stdio.h> 
#include "memory_manager.h"

MemoryRegion::MemoryRegion(const char* label, uint32_t base_addr, size_t total_size) 
    : label(label) {
    max_blocks = total_size / MIN_BLOCK_SIZE;
    size_t block_meta_size = max_blocks * sizeof(Block);

    if (block_meta_size > 64 * 1024) {  // 管理領域は最大64KBに制限
        max_blocks = (64 * 1024) / sizeof(Block);
        block_meta_size = max_blocks * sizeof(Block);
    }

    blocks = reinterpret_cast<Block*>(base_addr);
    base = reinterpret_cast<uint8_t*>(base_addr + block_meta_size);
    size = total_size - block_meta_size;

    block_count = 1;
    blocks[0] = { base, size, false };

    printf("[%s] usable=%u bytes, blocks=%u (meta=%u bytes)\n", label, size, max_blocks, block_meta_size);
}

void* MemoryRegion::alloc(size_t alloc_size, const char* b_label) {
    alloc_size = align_up(alloc_size, MIN_BLOCK_SIZE);
    for (size_t i = 0; i < block_count; ++i) {
        Block& blk = blocks[i];
        if (!blk.used && blk.size >= alloc_size) {
            if (blk.size > alloc_size) {
                blocks[block_count++] = { blk.ptr + alloc_size, blk.size - alloc_size, false };
                blk.size = alloc_size;
            }
            blk.used = true;
            blk.label = b_label;
            if (b_label) {
                printf("[%s][alloc] %u bytes at %p [%s]\n", label, (unsigned)alloc_size, blk.ptr, b_label);
            } else {
                printf("[%s][alloc] %u bytes at %p\n", label, (unsigned)alloc_size, blk.ptr);
            }
            return blk.ptr;
        }
    }
    printf("[%s] [alloc] FAILED for %u bytes", label, (unsigned)alloc_size);
    if (b_label) printf(" [%s]", b_label);
    printf("\n");
    return nullptr;
}

void MemoryRegion::free(void* ptr) {
    for (size_t i = 0; i < block_count; ++i) {
        if (blocks[i].ptr == ptr) {
            blocks[i].used = false;
            printf("[%s] [free] address %p\n", label, ptr);
            merge_free_blocks();
            return;
        }
    }
    printf("[%s] [free] address %p not found\n", label, ptr);
}

void MemoryRegion::merge_free_blocks() {
    for (size_t i = 0; i + 1 < block_count; ++i) {
        Block& a = blocks[i];
        Block& b = blocks[i + 1];
        if (!a.used && !b.used && a.ptr + a.size == b.ptr) {
            a.size += b.size;
            for (size_t j = i + 1; j + 1 < block_count; ++j) {
                blocks[j] = blocks[j + 1];
            }
            --block_count;
            --i;
        }
    }
}

void MemoryRegion::info() const {
    size_t used = 0, free = 0;
    printf("[%s][info] total=%u bytes\n", label, (unsigned)(size));
    for (size_t i = 0; i < block_count; ++i) {
        const Block& blk = blocks[i];
        const char* status = blk.used ? "USED " : "FREE ";
        if (blk.used) {
            printf("  [%s][%-15s] %p - %u bytes (%u blocks)\n",
                   status, blk.label ? blk.label : "N/A",
                   blk.ptr, (unsigned)blk.size, (unsigned)(blk.size / MIN_BLOCK_SIZE));
            used += blk.size;
        } else {
            printf("  [%s]              %p - %u bytes (%u blocks)\n",
                   status, blk.ptr, (unsigned)blk.size, (unsigned)(blk.size / MIN_BLOCK_SIZE));
            free += blk.size;
        }
    }
    float used_percent = (float)used * 100.0f / (used + free);
    float free_percent = 100.0f - used_percent;
    printf("[%s][summary] used=%u (%.1f%%), free=%u (%.1f%%)\n",
           label, (unsigned)used, used_percent, (unsigned)free, free_percent);
}