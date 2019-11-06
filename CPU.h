#include <Windows.h>
#pragma once
class CPU {
public:
	CPU();
	virtual ~CPU();

	void LoadInMemory(short int addr, unsigned char* buffer, short int size);
	void Step();
private:
	unsigned char RAM[4096] = { 0 }; //PC is has a 12 bit length, and 2^12 = 2048
	unsigned char A;
	unsigned short PC; // 2 bytes (well, the PC needs only 12 bits but why bother defining a struct =))
	unsigned long clockspeed;
	unsigned long lastclock;
	bool Locked[0xFFF];
	PDWORD lpfOldProtect;

};