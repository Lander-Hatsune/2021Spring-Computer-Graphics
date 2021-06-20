#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


// DONE: Implement Group - add data structure to store a list of Object*
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

private:
    std::vector<Object3D*> list;

};

#endif
	
