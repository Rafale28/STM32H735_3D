#include "memory_manager.h"

MemoryRegion::MemoryRegion(uint32_t base_addr, size_t size){
    base = reinterpret_cast<uint8_t*>(base_addr);
    capacity = size;
    offset = 0;
}

void* MemoryRegion::alloc(size_t size) {
    size = align32(size);
    if (offset + size > capacity) return nullptr;
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
    if (offset + size > capacity) return nullptr;
    void* ptr = base + offset;
    offset += size;
    return ptr;
}