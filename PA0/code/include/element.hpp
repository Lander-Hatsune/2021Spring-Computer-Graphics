
#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <utility>
#include <cstdio>

using std::swap;
using std::queue;
using std::pair;

class Element {
public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element {

public:
    int xA, yA;
    int xB, yB;
    Vector3f color;
    void draw(Image &img) override {
        // DONE: Implement Bresenham Algorithm
        printf("Draw a line from (%d, %d) to (%d, %d) \
using color (%f, %f, %f)\n",
               xA, yA, xB, yB, color.x(), color.y(), color.z());

        if (xA == xB) {
            if (yA > yB) swap(yA, yB);
            for (int y = yA; y <= yB; y++)
                img.SetPixel(xA, y, color);
            return;
        } else if (yA == yB) {
            if (xA > xB) swap(xA, xB);
            for (int x = xA; x <= xB; x++)
                img.SetPixel(x, yA, color);
            return;
        }

        bool neg_k = false;
        bool big_k = false;
        if ((xB - xA) * (yB - yA) < 0) neg_k = true;
        if (abs(yB - yA) > abs(xB - xA)) big_k = true;

        if ((big_k && yA > yB) || !big_k && xA > xB) {
            swap(xA, xB); swap(yA, yB);
        }

        int loop = big_k ? yB - yA + 1 : xB - xA + 1;
        
        int dz = (big_k ? yB - yA : xB - xA),// x
            dw = (neg_k ? -1 : 1) * (!big_k ? yB - yA : xB - xA);// f(x)
        
        int x = xA, y = yA, e = -dz;

        for (int i = 0; i < loop; i++) {
            img.SetPixel(x, y, color);
            (big_k ? y : x) += 1;
            e += dw << 1;
            if (e >= 0) {
                (big_k ? x : y) += (neg_k ? -1 : 1);
                e -= dz << 1;
            }
        }
    }
};

class Circle : public Element {


    
public:
    int cx, cy;
    int radius;
    Vector3f color;

    void symdraw(Image &img, int x, int y) {
        img.SetPixel(cx + x, cy + y, color);
        img.SetPixel(cx + y, cy + x, color);
        img.SetPixel(cx - x, cy + y, color);
        img.SetPixel(cx + y, cy - x, color);
        img.SetPixel(cx + x, cy - y, color);
        img.SetPixel(cx - y, cy + x, color);
        img.SetPixel(cx - x, cy - y, color);
        img.SetPixel(cx - y, cy - x, color);
    }

    void draw(Image &img) override {
        // DONE: Implement Algorithm to draw a Circle
        printf("Draw a circle with center (%d, %d) \
and radius %d using color (%f, %f, %f)\n",
               cx, cy, radius, color.x(), color.y(), color.z());
        
        int x = 0, y = radius;
        double d = 1.25 - radius;

        symdraw(img, x, y);

        while (x <= y) {
            if (d < 0) d += (2 * x + 3);
            else {d += (2 * (x - y) + 5); y -= 1;}
            x += 1;
            symdraw(img, x, y);
        }
        putchar('\n');
    }
};

class Fill : public Element {

public:
    int cx, cy;
    Vector3f color;
    void draw(Image &img) override {
        // DONE: Flood fill
        printf("Flood fill source point = (%d, %d) \
using color (%f, %f, %f)\n",
               cx, cy, color.x(), color.y(), color.z());

        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        queue<pair<int, int>> q;
        int w = img.Width(), h = img.Height();
        
        Vector3f oric = img.GetPixel(cx, cy);
        q.push({cx, cy});
        img.SetPixel(cx, cy, color);

        while (!q.empty()) {
            int x = q.front().first,
                y = q.front().second;
            q.pop();
            
            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx >= w || nx < 0 || ny >= h || ny < 0)
                    continue;
                if (img.GetPixel(nx, ny) == oric) {
                    q.push({nx, ny});
                    img.SetPixel(nx, ny, color);
                }
            }
        }
    }
};
