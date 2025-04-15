#include <stdio.h> 
#include "memory_manager.h"

MemoryRegion::MemoryRegion(uint32_t base_addr, size_t size){
    base = reinterpret_cast<uint8_t*>(base_addr);
    capacity = size;
    printf("MemRegion, base 0x%08x, capacity 0x%08x\n", base, capacity);
    offset = 0;
}

void* MemoryRegion::alloc(size_t size) {
    size = align32(size);
    if (offset + size > capacity) return nullptr;
    printf("Region alloc, start 0x%08x, size 0x%08x\n", base + offset, size);
    void* ptr = base + offset;
    offset += size;
    return ptr;
}

MemoryManager::MemoryManager(MemoryRegion& region, size_t size) {
    base = static_cast<uint8_t*>(region.alloc(size));
    capacity = size;
    offset = 0;
}

void MemoryManager::init(uint32_t start_addr, size_t size) {
    base = reinterpret_cast<uint8_t*>(start_addr);
    capacity = size;
    offset = 0;
}

void* MemoryManager::alloc(size_t size) {
    size = align32(size);
    printf("base 0x%08x, offset 0x%08x, size 0x%08x, capacity 0x%08x\n", base, offset, size, capacity);
    if (offset + size > capacity) return nullptr;
    void* ptr = base + offset;
    offset += size;
    return ptr;
}