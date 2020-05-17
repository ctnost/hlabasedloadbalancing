#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "WSFederate.h"
#include "WSFederateAmb.h"


WSFederateAmb::WSFederateAmb()
{
}

WSFederateAmb::~WSFederateAmb()
        throw (RTI::FederateInternalError)
{
}


////////////////////////////////////
// Federation Management Services //
////////////////////////////////////
void WSFederateAmb::synchronizationPointRegistrationSucceeded (
        const char *label) // supplied C4)
        throw (
        RTI::FederateInternalError)
{
        cout<<"SynchronizationPointRegistrationSucceded:: "<<label<<endl<<endl;
        SyncPointRegisterSucceeded = RTI::RTI_TRUE;
}

void WSFederateAmb::synchronizationPointRegistrationFailed (
        const char *label) // supplied C4)
        throw (
        RTI::FederateInternalError)
{
        SyncPointRegisterFailed = RTI::RTI_TRUE;        
}

void WSFederateAmb::announceSynchronizationPoint (
        const char *label, // supplied C4
        const char *tag)   // supplied C4
        throw (
        RTI::FederateInternalError)
{
        AnnounceSyncReceived = RTI::RTI_TRUE;
}

void WSFederateAmb::federationSynchronized (
        const char *label) // supplied C4)
        throw (
        RTI::FederateInternalError)
{
        FederationalsSynchronized = RTI::RTI_TRUE;
}

/////////////////////////////////////
// Declaration Management Services //
/////////////////////////////////////

void WSFederateAmb::startRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
        throw (
           RTI::ObjectClassNotPublished,
           RTI::FederateInternalError)
{
        if (theClass == myWS->GetWSRtiID())
        {
               // myWS->SetRegistration(RTI::RTI_TRUE);
                cout << "发现异地对Country 的订购，开启对Country的注册！startRegistrationForObjectClass!"<<endl<<endl;
        } 
        else
        {
                cerr << "startRegistrationForObjectClass unknow class: " << theClass <<endl;
        }
}

void WSFederateAmb::stopRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
        throw (
           RTI::ObjectClassNotPublished,
           RTI::FederateInternalError)
{
        if (theClass ==myWS->GetWSRtiID())
        {
              //  myWS->SetRegistration(RTI::RTI_FALSE);
                cout << "Turned registration off for Country class \n" << endl;
        } 
        else
        {
                cerr << "stopRegistrationForObjectClass unknow class: " << theClass <<endl;
        }

}

void WSFederateAmb::turnInteractionsOn (RTI::InteractionClassHandle theHandle) // supplied C1
        throw (
           RTI::InteractionClassNotPublished,
           RTI::FederateInternalError)         
{        
        myWS->SetInteractionControl(RTI::RTI_TRUE,theHandle);
}

void WSFederateAmb::turnInteractionsOff (RTI::InteractionClassHandle theHandle) // supplied C1
        throw (
           RTI::InteractionClassNotPublished,
           RTI::FederateInternalError)
{
        myWS->SetInteractionControl(RTI::RTI_FALSE,theHandle);
}


////////////////////////////////
// Object Management Services //
////////////////////////////////

void WSFederateAmb::discoverObjectInstance (
                                                                        RTI::ObjectHandle theObject,
                                                                        RTI::ObjectClassHandle theObjectClass, 
                                                                        const char* theObjectName)    
                                                                        throw (
                                                                        RTI::CouldNotDiscover,
                                                                        RTI::ObjectClassNotKnown,
                                                                        RTI::FederateInternalError)
{
        //cout << "FED_HW: WirelessSensor -*- " <<theObject <<endl;
		//WirelessSensor* ws = new WirelessSensor(theObject);
		//myWS->Insert(theObject,ws);
        discoverNum++;
		// cout << "*******FED_HW: WirelessSensor DiscoverNumber -*- " <<discoverNum <<endl;
}

void WSFederateAmb::reflectAttributeValues (
                                                                         RTI::ObjectHandle                 theObject,     
                                                                         const RTI::AttributeHandleValuePairSet& theAttributes, 
                                                                         const RTI::FedTime&                     theTime,       
                                                                         const char                             *theTag,        
                                                                         RTI::EventRetractionHandle        theHandle)     
                                                                         throw (
                                                                         RTI::ObjectNotKnown,
                                                                         RTI::AttributeNotKnown,
                                                                         RTI::FederateOwnsAttributes,
                                                                         RTI::InvalidFederationTime,
                                                                         RTI::FederateInternalError)                 
{
	//cout << "Reflection Received:";
	//myWS->Update(theAttributes,theObject);
}

void WSFederateAmb::reflectAttributeValues (
                                                                        RTI::ObjectHandle                 theObject,     
                                                                        const RTI::AttributeHandleValuePairSet& theAttributes, 
                                                                        const char                             *theTag)        
                                                                        throw (
                                                                        RTI::ObjectNotKnown,
                                                                        RTI::AttributeNotKnown,
                                                                        RTI::FederateOwnsAttributes,
                                                                        RTI::FederateInternalError) 
{
	//cout << "Reflection Received:";
//	myWS->Update(theAttributes,theObject);
}

void WSFederateAmb::receiveInteraction (
                                                                 RTI::InteractionClassHandle       theInteraction, 
                                                                 const RTI::ParameterHandleValuePairSet& theParameters,  
                                                                 const RTI::FedTime&                     theTime,        
                                                                 const char                             *theTag,        
                                                                 RTI::EventRetractionHandle        theHandle)      
                                                                 throw (
                                                                 RTI::InteractionClassNotKnown,
                                                                 RTI::InteractionParameterNotKnown,
                                                                 RTI::InvalidFederationTime,
                                                                 RTI::FederateInternalError)
{
	if(myWS->GetFederateState() == DEFAULT)
		myWS->ReceiveData(theInteraction,theParameters,theTag);
	else{
		cout<<"Receiv eden interaction kayit ediliyor!!!" << endl;
		LBSInteractionItem newItem;
		newItem.theInteraction = theInteraction;
		newItem.theParameters = RTI::ParameterSetFactory::create(theParameters.size());

		RTI::ParameterHandle paramHandle;
		RTI::ULong           valueLength;
		for (unsigned int i  = 0; i < theParameters.size(); i++)
		{
			paramHandle = theParameters.getHandle( i );
			valueLength = theParameters.getValueLength(i);
			char *valueCh = theParameters.getValuePointer(i, valueLength);
			newItem.theParameters->add(paramHandle,valueCh,valueLength);
		}
		newItem.theTag = atoi(theTag);
		double time_ = convertTime( theTime);
		newItem.theTime = time_;
		newItem.type = 0; 
		myWS->inQueue->push_back(newItem);
	}
}

void WSFederateAmb::receiveInteraction (
                                                                 RTI::InteractionClassHandle       theInteraction, 
                                                                 const RTI::ParameterHandleValuePairSet& theParameters,  
                                                                 const char                             *theTag)         
                                                                 throw (
                                                                 RTI::InteractionClassNotKnown,
                                                                 RTI::InteractionParameterNotKnown,
                                                                 RTI::FederateInternalError) 
{
    myWS->ReceiveData(theInteraction,theParameters,theTag);
}

void WSFederateAmb::removeObjectInstance (
                                                                   RTI::ObjectHandle                  theObject, 
                                                                   const RTI::FedTime&                theTime,  
                                                                   const char                      *theTag,    
                                                                   RTI::EventRetractionHandle theHandle) 
                                                                   throw (
                                                                   RTI::ObjectNotKnown,
                                                                   RTI::InvalidFederationTime,
                                                                   RTI::FederateInternalError)
{
        this->removeObjectInstance(theObject,theTag);
}

void WSFederateAmb::removeObjectInstance (
                                                                   RTI::ObjectHandle          theObject, 
                                                                   const char                      *theTag)   
                                                                   throw (
                                                                   RTI::ObjectNotKnown,
                                                                   RTI::FederateInternalError)
{
        cout << "Removed object :" << theObject <<endl;
//		myWS->Remove(theObject);
}

void WSFederateAmb::provideAttributeValueUpdate (
                                                                        RTI::ObjectHandle        theObject,    
                                                                        const RTI::AttributeHandleSet& theAttributes) 
                                                                        throw (
                                                                        RTI::ObjectNotKnown,
                                                                        RTI::AttributeNotKnown,
                                                                        RTI::AttributeNotOwned,
                                                                        RTI::FederateInternalError)
{
/*
        Country *pCountry = myCountry->Find(theObject);

        if (pCountry)
        {
                RTI::AttributeHandle attrHandle;
                for (unsigned int i= 0; i < theAttributes.size(); i++)
                {
                        attrHandle = theAttributes.getHandle(i);
                        if (attrHandle == myCountry->GetPopulationRtiID())
                        {
                                pCountry->SetPopulation(pCountry->GetPopulation());
                        } 
                        else if(attrHandle == myCountry->GetNameRtiID())
                        {
                                pCountry->SetName(pCountry->GetName());
                        }
                }
        }*/

}

void WSFederateAmb::turnUpdatesOnForObjectInstance (
                                                                        RTI::ObjectHandle        theObject,    
                                                                        const RTI::AttributeHandleSet& theAttributes) 
                                                                        throw (
                                                                        RTI::ObjectNotKnown,
                                                                        RTI::AttributeNotOwned,
                                                                        RTI::FederateInternalError)
{

}

void WSFederateAmb::turnUpdatesOffForObjectInstance (
                                                                        RTI::ObjectHandle        theObject,      
                                                                        const RTI::AttributeHandleSet& theAttributes) 
                                                                        throw (
                                                                        RTI::ObjectNotKnown,
                                                                        RTI::AttributeNotOwned,
                                                                        RTI::FederateInternalError)
{

}

//////////////////////////////
// Time Management Services //
//////////////////////////////

void WSFederateAmb::timeRegulationEnabled (
                                                                        const  RTI::FedTime& theFederateTime) 
                                                                        throw (
                                                                        RTI::InvalidFederationTime,
                                                                        RTI::EnableTimeRegulationWasNotPending,
                                                                        RTI::FederateInternalError) 
{
        //cout << "Time granted (timeRegulationEnabled) to: " << convertTime( theFederateTime)<<endl;

        GrantTime        = theFederateTime;
        timeAdvGrant = RTI::RTI_TRUE;
        this->TimeRegulation   = RTI::RTI_TRUE;
}
double WSFederateAmb::convertTime( const RTI::FedTime& theTime )
{
	RTIfedTime castedTime = (RTIfedTime)theTime;
	return castedTime.getTime();
}
void WSFederateAmb::timeConstrainedEnabled (
                                                                         const RTI::FedTime& theFederateTime) 
                                                                         throw (
                                                                         RTI::InvalidFederationTime,
                                                                         RTI::EnableTimeConstrainedWasNotPending,
                                                                         RTI::FederateInternalError) 
{
	//cout << this <<endl ;
      //  cout << "Time granted (timeConstrainedEnabled) to: " << convertTime(theFederateTime) << endl;

        GrantTime       = theFederateTime;
        timeAdvGrant    = RTI::RTI_TRUE;
        TimeConstrained = RTI::RTI_TRUE;
}

void WSFederateAmb::timeAdvanceGrant (
                                                                         const RTI::FedTime& theTime) 
                                                                         throw (
                                                                         RTI::InvalidFederationTime,
                                                                         RTI::TimeAdvanceWasNotInProgress,
                                                                         RTI::FederateInternalError) 
{
       // cout << "Time granted (timeAdvanceGrant) to: " << convertTime(theTime) <<endl;
        GrantTime = theTime;
		myWS->Update(GrantTime);
        timeAdvGrant = RTI::RTI_TRUE;
}

void WSFederateAmb::SetFederateInstance(LBSDefaultFederate* lbsFed)
{
	this->myWS = dynamic_cast<WSFederate*>(lbsFed);
}