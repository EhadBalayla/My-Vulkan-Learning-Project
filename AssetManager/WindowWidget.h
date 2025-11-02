#pragma once
#include <Windows.h>

class WindowWidget
{
public:

	virtual void createWidget() = 0;
	void destroyWidget();
protected:
	HWND handle;
};

