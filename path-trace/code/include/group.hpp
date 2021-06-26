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
    bool intersect(const Ray &r, Hit &H, float tmin) override {
        bool ret = false;
        for (auto object: list) {
            bool cur_ret = false;
            Hit h = H;
            if (object->intersect(r, h, tmin)) {
                float dens = h.getMaterial()->getDensity();
                if (1 - dens > 1e-3) {
                    Hit h2;
                    Ray r2(r.pointAtParameter(h.getT() + 0.001),
                           r.getDirection());
                    float len;
                    if (object->intersect(r2, h2, 0.001)) {
                        len = h2.getT();
                    } else {
                        len = h.getT();
                    }
                    float neg_inv_dens = -1 / dens;
                    float hit_len = neg_inv_dens *
                        log((double) rand() / RAND_MAX);

                    //printf("len: %f hit_len: %f\n", len, hit_len);
                    if (len > hit_len) {
                        h.set(hit_len, h.getMaterial(),
                              randUnitSphere(), 0, 0, true);
                        cur_ret = ret = true;
                    } else {
                        cur_ret = false;
                    }
                } else cur_ret = ret = true;
                
                if (cur_ret == true) {
                    H = h;
                }
            }
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
	
