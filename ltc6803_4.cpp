/*
 * ltc6803-4.cpp
 *
 * Created: 15/07/2014 9:05:54
 *  Author: Administrador
 */ 

#include "ltc6803_4.h"
#include "def_LTC.h"


// SendtoSpi , GetfromSPI & ParseSpiData
// copy from https://code.google.com/p/calsol/source/browse/trunk/BMS/bms.pde
// and modified
// This Functions are Arduino SPI abstraction

unsigned long time;

void settimeout(uint16_t mstimeout) {
	time = millis() + mstimeout;	
}

boolean timeout_ms(void) {
	if(millis() > time) {
		return(true);
	}
	return(false);
}

uint8_t spiCheckSDO(void) {
	if(digitalRead(SDO) == LOW) {
		return(LOWSDO);
	} else {
		return(HIGHSDO);
	}
}

void spiStart(void) {
	SPI.begin(); 
	if(digitalRead(CS) == LOW) {
		digitalWrite(CS,HIGH);
		delayMicroseconds(5); //Ensure a valid pulse
	}
	digitalWrite(CS, LOW);
	//delayMicroseconds(20); //TODO: Comentado para hacer pruebas con las placas viejas 2014/08/07 Parece que mejora
}



void spiEnd(uint8_t flag) { //Wait poll Status
	SPI.end();
	if(flag > 0) {
		delayMicroseconds(10); //TODO: Quitar cuando se acaben las pruebas con las placas viejas
		digitalWrite(CS, HIGH);
	}
}

void spiEnd(void) {
	spiEnd(1);
}

void sendtoSpi(const uint8_t *data, int length) {
	for (int i = 0; i < length; ++i) {
		SPI.transfer(data[i]);
	}
	#ifdef SPI_DEBUG
		Serial.print(F("SPI: Sent "));
		for (int i = 0; i < length; ++i) {
			Serial.print(data[i], HEX);
			Serial.print(F(", "));
		}
		Serial.println(F("end of message"));
	#endif
}

void sendtoSpi(uint8_t data) {
	
	SPI.transfer(data);
	
	#ifdef SPI_DEBUG
		Serial.print(F("SPI: Sent "));
		Serial.print(data, HEX);
		Serial.print(F(", "));
		Serial.println(F("end of message"));
	#endif
}

void getfromSpi(uint8_t *data, uint8_t info, int length) {
	for (int i = 0; i < length; ++i) {
		data[i] = SPI.transfer(info);
	}
	#ifdef SPI_DEBUG
		Serial.print(F("SPI: With "));
		Serial.print(info, HEX);
		Serial.print(F(", Got "));
		for (int i = 0; i < length; ++i) {
			Serial.print(data[i], HEX);
			Serial.print(F(", "));
		}
		Serial.println(F("end of message"));
	#endif
}

void initSpi(void) {
	#ifdef SPI_DEBUG
		Serial.println("SPI: Initializing");
	#endif
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV128);
	SPI.setDataMode(SPI_MODE3);
	SPI.setBitOrder(MSBFIRST);
	//spiEnd(); // CS in high state 
}

	

//--------------------------------------------------------------
//		PRIVATE FUNCTIONS
//
//--------------------------------------------------------------


/*
	calcPEC, this function calculate the CRC Code for bytes array
	
	return the CRC calculated
*/


uint8_t LTC6803::calcPEC(uint8_t *p, uint8_t bytes)
{
	//PEC initial value
	uint8_t PEC = B01000001;
	uint8_t i, j;
	uint8_t aux = 0; // use only four firsts bits

	for(i=0;i<bytes;i++){
		for(j=8;j>0;j--){
			bitWrite(aux,0,(bitRead(p[i],j-1)) ^ bitRead(PEC,7));
			bitWrite(aux,1,(bitRead(aux,0) ^ bitRead(PEC,0)));
			bitWrite(aux,2,(bitRead(aux,0) ^ bitRead(PEC,1)));
			PEC=PEC<<1;
			PEC &= B11111000;
			PEC |= (aux & B00000111);
		}
	}
	return(PEC);
}

uint8_t LTC6803::calcPEC(uint8_t p)
{
	return(calcPEC(&p,1));
}


/**
 * \brief Write in LTC the configuration register, saved in _ltCFGreg array
 * 
 * \param [void ]
 * 
 * \return [void]
 */
void LTC6803::_writeCFGRegister(void)
{
	
	_writeRegister(WRCFG,6,_ltCFGreg);
				
}


/**
 * \brief Read Configuration Register from LTC6803 chip and put in object memory
 * 
 * \param void
 * 
 * \return boolean (True if success) (False in PEC testing error)
 */
boolean LTC6803::_readCFGRegister(void)
{
	uint8_t buffer[7];
	
	if(_readRegister(RDCFG,7,E_readCFGRegister,buffer)) {
		memcpy(_ltCFGreg,buffer,6);
		return(true);
	}
	return(false);
	
}


/**
 * \brief Give the number of cell connected to LTC (Only Work with Li-ion Cell, dont work with Supercapacitor)
 * 
 * \param [void ] 
 * 
 * \return [uint8_t] number of cell connected
 */
uint8_t LTC6803::getNumberofCells(void)
{
	uint16_t Voltages[12];
	uint8_t Cell = 0; // number of cells detected
	uint8_t i;
	
	_STCVADCommand(0);  //Start Cell Voltage ADC conversions
	//wait for read all voltages 13ms
	delay(21);
	if(_readCVRegister(0)) { //Send read all Cell Voltages
		if(getLastVoltages(Voltages)) { //Read all Cell Voltages in mV
			for(i=11;i>0;i--){
				if((Voltages[i] < 1000) | (Voltages[i] > 5000))
					Cell = i;
			}	
		}
	}
		
	return(Cell);
}


/**
 * \brief Return the LTC6803 address in object memory, not physical
 * 
 * \param void
 * 
 * \return [uint8_t] address in object memory
 */
uint8_t LTC6803::getAddress(void)
{
	return(_address);	
}

/**
 * \brief  This method assigns the address to the object
 * 
 * \param [uint8_t address], the new address of Object
 * 
 * \return boolean True if success
 */
boolean LTC6803::setAddress(uint8_t address)
{
	if ((address >=0) && (address < 16)){
		_address = address;
		return(true);
	} else {
		return(false);	
	}
	//TODO: Object Reinit
}

/**
 * \brief This metod Write the configuration register into LTC6803
 * 
 * \param [uint8_t *ltconfig] array called ltconfig, must have 6 elements
 * 
 * \return uint8_t
 */
uint8_t LTC6803::setConfRegister(uint8_t *ltconfig)
{
	memcpy(_ltCFGreg, ltconfig, 6);
	_writeCFGRegister();
	
	return(0);
}

/**
 * \brief Read the configuration Register of LTC6803-4 and put the content in ltconfig
 * 
 * \param [uint8_t ltconfig] is a array must have room enough for 6 bytes
 * 
 * \return [boolean] True if read success
 */
boolean LTC6803::getConfRegister(uint8_t *ltconfig)
{
	if (_readCFGRegister()) {
		memcpy(ltconfig, _ltCFGreg, 6);		
		return(true);
	}
	return(false);
}


/**
 * \brief Create a Object. Initialize configuration register values, address values
 * 
 * \param [uint8_t address], Position in bus (0 to 16)
 * 
 * \return a LTC6803 object in that address
 */
LTC6803::LTC6803(uint8_t address)
{
	//Init Module address
	if (address >=0 && address < 16){
		_address = address;
	}
	//Register Initial Values
	_ltCFGreg[0] = 0x00 | LVLPL | CDC2 | CELL12;
	_ltCFGreg[1] = 0x00;
	_ltCFGreg[2] = 0x00;
	_ltCFGreg[3] = 0x00 | MC12I | MC11I | MC10I | MC9I | MC8I;
	_ltCFGreg[4] = 0x00;
	_ltCFGreg[5] = 0x00;
	
	#ifdef TRACE_ERROR //Error counting		
		_numberofoperations = 0;
		_numberoferror = 0;
		_lasterror = 0;
		_error = 0;
	#endif
	
}

void LTC6803::begin(void)
{
	initSpi();	
}

uint8_t LTC6803::prueba(uint8_t byte)
{
	spiStart();
	//SPI.begin();
	sendtoSpi(byte);
	//SPI.end();
	spiEnd();	
}


/**
 * \brief Common function for send commands and read physical registers
 * 
 * \param [uint8_t Comm] Command to send
 * \param [uint8_t nReg] Number of bytes to be read from device including PEC
 * \param [uint8_t nError] Error code number to log in error
 * \param [uint8 *bytesReaded] Initialized Array with room for nReg 
 * 
 * \return boolean True if success
 */
boolean LTC6803::_readRegister(uint8_t Reg, uint8_t nReg, uint8_t nError, uint8_t *bytestoRead)
{
	uint8_t aux1; //PEC from Read Command 
	uint8_t aux2; //PEC from Command
	
	//We calculate PEC before send it, to not include delays
	aux1 = calcPEC(_address | ACRW);
	aux2 = calcPEC(Reg);
	
	spiStart();
	sendtoSpi(_address | ACRW); //Send Address
	sendtoSpi(aux1);			//Send Address PEC
	sendtoSpi(Reg);				//Send Command 
	sendtoSpi(aux2);			//Send PEC Command
	getfromSpi(bytestoRead,0xAA,nReg); //We read nReg bytes including PEC
	spiEnd();
	
	#ifdef TRACE_ERROR
		_numberofoperations++; //Ïncrement Number of operations with LTC
	#endif // TRACE_ERROR

	if(bytestoRead[nReg-1] != calcPEC(bytestoRead,nReg-1)) {//Check for PEC
		#ifdef TRACE_ERROR
		_numberoferror++;
		_lasterror = nError; //Error to Return
		_error = 1;
		#endif // TRACE_ERROR
		return(false);
	}
	//memcpy(_ltFLGreg,bytesReaded,3);
	#ifdef TRACE_ERROR
	//_lasterror = 0; //No error
	_error = 0;
	#endif // TRACE_ERROR
	return(true);	
}

/**
 * \brief Common function to send Commands for write physical Registers
 * 
 * \param [uint8_t Reg] Command to Send
 * \param [uint8_t nReg] Number of bytes to Write in Physical Registers
 * \param [uint8_t *bytesReaded] Initialized array with Register data nReg long
 * 
 * \return 
 */
void LTC6803::_writeRegister(uint8_t Reg, uint8_t nReg, uint8_t *bytestoWrite)
{
	uint8_t aux1;	//PEC from address
	uint8_t aux2;	//PEC from command
	uint8_t aux3;	//PEC from Registers data
	
	//We calculate PEC before send to not include delays
	aux1 = calcPEC(_address | ACRW);
	aux2 = calcPEC(Reg);
	aux3 = calcPEC(bytestoWrite,nReg);
	
	spiStart();
	sendtoSpi(_address | ACRW); //Send Address
	sendtoSpi(aux1);			//Send Address PEC
	sendtoSpi(Reg);				//Send Command
	sendtoSpi(aux2);			//Send Command PEC
	sendtoSpi(bytestoWrite,nReg);	//Send Registers Data		
	sendtoSpi(aux3);			//Send Registers Data PEC
	spiEnd();
	
	#ifdef TRACE_ERROR
		_numberofoperations++; //Increment Number of operations with LTC
	#endif // TRACE_ERROR

}


/**
 * \brief Send a Command to LTC
 * 
 * \param [uint8_t command] Command Code
 *		  [uint8_t flag] if flag > 0 dont HIGH Chip Select (Keep CS LOW)
 * 
 * \return 
 */
void LTC6803::_sendCommand(uint8_t command, uint8_t flag)
{
	uint8_t aux1; //PEC from Address Command
	uint8_t aux2; //PEC from Command
	
	//We calculate PEC before send it, to not include delays
	aux1 = calcPEC(_address | ACRW);
	aux2 = calcPEC(command);
	
	spiStart();
	sendtoSpi(_address | ACRW); //Send Address
	sendtoSpi(aux1);			//Send Address PEC
	sendtoSpi(command);			//Send Command
	sendtoSpi(aux2);			//Send PEC Command
	spiEnd(flag);
	
	#ifdef TRACE_ERROR
		_numberofoperations++; //Ïncrement Number of operations with LTC
	#endif // TRACE_ERROR
}

/**
 * \brief Send Command to LTC with CS set to HIGH at finish
 * 
 * \param [uint8_t command] Command code
 * 
 * \return 
 */
void LTC6803::_sendCommand(uint8_t command)
{
	_sendCommand(command,HIGHCS);
}

/**
 * \brief Read physical FLAG Register Group
 * 
 * \param void
 * 
 * \return [boolean] True if success
 */
boolean LTC6803::_readFLGRegister(void)
{
	uint8_t buffer[3];
		
	if(_readRegister(RDFLG,4,E_readFLGRegister,buffer)) {
		memcpy(_ltFLGreg,buffer,3);
		return(true);
	}
	return(false);
}


/**
 * \brief Read Temperature Register Group 
 * 
 * \param [void ]
 * 
 * \return [boolean]
 */
boolean LTC6803::_readTMPRegister(void)
{
	uint8_t buffer[6];
	
	if(_readRegister(RDTMP,6,E_readTMPRegister,buffer)) {
		memcpy(_ltTMPreg,buffer,5);
		_setTemperatures(_ltTMPreg);
		return(true);
	}
	return(false);	
}


/**
 * \brief Read Diagnostics Register
 * 
 * \param [void ]
 * 
 * \return [boolean]
 */
boolean LTC6803::_readDIAGRegister(void)
{
	uint8_t buffer[3];
	
	if(_readRegister(RDDGNR,3,E_readDIAGRegister,buffer)) {
		memcpy(_ltDIAGreg,buffer,2);
		return(true);
	}
	return(false);	
}


/**
 * \brief Send Command [Read All Cell Voltage Group]
 * 
 * \param [void ]
 * 
 * \return [boolean]
 */
boolean LTC6803::_readCVRegister(void)
{
	uint8_t buffer[19];
	
	if(_readRegister(RDCV,19,E_readCVRegister,buffer)) {
		memcpy(_ltCVreg,buffer,18);
		_setVoltages(_ltCVreg);
		return(true);
	}
	return(false);
	//return(_readCVRegister(0));
}


/**
 * \brief Send Command [Read Cell Voltage Group]
 * 
 * \param [uint8_t group] group 0 is all Cells
 *								1 is Cells [1-4]
 *								2 is Cells [5-8]
 *								3 is Cells [9-12]
 * 
 * \return [boolean]
 */
boolean LTC6803::_readCVRegister(uint8_t group)
{
	uint8_t buffer[7]; 
	uint8_t command;
	
	command &= 0x03; // Only values between 0-3
	command = RDCV + (group*2); // RDCV, RDCVA, RDCVB, RDCVC [04 06 08 0A] 
	
	if (command == RDCV) {
		if(_readCVRegister()) {
			return(true);
		} else {
			return(false);
		}
	} 
	
	if(_readRegister(command,7,E_readCVRegister+group,buffer)) {
		memcpy((_ltCVreg+((group-1)*6)),buffer,6);
		//TODO: _setVoltages, how to implement
		return(true);
	}
	return(false);	
}


/**
 * \brief Send Command [Start Cell Voltage ADC Conversions and Poll Status]
 * 
 * \param [uint8_t cell] Cell value of 0, is all Cells.
 *							Values [1 to 12] Cell X.
 *							value 13 - Clear (FF)
 *							value 14 - Self Test1
 *							value 15 - Self Test2	
 * 
 * \return
 */
void LTC6803::_STCVADCommand(uint8_t cell)
{
	uint8_t command;
	
	cell &= 0x0F; //only values between 0 and 15, last values are self test
	
	command = STCVADXX;
	command += cell;
	
	_sendCommand(command,LOWCS);
}


/**
 * \brief Send Command [Start Open-Wire ADC Conversions and Poll Status]
 * 
 * \param [uint8_t cell] Cell value of 0, is all Cells.
 * 
 * \return 
 */
void LTC6803::_STOWADCommand(uint8_t cell)
{
	uint8_t command;
	
	cell &= 0x0F; //only values between 0 and 15 
	
	command = STOWADXX;
	command += cell;
	
	_sendCommand(command);
	
}


/**
 * \brief Send Command [Start Temperature ADC Conversions and Poll Status] 
 * 
 * \param [uint8_t sens] Sens value of 0, is all sensor.
 *							1 - External1
 *							2 - External2
 *							3 - Internal
 *							14 - Self Test1
 *							15 - Self Test2
 * 
 * \return 
 */
void LTC6803::_STTMPADCommand(uint8_t sens)
{
	uint8_t command;
	
	sens &= 0x0F; //only values between 0 and 15, last values are self test
	
	command = STTMPADXX;
	command += sens;
	
	_sendCommand(command,LOWCS);
}


/**
 * \brief Send Command [Start Cell Voltage ADC Conversions and Poll Status, with discharge permitted]
 * 
 * \param [uint8_t cell] Cell value of 0 is all cells.
 * 
 * \return 
 */
void LTC6803::_STCVDCCommand(uint8_t cell)
{
	uint8_t command;
	
	cell &= 0x0F; //only values between 0 and 15
	
	command = STCVDCXX;
	command += cell;
	
	_sendCommand(command);
}


/**
 * \brief Send Command [Start Open-Wire Conversions and Poll Status, with discharged permitted]
 * 
 * \param [uint8_t cell] Cell value of 0 is all cells.
 * 
 * \return 
 */
void LTC6803::_STOWDCCommand(uint8_t cell)
{
	uint8_t command;
	
	cell &= 0x0F; //only values between 0 and 15
	
	command = STOWDCXX;
	command += cell;
	
	_sendCommand(command);
}


/**
 * \brief Send Command [Poll ADC Converter Status]
 * 
 * \param void
 * 
 * \return void
 */
void LTC6803::_PLADCCommand(void)
{	
	_sendCommand(PLADC,LOWCS);
	//settimeout(20);
	//while((spiCheckSDO() != LOWSDO) & !timeout_ms()); //Wait for LTC Pull down SDO Line or timeout	
}

/**
 * \brief Send Command [Poll Interrupt Status]
 * 
 * \param void
 * 
 * \return void
 */
void LTC6803::_PLINTCommand(void)
{
	_sendCommand(PLINT);
}

/**
 * \brief Send Command [Start Diagnose and Poll Status]
 * 
 * \param void
 * 
 * \return 
 */
void LTC6803::_DAGNCommand(void)
{
	_sendCommand(DAGN);
}


/**
 * \brief Get the last measured temperatures in mV
 * 
 * \param [uint16_t * temperatures] Initialized array with room enough for 3 elements
 * 
 * \return [boolean]
 */
boolean LTC6803::getLastTemperatures(uint16_t *temperatures)
{		
	uint8_t i;
	
	for(i=0;i<3;i++) {
		temperatures[i] = _temps[i];	
	}
	
	return(true);
}


/**
 * \brief Start Temperature ADC and get the Values of Temperature in mV
 * 
 * \param [uint16_t * temperatures] Initialized array with room enough for 3 elements
 * 
 * \return void
 */
void LTC6803::getImmTemperatures(uint16_t *temperatures)
{
	//TODO
	//TODO
	//TODO
	_STTMPADCommand(0); // Send command STTMPAD for all temperature sensors
		// Wait for measure done
		//TODO
	delay(5);
	_readTMPRegister();
		
	getLastTemperatures(temperatures);
	
}


/**
 * \brief Get the last measured Cell Voltages in mV
 * 
 * \param [uint16_t * voltages] Initialized array with room enough for 12 elements
 * 
 * \return [boolean] Return True if success
 */
boolean LTC6803::getLastVoltages(uint16_t *voltage)
{
	getLastVoltages(voltage,12);
	return(true);
}

/**
 * \brief Get the last n-cell measured Cell Voltages in mV
 * 
 * \param [uint16_t * voltages] Initialized array with room enough for max 12 elements
 * 
 * \return [boolean] Return True if success
 */
boolean LTC6803::getLastVoltages(uint16_t *voltages, uint8_t cell)
{
	uint8_t i;
	
	for(i=0;i<cell;i++){
		voltages[i] = _voltages[i];
	}
	
	return(true);	
}

/**
 * \brief Introduce In Cell filter Matrix converted values in mV from raw information of Cell Registers
 * 
 * \param [uint16_t * rawcellvoltages]
 * 
 * \return [boolean]
 */
boolean LTC6803::_setVoltages(uint8_t *rawcellvoltages)
{
	uint8_t index;

	_voltages[0] = (uint16_t) ((((rawcellvoltages[0] & 0xFF) | (rawcellvoltages[1] & 0x0F) << 8) - 512) * 3) / 2 ;
	_voltages[1] = (uint16_t) ((((rawcellvoltages[1] & 0xF0) >> 4 | (rawcellvoltages[2] & 0xFF) << 4) - 512) * 3) / 2 ;
	_voltages[2] = (uint16_t) ((((rawcellvoltages[3] & 0xFF) | (rawcellvoltages[4] & 0x0F) << 8) - 512) * 3) / 2 ;
	_voltages[3] = (uint16_t) ((((rawcellvoltages[4] & 0xF0) >> 4 | (rawcellvoltages[5] & 0xFF) << 4) - 512) * 3) / 2 ;
	_voltages[4] = (uint16_t) ((((rawcellvoltages[6] & 0xFF) | (rawcellvoltages[7] & 0x0F) << 8) - 512) * 3) / 2 ;
	_voltages[5] = (uint16_t) ((((rawcellvoltages[7] & 0xF0) >> 4 | (rawcellvoltages[8] & 0xFF) << 4) - 512) * 3) / 2 ;
	_voltages[6] = (uint16_t) ((((rawcellvoltages[9] & 0xFF) | (rawcellvoltages[10] & 0x0F) << 8) - 512) * 3) / 2 ;
	_voltages[7] = (uint16_t) ((((rawcellvoltages[10] & 0xF0) >> 4 | (rawcellvoltages[11] & 0xFF) << 4) - 512) * 3) / 2 ;
	_voltages[8] = (uint16_t) ((((rawcellvoltages[12] & 0xFF) | (rawcellvoltages[13] & 0x0F) << 8) - 512) * 3) / 2 ;
	_voltages[9] = (uint16_t) ((((rawcellvoltages[13] & 0xF0) >> 4 | (rawcellvoltages[14] & 0xFF) << 4) - 512) * 3) / 2 ;
	_voltages[10] = (uint16_t) ((((rawcellvoltages[15] & 0xFF) | (rawcellvoltages[16] & 0x0F) << 8) - 512) * 3) / 2 ;
	_voltages[11] = (uint16_t) ((((rawcellvoltages[16] & 0xF0) >> 4 | (rawcellvoltages[17] & 0xFF) << 4) - 512) * 3) / 2 ;
	
	//if(state != NULL){
	//	state->addVoltages(_voltages,12);
	//}
	
	return(true);	
}

/**
 * \brief Introduce in Temperatures Filters List values in mV from raw information of Temperature Registers
 * 
 * \param [uint8_t * rawtemperatures] Values read from register
 * 
 * \return [boolean]
 */
boolean LTC6803::_setTemperatures(uint8_t *rawtemperatures)
{
	uint8_t index;
	
	
	_temps[0] = (int16_t) ((((rawtemperatures[0] & 0xFF) | (rawtemperatures[1] & 0x0F) << 8) - 512) * 3) / 2 ;
	_temps[1] = (int16_t) ((((rawtemperatures[1] & 0xF0) >> 4 | (rawtemperatures[2] & 0xFF) << 4) - 512) * 3) / 2;
	_temps[2] = (int16_t) ((((rawtemperatures[3] & 0xFF) | (rawtemperatures[4] & 0x0F) << 8) - 512) * 3) / 2;
	
	//if(state != NULL){
	//	state->addTemps(_temps,3);
	//}
	
	return(true);
}


/**
 * \brief Turn ON discharge on specified Cell without reset of discharge status in other cells.
 * 
 * \param [uint8_t cell] Cell number from 1 to 12
 * 
 * \return 
 */
void LTC6803::setCellDischargeON(uint8_t cell)
{
	uint16_t cellvalue;
	
	cellvalue = 1 << (cell-1);
	
	_ltCFGreg[1] |= cellvalue;
	_ltCFGreg[2] |= (cellvalue & 0x0F00) >> 8;
	
	_writeCFGRegister();
	#ifdef TRACE_DISCHARGE
		celldischarge[cell-1]++;
	#endif	
}

/**
 * \brief Turn On discharge on specified cells resetting other cells
 * 
 * \param [uint8_t * cells] pointer of matrix with 0 from disable and > 0 for enable
 * \param [uint8_t n_cells] number of cells to read from pointer
 * 
 * \return [void]
 */
void LTC6803::setCellDischargeValue(uint8_t *cells, uint8_t n_cells)
{
	uint8_t i;
	uint16_t bitvalue;
	
	/*for(i=0;i<n_cells;i++){
		Serial.println();
		Serial.print(cells[i]);
		Serial.print((" "));
	}
	Serial.println("");*/
	
	
	for(i=0;i<n_cells;i++){
		bitvalue = 1 << i;	
		if(cells[i] > 0) {
			_ltCFGreg[1] |= bitvalue;
			_ltCFGreg[2] |= (bitvalue & 0x0F00) >> 8;
			#ifdef TRACE_DISCHARGE
				celldischarge[i]++;
			#endif
		} else {
			_ltCFGreg[1] &= ~bitvalue;
			_ltCFGreg[2] &= ~((bitvalue & 0x0F00) >> 8);	
		}
	}
	_writeCFGRegister();
	
}

/**
 * \brief Turn OFF discharge on specified Cell without reset of discharge status in other cells.
 * 
 * \param [uint8_t cell] Cell number from 1 to 12
 * 
 * \return 
 */
void LTC6803::setCellDischargeOFF(uint8_t cell)
{
	uint16_t cellvalue;
	
	cellvalue = ~(1 << (cell-1));
	
	_ltCFGreg[1] &= cellvalue;
	_ltCFGreg[2] &= (cellvalue & 0x0F00) >> 8;
		
	_writeCFGRegister();
}

/**
 * \brief Get cell discharge status
 * 
 * \param [uint8_t cell] the number of cell querying
 * \param [boolean *status] True for discharging, False for not discharging
 * 
 * \return 
 */
void LTC6803::getCellDischargeStatus(uint8_t cell, boolean *status)
{
	uint16_t mask;
	
	mask = (1 << (cell -1));
	_readCFGRegister();
	
	*status = (_ltCFGreg[1] + ((_ltCFGreg[2] & 0x0F) << 8)) & mask;
	
}

/**
 * \brief Return a uint8_t matrix with 1 for discharge True and 0 for discharge False
 * 
 * \param [uint8_t * cells] uint8_t matrix
 * 
 * \return [void]
 */
void LTC6803::getCellsDischargeStatus(uint8_t *cells)
{
	getCellsDischargeStatus(cells,12);
}

/**
 * \brief Return a uint8_t matrix with 1 for discharge True and 0 for discharge False. Check only [cells] first dischargers status
 * 
 * \param [uint8_t * cells] matrix with enough space for [cells] elements
 * \param [uint8_t cells] number of dischargers checked
 * 
 * \return [void]
 */
void LTC6803::getCellsDischargeStatus(uint8_t *cells, uint8_t elements)
{
	uint8_t i;
	uint16_t mask;
	
	_readCFGRegister();
	
	mask = 1;
	for(i=0;i<elements;i++) {		
		if(((_ltCFGreg[1] + ((_ltCFGreg[2] & 0x0F) << 8)) & mask) > 0) {
			cells[i] = 1;
			} else {
			cells[i] = 0;
		}
		mask <<= 1;
	}	
}

/**
 * \brief Set the cells Discharge bit indicated for matrix cells, 1 for set ON, and 0 for set OFF
 * 
 * \param [uint8_t * cells] matrix with discharge values of every cell
 * 
 * \return [void]
 */
void LTC6803::setCellsDischarge(uint8_t *cells)
{
	uint8_t i;
	uint16_t cellvalue;
	
	for(i=0;i<12;i++){
		if(cells[i]>0){
			cellvalue = 1 << i;
			_ltCFGreg[1] |= cellvalue;
			_ltCFGreg[2] |= (cellvalue & 0x0F00) >> 8;
			#ifdef TRACE_DISCHARGE
				celldischarge[i]++;
			#endif
		} else {
			cellvalue = ~(1 << i);
			_ltCFGreg[1] &= cellvalue;
			_ltCFGreg[2] &= (cellvalue & 0x0F00) >> 8;	
		}
	}
	_writeCFGRegister();

}

/**
 * \brief Send read voltages command, wait for response and read voltage register.
 * 
 * \param [uint16_t * voltages] Initialized matrix for return voltages values
 * 
 * \return [boolean] True if no error, false for errors
 */
boolean LTC6803::getImmVoltages(uint16_t *voltages)
{	
	
	_STOWADCommand(0);
	//_STCVDCCommand(0);  //Start Cell Voltage ADC conversions with discharge permitted
	//wait for read all voltages 13-21 ms
	delay(21);
	if(_readCVRegister(0)) { //Send read all Cell Voltages
		if(getLastVoltages(voltages)) { //Read all Cell Voltages in mV
			return(true);
		}
	}
	return(false);	
}


/**
 * \brief Send read voltages command, wait for response and read voltage register for n cell.
 * 
 * \param [uint16_t * voltages] Initialized matrix for return voltages values
 * \param [uint8_t cell] Cells to be read
 * 
 * \return [boolean] True if no error, false for erros
 */
boolean LTC6803::getImmVoltages(uint16_t *voltages, uint8_t cell)
{
	
	_STOWADCommand(0);
	//_STCVDCCommand(0);  //Start Cell Voltage ADC conversions with discharge permitted
	//wait for read all voltages 13-21 ms
	delay(21);
	if(_readCVRegister(0)) { //Send read all Cell Voltages
		if(getLastVoltages(voltages, cell)) { //Read [cell] numbers of Cells Voltages in mV
			return(true);
		}
	}
	return(false);	
}	


/**
 * \brief Send CLEAR Command and read the cell voltages values, the cell voltages values must be 0xFFF
 * 
 * \param [uint16_t * voltages] Initialized matrix for return voltages readed values
 * \param [uint8_t cell] Number of cells to be read
 * 
 * \return [boolean] True if no error, false for error.
 */
boolean LTC6803::sendCLEAR(uint16_t *voltages, uint8_t cell)
{
	_STOWADCommand(0x0D); //Send CLEAR Command
	//delay(2);
	//_STCVDCCommand(0);  //Start Cell Voltage ADC conversions with discharge permitted
	//wait for read all voltages 13-21 ms
	delay(21);
	if(_readCVRegister(0)) { //Send read all Cell Voltages
		if(getLastVoltages(voltages, cell)) { //Read [cell] numbers of Cells Voltages in mV
			return(true);
		}
	}
	return(false);	
}

/**
 * \brief Get the Thermal ShutDown Status Bit, for get valid value, a Temperature Register must be read before
 * 
 * \param [void ]
 * 
 * \return [boolean] The Status of the bit, True for 1, False for 0
 */
boolean LTC6803::getTHSDStatus(void)
{
	if((_ltTMPreg[5] & 0x10) > 0) {
		return(true);
	}
	return(false);
}




#ifdef TRACE_ERROR
/**
 * \brief Return the number of operations of writing ans reading with LTC
 * 
 * \param void
 * 
 * \return [uint32_t]
 */
uint32_t LTC6803::getNumberofOperations(void)
{
	return(_numberofoperations);	
}

/**
 * \brief Return the number of error happened
 * 
 * \param void
 * 
 * \return uint16_t
 */
uint16_t LTC6803::getNumberofError(void)
{
	return(_numberoferror);		
}

/**
 * \brief Return the last Error
 * 
 * \param void
 * 
 * \return uint8_t
 */
uint8_t LTC6803::getLastError(void)
{
	return(_lasterror);	
}

/**
 * \brief This function return the error Flag
 * 
 * \param void
 * 
 * \return boolean
 */
boolean LTC6803::isError(void)
{
	return(_error);
}



#endif // TRACE_ERROR

