#include "Headers/Master.h"
#include "Headers/Engine.h"


int main(void)
{
	Engine win = Engine(1280, 720, "RayMesh");

	std::cout << "Hello, World!" << std::endl;
	win.Loop();
}