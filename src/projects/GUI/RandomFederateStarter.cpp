#include "StdAfx.h"
#include "RandomFederateStarter.h"


RandomFederateStarter::RandomFederateStarter(System::Windows::Forms::TrackBar^  tb1)
{
	trackBar1 = tb1;
}

void RandomFederateStarter::StartRandomFederateStarter()
{
	while(true){
		int timeVal = trackBar1->Value;
		timeVal *= 1000;
		if(timeVal != 0){
			Process::Start( "SimAgent.exe", "WS");
		}else timeVal = 1000;
		Thread::Sleep(timeVal);
	}
}
