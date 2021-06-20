#include <vecmath.h>
#include <cstdio>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

using namespace std;

int SAMPLE = 4;

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];

    SceneParser sceneParser(inputFile.c_str());
    Camera* camera = sceneParser.getCamera();
    Group* group = sceneParser.getGroup();

    Image img(camera->getWidth(), camera->getHeight());
    Image img_atals(camera->getWidth(), camera->getHeight());
    

    #pragma omp parallel for schedule(dynamic, 1)
    for (int x = 0; x < camera->getWidth(); x++) {
        for (int y = 0; y < camera->getHeight(); y++) {
            Vector3f finColor = Vector3f::ZERO;
            for (int sample = 0; sample < SAMPLE; sample++) {
                Ray ray = camera->generateRay(Vector2f(x, y));
                Hit hit;
                if (group->intersect(ray, hit, 0)) {
                    for (int i = 0; i < sceneParser.getNumLights(); i++) {
                        Light* light = sceneParser.getLight(i);
                        Vector3f direc, lightColor;
                        light->getIllumination(ray.pointAtParameter(hit.getT()),
                                               direc, lightColor);
                        finColor += hit.getMaterial()->Shade(ray, hit, direc,
                                                             lightColor);
                    }
                } else {
                    finColor += sceneParser.getBackgroundColor();
                }
            }
            finColor = finColor / SAMPLE;
            img.SetPixel(x, y, finColor);
        }
    }

    img.SaveBMP(outputFile.c_str());

    printf("done\n");
    return 0;
}
