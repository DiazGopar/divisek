/* 
* Current.cpp
*
* Created: 26/08/2014 14:33:12
* Author: Gopar
*/

#include "CURRENT.h"
#include "EEPROMEx/EEPROMex.h"
#include "LIBRARY.h"


// default constructor
CURRENT::CURRENT() : ads(ads1115_address)
{
	#ifdef ARDUINO
		pinMode(CURR_ACT, INPUT);
		digitalWrite(CURR_ACT, HIGH); //Turn ON pullup resistor
	#endif // ARDUINO

	ads.begin();
	ads.setGain(GAIN_TWO);   // +-2048 mv
	
} //Current

// default destructor
CURRENT::~CURRENT()
{
} //~Current

/**
 * \brief Get the value in mA from the Load channel
 * 
 * \param [void ]
 * 
 * \return [uint32_t] The actual current value in mA
 */
uint32_t CURRENT::getLoadCurrent(void)
{
	uint32_t current;
	current = _readLoadCurrent();
	current = (current > 32768) ? 0 : current;
	//ads.  TODO:
	
}

/**
 * \brief Get the Status of ALERT/READY pin 
 * 
 * \param [void ]
 * 
 * \return [boolean] True if pin is in LOW, false if HIGH (This pin is pull-up
 */
boolean CURRENT::getCurrentActivity(void)
{
	return(!_getPinValue(CURR_ACT));	
}

/**
 * \brief Set the ADS1115 in Comparator dual window mode
 * 
 * \param [void ]
 * 
 * \return [void]
 */
void CURRENT::setCurrentComparatorActive(void)
{
	ads.startComparator_Diferential(0,10,-10); //TODO: Values mus be refined, they are random at this time
}

/**
 * \brief Set the values for the calibration equation (We use 4 values for third order equations. We saved in EEPROM memory
 * 
 * \param [int32_t * values] 
 * 
 * \return [void]
 */
void CURRENT::setChargeCalibrationValues(int32_t *values)
{
	EEPROM.writeBlock(1,values,4);					
}


/**
 * \brief Read ads1115 Charge current channel in mv 
 * 
 * \param [void ]
 * 
 * \return [int16_t] 16bits value read from ads1115 channel 0
 */
int16_t CURRENT::_readChargeCurrent(void)
{
	int16_t measure;
	
	measure = _readADCChannel(CHARGECHANNEL);

	return(measure);	
}

/**
 * \brief Read ads1115 Load current channel in mv
 * 
 * \param [void ]
 * 
 * \return [int16_t] 16bits value read from ads1115 channel 1
 */
int16_t CURRENT::_readLoadCurrent(void)
{
	int16_t measure;
	
	measure = _readADCChannel(LOADCHANNEL);
		
	return(measure);	
}


/**
 * \brief Read ADC channel number from object ads 
 * 
 * \param [uint8_t channel] channel number to read [0..3]
 * 
 * \return [int16_t] 16 bits value from channel
 */
int16_t CURRENT::_readADCChannel(uint8_t channel)
{
	return(ads.readADC_SingleEnded(channel));	
}

/**
 * \brief Get the Pin value (True for HIGH state, False for LOW)
 * 
 * \param [uint8_t pin] Pin number
 * 
 * \return [boolean] True for HIGH, False for LOW
 */
boolean CURRENT::_getPinValue(uint8_t pin)
{
	#ifdef ARDUINO
		return((digitalRead(pin) == HIGH) ? true : false);
	#endif // ARDUINO	
}

/**
 * \brief Read The Channel 0 and 1 in differential mode
 * 
 * \param [void ]
 * 
 * \return [int16_t] Value in mv
 */
int16_t CURRENT::_readDifferential(void)
{
	return(ads.readADC_Differential_0_1());
}


