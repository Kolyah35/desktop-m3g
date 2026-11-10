#include "sample_app.hpp"
#include "m3g.hpp"
#include <iostream>
#include <vector>
using namespace std;
using namespace m3g;

std::vector<Object3D*> objs;
World* wld = nullptr;

static void display(void) {
    cout << "display\n";
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->render(wld);

    sample::drawText(10, 14, "4 : scroll left");
    sample::drawText(10, 27, "6 : scroll right");
}

static void reshape(int width, int height) {
    cout << "reshape\n";
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->setViewport(0, 0, width, height);
    Camera* cam = wld->getActiveCamera();
    cam->setPerspective(45, width / (float)height, 0.1, 100);
}

int crop_x = 0;

static void quit() { sample::requestClose(); }

static void keyboard(unsigned char key, int x, int y) {
    Background* bg = wld->getBackground();
    int img_width = bg->getImage()->getWidth();
    int img_height = bg->getImage()->getHeight();

    switch (key) {
        case 'q': quit(); break;
        case '4':
            crop_x = max(crop_x - 50, 0);
            bg->setCrop(crop_x, 0, 512, 512);
            break;
        case '6':
            crop_x = min(crop_x + 50, img_width - 512);
            bg->setCrop(crop_x, 0, 512, 512);
            break;
        default: break;
    }
}

int main(int, char** argv) {
    sample::App sampleApp(argv[0], "sample-13-background", 512, 512);

    VertexArray* positions = new VertexArray(4, 3, 2);
    short positions_value[] = {1, -1, 0, 1, 1, 0, -1, -1, 0, -1, 1, 0};
    positions->set(0, 4, positions_value);

    float scale = 1;
    float bias[3] = {0, 0, 0};
    VertexBuffer* vertices = new VertexBuffer();
    vertices->setPositions(positions, scale, bias);

    int indices[] = {0, 1, 2, 3};
    int strips[] = {4};
    TriangleStripArray* tris = new TriangleStripArray(4, indices, 1, strips);

    Material* mat = new Material();
    mat->setColor(Material::DIFFUSE, 0xff7f7fff);

    Appearance* app = new Appearance();
    app->setMaterial(mat);

    Mesh* mesh = new Mesh(vertices, tris, app);

    Camera* cam = new Camera();
    cam->translate(0, 0, 20);

    wld = new World();
    wld->addChild(cam);
    wld->setActiveCamera(cam);
    wld->addChild(mesh);

    Image2D* img = dynamic_cast<Image2D*>(Loader::load(sampleApp.asset("music-is.png").string().c_str())[0]);
    Background* bg = new Background();
    bg->setColor(0xff3f8f1f);
    bg->setImage(img);
    bg->setCrop(0, 0, 512, 512);
    bg->setImageMode(Background::REPEAT, Background::BORDER);
    wld->setBackground(bg);

    cout << *wld << "\n";

    objs.push_back(positions);
    objs.push_back(vertices);
    objs.push_back(tris);
    objs.push_back(mat);
    objs.push_back(app);
    objs.push_back(cam);
    objs.push_back(mesh);
    objs.push_back(wld);
    objs.push_back(img);
    objs.push_back(bg);
    sampleApp.run(display, reshape, keyboard);

    for (auto* obj : objs) {
        delete obj;
    }
    delete Graphics3D::getInstance();

    return 0;
}
