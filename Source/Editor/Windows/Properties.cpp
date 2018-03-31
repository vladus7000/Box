#include "Properties.h"
#include "RecourcesWindow.h"

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
		this->importStatusLabel->Visible = false;
	}
}

void Editor::PropertiesWindow::disableImportForm()
{
	this->tabPage1->Enabled = false;
}

System::Void Editor::PropertiesWindow::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	(void)sender;
	(void)e;
	char* destFileName = static_cast<char*>(Marshal::StringToHGlobalAnsi(assetPathTextBox->Text).ToPointer());
	char* srcFileName = static_cast<char*>(Marshal::StringToHGlobalAnsi(assetSrcPathTextBox->Text).ToPointer());

	try
	{
		String^ tmp = assetSrcPathTextBox->Text->Replace(L"/", L"\\");
		String^ path = System::IO::Path::Combine(L"..\\Assets\\Models\\", tmp);
		System::IO::Directory::CreateDirectory(System::IO::Path::GetDirectoryName(path));
		System::IO::File::Copy(fileNameLabel->Text, path);

		Exports::Editor::AddPreviewModelToCollection(destFileName, srcFileName);
		Exports::Resources::ResyncResourceFolders();
		m_globals->m_recourcesWindow->refreshResourceCollection();
		this->tabPage1->Enabled = false;
		this->importStatusLabel->Visible = true;
		this->importStatusLabel->Text = "Ok, created desc, and copied src files";
		this->importStatusLabel->ForeColor = System::Drawing::Color::Green;
	}
	catch (...)
	{
		this->importStatusLabel->Visible = true;
		this->importStatusLabel->Text = "Error, probably the same asset exists";
		this->importStatusLabel->ForeColor = System::Drawing::Color::Red;
	}

	Marshal::FreeHGlobal(static_cast<IntPtr>(destFileName));
	Marshal::FreeHGlobal(static_cast<IntPtr>(srcFileName));
}
