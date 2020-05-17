#include "FederationTracer.h"

FederationTracer::FederationTracer(void)
{
}

void FederationTracer::CreateNewFederation()
{
	SqlConnection^ conn = gcnew SqlConnection();
	conn->ConnectionString = "Data Source=ERDEMPC4\\SQLEXPRESS;Initial Catalog=LBS;User ID=sa;Password=dataci";
	conn->Open();

	String^ sqlScript = "INSERT INTO Federation (federationName) VALUES(@name)";

	SqlCommand^ insertCmd = gcnew SqlCommand(sqlScript,conn);
	insertCmd->Parameters->Add("@name","WSN");

	insertCmd->ExecuteNonQuery();
	conn->Close();
}

int FederationTracer::GetLastFederation(SqlConnection^ conn)
{
	int retName = 0;
	String^ sqlScript = "SELECT TOP 1 id FROM Federation ORDER BY id DESC";
	SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = sqlcmd->ExecuteReader(CommandBehavior::SequentialAccess);
	while(reader->Read())
	{
		retName = Convert::ToInt32(reader->GetValue(0));
	}
	reader->Close();
	return retName;
}

void FederationTracer::InsertNewFederate(String^ fedName, long startTime, String^ ip)
{
	SqlConnection^ conn = gcnew SqlConnection();
	conn->ConnectionString = "Data Source=ERDEMPC4\\SQLEXPRESS;Initial Catalog=LBS;User ID=sa;Password=dataci";
	conn->Open();
	int fedid = GetLastFederation(conn);
	String^ sqlScript = "INSERT INTO FederateHistory (federation,federateName,federateStartTime, pcIP) VALUES(@id,@name,@stime,@ip)";

	SqlCommand^ insertCmd = gcnew SqlCommand(sqlScript,conn);
	insertCmd->Parameters->Add("@id",fedid);
	insertCmd->Parameters->Add("@name",fedName);
	insertCmd->Parameters->Add("@stime",startTime);
	insertCmd->Parameters->Add("@ip",ip);
	insertCmd->ExecuteNonQuery();
	conn->Close();
}

void FederationTracer::InsertNewMigration(String^ fedName, long migrationTime, String^ ip)
{
	SqlConnection^ conn = gcnew SqlConnection();
	conn->ConnectionString = "Data Source=ERDEMPC4\\SQLEXPRESS;Initial Catalog=LBS;User ID=sa;Password=dataci";
	conn->Open();
	int fedid = GetFederateId(conn,fedName);
	String^ sqlScript = "INSERT INTO FederateMigrationHistory (federateId,federateMigrationTime,pcIP) VALUES(@fedid,@mTime,@ip)";

	SqlCommand^ insertCmd = gcnew SqlCommand(sqlScript,conn);
	insertCmd->Parameters->Add("@fedid",fedid);
	insertCmd->Parameters->Add("@mTime",migrationTime);
	insertCmd->Parameters->Add("@ip",ip);
	insertCmd->ExecuteNonQuery();
	conn->Close();
}

int FederationTracer::GetFederateId(SqlConnection^ conn, String^ fedName)
{
	int retName = 0;
	String^ sqlScript = "SELECT id FROM FederateHistory Where federateName = @fedName";
	SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
	sqlcmd->Parameters->Add("@fedName",fedName);
	SqlDataReader^ reader = sqlcmd->ExecuteReader(CommandBehavior::SequentialAccess);
	while(reader->Read())
	{
		retName = Convert::ToInt32(reader->GetValue(0));
	}
	reader->Close();
	return retName;
}

void FederationTracer::UpdateFederate(String^ fedName, long finishTime, String^ ip)
{
	SqlConnection^ conn = gcnew SqlConnection();
	conn->ConnectionString = "Data Source=ERDEMPC4\\SQLEXPRESS;Initial Catalog=LBS;User ID=sa;Password=dataci";
	conn->Open();

	String^ sqlScript = "UPDATE FederateHistory SET federateEndTime=@endTime WHERE federateName=@fedName and pcIP=@ip";
	SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
	sqlcmd->Parameters->Add("@endTime",finishTime);
	sqlcmd->Parameters->Add("@fedName",fedName);
	sqlcmd->Parameters->Add("@ip",ip);
	sqlcmd->ExecuteNonQuery();
	conn->Close();
}