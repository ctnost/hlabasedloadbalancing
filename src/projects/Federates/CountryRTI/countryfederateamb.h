#ifndef COUNTRYFEDERATEAMB_H_
#define COUNTRYFEDERATEAMB_H_

#pragma warning(disable: 4786)
#pragma warning(disable: 4290)

#include "country.h"
#include "LBSDefaultFederateAmb.h"
using namespace std;

class CountryFederateAmb :         public LBSDefaultFederateAmb
{
public:
        CountryFederateAmb();
        virtual ~CountryFederateAmb()
                throw (RTI::FederateInternalError);

			double convertTime( const RTI::FedTime& theTime );


        ////////////////////////////////////
        // Federation Management Services //
        ////////////////////////////////////
        virtual void synchronizationPointRegistrationSucceeded (
                const char *label) // supplied C4)
                throw (
                RTI::FederateInternalError);
         
        virtual void synchronizationPointRegistrationFailed (
                const char *label) // supplied C4)
                throw (
                RTI::FederateInternalError);

        virtual void announceSynchronizationPoint (
                const char *label, // supplied C4
                const char *tag)   // supplied C4
                throw (
                RTI::FederateInternalError);

        virtual void federationSynchronized (
                const char *label) // supplied C4)
                throw (
                RTI::FederateInternalError);


        /////////////////////////////////////
        // Declaration Management Services //
        /////////////////////////////////////

        virtual void startRegistrationForObjectClass (
                RTI::ObjectClassHandle   theClass)      // supplied C1
                throw (
                RTI::ObjectClassNotPublished,
                RTI::FederateInternalError);

        virtual void stopRegistrationForObjectClass (
                RTI::ObjectClassHandle   theClass)      // supplied C1
                throw (
                RTI::ObjectClassNotPublished,
                RTI::FederateInternalError) ;

        virtual void turnInteractionsOn (
                RTI::InteractionClassHandle theHandle) // supplied C1
                throw (
                RTI::InteractionClassNotPublished,
                RTI::FederateInternalError) ;

        virtual void turnInteractionsOff (
                RTI::InteractionClassHandle theHandle) // supplied C1
                throw (
                RTI::InteractionClassNotPublished,
                RTI::FederateInternalError) ;

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
                RTI::FederateInternalError) ;

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
                RTI::FederateInternalError) ;

        virtual void reflectAttributeValues (
                RTI::ObjectHandle                 theObject,     // supplied C1
                const RTI::AttributeHandleValuePairSet& theAttributes, // supplied C4
                const char                             *theTag)        // supplied C4
                throw (
                RTI::ObjectNotKnown,
                RTI::AttributeNotKnown,
                RTI::FederateOwnsAttributes,
                RTI::FederateInternalError) ;

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
                RTI::FederateInternalError) ;

        virtual void receiveInteraction (
                RTI::InteractionClassHandle       theInteraction, // supplied C1
                const RTI::ParameterHandleValuePairSet& theParameters,  // supplied C4
                const char                             *theTag)         // supplied C4
                throw (
                RTI::InteractionClassNotKnown,
                RTI::InteractionParameterNotKnown,
                RTI::FederateInternalError) ;

        virtual void removeObjectInstance (
                RTI::ObjectHandle          theObject, // supplied C1
                const RTI::FedTime&              theTime,   // supplied C4
                const char                      *theTag,    // supplied C4
                RTI::EventRetractionHandle theHandle) // supplied C1
                throw (
                RTI::ObjectNotKnown,
                RTI::InvalidFederationTime,
                RTI::FederateInternalError) ;

        virtual void removeObjectInstance (
                RTI::ObjectHandle          theObject, // supplied C1
                const char                      *theTag)    // supplied C4
                throw (
                RTI::ObjectNotKnown,
                RTI::FederateInternalError) ;

        virtual void provideAttributeValueUpdate (
                RTI::ObjectHandle        theObject,     // supplied C1
                const RTI::AttributeHandleSet& theAttributes) // supplied C4
                throw (
                RTI::ObjectNotKnown,
                RTI::AttributeNotKnown,
                RTI::AttributeNotOwned,
                RTI::FederateInternalError) ;

        virtual void turnUpdatesOnForObjectInstance (
                RTI::ObjectHandle        theObject,     // supplied C1
                const RTI::AttributeHandleSet& theAttributes) // supplied C4
                throw (
                RTI::ObjectNotKnown,
                RTI::AttributeNotOwned,
                RTI::FederateInternalError) ;

        virtual void turnUpdatesOffForObjectInstance (
                RTI::ObjectHandle        theObject,      // supplied C1
                const RTI::AttributeHandleSet& theAttributes) // supplied C4
                throw (
                RTI::ObjectNotKnown,
                RTI::AttributeNotOwned,
                RTI::FederateInternalError) ;

        //////////////////////////////
        // Time Management Services //
        //////////////////////////////

        virtual void timeRegulationEnabled (
                const  RTI::FedTime& theFederateTime) // supplied C4
                throw (
                RTI::InvalidFederationTime,
                RTI::EnableTimeRegulationWasNotPending,
                RTI::FederateInternalError) ;

        virtual void timeConstrainedEnabled (
                const RTI::FedTime& theFederateTime) // supplied C4
                throw (
                RTI::InvalidFederationTime,
                RTI::EnableTimeConstrainedWasNotPending,
                RTI::FederateInternalError) ;

        virtual void timeAdvanceGrant (
                const RTI::FedTime& theTime) // supplied C4
                throw (
                RTI::InvalidFederationTime,
                RTI::TimeAdvanceWasNotInProgress,
                RTI::FederateInternalError) ;

		virtual void	 SetFederateInstance(LBSDefaultFederate* lbsFed);
public:
	void SetCountry(Country *myC)
	{ myCountry = myC; };
private:
	Country *myCountry;

};
#endif