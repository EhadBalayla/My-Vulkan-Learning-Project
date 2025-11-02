#include <iostream>
#include "Application.h"

int main() {
    Application app;
    app.Init();
    app.Loop();
    app.Terminate();
    return 0;
}