#include "Headers/Engine.h"

Engine::Engine(int w, int h, std::string windowsName)
{
    this->_height = h;
    this->_width = w;
    this->_wRange.push_back((w/2));
    this->_wRange.push_back(-(w/2));
    this->_hRange.push_back((h/2));
    this->_hRange.push_back(-(h/2));

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
    glfwSetFramebufferSizeCallback(this->_win, framebuffer_size_callback);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Engine::~Engine()
{
    glfwTerminate(); // when the program is closed, the window is destroyed otherwise segfault because the window is not destroyed
}

void Engine::Loop()
{
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    int success;
    char infoLog[512];
    /*!SECTION
        GL_STREAM_DRAW: the data is set once and used by the GPU at most a few times.   
        GL_STATIC_DRAW: the data is set once and used many times.
        GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    */
    // Create the vertex shader
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glCreateShader is used to create a shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Glshader source is used to attach the shader source code to the shader object
    glCompileShader(vertexShader);
    // Create the fragment shader
    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glCreateShader is used to create a shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Glshader source is used to attach the shader source code to the shader object
    glCompileShader(fragmentShader);
    // Create the shader program

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    // glCreateProgram is used to create a program object
    glAttachShader(shaderProgram, vertexShader);
    // glAttachShader is used to attach the shader object to the program object
    glAttachShader(shaderProgram, fragmentShader);
    // glAttachShader is used to attach the shader object to the program object
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    GLuint VBO;

    glGenBuffers(1, &VBO);
    // glGenBuffers is used to generate buffer object names
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBindBuffer is used to bind a buffer object to the target
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // glBufferData is used to create and initialize a buffer object's data store
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glVertexAttribPointer is used to define an array of generic vertex attribute data
    glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray is used to enable a generic vertex attribute array
    glUseProgram(shaderProgram);
    // glUseProgram is used to install a program object as part of current rendering state
    

    while (!glfwWindowShouldClose(this->_win))
    {
        this->hookSystem();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        glfwSwapBuffers(this->_win);
    }
}

int Engine::hookSystem()
{
    if(glfwGetKey(this->_win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        (glfwSetWindowShouldClose(this->_win, true));
    return 0;
}