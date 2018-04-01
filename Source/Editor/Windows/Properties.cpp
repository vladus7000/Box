#include "Properties.h"
#include "RecourcesWindow.h"

void Editor::PropertiesWindow::showInfoAboutFile(System::String^ filepath, System::String^ type, bool inCollection)
{
	m_inCollection = inCollection;
	fileNameLabel->Text = filepath->Replace(L"\\", "/");
	typeLabel->Text = type;

	this->tabPage1->Enabled = !m_inCollection;
	this->tabPage2->Enabled = m_inCollection;

	if (!m_inCollection)
	{
		String^ name = System::IO::Path::GetFileNameWithoutExtension(fileNameLabel->Text);
		assetPathTextBox->Text = L"../Assets/Models/desc/" + typeLabel->Text + L"/" + name + ".model";
		assetSrcPathTextBox->Text = L"src/" + typeLabel->Text + L"/" + System::IO::Path::GetFileName(fileNameLabel->Text);
		modelNameTextBox->Text = name;
		this->importStatusLabel->Visible = false;
	}
}

void Editor::PropertiesWindow::disableImportForm()
{
	this->tabPage1->Enabled = false;
}

System::String^ Editor::PropertiesWindow::getResourcePathFromFullPath(System::String^ fullPath)
{
	array<String^>^ splittedPath = fullPath->Split(L'/');

	String^ tmp = gcnew String("");
	for (int i = 3; i < splittedPath->Length; i++)
	{
		tmp += splittedPath[i];
		if (i < splittedPath->Length - 1)
		{
			tmp += L"/";
		}
	}
	return tmp;
}

System::Void Editor::PropertiesWindow::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	(void)sender;
	(void)e;
	char* destFileName = static_cast<char*>(Marshal::StringToHGlobalAnsi(assetPathTextBox->Text).ToPointer());
	char* srcFileName = static_cast<char*>(Marshal::StringToHGlobalAnsi(assetSrcPathTextBox->Text).ToPointer());
	char* modelName = static_cast<char*>(Marshal::StringToHGlobalAnsi(modelNameTextBox->Text).ToPointer());

	try
	{
		String^ tmp = assetSrcPathTextBox->Text->Replace(L"/", L"\\");
		String^ path = System::IO::Path::Combine(L"..\\Assets\\Models\\", tmp);
		System::IO::Directory::CreateDirectory(System::IO::Path::GetDirectoryName(path));
		System::IO::File::Copy(fileNameLabel->Text, path);

		Exports::Editor::AddPreviewModelToCollection(modelName, destFileName, srcFileName);
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
	Marshal::FreeHGlobal(static_cast<IntPtr>(modelName));
}

System::Void Editor::PropertiesWindow::previewModelClick(System::Object^ sender, System::EventArgs^ e)
{
	char* desc = static_cast<char*>(Marshal::StringToHGlobalAnsi(getResourcePathFromFullPath(fileNameLabel->Text)).ToPointer());
	Exports::Editor::ClearPreviewModel();
	if (Exports::Editor::AddModelToScene(desc) == 0)
	{
		Exports::Editor::SetViewMode(1);
	}
	Marshal::FreeHGlobal(static_cast<IntPtr>(desc));
}

System::Void Editor::PropertiesWindow::addToSceneClick(System::Object^ sender, System::EventArgs^ e)
{
	char* desc = static_cast<char*>(Marshal::StringToHGlobalAnsi(getResourcePathFromFullPath(fileNameLabel->Text)).ToPointer());
	if (Exports::Editor::AddModelToScene(desc) == 0)
	{
		Exports::Editor::SetViewMode(0);
	}
	Marshal::FreeHGlobal(static_cast<IntPtr>(desc));
}
