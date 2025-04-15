#include <stdio.h>
#include "main.h"
#include "app_main.h"
#include "memory_manager.h"

int app_main() {
    printf("app Main!!!\n");
    MemoryRegion HyperRam(OCTOSPI2_BASE, 16 * MiB);
    MemoryRegion AxiSram(D1_AXISRAM_BASE, 128 * KiB);

    MemoryManager test_ram(HyperRam, 64);
    uint32_t* test = (uint32_t *)test_ram.alloc(sizeof(uint32_t) * 3);
    test[0] = 0xFFFF0000;
    test[1] = 0x0000FFFF;
    test[2] = 0x1234ABCD;

    printf("addr:0x%08x, data:%08x\n", &test[0], test[0]);
    printf("addr:0x%08x, data:%08x\n", &test[1], test[1]);
    printf("addr:0x%08x, data:%08x\n", &test[2], test[2]);
    while (1) {
        HAL_GPIO_TogglePin(USER_LED1_GPIO_Port, USER_LED2_Pin);
        HAL_GPIO_TogglePin(USER_LED2_GPIO_Port, USER_LED1_Pin);
        HAL_Delay(500);
    }
    return 0;
}