#pragma once

using namespace System;
//Federelerin ta��nmas� i�lemini ger�ekle�tiren s�n�f.
ref class MigrationManager
{
public:
	MigrationManager(void);

	void MigarateFederate(String^ ipAddress);
};
