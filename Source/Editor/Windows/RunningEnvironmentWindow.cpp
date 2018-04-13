#include "RunningEnvironmentWindow.h"

void Editor::RunningEnvironmentWindow::updateEnvironmentSettings()
{
	System::Xml::XmlDocument xmlDoc;
	System::Xml::XmlElement^ rootMaterial = xmlDoc.CreateElement(L"EnvironmentSettings");

	{
		System::Xml::XmlElement ^sun = xmlDoc.CreateElement(L"Sun");
		{
			System::Xml::XmlElement ^position = xmlDoc.CreateElement(L"Position");

			position->SetAttribute("X", XSunTextBox->Text);
			position->SetAttribute("Y", YSunTextBox->Text);
			position->SetAttribute("Z", ZSunTextBox->Text);
			sun->AppendChild(position);

			System::Xml::XmlElement ^color = xmlDoc.CreateElement(L"Color");

			color->SetAttribute("R", RSunColorTextBox->Text);
			color->SetAttribute("G", GSunColorTextBox->Text);
			color->SetAttribute("B", BSunColorTextBox->Text);
			sun->AppendChild(color);
		}

		rootMaterial->AppendChild(sun);
	}
	xmlDoc.AppendChild(rootMaterial);

	System::IO::StringWriter ^stringWriter = gcnew System::IO::StringWriter();
	XmlTextWriter ^xmlWriter = gcnew XmlTextWriter(stringWriter);
	xmlDoc.WriteTo(xmlWriter);

	char* xmlText = static_cast<char*>(Marshal::StringToHGlobalAnsi(stringWriter->ToString()).ToPointer());

	Exports::Editor::UpdateEnvironmentSettings(xmlText);

	Marshal::FreeHGlobal(static_cast<IntPtr>(xmlText));
}

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
		else if (ext == L"model")
		{
			ret = true;
		}
		else if (ext == L"xml")
		{
			ret = true;
		}
		else if (ext == L"material")
		{
			ret = true;
		}
	}

	return ret;
}

System::Void Editor::RunningEnvironmentWindow::updateEnvSettings_Click(System::Object^ sender, System::EventArgs^ e)
{
	(void)sender;
	(void)e;
	updateEnvironmentSettings();
}
