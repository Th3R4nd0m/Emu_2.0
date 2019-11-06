#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "CPU.h"
#include <memoryapi.h>
CPU::CPU() {
	memset(RAM, 0, sizeof(RAM));
	A = 0x0;
	PC = 0x100;
	Locked[0xFFF] = { 0 };
}
void CPU::LoadInMemory(short int addr, unsigned char* buffer, short int size)
{
	memcpy(RAM + addr, buffer, size);
}
void CPU::Step()
{
	short int Code = RAM[PC] / 0x10; // |Code| |N|  |X| |Y|
	short int N = RAM[PC] % 0x10;
	short int X = RAM[PC + 1] / 0x10;
	short int Y = RAM[PC + 1] % 0x10;
	//printf("Code %x, PC %x, N %x, X %x, Y %x, A %x\n", Code, PC, N, X, Y, A);
	switch (Code)
	{
	// Arithmetics
	case 0x0:
		if (N == 0x00)
		{
			A += (X * 0x10 + Y); // adds XY to A
			PC += 2;
		}
		else if (N == 0x01)
		{
			A = (X * 0x10 + Y); // sets A to XY
			PC += 2;
		}
		else if (N == 0x02)
		{
			A ^= (X * 0x10 + Y);// XORs A with XY
			PC += 2;
		}
		else if (N == 0x03)
		{
			A |= (X * 0x10 + Y);// ORs A with XY
			PC += 2;
		}
		else if (N == 0x04)
		{
			A &= (X * 0x10 + Y);// ANDs A with XY
			PC += 2;
		}
		else
		{
			--A; // decrements A
			PC += 2;
		}
		break;

	case 0x8:
		A = RAM[(N * 0x100 + X * 0x10 + Y)]; //sets A to byte at address 0xNXY
		PC += 2;
		break;

	case 0xD:
		if (!Locked[N * 0x100 + X * 0x10 + Y])
			RAM[(N * 0x100 + X * 0x10 + Y)] ^= A ; // XORs A with byte at address 0xNXY
		PC += 2;
		break;

	case 0xF:
		if (!Locked[N * 0x100 + X * 0x10 + Y])
		{
			RAM[(N * 0x100 + X * 0x10 + Y)] = A; // Sets bytes at address 0xNXY to A
		}
		PC += 2;
		break;
	
	// Output
	case 0x1:
		if (N == 0x3 && X == 0x3 && Y == 0x7)
		{
			printf("%c", A);
		}
		else
			--A;
		PC += 2;
		break;

	//Control flow
	case 0x2:
		PC = (N * 0x100 + X * 0x10 + Y); // jmp to 0xNXY
		break;

	case 0x3:
		if (A == 0)
		{
			PC = (N * 0x100 + X * 0x10 + Y); // jmp to 0xNXY
		}
		else
			PC += 2;
		break;

	case 0x4:
		if (A == 1)
		{
			PC = (N * 0x100 + X * 0x10 + Y);// jmp to 0xNXY
		}
		else
			PC += 2;
		break;

	case 0x5:
		if (A == 0xFF)
		{
			PC = (N * 0x100 + X * 0x10 + Y); //jmp to 0xNXY
		}
		else
			PC += 2;
		break;

	case 0x6: // comparisions
		if (N == 0x0)
		{
			if (A > (X * 0x10 + Y))
			{
				A = 0xFF;
			}
			else if (A < (X * 0x10 + Y))
			{
				A = 1;
			}
			else //equal
				A = 0;
		}
		else
			--A;
		PC += 2;
		break;

	case 0x7: //comparisions with value in memory
		if (A > RAM[N * 0x100 + X * 0x10 + Y])
			A = 0xFF;
		else if (A < RAM[N * 0x100 + X * 0x10 + Y])
			A = 1;
		else // equal
			A = 0;
		PC += 2;
		break;

	case 0xB:
		if (N == 0xE && X == 0xE && Y == 0xF) // opcode is 0xBEEF
		{
			PC = 0x100;
			A = 0x42;
		}
		else {
			--A;
			PC += 2;
		}
		break;
	
	// Security
	case 0x9: // block writes to specific address
		Locked[N * 0x100 + X * 0x10 + Y] = 1;
		PC += 2;
		break;

	case 0xA: //unblock writes to specific address
		Locked[N * 0x100 + X * 0x10 + Y] = 0;
		PC += 2;
		break;

	case 0xC:
		if(!Locked[N*0x100+X*0x10+Y])
			RAM[N * 0x100 + X * 0x10 + Y] ^= 0x42;
		PC += 2;
		break;

	case 0xE:
		if (N == 0xE && X == 0xE && Y == 0xE)
		{
			PC = PC;
		}
		else
			--A;
		PC += 2;
		break;

	default:
		--A;
		PC += 2;
	}
}
CPU::~CPU(){

}