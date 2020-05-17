// MigrationRequestListener.cpp : main project file.

#include "stdafx.h"
#include "FederateStarter.h"
#include "FederateTrigger.h"
using namespace System;
using namespace System::Threading;

int main(array<System::String ^> ^args)
{
	FederateTrigger^ fedTrigger = gcnew FederateTrigger();
	ThreadStart^ threadDelegate1 = gcnew ThreadStart( fedTrigger, &FederateTrigger::StartFederateTrigger );
	Thread^ newThread1 = gcnew Thread( threadDelegate1 );
	newThread1->Start();

	FederateStarter^ fedStarter = gcnew FederateStarter();
	ThreadStart^ threadDelegate2 = gcnew ThreadStart( fedStarter, &FederateStarter::StartFederateListener );
	Thread^ newThread2 = gcnew Thread( threadDelegate2 );
	newThread2->Start();

    Console::WriteLine(L"Hello World");
	Console::ReadLine();
    return 0;
}
