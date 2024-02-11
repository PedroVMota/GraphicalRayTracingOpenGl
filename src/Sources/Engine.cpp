#include "Headers/Engine.h"

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

Engine::Engine(int w, int h, std::string windowsName)
{
    this->_height = h;
    this->_width = w;
    this->_wRange.push_back((w/2));
    this->_wRange.push_back(-(w/2));
    this->_hRange.push_back((h/2));
    this->_hRange.push_back(-(h/2));

    std::cout << "Engine" << std::endl;
    std::cout << this->_wRange.at(0) << "," << this->_wRange.at(1) << "," << this->_wRange.size() << std::endl;
    std::cout << this->_hRange.at(0) << "," << this->_hRange.at(1) << "," << this->_hRange.size() << std::endl;


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // -> This represents the version of OpenGL that we are using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // -> This represents the version of OpenGL that we are using
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // -> This represents the version of OpenGL that we are using

    this->_win = glfwCreateWindow(w, h, windowsName.c_str(), NULL, NULL);
    if (this->_win == NULL)
        glfwTerminate();
    glfwMakeContextCurrent(this->_win);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }
    std::cout << "Engine" << std::endl;
    glfwSetFramebufferSizeCallback(this->_win, framebuffer_size_callback);
    
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Engine::~Engine()
{
    //clean up 
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteProgram(this->_shaderProgram);
    glfwTerminate(); // when the program is closed, the window is destroyed otherwise segfault because the window is not destroyed
}

void Engine::setShaderProgram()
{
    this->_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(_vertexShader);

    this->_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(_fragmentShader);

    this->_shaderProgram = glCreateProgram();
    glAttachShader(this->_shaderProgram, this->_vertexShader);
    glAttachShader(this->_shaderProgram, this->_fragmentShader);
    glLinkProgram(this->_shaderProgram);
    glDeleteShader(this->_vertexShader);
    glDeleteShader(this->_fragmentShader);
    glUseProgram(this->_shaderProgram);

    std::cout << "Shader Program" << std::endl;

    this->_vertices = {
        -0.5f, (float)(-0.5f * sqrt(3)), 0.0f,
        0.5f, (float)(-0.5f * sqrt(3)), 0.0f,
        0.0f, (float)(0.5f * sqrt(3)), 0.0f,
        0.3f, (float)(1.0f * sqrt(3)), 1.0f
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->_vertices.size() * sizeof(float), &this->_vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //VBO -> Vertex Buffer Object
    //VAO -> Vertex Array Object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

void Engine::Loop()
{
    std::cout << "Loop" << std::endl;
    auto t_start = std::chrono::high_resolution_clock::now();
    int frame_count = 0;
    this->setShaderProgram();

    
    while (!glfwWindowShouldClose(this->_win))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        this->hookSystem();
        glUseProgram(this->_shaderProgram);
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 4);

        glfwPollEvents();
        glfwSwapBuffers(this->_win);

        frame_count++;
        auto t_end = std::chrono::high_resolution_clock::now();
        double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();

        if (elapsed_time_ms > 1000) { // every second
            double fps = frame_count * 1000.0 / elapsed_time_ms;
            std::cout << "FPS: " << fps << std::endl;

            // reset for the next count
            t_start = t_end;
            frame_count = 0;
        }
    }
}

int Engine::hookSystem()
{
    if(glfwGetKey(this->_win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        (glfwSetWindowShouldClose(this->_win, true));
    return 0;
}