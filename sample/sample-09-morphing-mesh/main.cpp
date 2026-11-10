#include "sample_app.hpp"
#include "m3g.hpp"
#include <iostream>
#include <vector>
using namespace std;
using namespace m3g;

std::vector<Object3D*> objs;
World* wld = nullptr;

static void display() {
    Graphics3D* g3d = Graphics3D::getInstance();
    g3d->render(wld);

    sample::drawText(10, 14, "space : morph targets");
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
    switch (key) {
        case 'q': quit(); break;
        case ' ':
            cout << "main: time = " << world_time << "\n";
            wld->animate(world_time);
            world_time += 20;
            break;
        default: break;
    }
}

int main(int, char** argv) {
    sample::App sampleApp(argv[0], "sample-09-morphing-mesh", 300, 300);

    KeyframeSequence* keyframe_sequence = new KeyframeSequence(4, 2, KeyframeSequence::LINEAR);
    float keyframe_weights[4][2] = {{0, 0}, {1, 0}, {0, 1}, {0, 0}};
    keyframe_sequence->setKeyframe(0, 0, keyframe_weights[0]);
    keyframe_sequence->setKeyframe(1, 100, keyframe_weights[1]);
    keyframe_sequence->setKeyframe(2, 200, keyframe_weights[2]);
    keyframe_sequence->setKeyframe(3, 300, keyframe_weights[3]);
    keyframe_sequence->setValidRange(0, 3);
    keyframe_sequence->setDuration(400);
    keyframe_sequence->setRepeatMode(KeyframeSequence::LOOP);

    AnimationController* animation_controller = new AnimationController();
    AnimationTrack* animation_track = new AnimationTrack(keyframe_sequence, AnimationTrack::MORPH_WEIGHTS);
    animation_track->setController(animation_controller);

    float scale = 1;
    float bias[3] = {0, 0, 0};

    VertexArray* base_positions = new VertexArray(4, 3, 4);
    float base_positions_values[12] = {1, -1, 0, 1, 1, 0, -1, -1, 0, -1, 1, 0};
    base_positions->set(0, 4, base_positions_values);

    VertexArray* target1_positions = new VertexArray(4, 3, 4);
    float target1_positions_values[12] = {1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0};
    target1_positions->set(0, 4, target1_positions_values);

    VertexArray* target2_positions = new VertexArray(4, 3, 4);
    float target2_positions_values[12] = {2, -1, 0, 2, -0.5, 0, -2, -1, 0, -2, -0.5, 0};
    target2_positions->set(0, 4, target2_positions_values);

    VertexBuffer* base_vertices = new VertexBuffer();
    base_vertices->setPositions(base_positions, scale, bias);

    VertexBuffer* target_vertices[] = {0, 0};

    target_vertices[0] = new VertexBuffer();
    target_vertices[0]->setPositions(target1_positions, scale, bias);

    target_vertices[1] = new VertexBuffer();
    target_vertices[1]->setPositions(target2_positions, scale, bias);

    int strips[1] = {4};
    TriangleStripArray* tris = new TriangleStripArray(0, 1, strips);
    Appearance* app = new Appearance();

    MorphingMesh* mesh = new MorphingMesh(base_vertices, 2, target_vertices, tris, app);

    float weights[2] = {0, 0};
    mesh->setWeights(2, weights);
    mesh->addAnimationTrack(animation_track);

    Camera* cam = new Camera();
    cam->translate(0, 0, 10);

    wld = new World();
    wld->addChild(cam);
    wld->setActiveCamera(cam);
    wld->addChild(mesh);

    cout << *wld << "\n";

    objs.push_back(keyframe_sequence);
    objs.push_back(animation_controller);
    objs.push_back(animation_track);
    objs.push_back(base_positions);
    objs.push_back(target1_positions);
    objs.push_back(target2_positions);
    objs.push_back(base_vertices);
    objs.push_back(target_vertices[0]);
    objs.push_back(target_vertices[1]);
    objs.push_back(tris);
    objs.push_back(app);
    objs.push_back(mesh);
    objs.push_back(cam);
    objs.push_back(wld);
    sampleApp.run(display, reshape, keyboard);

    for (auto* obj : objs) {
        delete obj;
    }
    delete Graphics3D::getInstance();

    return 0;
}
