#include <iostream>
#include <stdio.h>
#include "CPU.h"
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
using namespace std;
int main(int argc, char** argv)
{
	if (argc <= 1) {
		printf("Please provide the rom file!\n");
		printf("%s", argv[0]);
		printf(" <rom>\n");
		return 0;
	}
	CPU* cpu = new CPU;

	printf("Trying to open file %s ...\n", argv[1]);
	FILE* f = fopen(argv[1], "rb");
	if (f == NULL) {
		printf("Unable to open file %s\n", argv[1]);
		return 1;
	}
	unsigned char buff[8192];
	int ROMSize = fread(buff, 1, 0xFFF, f);
	fclose(f);

	if (ROMSize == 0)
	{
		printf("Unable to open ROM File: %s\n", argv[1]);
	}
	else
		printf("File loaded\n");
	cpu->LoadInMemory(0x100, buff, ROMSize); //The program loads at offset 0x100
	int i = 0;
	while (true)
	{
		cpu->Step();
	}
	return 0;
}