/********************************************************************
	created:	2014/07/12
	created:	12:8:2014   10:07
	filename: 	C:\Documents and Settings\Administrador\Mis documentos\Dropbox\BMS\Arduino\ATMEGA\Prueba_puertos\Sketch1\ltc6803_4.h
	file path:	C:\Documents and Settings\Administrador\Mis documentos\Dropbox\BMS\Arduino\ATMEGA\Prueba_puertos\Sketch1
	file base:	ltc6803_4
	file ext:	h
	author:		Gopar
	
	purpose:	Class for accessing and LTC6803_4
				The LTC6803_4 have a watchdog that reset the chip between 1 Second to 2,5 Seconds, if they not are in CDC0,
				restart communication with the chip required send a valid command before the correct command.
					
*********************************************************************/
#ifndef __LTC6803_H__
#define __LTC6803_H__

#include <def_LTC.h>
#include "Arduino.h"
#include <SPI/SPI.h>


#define CS					53	//Chip Select from SPI bus //PB0 - Pin 19 = 10
#define SDO					50	// MISO from SPI bus	   //PB3 - Pin 22
//#define SPI_DEBUG			1	//Debug SPI can introduce delay between byte transmission
#define TRACE_ERROR			1	//Trace number of error
#define TRACE_DISCHARGE		1	// Trace number of cell discharge
#define	CELLFILTERLENGTH	10	// Length of Filter matrix of Cell Voltages

class LTC6803
{
	private: 
		uint8_t _address;
		// LTC Internal Registers		
		uint8_t _ltCFGreg[6];
		uint8_t _ltCVreg[18]; // Last read CellVoltages Registers values 
		uint8_t _ltFLGreg[3];
		uint8_t _ltTMPreg[5];
		uint8_t _ltPECreg;
		uint8_t _ltDIAGreg[2];	
		
		int16_t _temps[3];
		int16_t _voltages[12];
		
		#ifdef TRACE_ERROR //Error counting		
			uint32_t _numberofoperations;
			uint16_t _numberoferror;
			uint8_t _lasterror;
			uint8_t _error;
		#endif // TRACE_ERROR
		#ifdef TRACE_DISCHARGE 
			uint32_t celldischarge[12];
		#endif
		
				
	private:
		boolean _readRegister(uint8_t Reg, uint8_t nReg, uint8_t nError, uint8_t *bytestoRead);
		void _writeRegister(uint8_t Reg, uint8_t nReg, uint8_t *bytestoWrite);
		void _sendCommand(uint8_t command, uint8_t flag);
		void _sendCommand(uint8_t command);
		
		uint8_t calcPEC(uint8_t *p, uint8_t bytes); 
		uint8_t calcPEC(uint8_t p); 
		
		void _writeCFGRegister(void);
		
		boolean _readCFGRegister(void);
		boolean _readCVRegister(void); //Read all Cells
		boolean _readCVRegister(uint8_t group);
		boolean _readFLGRegister(void);
		boolean _readTMPRegister(void);
		boolean _readDIAGRegister(void);
		
		void _STCVADCommand(uint8_t cell);
		void _STOWADCommand(uint8_t cell);
		void _STTMPADCommand(uint8_t sens);
		void _STCVDCCommand(uint8_t cell);
		void _STOWDCCommand(uint8_t cell);
		
		void _PLADCCommand(void);
		void _PLINTCommand(void);
		void _DAGNCommand(void);
		
		boolean _setVoltages(uint8_t *rawcellvoltages);		
		boolean _setTemperatures(uint8_t *rawtemperatures);		
		
		
	public:
		LTC6803(uint8_t address);
		void begin(void);
		// 
		uint8_t setAddress(uint8_t address);
		uint8_t getAddress(void);
		uint8_t setConfRegister(uint8_t *ltconfig);
		boolean getConfRegister(uint8_t *ltconfig);

		// For debug
		uint8_t prueba(uint8_t byte);
		
		//Complex Operations
		boolean getLastTemperatures(uint16_t *temperatures);
		boolean getLastVoltages(uint16_t *voltages);
		boolean getLastVoltages(uint16_t *voltages, uint8_t cell);
		void getImmTemperatures(uint16_t *temperatures);
		boolean getImmVoltages(uint16_t *voltages);
		boolean getImmVoltages(uint16_t *voltages, uint8_t cell);
		boolean sendCLEAR(uint16_t *voltages, uint8_t cell);
		
		boolean getTHSDStatus(void);
		
		
		uint8_t getNumberofCells(void);
		
		//Cell Discharge
		void setCellDischargeON(uint8_t cell);
		void setCellDischargeValue(uint8_t *cells, uint8_t n_cells);
		void setCellDischargeOFF(uint8_t cell);
		void setCellDischargeOFF(uint8_t *cells);
		void setCellsDischarge(uint8_t *cells);
		void getCellDischargeStatus(uint8_t cell, boolean *status);
		void getCellsDischargeStatus(uint8_t *cells);
		void getCellsDischargeStatus(uint8_t *cells, uint8_t elements);
		
			
		#ifdef TRACE_ERROR 	//Error Counting
			uint32_t getNumberofOperations(void);
			uint16_t getNumberofError(void);
			uint8_t  getLastError(void);
			boolean	 isError(void);
	
		#endif // TRACE_ERROR
};

#endif //__LTC6803_H__