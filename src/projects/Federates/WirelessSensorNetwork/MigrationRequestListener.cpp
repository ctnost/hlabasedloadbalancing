// MigrationRequestListener.cpp : main project file.

#include "stdafx.h"
#include "FederateStarter.h"
#include "CommonHeaders.h"
using namespace System;

int main(array<System::String ^> ^args)
{
/*
	FederateStarter^ fedStarter = gcnew FederateStarter();
	fedStarter->StartFederateListener();
*/
	RTI::RTIambassador *rtiamb = new RTI::RTIambassador();
	char* fedExecName = "HelloWorld";

	try{
		rtiamb->createFederationExecution(fedExecName, "HelloWorld.fed");
	}
	catch (RTI::FederationExecutionAlreadyExists& e){cout << "execution already exists";}
	while (true)
	{
		Console::WriteLine(L"RTI Running");
		    Sleep(1000);
	}

    Console::WriteLine(L"Hello World");
	Console::ReadLine();
    return 0;
}
