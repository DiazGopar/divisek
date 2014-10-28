/* 
* control.cpp
*
* Created: 16/08/2014 13:26:33
* Author: Gopar
*/


#include "CONTROL.h"



// default constructor
CONTROL::CONTROL()
{
	#ifdef ARDUINO
		pinMode(EN_12V, OUTPUT);
		pinMode(C_DRIVER_ON, INPUT);
		pinMode(L_DRIVER_ON, INPUT);
		pinMode(C_CONNECTED, INPUT);
		pinMode(L_CONNECTED, INPUT);
		pinMode(CHECK_L_C, INPUT);
		pinMode(MP_CHARGE_ON, OUTPUT);
		digitalWrite(MP_CHARGE_ON,HIGH);
		pinMode(MP_CHARGE_OFF, OUTPUT);
		digitalWrite(MP_CHARGE_OFF,HIGH);
		pinMode(MP_DISCHARGE_ON, OUTPUT);
		digitalWrite(MP_DISCHARGE_ON,HIGH);
		pinMode(MP_DISCHARGE_OFF, OUTPUT);
		digitalWrite(MP_DISCHARGE_OFF,HIGH);
	#endif // ARDUINO
} //control

// default destructor
CONTROL::~CONTROL()
{
} //~control


/**
 * \brief Set a falling Pulse of 2 microseconds in pin
 * 
 * \param [uint8_t pin] The pin number where to do the pulse
 * 
 * \return [void]
 */
void CONTROL::_setPulseL(uint8_t pin)
{
	#ifdef ARDUINO
		digitalWrite(pin,HIGH);
		delayMicroseconds(2);
		digitalWrite(pin,LOW);
		delayMicroseconds(2);
		digitalWrite(pin,HIGH);
		delayMicroseconds(2);
	#endif // ARDUINO
}

/**
 * \brief Enable the 12V Power
 * 
 * \param [void ]
 * 
 * \return [void]
 */
void CONTROL::set12VON(void)
{
	_setPinHIGH(EN_12V);
}

/**
 * \brief Disable the 12V power 
 * 
 * \param [void ]
 * 
 * \return [void]
 */
void CONTROL::set12VOFF(void)
{
	_setPinLOW(EN_12V);
}

/**
 * \brief 
 * 
 * \param [void ]
 * 
 * \return [void]
 */
void CONTROL::setchargeDriverON(void)
{
	_setPulseL(MP_CHARGE_ON);	
}

/**
 * \brief 
 * 
 * \param [void ]
 * 
 * \return [void]
 */
void CONTROL::setchargeDriverOFF(void)
{
	_setPulseL(MP_CHARGE_OFF);	
}

/**
 * \brief 
 * 
 * \param [void ]
 * 
 * \return [void]
 */
void CONTROL::setdischargeDriverON(void)
{
	_setPulseL(MP_DISCHARGE_ON);	
}

/**
 * \brief 
 * 
 * \param [void ]
 * 
 * \return [void]
 */
void CONTROL::setdischargeDriverOFF(void)
{
	_setPulseL(MP_DISCHARGE_OFF);
}

/**
 * \brief Enable the Check for Load and Charge
 * 
 * \param [void ]
 * 
 * \return [void]
 */
void CONTROL::setCheck_L_C_ON(void)
{
	_setPinHIGH(CHECK_L_C);		
}

/**
 * \brief Disable the check for Load and Charge
 * 
 * \param [void ]
 * 
 * \return [void]
 */
void CONTROL::setCheck_L_C_OFF(void)
{
	_setPinLOW(CHECK_L_C);
}

/**
 * \brief Check if there are connection in Charger. You must enable check before doing that. (setCheck_L_C_ON)
 * 
 * \param [void ]
 * 
 * \return [boolean] True for Charger connection, False for disconnect.
 */
boolean CONTROL::isChargerConnected(void)
{
	return(_getPinValue(C_CONNECTED));
}

/**
 * \brief Check if there are connection in Load. You must enable check before doing that.(setCheck_L_C_ON)
 * 
 * \param [void ]
 * 
 * \return [boolean] True for connection in Load, False for disconnect.
 */
boolean CONTROL::isLoadConnected(void)
{
	return(_getPinValue(L_CONNECTED));
}

/**
 * \brief Get the Charger Drivers status.
 * 
 * \param [void ]
 * 
 * \return [boolean] True for Charger driver enable, False for charger driver disable
 */
boolean CONTROL::getChargerDriverStatus(void)
{
	return(_getPinValue(C_DRIVER_ON));
}

/**
 * \brief Get the Load driver status
 * 
 * \param [void ]
 * 
 * \return [boolean] True for Load driver enable, False for load driver disable
 */
boolean CONTROL::getLoadDriverStatus(void)
{
	return(_getPinValue(L_DRIVER_ON));
}

/**
 * \brief Get the DC-DC 12V Status
 * 
 * \param [void ]
 * 
 * \return [boolean] true if enable
 */
boolean CONTROL::get12VStatus(void)
{
	return(_getPinValue(EN_12V));
}


/**
 * \brief Abstract method for set a pin HIGH  
 * 
 * \param [uint8_t pin] number of pin
 * 
 * \return [void]
 */
void CONTROL::_setPinHIGH(uint8_t pin)
{
	#ifdef ARDUINO
		digitalWrite(pin,HIGH);
	#endif // ARDUINO	
}

/**
 * \brief Abstract method for set a pin LOW
 * 
 * \param [uint8_t pin] number of pin
 * 
 * \return [void]
 */
void CONTROL::_setPinLOW(uint8_t pin)
{
	#ifdef ARDUINO
		digitalWrite(pin,LOW);
	#endif // ARDUINO	
}

/**
 * \brief Get the Pin value (True for HIGH state, False for LOW)
 * 
 * \param [uint8_t pin] Pin number
 * 
 * \return [boolean] True for LOW, False for HIGH (NOTE: Inverse Logic)
 */
boolean CONTROL::_getPinValue(uint8_t pin)
{
	#ifdef ARDUINO
		return((digitalRead(pin) == HIGH) ? false : true);
	#endif // ARDUINO	
}













