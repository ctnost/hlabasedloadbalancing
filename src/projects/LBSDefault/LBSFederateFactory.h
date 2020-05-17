#pragma once

#include "LBSDefaultFederate.h"

public class LBSFederateFactory
{
public:
	virtual LBSDefaultFederate* CreateFederate( const char* fedName ) = 0;
};
