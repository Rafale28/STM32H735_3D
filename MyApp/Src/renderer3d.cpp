#include "renderer3d.h"


void Renderer3D::setZBuffer(int w, int h) {
    zwidth = w;
    zheight = h;
#if USE_FLOAT_Z
    zbuffer = static_cast<float*>(region.alloc(sizeof(float) * w * h));
#else
    zbuffer = static_cast<int32_t*>(region.alloc(sizeof(int32_t) * w * h));
#endif
}

void Renderer3D::clearZBuffer(float far = 10000.0f) {
    if (!zbuffer) return;
#if USE_FLOAT_Z
    for (int i = 0; i < zwidth * zheight; i++) {
        zbuffer[i] = far;
    }
#else
    for (int i = 0; i < zwidth * zheight; i++) {
        zbuffer[i] = TO_FIXED(far);
    }
#endif
}
bool Renderer3D::loadModel(const char* filename) {
    FIL file;
    if (f_open(&file, filename, FA_READ) != FR_OK) return false;

    // カウント
    int vcnt = 0, fcnt = 0;
    char line[128];
    while (f_gets(line, sizeof(line), &file)) {
        if (line[0] == 'v' && line[1] == ' ') vcnt++;
        else if (line[0] == 'f') fcnt++;
    }
    f_lseek(&file, 0);  // 戻す

    // 領域確保
    vertices = static_cast<Vertex*>(region.alloc(sizeof(Vertex) * vcnt));
    faces = static_cast<Face*>(region.alloc(sizeof(Face) * fcnt));
    vcount = 0;
    fcount = 0;
    int loadcnt = 0;
    int loadcnt_max = fcnt + vcnt;

    // 読み取り
    while (f_gets(line, sizeof(line), &file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            float x, y, z;
            if (sscanf(line, "v %f %f %f", &x, &y, &z) == 3) {
                vertices[vcount++] = {x, y, z};
            }
        } else if (line[0] == 'f') {
            int v1, v2, v3;
            if (sscanf(line, "f %d %d %d", &v1, &v2, &v3) == 3) {
                faces[fcount++] = {v1, v2, v3};
            } else if (sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3) == 3) {
                faces[fcount++] = {v1, v2, v3};
            } else if (sscanf(line, "f %d//%*d %d//%*d %d//%*d", &v1, &v2, &v3) == 3) {
                faces[fcount++] = {v1, v2, v3};
            }
        }
#if 0
        if (loadcnt != fcount + vcount) {
            loadcnt = fcount + vcount;
            printf("fbufaddr 0x%08lx, vbufaddr 0x%08lx, cnt = %6d, max = %6d, Load : %3.2f\%\r", vertices, faces, loadcnt, loadcnt_max, (float)loadcnt / (float)loadcnt_max * 100.0f);
        }
#endif
    }

    f_close(&file);
    printf("Model loaded: vertices=%d, faces=%d\n", vcount, fcount);
    return true;
}

void Renderer3D::draw(float angle_x, float angle_y, float angle_z, float scale, int cx, int cy) {
    clearZBuffer(); 
    if (cx < 0) cx = display.getWidth() / 2;
    if (cy < 0) cy = display.getHeight() / 2;

    int screen_w = display.getWidth();
    int screen_h = display.getHeight();

    for (int i = 0; i < fcount; ++i) {
        Face face = faces[i];
        Vertex v0 = rotate(vertices[face.v1 - 1], angle_x, angle_y, angle_z);
        Vertex v1 = rotate(vertices[face.v2 - 1], angle_x, angle_y, angle_z);
        Vertex v2 = rotate(vertices[face.v3 - 1], angle_x, angle_y, angle_z);

        Vertex normal = normalize(cross(
            {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z},
            {v2.x - v0.x, v2.y - v0.y, v2.z - v0.z}
        ));

        float brightness = dot(normal, light_dir);
        //if (brightness < 0.0f) brightness = 0.0f;
        if (brightness <= 0.0f) continue;  // バックフェースカリング

        int x0 = (int)(v0.x * scale) + cx;
        int y0 = (int)(-v0.y * scale) + cy;
        int x1 = (int)(v1.x * scale) + cx;
        int y1 = (int)(-v1.y * scale) + cy;
        int x2 = (int)(v2.x * scale) + cx;
        int y2 = (int)(-v2.y * scale) + cy;

        // Clipping: 画面外に完全に出てる三角形はスキップ
        if ((x0 < 0 && x1 < 0 && x2 < 0) || (x0 >= screen_w && x1 >= screen_w && x2 >= screen_w) ||
            (y0 < 0 && y1 < 0 && y2 < 0) || (y0 >= screen_h && y1 >= screen_h && y2 >= screen_h)) {
            continue;
        }

#if USE_FLOAT_Z
        float z0 = -v0.z * scale;
        float z1 = -v1.z * scale;
        float z2 = -v2.z * scale;
#else
        int z0 = TO_FIXED(-v0.z * scale);
        int z1 = TO_FIXED(-v1.z * scale);
        int z2 = TO_FIXED(-v2.z * scale);
#endif

        drawTriangleZ(x0, y0, z0, x1, y1, z1, x2, y2, z2, brightness);
    }
}

Vertex Renderer3D::rotate(const Vertex& v, float ax, float ay, float az) {
    Vertex r = v;
    float sinx = sinf(ax), cosx = cosf(ax);
    float y1 = r.y * cosx - r.z * sinx;
    float z1 = r.y * sinx + r.z * cosx;
    r.y = y1; r.z = z1;
    float siny = sinf(ay), cosy = cosf(ay);
    float x2 = r.x * cosy + r.z * siny;
    float z2 = -r.x * siny + r.z * cosy;
    r.x = x2; r.z = z2;
    float sinz = sinf(az), cosz = cosf(az);
    float x3 = r.x * cosz - r.y * sinz;
    float y3 = r.x * sinz + r.y * cosz;
    r.x = x3; r.y = y3;
    return r;
}

Vertex Renderer3D::normalize(const Vertex& v) {
    float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    return len > 0.0f ? Vertex{v.x / len, v.y / len, v.z / len} : Vertex{0, 0, 0};
}

Vertex Renderer3D::cross(const Vertex& a, const Vertex& b) {
    return Vertex{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float Renderer3D::dot(const Vertex& a, const Vertex& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


#if USE_FLOAT_Z
void Renderer3D::drawTriangleZ(int x0, int y0, float z0,
                               int x1, int y1, float z1,
                               int x2, int y2, float z2,
                               float brightness) {
    // ソート（y0 <= y1 <= y2）順にする
    if (y1 < y0) { std::swap(y0, y1); std::swap(x0, x1); std::swap(z0, z1); }
    if (y2 < y0) { std::swap(y0, y2); std::swap(x0, x2); std::swap(z0, z2); }
    if (y2 < y1) { std::swap(y1, y2); std::swap(x1, x2); std::swap(z1, z2); }

    if (y0 == y2) return;  // 高さ0の三角形は無視

    auto edge_interp = [](int y0, int y1, int x0, int x1, float z0, float z1, int y) {
        float t = (float)(y - y0) / (y1 - y0);
        int x = (int)(x0 + t * (x1 - x0));
        float z = z0 + t * (z1 - z0);
        return std::make_pair(x, z);
    };

    for (int y = y0; y <= y2; y++) {
        if (y < 0 || y >= zheight) continue;

        // 左右のエッジを決める（v0-v1, v0-v2, v1-v2）
        int xa, xb;
        float za, zb;

        if (y < y1) {
            if (y1 != y0 && y2 != y0) {
                auto a = edge_interp(y0, y1, x0, x1, z0, z1, y);
                auto b = edge_interp(y0, y2, x0, x2, z0, z2, y);
                xa = a.first; za = a.second;
                xb = b.first; zb = b.second;
            } else continue;
        } else {
            if (y2 != y1 && y2 != y0) {
                auto a = edge_interp(y1, y2, x1, x2, z1, z2, y);
                auto b = edge_interp(y0, y2, x0, x2, z0, z2, y);
                xa = a.first; za = a.second;
                xb = b.first; zb = b.second;
            } else continue;
        }

        if (xa > xb) { std::swap(xa, xb); std::swap(za, zb); }
        if (xb < 0 || xa >= zwidth) continue;

        float dz = (xb != xa) ? (zb - za) / (xb - xa) : 0.0f;
        float z = za;
        int index = y * zwidth + xa;

        uint8_t level = (uint8_t)(brightness * 255.0f);
        uint32_t color = 0xFF000000 | (level << 16) | (level << 8) | level;

        for (int x = xa; x <= xb; x++) {
            if (x < 0 || x >= zwidth) { z += dz; index++; continue; }
            if (z < zbuffer[index]) {
                zbuffer[index] = z;
                display.getDrawBuffer()[index] = color;
            }
            z += dz;
            index++;
        }
    }
}
#else
void Renderer3D::drawTriangleZ(int x0, int y0, int z0,
                               int x1, int y1, int z1,
                               int x2, int y2, int z2,
                               float brightness) {
    int min_x = std::max(0, std::min(std::min(x0, x1), x2));
    int max_x = std::min(zwidth - 1, std::max(std::max(x0, x1), x2));
    int min_y = std::max(0, std::min(std::min(y0, y1), y2));
    int max_y = std::min(zheight - 1, std::max(std::max(y0, y1), y2));

    int denom = (x1 - x0)*(y2 - y0) - (y1 - y0)*(x2 - x0);
    if (denom == 0) return;

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            int w0 = (((x1 - x)*(y2 - y) - (y1 - y)*(x2 - x)) << FIXED_SHIFT) / denom;
            int w1 = (((x2 - x)*(y0 - y) - (y2 - y)*(x0 - x)) << FIXED_SHIFT) / denom;
            int w2 = FIXED_ONE - w0 - w1;
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                int z = (z0 * w0 + z1 * w1 + z2 * w2) >> FIXED_SHIFT;
                int index = y * zwidth + x;
                if (z < zbuffer[index]) {
                    zbuffer[index] = z;
                    uint8_t level = (uint8_t)(brightness * 255.0f);
                    uint32_t color = 0xFF000000 | (level << 16) | (level << 8) | level;
                    display.getDrawBuffer()[index] = color;
                }
            }
        }
    }
}
#endif