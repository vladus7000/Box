#include "RenderWindow.h"

void* Editor::RenderWindow::getHwnd()
{
	return this->panel1->Handle.ToPointer();
}
