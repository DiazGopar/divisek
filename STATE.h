/* 
* STATE.h
*
* Created: 10/09/2014 13:23:49
* Author: Administrador
*/


#ifndef __STATE_H__
#define __STATE_H__

#if defined(ARDUINO)
	#if ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
	#endif
#endif

#include "FILTER.h"
#include "CONTROL.h"
#include "ltc6803_4.h"
#include "CURRENT.h"
#include "SerialComm.h"


#define VOLTS_FRAME 0x01
#define FLAGS_FRAME 0x02
#define DISCH_FRAME 0x03
#define TEMPS_FRAME 0x04
#define CURRENTS_FRAME 0x05
#define CHANNEL_FRAME 0x06

using namespace std;

class STATE
{
//variables
public:
	
protected:
	filterList _filterlist;
	
	LTC6803	*_ltc;
	CONTROL *_control;
	CURRENT *_current;
	
private:
	uint8_t _indexsoc;
	uint8_t _indexvoltages;
	uint8_t _indexloadcurrents;
	uint8_t _indexchargecurrents;
	uint8_t _indextemps;
	uint8_t _indexcontroldischargers;
	uint8_t _flagsoc;
	uint8_t _flagvoltages;
	uint8_t _flagloadcurrents;
	uint8_t _flagchargecurrents;
	uint8_t _flagtemps;
	uint8_t _flagcontroldischargers;
	//BMS Status variables
	int16_t **_matrixsoc;
	int16_t **_matrixvoltages;
	int16_t **_matrixcurrents; // matrix index of 0 for CHARGE and 1 for LOAD
	int16_t **_matrixtemps;
	uint8_t *_matrixdischargers;
	uint8_t _loadstatus;
	uint8_t _chargestatus;
	//BMS Control Variables
	uint8_t _controlload;
	uint8_t _controlcharge;
	uint8_t **_matrixcontroldischargers;
	//DEBUG
	uint8_t *_dischargers;
	uint8_t *_flagcells; //Matrix with flag related to cells( bit 0 - OverVoltage flag, bit 1 - UnderVoltage, bit 2 - Open Wire, ...)
	
	uint8_t _numberofcolumns;
	uint8_t _numberofvoltages;
	uint8_t	_numberofcurrents;
	uint8_t _numberoftemps;
	SerialComm *_SerialComm1;


//Methods
public:
	STATE(	const uint8_t _n_voltages = 12, 
			const uint8_t _n_temps = 3, 
			const uint8_t _n_states = 32); // Currents Fixed at TWO elements(CHARGE and LOAD)
	~STATE();
	
	//Status Methods
	void addSoc(int16_t *elements, uint8_t n_elements);
	void addVoltages(int16_t *elements, uint8_t n_elements);
	void addLOADCurrent(int16_t element);
	void addCHARGECurrent(int16_t  element);
	void addTemps(int16_t *elements, uint8_t n_elements);
	void addDischargers(uint8_t *cells);
	void addLOADStatus(uint8_t status);
	void addCHARGEStatus(uint8_t status);
	
	int16_t **getSoc(void);
	int16_t **getVoltages(void);
	int16_t *getLOADCurrents(void);
	int16_t *getCHARGECurrents(void);
	int16_t **getTemps(void);
	
	uint8_t getIndexSoc(void);
	uint8_t getIndexVoltages(void);
	uint8_t getIndexLOADCurrents(void);
	uint8_t getIndexCHARGECurrents(void);
	uint8_t getIndexTemps(void);
	
	uint8_t getNumberofColumns(void);
	uint8_t getNumberofVoltages(void);
	uint8_t getNumberofTemps(void);
	
	//Adding External Objects
	void addControl(CONTROL *control);
	void addCurrent(CURRENT *current);
	void addLTC(LTC6803 *ltc);
	
	//Flag Methods
	void setCellFlag(uint8_t cell, uint8_t flag);
	void setCellFlagOV(uint8_t cell, uint8_t value);
	void setCellFlagUV(uint8_t cell, uint8_t value);
	void setCellFlagOW(uint8_t cell, uint8_t value);
	uint8_t getCellFlag(uint8_t cell);
	
	
	//Control Methods
	void addControlDischargers(uint8_t *elements, uint8_t n_elements);
	void setControlCellDischarge(uint8_t cell, uint8_t value);
	void addControlLOAD(uint8_t status);
	void addControlCHARGE(uint8_t status);
	// Filter Methods
	void addFilter(FILTER *filter);
	void calcFilters(void);
	// Acting Methods
	void actuate(void);
	// Debug
	void debug();
	//Communications
	void sendData();

	
protected:
private:
	void _addCurrent(int16_t element, uint8_t type);
	void _accDischargers();
	void _accLOAD();
	void _accCHARGE();
	
	STATE( const STATE &c );
	STATE& operator=( const STATE &c );

}; //STATE

#endif //__STATE_H__
