/* 
* UV_OV_ACT.h
*
* Created: 26/09/2014 12:46:02
* Author: Gopar
*/


#ifndef __UV_OV_ACT_H__
#define __UV_OV_ACT_H__

#include "FILTER.h"
#include "STATE.h"


class UV_OV_ACT : public FILTER
{
//variables
public:
protected:
private:
	int16_t *_values;
	int16_t _OV_value;
	int16_t _UV_value;
	uint8_t _n_values;
	uint8_t _factor;
	uint8_t *_burn_time;
	int16_t _ltc_temp;

//functions
public:
	UV_OV_ACT();
	UV_OV_ACT(uint8_t n_voltages, int16_t OV_value, int16_t UV_value);
	~UV_OV_ACT();
	virtual int16_t *getFiltered(void);
	virtual uint8_t getNumberofFilteredValues(void);

	virtual void calculate(STATE *state);
	
protected:
private:
	UV_OV_ACT( const UV_OV_ACT &c );
	UV_OV_ACT& operator=( const UV_OV_ACT &c );

}; //UV_OV_ACT

#endif //__UV_OV_ACT_H__
