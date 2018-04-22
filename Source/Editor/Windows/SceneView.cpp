#include "SceneView.h"
#include "Properties.h"

void Editor::SceneView::refreshActors()
{
	for (int i = 0; i < cachedSizeInBytes; i++)
	{
		buffer[i] = 0;
	}
	int res = Exports::Editor::GetActorsXml(buffer, cachedSizeInBytes);

	if (res > 0)
	{
		cachedSizeInBytes = static_cast<int>((float)(cachedSizeInBytes + res) * 1.5f);
		delete[] buffer;
		buffer = new char[cachedSizeInBytes];
		for (int i = 0; i < cachedSizeInBytes; i++)
		{
			buffer[i] = 0;
		}

		int res = Exports::Editor::GetActorsXml(buffer, cachedSizeInBytes);
	}
	else if (res < 0)
	{
		return;
	}

	String^ strNew = gcnew String(buffer);

	array<Byte>^ byteArray = System::Text::Encoding::UTF8->GetBytes(strNew);
	System::IO::MemoryStream^ stream = gcnew System::IO::MemoryStream(byteArray);

	m_reader = XmlReader::Create(stream, m_XmlReaderSettings);
	m_doc->PreserveWhitespace = true;
	m_doc->Load(m_reader);
	m_reader->Close();
	this->treeView1->Nodes->Clear();

	System::Windows::Forms::TreeNode^  root = (gcnew System::Windows::Forms::TreeNode(L"Actors"));
	root->Name = L"Actors";
	root->Text = L"Actors";
	this->treeView1->Nodes->Add(root);
	System::Xml::XmlElement^ rootElement = m_doc->DocumentElement;
	if (root)
	{
		parseActor(root, rootElement);
	}
}

void Editor::SceneView::parseActor(System::Windows::Forms::TreeNode^ root, System::Xml::XmlElement^ rootElement)
{
	String^ actorType = rootElement->Name;
	String^ id = rootElement->GetAttribute("ID")->ToString();

	String^ nodeName = actorType + ":" + id;

	System::Windows::Forms::TreeNode^ newNode = (gcnew System::Windows::Forms::TreeNode(nodeName));
	newNode->Name = nodeName;
	newNode->Text = nodeName;
	System::Xml::XmlElement^ childrenElement = (System::Xml::XmlElement^)rootElement->LastChild;
	for (System::Xml::XmlElement^ child = (System::Xml::XmlElement^)childrenElement->FirstChild; child; child = (System::Xml::XmlElement^)child->NextSibling)
	{
		parseActor(newNode, child);
	}

	root->Nodes->Add(newNode);
}

System::Void Editor::SceneView::treeView1_NodeMouseClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e)
{
	(void)sender;
	if (e && e->Node)
	{
		String^ name = e->Node->Name;
		array<String^>^ splittedPath = name->Split(L':');
		if (splittedPath->Length < 2)
		{
			return;
		}
		int id = int::Parse(splittedPath[1]);

		for (int i = 0; i < cachedSizeInBytes; i++)
		{
			buffer[i] = 0;
		}
		int res = Exports::Editor::GetActorInfo(id, buffer, cachedSizeInBytes);

		if (res > 0)
		{
			cachedSizeInBytes = static_cast<int>((float)(cachedSizeInBytes + res) * 1.5f);
			delete[] buffer;
			buffer = new char[cachedSizeInBytes];
			for (int i = 0; i < cachedSizeInBytes; i++)
			{
				buffer[i] = 0;
			}

			int res = Exports::Editor::GetActorInfo(id, buffer, cachedSizeInBytes);
		}
		else if (res < 0)
		{
			return;
		}

		m_globals->m_propertiesWindow->updateActorXml(gcnew String(buffer), id);
	}
}
