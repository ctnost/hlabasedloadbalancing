#ifndef WSN_H_
#define WSN_H_

#include "RTI.hh"
#include <fedtime.hh>
#include "LBSDefaultFederate.h"
#include "WirelessSensor.h"
#include <map>
using std::ostream;

class WSN : public LBSDefaultFederate
{
public:
        WSN(char* p_type);
        virtual ~WSN();

		//Virtual Methods
		virtual void	 FederateSave(OArchive& arch) const{}
		virtual void	 FederateRestore(InArchive& arch){}
		virtual void	 RequestInteractions(OArchive& arch) const{}
		virtual void	 RestoreInteractions(InArchive& arch){}
		virtual void	 ProcessInteractions(){}
		virtual void     PublishAndSubscribe();
		virtual void	 Register();
		virtual void     JoinFederation(){}
		virtual void     Init(RTI::RTIambassador* rtiAmb);
		virtual void	 Close(){}

		void			 updateAttributeValues();
		int				 CheckFederateState();
        void             Update(RTI::FedTime& newTime);
        void             Update(const RTI::AttributeHandleValuePairSet& theAttributes,RTI::ObjectHandle objectID);
		void             Update(const RTI::AttributeHandleValuePairSet& theAttributes);
        static void		 Update(RTI::InteractionClassHandle theInteraction,const RTI::ParameterHandleValuePairSet& theParameters);

        static RTI::ObjectClassHandle      GetWSNRtiID()   { return ms_WSNTypeID; };
        static RTI::AttributeHandle        GetNameRtiID()      { return ms_nameTypeID; };
        static RTI::InteractionClassHandle GetCommRtiID()      { return ms_commTypeID; };
        static RTI::ParameterHandle        GetMessageRtiID()   { return ms_commMsgTypeID;};
        //static RTI::FedTime&               GetLookahead()      { return ms_lookahead; };

		void SetUpdateControl(RTI::Boolean status, const RTI::AttributeHandleSet& attrs);
		WirelessSensor*  Find(RTI::ObjectHandle objectID);
		void Insert(RTI::ObjectHandle objectID, WirelessSensor* ws);
		void Remove(RTI::ObjectHandle objectID);

        static void SetInteractionControl(RTI::Boolean status, RTI::InteractionClassHandle theClass);

        static const double ms_growthRate;
        static unsigned int ms_extentCardinality;
private:
		System::String^ GetIP();
public:
		static map<RTI::ObjectHandle,WirelessSensor*> wSensors;
private:
        //Run-time Type Identification data
        static RTI::ObjectClassHandle      ms_WSNTypeID;
        static RTI::AttributeHandle        ms_nameTypeID;
        static RTI::AttributeHandle        ah_posx;
		static RTI::AttributeHandle        ah_posy;
		static RTI::AttributeHandle        ah_power;
		static RTI::AttributeHandle        ah_rc;
		static RTI::AttributeHandle        ah_sc;
        static RTI::InteractionClassHandle ms_commTypeID;
        static RTI::ParameterHandle        ms_commMsgTypeID;

        //Names for querying RTTI values
        static char* const  ms_WSNTypeStr;
        static char* const  ms_nameTypeStr;
        static char* const  ah_posxTypeStr;
		static char* const  ah_posyTypeStr;
		static char* const  ah_powerTypeStr;
		static char* const  ah_rcTypeStr;
		static char* const  ah_scTypeStr;
        static char* const  ms_commTypeStr;
        static char* const  ms_commMsgTypeStr;
};
#endif