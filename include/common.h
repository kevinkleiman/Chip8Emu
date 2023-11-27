#pragma once

#include <iostream>

#define FOREGROUND_LAYER 0
#define BACKGROUND_LAYER 1
#define RESOLUTION_WIDTH 64
#define RESOLUTION_HEIGHT 32

#define ROM_ENTRY 0x200

#define DEBUG_LOG(S) \
	std::cout << "EMU LOG:::" << S << std::endl; 

#define PRINT_HEX(X) \
	std::cout << std::hex<< "HEX VALUE:::" << X << std::endl; 

#define NO_IMPL(S)								 \
do {												 \
	std::cout << "ERROR:: Instruction" << S << " not yet implemented" << std::endl; \
	exit(1) \
} while(0)

