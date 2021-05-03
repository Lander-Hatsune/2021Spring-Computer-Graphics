#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


// DONE (PA2): Implement Group - copy from PA1
class Group : public Object3D {

public:

    Group() {

    }

    explicit Group (int num_objects) {

    }

    ~Group() override {

    }

    // return in hit
    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool ret = false;
        for (auto object: list) {
            if (object->intersect(r, h, tmin)) ret = true;
        }
        return ret;
    }

    void addObject(int index, Object3D *obj) {
        list.push_back(obj);
    }

    int getGroupSize() {
        return list.size();
    }

    void drawGL() override {
        Object3D::drawGL();
        for (auto obj: list) {
            obj->drawGL();
        }
    }

    std::vector<Object3D*> list;

};

#endif
	
