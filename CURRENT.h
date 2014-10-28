/* 
* CURRENT.h
*
* Created: 26/08/2014 14:33:12
* Author: Gopar
*/


#ifndef __CURRENT_H__
#define __CURRENT_H__

#ifdef ARDUINO
	#include "Arduino.h"
	#include "ADS1105.h"
	#define CURR_ACT			2	//PE4 - Pin 6	= 2  // Current Activity // ISR Signal
#endif // ARDUINO


#define ads1115_address		0x48	// ADRR to Ground => 1001000
#define LOADCHANNEL			1	// Ads1115 ADC LOAD current channel
#define CHARGECHANNEL		0	// Ads1115 ADC CHARGE current channel


class CURRENT
{
//variables
public:
protected:
private:
	ADS1115 ads;

//functions
public:
	CURRENT();
	~CURRENT();
	
	void setCurrentComparatorActive(void);
	
	uint32_t getChargeCurrent(void);
	uint32_t getLoadCurrent(void);
	boolean getCurrentActivity(void);
	
	// Methods for calibration features, the number of coefficient for calibration equation are signed integer of 32 bits,
	// but the values are multiplied by 1^5, so we ca use five decimals places.
	void setChargeCalibrationValues(int32_t *values);
	int32_t* getChargeCalibrationValues(void);
	void setLoadCalibrationValues(int32_t *values);
	int32_t* getLoadCalibrationValues(void);
	
	//DEBUG functions
	//Put here only for debug, return to private section when finish
	int16_t _readChargeCurrent(void);
	int16_t _readLoadCurrent(void);
	int16_t _readDifferential(void);

protected:
private:
	CURRENT( const CURRENT &c );
	CURRENT& operator=( const CURRENT &c );
		
	int16_t _readADCChannel(uint8_t channel);
	int16_t _calculateLoadCurrentAverage(void);
	int16_t _calculateChargeCurrentAverage(void);
	boolean _getPinValue(uint8_t pin);
	

}; //Current

#endif //__CURRENT_H__
