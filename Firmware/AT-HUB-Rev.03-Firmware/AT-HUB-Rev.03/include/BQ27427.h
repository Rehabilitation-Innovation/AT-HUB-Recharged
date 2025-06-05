#pragma once

#include "Arduino.h"
#include "driver/bq274xx.h"

#define BQ27427_I2C_ADDRESS 0x55

enum current_measure {
	AVG = 0,
	STBY = 1,
	MAX = 2
};

enum capacity_measure {
	REMAIN = 0,
	FULL = 1,
	AVAIL = 2,
	AVAIL_FULL = 3
};

class BQ27427 {
public:
	//////////////////////////////
	// Initialization Functions //
	//////////////////////////////
	/**
	    Initializes class variables
	*/
 BQ27427(	uint16_t capacity,
					uint8_t deviceAddress = BQ27427_I2C_ADDRESS, 
					uint16_t designVoltage = 4200, 
					uint16_t taperCurrent = 20, 
					uint16_t terminateVoltage = 3000,
          uint8_t chemId = BQ27427_CHEM_ID_A); // Lithium Cobalt Oxide (LiCoO₂)
	
	/**
	    Initializes I2C and verifies communication with the BQ274xx.
		Must be called before using any other functions.
		
		@return true if communication was successful.
	*/
	bool begin(void);
	

	/////////////////////////////
	// Battery Characteristics //
	/////////////////////////////
	/**
	    Reads and returns the battery voltage
		
		@return battery voltage in mV
	*/
	uint16_t voltage(void);
	
	/**
	    Reads and returns the specified current measurement in mA
		
		@param current_measure enum specifying current value to be read
		@return specified current measurement in mA. >0 indicates charging.
	*/
	int16_t current(current_measure type = AVG);
	
	/**
	    Reads and returns the specified capacity measurement
		
		@param capacity_measure enum specifying capacity value to be read
		@return specified capacity measurement in mAh.
	*/
	uint16_t capacity(capacity_measure type = REMAIN);
	
	/**
	    Reads and returns measured average power
		
		@return average power in mAh. >0 indicates charging.
	*/
	int16_t power(void);
	
	/**
		Reads and returns specified state of charge measurement
		@return specified state of charge measurement in %
	*/
	uint16_t soc();
	
	/**
		Reads and returns specified state of health measurement
		@return specified state of health measurement in %, or status bits
	*/
uint16_t soh();
	
	/**
		Reads and returns specified temperature measurement
		@return specified temperature measurement in degrees C
	*/
	uint16_t temperature();
	
	//////////////////////////
	// Control Sub-commands //
	//////////////////////////
	
	/**
	    Read the device type - should be 0x0421
		
		@return 16-bit value read from DEVICE_TYPE subcommand
	*/
	uint16_t deviceType(void);

	
private:
	struct bq274xx_config _bq27427_config;
	struct bq274xx_data _bq27427_data;
	struct i2c_dt_spec _wire;
	device _bq27427 = {&_bq27427_config, &_bq27427_data};
};

