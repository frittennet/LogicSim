#pragma once

#include <stdio.h> 

// #define DEBUG 

#ifdef DEBUG 
	#define debugPrint printf
#else
	#define debugPrint __noop
#endif