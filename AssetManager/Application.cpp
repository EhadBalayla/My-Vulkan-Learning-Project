#include "Application.h"
#include "ButtonWidget.h"

Window Application::window;



void Application::Init() {
    Window window;
    window.initializeClass();
    window.initializeWindow();

    window.addWidget<ButtonWidget>();
}
void Application::Loop() {
    while (!window.shouldWindowClose()) {
        window.pollEvents();
    }
}
void Application::Terminate() {
    window.terminateWindow();
    window.terminateClass();
}