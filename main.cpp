#include <iostream>

// A useful shorthand that not all compilers provide by default
using uint = unsigned int;

#ifdef _MSC_VER
// This is needed to be able to use M_PI on MSVC compiler
#define _USE_MATH_DEFINES
#include <math.h>
#endif

// Include the math library we will be using, GLM
#include <glm/glm.hpp>
// Also include some helpers for common transformation matrices
#include <glm/gtx/transform.hpp>

// Include the glad lib, which contains our OpenGL bindings
#include <glad/gl.h>

// Include the GLFW lib, which is out windowing library, but also tell it
// to not include its own OpenGL bindings with the use of the GLFW_INCLUDE_NONE macro.
// This is done to prevent conflicts
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
// Define the OpenGL version to use at runtime, later passed to GLFW since it
// is responsible for creating the context.
// 4.1 is the highest version supported by basically all Windows, Linux and Mac machines.
// This is limited by Apple who have deprecated support for OpenGL and so are no longer
// supporting newer version (currently there is up to 4.6).
#define OPENGL_VERSION_MAJOR 4
#define OPENGL_VERSION_MINOR 1

// Include some code that helps with shader creation
#include "helpers/ShaderHelper.h"

// Include some code that helps with using ImGui
#include "helpers/imgui/ImGuiManager.h"

// Some constant window properties we define here for now, since we currently don't handle
// window resizing.
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define WINDOW_TITLE "Lab 6"

// A flag to enable or disable vsync (aka frame limiting)
#define V_SYNC false

const int NUM_SIDES = 6;
const int NUM_TRIANGLES = 2 * NUM_SIDES;
const int NUM_VERTICES = 3 * NUM_TRIANGLES;

// This time create a struct representing the data of a single vertex, because we will be
// interleaving the vertex attributes instead of laying it out with each attribute together like before.
struct Vertex {
    glm::vec3 position;
    glm::vec3 colour;
};

Vertex vertices[NUM_VERTICES] = {
        Vertex{glm::vec3(-0.5, -0.5, 0.5), glm::vec3(1.0, 0.0, 0.0)},
        Vertex{glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.0, 0.0, 1.0)},
        Vertex{glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 1.0, 1.0)},
        Vertex{glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.0, 0.0, 1.0)},

        Vertex{glm::vec3(-0.5, -0.5, -0.5), glm::vec3(1.0, 0.0, 0.0)},
        Vertex{glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.0, 0.0, 1.0)},
        Vertex{glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.0, 1.0, 1.0)},
        Vertex{glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.0, 0.0, 1.0)},

        Vertex{glm::vec3(0.5, -0.5, -0.5), glm::vec3(1.0, 0.0, 0.0)},
        Vertex{glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.0, 0.0, 1.0)},
        Vertex{glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 1.0, 1.0)},
        Vertex{glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.0, 0.0, 1.0)},

        Vertex{glm::vec3(-0.5, -0.5, -0.5), glm::vec3(1.0, 0.0, 0.0)},
        Vertex{glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.0, 0.0, 1.0)},
        Vertex{glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.0, 1.0, 1.0)},
        Vertex{glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.0, 0.0, 1.0)},

        Vertex{glm::vec3(-0.5, 0.5, -0.5), glm::vec3(1.0, 0.0, 0.0)},
        Vertex{glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.0, 0.0, 1.0)},
        Vertex{glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 1.0, 1.0)},
        Vertex{glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.0, 0.0, 1.0)},

        Vertex{glm::vec3(-0.5, -0.5, -0.5), glm::vec3(1.0, 0.0, 0.0)},
        Vertex{glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.0, 0.0, 1.0)},
        Vertex{glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.0, 1.0, 1.0)},
        Vertex{glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.0, 1.0, 0.0)},
        Vertex{glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.0, 0.0, 1.0)}
};

int xyz_multipliers_location;

void init() {
    // Create a vertex array object
    uint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object for vertex data
    uint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    // Since the data is laid out contiguously already,
    // can just directly upload instead of needing to do it in two steps
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * NUM_VERTICES, vertices, GL_STATIC_DRAW);

    // Load shaders and use the resulting shader program
    uint program = ShaderHelper::load_shader("vert.glsl", "frag.glsl");
    glUseProgram(program);

    // Initialize the vertex position attribute from the vertex shader.
    // Note: that this time in the shader we use a layout qualifier to specify the attribute location,
    // so there is no need to fetch it.
    glEnableVertexAttribArray(0);
    // Also note: now that we are interleaving data, the stride and offset are different
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));

    // Likewise, initialize the vertex color attribute. And likewise location is specified in the shader.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, colour));

    xyz_multipliers_location = glGetUniformLocation(program, "xyzMultipliers");

    // We need to enable the depth test to discard fragments that are behind
    // previously drawn fragments for the same pixel.
    glEnable(GL_DEPTH_TEST);

    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

bool animate_rotation = true;
glm::vec3 rotation_angles{0.0f};
glm::vec3 rotation_speed{1.0f};

void ui() {
    // Create an ImGUI window, the function returns true if the window is expanded and false if collapsed,
    // so we use an if to only add things to the window it is open
    if (ImGui::Begin("ImGUI Window", nullptr, ImGuiWindowFlags_NoFocusOnAppearing)) {
        // Add a checkbox to control whether to animate the rotation
        ImGui::Checkbox("Animate Rotation", &animate_rotation);

        // Add a drag to edit the 3 components of the rotation_speed vector, setting the drag speed
        // to 0.1f and don't clamp the range.
        ImGui::DragFloat3("Rotation Speeds", &rotation_speed[0], 0.1f);

        // Add a slider to edit the 3 components of the rotation_angle vector, setting the rang to be [0, 2pi]
        ImGui::SliderFloat3("Rotation Angle", &rotation_angles[0], 0.0f, 2.0f * (float) M_PI);

    }
    // Since ImGUI is an immediate mode UI with hidden internal state, we need to explicitly tell it that
    // we are done talking about the current window, to do that we call End();
    ImGui::End();
}

void draw(GLFWwindow *window, ImGuiManager& imgui_manager) {
    // Tell ImGUI we are starting a new frame
    imgui_manager.new_frame();
    ImGuiManager::enable_main_window_docking();

    // Here we add the UI elements, this can be called any time between imgui_manager.new_frame() and imgui_manager.render().
    // However, values update by the UI are updated when calling the function like DragFloat3, that means that processing the UI
    // before using the values like this will minimise the latency between changing something, and it's showing up. As otherwise
    // it wouldn't have a visual effect until the next frame.
    ui();

    static auto last_time = glfwGetTime();
    auto time = glfwGetTime();
    auto delta = (float) (time - last_time);
    last_time = time;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (animate_rotation) {
        rotation_angles += delta * rotation_speed;
        rotation_angles = glm::mod(rotation_angles, 2.0f * (float) M_PI);
    }

    // Note that the glm matrix constructor takes column vectors,
    // so it visually appears to be the transpose of the actual matrix.
    auto x_angle = rotation_angles.x;
    glm::mat3 x_rotation = glm::mat3(
            {1.0f, 0.0f, 0.0f},
            {0.0f, std::cos(x_angle), std::sin(x_angle)},
            {0.0f, -std::sin(x_angle), std::cos(x_angle)}
    );
    auto y_angle = rotation_angles.y;
    glm::mat3 y_rotation = glm::mat3(
            {std::cos(y_angle), 0.0, -std::sin(y_angle)},
            {0.0f, 1.0f, 0.0f},
            {std::sin(y_angle), 0.0, std::cos(y_angle)}
    );

    // Scale the x by 0.25
    glm::mat3 shrink_x = glm::mat3(
            {0.25f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f}
    );

    // Since we want the shrink to handle in model space we put in on the right,
    // because matrices are "applied" right to left in the conventions most commonly used with OpenGL
    glm::mat3 combined_matrix = y_rotation * x_rotation * shrink_x;

    // NOTE: Since glm was designed with OpenGL in mind, it stores matrices in column major, so
    // the is no need to transpose, hence using GL_FALSE
    glUniformMatrix3fv(xyz_multipliers_location, 1, GL_FALSE, &combined_matrix[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

    // Tell ImGUI to now render itself, this is done after drawing the scene so that it appears on top.
    imgui_manager.render();

    glfwSwapBuffers(window);
}

void key_callback(GLFWwindow *window, int key, int /*scancode*/, int /*action*/, int /*mods*/) {
    switch (key) {
        case GLFW_KEY_ESCAPE: {
            // Don't directly close this time, instead tell GLFW to mark the window as "Should close",
            // the same as if we have hit the close button.
            glfwSetWindowShouldClose(window, true);
            break;
        }
    }
}

void refresh_callback(GLFWwindow *window) {
    auto* imgui_manager = static_cast<ImGuiManager *>(glfwGetWindowUserPointer(window));
    draw(window, *imgui_manager);
}

// A callback function that we give GLFW to report errors with.
void glfwErrorCallback(int code, const char *msg) {
    std::cout << "GLFW Error (" << code << ")\n\t" << "msg: " << msg << std::endl;
}

int main() {
    glfwSetErrorCallback(glfwErrorCallback);
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    glfwMakeContextCurrent(window);


    glfwSwapInterval(V_SYNC ? 1 : 0);

    int status = gladLoadGL((GLADloadfunc) glfwGetProcAddress);
    if (!status) {
        std::cerr << "Failed to Load OpenGL functions, via GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Print some info about the OpenGL context
    std::cout << "" << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "" << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "" << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "" << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "" << "------------------------------------------------------------" << std::endl;
    std::cout << "" << std::endl;

    // Set most callbacks before creating ImGuiManager, as ImGUI will hook into
    // the same glfw callbacks and chain call the ones previous set.
    // We could do the same if we set out callbacks afterwards, but this is less work.
    glfwSetKeyCallback(window, key_callback);

    ImGuiManager imgui_manager{window};

    // Store a pointer to ImGuiManager with the window, so that we can access it in refresh_callback
    glfwSetWindowUserPointer(window, &imgui_manager);

    init();

    // This callback may only end up being called very rarely if at all, hence why we use a main loop.
    // Also, ImGuiManager doesn't override this callback, so it can be called afterwards.
    // In fact in needs to be set after init() and glfwSetWindowUserPointer() otherwise the draw() call
    // will be invalid.
    glfwSetWindowRefreshCallback(window, refresh_callback);

    // GLFW works a bit differently than GLUT, we have to write the main loop ourselves, though this gives more control.
    // This does have the downside though that the loop may block when resizing or moving the window on some systems,
    // see docs of glfwPollEvents call for more info, this is why we also use glfwSetWindowRefreshCallback.
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // Tell glfw to check for new events that have arrived from the OS since the last call.

        draw(window, imgui_manager); // Just call draw
    }

    ImGuiManager::cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}
