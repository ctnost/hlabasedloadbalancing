#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;

namespace LoadManager {

	/// <summary>
	/// Summary for PCNodeUI
	/// </summary>
	public ref class PCNodeUI : public System::Windows::Forms::UserControl
	{
	public:
		PCNodeUI(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PCNodeUI()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::ListView^  lvFederates;
	protected: 
	public: System::Windows::Forms::Label^  label1;
	public: System::Windows::Forms::Panel^  panelLS;

	private:
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
			this->lvFederates = (gcnew System::Windows::Forms::ListView());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->panelLS = (gcnew System::Windows::Forms::Panel());
			this->SuspendLayout();
			// 
			// lvFederates
			// 
			this->lvFederates->BackColor = System::Drawing::SystemColors::Control;
			this->lvFederates->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->lvFederates->Location = System::Drawing::Point(21, 28);
			this->lvFederates->Name = L"lvFederates";
			this->lvFederates->Size = System::Drawing::Size(110, 97);
			this->lvFederates->TabIndex = 0;
			this->lvFederates->UseCompatibleStateImageBehavior = false;
			this->lvFederates->View = System::Windows::Forms::View::SmallIcon;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(162)));
			this->label1->Location = System::Drawing::Point(41, 12);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(75, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"192.168.2.1";
			// 
			// panelLS
			// 
			this->panelLS->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panelLS->Location = System::Drawing::Point(62, 125);
			this->panelLS->Name = L"panelLS";
			this->panelLS->Size = System::Drawing::Size(36, 16);
			this->panelLS->TabIndex = 2;
			// 
			// PCNodeUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->panelLS);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->lvFederates);
			this->Name = L"PCNodeUI";
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PCNodeUI::PCNodeUI_Paint);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void PCNodeUI_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
			 {
				 GraphicsPath^ gp = gcnew System::Drawing::Drawing2D::GraphicsPath;
				 int chgcorn = 150;

				 RectangleF r1 = RectangleF(0, this->Height - chgcorn, chgcorn, chgcorn);
				 RectangleF r2 = RectangleF(this->Width - chgcorn + 1, this->Height - chgcorn, chgcorn, chgcorn);

				 gp->AddArc(0, 0, chgcorn, chgcorn, 180, 90);
				 gp->AddArc(this->Width - chgcorn, 0, chgcorn, chgcorn, 270, 90);
				 gp->AddRectangle(Rectangle(0, chgcorn / 2, this->Width, this->Height - chgcorn));

				 gp->AddArc(r1, -270, 90);
				 gp->AddArc(r2, 360, 90);
				 System::Drawing::Region ^sekilbolge = gcnew System::Drawing::Region(gp);
				 this->Region = sekilbolge;
			 }
	};
}
