#pragma once

using namespace System;
//Federelerin taþýnmasý iþlemini gerçekleþtiren sýnýf.
ref class MigrationManager
{
public:
	MigrationManager(void);

	void MigarateFederate(String^ ipAddress);
};
