/* 
* FILTER.h
*
* Created: 03/09/2014 13:52:42
* Author: Gopar
*/


#ifndef __FILTER_H__
#define __FILTER_H__

#if defined(ARDUINO)
	#if ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
	#endif
#endif

#include <iterator>
#include <list>

class STATE;


class FILTER
{
//variables
public:
protected:
private:

//functions
public:
	FILTER();
	~FILTER();
	//virtual void add(int16_t *elements, uint8_t n_elements) = 0;
	//virtual int16_t get(uint8_t row, uint8_t col) = 0;
	//virtual int16_t *get(uint8_t row) = 0;
	virtual int16_t *getFiltered(void)  = 0;
	virtual uint8_t getNumberofFilteredValues(void)	= 0;
	//virtual int16_t getFiltered(uint8_t row) = 0;
	//virtual uint8_t getNumberofCols(void) = 0;
	//virtual uint8_t getNumberofRows(void) = 0;
	//virtual uint8_t getActualLine(void) = 0;
	/*virtual void calculate(	int16_t **_matrix, 
							uint8_t _numberofrows, 
							uint8_t _numberofcols,
							uint8_t firstrow, 
							uint8_t firstelement,
							uint8_t lastelement) = 0; */

	virtual void calculate(STATE *state) = 0;
														
	
protected:
private:
	FILTER( const FILTER &c );
	FILTER& operator=( const FILTER &c );

}; //FILTER


typedef std::list<FILTER*> filterList;
typedef filterList::iterator filterIterator;

#endif //__FILTER_H__
