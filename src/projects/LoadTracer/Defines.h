#pragma once

#define LOAD_PERIOD 1000	// Algoritmaya g�re 1 sn aral�klarla y�k izlenir.
#define LOAD_NUMBER 5		// 5 kez hesap tekrarlan�r.
#define LOAD_W 100			// Geni�letmek i�in

#define LOAD_HT 1.2			// Y�k y�ksek e�ik de�eri
#define LOAD_LT 0.7			// Y�k al�ak e�ik de�eri

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