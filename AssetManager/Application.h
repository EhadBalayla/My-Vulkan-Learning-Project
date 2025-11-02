#pragma once
#include "Window.h"

class Application
{
public:

	static Window window;

	void Init();
	void Loop();
	void Terminate();
private:

};

