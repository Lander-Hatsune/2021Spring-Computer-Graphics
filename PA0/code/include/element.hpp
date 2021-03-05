
#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>

using std::swap;

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
        // TODO: Implement Bresenham Algorithm
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

        // printf("neg k: %d, big k: %d\n", neg_k, big_k);

        for (int i = 0; i < loop; i++) {
            // printf("(%d %d) ", x, y);
            img.SetPixel(x, y, color);
            (big_k ? y : x) += 1;
            e += dw << 1;
            if (e >= 0) {
                (big_k ? x : y) += (neg_k ? -1 : 1);
                e -= dz << 1;
            }
        }
        // putchar('\n');
    }
};

class Circle : public Element {

public:
    int cx, cy;
    int radius;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Implement Algorithm to draw a Circle
        printf("Draw a circle with center (%d, %d) \
and radius %d using color (%f, %f, %f)\n",
               cx, cy, radius, color.x(), color.y(), color.z());
    }
};

class Fill : public Element {

public:
    int cx, cy;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Flood fill
        printf("Flood fill source point = (%d, %d) \
using color (%f, %f, %f)\n",
               cx, cy, color.x(), color.y(), color.z());
    }
};
