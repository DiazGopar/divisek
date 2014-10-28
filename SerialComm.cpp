/* 
* SerialComm.cpp
*
* Created: 25/10/2014 11:02:42
* Author: Gopar
*/


#include "SerialComm.h"

// default constructor
SerialComm::SerialComm()
{
} //SerialComm

// default destructor
SerialComm::~SerialComm()
{
} //~SerialComm

void SerialComm::begin(uint8_t _frametype)
{
	_buffer.clear();
	_buffer.push_back(FLAG_BYTE);
	add(_frametype);	
}

void SerialComm::add(uint8_t _byte)
{
	if((_byte != ESC_BYTE) && (_byte != FLAG_BYTE)) {
		_buffer.push_back(_byte);
		} else {
		_buffer.push_back(ESC_BYTE);
		_buffer.push_back(_byte ^ 0x20);
	}	
}

void SerialComm::end()
{
	_buffer.push_back(FLAG_BYTE);
}

void SerialComm::send()
{
	for (std::list<uint8_t>::const_iterator it = _buffer.begin(); it != _buffer.end(); it++) {
		Serial.write(*it);
	}
}

void SerialComm::clear()
{
	_buffer.clear();
}

uint8_t SerialComm::getlength()
{
	_buffer.size();	
}


