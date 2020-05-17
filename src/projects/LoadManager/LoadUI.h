#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace LoadManager {

	/// <summary>
	/// Summary for LoadUI
	/// </summary>
	public ref class LoadUI : public System::Windows::Forms::UserControl
	{
	public:
		LoadUI(void)
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
		~LoadUI()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::Label^  lblLoad;
	protected: 
	public: System::Windows::Forms::Panel^  pnlLoadState;



	public: System::Windows::Forms::Panel^  pnlLoad;

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
			this->lblLoad = (gcnew System::Windows::Forms::Label());
			this->pnlLoadState = (gcnew System::Windows::Forms::Panel());
			this->pnlLoad = (gcnew System::Windows::Forms::Panel());
			this->SuspendLayout();
			// 
			// lblLoad
			// 
			this->lblLoad->AutoSize = true;
			this->lblLoad->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->lblLoad->Location = System::Drawing::Point(-2, 3);
			this->lblLoad->Name = L"lblLoad";
			this->lblLoad->Size = System::Drawing::Size(20, 13);
			this->lblLoad->TabIndex = 0;
			this->lblLoad->Text = L"lbl";
			// 
			// pnlLoadState
			// 
			this->pnlLoadState->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pnlLoadState->Location = System::Drawing::Point(3, 17);
			this->pnlLoadState->Name = L"pnlLoadState";
			this->pnlLoadState->Size = System::Drawing::Size(15, 10);
			this->pnlLoadState->TabIndex = 1;
			// 
			// pnlLoad
			// 
			this->pnlLoad->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pnlLoad->Location = System::Drawing::Point(3, 30);
			this->pnlLoad->Name = L"pnlLoad";
			this->pnlLoad->Size = System::Drawing::Size(15, 116);
			this->pnlLoad->TabIndex = 3;
			// 
			// LoadUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->Controls->Add(this->pnlLoad);
			this->Controls->Add(this->pnlLoadState);
			this->Controls->Add(this->lblLoad);
			this->Name = L"LoadUI";
			this->Size = System::Drawing::Size(26, 148);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
