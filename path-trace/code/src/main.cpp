#include <vecmath.h>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

using namespace std;

int SAMPLE = 4;
int RECURSIVE_MAX = 10;
Camera *camera;
Group *group;
Vector3f bgcolor;
const double PI = acos(-1);

Ray diffuseRay(const Vector3f& p, const Vector3f& n) {
    Vector3f ori = p + n;
    double r = 1.;
    double theta = 2 * PI * rand() / RAND_MAX,
        rho = PI * rand() / RAND_MAX;
    return Ray(p, (ori + Vector3f(r * cos(rho) * cos(theta),
                                  r * cos(rho) * sin(theta),
                                  r * sin(rho)) - p).normalized());
}

Vector3f getColor(const Ray r, int rec) {
    if (rec >= RECURSIVE_MAX) return Vector3f::ZERO;
    Hit h;
    if (group->intersect(r, h, 0.001)) {
        Vector3f p = r.pointAtParameter(h.getT());
        Vector3f n = h.getNormal();
        return 0.5 * getColor(diffuseRay(p, n), rec + 1);
    } else {
        return bgcolor;
    }
}

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
    

    #pragma omp parallel for schedule(dynamic, 1)
    for (int x = 0; x < camera->getWidth(); x++) {
        for (int y = 0; y < camera->getHeight(); y++) {
            Vector3f finColor = Vector3f::ZERO;
            for (int sample = 0; sample < SAMPLE; sample++) {
                Ray ray = camera->generateRay(Vector2f(x, y));
                finColor += getColor(ray, 0);
            }
            finColor = finColor / SAMPLE;
            img.SetPixel(x, y, finColor);
        }
    }

    img.SaveBMP(outputFile.c_str());

    printf("done\n");
    return 0;
}
