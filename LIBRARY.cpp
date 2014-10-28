// 
// 
// 

#include "LIBRARY.h"

//--------------------------------
// Integer division with round up
// http://stackoverflow.com/questions/17005364/dividing-two-integers-and-rounding-up-the-result-without-using-floating-pont?lq=1
// Ben Voigt Code in stackoverflow
//---------------------------------

int16_t idiv_ceil ( int16_t numerator, int8_t denominator )
{
	return numerator / denominator
	+ (((numerator < 0) ^ (denominator > 0)) && (numerator%denominator));
}

int32_t idiv_ceil ( int32_t numerator, int8_t denominator )
{
	return numerator / denominator
	+ (((numerator < 0) ^ (denominator > 0)) && (numerator%denominator));
}

// http://jeelabs.org/2011/05/22/atmega-memory-use/

int freeRam () {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}