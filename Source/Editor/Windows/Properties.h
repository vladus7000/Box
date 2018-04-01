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
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for Properties
	/// </summary>
	public ref class PropertiesWindow : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		PropertiesWindow()
		{
			InitializeComponent();
			
			m_inCollection = false;
		}

		void setGlobals(Globals^ globals) { m_globals = globals; }
		void showInfoAboutFile(String^ file, String^ type, bool inCollection);
		void disableImportForm();
		System::String^ getResourcePathFromFullPath(System::String^ fullPath);

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
		Globals^ m_globals;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  assetSrcPathTextBox;

	private: System::Windows::Forms::TextBox^  assetPathTextBox;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  importStatusLabel;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  modelNameTextBox;
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
			this->importStatusLabel = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->assetSrcPathTextBox = (gcnew System::Windows::Forms::TextBox());
			this->assetPathTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->modelNameTextBox = (gcnew System::Windows::Forms::TextBox());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
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
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Controls->Add(this->modelNameTextBox);
			this->tabPage1->Controls->Add(this->importStatusLabel);
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
			// importStatusLabel
			// 
			this->importStatusLabel->AutoSize = true;
			this->importStatusLabel->ForeColor = System::Drawing::SystemColors::ControlText;
			this->importStatusLabel->Location = System::Drawing::Point(104, 141);
			this->importStatusLabel->Name = L"importStatusLabel";
			this->importStatusLabel->Size = System::Drawing::Size(37, 13);
			this->importStatusLabel->TabIndex = 6;
			this->importStatusLabel->Text = L"Status";
			this->importStatusLabel->Visible = false;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 59);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(76, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"asset src path:";
			// 
			// assetSrcPathTextBox
			// 
			this->assetSrcPathTextBox->Location = System::Drawing::Point(82, 52);
			this->assetSrcPathTextBox->Name = L"assetSrcPathTextBox";
			this->assetSrcPathTextBox->Size = System::Drawing::Size(233, 20);
			this->assetSrcPathTextBox->TabIndex = 7;
			// 
			// assetPathTextBox
			// 
			this->assetPathTextBox->Location = System::Drawing::Point(82, 26);
			this->assetPathTextBox->Name = L"assetPathTextBox";
			this->assetPathTextBox->Size = System::Drawing::Size(233, 20);
			this->assetPathTextBox->TabIndex = 6;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(17, 29);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(59, 13);
			this->label3->TabIndex = 5;
			this->label3->Text = L"asset path:";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->button2);
			this->tabPage2->Controls->Add(this->button1);
			this->tabPage2->Enabled = false;
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(321, 165);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"addToScene";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(133, 17);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(93, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"addToScene";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &PropertiesWindow::addToSceneClick);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(20, 17);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(92, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"previewModel";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &PropertiesWindow::previewModelClick);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(6, 85);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(66, 13);
			this->label5->TabIndex = 10;
			this->label5->Text = L"model Name";
			// 
			// modelNameTextBox
			// 
			this->modelNameTextBox->Location = System::Drawing::Point(82, 78);
			this->modelNameTextBox->Name = L"modelNameTextBox";
			this->modelNameTextBox->Size = System::Drawing::Size(233, 20);
			this->modelNameTextBox->TabIndex = 9;
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
			this->tabPage2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void addToAssets_Click(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void previewModelClick(System::Object^  sender, System::EventArgs^  e);
	private: System::Void addToSceneClick(System::Object^  sender, System::EventArgs^  e);
};
}
