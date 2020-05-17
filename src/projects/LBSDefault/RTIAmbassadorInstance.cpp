#include "RTIAmbassadorInstance.h"

RTI::RTIambassador RTIAmbassadorInstance::GetInstance()
{
	//EnterCriticalSection(&cs);
	if (!rtiCreated){
		try{
			cout << "FED_HW:" << fedExecName <<endl;
			rtiAmb.createFederationExecution(fedExecName,"HelloWorld.fed");
			rtiCreated = true;
			cout << "Yaratýldý" << endl;
		}
		catch (RTI::FederationExecutionAlreadyExists &e){
			cout << "Zaten var:" << &e <<endl;
		}
		catch (RTI::Exception &e){
			cerr << "Error:" <<&e <<endl;
		}
	}
	//LeaveCriticalSection(&cs);
	return rtiAmb;
}

void RTIAmbassadorInstance::Destroy()
{
	try
	{
		cout << "FED_HW: Destroy federation execution called" << endl;
		rtiAmb.destroyFederationExecution(fedExecName);
		cout << "FED_HW: Successful destroy federation execution called" << endl;
	}
	catch (RTI::FederatesCurrentlyJoined& e)
	{
		cerr << "FED_HW: FederatesCurrentlyJoined" << endl;
	}
	catch (RTI::FederationExecutionDoesNotExist& e)
	{
		cerr <<"FED_HW: FederationExecutionDoesNotExist"<<endl;
	}
	catch (RTI::Exception& e) 
	{
		cerr << "FED_HW: Error:" << &e <<endl;
	}
}

char* const RTIAmbassadorInstance::GetFedExecName()
{
	return fedExecName;
}