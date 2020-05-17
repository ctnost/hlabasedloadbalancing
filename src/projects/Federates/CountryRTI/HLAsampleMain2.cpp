#include "Country.h"
#include "CountryFederateAmb.h"

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

#include <fedtime.hh>

//时间控制与推进状态变量
RTI::Boolean  timeAdvGrant =RTI::RTI_TRUE;
RTI::Boolean  TimeRegulation = RTI::RTI_FALSE;
RTI::Boolean  TimeConstrained = RTI::RTI_FALSE;

//联邦同步控制状态变量
RTI::Boolean  SyncPointRegisterSucceeded = RTI::RTI_FALSE;
RTI::Boolean  SyncPointRegisterFailed    = RTI::RTI_FALSE;
RTI::Boolean  AnnounceSyncReceived       = RTI::RTI_FALSE;
RTI::Boolean  FederationalsSynchronized  = RTI::RTI_FALSE;

int discoverNum = 0;

std::string SynPoint("zhanghui");
std::string Syntag("tag");


RTI::FedTime &GrantTime = (*(RTI::FedTimeFactory::makeZero()));

double convertTime( const RTI::FedTime& theTime );
RTI::RTIambassador*         RTIAmbassadorInstance::rtiAmb           = NULL;
char* const					RTIAmbassadorInstance::fedExecName		= "HelloWorld";


void printUsage(const char *pExeName)
{
        cout << "usage: "
                 << pExeName
                 << "<Country Name> <Initial Population> [<Number of Ticks>]"
                 << endl<<endl;
}


void waitForUser()
{
	cout << " >>>>>>>>>> Press Enter to Continue <<<<<<<<<<" << endl;
	string line;
	getline( cin, line );
}

int hw_main(int argc,char *argv[])
{
        const char *exeName = argv[0];

        Country *myCountry = NULL;
        int numberofTicks(100);

        int loopCounter = 0;

        if(argc<3)
        {
            printUsage(exeName);
            return -1;
        }else if(!argv[1] && argv[2] &&(argc == 3 || !argv[3]))
        {
            printUsage(exeName);
            return -1;
        }
        else
        {
            myCountry = new Country(argv[1],argv[2]);
            if (argc>3)
            {
                    numberofTicks = atoi(argv[3]);
            }
        }

        try
        {
                RTI::RTIambassador  rtiAmb;
                CountryFederateAmb  fedAmb;
                RTI::FederateHandle federateID;

				rtiAmb = *RTIAmbassadorInstance::GetInstance();

                RTI::Boolean Joined   = RTI::RTI_FALSE;
                int          numTries = 0;
                
                while (!Joined && (numTries ++ <20))
                {
                    try
                    {
							cout << "Fed Name : " << exeName <<endl;
                            federateID = rtiAmb.joinFederationExecution(myCountry->GetName(),RTIAmbassadorInstance::GetFedExecName(),&fedAmb);
                            Joined = RTI::RTI_TRUE;
							fedAmb.SetCountry(myCountry);
                            cout<< argv[1] <<" of federate ID = "<< federateID <<endl;
                    }
                    catch (RTI::FederateAlreadyExecutionMember &e)
                    {
                            cerr <<"error: " << myCountry->GetName()
                                     << " already exists in the Federation Execution "
									 << RTIAmbassadorInstance::GetFedExecName() << "." <<endl;
                            cerr <<&e <<endl;
                            return -1;
                    }
                    catch (RTI::FederationExecutionDoesNotExist&)
                    {
                            cerr << "Error: " <<  RTIAmbassadorInstance::GetFedExecName() << " Federation Execution "
                                     << "does not exists. " << endl;
                            rtiAmb.tick(0.1,2.0);
                    }
                    catch (RTI::Exception &e) 
                    {
                            cerr <<"Error: " << &e <<endl;
                            return -1;
                    }
                }

                cout << "FED_HW: FedName : " << exeName
                         << "  Fedrate Handle = " << federateID << endl;

                myCountry->Init(&rtiAmb);

                try
                {
                        rtiAmb.enableClassRelevanceAdvisorySwitch();
                }
                catch (RTI::Exception& e)
                {
                        cout <<"Error: " <<" enableClassRelevanceAdvisorySwitch "<< &e <<endl;
                }
/*                cout <<"完成开启联邦成员是否存在异地定购恰当性建议！"<<endl<<endl;
*/
                myCountry->PublishAndSubscribe();
                myCountry->Register();
                const RTIfedTime timeStep(1.0);

                timeAdvGrant = RTI::RTI_FALSE;

                try
                {
					/*
					This service instructs the LRC to deliver receive-ordered events to the federate 
					in the absence of an in-progress time-advancement service. 
					Subsequent to invoking this service, receive-ordered events may be delivered to the federate 
					during any invocation of tick(). This setting is only relevant for federates that are time-constrained: 
					all events are always delivered asynchronously to non-time-constrained federates.
					*/
                    cout <<"enableAsynchronousDelivery " <<endl;
                    rtiAmb.enableAsynchronousDelivery();
                }
                catch (RTI::Exception& e)
                {
					cerr << "Error :" << &e <<endl;
                }

                try
                {
					/*
					This service instructs the LRC to constrain the advancement of the 
					federate抯 time based on the federation抯 time, 
					and to deliver time-stamp-ordered events in the correct order. 
					*/
                    cout << " enableTimeConstrained " << endl;
                    rtiAmb.enableTimeConstrained();
                    timeAdvGrant = RTI::RTI_FALSE;
                    TimeConstrained = RTI::RTI_FALSE;
                    while (!TimeConstrained)
                    {
                            rtiAmb.tick(0.01,1.0);
                    }
                }
                catch (RTI::Exception& e)
                {
					cerr <<"Error: "<< &e<<endl;
                }
                //cout<<"完成时间受限策略开启！"<<endl<<endl;

                try
                {
					cout << "lookahead = "<< convertTime( myCountry->GetLookahead()) <<" - "<<endl;

                    rtiAmb.enableTimeRegulation(GrantTime,myCountry->GetLookahead());

                    timeAdvGrant = RTI::RTI_FALSE;
                    TimeRegulation = RTI::RTI_FALSE;
                    while (!TimeRegulation)
                    {
                            rtiAmb.tick(0.01,1.0);
                    }
                }
                catch (RTI::Exception& e)
                {
					cerr <<"Error: " << &e <<endl;
                }
                cout<<"look head finished"<<endl<<endl;

                ////////////////////////////////////////////////////////////////////
                //联邦同步部分
                ////////////////////////////////////////////////////////////////////
                rtiAmb.tick(0.1,0.2);
/*
                //wait until other federates have joined.
                while (discoverNum < FederateNum -1)
                {
                        rtiAmb.tick(0.1,0.2);
                }
                cout<<" wait finished "<<endl<<endl;
*/
                rtiAmb.registerFederationSynchronizationPoint(SynPoint.c_str(),Syntag.c_str());
                rtiAmb.tick(0.1,0.2);

                while (!SyncPointRegisterFailed && !SyncPointRegisterSucceeded)
                {
					rtiAmb.tick(0.1,0.2);
                }
                cout<<" SyncPointRegisterFailed "<<endl<<endl;
                rtiAmb.tick(0.1,0.2);
                while (!AnnounceSyncReceived)
                {
					rtiAmb.tick(0.1,0.2);
                }

                cout<<"AnnounceSyncReceived  "<<endl<<endl;
                
                rtiAmb.tick(0.1,0.2);
                rtiAmb.synchronizationPointAchieved(SynPoint.c_str());
                cout <<" synchronizationPointAchieved "<<endl<<endl;

                while (!FederationalsSynchronized)
                {
					rtiAmb.tick(0.1,0.2);
                }
                cout<<"-----FederationalsSynchronized-----"<<endl<<endl;
                rtiAmb.tick(0.1,0.2);
                
                cout<<"-----------------------------------------------------"<<endl<<"  "<<endl<<endl;
     
                int counter = 0;
                timeAdvGrant = RTI::RTI_TRUE;

                while (counter++ < numberofTicks -1)
                {
                        cout << "HelloWorld Event Loop Iteration # :" << counter <<endl;

                        //--------------------------------------------------------------
                        //  1)更新当前状态
                        //--------------------------------------------------------------
                        
                        myCountry->Update(GrantTime);
                        

                        Country* pCountry = NULL;
                        for (unsigned int i = 0; i < Country::ms_extentCardinality; i++ )
                        {

                                pCountry = Country::ms_countryExtent[i];
                                if (pCountry)
                                {
                                        cout <<"{ country[" << i <<"]=" << pCountry <<"}"<<endl;
                                }
                        }
                        
                        //--------------------------------------------------------------
                        //  2)询问时间推进
                        //--------------------------------------------------------------
                        if (timeAdvGrant)
                        {
                                try
                                {
                                        RTIfedTime requestTime(timeStep.getTime());
                                        requestTime +=GrantTime;
                                        timeAdvGrant =RTI::RTI_FALSE;
                                        loopCounter++;
                                        rtiAmb.timeAdvanceRequest(requestTime);
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
                                        timeAdvGrant = RTI::RTI_FALSE;
                                        loopCounter = 0;
                                        rtiAmb.nextEventRequest(requestTime);
                                        
                                }
                                catch (RTI::Exception& e)
                                {
                                        cerr <<"FED_HW: Error: " << &e <<endl;
                                }
                        }

                        while (timeAdvGrant != RTI::RTI_TRUE)
                        {
                                rtiAmb.tick(0.01,1.0);
                        }
                        Sleep(1000);
                }

                if (myCountry)
                {
                        myCountry->Update(GrantTime);
                        delete myCountry;
                }                        

                
                try
                {
                        rtiAmb.disableTimeConstrained();
                        TimeConstrained = RTI::RTI_FALSE;
                }
                catch (RTI::Exception& e)
                {
                        cerr << "FED_HW: Err: " << &e <<endl;                        
                }

                try
                {
                        rtiAmb.disableTimeRegulation();
                        TimeRegulation = RTI::RTI_FALSE;
                }
                catch (RTI::Exception& e)
                {
                        cerr << "FED_HW: Error:" << &e <<endl;
                }

                try
                {
                        cout << "FED_HW: resign federation execution called" << endl;
                        rtiAmb.resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
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

        cout << "FED_HW: Exiting " << exeName <<"."<<endl;
        return 0;
}

int main(int argc,char* argv[])
{
        //return hw_main(argc,argv);
	char* argvq[3];
	argvq[0] = "Turkey";
	argvq[1] = "ankara";
	argvq[2] = "10";
	argvq[3] = "5";
	//return hw_main(33,argvq);
	
	LoadManager::LoadController^ lc = gcnew LoadManager::LoadController();
	lc->StartMonitoring();
	lc->TraceLoad();
	
	return 0;
}

double convertTime( const RTI::FedTime& theTime )
{
	RTIfedTime castedTime = (RTIfedTime)theTime;
	return castedTime.getTime();
}