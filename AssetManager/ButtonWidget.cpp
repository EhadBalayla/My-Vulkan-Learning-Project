#include "ButtonWidget.h"
#include "Application.h"

ButtonWidget::ButtonWidget() {
	createWidget();
}

void ButtonWidget::createWidget() {
	handle = CreateWindow(L"BUTTON", L"press me", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 10, 100, 100, Application::window.getHWND(), NULL, (HINSTANCE)GetWindowLongPtr(Application::window.getHWND(), GWLP_HINSTANCE), NULL);
}