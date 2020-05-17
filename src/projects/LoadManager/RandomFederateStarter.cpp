#include "StdAfx.h"
#include "RandomFederateStarter.h"


RandomFederateStarter::RandomFederateStarter(System::Windows::Forms::TrackBar^  tb1, System::Windows::Forms::NumericUpDown^  numericUpDown)
{
	trackBar1 = tb1;
	numericUpDown1 = numericUpDown;
}

void RandomFederateStarter::StartRandomFederateStarter()
{
	while(true){
		int timeVal = trackBar1->Value;
		int probVal = (int)numericUpDown1->Maximum - (int)numericUpDown1->Value;
		System::Random^ rnd = gcnew System::Random();
		int rndVal = rnd->Next(1,100);
		timeVal *= 10000;
		if(timeVal != 0 && rndVal >= probVal){
			Process::Start( "SimAgent.exe", "WS");
		}
		if(timeVal == 0) timeVal = 2000;
		Thread::Sleep(timeVal);
	}
}
