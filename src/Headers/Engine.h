#include "Master.h"

class Engine
{
private:
    /// Shading:
    unsigned int _vertexShader;
    unsigned int _fragmentShader;
    unsigned int _shaderProgram;
    GLuint VBO, VAO;
    void setShaderProgram();
    GLFWwindow *_win;
    std::vector<float> _wRange;
    std::vector<float> _hRange;

    /// Window:
    int _width;
    int _height;
    std::vector<float> _vertices;

    //private methods
    int hookSystem();
public:
    Engine(int w, int h, std::string windowsName);
    void Loop();
    ~Engine();
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);