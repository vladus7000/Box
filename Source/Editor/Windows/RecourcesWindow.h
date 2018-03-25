#pragma once

#include <EngineExports/EngineExports.hpp>

namespace Editor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Xml;
	using namespace System::Runtime::InteropServices;
	/// <summary>
	/// Summary for RecourcesWindow
	/// </summary>
	public ref class RecourcesWindow : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		RecourcesWindow(void)
		{
			InitializeComponent();
			
			cachedSizeInBytes = 1024;
			buffer = new char[cachedSizeInBytes];

			m_settings = gcnew XmlReaderSettings();
			m_doc = gcnew XmlDocument();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RecourcesWindow()
		{
			if (buffer)
			{
				delete[] buffer;
				cachedSizeInBytes = 0;
			}
			if (components)
			{
				delete components;
			}
		}

	protected:
	private: System::Windows::Forms::Button^  button1;
	protected:

	private:
		int cachedSizeInBytes;
		char* buffer;
		XmlReader^ m_reader;
		XmlReaderSettings^ m_settings;
		XmlDocument^ m_doc;
	private: System::Windows::Forms::TreeView^  treeView1;
	private: System::Windows::Forms::Button^  button2;
			 /// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(30, 3);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Refresh";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &RecourcesWindow::button1_Click);
			// 
			// treeView1
			// 
			this->treeView1->Location = System::Drawing::Point(3, 32);
			this->treeView1->Name = L"treeView1";
			this->treeView1->Size = System::Drawing::Size(342, 385);
			this->treeView1->TabIndex = 2;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(111, 3);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 3;
			this->button2->Text = L"collapseAll";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &RecourcesWindow::button2_Click);
			// 
			// RecourcesWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(353, 429);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->treeView1);
			this->Controls->Add(this->button1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->HideOnClose = true;
			this->Name = L"RecourcesWindow";
			this->Text = L"RecourcesWindow";
			this->Resize += gcnew System::EventHandler(this, &RecourcesWindow::RecourcesWindow_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
	{
		int sizeInBytes = Exports::Resources::GetResourceCollectionSizeForXml();
		if (sizeInBytes > cachedSizeInBytes)
		{
			cachedSizeInBytes = sizeInBytes * 1.5f;
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

		m_reader = XmlReader::Create(stream, m_settings);
		m_doc->PreserveWhitespace = true;
		m_doc->Load(m_reader);
		m_reader->Close();

		XmlNodeList^ list = m_doc->GetElementsByTagName("Item");
		this->treeView1->Nodes->Clear();

		System::Windows::Forms::TreeNode^  root = (gcnew System::Windows::Forms::TreeNode(L"Assets"));
		root->Name = L"Assets";
		root->Text = L"Assets";
		this->treeView1->Nodes->Add(root);

		for (int i = 0; i <  list->Count; i++)
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
				}
			}
		}
	}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
	{
		this->treeView1->CollapseAll();
	}
private: System::Void RecourcesWindow_Resize(System::Object^  sender, System::EventArgs^  e)
{
	this->treeView1->Size = this->ClientSize - System::Drawing::Size(8, 35);
}
};
}
