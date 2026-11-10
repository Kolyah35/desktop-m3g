#include "sample_app.hpp"
#include "m3g.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <numbers>
using namespace std;
using namespace m3g;

World* wld = nullptr;
int fovy;
float near;
float far;

static void display(void) {
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->render(wld);

    sample::drawText(10, 14, "space : pause/resume");
}

static void reshape(int width, int height) {
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->setViewport(0, 0, width, height);
    Camera* cam = wld->getActiveCamera();
    cam->setPerspective(45, width / (float)height, 0.1, 1000);
}

static void quit() { sample::requestClose(); }

int world_time = 0;
bool stopped = true;
float angle = 0;

float rad(float degree) {
    return degree / (180.0 / std::numbers::pi_v<double>);
}

static int frame = 0;

void idle() {
    if (stopped) {
        return;
    }

    cout << "frame = " << frame++ << "\n";

    world_time = world_time + 20;
    wld->animate(world_time);

    // float from_x = 0;
    // float from_y = 10*sin(rad(angle));
    // float from_z = 10*cos(rad(angle));
    // cam->lookAt (from_x, from_y, from_z,
    //             0, 0, 0,
    //             0, 1, 0);
}

static void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'q': quit(); break;
        case ' ':
            stopped = !stopped;
            cout << (stopped ? "stopped" : "running") << "\n";
            ;
            break;
        default: break;
    }
}

static void mouse(int button, int state, int x, int y) {
    Graphics3D* g3d = Graphics3D::getInstance();
    float width = g3d->getViewportWidth();
    float height = g3d->getViewportHeight();
    Camera* cam = wld->getActiveCamera();
    if (button == 0 && state == 0) {
        RayIntersection ri;
        bool hit = wld->pick(-1, x / width, y / height, cam, &ri);
        cout << ((hit) ? "HIT!" : "miss") << "\n";
    }
}

int main(int, char** argv) {
    sample::App sampleApp(argv[0], "sample-12-loader", 300, 300);

    // auto document = Loader::loadDocument(sampleApp.asset("simple.m3g").string().c_str());
    auto document = Loader::loadDocument(sampleApp.asset("test.m3g").string().c_str());
    // auto document = Loader::loadDocument(sampleApp.asset("kitune-miko.m3g").string().c_str());

    wld = document.findFirst<m3g::World>();
    auto varry = document.findFirst<m3g::VertexArray>();

    Camera* cam = wld->getActiveCamera();
    float params[4];
    cam->getProjection(params);
    fovy = params[0];
    near = params[2];
    far = params[3];
    cout << "fovy = " << fovy << "\n";
    cout << "near = " << near << "\n";

    Background* bg = wld->getBackground();
    if (bg == nullptr) {
        bg = new Background();
        wld->setBackground(bg);
    }
    bg->setColor(0xff3f7fff);
    sampleApp.run(display, reshape, keyboard, idle, mouse);

    delete Graphics3D::getInstance();

    return 0;
}
