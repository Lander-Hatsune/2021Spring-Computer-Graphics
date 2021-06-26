#include <vecmath.h>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "Vector3f.h"
#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"
#include "trace.hpp"

using std::cout;
using std::endl;
using std::string;

Camera *camera;
Group *group;
Vector3f bgcolor;

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/path-trace <input scene file> <output bmp file>"
             << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];

    SceneParser sceneParser(inputFile.c_str());
    camera = sceneParser.getCamera();
    group = sceneParser.getGroup();
    bgcolor = sceneParser.getBackgroundColor();

    Image img(camera->getWidth(), camera->getHeight());
    Image img_atals(camera->getWidth(), camera->getHeight());
    
    #pragma omp parallel for
    for (int x = 0; x < camera->getWidth(); x++) {
        for (int y = 0; y < camera->getHeight(); y++) {
            Vector3f finColor = Vector3f::ZERO;
            for (int sample = 0; sample < ATALS_SAMPLE; sample++) {
                Ray ray = camera->randGenRay(Vector2f(x, y));
                finColor += trace::getColor(ray, group, bgcolor,
                                            Vector3f(1, 1, 1), 1);
            }
            finColor = finColor / ATALS_SAMPLE;
            finColor = Vector3f(sqrt(finColor.x()),
                                sqrt(finColor.y()),
                                sqrt(finColor.z()));
            img.SetPixel(x, y, finColor);
            if (x % 20 == 0 && y == camera->getHeight() - 1) {
                printf("temp %d\n", x);
                img.SaveBMP(outputFile.c_str());
            }
        }
    }

    img.SaveBMP(outputFile.c_str());

    printf("done\n");
    return 0;
}
