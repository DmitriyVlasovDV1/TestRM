#include "def.h"

namespace cgsg {
    class render {
    public:
        explicit render() noexcept = default;
    };

    class shader {
    private:
        GLuint shader_id;

        explicit shader() noexcept : shader_id(0) {
        }
    };

    class vertex_buffer {
    private:
        explicit vertex_buffer() noexcept = default;
    };

    class primitive {
    private:
        explicit primitive() noexcept = default;
    };

    class manager : public render {
    private:
        ::std::vector<::std::unique_ptr<primitive>> primiteves;
        ::std::vector<::std::unique_ptr<shader>> shaders;

        explicit manager() noexcept : render() {
        }

    public:
        [[nodiscard]] static manager & get_manager() {
            static manager singleton;
            return singleton;
        }
    };
}

GLuint shader_program, vertex_shader, fragment_shader;

bool check_shader_compile_status(GLuint obj) {
    GLint status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
        ::std::vector<char> log(length);
        glGetShaderInfoLog(obj, length, &length, &log[0]);
        ::std::cout << &log[0];
        return false;
    }
    return true;
}

// helper to check and display for shader linker error
bool check_program_link_status(GLuint obj) {
    GLint status;
    glGetProgramiv(obj, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
        ::std::vector<char> log(length);
        glGetProgramInfoLog(obj, length, &length, &log[0]);
        ::std::cout << &log[0];
        return false;
    }
    return true;
}

void renderScene() {
    glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_program);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f(0.5, 0.0, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
    }
    glEnd();
    glFlush();
}

int main() {
    if (!glfwInit())
        assert("error in glfw initialization");
#if MODE == 1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#endif

    GLFWwindow * window = glfwCreateWindow(400, 400, "SK4", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        assert("error in glfw window creation");
    }
    glfwMakeContextCurrent(window);

    if (!glewInit())
        assert("error in glew initialization");

    ::std::cout << "Shader language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << ::std::endl;
    ::std::cout << "OpenGL: " << glGetString(GL_VERSION) << ::std::endl;

    // shaders realization
    {
        ::std::string vertex_source =
                "#version 330\n"
                "layout(location = 0) in vec4 input_position;\n"
                "// layout(location = 1) in vec4 input_color;\n"
                "out vec4 output_color;\n"
                "void main() {\n"
                "   output_color = vec4(1, 0, 0, 1);\n"
                "   gl_Position = input_position;\n"
                "}\n";

        ::std::string fragment_source =
                "#version 330\n"
                "in vec4 input_color;\n"
                "layout(location = 0) out vec4 output_color;\n"
                "void main() {\n"
                "   output_color = vec4(1, 0, 0, 1);\n"
                "}\n";

        const char *source;
        int length;

        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        source = vertex_source.c_str();
        length = static_cast<int>(vertex_source.size());
        glShaderSource(vertex_shader, 1, &source, &length);
        glCompileShader(vertex_shader);
        if(!check_shader_compile_status(vertex_shader))
            assert("vertex shader failed to compile");
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        source = fragment_source.c_str();
        length = static_cast<int>(fragment_source.size());
        glShaderSource(fragment_shader, 1, &source, &length);
        glCompileShader(fragment_shader);
        if(!check_shader_compile_status(fragment_shader))
            assert("fragment shader failed to compile");

        shader_program = glCreateProgram();

        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);

        glLinkProgram(shader_program);
        if(!check_program_link_status(shader_program))
            assert("program linking failed");
    }

    while (!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}