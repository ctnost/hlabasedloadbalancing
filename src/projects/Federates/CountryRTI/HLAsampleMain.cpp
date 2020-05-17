#include <stdlib.h>
#include <string.h>

#include "RTI.hh"
#include "fedtime.hh"
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;
using std::cout;
using std::cerr;
using std::endl;

#include "RTIAmbassadorInstance.h"
#include "LBSDefaultFederateAmb.h"
#include "FederateFactory.h"
#include <fedtime.hh>

std::string SynPoint("zhanghui");
std::string Syntag("tag");

RTI::FedTime &GrantTime = (*(RTI::FedTimeFactory::makeZero()));

double convertTime( const RTI::FedTime& theTime );
RTI::RTIambassador         RTIAmbassadorInstance::rtiAmb;
bool			           RTIAmbassadorInstance::rtiCreated = false;
char* const	    		   RTIAmbassadorInstance::fedExecName = "HelloWorld";


void waitForUser()
{
	cout << " >>>>>>>>>> Press Enter to Continue <<<<<<<<<<" << endl;
	string line;
	getline( cin, line );
}

int hw_main()
{
	FederateFactory* federateFactory = new FederateFactory();
	const char *fedName = "Country";
	int numberofTicks(100);
    int loopCounter = 0;
    try
    {
		RTIAmbassadorInstance::rtiAmb.createFederationExecution(RTIAmbassadorInstance::fedExecName,"HelloWorld.fed");
		RTIAmbassadorInstance::rtiAmb.tick(0.1,2.0);
		SimAgent* simAgent = federateFactory->CreateFederate(fedName);
		RTI::FederateHandle federateID = RTIAmbassadorInstance::rtiAmb.joinFederationExecution
			(fedName,RTIAmbassadorInstance::fedExecName,simAgent->GetLBSFederateAmbassador());

		simAgent->GetLBSFederate()->Init(&RTIAmbassadorInstance::rtiAmb);

        simAgent->GetLBSFederate()->PublishAndSubscribe();
        simAgent->GetLBSFederate()->Register();
        const RTIfedTime timeStep(1.0);
		simAgent->GetLBSFederateAmbassador()->timeAdvGrant = false;
		RTIAmbassadorInstance::rtiAmb.enableAsynchronousDelivery();
        RTIAmbassadorInstance::rtiAmb.enableTimeConstrained();
		simAgent->GetLBSFederateAmbassador()->timeAdvGrant = false;
        simAgent->GetLBSFederateAmbassador()->TimeConstrained = false;
		while (simAgent->GetLBSFederateAmbassador()->TimeConstrained == false)
        {
			RTIAmbassadorInstance::rtiAmb.tick(0.01,1.0);
        }
		cout << simAgent->GetLBSFederateAmbassador()->TimeConstrained << endl;

        try
        {
			cout << "lookahead = "
				<< convertTime( simAgent->GetLBSFederate()->GetLookahead()) <<" - "<<endl;

			RTIAmbassadorInstance::rtiAmb.enableTimeRegulation(GrantTime,simAgent->GetLBSFederate()->GetLookahead());
            simAgent->GetLBSFederateAmbassador()->timeAdvGrant = false;
            simAgent->GetLBSFederateAmbassador()->TimeRegulation = false;
            while (!simAgent->GetLBSFederateAmbassador()->TimeRegulation)
            {
				RTIAmbassadorInstance::rtiAmb.tick(0.01,1.0);
            }
        }
        catch (RTI::Exception& e)
        {
			cerr <<"Error: " << &e <<endl;
        }
        cout<<"look head finished"<<endl<<endl;
        RTIAmbassadorInstance::rtiAmb.tick(0.1,0.2);

		//wait until other federates have joined.
		/*while (discoverNum < FederateNum -1)
		{
				rtiAmb.tick(0.1,0.2);
		}
		cout<<" wait finished "<<endl<<endl;*/

        RTIAmbassadorInstance::rtiAmb.registerFederationSynchronizationPoint(SynPoint.c_str(),Syntag.c_str());
        RTIAmbassadorInstance::rtiAmb.tick(0.1,0.2);
        while (!simAgent->GetLBSFederateAmbassador()->SyncPointRegisterFailed 
				&& !simAgent->GetLBSFederateAmbassador()->SyncPointRegisterSucceeded)
        {
			RTIAmbassadorInstance::rtiAmb.tick(0.1,0.2);
        }
        cout<<" SyncPointRegisterFailed "<<endl<<endl;
        RTIAmbassadorInstance::rtiAmb.tick(0.1,0.2);
        while (!simAgent->GetLBSFederateAmbassador()->AnnounceSyncReceived)
        {
			RTIAmbassadorInstance::rtiAmb.tick(0.1,0.2);
        }

        cout<<"AnnounceSyncReceived  "<<endl<<endl;        
        RTIAmbassadorInstance::rtiAmb.tick(0.1,0.2);
        RTIAmbassadorInstance::rtiAmb.synchronizationPointAchieved(SynPoint.c_str());
        cout <<" synchronizationPointAchieved "<<endl<<endl;

        while (!simAgent->GetLBSFederateAmbassador()->FederationalsSynchronized)
        {
			RTIAmbassadorInstance::rtiAmb.tick(0.1,0.2);
        }

        cout<<"-----FederationalsSynchronized-----"<<endl<<endl;
        RTIAmbassadorInstance::rtiAmb.tick(0.1,0.2);
        cout<<"-----------------------------------------------------"<<endl<<"  "<<endl<<endl;
        int counter = 0;
	    simAgent->GetLBSFederateAmbassador()->timeAdvGrant = true;

        while (counter++ < numberofTicks -1)
        {
            cout << "HelloWorld Event Loop Iteration # :" << counter <<endl;
            simAgent->GetLBSFederate()->Update(GrantTime);       

			//ErdControl
            /*Country* pCountry = NULL;
            for (unsigned int i = 0; i < Country::ms_extentCardinality; i++ )
            {
                pCountry = Country::ms_countryExtent[i];
                if (pCountry)
                {
                    cout <<"{ country[" << i <<"]=" << pCountry <<"}"<<endl;
                }
            }*/
                        
            if (simAgent->GetLBSFederateAmbassador()->timeAdvGrant)
            {
                try
                {
                    RTIfedTime requestTime(timeStep.getTime());
                    requestTime +=GrantTime;
					simAgent->GetLBSFederateAmbassador()->timeAdvGrant =false;
                    loopCounter++;
                    RTIAmbassadorInstance::rtiAmb.timeAdvanceRequest(requestTime);
                    cout << "timeAdvanceRequest= " <<convertTime(requestTime)<<endl;
                }
                catch (RTI::Exception& e)
                {
					cerr << " Error: " << &e <<endl;
                }
            }
            else
            {
                try
                {
                    RTIfedTime requestTime (timeStep.getTime());
                    requestTime+=GrantTime;
					simAgent->GetLBSFederateAmbassador()->timeAdvGrant = false;
                    loopCounter = 0;
                    RTIAmbassadorInstance::rtiAmb.nextEventRequest(requestTime);    
                }
                catch (RTI::Exception& e)
                {
					cerr <<"FED_HW: Error: " << &e <<endl;
                }
            }
            while (simAgent->GetLBSFederateAmbassador()->timeAdvGrant != true)
            {
				RTIAmbassadorInstance::rtiAmb.tick(0.01,1.0);
            }
            Sleep(1000);
		}

        if (simAgent->GetLBSFederate()!=nullptr)
        {
            simAgent->GetLBSFederate()->Update(GrantTime);
            delete simAgent;//ErdControl delete from factory
        }   

        try
        {
            RTIAmbassadorInstance::rtiAmb.disableTimeConstrained();
			simAgent->GetLBSFederateAmbassador()->TimeConstrained = false;
        }
        catch (RTI::Exception& e)
        {
			cerr << "FED_HW: Err: " << &e <<endl;                        
        }

        try
        {
            RTIAmbassadorInstance::rtiAmb.disableTimeRegulation();
			simAgent->GetLBSFederateAmbassador()->TimeRegulation = false;
        }
        catch (RTI::Exception& e)
        {
			cerr << "FED_HW: Error:" << &e <<endl;
        }

        try
        {
            cout << "FED_HW: resign federation execution called" << endl;
            RTIAmbassadorInstance::rtiAmb.resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
            cout << "FED_HW: successful resign federation execution called" << endl;
        }
        catch (RTI::Exception& e)
        {
            cerr << "FED_HW: Error: "<< &e <<endl;
            return -1;
        }

		waitForUser();
		RTIAmbassadorInstance::Destroy();
    }
    catch (RTI::ConcurrentAccessAttempted& e)
    {
            cerr <<"FED_HW: Error: Concurrent access to the RTI was attemted. \n"
                     <<"        Exception caught in main()  -PROGRAM EXITING. \n"
                     <<"\n"
                     <<"Note:   Concurrent access will result from invoking\n"
                     <<"        RTIambassadorServices within the scope of\n"
                     <<"        federateAmbassadorService invocations.\n"
                     <<"\n"
                     <<"        e.g. RTI calls provideAttributeValueUpdate() and\n"
                     <<"        within that method you invoke updateAttributeValues\n"
                     <<endl;
            cerr <<&e<<endl;
            return -1;
    }
    catch (RTI::Exception& e) 
    {
            cerr <<"FED_HW: Error: "<<&e<<endl;
            return -1;
    }

    cout << "FED_HW: Exiting " <<"."<<endl;
    return 0;
}

int main(int argc,char* argv[])
{
	char* argvq[3];
	argvq[0] = "Turkey";
	argvq[1] = "ankara";
	argvq[2] = "10";
	argvq[3] = "5";
	return hw_main();
	
	/*
	LoadManager::LoadController^ lc = gcnew LoadManager::LoadController();
		lc->StartMonitoring();
		lc->TraceLoad();
		*/
	
	return 0;
}

double convertTime( const RTI::FedTime& theTime )
{
	RTIfedTime castedTime = (RTIfedTime)theTime;
	return castedTime.getTime();
}