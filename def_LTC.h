/*//////////////////////////////////////////////////////////////////////////////////////////////////////////
//																										//
//		DEFINICI�N DE COMANDOS DE LOS MONITORES DE CELDA PARA LTC 6803-4									//
//																										//
//		11/08/2012																						//
//																										//
//////////////////////////////////////////////////////////////////////////////////////////////////////////*/

#ifndef DEF_LTC_H
#define DEF_LTC_H

// Addressable Command/Write or Read
#define		ACRW				0x80		//First 8 bits of command/write or read

//Comandos de configuraci�n
#define 	WRCFG 				0x01		//Escribir grupo de registros de configuraci�n
#define 	WRCFG_PEC			0XC7		//Pec 

//Comandos de lectura de registros
#define		RDCFG 				0x02		//Leer grupo de registros de configuraci�n
#define		RDCFG_PEC			0xCE		//Pec

#define		RDCV  				0x04		//Leer grupo de registros de voltajes de celda
#define		RDCV_PEC 			0xDC		//Pec

#define		RDCVA				0x06		//Leer grupo de registros de voltajes de celda 1-4
#define		RDCVA_PEC			0xD2		//Pec	

#define		RDCVB 				0x08		//Leer grupo de registros de voltajes de celda 5-8
#define		RDCVB_PEC			0xF8		//Pec

#define		RDCVC 				0x0A		//Leer grupo de registros de voltajes de celda 9-12
#define		RDCV_C_PEC			0xF6		//Pec

#define		RDFLG 				0x0C		//Leer grupo de registros de flags
#define		RDFLG_PEC			0xE4		//Pec

#define		RDTMP 				0x0E		//Leer grupo de registros de temperatura
#define		RDTMP_PEC			0xEA		//Pec

//Comenzar conversi�n de voltajes de celda y poll status
#define		STCVADXX			0x10		//Comenzar conversi�n de voltaje en todas las celdas
#define		STCVADXX_PEC		0xB0		//Pec
#define		STCVAD01			0x11		//Comenzar conversi�n de voltaje de la celda N�1
#define		STCVAD01_PEC		0xB7		//Pec
#define		STCVAD02			0x12		//Comenzar conversi�n de voltaje de la celda N�2
#define		STCVAD02_PEC		0xBE		//Pec
#define		STCVAD03			0x13		//Comenzar conversi�n de voltaje de la celda N�3
#define		STCVAD03_PEC		0xB9		//Pec
#define		STCVAD04			0x14		//Comenzar conversi�n de voltaje de la celda N�4
#define		STCVAD04_PEC		0xAC		//Pec
#define		STCVAD05			0x15		//Comenzar conversi�n de voltaje de la celda N�5
#define		STCVAD05_PEC		0xAB		//Pec
#define		STCVAD06			0x16		//Comenzar conversi�n de voltaje de la celda N�6
#define		STCVAD06_PEC		0xA2		//Pec
#define		STCVAD07			0x17		//Comenzar conversi�n de voltaje de la celda N�7
#define		STCVAD07_PEC		0xA5		//Pec
#define		STCVAD08			0x18		//Comenzar conversi�n de voltaje de la celda N�8
#define		STCVAD08_PEC		0x88		//Pec
#define		STCVAD09			0x19		//Comenzar conversi�n de voltaje de la celda N�9
#define		STCVAD09_PEC		0x8F		//Pec
#define		STCVAD10			0x1A		//Comenzar conversi�n de voltaje de la celda N�10
#define		STCVAD10_PEC		0x86		//Pec
#define		STCVAD11			0x1B		//Comenzar conversi�n de voltaje de la celda N�11
#define		STCVAD11_PEC		0x81		//Pec
#define		STCVAD12			0x1C		//Comenzar conversi�n de voltaje de la celda N�12
#define		STCVAD12_PEC		0x94		//Pec
#define		CLEAR_FF			0x1D		//Clear
#define		CLEAR_FF_PEC		0x93		//Pec
#define		STCVAD55			0x1E		//Auto test de celdas. CV=0x555
#define		STCVAD55_PEC		0x9A		//Pec
#define		STCVADAA			0x1F		//Auto test de celdas. CV=0xAAA
#define		STCVADAA_PEC		0x9D		//Pec

//Comenzar conversi�n A/D para comprobar conductores abiertos y poll status
#define		STOWADXX			0x20		//Comenzar comprobaci�n de conductores abiertos en todas las celdas
#define		STOWADXX_PEC		0x20		//Pec
#define		STOWAD01			0x21		//Comenzar comprobaci�n de conductor abierto de la celda N�1
#define		STOWAD01_PEC		0x27		//Pec
#define		STOWAD02			0x22		//Comenzar comprobaci�n de conductor abierto de la celda N�2
#define		STOWAD02_PEC		0x2E		//Pec
#define		STOWAD03			0x23		//Comenzar comprobaci�n de conductor abierto de la celda N�3
#define		STOWAD03_PEC		0x29		//Pec
#define		STOWAD04			0x24		//Comenzar comprobaci�n de conductor abierto de la celda N�4
#define		STOWAD04_PEC		0x3C		//Pec
#define		STOWAD05			0x25		//Comenzar comprobaci�n de conductor abierto de la celda N�5
#define		STOWAD05_PEC		0x3B		//Pec
#define		STOWAD06			0x26		//Comenzar comprobaci�n de conductor abierto de la celda N�6
#define		STOWAD06_PEC		0x32		//Pec
#define		STOWAD07			0x27		//Comenzar comprobaci�n de conductor abierto de la celda N�7
#define		STOWAD07_PEC		0x35		//Pec
#define		STOWAD08			0x28		//Comenzar comprobaci�n de conductor abierto de la celda N�8
#define		STOWAD08_PEC		0x18		//Pec
#define		STOWAD09			0x29		//Comenzar comprobaci�n de conductor abierto de la celda N�9
#define		STOWAD09_PEC		0x1F		//Pec
#define		STOWAD10			0x2A		//Comenzar comprobaci�n de conductor abierto de la celda N�10
#define		STOWAD10_PEC		0x16		//Pec
#define		STOWAD11			0x2B		//Comenzar comprobaci�n de conductor abierto de la celda N�11
#define		STOWAD11_PEC		0x11		//Pec
#define		STOWAD12			0x2C		//Comenzar comprobaci�n de conductor abierto de la celda N�12
#define		STOWAD12_PEC		0x04		//Pec

//Comenzar conversi�n A/D de sensores de temperatura y poll status
#define		STTMPADXX			0x30		//Comenzar conversi�n de todas las entradas de temperatura
#define		STTMPADXX_PEC		0x50		//Pec
#define		STTMPAD01			0x31		//Comenzar conversi�n de temperatura exterior 1 solamente
#define		STTMPAD01_PEC		0x57		//Pec
#define		STTMPAD02			0x32		//Comenzar conversi�n de temperatura exterior 2 solamente
#define		STTMPAD02_PEC		0x5E		//Pec
#define		STTMPAD03			0x33		//Comenzar conversi�n de temperatura interna solamente
#define		STTMPAD03_PEC		0x59		//Pec
#define		STTMPAD55			0x3E		//Auto test de celdas. CV=0x555
#define		STTMPAD55_PEC		0x7A		//Pec
#define		STTMPADAA			0x3F		//Auto test de celdas. CV=0xAAA
#define		STTMPADAA_PEC		0x7D		//Pec

//Poll del estado de los conversores
#define		PLADC				0x40		//Comprobar estado de los conversores A/D
#define		PLADC_PEC			0x07		//Pec

//Poll del estado de la interrupci�n
#define		PLINT				0x50		//Comprobar el estado de la interrupci�n
#define		PLINT_PEC			0x77		//Pec

//Inicio de diagn�stico y poll status
#define		DAGN				0x52		//Comenzar el diagn�stico
#define		DAGN_PEC			0x79		//Pec

//Lectura del registro de diagn�stico
#define		RDDGNR				0x54		//Leer el diagn�stico
#define		RDDGNR_PEC			0x6B		//Pec	

//Comenzar conversi�n de voltajes de celda CON DESCARGA y poll status
#define		STCVDCXX			0x60		//Comenzar conversi�n de voltaje en todas las celdas
#define		STCVDCXX_PEC		0xE7		//Pec
#define		STCVDC01			0x61		//Comenzar conversi�n de voltaje de la celda N�1
#define		STCVDC01_PEC		0xE0		//Pec
#define		STCVDC02			0x62		//Comenzar conversi�n de voltaje de la celda N�2
#define		STCVDC02_PEC		0xE9		//Pec
#define		STCVDC03			0x63		//Comenzar conversi�n de voltaje de la celda N�3
#define		STCVDC03_PEC		0xEE		//Pec
#define		STCVDC04			0x64		//Comenzar conversi�n de voltaje de la celda N�4
#define		STCVDC04_PEC		0xFB		//Pec
#define		STCVDC05			0x65		//Comenzar conversi�n de voltaje de la celda N�5
#define		STCVDC05_PEC		0xFC		//Pec
#define		STCVDC06			0x66		//Comenzar conversi�n de voltaje de la celda N�6
#define		STCVDC06_PEC		0xF5		//Pec
#define		STCVDC07			0x67		//Comenzar conversi�n de voltaje de la celda N�7
#define		STCVDC07_PEC		0xF2		//Pec
#define		STCVDC08			0x68		//Comenzar conversi�n de voltaje de la celda N�8
#define		STCVDC08_PEC		0xDF		//Pec
#define		STCVDC09			0x69		//Comenzar conversi�n de voltaje de la celda N�9
#define		STCVDC09_PEC		0xD8		//Pec
#define		STCVDC10			0x6A		//Comenzar conversi�n de voltaje de la celda N�10
#define		STCVDC10_PEC		0xD1		//Pec
#define		STCVDC11			0x6B		//Comenzar conversi�n de voltaje de la celda N�11
#define		STCVDC11_PEC		0xD6		//Pec
#define		STCVDC12			0x6C		//Comenzar conversi�n de voltaje de la celda N�12
#define		STCVDC12_PEC		0xC3		//Pec

//Comenzar conversi�n A/D para comprobar conductores abiertos CON DESCARGA y poll status
#define		STOWDCXX			0x70		//Comenzar comprobaci�n de conductores abiertos en todas las celdas
#define		STOWDCXX_PEC		0x97		//Pec
#define		STOWDC01			0x71		//Comenzar comprobaci�n de conductor abierto de la celda N�1
#define		STOWDC01_PEC		0x90		//Pec
#define		STOWDC02			0x72		//Comenzar comprobaci�n de conductor abierto de la celda N�2
#define		STOWDC02_PEC		0x99		//Pec
#define		STOWDC03			0x73		//Comenzar comprobaci�n de conductor abierto de la celda N�3
#define		STOWDC03_PEC		0x9E		//Pec
#define		STOWDC04			0x74		//Comenzar comprobaci�n de conductor abierto de la celda N�4
#define		STOWDC04_PEC		0x8B		//Pec
#define		STOWDC05			0x75		//Comenzar comprobaci�n de conductor abierto de la celda N�5
#define		STOWDC05_PEC		0x8C		//Pec
#define		STOWDC06			0x76		//Comenzar comprobaci�n de conductor abierto de la celda N�6
#define		STOWDC06_PEC		0x85		//Pec
#define		STOWDC07			0x77		//Comenzar comprobaci�n de conductor abierto de la celda N�7
#define		STOWDC07_PEC		0x82		//Pec
#define		STOWDC08			0x78		//Comenzar comprobaci�n de conductor abierto de la celda N�8
#define		STOWDC08_PEC		0xAF		//Pec
#define		STOWDC09			0x79		//Comenzar comprobaci�n de conductor abierto de la celda N�9
#define		STOWDC09_PEC		0xA8		//Pec
#define		STOWDC10			0x7A		//Comenzar comprobaci�n de conductor abierto de la celda N�10
#define		STOWDC10_PEC		0xA1		//Pec
#define		STOWDC11			0x7B		//Comenzar comprobaci�n de conductor abierto de la celda N�11
#define		STOWDC11_PEC		0xA6		//Pec
#define		STOWDC12			0x7C		//Comenzar comprobaci�n de conductor abierto de la celda N�12
#define		STOWDC12_PEC		0xB3		//Pec


//Values for CFG Registers
//											//CDC			UV/OV COMPARATOR PERIOD				VREF POWERED DOWN BETWEEN MEASUREMENTS	CELL VOLTAGE MEASUREMENT TIME
#define		CDC0				B00000000	//0(Default)	N/A (Comparator Off)Standby Mode		Yes										N/A
#define		CDC1				B00000001	//1				N/A (Comparator Off)					No										13ms
#define		CDC2				B00000010	//2				13ms									No										13ms
#define		CDC3				B00000011	//3				130ms									No										13ms
#define		CDC4				B00000100	//4				500ms									No										13ms
#define		CDC5				B00000101	//5				130ms									Yes										21ms
#define		CDC6				B00000110	//6				500ms									Yes										21ms
#define		CDC7				B00000111	//7				2000ms									Yes										21ms


#define		CELL10				B00001000	// 10 Cell Mode
#define		CELL12				B00000000	// 12 Cell mode (default)

#define		LVLPL				B00010000	// Level Polling
#define		TGPVL				B00000000	// Toggle polling (default)

#define		GPIO1PD				B00000000	// GPIO1 pin pull-down ON
#define		GPIO1PDOFF			B00100000	// GPIO1 pin pull-down OFF (default)

#define		GPIO2PD				B00000000	// GPIO2 pin pull-down ON
#define		GPIO2PDOFF			B01000000	// GPIO2 pin pull-down OFF (default)

#define		MC1I				B00010000	// Mask Cell for interrupt of Cell 1 (Turn Off)
#define		MC2I				B00100000	// Mask Cell for interrupt of Cell 2 (Turn Off)
#define		MC3I				B01000000	// Mask Cell for interrupt of Cell 3 (Turn Off)
#define		MC4I				B10000000	// Mask Cell for interrupt of Cell 4 (Turn Off)
#define		MC5I				B00000001	// Mask Cell for interrupt of Cell 5 (Turn Off)
#define		MC6I				B00000010	// Mask Cell for interrupt of Cell 6 (Turn Off)
#define		MC7I				B00000100	// Mask Cell for interrupt of Cell 7 (Turn Off)
#define		MC8I				B00001000	// Mask Cell for interrupt of Cell 8 (Turn Off)
#define		MC9I				B00010000	// Mask Cell for interrupt of Cell 9 (Turn Off)
#define		MC10I				B00100000	// Mask Cell for interrupt of Cell 10 (Turn Off)
#define		MC11I				B01000000	// Mask Cell for interrupt of Cell 11 (Turn Off)
#define		MC12I				B10000000	// Mask Cell for interrupt of Cell 12 (Turn Off)

// Values for Operation Errors
#define		E_readCFGRegister	0x01		// Error reading Configuration Register
#define		E_readFLGRegister	0x02		// Error reading Flag Register
#define		E_readCVRegister	0x03		// Error reading Cell Value Register
#define		E_readCVARegister	0x04		// Error reading Cell A Value Register
#define		E_readCVBRegister	0x05		// Error reading Cell B Value Register
#define		E_readCVCRegister	0x06		// Error reading Cell C Value Register
#define		E_readTMPRegister	0x07		// Error reading Temp Register
#define		E_readDIAGRegister	0x08		// Error reading DIAG Register


#define		CMPMVOFFSET			1000		//Offset in mV for comparative between Cells and search number of Connected Cells

#define		HIGHCS				1			// Use for send command and High CS after
#define		LOWCS				0			// Use for send command and Keep LOW CS after

#define		HIGHSDO				1			// Use for Read MISO Status
#define		LOWSDO				0			// USe for Read MISO Status


#endif //  DEF_LTC_H