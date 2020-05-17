#include "LoadTracer.h"

using namespace LoadHistory;

LoadTracer::LoadTracer(void)
{
	conn = gcnew SqlConnection();
}

LoadTracer::~LoadTracer(void)
{
	DisconnectToDb();
}

void LoadTracer::DisconnectToDb()
{
	conn->Close();
}

void LoadTracer::ConnectToDb()
{
	conn->ConnectionString = "Data Source=.\\SQLEXPRESS;Initial Catalog=LBS;Integrated Security=True";
	conn->Open();
}

List<double>^ LoadTracer::GetCPUWeights()
{
	String^ sqlScript = "SELECT cpuWeight.cpuWeight FROM pcNode\
						INNER JOIN cpuWeight On cpuWeight.pcId = pcNode.id\
						Where pcNode.active = 'True' and\
						cpuWeight.id IN ( SELECT MAX(id) FROM cpuWeight GROUP BY pcId )";

	List<double>^ ret = gcnew List<double>();
	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	while(reader->Read())
	{
		ret->Add(Convert::ToDouble(reader->GetValue(0)));
	}
	reader->Close();
	return ret;
}

List<double>^ LoadTracer::GetCPUUsages()
{
	String^ sqlScript = "SELECT cpuLoad.cpuLoad FROM pcNode\
						INNER JOIN cpuLoad On cpuLoad.pcId = pcNode.id\
						Where pcNode.active = 'True' and\
						cpuLoad.id IN ( SELECT MAX(id) FROM cpuLoad GROUP BY pcId )";

	List<double>^ ret = gcnew List<double>();
	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	while(reader->Read())
	{
		ret->Add(Convert::ToDouble(reader->GetValue(0)));
	}
	reader->Close();
	return ret;
}

List<double>^ LoadTracer::GetMEMWeights() 
{
	String^ sqlScript = "SELECT memWeight.memWeight FROM pcNode\
						INNER JOIN memWeight On memWeight.pcId = pcNode.id\
						Where pcNode.active = 'True' and\
						memWeight.id IN ( SELECT MAX(id) FROM memWeight GROUP BY pcId )";

	List<double>^ ret = gcnew List<double>();
	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	while(reader->Read())
	{
		ret->Add(Convert::ToDouble(reader->GetValue(0)));
	}
	reader->Close();
	return ret;
}

List<double>^ LoadTracer::GetMEMUsages() 
{
	String^ sqlScript = "SELECT memLoad.memLoad FROM pcNode\
						INNER JOIN memLoad On memLoad.pcId = pcNode.id\
						Where pcNode.active = 'True' and\
						memLoad.id IN ( SELECT MAX(id) FROM memLoad GROUP BY pcId )";

	List<double>^ ret = gcnew List<double>();
	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	while(reader->Read())
	{
		ret->Add(Convert::ToDouble(reader->GetValue(0)));
	}
	reader->Close();
	return ret;
}

List<double>^ LoadTracer::GetNETWeights()
{
	String^ sqlScript = "SELECT netWeight.netWeight FROM pcNode\
						INNER JOIN netWeight On netWeight.pcId = pcNode.id\
						Where pcNode.active = 'True' and\
						netWeight.id IN ( SELECT MAX(id) FROM netWeight GROUP BY pcId )";

	List<double>^ ret = gcnew List<double>();
	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	while(reader->Read())
	{
		ret->Add(Convert::ToDouble(reader->GetValue(0)));
	}
	reader->Close();
	return ret;
}

double LoadTracer::GetCPUMaxLoad()
{
	String^ sqlScript = "SELECT MAX(cpuLoad.cpuLoad) FROM pcNode\
						INNER JOIN cpuLoad On cpuLoad.pcId = pcNode.id\
						Where pcNode.active = 'True' and\
						cpuLoad.id IN ( SELECT MAX(id) FROM cpuLoad GROUP BY pcId )";

	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	double ret = 0;
	if(reader->Read())
	{
		ret = Convert::ToDouble(reader->GetValue(0));
	}
	reader->Close();
	return ret;
}

double LoadTracer::GetMEMMaxLoad()
{
	String^ sqlScript = "SELECT MAX(memLoad.memLoad) FROM pcNode\
						INNER JOIN memLoad On memLoad.pcId = pcNode.id\
						Where pcNode.active = 'True' and\
						memLoad.id IN ( SELECT MAX(id) FROM memLoad GROUP BY pcId )";

	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	double ret = 0;
	if(reader->Read())
	{
		ret = Convert::ToDouble(reader->GetValue(0));
	}
	reader->Close();
	return ret;
}

double LoadTracer::GetNETMaxLoad()
{
	String^ sqlScript = "SELECT MAX(netLoad.netLoad) FROM pcNode\
						INNER JOIN netLoad On netLoad.pcId = pcNode.id\
						Where pcNode.active = 'True' and\
						netLoad.id IN ( SELECT MAX(id) FROM netLoad GROUP BY pcId )";

	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	double ret = 0;
	if(reader->Read())
	{
		ret = Convert::ToDouble(reader->GetValue(0));
	}
	reader->Close();
	return ret;
}

void LoadTracer::SetCPULoad(double load, String^ pcIP)
{
	String^ sqlScript = "INSERT INTO cpuLoad (cpuLoad, pcId)\
						SELECT @cpuLoad,pcNode.id FROM pcNode Where pcNode.pcIP = @pcIP";

	SqlCommand^ insertCmd = gcnew SqlCommand(sqlScript,conn);
	insertCmd->Parameters->Add("@cpuLoad",load);
	insertCmd->Parameters->Add("@pcIP",pcIP);

	insertCmd->ExecuteNonQuery();
}

void LoadTracer::SetCPUQueue(double load, String^ pcIP)
{
	String^ sqlScript = "INSERT INTO cpuQueue (cpuQueue, pcId)\
						SELECT @cpuQueue,pcNode.id FROM pcNode Where pcNode.pcIP = @pcIP";

	SqlCommand^ insertCmd = gcnew SqlCommand(sqlScript,conn);
	insertCmd->Parameters->Add("@cpuQueue",load);
	insertCmd->Parameters->Add("@pcIP",pcIP);

	insertCmd->ExecuteNonQuery();
}

void LoadTracer::SetCPUWeight(double weight, String^ pcIP)
{
	String^ sqlScript = "INSERT INTO cpuWeight (cpuWeight, pcId)\
						SELECT @cpuWeight,pcNode.id FROM pcNode Where pcNode.pcIP = @pcIP";

	SqlCommand^ insertCmd = gcnew SqlCommand(sqlScript,conn);
	insertCmd->Parameters->Add("@cpuWeight",weight);
	insertCmd->Parameters->Add("@pcIP",pcIP);

	insertCmd->ExecuteNonQuery();
}
void LoadTracer::SetMEMLoad(double load, String^ pcIP)
{
	String^ sqlScript = "INSERT INTO memLoad (memLoad, pcId)\
						SELECT @memLoad,pcNode.id FROM pcNode Where pcNode.pcIP = @pcIP";

	SqlCommand^ insertCmd = gcnew SqlCommand(sqlScript,conn);
	insertCmd->Parameters->Add("@memLoad",load);
	insertCmd->Parameters->Add("@pcIP",pcIP);

	insertCmd->ExecuteNonQuery();
}


void LoadTracer::SetMEMWeight(double weight, String^ pcIP) 
{
	String^ sqlScript = "INSERT INTO memWeight (memWeight, pcId)\
						SELECT @memWeight,pcNode.id FROM pcNode Where pcNode.pcIP = @pcIP";

	SqlCommand^ insertCmd = gcnew SqlCommand(sqlScript,conn);
	insertCmd->Parameters->Add("@memWeight",weight);
	insertCmd->Parameters->Add("@pcIP",pcIP);

	insertCmd->ExecuteNonQuery();
}

void LoadTracer::SetNETLoad(double load, String^ pcIP)
{
	String^ sqlScript = "INSERT INTO netLoad (netLoad, pcId)\
						SELECT @netLoad,pcNode.id FROM pcNode Where pcNode.pcIP = @pcIP";

	SqlCommand^ insertCmd = gcnew SqlCommand(sqlScript,conn);
	insertCmd->Parameters->Add("@netLoad",load);
	insertCmd->Parameters->Add("@pcIP",pcIP);

	insertCmd->ExecuteNonQuery();
}
void LoadTracer::SetNETWeight(double weight, String^ pcIP)
{
	String^ sqlScript = "INSERT INTO netWeight (netWeight, pcId)\
						SELECT @netWeight,pcNode.id FROM pcNode Where pcNode.pcIP = @pcIP";

	SqlCommand^ insertCmd = gcnew SqlCommand(sqlScript,conn);
	insertCmd->Parameters->Add("@netWeight",weight);
	insertCmd->Parameters->Add("@pcIP",pcIP);

	insertCmd->ExecuteNonQuery();
}

List<String^>^ LoadTracer::GetActivePCList()
{
	String^ sqlScript = "SELECT * from pcNode Where active='true'";

	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	List<String^>^ ret = gcnew List<String^>();
	while(reader->Read())
	{
		ret->Add(reader->GetValue(2)->ToString());
	}
	reader->Close();
	return ret;
}

bool LoadTracer::IsPCInDB(String^ node)
{
	String^ sqlScript = "SELECT * from pcNode Where pcIp=@pcIP";

	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	get->Parameters->Add("@pcIP",node);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	bool inDb = false;
	while(reader->Read()){
		inDb = true;
		break;
	}
	reader->Close();
	return inDb;
}

void LoadTracer::SetDeActivePCList(List<String^>^ pcList)
{
	for each(String^ ip in pcList){
		String^ sqlScript;
		if(IsPCInDB(ip)){
			sqlScript = "UPDATE pcNode SET active='false' Where pcIP = @pcIP";
		}else{
			sqlScript = "INSERT INTO pcNode (active,pcIP) VALUES('false',@pcIP)";
		}
		SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
		sqlcmd->Parameters->Add("@pcIP",ip);
		sqlcmd->ExecuteNonQuery();
	}
}
void LoadTracer::SetActivePCList(List<String^>^ pcList)
{
	for each(String^ ip in pcList){
		String^ sqlScript;
		if(IsPCInDB(ip)){
			sqlScript = "UPDATE pcNode SET active='true' Where pcIP = @pcIP";
		}else{
			sqlScript = "INSERT INTO pcNode (active,pcIP) VALUES('true',@pcIP)";
		}
		SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
		sqlcmd->Parameters->Add("@pcIP",ip);
		sqlcmd->ExecuteNonQuery();
	}
}

List<List<String^>^>^ LoadTracer::GetActivePCInfo()
{
	List<List<String^>^>^ pcInfo = gcnew List<List<String^>^>();
	String^ sqlScript = "SELECT * from pcNode Where active='true'";

	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	while(reader->Read())
	{
		List<String^>^ ret = gcnew List<String^>();
		ret->Add(reader->GetValue(0)->ToString());
		ret->Add(reader->GetValue(2)->ToString());
		pcInfo->Add(ret);
	}
	reader->Close();
	return pcInfo;
}

List<String^>^ LoadTracer::GetFederateListByIp(String^ pcIP)
{
	String^ sqlScript = "Select FT.FederateObjId from FederateTable FT \
							inner join pcNode on FT.PcNodeId = pcNode.id \
							inner join FederationTable on FT.FederationId = FederationTable.id \
							Where PcNode.pcIP =	@pcIP and FederationTable.id = (Select MAX(FederationTable.id) from FederationTable)";

	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	get->Parameters->Add("@pcIP",pcIP);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	List<String^>^ ret = gcnew List<String^>();
	while(reader->Read())
	{
		ret->Add(reader->GetValue(0)->ToString());
	}
	reader->Close();
	return ret;
}
void LoadTracer::InsertFederation(String ^tag)
{
	String^ sqlScript = "INSERT INTO FederationTable (tag) VALUES(@tag)";

	SqlCommand^ insertCmd = gcnew SqlCommand(sqlScript,conn);
	insertCmd->Parameters->Add("@tag",tag);

	insertCmd->ExecuteNonQuery();
}

bool LoadTracer::QueryFederate4Update(int federationId, String^ fedId)
{
	String^ sqlScriptforSelect = "SELECT * FROM FederateTable Where \
									FederateObjId = @fedId and FederationId = @federationId";

	SqlCommand^ get = gcnew SqlCommand(sqlScriptforSelect,conn);
	get->Parameters->Add("@fedId",fedId);
	get->Parameters->Add("@federationId",federationId);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	List<String^>^ ret = gcnew List<String^>();
	while(reader->Read())
	{
		ret->Add(reader->GetValue(0)->ToString());
	}
	reader->Close();
	if(ret->Count > 0)
		return true;
	return false;
}

List<String^>^ LoadTracer::GetFederationAndIpID(String ^ip)
{
	String^ sqlScriptforSelect = "SELECT Max(FederationTable.id) FROM FederationTable UNION ALL \
								 SELECT pcNode.id FROM pcNode Where pcNode.pcIP = @pcIP";

	SqlCommand^ get = gcnew SqlCommand(sqlScriptforSelect,conn);
	get->Parameters->Add("@pcIP",ip);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	List<String^>^ ret = gcnew List<String^>();
	while(reader->Read())
	{
		ret->Add(reader->GetValue(0)->ToString());
	}
	reader->Close();
	return ret;
}

void LoadTracer::InsertFederate(String^ fedId, String^ pcIp)
{
	List<String^>^ ret = GetFederationAndIpID(pcIp);
	String^ sqlScript;
	//Eger Federe diger pc uzerindeyse update edilir.
	if(QueryFederate4Update(Convert::ToInt32(ret[0]),fedId)){
		sqlScript = "UPDATE FederateTable SET PcNodeId=@PcNodeId \
					Where FederateObjId = @FederateObjId and FederationId = @FederationId";
	}else{
		sqlScript = "INSERT INTO FederateTable (FederateObjId,PcNodeId,FederationId,CanMigrate)\
									VALUES(@FederateObjId,@PcNodeId,@FederationId,'true')";
	}
	SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
	sqlcmd->Parameters->Add("@FederateObjId",fedId);
	sqlcmd->Parameters->Add("@FederationId",Convert::ToInt32(ret[0]));
	sqlcmd->Parameters->Add("@PcNodeId",Convert::ToInt32(ret[1]));
	sqlcmd->ExecuteNonQuery();
}

void LoadTracer::RemoveFederate(String^ fedId, String^ pcIp)
{
	List<String^>^ ret = GetFederationAndIpID(pcIp);
	String^ sqlScript = "Delete From FederateTable Where FederateObjId = @FederateObjId and FederationId = @FederationId and FederateObjId=@FederateObjId";
	
	SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
	sqlcmd->Parameters->Add("@FederateObjId",fedId);
	sqlcmd->Parameters->Add("@FederationId",Convert::ToInt32(ret[0]));
	sqlcmd->Parameters->Add("@PcNodeId",Convert::ToInt32(ret[1]));
	sqlcmd->ExecuteNonQuery();
}

List<String^>^ LoadTracer::GetMigratableFederates(String^ pcIP, List<String^>^ activeFedList)
{
	String^ activeFedListSelect="";
	for each(String^ fedName in activeFedList){
		activeFedListSelect += "'"+fedName+"',";
	}
	if (activeFedListSelect != ""){
		activeFedListSelect = activeFedListSelect->Substring(0,activeFedListSelect->Length-1);
		activeFedListSelect = " and FT.FederateObjId IN("+activeFedListSelect+")";
	}
	

	String^ sqlScript = "Select FT.FederateObjId from FederateTable FT \
						inner join pcNode on FT.PcNodeId = pcNode.id \
						inner join FederationTable on FT.FederationId = FederationTable.id \
						Where PcNode.pcIP =	@pcIP and FederationTable.id = (Select MAX(FederationTable.id) from FederationTable) and FT.CanMigrate = 'True'";
	sqlScript += activeFedListSelect;
	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	get->Parameters->Add("@pcIP",pcIP);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	List<String^>^ ret = gcnew List<String^>();
	while(reader->Read())
	{
		ret->Add(reader->GetValue(0)->ToString());
	}
	reader->Close();
	return ret;
}

void LoadTracer::UpdateFederateMigrateStatus(String^ fedId, String^ pcIp, bool canMigrate)
{
	List<String^>^ ret = GetFederationAndIpID(pcIp);
	String^ sqlScript = "UPDATE FederateTable SET CanMigrate=@CanMigrate \
					Where FederateObjId = @FederateObjId and FederationId = @FederationId and FederateObjId = @FederateObjId ";
	SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
	sqlcmd->Parameters->Add("@FederateObjId",fedId);
	sqlcmd->Parameters->Add("@FederationId",Convert::ToInt32(ret[0]));
	sqlcmd->Parameters->Add("@PcNodeId",Convert::ToInt32(ret[1]));
	sqlcmd->Parameters->Add("@CanMigrate",canMigrate);
	sqlcmd->ExecuteNonQuery();
}

void LoadTracer::UpdateFederateLife(String^ fedId, String^ pcIp, double fedLife)
{
	List<String^>^ ret = GetFederationAndIpID(pcIp);
	String^ sqlScript = "UPDATE FederateTable SET FederateLife=@FederateLife \
						Where FederateObjId = @FederateObjId and FederationId = @FederationId and FederateObjId = @FederateObjId ";
	SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
	sqlcmd->Parameters->Add("@FederateObjId",fedId);
	sqlcmd->Parameters->Add("@FederationId",Convert::ToInt32(ret[0]));
	sqlcmd->Parameters->Add("@PcNodeId",Convert::ToInt32(ret[1]));
	sqlcmd->Parameters->Add("@FederateLife",fedLife);
	sqlcmd->ExecuteNonQuery();
}

void LoadTracer::UpdateFederateUsages(String^ fedId, String^ pcIp, double cpuU, double memU)
{
	List<String^>^ ret = GetFederationAndIpID(pcIp);
	String^ sqlScript = "UPDATE FederateTable SET CPUUsage=@CPUUsage, MEMUsage=@MEMUsage \
						Where FederateObjId = @FederateObjId and FederationId = @FederationId";
	SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
	sqlcmd->Parameters->Add("@FederateObjId",fedId);
	sqlcmd->Parameters->Add("@FederationId",Convert::ToInt32(ret[0]));
	sqlcmd->Parameters->Add("@CPUUsage",cpuU);
	sqlcmd->Parameters->Add("@MEMUsage",memU);
	sqlcmd->ExecuteNonQuery();
}

List<double>^ LoadTracer::GetFederateResourceUsages(String^ fedId, String^ pcIp)
{
	List<String^>^ ret = GetFederationAndIpID(pcIp);
	String^ sqlScript = "SELECT CPUUsage,MEMUsage FROM FederateTable\
						Where FederateObjId = @FederateObjId and FederationId = @FederationId ";
	List<double>^ retFRU = gcnew List<double>();
	SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
	sqlcmd->Parameters->Add("@FederateObjId",fedId);
	sqlcmd->Parameters->Add("@FederationId",Convert::ToInt32(ret[0]));
	//sqlcmd->Parameters->Add("@PcNodeId",Convert::ToInt32(ret[1]));
	SqlDataReader^ reader = sqlcmd->ExecuteReader(CommandBehavior::SequentialAccess);
	while(reader->Read())
	{
		retFRU->Add(Convert::ToDouble(reader->GetValue(0)));
		retFRU->Add(Convert::ToDouble(reader->GetValue(1)));
	}
	reader->Close();
	return retFRU;
}

double LoadTracer::GetFederateLife(String^ fedId, String^ pcIp)
{
	List<String^>^ ret = GetFederationAndIpID(pcIp);
	String^ sqlScript = "SELECT FederateLife FROM FederateTable\
						Where FederateObjId = @FederateObjId and FederationId = @FederationId ";
	double retFL = -1;
	SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
	sqlcmd->Parameters->Add("@FederateObjId",fedId);
	sqlcmd->Parameters->Add("@FederationId",Convert::ToInt32(ret[0]));
	sqlcmd->Parameters->Add("@PcNodeId",Convert::ToInt32(ret[1]));
	SqlDataReader^ reader = sqlcmd->ExecuteReader(CommandBehavior::SequentialAccess);
	while(reader->Read())
	{
		if(reader->IsDBNull(0))
			break;
		retFL = Convert::ToDouble(reader->GetValue(0));
	}
	reader->Close();
	return retFL;
}

void LoadTracer::UpdateMigrationTable(long migTime)
{
	String^ sqlScript = "UPDATE MigrationTable SET migrationTime=@migTime";
	SqlCommand^ sqlcmd = gcnew SqlCommand(sqlScript,conn);
	sqlcmd->Parameters->Add("@migTime",migTime);
	sqlcmd->ExecuteNonQuery();
}

long LoadTracer::GetMigrationTime()
{
	String^ sqlScript = "SELECT * FROM MigrationTable";
	long ret = 0;
	SqlCommand^ get = gcnew SqlCommand(sqlScript,conn);
	SqlDataReader^ reader = get->ExecuteReader(CommandBehavior::SequentialAccess);
	while(reader->Read())
	{
		ret = Convert::ToInt64(reader->GetValue(0));
	}
	reader->Close();
	return ret;
}