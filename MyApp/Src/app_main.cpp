#include <stdio.h>
#include "main.h"
#include "app_main.h"
#include "memory_manager.h"
#include "display_manager.h"
#include "renderer3d.h"

bool is_sd_card_inserted(void) {
    return HAL_GPIO_ReadPin(GPIOF, uSD_Detect_Pin) == GPIO_PIN_RESET;
}
void check_sd_card(void) {
    f_mount(&SDFatFS, SDPath, 1);
    FRESULT res = f_mount(&SDFatFS, SDPath, 1);
    printf("f_mount result = %d\r\n", res);
    if (res == FR_OK) {
        printf("SD mounted!\r\n");
    } 
}

void open_testfile() {
    FIL file;
    char buffer[64];
    UINT br;
    if (f_open(&file, "test.txt", FA_READ) == FR_OK) {
        printf("Opened test.txt\r\n");
        while (f_read(&file, buffer, sizeof(buffer) - 1, &br) == FR_OK && br > 0) {
            buffer[br] = '\0';  // null終端
            printf("%s\n", buffer);  // UARTに出力
        }
        f_close(&file);
    } else {
        printf("Failed to open test.txt\r\n");
    }
}

int app_main() {
    printf("app Main!!!\n");
    MemoryRegion HyperRam("Hyper RAM", OCTOSPI2_BASE, 16 * MiB);
    MemoryRegion AxiSram("AXI SRAM", D1_AXISRAM_BASE, 128 * KiB);
    DisplayManager disp(HyperRam, 480, 272);
    Renderer3D render(HyperRam, disp);

#if 0
    MemoryManager test_ram(HyperRam, 64);
    uint32_t* test = (uint32_t *)test_ram.alloc(sizeof(uint32_t) * 3);
    test[0] = 0xFFFF0000;
    test[1] = 0x0000FFFF;
    test[2] = 0x1234ABCD;

    printf("addr:0x%08lx, data:%08lx\n", (uint32_t)&test[0], test[0]);
    printf("addr:0x%08lx, data:%08lx\n", (uint32_t)&test[1], test[1]);
    printf("addr:0x%08lx, data:%08lx\n", (uint32_t)&test[2], test[2]);
#endif

    if (!is_sd_card_inserted()) {
        printf("SDカードが挿入されていません！\r\n");
        while(!is_sd_card_inserted())HAL_Delay(50);
    } 
    HAL_Delay(100);
    check_sd_card();
    open_testfile();
    disp.clear(COLOR_BLUE);
    disp.switchBuffer();

    float angle_x = 0.0f;
    float angle_y = 1.0f;
    float angle_z = 0.0f;
    render.loadModel("delo.obj");
    printf("draw!!\n");
    render.draw(angle_x, angle_y, angle_z, 40.0f, 240, 280);
    disp.switchBuffer();
    HyperRam.info();
    while (1) {
        disp.clear(COLOR_BLACK);
        angle_y += 0.06f;  // 毎フレーム少しずつ回す
        if (angle_y > 2.0f * 3.1415926f) angle_y -= 2.0f * 3.1415926f;
        //DWORD start = HAL_GetTick(); 
        render.draw(angle_x, angle_y, angle_z, 40.0f, 240, 280);
        //printf("Draw Time %ldms\n", HAL_GetTick() - start);
        disp.switchBuffer();
        HAL_GPIO_TogglePin(USER_LED1_GPIO_Port, USER_LED2_Pin);
        HAL_GPIO_TogglePin(USER_LED2_GPIO_Port, USER_LED1_Pin);
        //HAL_Delay(50);
        //touch_read();
    }
    return 0;
}