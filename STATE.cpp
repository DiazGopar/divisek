/* 
* STATE.cpp
*
* Created: 10/09/2014 13:23:48
* Author: Gopar
*/


#include "STATE.h"

// default constructor

 STATE::STATE(const uint8_t _n_voltages /*= 12*/, const uint8_t _n_temps /*= 3*/, const uint8_t _n_states /*= 32*/)
{
	uint8_t _n_currents = 2;
	
	_indexsoc = 0; //
	_indexvoltages = 0;
	_indexloadcurrents = 0;
	_indexchargecurrents = 0;
	_indextemps = 0;
	_indexcontroldischargers = 0;
	_flagsoc = 0;
	_flagvoltages = 0;
	_flagloadcurrents = 0;
	_flagchargecurrents = 0;
	_flagtemps = 0;
	_numberofcolumns = _n_states;
	_numberofvoltages = _n_voltages;
	_numberofcurrents = _n_currents;
	_numberoftemps = _n_temps;
	_controlcharge = 0;
	_controlload = 0;

	
	/* Allocating memory for SOC values */
	_matrixsoc = (int16_t **) malloc(_n_voltages * sizeof(int16_t *));
	for(int i=0;i<_n_voltages;i++) {
		_matrixsoc[i] = (int16_t*) calloc(_n_states,sizeof(int16_t));
	}
	
	/* Allocating memory for voltages values */
	_matrixvoltages = (int16_t **) malloc(_n_voltages * sizeof(int16_t *));
	for(int i=0;i<_n_voltages;i++) {
		_matrixvoltages[i] = (int16_t*) calloc(_n_states,sizeof(int16_t));
	}
	
	/* Allocating memory for currents values */ //0 for CHARGE and 1 for LOAD
	_matrixcurrents = (int16_t **) malloc(_n_currents * sizeof(int16_t *));
	for(int i=0;i<_n_currents;i++) {
		_matrixcurrents[i] = (int16_t*) calloc(_n_states,sizeof(int16_t));
	}
	
	/* Allocating memory for temperatures values */
	_matrixtemps = (int16_t **) malloc(_n_temps * sizeof(int16_t *));
	for(int i=0;i<_n_temps;i++) {
		_matrixtemps[i] = (int16_t*) calloc(_n_states,sizeof(int16_t));
	}
	
	/* Allocating memory for Control Dischargers*/
	_matrixcontroldischargers = (uint8_t **) malloc(_n_voltages * sizeof(uint8_t *));
	for(int i=0;i<_n_voltages;i++) {
		_matrixcontroldischargers[i] = (uint8_t*) calloc(_n_states,sizeof(uint8_t));
	}
	/* Allocating memory for Dischargers status */
	_matrixdischargers = (uint8_t *) calloc(_n_voltages, sizeof(uint8_t));
	
	/* Allocating memory for Flag Cells status */
	_flagcells = (uint8_t *) calloc(_n_voltages, sizeof(uint8_t));
	
	/* DEBUG Allocating memory for temp cell dischargers status */	
	_dischargers = (uint8_t *) calloc(_n_voltages, sizeof(uint8_t));
	
	_SerialComm1 = new SerialComm();
}

// default destructor
STATE::~STATE()
{
	uint8_t i;
	
	free(_dischargers);
	free(_flagcells);
	free(_matrixdischargers);
	
	for(i=0;i<_numberofvoltages;i++) {
		free(_matrixcontroldischargers[i]);
	}
	free(*_matrixcontroldischargers);
	for(i=0;i<_numberoftemps;i++) {	
		free(_matrixtemps[i]);
	}
	free(*_matrixtemps);
	for(i=0;i<_numberofcurrents;i++) {
		free(_matrixcurrents[i]);
	}
	free(*_matrixcurrents);
	for(i=0;i<_numberofvoltages;i++) {
		free(_matrixvoltages[i]);
	}
	free(*_matrixvoltages);
	for(i=0;i<_numberofvoltages;i++) {
		free(_matrixsoc[i]);
	}
	free(*_matrixsoc);
	
} //~STATE


/**
 * \brief Add Voltages SOC into matrixsoc
 * 
 * \param [int16_t * elements] pointer to Calculated SOC voltages
 * \param [uint8_t n_elements] number of elements to insert
 * 
 * \return [void]
 */
void STATE::addSoc(int16_t *elements, uint8_t n_elements)
{
	int8_t values, firstelement;
	
	if(++_indexsoc > _numberofcolumns - 1) {
		_flagsoc = 1;
		_indexsoc = 0;
	}
	
	values = (n_elements > _numberofvoltages) ? _numberofvoltages : n_elements;
	
	for(uint8_t i=0;i<values;i++) {
		_matrixsoc[i][_indexsoc] = elements[i];
	}
	// Which element in circular matrix are first, the last all always _indexvoltages
	if(_flagsoc > 0) { // the matrix has been rewrote
		firstelement = (_indexsoc == _numberofcolumns) ? (0) : (_indexsoc + 1);
	} else {
		firstelement = 0;
	}
	

}

/**
 * \brief Add Voltages measures into matrixvoltages
 * 
 * \param [int16_t * elements] pointer to voltages
 * \param [uint8_t n_elements] number of elements to insert
 * 
 * \return [void]
 */
void STATE::addVoltages(int16_t *elements, uint8_t n_elements)
{
	int8_t values, firstelement;
	
	
	if(++_indexvoltages > _numberofcolumns - 1) {
		_flagvoltages = 1;
		_indexvoltages = 0;
	}
	
	values = (n_elements > _numberofvoltages) ? _numberofvoltages : n_elements;
	
	for(uint8_t i=0;i<values;i++) {
		_matrixvoltages[i][_indexvoltages] = elements[i];
	}
	// Which element in circular matrix are first, the last all always _indexvoltages
	if(_flagvoltages > 0) { // the matrix has been rewrote
		firstelement = (_indexvoltages == _numberofcolumns - 1) ? (0) : (_indexvoltages + 1); 
	} else {
		firstelement = 0;
	}
		
}

/**
 * \brief Add a LOAD Current measure
 * 
 * \param [int16_t element]
 * 
 * \return [void]
 */
void STATE::addLOADCurrent(int16_t element)
{
	_addCurrent(element,1);
}

/**
 * \brief 
 * 
 * \param [int16_t element]
 * 
 * \return [void]
 */
void STATE::addCHARGECurrent(int16_t element)
{
	_addCurrent(element,0);	
}

/**
 * \brief Add Temperatures measures into matrixtemps
 * 
 * \param [int16_t * elements] pointer to temperatures
 * \param [uint8_t n_elements] number of elements to insert
 * 
 * \return [void]
 */
void STATE::addTemps(int16_t *elements, uint8_t n_elements)
{
	int8_t values, firstelement;
	
	if(++_indextemps > _numberofcolumns - 1) {
		_flagtemps = 1;
		_indextemps = 0;
	}
	
	values = (n_elements > _numberoftemps) ? _numberoftemps : n_elements;
	
	for(uint8_t i=0;i<values;i++) {
		_matrixtemps[i][_indextemps] = elements[i];
	}
	// Which element in circular matrix are first, the last all always _indextemps
	if(_flagtemps > 0) { // the matrix has been rewrote
		firstelement = (_indextemps == _numberoftemps - 1) ? (0) : (_indextemps + 1);
	} else {
		firstelement = 0;
	}

}

/**
 * \brief Add currents measures into matrixcurrents
 * 
 * \param [int16_t element] current value
 * \param [uint8_t type] matrix index of 0 for CHARGE and 1 for LOAD
 * 
 * \return [void]
 */
void STATE::_addCurrent(int16_t element, uint8_t type)
{
	int8_t firstelement;
			
	if(type > 0) {
		if(++_indexloadcurrents > _numberofcolumns - 1) {
			_flagloadcurrents = 1;
			_indexloadcurrents = 0;
		}		
		_matrixcurrents[1][_indexloadcurrents] = element;		
		// Which element in circular matrix are first, the last all always _indexcurrents
		if(_flagloadcurrents > 0) { // the matrix has been rewrote
			firstelement = (_indexloadcurrents == _numberofcolumns - 1) ? (0) : (_indexloadcurrents + 1);
		} else {
			firstelement = 0;
		}

	} else{
		if(++_indexchargecurrents > _numberofcolumns - 1) {
			_flagchargecurrents = 1;
			_indexchargecurrents = 0;
		}		
		_matrixcurrents[0][_indexchargecurrents] = element;	
		// Which element in circular matrix are first, the last all always _indexcurrents
		if(_flagchargecurrents > 0) { // the matrix has been rewrote
			firstelement = (_indexchargecurrents == _numberofcolumns - 1) ? (0) : (_indexchargecurrents + 1);
		} else {
			firstelement = 0;
		}
			
	}		
}

/**
 * \brief Add Discharge Cell Status in _matrixdischargers
 * 
 * \param [uint8_t * cells] discharge matrix values
 * 
 * \return [void]
 */
void STATE::addDischargers(uint8_t *cells)
{
	uint8_t i;
	
	for(i=0;i<_numberofvoltages;i++){
		_matrixdischargers[i] = cells[i];	
	}
}

/**
 * \brief 
 * 
 * \param [uint8_t status]
 * 
 * \return [void]
 */
void STATE::addLOADStatus(uint8_t status)
{
	_loadstatus = status;	
}

/**
 * \brief 
 * 
 * \param [uint8_t status]
 * 
 * \return [void]
 */
void STATE::addCHARGEStatus(uint8_t status)
{
	_chargestatus = status;
}


/**
 * \brief 
 * 
 * \param [uint8_t * elements]
 * \param [uint8_t n_elements]
 * 
 * \return [void]
 */
void STATE::addControlDischargers(uint8_t *elements, uint8_t n_elements)
{
	int8_t values, firstelement;
	
	if(++_indexcontroldischargers > _numberofcolumns - 1) {
		_flagcontroldischargers = 1;
		_indexcontroldischargers = 0;
	}
	
	values = (n_elements > _numberofvoltages) ? _numberofvoltages : n_elements;
	
	for(uint8_t i=0;i<values;i++) {
		_matrixcontroldischargers[i][_indexcontroldischargers] = elements[i];
	}
	// Which element in circular matrix are first, the last all always _indexvoltages
	if(_flagcontroldischargers > 0) { // the matrix has been rewrote
		firstelement = (_indexcontroldischargers == _numberofcolumns - 1) ? (0) : (_indexcontroldischargers + 1);
	} else {
		firstelement = 0;
	}
	
}

/**
 * \brief 
 * 
 * \param [uint8_t cell]
 * \param [uint8_t value]
 * 
 * \return [void]
 */
void STATE::setControlCellDischarge(uint8_t cell, uint8_t value)
{
	_matrixcontroldischargers[cell][_indexcontroldischargers] = value;	
}



/**
 * \brief 
 * 
 * \param [uint8_t status]
 * 
 * \return [void]
 */
void STATE::addControlLOAD(uint8_t status)
{
	_controlload = status;	
}

/**
 * \brief 
 * 
 * \param [uint8_t status]
 * 
 * \return [void]
 */
void STATE::addControlCHARGE(uint8_t status)
{
	_controlcharge = status;
}

/**
 * \brief 
 * 
 * \param [void ]
 * 
 * \return [void]
 */
void STATE::calcFilters(void)
{
	for (filterIterator it = _filterlist.begin(); it != _filterlist.end(); it++) {
		(*it)->calculate(this);
	}
	
}

/**
 * \brief Add a New filter to the _filterlist
 * 
 * \param [FILTER * filter] Filter to be added
 * 
 * \return [void]
 */
void STATE::addFilter(FILTER *filter)
{
	_filterlist.push_back(filter);
}

/**
 * \brief Get a Pointer to a bi dimensional matrix with SOC values
 * 
 * \param [void ]
 * 
 * \return [int16_t **] the pointer
 */
int16_t ** STATE::getSoc(void)
{
	return(_matrixsoc);
}

/**
 * \brief Get Pointer to a bi dimensional matrix with voltages values
 * 
 * \param [void ]
 * 
 * \return [int16_t **] the pointer
 */
int16_t ** STATE::getVoltages(void)
{
	return(_matrixvoltages);
}

/**
 * \brief Get pointer to Load Currents matrix
 * 
 * \param [void ]
 * 
 * \return [int16_t *] pointer to a matrix of load currents
 */
int16_t * STATE::getLOADCurrents(void)
{
	return(_matrixcurrents[1]);
}

/**
 * \brief Get pointer to Charge Currents matrix
 * 
 * \param [void ]
 * 
 * \return [int16_t *] pointer to a matrix of load currents
 */
int16_t * STATE::getCHARGECurrents(void)
{
	return(_matrixcurrents[0]);
}

/**
 * \brief Get pointer to Temperatures matrix
 * 
 * \param [void ]
 * 
 * \return [int16_t **]
 */
int16_t ** STATE::getTemps(void)
{
	return(_matrixtemps);
}


/**
 * \brief Set Flags in Cell Flag MAtrix
 * 
 * \param [uint8_t cell] number of Cell
 * \param [uint8_t flag] Flag value
 * 
 * \return [void]
 */
void STATE::setCellFlag(uint8_t cell, uint8_t flag)
{
	_flagcells[cell] = flag;	
}

/**
 * \brief Set OverVoltage Cell Flag
 * 
 * \param [uint8_t cell] Cell Number
 * \param [uint8_t value] if value is 1, flag is set, if 0 unset
 * 
 * \return [void]
 */
void STATE::setCellFlagOV(uint8_t cell, uint8_t value)
{
	if(value > 0){
		_flagcells[cell] |= 0x01;
	} else {
		_flagcells[cell] &= ~0x01;
	}
}

/**
 * \brief Set UnderVltage Cell Flag
 * 
 * \param [uint8_t cell] Cell number
 * \param [uint8_t value] if value is 1, flag is set, if o unset
 * 
 * \return [void]
 */
void STATE::setCellFlagUV(uint8_t cell, uint8_t value)
{
	if(value > 0){
		_flagcells[cell] |= 0x02;
		} else {
		_flagcells[cell] &= ~0x02;
	}	
}

/**
 * \brief 
 * 
 * \param [uint8_t cell]
 * \param [uint8_t value]
 * 
 * \return [void]
 */
void STATE::setCellFlagOW(uint8_t cell, uint8_t value)
{
	if(value > 0){
		_flagcells[cell] |= 0x04;
		} else {
		_flagcells[cell] &= ~0x04;
	}	
}


/**
 * \brief Get Cell Flag Status
 * 
 * \param [uint8_t cell] The Cell number 
 * 
 * \return [uint8_t] The flags value (bit 0 - OV) (bit 1 - UV) (bit 2 - Open Wire)
 */
uint8_t STATE::getCellFlag(uint8_t cell)
{
	if(cell<_numberofvoltages) {
		return(_flagcells[cell]);	
	}
	return(0xFF); // Invalid Cell Number
}



uint8_t STATE::getIndexSoc(void)
{
	return(_indexsoc);	
}


uint8_t STATE::getIndexVoltages(void)
{
	return(_indexvoltages);	
}

uint8_t STATE::getIndexLOADCurrents(void)
{
	return(_indexloadcurrents);
}

uint8_t STATE::getIndexCHARGECurrents(void)
{
	return(_indexchargecurrents);
}


uint8_t STATE::getIndexTemps(void)
{
	return(_indextemps);
}

uint8_t STATE::getNumberofColumns(void)
{
	return(_numberofcolumns);
}

uint8_t STATE::getNumberofVoltages(void)
{
	return(_numberofvoltages);
}

uint8_t STATE::getNumberofTemps(void)
{
	return(_numberoftemps);
}


void STATE::actuate(void)
{
	uint8_t  i, haschanged;
	
	//LOAD and Charge First
	_control->set12VON();
	if(_controlload > 0){
		_control->setdischargeDriverON();
	} else {
		_control->setdischargeDriverOFF();
	}
	
	if(_controlcharge > 0){
		_control->setchargeDriverON();
	} else {
		_control->setchargeDriverOFF();
	}	
	//DISCHARGE Cells
	//TODO:
	
	//dischargers = (uint8_t *) calloc(_numberofvoltages, sizeof(uint8_t));
	
	haschanged = 0;
	for(i=0;i<_numberofvoltages;i++) {
		if(_matrixcontroldischargers[i][_indexcontroldischargers] != _matrixdischargers[i]) {
			_dischargers[i] = _matrixcontroldischargers[i][_indexcontroldischargers];
			haschanged++;
		}
		//_dischargers[i] = _matrixcontroldischargers[i][_indexcontroldischargers];
	}
	if(haschanged>0) {
		//DEBUG
		//Serial.println("=======SENDING CELL DISCHARGE COMMAND=======");
		_ltc->setCellDischargeValue(_dischargers,_numberofvoltages);
	}
	
	//free(dischargers);
		
}


void STATE::addControl(CONTROL *control)
{
	_control = control;	
}

void STATE::addLTC(LTC6803 *ltc)
{
	_ltc = ltc;	
}





void STATE::debug()
{
	uint8_t i,j;
	
	Serial.println("-------------STATUS----------------");
	for(i=0;i<_numberofvoltages;i++) {
		Serial.print("Voltage[");
		Serial.print(i);
		Serial.print("]: ");
		for(j=0;j<_numberofcolumns;j++){
			Serial.print(_matrixvoltages[i][j]);
			Serial.print(" ");	
		}
		Serial.println("");		
	}
	Serial.println("-----------------------------------");
	for(i=0;i<_numberofvoltages;i++) {
		Serial.print("SOC[");
		Serial.print(i);
		Serial.print("]: ");
		for(j=0;j<_numberofcolumns;j++){
			Serial.print(_matrixsoc[i][j]);
			Serial.print(" ");
		}
		Serial.println("");
	}
	for(i=0;i<_numberofvoltages;i++) {
		Serial.print("Cell Flag Status[");
		Serial.print(i);
		Serial.print("]: ");
		Serial.print(_flagcells[i]);
		Serial.println("");
	}
	for(i=0;i<_numberofvoltages;i++) {
		Serial.print("Discharge Cell Status[");
		Serial.print(i);
		Serial.print("]: ");		
		Serial.print(_matrixdischargers[i]);
		Serial.println("");
	}	
	for(i=0;i<_numberoftemps;i++) {
		Serial.print("Temps[");
		Serial.print(i);
		Serial.print("]: ");
		for(j=0;j<_numberofcolumns;j++){
			Serial.print(_matrixtemps[i][j]);
			Serial.print(" ");
		}
		Serial.println("");
	}
	Serial.print("CHARGE Current: ");
	for(j=0;j<_numberofcolumns;j++){
		Serial.print(_matrixcurrents[0][j]);
		Serial.print(" ");
	}
	Serial.println("");
	Serial.print("LOAD Current: ");
	for(j=0;j<_numberofcolumns;j++){
		Serial.print(_matrixcurrents[1][j]);
		Serial.print(" ");
	}
	Serial.println("");
	Serial.print("LOAD Channel Status: ");
	Serial.print(_loadstatus);
	Serial.println("");
	Serial.print("CHARGE Channel Status: ");
	Serial.print(_chargestatus);			
	Serial.println("");	
	
	Serial.println("-------------CONTROL----------------");
	for(i=0;i<_numberofvoltages;i++) {
		Serial.print("Discharge Control[");
		Serial.print(i);
		Serial.print("]: ");
		for(j=0;j<_numberofcolumns;j++){
			Serial.print(_matrixcontroldischargers[i][j]);
			Serial.print(" ");
		}
		Serial.println("");
	}
	Serial.println("");
	Serial.print("Control LOAD Channel: ");
	Serial.print(_controlload);
	Serial.println("");
	Serial.print("Control CHARGE Channel: ");
	Serial.print(_controlcharge);
	Serial.println("");
}

/**
 * \brief Send Data to a Remote Host 
 * 
 * 
 * \return [void]
 */
void STATE::sendData()
{
//Voltages	[FLAG_BYTE]			1 byte	Start frame	
//			[VOLTS_FRAME]		1 byte	Voltages Frame type
//			[numberofvoltages]	1 byte	Numbers of cell voltages
//			[indexofcolumn]		1 byte	Index in status cell voltage matrix
//			[voltages..]		numberofvoltages * 2 bytes - 16 bits cell voltages values in bigendian notation
//			[FLAG_BYTE]			1 byte	End frame

	uint8_t LoByte, HiByte, i,  n;
	
		

	//Send a Cell Voltage Frame
	_SerialComm1->clear();
	_SerialComm1->begin(VOLTS_FRAME);
	_SerialComm1->add(_numberofvoltages);
	_SerialComm1->add(_indexvoltages);
	for(i=0;i<_numberofvoltages;i++) {
		LoByte = _matrixvoltages[i][_indexvoltages] & 0x00FF;
		HiByte = (_matrixvoltages[i][_indexvoltages] & 0xFF00) >> 8;
		_SerialComm1->add(HiByte);
		_SerialComm1->add(LoByte);
	}
	_SerialComm1->end();
	_SerialComm1->send();
	
	//Send a OV UV Flags Frame
	_SerialComm1->clear();
	_SerialComm1->begin(FLAGS_FRAME);
	_SerialComm1->add(_numberofvoltages);
	for(i=0;i<_numberofvoltages;i++) {
		_SerialComm1->add(_flagcells[i]);
	}
	_SerialComm1->end();
	_SerialComm1->send();
	
	//Send a  Cell Discharge Status Frame
	_SerialComm1->clear();
	_SerialComm1->begin(DISCH_FRAME);
	_SerialComm1->add(_numberofvoltages);
	for(i=0;i<_numberofvoltages;i++) {
		_SerialComm1->add(_matrixdischargers[i]);
	}
	_SerialComm1->end();
	_SerialComm1->send();
	
	//Send a Temperature Status Frame
	_SerialComm1->clear();
	_SerialComm1->begin(TEMPS_FRAME);
	_SerialComm1->add(_numberoftemps);
	_SerialComm1->add(_indextemps);
	for(i=0;i<_numberoftemps;i++) {
		LoByte = _matrixtemps[i][_indextemps] & 0x00FF;
		HiByte = (_matrixtemps[i][_indextemps] & 0xFF00) >> 8;
		_SerialComm1->add(HiByte);
		_SerialComm1->add(LoByte);
	}
	_SerialComm1->end();
	_SerialComm1->send();
	
	//Send a Current Status frame
	_SerialComm1->clear();
	_SerialComm1->begin(CURRENTS_FRAME);
	_SerialComm1->add(_numberofcurrents);
	_SerialComm1->add(_indexchargecurrents);
	for(i=0;i<_numberofcurrents;i++) {
		LoByte = _matrixcurrents[i][_indexchargecurrents] & 0x00FF;
		HiByte = (_matrixcurrents[i][_indexchargecurrents] & 0xFF00) >> 8;
		_SerialComm1->add(HiByte);
		_SerialComm1->add(LoByte);
	}
	_SerialComm1->end();
	_SerialComm1->send();	
	
	//Send a Channel Status frame
	_SerialComm1->clear();
	_SerialComm1->begin(CHANNEL_FRAME);
	_SerialComm1->add(2);
	_SerialComm1->add(_chargestatus);
	_SerialComm1->add(_loadstatus);
	_SerialComm1->end();
	_SerialComm1->send();
		

/*	

	
	buffer[n] = FLAG_BYTE;
	n++;
	buffer[n] = VOLTS_FRAME;
	n++;
	buffer[n] = _numberofvoltages;
	n++;
	buffer[n] = _indexvoltages;
	n++:
	for(i=0;i<_numberofvoltages;i++) {
		LoByte = _matrixvoltages[i][_indexvoltages] & 0x00FF;
		HiByte = (_matrixvoltages[i][_indexvoltages] & 0xFF00) >> 8;
		Serial.write(HiByte);
		Serial.write(LoByte);
	}
	
	
	Serial.write(FLAG_BYTE);
		Serial.write(_numberofvoltages);
		Serial.write(_indexvoltages);
		for(i=0;i<_numberofvoltages;i++) {
			LoByte = _matrixvoltages[i][_indexvoltages] & 0x00FF;
			HiByte = (_matrixvoltages[i][_indexvoltages] & 0xFF00) >> 8;
			Serial.write(HiByte);
			Serial.write(LoByte);			
		}
	Serial.write(FLAG_BYTE);
		
		
		PRIVATE void vFrameProcess(PDUM_thAPduInstance hAPdu)
		{
			PRIVATE uint16 u16Size;
			uint8 i,j, *pointer;
			uint8 data[100];

			u16Size = PDUM_u16APduInstanceGetPayloadSize(hAPdu);
			DBG_vPrintf(TRACE_APP, "APP: APP_taskEndPoint: vFrameProcess length: %d\n", u16Size);
			PDUM_vDBGPrintAPduInstance(hAPdu);
			pointer = PDUM_pvAPduInstanceGetPayload(hAPdu);

			i = 0;
			j = 0;
			data[j++] = 0x7E;
			while(i < u16Size)
			{
				if((pointer[i] != ESC_BYTE) && (pointer[i] != FLAG_BYTE)) {
					data[j++] = pointer[i];
					} else {
					data[j++] = ESC_BYTE;
					data[j++] = pointer[i] ^ 0x20;
				}
				i++;
			}
			data[j] = 0x7E;
			i = 0;
			while(i <= j)
			{
				UART_bPutChar(UART_UART,data[i]);
				//UART_bPrintf(UART_UART, "[%02x] ",data[i]);
				i++;
			}

			UART_bPrintf(UART_UART,"\n");
			UART_vStartTx(UART_UART);
		}

		
		
*/		
}





























