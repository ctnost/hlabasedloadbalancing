#pragma once
using namespace System;

ref class RemoteNodeClient
{
public:
	RemoteNodeClient(void);

	int AskForMigration(String^ ipAddress);
	int HasAnyMigrationRequest(String^ ipAddress);
	String^ GetUtilization(String^ ipAddress);
	String^ GetFederates(String^ ipAddress);
	void SetMigrationStartTime(String^ ipAddress,long migStartTime);
};

