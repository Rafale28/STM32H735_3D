#pragma once
#include <cstdint>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <memory>

#include "display_manager.h"
#include "memory_manager.h"
#include "app_main.h"

#define FIXED_SHIFT 16
#define FIXED_ONE (1 << FIXED_SHIFT)
#define TO_FIXED(x) ((int32_t)((x) * FIXED_ONE))
#define FROM_FIXED(x) ((x) >> FIXED_SHIFT)

// Zバッファの実装方式切り替え（0: 固定小数点, 1: float）
#define USE_FLOAT_Z 1

struct Vertex {
    float x, y, z;
};

struct Face {
    int v1, v2, v3;
};

class Renderer3D {
public:
    Renderer3D(MemoryRegion& region, DisplayManager& display)
        : region(region), display(display), light_dir({0.0f, 0.0f, 1.0f}), 
            vertices(nullptr), faces(nullptr), vcount(0), fcount(0) {
        setZBuffer(display.getWidth(), display.getHeight());
    }


    void clearZBuffer(float far);
    bool loadModel(const char* filename);
    void draw(float angle_x, float angle_y, float angle_z, float scale , int cx , int cy);

private:
    MemoryRegion& region;
    DisplayManager& display;
    Vertex* vertices;
    Face* faces;
    int vcount;
    int fcount;

    Vertex light_dir;
#if USE_FLOAT_Z
    float* zbuffer = nullptr;
#else
    int32_t* zbuffer = nullptr;
#endif
    int zwidth = 0, zheight = 0;

    static Vertex rotate(const Vertex& v, float ax, float ay, float az);
    static Vertex normalize(const Vertex& v);
    static Vertex cross(const Vertex& a, const Vertex& b);
    static float dot(const Vertex& a, const Vertex& b);

    void setZBuffer(int width, int height);

#if USE_FLOAT_Z
    void drawTriangleZ(int x0, int y0, float z0,
                       int x1, int y1, float z1,
                       int x2, int y2, float z2,
                       float brightness);
#else
    void drawTriangleZ(int x0, int y0, int z0,
                       int x1, int y1, int z1,
                       int x2, int y2, int z2,
                       float brightness);
#endif
};