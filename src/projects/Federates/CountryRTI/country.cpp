//文件country.cpp
#include "country.h"
#include "stdio.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <WinSock2.h>

using std::cout;
using std::cerr;
using std::endl;

//RTI::RTIambassador*         Country::ms_rtiAmb           = NULL;
RTI::ObjectClassHandle      Country::ms_countryTypeID    = 0;
RTI::AttributeHandle        Country::ms_nameTypeID       = 0;
RTI::AttributeHandle        Country::ms_populationTypeID = 0;
RTI::InteractionClassHandle Country::ms_commTypeID       = 0;
RTI::ParameterHandle        Country::ms_commMsgTypeID    = 0;

//RTIfedTime Country::ms_lookahead(0.5);

RTI::Boolean Country::ms_sendCommInteractions = RTI::RTI_TRUE;
RTI::Boolean Country::ms_doRegistry           = RTI::RTI_TRUE;

char* const Country::ms_countryTypeStr    = "ObjectRoot.Country";
char* const Country::ms_nameTypeStr       = "Name";
char* const Country::ms_populationTypeStr = "Population";
char* const Country::ms_commTypeStr       = "Communication";
char* const Country::ms_commMsgTypeStr    = "Message";

const double Country::ms_growthRate(0.01);

CountryPtr Country::ms_countryExtent[FederateNum]=
{ NULL,NULL,NULL};

unsigned int Country::ms_extentCardinality = 0;

const double countryDefaultPopulation = 100;


Country::Country(char* p_type)
        :LBSDefaultFederate(p_type)
{
        Country::ms_countryExtent[Country::ms_extentCardinality++]= this;
        this->SetName(NULL);
        this->SetPopulation(countryDefaultPopulation);
}

Country::Country(const char* name, const char* populationStr, char* p_type)
        :LBSDefaultFederate(p_type),
		 m_sendNameAttrUpdates(RTI::RTI_FALSE),
         m_sendPopulationAttrUpdates(RTI::RTI_FALSE)
{
        Country::ms_countryExtent[Country::ms_extentCardinality++] = this;
        this->SetName(name);
        if (populationStr &&strlen(populationStr)>0)
        {
                this->SetPopulation(atof(populationStr));
        } 
        else
        {
                this->SetPopulation(countryDefaultPopulation);
        }
}

Country::Country(const char* name, const double& population, char* p_type)
        :LBSDefaultFederate(p_type),
         m_sendNameAttrUpdates(RTI::RTI_FALSE),
         m_sendPopulationAttrUpdates(RTI::RTI_FALSE)
{
        Country::ms_countryExtent[Country::ms_extentCardinality++] = this;
        this->SetName(name);
        this->SetPopulation(population);
}

Country::Country(RTI::ObjectHandle id, char* p_type)
        :LBSDefaultFederate(id,p_type),
         m_sendNameAttrUpdates(RTI::RTI_FALSE),
         m_sendPopulationAttrUpdates(RTI::RTI_FALSE)
{
        Country::ms_countryExtent[Country::ms_extentCardinality++] = this;
        this->SetName(NULL);
        this->SetPopulation(countryDefaultPopulation);
}

Country::~Country()
{
        Country *pCountry = NULL;
        unsigned int ndx = 0;

        for(ndx = 0;ndx<Country::ms_extentCardinality;ndx++)
        {
                pCountry = Country::ms_countryExtent[ndx];

                if (pCountry && pCountry->GetInstanceID() == this->GetInstanceID())
                {
                        break;
                }
        }

        if (pCountry)
        {
                for (unsigned int i=ndx;(i<Country::ms_extentCardinality) && (Country::ms_countryExtent[i]!=NULL); i++)
                {
                        Country::ms_countryExtent[i] = Country::ms_countryExtent[i+1];
                }
                Country::ms_extentCardinality = Country::ms_extentCardinality - 1;
                if (ms_rtiAmb && ndx == 0)
                {
                        (void)ms_rtiAmb->deleteObjectInstance(this->GetInstanceID(),this->GetLastTimePlusLookahead(),NULL);
                } else
                {
                }
        }
        delete [] m_name;
}

Country* Country::Find(RTI::ObjectHandle objectID)
{
        Country *pCountry = NULL;

        for (unsigned int i = 0; i < Country::ms_extentCardinality; i++)
        {
                pCountry = Country::ms_countryExtent[i];
                if (pCountry && pCountry->GetInstanceID() == objectID)
                {
                        break;
                } 
                else
                {
                        pCountry = NULL;
                }
        }
        return pCountry;
}


void Country::FederateSave(OArchive& arch) const 
{
	std::string writeBuf = this->m_name;
	arch << writeBuf;
	arch << this->m_population;
}
void Country::FederateRestore(InArchive& arch)
{
	string readBuf;
	
	arch >> readBuf;
	strcpy(this->m_name, readBuf.c_str());
	arch >> this->m_population;
}
void Country::RequestInteractions(OArchive& arch) const 
{
	/*
	arch.Write(inQueue->size());
	//LBSInteractionQueue::iterator* it;
	vector<LBSInteractionItem*>::iterator it;
	for (it = inQueue->begin(); it!= inQueue->end(); it++)
	{
		LBSInteractionItem* lbsIt = *it; 
		arch.Write(lbsIt->theInteraction);
		arch.Write(convertTime(lbsIt->theTime));
		arch.WriteStr(lbsIt->theTag);
		RTI::ParameterHandle paramHandle;
		RTI::ULong           valueLength;
		for (unsigned int i  = 0; i < lbsIt->theParameters.size(); i++)
		{
			paramHandle = lbsIt->theParameters.getHandle( i );
			if (paramHandle == Country::GetMessageRtiID())
			{
				char msg[1024];
				lbsIt->theParameters.getValue(i,(char*)msg,valueLength);
				arch.WriteStr(msg);
			}
		}

	}
	*/
}
void Country::PublishAndSubscribe()
{
        if (ms_rtiAmb)
        {
                RTI::AttributeHandleSet *countryAttribute;
                try
                {
                        
                        countryAttribute = RTI::AttributeHandleSetFactory::create(2);
                        countryAttribute->add(ms_nameTypeID);
                        countryAttribute->add(ms_populationTypeID);


                        ms_rtiAmb->publishObjectClass(ms_countryTypeID,*countryAttribute);
                        ms_rtiAmb->subscribeObjectClassAttributes(ms_countryTypeID,*countryAttribute);
                }
                catch (RTI::Exception& e)
                {
                        cout <<"公布定购对象类失败：" << &e <<endl;
                        exit(1);
                }        
        
                countryAttribute->empty();
                delete countryAttribute;
        

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

void Country::Register()
{
        if (ms_rtiAmb)
        {
                this->m_instanceID = ms_rtiAmb->registerObjectInstance(this->GetCountryRtiID(),this->GetName());
                cout <<"FED_HW: Registered m_instanceID =" <<m_instanceID <<" "<<endl;
        }                
}

void Country::SetInteractionControl(RTI::Boolean status, RTI::InteractionClassHandle theClass)
{
        if ( theClass == Country::GetCommRtiID())
        {
                ms_sendCommInteractions = status;
                const char *pStr = ms_sendCommInteractions ? "ON":"OFF";
                cout<<"FED_HW: 通信交互状态（turnInteractionsOn）："<< pStr << "."<<endl;
        } else
        {
                const char *pStr = status ? "Start":"Stop";
                cerr<<pStr <<" interaction for unknown class: "<<theClass <<endl;
        }

}

void Country::SetUpdateControl(RTI::Boolean status, const RTI::AttributeHandleSet& attrs)
{
        RTI::AttributeHandle attrHandle;

        for (unsigned int i = 0; i < attrs.size(); i++)
        {
                attrHandle = attrs.getHandle(i);
                if (attrHandle==Country::GetPopulationRtiID())
                {
                        m_sendPopulationAttrUpdates = status;

                        const char *pStr = m_sendPopulationAttrUpdates ? "ON" : "OFF";
                        cout << "FED_HW: Turning Country. Population Updates "
                                 << pStr <<" for object " << this->GetInstanceID()
                                 << ". "<<endl;
                } 
                else if(attrHandle == Country::GetNameRtiID())
                {
                        m_sendNameAttrUpdates = status;

                        const char *pStr = m_sendNameAttrUpdates ? "ON" : "OFF";
                        cout << "FED_HW: Turning Country. Name Updates "
                                 << pStr <<" for object " << this->GetInstanceID()
                                 << ". "<<endl;
                        
                }
        }
}

void Country::updateAttributeValues()
{
	///////////////////////////////////////////////
	// create the necessary container and values //
	///////////////////////////////////////////////
	// create the collection to store the values in, as you can see
	// this is quite a lot of work
	RTI::AttributeHandleValuePairSet *pNvpSet = CreateNVPSet();
	ms_rtiAmb->updateAttributeValues( this->GetInstanceID(), *pNvpSet, "hi!" );

	// note that if you want to associate a particular timestamp with the
	// update. here we send another update, this time with a timestamp:
	//RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
	ms_rtiAmb->updateAttributeValues( this->GetInstanceID(), *pNvpSet, this->GetLastTimePlusLookahead(), "hi!" );

	pNvpSet->empty();
	delete pNvpSet;
}


int Country::CheckFederateState(){return 0;}
void Country::Update(RTI::FedTime& newTime)
{
	RTI::FedTime *pTime = RTI::FedTimeFactory::makeZero();

	*pTime = newTime;
	*pTime -=this->GetLastTime();

	this->SetLastTime(newTime);

	if (!(pTime->isZero()))
	{
		SetPopulation(GetPopulation()+GetPopulation()*ms_growthRate);
	}

	if (ms_rtiAmb)
	{
		try
		{
			RTI::AttributeHandleValuePairSet* pNvpSet = this->CreateNVPSet();
			(void)ms_rtiAmb->updateAttributeValues(this->GetInstanceID(),*pNvpSet,this->GetLastTimePlusLookahead(),NULL);

			pNvpSet->empty();
			delete pNvpSet;
		}
		catch (RTI::Exception& e)
		{
			cerr << "Error:" << &e <<endl;

		}

		static int periodicMessage = 0;
		if ((periodicMessage ++ %100)==0)
		{
			RTI::ParameterHandleValuePairSet* pParams = NULL;
			hasNameChanged = RTI::RTI_TRUE;

			pParams = RTI::ParameterSetFactory::create(1);


			char *pMessage = "ankara mesaj interaction!";

			pParams->add(this->GetMessageRtiID(),(char*)pMessage,((strlen(pMessage)+1)*sizeof(char)));

			try
			{
				(void)ms_rtiAmb->sendInteraction(GetCommRtiID(),*pParams,this->GetLastTimePlusLookahead(),NULL);
			}
			catch (RTI::Exception& e)
			{
				cerr << "FED_HW: Error:" << &e <<endl;

			}
			delete pParams;
		}
	}
	delete pTime;
}

void Country::Update(const RTI::AttributeHandleValuePairSet& theAttributes)
{
	RTI::AttributeHandle attrHandle;
	RTI::ULong           valueLength;

	for(unsigned int i = 0; i < theAttributes.size(); i++)
	{
		attrHandle = theAttributes.getHandle(i);

		if (attrHandle = Country::GetPopulationRtiID())
		{
			double population;
			theAttributes.getValue(i,(char*)&population,valueLength);

			long x = ntohl(*(long*)&population);
			*(long*)&population = ntohl(*((long*)&population)+1);
			*(((long*)&population)+1) = x;

			SetPopulation((double)population);
		}else if(attrHandle == Country::GetNameRtiID())
		{
			char name[1024];

			theAttributes.getValue(i,(char*)name,valueLength);

			SetName((const char*)name);
		}
	}
}


void Country::Update(RTI::InteractionClassHandle theInteraction,const RTI::ParameterHandleValuePairSet& theParameters)
{
        if (theInteraction == Country::GetCommRtiID())
        {
                RTI::ParameterHandle paramHandle;
                RTI::ULong           valueLength;

                for (unsigned int i  = 0; i < theParameters.size(); i++)
                {
                        paramHandle = theParameters.getHandle( i );
                        if (paramHandle == Country::GetMessageRtiID())
                        {
                                char msg[1024];
                                theParameters.getValue(i,(char*)msg,valueLength);
                                cout << "****       Interaction Received: " << msg <<endl;
                        }else
                        {
                                cerr << "FED_HW: Error: I seem to have received a parameter for"
                                         << "interaction class Communication that I don't "
                                         << "know about." << endl;
                        }
                }
        }else
        {
                cerr << "FED_HW: Received an interaction class I don't know about." << endl;
        }
}

void Country::SetPopulation(const double& population)
{
        m_population = population;

        hasPopulationChanged = RTI::RTI_TRUE;
}

RTI::AttributeHandleValuePairSet* Country::CreateNVPSet()
{
	RTI::AttributeHandleValuePairSet* pCountrAttributes = NULL;

	if (ms_rtiAmb)
	{
		pCountrAttributes = RTI::AttributeSetFactory::create(2);
		pCountrAttributes->add(this->GetNameRtiID(),(char*)this->GetName(),((strlen(this->GetName())+1)*sizeof(char)));
		double tmp;
		*((long*)&tmp) = htonl(*(((long*)&this->GetPopulation())+1));
		*(((long*)&tmp)+1) =htonl(*((long*)&this->GetPopulation()));
		pCountrAttributes->add(this->GetPopulationRtiID(),(char*)&tmp,sizeof(double));
		
	}
	return pCountrAttributes;
}
void Country::JoinFederation()
{
	/*
	CountryFederateAmb fedAmb;
	RTI::RTIambassador rtiAmb = RTIAmbassadorInstance::GetInstance();
	try
	{
		RTI::FederateHandle federateID;
		federateID = rtiAmb.joinFederationExecution(GetName(),RTIAmbassadorInstance::GetFedExecName(),&fedAmb);
		cout<< GetName() <<" of federate ID = "<< federateID <<endl;
	}
	catch (RTI::FederateAlreadyExecutionMember &e)
	{
		cerr <<"error: already exists in the Federation Execution "  << endl;
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
*/
}
void Country::Init(RTI::RTIambassador* rtiAmb)
{
	LBSDefaultFederate::Init(rtiAmb);
	if (ms_rtiAmb)
	{
		try
		{
			ms_countryTypeID    = ms_rtiAmb->getObjectClassHandle(ms_countryTypeStr);
			ms_nameTypeID       = ms_rtiAmb->getAttributeHandle(ms_nameTypeStr,ms_countryTypeID);
			ms_populationTypeID = ms_rtiAmb->getAttributeHandle(ms_populationTypeStr,ms_countryTypeID);                        
		}
		catch (RTI::Exception& e)
		{
			cout << "对象类初始化失败！" << &e <<endl;
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
			cout <<"交互类初始化失败！"<< &e << endl;
			exit(1);
		}
		cout<<"Init Finished"<<endl<<endl;
	}
}
