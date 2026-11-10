#include "sample_app.hpp"
#include <m3g/m3g.hpp>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <cstdlib>
using namespace std;
using namespace m3g;

std::vector<Object3D*> objs;
World* wld = nullptr;

static void display(void) {
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->render(wld);
}

static void resize(int width, int height) {
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->setViewport(0, 0, width, height);
    Camera* cam = wld->getActiveCamera();
    cam->setPerspective(45, width / (float)height, 0.1, 100);
}

static void quit() { sample::requestClose(); }

static void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'q': quit(); break;
        default: break;
    }
}

int main(int, char** argv) {
    sample::App sampleApp(argv[0], "sample-02-texture", 300, 300);

    VertexArray* positions = new VertexArray(4, 3, 2);
    short position_values[] = {1, -1, 0, 1, 1, 0, -1, -1, 0, -1, 1, 0};
    positions->set(0, 4, position_values);

    VertexArray* tex_coords = new VertexArray(4, 2, 2);
    short tex_coord_values[] = {1, 0, 1, 1, 0, 0, 0, 1};
    tex_coords->set(0, 4, tex_coord_values);

    float scale = 1;
    float bias[3] = {0, 0, 0};
    VertexBuffer* vertices = new VertexBuffer();
    vertices->setPositions(positions, scale, bias);
    vertices->setTexCoords(0, tex_coords, scale, bias);
    vertices->setTexCoords(1, tex_coords, scale, bias);

    int indices[] = {0, 1, 2, 2, 1, 3};
    int strips[] = {3, 3};
    TriangleStripArray* tris = new TriangleStripArray(6, indices, 2, strips);

    auto document = Loader::loadDocument(sampleApp.asset("yumemi.png").string().c_str());

    Image2D* img0 = document.findFirst<Image2D>();
    Texture2D* tex0 = new Texture2D(img0);

    document = Loader::loadDocument(sampleApp.asset("onnanoko.png").string().c_str());

    Image2D* img1 = document.findFirst<Image2D>();
    Texture2D* tex1 = new Texture2D(img1);
    tex1->setBlending(Texture2D::FUNC_DECAL);

    Appearance* app = new Appearance();
    app->setTexture(0, tex0);
    app->setTexture(1, tex1);

    Mesh* mesh = new Mesh(vertices, tris, app);
    mesh->translate(0, 0, 2);

    Camera* cam = new Camera();
    cam->translate(0, 0, 5);

    wld = new World();
    wld->addChild(cam);
    wld->setActiveCamera(cam);
    wld->addChild(mesh);

    cout << *wld << "\n";

    objs.push_back(positions);
    objs.push_back(tex_coords);
    objs.push_back(vertices);
    objs.push_back(tris);
    objs.push_back(img0);
    objs.push_back(img1);
    objs.push_back(tex0);
    objs.push_back(tex1);
    objs.push_back(app);
    objs.push_back(mesh);
    objs.push_back(cam);
    objs.push_back(wld);
    sampleApp.run(display, resize, keyboard);

    for (auto* obj : objs) {
        delete obj;
    }
    delete Graphics3D::getInstance();

    return 0;
}
