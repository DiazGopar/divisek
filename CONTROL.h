/* 
* control.h
*
* Created: 16/08/2014 13:26:34
* Author: Gopar
*/


#ifndef __CONTROL_H__
#define __CONTROL_H__

#ifdef ARDUINO
	#include "Arduino.h"
	//***************************************
	#define EN_12V			A13 //PK5 - Pin 84  = 67 // Output
	#define C_DRIVER_ON		A12 //PK4 - Pin 85	= 66 // Input 
	#define L_DRIVER_ON		A11 //PK3 - Pin 86  = 65 // Input
	#define C_CONNECTED		A10 //PK2 - Pin 87  = 64 // Input 
	#define L_CONNECTED		A9  //PK1 - Pin 88  = 63 // Input 
	#define CHECK_L_C		A8  //PK0 - Pin 89  = 62 // Output 
	#define MP_CHARGE_ON		10	//PB4 - Pin 23 = 10  // Output
	#define MP_CHARGE_OFF		11  //PB5 - Pin 24 = 11  // Output
	#define MP_DISCHARGE_ON		12  //PB6 - Pin 25 = 12	 // Output
	#define MP_DISCHARGE_OFF	13  //PB7 - Pin 26 = 13	 // Output
	//******************************

#endif // ARDUINO



class CONTROL
{
//variables
public:
protected:
private:

//functions
public:
	CONTROL();
	~CONTROL();
	void set12VON(void);
	void set12VOFF(void);
	void setchargeDriverON(void);
	void setchargeDriverOFF(void);
	void setdischargeDriverON(void);
	void setdischargeDriverOFF(void);
	void setCheck_L_C_ON(void);
	void setCheck_L_C_OFF(void);

	boolean isChargerConnected(void);
	boolean isLoadConnected(void);
	boolean getChargerDriverStatus(void);
	boolean getLoadDriverStatus(void);
	boolean get12VStatus(void);
	
protected:
private:
	CONTROL(const CONTROL &c);
	CONTROL& operator=( const CONTROL &c );
	
	void _setPulseL(uint8_t pin);
	void _setPinHIGH(uint8_t pin);
	void _setPinLOW(uint8_t pin);
	boolean _getPinValue(uint8_t pin);
	
}; //control

#endif //__CONTROL_H__
