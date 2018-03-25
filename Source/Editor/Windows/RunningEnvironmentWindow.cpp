#include "RunningEnvironmentWindow.h"

System::String^ Editor::RunningEnvironmentWindow::getTextEditor()
{
	return this->TextEditor->Text;
}

bool Editor::RunningEnvironmentWindow::isTextFile(System::String^ file)
{
	bool ret = false;

	array<String^>^ splittedPath = file->Split(L'.');
	if (splittedPath->Length > 0)
	{
		String^ ext = splittedPath[splittedPath->Length - 1];

		if (ext == L"shader")
		{
			ret = true;
		}
		else if (ext == L"xml")
		{
			ret = true;
		}
	}

	return ret;
}
