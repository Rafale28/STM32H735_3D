#pragma once
#include <cstddef>
#include <cstdint>

#define KiB 1024
#define MiB 1024 * 1024

static inline size_t align32(size_t size) {
    return (size + 31) & ~31;
}
// メモリ領域管理（例：HyperRAMやSRAM）
class MemoryRegion {
public:
    MemoryRegion(uint32_t base_addr, size_t size);

    void* alloc(size_t size);
    void reset() { offset = 0; }
    size_t getUsed() const { return offset; }
    size_t getFree() const { return capacity - offset; }

private:
    uint8_t* base;
    size_t capacity;
    size_t offset;
};

// メモリマネージャ：任意の領域を使ってバッファ管理
class MemoryManager {
public:
    MemoryManager() = default;
    MemoryManager(uint32_t base_addr, size_t size) {
        init(base_addr, size);
    }
    MemoryManager(MemoryRegion& region, size_t size);
    void init(uint32_t start_addr, size_t size);
    void* alloc(size_t size);
    void reset() { offset = 0; }
    size_t getUsed() const { return offset; }
    size_t getFree() const { return capacity - offset; }
    void* getBase() const { return base; }

private:
    uint8_t* base = nullptr;
    size_t capacity = 0;
    size_t offset = 0;
};
