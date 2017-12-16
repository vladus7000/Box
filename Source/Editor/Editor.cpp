#include "Editor.h"
#include "Window\Window.hpp"

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
	if (!m_engine)
	{
		void* hwnd = this->panel1->Handle.ToPointer();
		m_engine = new box::Engine();

		m_engine->startup(hwnd);
	}
}

void winforms::MyForm::deinit()
{
	if (m_engine)
	{
		m_engine->shutdown();
		delete m_engine;
		m_engine = nullptr;
	}
}
