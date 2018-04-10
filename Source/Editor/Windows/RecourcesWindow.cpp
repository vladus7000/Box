#include "RecourcesWindow.h"

void Editor::RecourcesWindow::refreshResourceCollection()
{
	m_propertiesWindow->clearComboBoxes();
	int sizeInBytes = Exports::Resources::GetResourceCollectionSizeForXml();
	if (sizeInBytes > cachedSizeInBytes)
	{
		cachedSizeInBytes = static_cast<int>((float)sizeInBytes * 1.5f);
		delete[] buffer;

		buffer = new char[cachedSizeInBytes];
	}
	for (int i = 0; i < cachedSizeInBytes; i++)
	{
		buffer[i] = 0;
	}
	Exports::Resources::SerializeResourceCollectionToXml(buffer);
	String^ strNew = gcnew String(buffer);

	array<Byte>^ byteArray = System::Text::Encoding::UTF8->GetBytes(strNew);
	System::IO::MemoryStream^ stream = gcnew System::IO::MemoryStream(byteArray);

	m_reader = XmlReader::Create(stream, m_XmlReaderSettings);
	m_doc->PreserveWhitespace = true;
	m_doc->Load(m_reader);
	m_reader->Close();

	XmlNodeList^ list = m_doc->GetElementsByTagName("Item");
	this->treeView1->Nodes->Clear();

	System::Windows::Forms::TreeNode^  root = (gcnew System::Windows::Forms::TreeNode(L"Assets"));
	root->Name = L"Assets";
	root->Text = L"Assets";
	this->treeView1->Nodes->Add(root);

	for (int i = 0; i < list->Count; i++)
	{
		String^ path = list->Item(i)->InnerText;
		array<String^>^ splittedPath = path->Split(L'/');

		System::Windows::Forms::TreeNode^ nextNode = root;
		for (int j = 2; j < splittedPath->Length; j++)
		{
			String^ currentFolder = splittedPath[j];

			bool found = false;
			for (int nodeI = 0; nodeI < nextNode->Nodes->Count; nodeI++)
			{
				System::Windows::Forms::TreeNode^ node = nextNode->Nodes[nodeI];
				if (node->Name == currentFolder)
				{
					found = true;
					nextNode = node;
					break;
				}
			}
			if (!found)
			{
				System::Windows::Forms::TreeNode^ newNode = (gcnew System::Windows::Forms::TreeNode(currentFolder));
				newNode->Name = currentFolder;
				newNode->Text = currentFolder;
				nextNode->Nodes->Add(newNode);
				nextNode = newNode;

				if (currentFolder->Contains(L"."))
				{
					String^ fullPath = L"../" + newNode->FullPath;
					m_propertiesWindow->addResourceToComboBoxes(fullPath);
				}
			}
		}
	}
	root->Expand();
}
