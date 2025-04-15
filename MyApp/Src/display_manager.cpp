    #include "display_manager.h"

    void DisplayManager::clear(uint32_t color = 0x00000000) {
        uint32_t* buf = getDrawBuffer();
        size_t total = width * height;
        for (size_t i = 0; i < total; ++i) {
            buf[i] = color;
        }
    }

    void DisplayManager::drawPixel(int x, int y, uint32_t color) {
        if (x < 0 || y < 0 || x >= width || y >= height) return;
        getDrawBuffer()[y * width + x] = color;
    }

    void DisplayManager::drawLine(int x0, int y0, int x1, int y1, uint32_t color) {
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;
        while (true) {
            drawPixel(x0, y0, color);
            if (x0 == x1 && y0 == y1) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }