#pragma once

#include <EngineExports/EngineExports.hpp>

namespace Editor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for Properties
	/// </summary>
	public ref class PropertiesWindow : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		PropertiesWindow(void)
		{
			InitializeComponent();
			
			m_inCollection = false;
		}

		void showInfoAboutFile(String^ file, String^ type, bool inCollection);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PropertiesWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  fileNameLabel;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  typeLabel;
	private: System::Windows::Forms::Button^  addToAssetsButton;

	protected:
	protected:

	private:
		bool m_inCollection;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  assetSrcPathTextBox;

	private: System::Windows::Forms::TextBox^  assetPathTextBox;

	private: System::Windows::Forms::Label^  label3;
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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->fileNameLabel = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->typeLabel = (gcnew System::Windows::Forms::Label());
			this->addToAssetsButton = (gcnew System::Windows::Forms::Button());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->assetSrcPathTextBox = (gcnew System::Windows::Forms::TextBox());
			this->assetPathTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(41, 19);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(26, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"File:";
			// 
			// fileNameLabel
			// 
			this->fileNameLabel->AutoSize = true;
			this->fileNameLabel->Location = System::Drawing::Point(87, 19);
			this->fileNameLabel->Name = L"fileNameLabel";
			this->fileNameLabel->Size = System::Drawing::Size(48, 13);
			this->fileNameLabel->TabIndex = 1;
			this->fileNameLabel->Text = L"fileName";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(33, 43);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(34, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Type:";
			// 
			// typeLabel
			// 
			this->typeLabel->AutoSize = true;
			this->typeLabel->Location = System::Drawing::Point(87, 43);
			this->typeLabel->Name = L"typeLabel";
			this->typeLabel->Size = System::Drawing::Size(27, 13);
			this->typeLabel->TabIndex = 3;
			this->typeLabel->Text = L"type";
			// 
			// addToAssetsButton
			// 
			this->addToAssetsButton->Location = System::Drawing::Point(9, 136);
			this->addToAssetsButton->Name = L"addToAssetsButton";
			this->addToAssetsButton->Size = System::Drawing::Size(89, 23);
			this->addToAssetsButton->TabIndex = 4;
			this->addToAssetsButton->Text = L"Add to assets";
			this->addToAssetsButton->UseVisualStyleBackColor = true;
			this->addToAssetsButton->Click += gcnew System::EventHandler(this, &PropertiesWindow::button1_Click);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(12, 68);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(329, 191);
			this->tabControl1->TabIndex = 5;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->label4);
			this->tabPage1->Controls->Add(this->assetSrcPathTextBox);
			this->tabPage1->Controls->Add(this->assetPathTextBox);
			this->tabPage1->Controls->Add(this->label3);
			this->tabPage1->Controls->Add(this->addToAssetsButton);
			this->tabPage1->Enabled = false;
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(321, 165);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Import";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 52);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(76, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"asset src path:";
			// 
			// assetSrcPathTextBox
			// 
			this->assetSrcPathTextBox->Location = System::Drawing::Point(82, 45);
			this->assetSrcPathTextBox->Name = L"assetSrcPathTextBox";
			this->assetSrcPathTextBox->Size = System::Drawing::Size(233, 20);
			this->assetSrcPathTextBox->TabIndex = 7;
			// 
			// assetPathTextBox
			// 
			this->assetPathTextBox->Location = System::Drawing::Point(82, 19);
			this->assetPathTextBox->Name = L"assetPathTextBox";
			this->assetPathTextBox->Size = System::Drawing::Size(233, 20);
			this->assetPathTextBox->TabIndex = 6;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(17, 22);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(59, 13);
			this->label3->TabIndex = 5;
			this->label3->Text = L"asset path:";
			// 
			// tabPage2
			// 
			this->tabPage2->Enabled = false;
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(321, 165);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"addToScene";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// PropertiesWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(353, 301);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->typeLabel);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->fileNameLabel);
			this->Controls->Add(this->label1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->HideOnClose = true;
			this->Name = L"PropertiesWindow";
			this->Text = L"Properties";
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void addToAssets_Click(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;

		char* destFileName = static_cast<char*>(Marshal::StringToHGlobalAnsi(assetPathTextBox->Text).ToPointer());
		char* srcFileName = static_cast<char*>(Marshal::StringToHGlobalAnsi(assetSrcPathTextBox->Text).ToPointer());

		String^ tmp = assetSrcPathTextBox->Text->Replace(L"/", L"\\");
		String^ path = System::IO::Path::Combine(L"..\\Assets\\Models\\", tmp);
		System::IO::Directory::CreateDirectory(System::IO::Path::GetDirectoryName(path));
		System::IO::File::Copy(fileNameLabel->Text, path);

		Exports::Editor::AddPreviewModelToCollection(destFileName, srcFileName);
		Marshal::FreeHGlobal(static_cast<IntPtr>(destFileName));
		Marshal::FreeHGlobal(static_cast<IntPtr>(srcFileName));

		this->tabPage1->Enabled = false;
	}
};
}
