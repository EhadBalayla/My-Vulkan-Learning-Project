#include "WindowWidget.h"

void WindowWidget::destroyWidget() {
	DeleteObject(handle);
}