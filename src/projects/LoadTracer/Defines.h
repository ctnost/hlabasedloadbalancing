#pragma once

#define LOAD_PERIOD 1000	// Algoritmaya göre 1 sn aralýklarla yük izlenir.
#define LOAD_NUMBER 5		// 5 kez hesap tekrarlanýr.
#define LOAD_W 100			// Geniþletmek için

#define LOAD_HT 1.2			// Yük yüksek eþik deðeri
#define LOAD_LT 0.7			// Yük alçak eþik deðeri

enum LOAD_TYPES
{
	CPU,	//CPU
	CPUQ,	//CPU Queue	
	MEM,	//Memory
	BW		//BandWith
};

enum LOAD_STATES
{
	ST_NORMAL,
	ST_HIGH,
	ST_LOW,
	ST_IDLE
};