#include "sample_app.hpp"
#include "m3g.hpp"
#include <iostream>
#include <vector>
using namespace std;
using namespace m3g;

std::vector<Object3D*> objs;
World* wld = nullptr;
float width;
float height;

static void display(void) {
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->render(wld);

    sample::drawText(10, 14, "8 : move forward");
    sample::drawText(10, 27, "2 : move back");
    sample::drawText(10, 40, "mouse : pick up");
}

static void reshape(int w, int h) {
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->setViewport(0, 0, w, h);
    Camera* cam = wld->getActiveCamera();
    width = w;
    height = h;
    cam->setPerspective(45, width / height, 1, 100);
}

static void quit() { sample::requestClose(); }

static void keyboard(unsigned char key, int x, int y) {
    Camera* cam = wld->getActiveCamera();
    switch (key) {
        case '2': cam->translate(0, 0, 1); break;
        case '8': cam->translate(0, 0, -1); break;
        case 'q': quit(); break;
        default: break;
    }
}

static void mouse(int button, int state, int x, int y) {
    if (button != 0 || state != 0) return;
    Camera* cam = wld->getActiveCamera();
    RayIntersection ri;
    bool hit = wld->pick(-1, x / width, y / height, cam, &ri);
    cout << ((hit) ? "HIT!" : "miss") << "\n";
    // if (hit) {
    //    cout << ri << "\n";
    //}
}

int main(int, char** argv) {
    sample::App sampleApp(argv[0], "sample-07-sprite", 300, 300);

    Camera* cam = new Camera();
    cam->lookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    Background* bg = new Background();
    bg->setColor(0xff1f1f7f);

    Image2D* img = dynamic_cast<Image2D*>(Loader::load(sampleApp.asset("mizugi-small.png").string().c_str())[0]);
    int width = img->getWidth();
    int height = img->getHeight();

    CompositingMode* cmp = new CompositingMode();
    cmp->setDepthTestEnable(true);

    Appearance* app1 = new Appearance();
    app1->setCompositingMode(cmp);

    Sprite3D* spr1 = new Sprite3D(true, img, app1);
    spr1->translate(1, 0, 0);
    spr1->setCrop(0, 0, -width, height);

    Appearance* app2 = new Appearance();
    app2->setCompositingMode(cmp);

    Sprite3D* spr2 = new Sprite3D(false, img, app2);
    spr2->translate(-1, 0, 0);
    spr2->setCrop(0, 0, width, height);

    wld = new World();
    wld->addChild(cam);
    wld->setActiveCamera(cam);
    wld->setBackground(bg);
    wld->addChild(spr1);
    wld->addChild(spr2);

    cout << *wld << "\n";

    objs.push_back(cam);
    objs.push_back(bg);
    objs.push_back(img);
    objs.push_back(cmp);
    objs.push_back(app1);
    objs.push_back(spr1);
    objs.push_back(app2);
    objs.push_back(spr2);
    objs.push_back(wld);
    sampleApp.run(display, reshape, keyboard, nullptr, mouse);

    for (auto* obj : objs) {
        delete obj;
    }
    delete Graphics3D::getInstance();

    return 0;
}
