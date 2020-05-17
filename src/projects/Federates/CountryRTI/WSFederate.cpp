#include "WSFederate.h"
#include "stdio.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <WinSock2.h>
#include <math.h> 
#include "StringOperation.h"

using std::cout;
using std::cerr;
using std::endl;

RTI::ObjectClassHandle      WSFederate::ms_WSTypeID = 0;
RTI::AttributeHandle        WSFederate::ms_nameTypeID = 0;
RTI::AttributeHandle        WSFederate::ah_posx = 0;
RTI::AttributeHandle        WSFederate::ah_posy = 0;
RTI::AttributeHandle        WSFederate::ah_power = 0;
RTI::AttributeHandle        WSFederate::ah_rc = 0;
RTI::AttributeHandle        WSFederate::ah_sc = 0;
RTI::InteractionClassHandle WSFederate::ich_SendDataTypeID = 0;
RTI::InteractionClassHandle WSFederate::ich_LBSCheck = 0;
RTI::ParameterHandle        WSFederate::ph_posx = 0;
RTI::ParameterHandle        WSFederate::ph_posy = 0;
RTI::ParameterHandle        WSFederate::ph_radius = 0;
RTI::ParameterHandle        WSFederate::ph_LastMessageTag = 0;
RTI::ParameterHandle		WSFederate::ph_Target = 0;
RTI::ParameterHandle		WSFederate::ph_Source = 0;
RTI::ParameterHandle		WSFederate::ph_LastMessageId = 0;

WirelessSensor* WSFederate::ws;


char* const WSFederate::ms_WSTypeStr		= "ObjectRoot.WS";
char* const WSFederate::ms_nameTypeStr      = "Name";
char* const WSFederate::ah_posxTypeStr		= "PosX";
char* const WSFederate::ah_posyTypeStr		= "PosY";
char* const WSFederate::ah_powerTypeStr		= "Power";
char* const WSFederate::ah_rcTypeStr		= "ReceiveCount";
char* const WSFederate::ah_scTypeStr		= "SendCount";
char* const WSFederate::ich_SendDataTypeStr      = "SendData";
char* const WSFederate::ich_LBSCheckStr			 = "LBSCheck";
char* const WSFederate::ph_TargetStr			= "Target";
char* const WSFederate::ph_SourceStr			= "Source";
char* const WSFederate::ph_posxTypeStr   = "PosX";
char* const WSFederate::ph_LastMessageTagStr   = "LastMessageTag";
char* const WSFederate::ph_posyTypeStr   = "PosY";
char* const WSFederate::ph_radiusTypeStr   = "Radius";

const double WSFederate::ms_growthRate(0.01);

unsigned int WSFederate::ms_extentCardinality = 0;


WSFederate::WSFederate(char* p_type)
:LBSDefaultFederate(p_type)
{
    this->SetName(NULL);
	rs = new RSSTATE();
	rs->sendingTag = -1;
	rs->receivedTag = -1;
	dao = gcnew LoadHistory::LoadTracer();
	dao->ConnectToDb();
	//total memory size
	systemTotalMemory = 0;
	System::Management::ManagementObjectSearcher ^searcher
		= gcnew System::Management::ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_LogicalMemoryConfiguration"); 
	for each (System::Management::ManagementObject^ queryObj in searcher->Get())
	{
		systemTotalMemory += Convert::ToDouble(queryObj["TotalPhysicalMemory"]);
	}
}

WSFederate::~WSFederate()
{
}

String^ GetIP()
{
	String^ strHostName = "";
	strHostName = System::Net::Dns::GetHostName();
	System::Net::IPHostEntry^ ipEntry = System::Net::Dns::GetHostEntry(strHostName);
	cli::array<System::Net::IPAddress^>^ addr = ipEntry->AddressList;
	return addr[addr.Length-1]->ToString();
}

void WSFederate::PublishAndSubscribe()
{
		String^ cpath = System::IO::Directory::GetCurrentDirectory();
		Console::WriteLine("The current directory: " + cpath);
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

                        ms_rtiAmb->publishObjectClass(ms_WSTypeID,*attributes);
                        ms_rtiAmb->subscribeObjectClassAttributes(ms_WSTypeID,*attributes);
                }
                catch (RTI::Exception& e)
                {
                        cout <<"¹«²¼¶¨¹º¶ÔÏóÀàÊ§°Ü£º" << &e <<endl;
                        exit(1);
                }        
        
                attributes->empty();
                delete attributes;
        

                try
                {
                        ms_rtiAmb->publishInteractionClass(ich_SendDataTypeID);
                        ms_rtiAmb->subscribeInteractionClass(ich_SendDataTypeID);
						ms_rtiAmb->publishInteractionClass(ich_LBSCheck);
						ms_rtiAmb->subscribeInteractionClass(ich_LBSCheck);
                }
                catch (RTI::Exception& e)
                {
                        cout <<"¹«²¼¶¨¹º½»»¥Àà´íÎó£¡" << &e << endl;
                        exit(1);
                }                

        }
        cout <<" Publish And Subscribe"<<endl<<endl;        
}

void WSFederate::Register()
{
	if (ms_rtiAmb)
	{
		this->m_instanceID = ms_rtiAmb->registerObjectInstance(this->GetWSRtiID(),this->GetName());
		ws = new WirelessSensor(this->m_instanceID);
		System::Random^ rnd = gcnew System::Random();
		ws->posx = rnd->Next(MapWidth);
		ws->posy = rnd->Next(MapHeight);
		ws->radius = MaxRadius;
		ws->power = MaxPower;
		ws->rc = 0;
		ws->sc = 0;
		//TODO:Sil rnd->Next(0,ws->power);
		nextDetectionTime = rnd->Next(1,20);
		ws->nextSendingTime = -1;
		cout <<"FED_HW: Registered m_instanceID =" <<m_instanceID <<" "<<endl;

		LoadHistory::LoadTracer^ dao = gcnew LoadHistory::LoadTracer();
		dao->ConnectToDb();
		String^ fedName = gcnew String(this->GetName());
		dao->InsertFederate(fedName,GetIP());
	}                
}

void WSFederate::SetInteractionControl(RTI::Boolean status, RTI::InteractionClassHandle theClass)
{
/*
        if ( theClass == WSFederate::GetCommRtiID())
        {
             //   ms_sendCommInteractions = status;
//                const char *pStr = ms_sendCommInteractions ? "ON":"OFF";
            //    cout<<"FED_HW: Í¨ÐÅ½»»¥×´Ì¬£¨turnInteractionsOn£©£º"<< pStr << "."<<endl;
        } else
        {
                const char *pStr = status ? "Start":"Stop";
                cerr<<pStr <<" interaction for unknown class: "<<theClass <<endl;
        }*/


}

void WSFederate::SetUpdateControl(RTI::Boolean status, const RTI::AttributeHandleSet& attrs)
{
}

void WSFederate::UpdateFederateOnDB()
{
	double fedLife = (ws->power * 100)/MaxPower;
	String^ fedName = gcnew String(this->GetName());
	String^ myIp = GetIP();
	dao->UpdateFederateLife(fedName,myIp,fedLife);
	if(fedLife < 30)
		dao->UpdateFederateMigrateStatus(fedName,myIp,false);

	double cpu_usage=0;
	double proc_time = System::Diagnostics::Process::GetCurrentProcess()->TotalProcessorTime.Ticks;
	double sys_time = DateTime::Now.Ticks;
	cpu_usage=(proc_time-proc_timeLast)/(sys_time-sys_timeLast);
	proc_timeLast=proc_time;
	sys_timeLast= sys_time;
	double valCpuUsed = (cpu_usage*100) ;// /nofCore
	double valMemUsed = System::Diagnostics::Process::GetCurrentProcess()->PrivateMemorySize/1024;
	valMemUsed = (valMemUsed*100/systemTotalMemory);
	dao->UpdateFederateUsages(fedName,myIp,valCpuUsed,valMemUsed);
}

void WSFederate::updateAttributeValues()
{
	//cout << "LocalTime: " << localTime << "NEW Power: " << ws->power << " NextDetecTime: " << (int)nextDetectionTime <<endl;
	ws->power -= 1;
	UpdateFederateOnDB();
	RTI::AttributeHandleValuePairSet *pNvpSet = CreateAttributeSet();
	if((int)nextDetectionTime == (int)localTime || (int)ws->nextSendingTime == (int)localTime)
	{
		System::Random^ rnd = gcnew System::Random();
		int m_max = MaxPower;
		//TODO:Sil rnd->Next((int)localTime,m_max);
		nextDetectionTime = rnd->Next((int)localTime+1,localTime+5);//rnd->Next((int)localTime+1,m_max);
		//cout << " NEXTDETECTIONTIMEFOUND " << (int)nextDetectionTime <<endl;
		if((int)ws->nextSendingTime == (int)localTime)
			SendDetectedData(false);
		else
			SendDetectedData(true);
	}
	ms_rtiAmb->updateAttributeValues( this->GetInstanceID(), *pNvpSet, "ws_update" );

	pNvpSet->empty();
	delete pNvpSet;
}

void WSFederate::Close()
{
	String^ fedName = gcnew String(this->GetName());
	dao->RemoveFederate(fedName,GetIP());
	if(this->GetFederateState() == MIGRATED){
		dao->UpdateMigrationTable(0);
	}
}

void WSFederate::SendDetectedData(bool newData)
{
	int transCost = TCost;
	ws->power -= transCost;
	RTI::ParameterHandleValuePairSet* pParams = NULL;
	pParams = RTI::ParameterSetFactory::create(6);
	string posxstr = itostring(ws->posx);
	pParams->add(ph_posx, (posxstr.c_str()), (RTI::ULong)posxstr.length() + 1); 
	string posystr = itostring(ws->posy);
	pParams->add(ph_posy, (posystr.c_str()), (RTI::ULong)posystr.length() + 1); 
	string radiusstr  = itostring(ws->radius);
	pParams->add(ph_radius, (radiusstr.c_str()), (RTI::ULong)radiusstr.length() + 1); 
	string fedNameStr = this->GetName();
	pParams->add(ph_Source, (fedNameStr.c_str()), (RTI::ULong)fedNameStr.length() + 1);
	string targetStr = "all";
	pParams->add(ph_Target, (targetStr.c_str()), (RTI::ULong)targetStr.length() + 1);
	string lmIdstr = itostring(lastInteractionNumber);
	pParams->add(ph_LastMessageId, (lmIdstr.c_str()), (RTI::ULong)lmIdstr.length() + 1); 
	try
	{
		System::Random^ rnd = gcnew System::Random();
		int randomNxt = rnd->Next(99999);
		string randomNxtString = StringOperation::toStandardString(randomNxt.ToString());
		if(newData)
			rs->sendingTag = atoi(randomNxtString.c_str());
		else
			rs->sendingTag = rs->receivedTag;

		char buffer [5];
		string sendStrTag = StringOperation::toStandardString(rs->sendingTag.ToString());
		//cout << " ********INTERACTION SENDING, Tag: " << sendStrTag << endl;
		ws->sc = ws->sc+1;

		LBSInteractionItem newItem;
		newItem.theInteraction = ich_SendDataTypeID;
		newItem.theParameters = RTI::ParameterSetFactory::create(pParams->size());

		RTI::ParameterHandle paramHandle;
		RTI::ULong           valueLength;
		for (unsigned int i  = 0; i < pParams->size(); i++)
		{
			paramHandle = pParams->getHandle( i );
			valueLength = pParams->getValueLength(i);
			char *valueCh = pParams->getValuePointer(i, valueLength);
			newItem.theParameters->add(paramHandle,valueCh,valueLength);
		}
		newItem.theTag = Convert::ToInt32(StringOperation::toSystemString(sendStrTag));
		newItem.theTime = localTime;
		newItem.type = 1; 
		if(this->GetFederateState() == DEFAULT){
			(void)ms_rtiAmb->sendInteraction(ich_SendDataTypeID,*pParams,sendStrTag.c_str());
			lastInteractionNumber++;
			inStack->push_back(newItem);
			if(inStack->size() > 10)
				inStack->pop_front();
		}
		else{	
			this->inQueue->push_back(newItem);
		}
	}
	catch (RTI::Exception& e)
	{
		cerr << "FED_HW: Error:" << &e <<endl;
	}
	delete pParams;
}

int WSFederate::CheckFederateState()
{
	if (ws->power < 1)
		return 1;
	else return 0;
}

void WSFederate::Update(RTI::FedTime& newTime)
{
	localTime = convertTime(newTime);
}

void WSFederate::Update(const RTI::AttributeHandleValuePairSet& theAttributes)
{

}

void WSFederate::Update(RTI::InteractionClassHandle theInteraction,const RTI::ParameterHandleValuePairSet& theParameters)
{
}

int WSFederate::GetLastMessageFromParameters(const RTI::ParameterHandleValuePairSet& theParameters)
{
	RTI::ParameterHandle paramHandle;
	RTI::ULong           valueLength;

	for (unsigned int i  = 0; i < theParameters.size(); i++)
	{
		paramHandle = theParameters.getHandle( i );
		valueLength = theParameters.getValueLength(i);
		char *valueCh = theParameters.getValuePointer(i, valueLength);
		if(paramHandle == WSFederate::ph_LastMessageId){
			int lastMessId = atoi(valueCh);
			return lastMessId;
		}
	}
	return -1;
}

void WSFederate::ReceiveData(RTI::InteractionClassHandle theInteraction,const RTI::ParameterHandleValuePairSet& theParameters,const char *theTag)
{
	int orginalTagId = atoi(theTag);
	//Eger gonderilen tag ile alýnan tag ayný ise islem yapma.
	if (rs->sendingTag==orginalTagId)
		return;

	rs->receivedTag = -1;
	if (theInteraction == ich_SendDataTypeID)
	{
		RTI::ParameterHandle paramHandle;
		RTI::ULong           valueLength;
		int posx,posy,radius;

		for (unsigned int i  = 0; i < theParameters.size(); i++)
		{
			paramHandle = theParameters.getHandle( i );
			valueLength = theParameters.getValueLength(i);
			char *valueCh = theParameters.getValuePointer(i, valueLength);
			if(paramHandle == WSFederate::ph_Target){
				if( strcmp(valueCh,"all") != 0){
					if( strcmp(this->GetName(),valueCh) != 0)
						return;	//Bir target var ise ve bu ben degil isem islem yapma.
				}
			}
			else if(paramHandle == WSFederate::ph_Source){
				//cout << " - Source* - " << valueCh << endl;
			}
			else{
				int val = atoi(valueCh);
				if (paramHandle == WSFederate::ph_posx) 
					posx = val;
				else if(paramHandle == WSFederate::ph_posy)
					posy = val;
				else if(paramHandle == WSFederate::ph_radius)
					radius = val;
			}
		}
		double radSum = ws->radius + radius;
		double distance = abs(sqrt(pow((double)(ws->posx - posx),2.0) + pow((double)(ws->posy - posy),2.0)));
		if (distance < radSum)
		{
			int receiveCost = RCost;
			ws->power -= receiveCost;
			ws->nextSendingTime = localTime+2;
			rs->receivedTag = orginalTagId;
			//cout << " INTERACTION RECEIVING *************  theTag: " << orginalTagId << endl;
			ws->rc = ws->rc+1;
		}
	}
	else if(theInteraction == ich_LBSCheck){
		RTI::ParameterHandle paramHandle = theParameters.getHandle( 0 );
		RTI::ULong valueLength = theParameters.getValueLength(0);
		char *valueCh = theParameters.getValuePointer(0, valueLength);
		String^ LBSCheckVal = StringOperation::toSystemString(valueCh);
		Console::WriteLine("***LBSCheckInteraction*** " + LBSCheckVal);
		cli::array<String^>^ arLBS = LBSCheckVal->Split('|');
		int lastMessageId = Convert::ToInt32(arLBS[0]);
		String^ fedName = arLBS[1]; 
		//Eger alinan LBSCheck mesajýnda benim son gonderdigim ise islem yapma.
		if(lastMessageId == lastInteractionNumber)
			return;	
		else{
			//Benim son gonderdigim mesaj id si degil ise bu mesajý gonderene tekrar gonder.
			int index = 0;
			list<LBSInteractionItem>::iterator it;
			for (it = inStack->begin(); it!= inStack->end(); it++)
			{
				LBSInteractionItem lbsIt = *it; 
				int itId = GetLastMessageFromParameters(*lbsIt.theParameters);
				if(itId <= lastMessageId)
					continue;
				if(lbsIt.type == 1){ 
					//ToDo yalnýzca isteyene göndermesi gerekir.
					string fedNames = StringOperation::toStandardString(fedName);
					lbsIt.theParameters->add(ph_Target, (fedNames.c_str()), (RTI::ULong)fedNames.length() + 1); 
					string itIdStr = StringOperation::toStandardString(itId.ToString());
					lbsIt.theParameters->add(ph_LastMessageId, (itIdStr.c_str()), (RTI::ULong)itIdStr.length() + 1); 
					string aalbstittag = StringOperation::toStandardString(lbsIt.theTag.ToString());
					(void)ms_rtiAmb->sendInteraction(ich_SendDataTypeID,*lbsIt.theParameters,aalbstittag.c_str());
				}
			}
		}

	}	
	else
	{
		cerr << "FED_HW: Received an interaction class I don't know about." << endl;
	}
}

void WSFederate::Init(RTI::RTIambassador* rtiAmb)
{
	LBSDefaultFederate::Init(rtiAmb);
	if (ms_rtiAmb)
	{
		try
		{
			ms_WSTypeID	= ms_rtiAmb->getObjectClassHandle(ms_WSTypeStr);
			ms_nameTypeID	= ms_rtiAmb->getAttributeHandle(ms_nameTypeStr,ms_WSTypeID);
			ah_posx			= ms_rtiAmb->getAttributeHandle(ah_posxTypeStr,ms_WSTypeID);
			ah_posy			= ms_rtiAmb->getAttributeHandle(ah_posyTypeStr,ms_WSTypeID);
			ah_power		= ms_rtiAmb->getAttributeHandle(ah_powerTypeStr,ms_WSTypeID);
			ah_rc			= ms_rtiAmb->getAttributeHandle(ah_rcTypeStr,ms_WSTypeID);
			ah_sc			= ms_rtiAmb->getAttributeHandle(ah_scTypeStr,ms_WSTypeID);
		}
		catch (RTI::Exception& e)
		{
			cout << "Attribute Handle Hata!!!" << &e <<endl;
			exit(1);                        
		}
		cout <<"Init:"<<endl<<endl;

		try
		{
			ich_SendDataTypeID    = ms_rtiAmb->getInteractionClassHandle(ich_SendDataTypeStr);
			ph_posx = ms_rtiAmb->getParameterHandle(ph_posxTypeStr,ich_SendDataTypeID);
			ph_posy = ms_rtiAmb->getParameterHandle(ph_posyTypeStr,ich_SendDataTypeID);
			ph_radius = ms_rtiAmb->getParameterHandle(ph_radiusTypeStr,ich_SendDataTypeID);
			ph_Target = ms_rtiAmb->getParameterHandle(ph_TargetStr,ich_SendDataTypeID);
			ph_Source = ms_rtiAmb->getParameterHandle(ph_SourceStr,ich_SendDataTypeID);
			ph_LastMessageId = ms_rtiAmb->getParameterHandle("LastMesId",ich_SendDataTypeID);
			ich_LBSCheck  = ms_rtiAmb->getInteractionClassHandle(ich_LBSCheckStr); 
			ph_LastMessageTag = ms_rtiAmb->getParameterHandle(ph_LastMessageTagStr,ich_LBSCheck);
		}
		catch (RTI::Exception& e)
		{
			cout <<"Interaction Hata!!!"<< &e << endl;
			exit(1);
		}
		cout<<"Init Finished"<<endl<<endl;
	}
}

double WSFederate::Expntl(double x)
{
	double z;
	do
	{
		z = ((double) rand() / RAND_MAX);
	}
	while ((z == 0) || (z == 1));

	return(-x * log(z));
}

RTI::AttributeHandleValuePairSet* WSFederate::CreateAttributeSet()
{
	RTI::AttributeHandleValuePairSet* attributes = NULL;

	if (ms_rtiAmb)
	{
		attributes = RTI::AttributeSetFactory::create(3);
		string posxstr = itostring(ws->posx);
		attributes->add(ah_posx, (posxstr.c_str()), (RTI::ULong)posxstr.length() + 1); 
		string posystr = itostring(ws->posy);
		attributes->add(ah_posy, (posystr.c_str()), (RTI::ULong)posystr.length() + 1); 
		string powerstr  = itostring(ws->power);
		attributes->add(ah_power, (powerstr.c_str()), (RTI::ULong)powerstr.length() + 1); 
		string rcstr = itostring(ws->rc);
		attributes->add(ah_rc, (rcstr.c_str()), (RTI::ULong)rcstr.length() + 1); 
		string scstr = itostring(ws->sc);
		attributes->add(ah_sc, (scstr.c_str()), (RTI::ULong)scstr.length() + 1); 
		//cout << "RC::> " << rcstr << " SC::> " << scstr <<endl;
	}
	return attributes;
}

std::string WSFederate::itostring(int i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}

void WSFederate::FederateSave(OArchive& arch) const
{
	std::string writeBuf = ws->name;
	arch << writeBuf;
	arch << nextDetectionTime;
	arch << nextSendingTime;
	arch << localTime;
	arch << ws->posx;
	arch << ws->posy;
	arch << ws->radius;
	arch << ws->power;
	arch << ws->nextSendingTime;
	arch << ws->objectID;
	arch << ws->rc;
	arch << ws->sc;
}

void WSFederate::FederateRestore(InArchive& arch)
{
	ws = new WirelessSensor(this->m_instanceID);
	string writeBuf;
	arch >> writeBuf;
	ws->name = writeBuf;
	arch >> nextDetectionTime;
	arch >> nextSendingTime;
	arch >> localTime;
	arch >> ws->posx;
	arch >> ws->posy;
	arch >> ws->radius;
	arch >> ws->power;
	arch >> ws->nextSendingTime;
	arch >> ws->objectID;
	arch >> ws->rc;
	arch >> ws->sc;
}

void WSFederate::RequestInteractions(OArchive& arch) const 
{
	int inQueueS = inQueue->size();
	arch << inQueueS;
	list<LBSInteractionItem>::iterator it;
	RTI::InteractionClassHandle handle;
	double thetime;
	int thetag;
	int type,paramsize;
	RTI::ParameterHandle paramHandle;
	RTI::ULong           valueLength;

	for (it = inQueue->begin(); it!= inQueue->end(); it++)
	{
		LBSInteractionItem lbsIt = *it; 
		handle = lbsIt.theInteraction;				arch << handle;
		thetime = lbsIt.theTime;					arch << thetime;
		thetag =lbsIt.theTag;						arch << thetag;
		type = lbsIt.type;							arch << type;
		paramsize = lbsIt.theParameters->size();	arch << paramsize;
		
		for (unsigned int i  = 0; i < paramsize; i++){
			paramHandle = lbsIt.theParameters->getHandle( i );		
			valueLength = lbsIt.theParameters->getValueLength(i);
			char *valueCh = lbsIt.theParameters->getValuePointer(i, valueLength);
			string valueStr = valueCh;
			cout << "OZMN Bunlarda bozulsunn" << valueStr <<endl;
			arch << paramHandle;
			arch << valueLength;
			arch << valueStr;
		}
	}
	inQueue->clear();
}

void WSFederate::RestoreInteractions(InArchive& arch)
{
	int inQueueS = 0;
	arch >> inQueueS;
	Console::WriteLine("RestoreInteractionsBasladi....");
	//list<LBSInteractionItem> *inQueueTemp = new list<LBSInteractionItem>();
	RTI::InteractionClassHandle handle;
	double thetime;
	int thetag;
	int type,paramsize;
	RTI::ParameterHandle paramHandle;
	RTI::ULong           valueLength;
	std::map<string,LBSInteractionItem> lbsCheckMap;
	std::map<string,LBSInteractionItem>::iterator lbsCheckMapIt;
	for (int i = 0; i < inQueueS; i++){
		LBSInteractionItem lbsIt;
		arch >> handle;		lbsIt.theInteraction = handle;
		arch >> thetime;	lbsIt.theTime = thetime;
		arch >> thetag;		lbsIt.theTag = thetag;
		arch >> type;		lbsIt.type = type;
		arch >> paramsize;	
		lbsIt.theParameters = RTI::ParameterSetFactory::create(paramsize);
		string sourceFederate="";
		int lastMessageIdPar=-1;
		cout<<"RestoreInteractionsBasladi...." << i << "-T-:" << type << " TAG "  << thetag << endl;
		for (unsigned int j  = 0; j < paramsize; j++)
		{
			string valueStr;
			arch >> paramHandle;
			arch >> valueLength;
			arch >> valueStr;
			if(paramHandle == WSFederate::ph_Source)
				sourceFederate = valueStr;
			if(paramHandle == WSFederate::ph_LastMessageId)
				lastMessageIdPar = atoi(valueStr.c_str());
			
			lbsIt.theParameters->add(paramHandle,valueStr.c_str(),valueLength);
		}

		inQueue->push_back(lbsIt);
		if( strcmp(sourceFederate.c_str(),"") != 0 && type == 0)
		{
			cout << " Bu SourceFedereye Gondericek : " << sourceFederate << endl;
			lbsCheckMapIt = lbsCheckMap.find(sourceFederate);
			if(lbsCheckMapIt == lbsCheckMap.end()){
				lbsIt.temp = lastMessageIdPar;
				lbsCheckMap.insert(make_pair<string,LBSInteractionItem>(sourceFederate,lbsIt));
			}else{
					int lastMessageIdTemp1 = lbsCheckMapIt->second.temp;
					int lastMessageIdTemp2 = lbsIt.temp;
					if(lastMessageIdTemp1 < lastMessageIdTemp2)
							lbsCheckMapIt->second = lbsIt;
				}
			}
		
		//inQueueTemp->push_back(lbsIt);
	}

	for (lbsCheckMapIt = lbsCheckMap.begin(); lbsCheckMapIt != lbsCheckMap.end(); lbsCheckMapIt++)
	{
		Console::WriteLine("LBSCheck algilandi..");
		RTI::ParameterHandleValuePairSet* pParams = NULL;
		pParams = RTI::ParameterSetFactory::create(1);

		string messageStr = StringOperation::toStandardString(lbsCheckMapIt->second.temp.ToString()) +"|" + lbsCheckMapIt->first;
		pParams->add(ph_LastMessageTag, (messageStr.c_str()), (RTI::ULong)messageStr.length() + 1); 
		(void)ms_rtiAmb->sendInteraction(ich_LBSCheck,*pParams,"");
	}
	
}

void WSFederate::ProcessInteractions()
{
	//Sort by Time
	inQueue->sort(SortInteractionsByTime());
	//
	list<LBSInteractionItem>::iterator it;
	for (it = inQueue->begin(); it!= inQueue->end(); it++)
	{
		LBSInteractionItem lbsIt = *it; 
		string aalbstittag = StringOperation::toStandardString(lbsIt.theTag.ToString());
		if(lbsIt.type == 0){ //ReceiveData
			this->ReceiveData(lbsIt.theInteraction,*lbsIt.theParameters,aalbstittag.c_str());
		}
		else{
			(void)ms_rtiAmb->sendInteraction(ich_SendDataTypeID,*lbsIt.theParameters,aalbstittag.c_str());
			lastInteractionNumber++;
		}
	}
}