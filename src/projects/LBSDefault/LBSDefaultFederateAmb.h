//нд╪Ч countryfederateamb.h
#ifndef LBSDEFAULTFEDERATEAMB_H_
#define LBSDEFAULTFEDERATEAMB_H_

#pragma warning(disable: 4786)
#pragma warning(disable: 4290)

#include "NullFederateAmbassador.hh"
#include "LBSDefaultFederate.h"
using namespace std;

class LBSDefaultFederateAmb :         public NullFederateAmbassador
{
public:
	LBSDefaultFederateAmb()
		:GrantTime(*(RTI::FedTimeFactory::makeZero()))
	{
		timeAdvGrant =true;
		TimeRegulation = false;
		TimeConstrained = false;
		SyncPointRegisterSucceeded = false;
		SyncPointRegisterFailed    = false;
		AnnounceSyncReceived       = false;
		FederationalsSynchronized  = false;
		discoverNum = 0;
	}
	virtual ~LBSDefaultFederateAmb()
		throw (RTI::FederateInternalError){}


	////////////////////////////////////
	// Federation Management Services //
	////////////////////////////////////
	virtual void synchronizationPointRegistrationSucceeded (
		const char *label) // supplied C4)
		throw (
		RTI::FederateInternalError){
			cout <<	"Test";
	}

	virtual void synchronizationPointRegistrationFailed (
		const char *label) // supplied C4)
		throw (
		RTI::FederateInternalError){cout <<	"Test";}

	virtual void announceSynchronizationPoint (
		const char *label, // supplied C4
		const char *tag)   // supplied C4
		throw (
		RTI::FederateInternalError){cout <<	"Test";}

	virtual void federationSynchronized (
		const char *label) // supplied C4)
		throw (
		RTI::FederateInternalError){cout <<	"Test";}


	/////////////////////////////////////
	// Declaration Management Services //
	/////////////////////////////////////

	virtual void startRegistrationForObjectClass (
		RTI::ObjectClassHandle   theClass)      // supplied C1
		throw (
		RTI::ObjectClassNotPublished,
		RTI::FederateInternalError){cout <<	"Test";}

	virtual void stopRegistrationForObjectClass (
		RTI::ObjectClassHandle   theClass)      // supplied C1
		throw (
		RTI::ObjectClassNotPublished,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void turnInteractionsOn (
		RTI::InteractionClassHandle theHandle) // supplied C1
		throw (
		RTI::InteractionClassNotPublished,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void turnInteractionsOff (
		RTI::InteractionClassHandle theHandle) // supplied C1
		throw (
		RTI::InteractionClassNotPublished,
		RTI::FederateInternalError) {cout <<	"Test";}

	////////////////////////////////
	// Object Management Services //
	////////////////////////////////

	virtual void discoverObjectInstance (
		RTI::ObjectHandle          theObject,      // supplied C1
		RTI::ObjectClassHandle     theObjectClass, // supplied C1
		const char*                      theObjectName)  // supplied C4  
		throw (
		RTI::CouldNotDiscover,
		RTI::ObjectClassNotKnown,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void reflectAttributeValues (
		RTI::ObjectHandle                 theObject,     // supplied C1
		const RTI::AttributeHandleValuePairSet& theAttributes, // supplied C4
		const RTI::FedTime&                     theTime,       // supplied C1
		const char                             *theTag,        // supplied C4
		RTI::EventRetractionHandle        theHandle)     // supplied C1
		throw (
		RTI::ObjectNotKnown,
		RTI::AttributeNotKnown,
		RTI::FederateOwnsAttributes,
		RTI::InvalidFederationTime,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void reflectAttributeValues (
		RTI::ObjectHandle                 theObject,     // supplied C1
		const RTI::AttributeHandleValuePairSet& theAttributes, // supplied C4
		const char                             *theTag)        // supplied C4
		throw (
		RTI::ObjectNotKnown,
		RTI::AttributeNotKnown,
		RTI::FederateOwnsAttributes,
		RTI::FederateInternalError) {cout <<	"Test";}

	// 4.6
	virtual void receiveInteraction (
		RTI::InteractionClassHandle       theInteraction, // supplied C1
		const RTI::ParameterHandleValuePairSet& theParameters,  // supplied C4
		const RTI::FedTime&                     theTime,        // supplied C4
		const char                             *theTag,         // supplied C4
		RTI::EventRetractionHandle        theHandle)      // supplied C1
		throw (
		RTI::InteractionClassNotKnown,
		RTI::InteractionParameterNotKnown,
		RTI::InvalidFederationTime,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void receiveInteraction (
		RTI::InteractionClassHandle       theInteraction, // supplied C1
		const RTI::ParameterHandleValuePairSet& theParameters,  // supplied C4
		const char                             *theTag)         // supplied C4
		throw (
		RTI::InteractionClassNotKnown,
		RTI::InteractionParameterNotKnown,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void removeObjectInstance (
		RTI::ObjectHandle          theObject, // supplied C1
		const RTI::FedTime&              theTime,   // supplied C4
		const char                      *theTag,    // supplied C4
		RTI::EventRetractionHandle theHandle) // supplied C1
		throw (
		RTI::ObjectNotKnown,
		RTI::InvalidFederationTime,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void removeObjectInstance (
		RTI::ObjectHandle          theObject, // supplied C1
		const char                      *theTag)    // supplied C4
		throw (
		RTI::ObjectNotKnown,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void provideAttributeValueUpdate (
		RTI::ObjectHandle        theObject,     // supplied C1
		const RTI::AttributeHandleSet& theAttributes) // supplied C4
		throw (
		RTI::ObjectNotKnown,
		RTI::AttributeNotKnown,
		RTI::AttributeNotOwned,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void turnUpdatesOnForObjectInstance (
		RTI::ObjectHandle        theObject,     // supplied C1
		const RTI::AttributeHandleSet& theAttributes) // supplied C4
		throw (
		RTI::ObjectNotKnown,
		RTI::AttributeNotOwned,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void turnUpdatesOffForObjectInstance (
		RTI::ObjectHandle        theObject,      // supplied C1
		const RTI::AttributeHandleSet& theAttributes) // supplied C4
		throw (
		RTI::ObjectNotKnown,
		RTI::AttributeNotOwned,
		RTI::FederateInternalError) {cout <<	"Test";}

	//////////////////////////////
	// Time Management Services //
	//////////////////////////////

	virtual void timeRegulationEnabled (
		const  RTI::FedTime& theFederateTime) // supplied C4
		throw (
		RTI::InvalidFederationTime,
		RTI::EnableTimeRegulationWasNotPending,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void timeConstrainedEnabled (
		const RTI::FedTime& theFederateTime) // supplied C4
		throw (
		RTI::InvalidFederationTime,
		RTI::EnableTimeConstrainedWasNotPending,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void timeAdvanceGrant (
		const RTI::FedTime& theTime) // supplied C4
		throw (
		RTI::InvalidFederationTime,
		RTI::TimeAdvanceWasNotInProgress,
		RTI::FederateInternalError) {cout <<	"Test";}

	virtual void	 SetFederateInstance(LBSDefaultFederate* lbsFed){ };
public:
	bool  timeAdvGrant;
	bool  TimeRegulation;
	bool  TimeConstrained;
	RTI::FedTime& GrantTime ;


	bool  SyncPointRegisterSucceeded ;
	bool  SyncPointRegisterFailed;
	bool  AnnounceSyncReceived;
	bool  FederationalsSynchronized;

	int discoverNum;

	void SetTimeConstrained()
	{
		cout << "Test tc " << TimeConstrained <<"Adres" << this <<endl;
	}
};
#endif