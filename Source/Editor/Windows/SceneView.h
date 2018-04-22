#pragma once
#include <EngineExports/EngineExports.hpp>
#include "Globals.hpp"

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
	/// Summary for SceneView
	/// </summary>
	public ref class SceneView : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		SceneView(void)
		{
			InitializeComponent();
			cachedSizeInBytes = 2048;
			buffer = new char[cachedSizeInBytes];

			m_XmlReaderSettings = gcnew XmlReaderSettings();
			m_doc = gcnew XmlDocument();
		}

		void refreshActors();
		void setGlobals(Globals^ globals) { m_globals = globals; }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SceneView()
		{
			if (components)
			{
				delete components;
			}
			delete[] buffer;
		}
	private: System::Windows::Forms::TreeView^  treeView1;
	private: System::Windows::Forms::Button^  button1;
	Globals^ m_globals;
	int cachedSizeInBytes;
	char* buffer;
	XmlReader^ m_reader;
	XmlReaderSettings^ m_XmlReaderSettings;
	XmlDocument^ m_doc;
	protected:
	protected:

	private:
		void parseActor(System::Windows::Forms::TreeNode^ root, System::Xml::XmlElement^ rootElement);
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
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// treeView1
			// 
			this->treeView1->Location = System::Drawing::Point(12, 33);
			this->treeView1->Name = L"treeView1";
			this->treeView1->Size = System::Drawing::Size(318, 463);
			this->treeView1->TabIndex = 0;
			this->treeView1->NodeMouseClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &SceneView::treeView1_NodeMouseClick);
			this->treeView1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &SceneView::treeView1_MouseClick);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(40, 4);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Refresh";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &SceneView::button1_Click);
			// 
			// SceneView
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(342, 525);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->treeView1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->HideOnClose = true;
			this->Name = L"SceneView";
			this->Text = L"SceneView";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
		refreshActors();
	}
	private: System::Void treeView1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		(void)sender;
		(void)e;
	}

	private: System::Void treeView1_NodeMouseClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e);
};
}
