/* 
* UV_OV_ACT.cpp
*
* Created: 26/09/2014 12:46:01
* Author: Administrador
*/


#include "UV_OV_ACT.h"

// default constructor
UV_OV_ACT::UV_OV_ACT(uint8_t n_voltages, int16_t OV_value, int16_t UV_value)
{
	_values = (int16_t*) calloc(n_voltages,sizeof(int16_t));
	_burn_time = (uint8_t*) calloc(n_voltages,sizeof(uint8_t));
	_OV_value = OV_value;
	_UV_value = UV_value;
	_n_values = n_voltages;
	_factor = 150;
	for(uint8_t i=0;i<n_voltages;i++){
		_burn_time[i] = 16;
	}
	
} //UV_OV_ACT

 UV_OV_ACT::UV_OV_ACT()
{
	UV_OV_ACT(12,4100,3600);
}

// default destructor
UV_OV_ACT::~UV_OV_ACT()
{
	free(_burn_time);
	free(_values);
} //~UV_OV_ACT

void UV_OV_ACT::calculate(STATE *state)
{
	uint8_t n_elements, n_columns, i, j, changed;
	int32_t sum;
	int8_t index_min, belowOV, overUV;
	int16_t min_value;
	int16_t **values;
		
	
	n_elements = state->getNumberofVoltages();
	if(n_elements > _n_values){ //Check for not write outside allocated memory
		n_elements = _n_values;
	}
	n_columns = state->getNumberofColumns();
	
	values = state->getVoltages();
	min_value = 10000;
	
	//Calculate average with voltages
	for(i=0;i<n_elements;i++) {
		sum = 0;
		for(j=0;j<n_columns;j++) {
			sum += values[i][j];	
		}
		_values[i] = sum / n_columns;
		if(_values[i] <  min_value) { //Search for minimum cell value and its index
			min_value = _values[i];
			index_min = i;
		}
	}
	//Calculate average of LTC Temperature
	sum = 0;
	values = state->getTemps();
	for(i=0;i<n_columns;i++) {
		sum += values[2][i];
	}
	_ltc_temp = sum / n_columns;
	//DEBUG
	//Serial.print("LTC Temp: ");
	//Serial.println(_ltc_temp);
	
	//Search for OV or UV
	for(i=0;i<n_elements;i++) { //If at least one value
		if(_values[i] > _OV_value) {
			state->setCellFlagOV(i,1);
		}	else {
			if(_values[i] < (_OV_value - 50)) { //Hysteresis 100 mv
				state->setCellFlagOV(i,0);
			}
		}
		if(_values[i] < _UV_value){
			state->setCellFlagUV(i,1);
		} else {
			if(_values[i] > (_UV_value + 50)) { //Hysteresis 100 mv
				state->setCellFlagUV(i,0);
			}
		}
	}
	// Set LOAD/CHARGE Status in function of OV/UV cells status 
	state->addControlLOAD(1);
	state->addControlCHARGE(1);
	for(i=0;i<n_elements;i++) {
		if(state->getCellFlag(i) & 0x02){
			state->addControlLOAD(0);
		}
		if(state->getCellFlag(i) & 0x01){
			state->addControlCHARGE(0);
		}
	}
	
	//Search for not balanced cells
	if(_ltc_temp < 2512) {
		changed = 0;
		for(i=0;i<n_elements;i++){
			if(_values[i] > (_values[index_min] + _factor)) {
				if( _burn_time[i]==0) {
					_burn_time[i] = 32;
				} 
				else if(_burn_time[i]<16) {
					if((state->getCellFlag(i) & 0x02) == 0) { // Check UV Cell Flag
						state->setControlCellDischarge(i,1);
						_burn_time[i]--;	
					}
				} else { //Recovering time for thermal
					state->setControlCellDischarge(i,0); 
					_burn_time[i]--;	
				}	
				changed++;				
			} else {
				state->setControlCellDischarge(i,0);
				_burn_time[i] = 32;
			}
		}	
		if(changed == 0){
			if(_factor>10){
				_factor--;
			}
		}	
	} else { //OVER TEMPERATURE
		for(i=0;i<n_elements;i++){
			state->setControlCellDischarge(i,0);
		}
	} 
	//DEBUG
	//Serial.print("Factor: ");
	//Serial.println(_factor);
}

int16_t * UV_OV_ACT::getFiltered(void)
{
	return(_values);	
}

uint8_t UV_OV_ACT::getNumberofFilteredValues(void)
{
	return(_n_values); 	
}
