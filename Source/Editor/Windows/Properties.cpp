#include "Properties.h"

void Editor::PropertiesWindow::showInfoAboutFile(System::String^ file)
{
	fileNameLabel->Text = file;
}
