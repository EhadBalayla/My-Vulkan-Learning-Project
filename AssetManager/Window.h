#pragma once
#include <Windows.h>
#include "WindowWidget.h"

#include <vector>
#include <iostream>

class Window
{
public:
	Window();
	void initializeClass();
	void initializeWindow();
	
	void terminateWindow();
	void terminateClass();

	bool shouldWindowClose();
	void pollEvents();

	HWND& getHWND();
	WNDCLASS& getWindowClass();


	template <typename T>
	void addWidget() {
		WindowWidget* newWidget = dynamic_cast<WindowWidget*>(new T());
		if (newWidget == nullptr) {
			std::cout << "couldn't create the new widget" << std::endl;
			return;
		}
		widgets.push_back(newWidget);
	}


private:
	WNDCLASS wc;
	HWND windowHandle;
	MSG messages;


	std::vector<WindowWidget*> widgets;
};

