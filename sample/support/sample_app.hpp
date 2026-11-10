#pragma once

#ifndef GLFW_INCLUDE_NONE
    #define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <cctype>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#if defined(_WIN32)
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>

    #undef near
    #undef far
#endif

namespace sample {
    namespace detail {

        inline std::filesystem::path executableDir(const char* argv0) {
            // https://gist.github.com/Jacob-Tate/7b326a086cf3f9d46e32315841101109
#if defined(_WIN32)
            wchar_t path[FILENAME_MAX] = {0};
            GetModuleFileNameW(nullptr, path, FILENAME_MAX);
            return std::filesystem::path(path).parent_path();
#else
            char path[FILENAME_MAX];
            ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
            return std::filesystem::path(std::string(path, (count > 0) ? count : 0)).parent_path();
#endif

            return argv0 && *argv0 ? std::filesystem::absolute(argv0).parent_path() : std::filesystem::current_path();
        }

        inline void errorCallback(int code, const char* description) {
            std::cerr << "GLFW error " << code << ": " << (description ? description : "unknown") << '\n';
        }

        inline int asciiKey(int key) {
            if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) return 'a' + key - GLFW_KEY_A;
            if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) return '0' + key - GLFW_KEY_0;
            if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_9) return '0' + key - GLFW_KEY_KP_0;

            switch (key) {
                case GLFW_KEY_SPACE: return ' ';
                case GLFW_KEY_LEFT: return '4';
                case GLFW_KEY_RIGHT: return '6';
                case GLFW_KEY_UP: return '8';
                case GLFW_KEY_DOWN: return '2';
                default: return 0;
            }
        }

        struct Glyph {
            char c;
            std::uint8_t rows[7];
        };

       inline constexpr Glyph font[] = {
            {'A', {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}},
            {'B', {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E}},
            {'C', {0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E}},
            {'D', {0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E}},
            {'E', {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F}},
            {'F', {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10}},
            {'G', {0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0E}},
            {'H', {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}},
            {'I', {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1F}},
            {'J', {0x07, 0x02, 0x02, 0x02, 0x12, 0x12, 0x0C}},
            {'K', {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11}},
            {'L', {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F}},
            {'M', {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11}},
            {'N', {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11}},
            {'O', {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}},
            {'P', {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10}},
            {'Q', {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D}},
            {'R', {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11}},
            {'S', {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E}},
            {'T', {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
            {'U', {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}},
            {'V', {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04}},
            {'W', {0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A}},
            {'X', {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11}},
            {'Y', {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04}},
            {'Z', {0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F}},

            {'0', {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E}},
            {'1', {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E}},
            {'2', {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F}},
            {'3', {0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E}},
            {'4', {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02}},
            {'5', {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E}},
            {'6', {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E}},
            {'7', {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08}},
            {'8', {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E}},
            {'9', {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C}},

            {':', {0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x00}},
            {'.', {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06}},
            {',', {0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x04}},
            {'/', {0x01, 0x01, 0x02, 0x04, 0x08, 0x10, 0x10}},
            {'-', {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00}},
            {'+', {0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00}},
            {'(', {0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02}},
            {')', {0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08}},
            {'?', {0x0E, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04}},
            {'=', {0x00, 0x1F, 0x00, 0x00, 0x1F, 0x00, 0x00}},
        };

        inline const Glyph* getGlyph(unsigned char c) {
            const char uppercase = static_cast<char>(std::toupper(c));
            for (const auto& glyph : font) {
                if (glyph.c == uppercase) return &glyph;
            }

            return nullptr;
        }

        inline void renderTextPixels(float x, float y, const char* str) {
            glBegin(GL_QUADS);

            for (const auto* p = reinterpret_cast<const unsigned char*>(str); *p; ++p, x += 8.f) {
                const Glyph* g = getGlyph(*p);
                if (!g) {
                    continue;
                }

                for (int row = 0; row < 7; ++row) {
                    for (int col = 0; col < 5; ++col) {
                        if ((g->rows[row] & (1u << (4 - col))) == 0) {
                            continue;
                        }
                        
                        const float px = x + col * 1.2f;
                        const float py = y + row * 1.55f;
                        glVertex2f(px, py);
                        glVertex2f(px + 1.2f, py);
                        glVertex2f(px + 1.2f, py + 1.55f);
                        glVertex2f(px, py + 1.55f);
                    }
                }
            }
            glEnd();
        }

    } // namespace detail

    inline void drawText(int x, int y, const char* str) {
        auto* window = glfwGetCurrentContext();

        if (!window || !str || !*str) {
            return;
        }

        int width = 0;
        int height = 0;

        glfwGetFramebufferSize(window, &width, &height);

        if (width == 0 || height == 0) {
            return;
        }

        GLint previousMatrixMode = GL_MODELVIEW;
        glGetIntegerv(GL_MATRIX_MODE, &previousMatrixMode);
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_CULL_FACE);
        glDisable(GL_FOG);
        glDisable(GL_BLEND);
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_STENCIL_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(0.f, 0.f, 0.f);
        detail::renderTextPixels(x + 1.f, y + 1.f, str);

        glColor3f(1.f, 1.f, 1.f);
        detail::renderTextPixels(static_cast<float>(x), static_cast<float>(y), str);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(previousMatrixMode);
        glPopAttrib();
    }

    inline void requestClose() {
        if (auto* window = glfwGetCurrentContext()) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    class App {
    public:
        using DisplayCallback = void (*)();
        using ResizeCallback = void (*)(int, int);
        using KeyboardCallback = void (*)(unsigned char, int, int);
        using IdleCallback = void (*)();
        using MouseCallback = void (*)(int, int, int, int);

        App(const char* argv0, const char* title, int width, int height) : m_assetDir(detail::executableDir(argv0) / "resources") {
            glfwSetErrorCallback(detail::errorCallback);

            if (glfwInit() != GLFW_TRUE) {
                throw std::runtime_error("Could not initialize GLFW");
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_DEPTH_BITS, 24);

            m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
            
            if (!m_window) {
                glfwTerminate();
                throw std::runtime_error("Could not create an OpenGL compatibility window");
            }
            
            glfwMakeContextCurrent(m_window);
            gladLoadGL();
            glfwSwapInterval(1);
        }

        App(const App&) = delete;
        App& operator=(const App&) = delete;

        ~App() {
            if (m_window) {
                glfwDestroyWindow(m_window);
            }

            glfwTerminate();
        }

        [[nodiscard]] std::filesystem::path asset(std::string_view file) const {
            const auto result = m_assetDir / std::filesystem::path(file);

            if (!std::filesystem::exists(result)) {
                throw std::runtime_error("Missing sample resource: " + result.string());
            }
            
            return result;
        }

        void run(DisplayCallback display, ResizeCallback resize, KeyboardCallback keyboard, IdleCallback idle = nullptr, MouseCallback mouse = nullptr) {
            m_keyboard = keyboard;
            m_idle = idle;
            m_mouse = mouse;
            m_resize = resize;

            glfwSetWindowUserPointer(m_window, this);
            glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int, int action, int) {
                if (action != GLFW_PRESS && action != GLFW_REPEAT) {
                    return;
                }

                if (key == GLFW_KEY_ESCAPE) {
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }

                auto* self = static_cast<App*>(glfwGetWindowUserPointer(window));
                const auto c = detail::asciiKey(key);

                if (c && self->m_keyboard) {
                    self->m_keyboard(static_cast<unsigned char>(c), 0, 0);
                }
            });

            glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
                auto* self = static_cast<App*>(glfwGetWindowUserPointer(window));
                
                if (width > 0 && height > 0 && self->m_resize) {
                    self->m_resize(width, height);
                }
            });

            if (mouse) {
                glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int) {
                    auto* self = static_cast<App*>(glfwGetWindowUserPointer(window));
                    double cursorX = 0, cursorY = 0;
                    glfwGetCursorPos(window, &cursorX, &cursorY);

                    int windowW = 0, windowH = 0, frameW = 0, frameH = 0;
                    glfwGetWindowSize(window, &windowW, &windowH);
                    glfwGetFramebufferSize(window, &frameW, &frameH);

                    if (windowW < 1 || windowH < 1 || frameW < 1 || frameH < 1) {
                        return;
                    }

                    self->m_mouse(button, action == GLFW_PRESS ? 0 : 1, static_cast<int>(cursorX * frameW / windowW), static_cast<int>(cursorY * frameH / windowH));
                });
            }

            int frameW = 0, frameH = 0;
            glfwGetFramebufferSize(m_window, &frameW, &frameH);

            if (frameW > 0 && frameH > 0 && resize) {
                resize(frameW, frameH);
            }

            while (!glfwWindowShouldClose(m_window)) {
                glfwPollEvents();
                glfwGetFramebufferSize(m_window, &frameW, &frameH);

                if (frameW < 1 || frameH < 1) {
                    glfwWaitEvents();
                    continue;
                }

                if (m_idle) {
                    m_idle();
                }
                
                display();
                glfwSwapBuffers(m_window);
            }
        }

    private:
        GLFWwindow* m_window = nullptr;
        std::filesystem::path m_assetDir;
        ResizeCallback m_resize = nullptr;
        KeyboardCallback m_keyboard = nullptr;
        IdleCallback m_idle = nullptr;
        MouseCallback m_mouse = nullptr;
    };

} // namespace sample
