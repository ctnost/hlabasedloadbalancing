#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "WSN.h"
#include "WSNFederateAmb.h"
#include <sstream>

WSNFederateAmb::WSNFederateAmb()
{
}

WSNFederateAmb::~WSNFederateAmb()
        throw (RTI::FederateInternalError)
{
}


////////////////////////////////////
// Federation Management Services //
////////////////////////////////////
void WSNFederateAmb::synchronizationPointRegistrationSucceeded (
        const char *label) // supplied C4)
        throw (
        RTI::FederateInternalError)
{
        cout<<"SynchronizationPointRegistrationSucceded:: "<<label<<endl<<endl;
        SyncPointRegisterSucceeded = RTI::RTI_TRUE;
}

void WSNFederateAmb::synchronizationPointRegistrationFailed (
        const char *label) // supplied C4)
        throw (
        RTI::FederateInternalError)
{
        SyncPointRegisterFailed = RTI::RTI_TRUE;        
}

void WSNFederateAmb::announceSynchronizationPoint (
        const char *label, // supplied C4
        const char *tag)   // supplied C4
        throw (
        RTI::FederateInternalError)
{
        AnnounceSyncReceived = RTI::RTI_TRUE;
}

void WSNFederateAmb::federationSynchronized (
        const char *label) // supplied C4)
        throw (
        RTI::FederateInternalError)
{
        FederationalsSynchronized = RTI::RTI_TRUE;
}

/////////////////////////////////////
// Declaration Management Services //
/////////////////////////////////////

void WSNFederateAmb::startRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
        throw (
           RTI::ObjectClassNotPublished,
           RTI::FederateInternalError)
{
        if (theClass == myWSN->GetWSNRtiID())
        {
               // myWSN->SetRegistration(RTI::RTI_TRUE);
                cout << "发现异地对Country 的订购，开启对Country的注册！startRegistrationForObjectClass!"<<endl<<endl;
        } 
        else
        {
                cerr << "startRegistrationForObjectClass unknow class: " << theClass <<endl;
        }
}

void WSNFederateAmb::stopRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
        throw (
           RTI::ObjectClassNotPublished,
           RTI::FederateInternalError)
{
        if (theClass ==myWSN->GetWSNRtiID())
        {
              //  myWSN->SetRegistration(RTI::RTI_FALSE);
                cout << "Turned registration off for Country class \n" << endl;
        } 
        else
        {
                cerr << "stopRegistrationForObjectClass unknow class: " << theClass <<endl;
        }

}

void WSNFederateAmb::turnInteractionsOn (RTI::InteractionClassHandle theHandle) // supplied C1
        throw (
           RTI::InteractionClassNotPublished,
           RTI::FederateInternalError)         
{        
        myWSN->SetInteractionControl(RTI::RTI_TRUE,theHandle);
}

void WSNFederateAmb::turnInteractionsOff (RTI::InteractionClassHandle theHandle) // supplied C1
        throw (
           RTI::InteractionClassNotPublished,
           RTI::FederateInternalError)
{
        myWSN->SetInteractionControl(RTI::RTI_FALSE,theHandle);
}


////////////////////////////////
// Object Management Services //
////////////////////////////////

void WSNFederateAmb::discoverObjectInstance (
                                                                        RTI::ObjectHandle theObject,
                                                                        RTI::ObjectClassHandle theObjectClass, 
                                                                        const char* theObjectName)    
                                                                        throw (
                                                                        RTI::CouldNotDiscover,
                                                                        RTI::ObjectClassNotKnown,
                                                                        RTI::FederateInternalError)
{
		cout << "FED_HW: WirelessSensor -*- " <<theObjectName <<endl;
		WirelessSensor* ws = new WirelessSensor(theObject);
		ws->name = theObjectName ;
		myWSN->Insert(theObject,ws);
        discoverNum++;
}

void WSNFederateAmb::reflectAttributeValues (
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
	cout << "Reflection Received:";
	myWSN->Update(theAttributes,theObject);
}

void WSNFederateAmb::reflectAttributeValues (
                                                                        RTI::ObjectHandle                 theObject,     
                                                                        const RTI::AttributeHandleValuePairSet& theAttributes, 
                                                                        const char                             *theTag)        
                                                                        throw (
                                                                        RTI::ObjectNotKnown,
                                                                        RTI::AttributeNotKnown,
                                                                        RTI::FederateOwnsAttributes,
                                                                        RTI::FederateInternalError) 
{
	cout << "Reflection Received:";
	myWSN->Update(theAttributes,theObject);
}

void WSNFederateAmb::receiveInteraction (
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
/*
	if(myCountry->GetFederateState() == DEFAULT)
		this->receiveInteraction(theInteraction,theParameters,theTag);
	else
	{
		LBSInteractionItem *newItem;
		newItem->theInteraction = theInteraction;
		newItem->theParameters = theParameters;
		newItem->theTag = const_cast<char*>(theTag);
		newItem->theTime = theTime;
		newItem->type = 0; 
		myCountry->inQueue->push_back(newItem);
	}*/

}

void WSNFederateAmb::receiveInteraction (
                                                                 RTI::InteractionClassHandle       theInteraction, 
                                                                 const RTI::ParameterHandleValuePairSet& theParameters,  
                                                                 const char                             *theTag)         
                                                                 throw (
                                                                 RTI::InteractionClassNotKnown,
                                                                 RTI::InteractionParameterNotKnown,
                                                                 RTI::FederateInternalError) 
{
        myWSN->Update(theInteraction,theParameters);
}

void WSNFederateAmb::removeObjectInstance (
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

void WSNFederateAmb::removeObjectInstance (
                                                                   RTI::ObjectHandle          theObject, 
                                                                   const char                      *theTag)   
                                                                   throw (
                                                                   RTI::ObjectNotKnown,
                                                                   RTI::FederateInternalError)
{
        cout << "Removed object :" << theObject <<endl;
		myWSN->Remove(theObject);
}

void WSNFederateAmb::provideAttributeValueUpdate (
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

void WSNFederateAmb::turnUpdatesOnForObjectInstance (
                                                                        RTI::ObjectHandle        theObject,    
                                                                        const RTI::AttributeHandleSet& theAttributes) 
                                                                        throw (
                                                                        RTI::ObjectNotKnown,
                                                                        RTI::AttributeNotOwned,
                                                                        RTI::FederateInternalError)
{

}

void WSNFederateAmb::turnUpdatesOffForObjectInstance (
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

void WSNFederateAmb::timeRegulationEnabled (
                                                                        const  RTI::FedTime& theFederateTime) 
                                                                        throw (
                                                                        RTI::InvalidFederationTime,
                                                                        RTI::EnableTimeRegulationWasNotPending,
                                                                        RTI::FederateInternalError) 
{
        cout << "Time granted (timeRegulationEnabled) to: " << convertTime( theFederateTime)<<endl;

        GrantTime        = theFederateTime;
        timeAdvGrant = RTI::RTI_TRUE;
        this->TimeRegulation   = RTI::RTI_TRUE;
}
double WSNFederateAmb::convertTime( const RTI::FedTime& theTime )
{
	RTIfedTime castedTime = (RTIfedTime)theTime;
	return castedTime.getTime();
}
void WSNFederateAmb::timeConstrainedEnabled (
                                                                         const RTI::FedTime& theFederateTime) 
                                                                         throw (
                                                                         RTI::InvalidFederationTime,
                                                                         RTI::EnableTimeConstrainedWasNotPending,
                                                                         RTI::FederateInternalError) 
{
	cout << this <<endl ;
        cout << "Time granted (timeConstrainedEnabled) to: " << convertTime(theFederateTime) << endl;

        GrantTime       = theFederateTime;
        timeAdvGrant    = RTI::RTI_TRUE;
        TimeConstrained = RTI::RTI_TRUE;
}

void WSNFederateAmb::timeAdvanceGrant (
                                                                         const RTI::FedTime& theTime) 
                                                                         throw (
                                                                         RTI::InvalidFederationTime,
                                                                         RTI::TimeAdvanceWasNotInProgress,
                                                                         RTI::FederateInternalError) 
{
      //  cout << "Time granted (timeAdvanceGrant) to: " << convertTime(theTime) <<endl;
        GrantTime = theTime;
        timeAdvGrant = RTI::RTI_TRUE;
}

void WSNFederateAmb::SetFederateInstance(LBSDefaultFederate* lbsFed)
{
	this->myWSN = dynamic_cast<WSN*>(lbsFed);
}