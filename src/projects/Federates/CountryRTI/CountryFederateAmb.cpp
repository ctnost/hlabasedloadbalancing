#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Country.h"
#include "CountryFederateAmb.h"

/*
extern RTI::Boolean  timeAdvGrant;
extern RTI::Boolean  TimeRegulation;
extern RTI::Boolean  TimeConstrained;
extern RTI::FedTime& GrantTime ;


extern RTI::Boolean  SyncPointRegisterSucceeded ;
extern RTI::Boolean  SyncPointRegisterFailed;
extern RTI::Boolean  AnnounceSyncReceived;
extern RTI::Boolean  FederationalsSynchronized;

extern int discoverNum;
*/


CountryFederateAmb::CountryFederateAmb()
{
}

CountryFederateAmb::~CountryFederateAmb()
        throw (RTI::FederateInternalError)
{
}


////////////////////////////////////
// Federation Management Services //
////////////////////////////////////
void CountryFederateAmb::synchronizationPointRegistrationSucceeded (
        const char *label) // supplied C4)
        throw (
        RTI::FederateInternalError)
{
        cout<<"SynchronizationPointRegistrationSucceded:: "<<label<<endl<<endl;
        SyncPointRegisterSucceeded = RTI::RTI_TRUE;
}

void CountryFederateAmb::synchronizationPointRegistrationFailed (
        const char *label) // supplied C4)
        throw (
        RTI::FederateInternalError)
{
        SyncPointRegisterFailed = RTI::RTI_TRUE;        
}

void CountryFederateAmb::announceSynchronizationPoint (
        const char *label, // supplied C4
        const char *tag)   // supplied C4
        throw (
        RTI::FederateInternalError)
{
        AnnounceSyncReceived = RTI::RTI_TRUE;
}

void CountryFederateAmb::federationSynchronized (
        const char *label) // supplied C4)
        throw (
        RTI::FederateInternalError)
{
        FederationalsSynchronized = RTI::RTI_TRUE;
}

/////////////////////////////////////
// Declaration Management Services //
/////////////////////////////////////

void CountryFederateAmb::startRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
        throw (
           RTI::ObjectClassNotPublished,
           RTI::FederateInternalError)
{
        if (theClass == myCountry->GetCountryRtiID())
        {
                myCountry->SetRegistration(RTI::RTI_TRUE);
                cout << "发现异地对Country 的订购，开启对Country的注册！startRegistrationForObjectClass!"<<endl<<endl;
        } 
        else
        {
                cerr << "startRegistrationForObjectClass unknow class: " << theClass <<endl;
        }
}

void CountryFederateAmb::stopRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
        throw (
           RTI::ObjectClassNotPublished,
           RTI::FederateInternalError)
{
        if (theClass ==myCountry->GetCountryRtiID())
        {
                myCountry->SetRegistration(RTI::RTI_FALSE);
                cout << "Turned registration off for Country class \n" << endl;
        } 
        else
        {
                cerr << "stopRegistrationForObjectClass unknow class: " << theClass <<endl;
        }

}

void CountryFederateAmb::turnInteractionsOn (RTI::InteractionClassHandle theHandle) // supplied C1
        throw (
           RTI::InteractionClassNotPublished,
           RTI::FederateInternalError)         
{        
        myCountry->SetInteractionControl(RTI::RTI_TRUE,theHandle);
}

void CountryFederateAmb::turnInteractionsOff (RTI::InteractionClassHandle theHandle) // supplied C1
        throw (
           RTI::InteractionClassNotPublished,
           RTI::FederateInternalError)
{
        myCountry->SetInteractionControl(RTI::RTI_FALSE,theHandle);
}


////////////////////////////////
// Object Management Services //
////////////////////////////////

void CountryFederateAmb::discoverObjectInstance (
                                                                        RTI::ObjectHandle theObject,
                                                                        RTI::ObjectClassHandle theObjectClass, 
                                                                        const char* theObjectName)    
                                                                        throw (
                                                                        RTI::CouldNotDiscover,
                                                                        RTI::ObjectClassNotKnown,
                                                                        RTI::FederateInternalError)
{
        cout << "FED_HW: -*- " <<theObject <<endl;
/*
        if (theObjectClass == myCountry->GetCountryRtiID())
        {
                Country *tmpPtr = new Country(theObject,"Country");
                tmpPtr->SetName(theObjectName);
        } 
        else
        {
                cerr << "Discovered object class unknow to me." << endl;
        }*/

        discoverNum++;
}

void CountryFederateAmb::reflectAttributeValues (
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

	// print the handle
	cout << " object=" << theObject;
	// print the tag
	cout << ", tag=" << theTag;
	// print the time
	cout << ", time=" << convertTime( theTime );

	// print the attribute information
	cout << ", attributeCount=" << theAttributes.size() << endl;
	for( RTI::ULong i = 0; i < theAttributes.size(); i++ )
	{
		// print the attribute handle
		cout << "\tattrHandle=" << theAttributes.getHandle(i);
		// print the attribute value
		RTI::ULong length = theAttributes.getValueLength(i);
		char *value = theAttributes.getValuePointer(i,length);

		cout << ", attrValue=" << value << endl;
	}

        Country *pCountry = myCountry->Find(theObject);

        if (pCountry)
        {
                pCountry->Update(const_cast<RTI::AttributeHandleValuePairSet&>(theAttributes));
                pCountry->SetLastTime(theTime);
        } 
        else
        {
                throw RTI::ObjectNotKnown("received reflection for unknow 0 ID");
        }
}

void CountryFederateAmb::reflectAttributeValues (
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

	// print the handle
	cout << " object=" << theObject;
	// print the tag
	cout << ", tag=" << theTag;

	// print the attribute information
	cout << ", attributeCount=" << theAttributes.size() << endl;
	for( RTI::ULong i = 0; i < theAttributes.size(); i++ )
	{
		// print the attribute handle
		cout << "\tattrHandle=" << theAttributes.getHandle(i);
		// print the attribute value
		RTI::ULong length = theAttributes.getValueLength(i);
		char *value = theAttributes.getValuePointer(i,length);

		cout << ", attrValue=" << value << endl;
	}
        Country *pCountry = myCountry->Find(theObject);

        if (pCountry)
        {
                pCountry->Update(const_cast<RTI::AttributeHandleValuePairSet&>(theAttributes));
                RTI::FedTime *zero_time = RTI::FedTimeFactory::makeZero();
                pCountry->SetLastTime(*zero_time);
                delete zero_time;        
        }         
}

void CountryFederateAmb::receiveInteraction (
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
	//if(myCountry->GetFederateState() == DEFAULT)
	//	this->receiveInteraction(theInteraction,theParameters,theTag);
	//else
	//{
	//	LBSInteractionItem *newItem;
	//	newItem->theInteraction = theInteraction;
	//	newItem->theParameters = theParameters;
	//	newItem->theTag = const_cast<char*>(theTag);
	//	newItem->theTime = theTime;
	//	newItem->type = 0; 
	//	myCountry->inQueue->push_back(newItem);
	//}
}

void CountryFederateAmb::receiveInteraction (
                                                                 RTI::InteractionClassHandle       theInteraction, 
                                                                 const RTI::ParameterHandleValuePairSet& theParameters,  
                                                                 const char                             *theTag)         
                                                                 throw (
                                                                 RTI::InteractionClassNotKnown,
                                                                 RTI::InteractionParameterNotKnown,
                                                                 RTI::FederateInternalError) 
{
        myCountry->Update(theInteraction,theParameters);
}

void CountryFederateAmb::removeObjectInstance (
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

void CountryFederateAmb::removeObjectInstance (
                                                                   RTI::ObjectHandle          theObject, 
                                                                   const char                      *theTag)   
                                                                   throw (
                                                                   RTI::ObjectNotKnown,
                                                                   RTI::FederateInternalError)
{
        cout << "Removed object :" << theObject <<endl;

        Country *pCountry = myCountry->Find(theObject);
        if (pCountry)
        {
                delete pCountry;
        }
}

void CountryFederateAmb::provideAttributeValueUpdate (
                                                                        RTI::ObjectHandle        theObject,    
                                                                        const RTI::AttributeHandleSet& theAttributes) 
                                                                        throw (
                                                                        RTI::ObjectNotKnown,
                                                                        RTI::AttributeNotKnown,
                                                                        RTI::AttributeNotOwned,
                                                                        RTI::FederateInternalError)
{
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
        }
}

void CountryFederateAmb::turnUpdatesOnForObjectInstance (
                                                                        RTI::ObjectHandle        theObject,    
                                                                        const RTI::AttributeHandleSet& theAttributes) 
                                                                        throw (
                                                                        RTI::ObjectNotKnown,
                                                                        RTI::AttributeNotOwned,
                                                                        RTI::FederateInternalError)
{
        Country *pCountry = myCountry->Find(theObject);

        if (pCountry)
        {
                pCountry->SetUpdateControl(RTI::RTI_TRUE,theAttributes);
        }else
        {
                cout << "Country object " << theObject << "not found" <<endl;
        }
}

void CountryFederateAmb::turnUpdatesOffForObjectInstance (
                                                                        RTI::ObjectHandle        theObject,      
                                                                        const RTI::AttributeHandleSet& theAttributes) 
                                                                        throw (
                                                                        RTI::ObjectNotKnown,
                                                                        RTI::AttributeNotOwned,
                                                                        RTI::FederateInternalError)
{
        Country *pCountry = myCountry->Find(theObject);

        if (pCountry)
        {
                pCountry->SetUpdateControl(RTI::RTI_FALSE,theAttributes);
        }else
        {
                cout << "Country object " << theObject << "not found" <<endl;
        }
}

//////////////////////////////
// Time Management Services //
//////////////////////////////

void CountryFederateAmb::timeRegulationEnabled (
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
double CountryFederateAmb::convertTime( const RTI::FedTime& theTime )
{
	RTIfedTime castedTime = (RTIfedTime)theTime;
	return castedTime.getTime();
}
void CountryFederateAmb::timeConstrainedEnabled (
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

void CountryFederateAmb::timeAdvanceGrant (
                                                                         const RTI::FedTime& theTime) 
                                                                         throw (
                                                                         RTI::InvalidFederationTime,
                                                                         RTI::TimeAdvanceWasNotInProgress,
                                                                         RTI::FederateInternalError) 
{
        cout << "Time granted (timeAdvanceGrant) to: " << convertTime(theTime) <<endl;
        GrantTime = theTime;
        timeAdvGrant = RTI::RTI_TRUE;
}

void CountryFederateAmb::SetFederateInstance(LBSDefaultFederate* lbsFed)
{
	this->myCountry = dynamic_cast<Country*>(lbsFed);
}