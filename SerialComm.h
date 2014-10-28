/* 
* SerialComm.h
*
* Created: 25/10/2014 11:02:43
* Author: Administrador
*/


#ifndef __SERIALCOMM_H__
#define __SERIALCOMM_H__

#if defined(ARDUINO)
	#if ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
	#endif
#endif

#define ESC_BYTE  0x7D  //0x7d
#define FLAG_BYTE 0x7E  //0x7e

using namespace std;
#include <iterator>
#include <list>

class SerialComm
{
//variables
public:
protected:
private:
	std::list<uint8_t> _buffer;	


//functions
public:
	SerialComm();
	~SerialComm();
	void begin(uint8_t _frametype);
	void end();
	void add(uint8_t _byte);
	void send();
	void clear();
	uint8_t getlength();
protected:
private:
	
	SerialComm( const SerialComm &c );
	SerialComm& operator=( const SerialComm &c );

}; //SerialComm

#endif //__SERIALCOMM_H__
