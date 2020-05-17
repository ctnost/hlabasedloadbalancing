#include "WSN.h"
#include "stdio.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <WinSock2.h>

using std::cout;
using std::cerr;
using std::endl;

RTI::ObjectClassHandle      WSN::ms_WSNTypeID    = 0;
RTI::AttributeHandle        WSN::ms_nameTypeID       = 0;
RTI::AttributeHandle        WSN::ah_posx = 0;
RTI::AttributeHandle        WSN::ah_posy = 0;
RTI::AttributeHandle        WSN::ah_power = 0;
RTI::AttributeHandle        WSN::ah_rc = 0;
RTI::AttributeHandle        WSN::ah_sc = 0;
RTI::InteractionClassHandle WSN::ms_commTypeID       = 0;
RTI::ParameterHandle        WSN::ms_commMsgTypeID    = 0;

map<RTI::ObjectHandle,WirelessSensor*> WSN::wSensors;


char* const WSN::ms_WSNTypeStr    = "ObjectRoot.WS";
char* const WSN::ms_nameTypeStr       = "Name";
char* const WSN::ah_posxTypeStr = "PosX";
char* const WSN::ah_posyTypeStr = "PosY";
char* const WSN::ah_powerTypeStr = "Power";
char* const WSN::ah_rcTypeStr = "ReceiveCount";
char* const WSN::ah_scTypeStr = "SendCount";
char* const WSN::ms_commTypeStr       = "Communication";
char* const WSN::ms_commMsgTypeStr    = "Message";

const double WSN::ms_growthRate(0.01);

unsigned int WSN::ms_extentCardinality = 0;


WSN::WSN(char* p_type)
        :LBSDefaultFederate(p_type)
{
        this->SetName(NULL);
}

WSN::~WSN()
{
}

void WSN::PublishAndSubscribe()
{
        if (ms_rtiAmb)
        {

			RTI::AttributeHandleSet *attributes;
			try
			{

				attributes = RTI::AttributeHandleSetFactory::create(3);
				//countryAttribute->add(ms_nameTypeID);
				attributes->add(ah_posx);
				attributes->add(ah_posy);
				attributes->add(ah_power);
				attributes->add(ah_rc);
				attributes->add(ah_sc);

				ms_rtiAmb->publishObjectClass(ms_WSNTypeID,*attributes);
				ms_rtiAmb->subscribeObjectClassAttributes(ms_WSNTypeID,*attributes);
			}
			catch (RTI::Exception& e)
			{
				cout <<"公布定购对象类失败：" << &e <<endl;
				exit(1);
			}        

			attributes->empty();
			delete attributes;

        

                try
                {
                        ms_rtiAmb->publishInteractionClass(ms_commTypeID);
                        ms_rtiAmb->subscribeInteractionClass(ms_commTypeID);
                }
                catch (RTI::Exception& e)
                {
                        cout <<"公布定购交互类错误！" << &e << endl;
                        exit(1);
                }                

        }
        cout <<" Publish And Subscribe"<<endl<<endl;        
}

void WSN::Register()
{
        if (ms_rtiAmb)
        {
              //  this->m_instanceID = ms_rtiAmb->registerObjectInstance(this->GetWSNRtiID(),this->GetName());
            //    cout <<"FED_HW: Registered m_instanceID =" <<m_instanceID <<" "<<endl;
        }                
}

void WSN::SetInteractionControl(RTI::Boolean status, RTI::InteractionClassHandle theClass)
{
        if ( theClass == WSN::GetCommRtiID())
        {
             //   ms_sendCommInteractions = status;
//                const char *pStr = ms_sendCommInteractions ? "ON":"OFF";
            //    cout<<"FED_HW: 通信交互状态（turnInteractionsOn）："<< pStr << "."<<endl;
        } else
        {
                const char *pStr = status ? "Start":"Stop";
                cerr<<pStr <<" interaction for unknown class: "<<theClass <<endl;
        }

}

void WSN::SetUpdateControl(RTI::Boolean status, const RTI::AttributeHandleSet& attrs)
{
}

void WSN::updateAttributeValues()
{
/*
	WirelessSensor* ws1 = new WirelessSensor(3,6,5,100,1);
	WirelessSensor* ws2 = new WirelessSensor(40,30,10,100,2);
	WirelessSensor* ws3 = new WirelessSensor(90,50,30,100,3);
	Insert(1,ws1);Insert(2,ws2);Insert(3,ws3);*/

}

int WSN::CheckFederateState()
{
	return 0;
}

void WSN::Update(RTI::FedTime& newTime)
{
	
}

void WSN::Update(const RTI::AttributeHandleValuePairSet& theAttributes, RTI::ObjectHandle objectID)
{
	WirelessSensor* ws = Find(objectID);
	RTI::AttributeHandle attrHandle;
	RTI::ULong           valueLength;

	for(unsigned int i = 0; i < theAttributes.size(); i++)
	{
		RTI::ULong length = theAttributes.getValueLength(i);
		char *valueCh = theAttributes.getValuePointer(i, length);
		int val = atoi(valueCh);
		attrHandle = theAttributes.getHandle(i);

		if (attrHandle == WSN::ah_posx) 
			ws->posx = val;
		else if(attrHandle == WSN::ah_posy)
			ws->posy = val;
		else if(attrHandle == WSN::ah_power)
			ws->power = val;
		else if(attrHandle == WSN::ah_rc)
			ws->rc = val;
		else if(attrHandle == WSN::ah_sc)
			ws->sc = val;
	}
}

void WSN::Update(const RTI::AttributeHandleValuePairSet& theAttributes)
{

}

void WSN::Update(RTI::InteractionClassHandle theInteraction,const RTI::ParameterHandleValuePairSet& theParameters)
{
}

System::String^ WSN::GetIP()
{
	System::String^ strHostName = "";
	strHostName = System::Net::Dns::GetHostName();
	System::Net::IPHostEntry^ ipEntry = System::Net::Dns::GetHostEntry(strHostName);
	cli::array<System::Net::IPAddress^>^ addr = ipEntry->AddressList;
	return addr[addr.Length-1]->ToString();
}

void WSN::Init(RTI::RTIambassador* rtiAmb)
{
	System::String^ fedName = gcnew System::String(this->GetName());
	System::String^ myIp = GetIP();
	LoadHistory::LoadTracer^ dao = gcnew LoadHistory::LoadTracer();
	dao->ConnectToDb();
	dao->UpdateFederateMigrateStatus(fedName,myIp,false);

	LBSDefaultFederate::Init(rtiAmb);
	if (ms_rtiAmb)
	{
		try
		{
			ms_WSNTypeID	= ms_rtiAmb->getObjectClassHandle(ms_WSNTypeStr);
			//ms_nameTypeID	= ms_rtiAmb->getAttributeHandle(ms_nameTypeStr,ms_WSNTypeID);
			ah_posx			= ms_rtiAmb->getAttributeHandle(ah_posxTypeStr,ms_WSNTypeID);
			ah_posy			= ms_rtiAmb->getAttributeHandle(ah_posyTypeStr,ms_WSNTypeID);
			ah_power		= ms_rtiAmb->getAttributeHandle(ah_powerTypeStr,ms_WSNTypeID);
			ah_rc			= ms_rtiAmb->getAttributeHandle(ah_rcTypeStr,ms_WSNTypeID);
			ah_sc			= ms_rtiAmb->getAttributeHandle(ah_scTypeStr,ms_WSNTypeID);
		}
		catch (RTI::Exception& e)
		{
			cout << "Attribute Handle Hata!!!" << &e <<endl;
			exit(1);                        
		}
		cout <<"Init:"<<endl<<endl;

		try
		{
			ms_commTypeID    = ms_rtiAmb->getInteractionClassHandle(ms_commTypeStr);
			ms_commMsgTypeID = ms_rtiAmb->getParameterHandle(ms_commMsgTypeStr,ms_commTypeID);
		}
		catch (RTI::Exception& e)
		{
			cout <<"Interaction Hata!!!"<< &e << endl;
			exit(1);
		}
		cout<<"Init Finished"<<endl<<endl;
	}
}


WirelessSensor*  WSN::Find(RTI::ObjectHandle objectID)
{
	return wSensors[objectID];
}

void WSN::Insert(RTI::ObjectHandle objectID, WirelessSensor* ws)
{
	wSensors.insert(make_pair<RTI::ObjectHandle,WirelessSensor*>(objectID,ws));
}

void WSN::Remove(RTI::ObjectHandle objectID)
{
	wSensors.erase(objectID);
}
