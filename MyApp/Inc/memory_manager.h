#pragma once
#include <cstddef>
#include <cstdint>

#define KiB 1024
#define MiB 1024 * 1024

static inline size_t align32(size_t size) {
    return (size + 31) & ~31;
}

class MemoryRegion {
public:
    MemoryRegion(const char* label, uint32_t base_addr, size_t total_size);
    ~MemoryRegion() {
        delete[] blocks;  // メモリリーク回避
    }
    void* alloc(size_t size, const char* b_label = nullptr);
    void free(void* ptr);
    void info() const;

private:
    struct Block {
        uint8_t* ptr;
        size_t size;
        bool used;
        const char* label = nullptr;
    };

    static constexpr size_t MIN_BLOCK_SIZE = 4096;  //4KiB Alignする

    size_t align_up(size_t val, size_t align) {
        return (val + align - 1) & ~(align - 1);
    }

    const char* label;
    uint8_t* base;
    size_t size;

    Block* blocks = nullptr;
    size_t block_count = 0;
    size_t max_blocks = 0;

    void merge_free_blocks();
};
