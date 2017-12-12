#include "Editor.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	winforms::MyForm form;

	Application::Run(%form);
}

void winforms::MyForm::init()
{
	box::Engine* e = new box::Engine();
	delete e;
}
