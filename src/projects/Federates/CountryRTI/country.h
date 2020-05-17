#ifndef COUNTRY_H_
#define COUNTRY_H_

#include "RTI.hh"
#include <fedtime.hh>
#include "LBSDefaultFederate.h"
using std::ostream;

class Country;
const int FederateNum = 3;
typedef Country* CountryPtr;

class Country : public LBSDefaultFederate
{
public:
        Country(char* p_type);
        Country(const char* name, const char* populationStr, char* p_type);
        Country(const char* name, const double& population, char* p_type);
        Country(RTI::ObjectHandle id, char* p_type);
        virtual ~Country();

        static Country*  Find(RTI::ObjectHandle objectID);
		//Virtual Methods
		virtual void	 FederateSave(OArchive& arch) const;
		virtual void	 FederateRestore(InArchive& arch);
		virtual void	 RequestInteractions(OArchive& arch) const;
		virtual void	 RestoreInteractions(InArchive& arch) const{}
        virtual void     PublishAndSubscribe();
        virtual void	 Register();
		virtual void     JoinFederation();
		virtual void     Init(RTI::RTIambassador* rtiAmb);

		void			 updateAttributeValues();
		virtual void	 Close(){};
		int				 CheckFederateState();
        void             Update(RTI::FedTime& newTime);
        void             Update(const RTI::AttributeHandleValuePairSet& theAttributes);
        static void		 Update(RTI::InteractionClassHandle theInteraction,const RTI::ParameterHandleValuePairSet& theParameters);

        double&          GetPopulation(){ return m_population; };

        static RTI::ObjectClassHandle      GetCountryRtiID()   { return ms_countryTypeID; };
        static RTI::AttributeHandle        GetNameRtiID()      { return ms_nameTypeID; };
        static RTI::AttributeHandle        GetPopulationRtiID(){ return ms_populationTypeID;};
        static RTI::InteractionClassHandle GetCommRtiID()      { return ms_commTypeID; };
        static RTI::ParameterHandle        GetMessageRtiID()   { return ms_commMsgTypeID;};
        //static RTI::FedTime&               GetLookahead()      { return ms_lookahead; };

		void SetUpdateControl(RTI::Boolean status, const RTI::AttributeHandleSet& attrs);
        void SetPopulation(const double&);

        static void SetInteractionControl(RTI::Boolean status, RTI::InteractionClassHandle theClass);
        static void SetRegistration(RTI::Boolean status) {ms_doRegistry = status; };

        static const double ms_growthRate;
        static CountryPtr   ms_countryExtent[FederateNum];
        static unsigned int ms_extentCardinality;

//protected:
        RTI::AttributeHandleValuePairSet* CreateNVPSet();

private:
        double            m_population;

        RTI::Boolean  hasNameChanged;
        RTI::Boolean  hasPopulationChanged;

        RTI::Boolean  m_sendNameAttrUpdates;
        RTI::Boolean  m_sendPopulationAttrUpdates;

        static RTI::Boolean ms_doRegistry;
        static RTI::Boolean ms_sendCommInteractions;

        //Run-time Type Identification data
        static RTI::ObjectClassHandle      ms_countryTypeID;
        static RTI::AttributeHandle        ms_nameTypeID;
        static RTI::AttributeHandle        ms_populationTypeID;
        static RTI::InteractionClassHandle ms_commTypeID;
        static RTI::ParameterHandle        ms_commMsgTypeID;

        //Names for querying RTTI values
        static char* const  ms_countryTypeStr;
        static char* const  ms_nameTypeStr;
        static char* const  ms_populationTypeStr;
        static char* const  ms_commTypeStr;
        static char* const  ms_commMsgTypeStr;
};
#endif