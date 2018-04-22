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
	else
	{
		m_descriptionPath = getResourcePathFromFullPath(fileNameLabel->Text);
		if (type == "Materials")
		{
			updateMaterialUI(fileNameLabel->Text);
		}
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

void Editor::PropertiesWindow::addResourceToComboBoxes(System::String^ fullPath)
{
	fullPath = fullPath->Replace(L"\\", "/");
	System::String^ resName = getResourcePathFromFullPath(fullPath);
	System::String^ ext = System::IO::Path::GetExtension(fullPath);

	if (ext == ".shader")
	{
		addShader(resName);
	}
	else if (ext == ".material")
	{
		addMaterial(resName);
	}
	else if (ext == ".dds")
	{
		addTexture(resName);
	}
}

void Editor::PropertiesWindow::addShader(System::String^ descName)
{
	materialSelectShaderComboBox->Items->Add(descName);
	materialSelectShaderComboBox->SelectedIndex = 0;

	shaderSelectShaderComboBox->Items->Add(descName);
	shaderSelectShaderComboBox->SelectedIndex = 0;
}

void Editor::PropertiesWindow::addMaterial(System::String^ descName)
{
	selectMaterialComboBox->Items->Add(descName);
	selectMaterialComboBox->SelectedIndex = 0;
}

void Editor::PropertiesWindow::addTexture(System::String^ descName)
{
	textureSlot1ComboBox->Items->Add(descName);
	textureSlot2ComboBox->Items->Add(descName);
	textureSlot3ComboBox->Items->Add(descName);

	textureSlot1ComboBox->SelectedIndex = 0;
	textureSlot2ComboBox->SelectedIndex = 0;
	textureSlot3ComboBox->SelectedIndex = 0;
}

void Editor::PropertiesWindow::clearComboBoxes()
{
	materialSelectShaderComboBox->Items->Clear();
	shaderSelectShaderComboBox->Items->Clear();
	selectMaterialComboBox->Items->Clear();
	textureSlot1ComboBox->Items->Clear();
	textureSlot2ComboBox->Items->Clear();
	textureSlot3ComboBox->Items->Clear();
}

void Editor::PropertiesWindow::updateMaterialUI(System::String^ fileName)
{
	System::Xml::XmlDocument xmlDoc;
	xmlDoc.Load(fileName);
	System::Xml::XmlElement^ root = xmlDoc.DocumentElement;
	if (root)
	{
		materialDescPathTextBox->Text = m_descriptionPath;
		materialNameTextBox->Text = root->GetAttribute("name")->ToString();

		System::Xml::XmlElement^ shader = (System::Xml::XmlElement^)root->FirstChild;
		materialSelectShaderComboBox->Text = shader->GetAttribute("desc")->ToString();
		System::Xml::XmlElement^ textures = (System::Xml::XmlElement^)shader->NextSibling;

		System::Xml::XmlElement^ slot1 = (System::Xml::XmlElement^)textures->FirstChild;
		textureSlot1ComboBox->Text = slot1->GetAttribute("desc")->ToString();

		System::Xml::XmlElement^ slot2 = (System::Xml::XmlElement^)slot1->NextSibling;
		textureSlot2ComboBox->Text = slot2->GetAttribute("desc")->ToString();

		System::Xml::XmlElement^ slot3 = (System::Xml::XmlElement^)slot2->NextSibling;
		textureSlot3ComboBox->Text = slot3->GetAttribute("desc")->ToString();
	}
}

void Editor::PropertiesWindow::updateActorXml(System::String^ xml, int id)
{
	actorsXmlText->Text = xml;
	actorIDLabel->Text = id.ToString();
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
		System::IO::Directory::CreateDirectory(System::IO::Path::GetDirectoryName(assetPathTextBox->Text));
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
	(void)sender;
	(void)e;

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
	(void)sender;
	(void)e;

	char* desc = static_cast<char*>(Marshal::StringToHGlobalAnsi(getResourcePathFromFullPath(fileNameLabel->Text)).ToPointer());
	if (Exports::Editor::AddModelToScene(desc) == 0)
	{
		Exports::Editor::SetViewMode(0);
	}
	Marshal::FreeHGlobal(static_cast<IntPtr>(desc));
}

System::Void Editor::PropertiesWindow::newMaterial_Click(System::Object^ sender, System::EventArgs^ e)
{
	(void)sender;
	(void)e;
	System::Xml::XmlDocument xmlDoc;
	System::Xml::XmlElement^ rootMaterial = xmlDoc.CreateElement(L"Material");
	
	rootMaterial->SetAttribute("name", materialNameTextBox->Text);

	{
		System::Xml::XmlElement ^el = xmlDoc.CreateElement(L"Shader");
		el->SetAttribute("desc", materialSelectShaderComboBox->GetItemText(materialSelectShaderComboBox->SelectedItem));
		rootMaterial->AppendChild(el);
	}
	{
		System::Xml::XmlElement ^text = xmlDoc.CreateElement(L"Textures");
		{
			System::Xml::XmlElement ^el = xmlDoc.CreateElement(L"slot1");
			el->SetAttribute("desc", textureSlot1ComboBox->GetItemText(textureSlot1ComboBox->SelectedItem));
			text->AppendChild(el);
		}
		{
			System::Xml::XmlElement ^el = xmlDoc.CreateElement(L"slot2");
			el->SetAttribute("desc", textureSlot2ComboBox->GetItemText(textureSlot2ComboBox->SelectedItem));
			text->AppendChild(el);
		}
		{
			System::Xml::XmlElement ^el = xmlDoc.CreateElement(L"slot3");
			el->SetAttribute("desc", textureSlot3ComboBox->GetItemText(textureSlot3ComboBox->SelectedItem));
			text->AppendChild(el);
		}
		rootMaterial->AppendChild(text);
	}
	xmlDoc.AppendChild(rootMaterial);

	System::IO::Directory::CreateDirectory(System::IO::Path::GetDirectoryName("../Assets/Materials/" + materialDescPathTextBox->Text));
	xmlDoc.Save("../Assets/Materials/" + materialDescPathTextBox->Text);
}

//update actor
System::Void Editor::PropertiesWindow::button7_Click(System::Object^ sender, System::EventArgs^ e)
{
	(void)sender;
	(void)e;

	char* xml = static_cast<char*>(Marshal::StringToHGlobalAnsi(actorsXmlText->Text).ToPointer());
	
	Exports::Editor::UpdateActor(int::Parse(actorIDLabel->Text), xml);

	Marshal::FreeHGlobal(static_cast<IntPtr>(xml));
}
