#include "Headers/Master.h"
#include "Headers/Engine.h"


int main(void)
{
	Engine win = Engine(300, 300, "RayMesh");

	win.Loop();
}