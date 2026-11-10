#include "sample_app.hpp"
#include <m3g/m3g.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace m3g;

std::vector<Object3D*> objs;
World* wld = nullptr;

static void display(void) {
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->render(wld);

    sample::drawText(10, 14, "2,4,6,8 : move camera");
}

static void reshape(int width, int height) {
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->setViewport(0, 0, width, height);
    Camera* cam = wld->getActiveCamera();
    cam->setPerspective(45, width / (float)height, 0.1, 100);
}

static void quit() { sample::requestClose(); }

int world_time = 0;

static void keyboard(unsigned char key, int x, int y) {
    Camera* cam = wld->getActiveCamera();

    switch (key) {
        case 'q': quit(); break;
        case ' ':
            cout << "main: time = " << world_time << "\n";
            wld->animate(world_time);
            world_time += 2;
            break;
        case '4':
            cam->postRotate(5, 0, 1, 0);
            wld->align(nullptr);
            break;
        case '6':
            cam->postRotate(-5, 0, 1, 0);
            wld->align(nullptr);
            break;
        case '8':
            cam->postRotate(5, 1, 0, 0);
            wld->align(nullptr);
            break;
        case '2':
            cam->postRotate(-5, 1, 0, 0);
            wld->align(nullptr);
            break;
        default: break;
    }
}

int main(int, char** argv) {
    sample::App sampleApp(argv[0], "sample-11-align", 300, 300);

    Background* bg = new Background();
    bg->setColor(0xff3f3fff);

    VertexArray* positions = new VertexArray(4, 3, 4);
    float position_values[] = {1, -1, 0, 1, 1, 0, -1, -1, 0, -1, 1, 0};
    positions->set(0, 4, position_values);

    VertexArray* colors = new VertexArray(4, 4, 1);
    unsigned char colors_values[] = {255, 255, 255, 255, 255, 0, 0, 255, 255, 255, 255, 255, 0, 0, 255, 255, 255, 255, 255, 255};
    colors->set(0, 4, (char*)colors_values);

    float scale = 1;
    float bias[3] = {0, 0, 0};
    VertexBuffer* vertices = new VertexBuffer();
    vertices->setPositions(positions, scale, bias);
    vertices->setColors(colors);

    int indices[] = {0, 1, 2, 3};
    int strips[] = {4};
    TriangleStripArray* tris = new TriangleStripArray(4, indices, 1, strips);

    PolygonMode* poly = new PolygonMode();
    poly->setCulling(PolygonMode::CULL_NONE);

    Material* mat = new Material();
    mat->setVertexColorTrackingEnable(true);

    Appearance* app = new Appearance();
    app->setPolygonMode(poly);
    app->setMaterial(mat);

    Camera* cam = new Camera();
    cam->lookAt(0, 0, 15, 0, 0, 0, 0, 1, 0);

    Mesh* mesh1 = new Mesh(vertices, tris, app);

    Mesh* mesh2 = new Mesh(vertices, tris, app);
    mesh2->translate(3, 0, 0);

    Mesh* mesh3 = new Mesh(vertices, tris, app);
    mesh3->translate(-3, 0, 0);

    wld = new World();
    wld->addChild(cam);
    wld->setActiveCamera(cam);
    wld->setBackground(bg);
    wld->addChild(mesh1);
    wld->addChild(mesh2);
    wld->addChild(mesh3);

    cout << *wld << "\n";

    mesh1->setAlignment(cam, Node::NONE, wld, Node::NONE);
    mesh2->setAlignment(cam, Node::ORIGIN, wld, Node::ORIGIN);
    mesh3->setAlignment(cam, Node::ORIGIN, cam, Node::ORIGIN);

    // wld->align (0);

    objs.push_back(cam);
    objs.push_back(bg);
    objs.push_back(positions);
    objs.push_back(tris);
    objs.push_back(poly);
    objs.push_back(app);
    objs.push_back(mesh1);
    objs.push_back(mesh2);
    objs.push_back(mesh3);
    objs.push_back(wld);
    sampleApp.run(display, reshape, keyboard);

    for (auto* obj : objs) {
        delete obj;
    }
    delete Graphics3D::getInstance();

    return 0;
}
