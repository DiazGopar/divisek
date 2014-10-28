// LIBRARY.h

#ifndef _LIBRARY_h
#define _LIBRARY_h

#if defined(ARDUINO)
	#if ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
	#endif
#endif

int32_t idiv_ceil ( int32_t numerator, int8_t denominator );
int16_t idiv_ceil ( int16_t numerator, int8_t denominator );

int freeRam ();

#endif

