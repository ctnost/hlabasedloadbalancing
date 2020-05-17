#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::Diagnostics;

ref class RandomFederateStarter
{
private:
	System::Windows::Forms::TrackBar^  trackBar1;
	System::Windows::Forms::NumericUpDown^  numericUpDown1;
public:
	RandomFederateStarter(System::Windows::Forms::TrackBar^  tb1, System::Windows::Forms::NumericUpDown^  numericUpDown);
	void StartRandomFederateStarter();
};

