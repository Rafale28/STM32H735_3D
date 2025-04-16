#pragma once
#include <stdio.h>
#include "app_main.h"
#include "memory_manager.h"

#define COLOR_BLACK   0xFF000000
#define COLOR_GLAY    0xFFF8F8F8
#define COLOR_RED     0xFFFF0000
#define COLOR_GREEN   0xFF00FF00
#define COLOR_BLUE    0xFF0000FF
#define COLOR_WHITE   0xFFFFFFFF

extern LTDC_HandleTypeDef hltdc; 

class DisplayManager {
public:
    DisplayManager(MemoryRegion& region, int width, int height) 
        : width(width), height(height), current(0) {
        size_t fb_size = width * height * sizeof(uint32_t);
        printf("fb1 alloc\n");
        fb[0] = static_cast<uint32_t*>(region.alloc(fb_size, "Framebuffer 0"));
        printf("fb2 alloc\n");
        fb[1] = static_cast<uint32_t*>(region.alloc(fb_size, "Framebuffer 1"));
        initDisplay();
    }

    void initDisplay() {
        size_t fb_size = width * height * sizeof(uint32_t);
        __HAL_LTDC_LAYER(&hltdc, 0)->CFBAR = (uint32_t)getDisplayBuffer();
        __HAL_LTDC_RELOAD_CONFIG(&hltdc); // 即時リロード
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);  // DISP = ON
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_SET);  // Backlight = ON
        memset(fb[0], 0, fb_size);
        memset(fb[1], 0, fb_size);
    }

    uint32_t* getDrawBuffer() {
        return fb[1 - current]; // 裏バッファに描画
    }

    uint32_t* getDisplayBuffer() {
        return fb[current]; // 表バッファ
    }

    void switchBuffer() {
        current = 1 - current;
        __HAL_LTDC_LAYER(&hltdc, 0)->CFBAR = (uint32_t)getDisplayBuffer();
        __HAL_LTDC_RELOAD_CONFIG(&hltdc); // 即時リロード
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void clear(uint32_t color);
    void drawPixel(int x, int y, uint32_t color);
    void drawLine(int x0, int y0, int x1, int y1, uint32_t color);

private:
    uint32_t* fb[2];
    int width;
    int height;
    int current; // 0 or 1
};