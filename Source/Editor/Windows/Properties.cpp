#include "Properties.h"

void Editor::PropertiesWindow::showInfoAboutFile(System::String^ filepath, System::String^ type, bool inCollection)
{
	m_inCollection = inCollection;
	fileNameLabel->Text = filepath;
	typeLabel->Text = type;

	this->tabPage1->Enabled = !m_inCollection;
	this->tabPage2->Enabled = m_inCollection;

	if (!m_inCollection)
	{
		fileNameLabel->Text = fileNameLabel->Text->Replace(L"\\", "/");
		assetPathTextBox->Text = L"../Assets/Models/desc/" + typeLabel->Text + L"/" + System::IO::Path::GetFileNameWithoutExtension(fileNameLabel->Text) + ".model";
		assetSrcPathTextBox->Text = L"src/" + typeLabel->Text + L"/" + System::IO::Path::GetFileName(fileNameLabel->Text);
	}
}
