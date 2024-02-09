#include "Master.h"

class Engine
{
private:
    int _width;
    int _height;
    std::vector<float> _wRange;
    std::vector<float> _hRange;
    GLFWwindow *_win;
    int hookSystem();
public:
    Engine(int w, int h, std::string windowsName);
    void Loop();
    ~Engine();
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);