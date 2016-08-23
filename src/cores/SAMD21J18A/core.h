#ifndef SYSTEM_CORE_H
#define SYSTEM_CORE_H

// Includes
extern "C"
{
	#include <stdint.h>
	#include <stdlib.h>
	#include <string.h>
	#include <math.h>
}

#include <sam.h>
#include "sys_support.h"

extern void Setup();
extern void Loop();

#endif