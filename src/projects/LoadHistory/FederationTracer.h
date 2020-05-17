#pragma once
using namespace System::Data;
using namespace System::Data::SqlClient;
using namespace System;

public ref class FederationTracer
{
public:
	FederationTracer(void);

	void CreateNewFederation();
	void InsertNewFederate(String^ fedName, long startTime, String^ ip);
	void UpdateFederate(String^ fedName, long finishTime, String^ ip);
	void InsertNewMigration(String^ fedName, long migrationTime, String^ ip);
private:
	int GetLastFederation(SqlConnection^ conn);
	int GetFederateId(SqlConnection^ conn, String^ fedName);
};

