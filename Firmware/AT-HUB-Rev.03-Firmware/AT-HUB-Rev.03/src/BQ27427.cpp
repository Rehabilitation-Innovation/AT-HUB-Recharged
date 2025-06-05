/******************************************************************************
Inspired by:

SparkFunBQ274xx.cpp
BQ274xx Arduino Library Main Source File
Jim Lindblom @ SparkFun Electronics
May 9, 2016
https://github.com/sparkfun/SparkFun_BQ274xx_Arduino_Library

******************************************************************************/

#include "Arduino.h"
#include <Wire.h>
#include "BQ27427.h"

/*****************************************************************************
 ************************** Initialization Functions *************************
 *****************************************************************************/
// Initializes class variables
BQ27427::BQ27427(uint16_t capacity, uint8_t deviceAddress, uint16_t designVoltage, uint16_t taperCurrent, uint16_t terminateVoltage, uint8_t chemId)
{
	_wire.addr = deviceAddress;
	_wire.bus = &_bq27427;

	_bq27427_config.i2c = _wire;
	_bq27427_config.design_voltage = designVoltage; 
	_bq27427_config.design_capacity = capacity; 
	_bq27427_config.taper_current = taperCurrent; 
	_bq27427_config.terminate_voltage = terminateVoltage; 
	_bq27427_config.chemistry_id = chemId; 
	_bq27427_config.lazy_loading = true;
}

// Initializes I2C and verifies communication with the BQ274xx.
bool BQ27427::begin(void)
{
	Wire.begin();

	uint16_t deviceID = 0;
	int rc = bq274xx_get_device_type(&_bq27427, &deviceID);
	if ( rc || deviceID !=BQ27427_DEVICE_ID) return false;

	rc = bq274xx_gauge_init(&_bq27427);
	if ( rc ) return false;

	rc = bq274xx_gauge_configure(&_bq27427);
	return (rc == 0);
}

/*****************************************************************************
 ********************** Battery Characteristics Functions ********************
 *****************************************************************************/

// Reads and returns the battery voltage
uint16_t BQ27427::voltage(void)
{	
	struct sensor_value v;
	int rc = bq274xx_sample_fetch(&_bq27427, SENSOR_CHAN_GAUGE_VOLTAGE);
	if ( rc ) return UINT16_MAX;

	return _bq27427_data.voltage;
}

// Reads and returns the specified current measurement
int16_t BQ27427::current(current_measure type)
{
	struct sensor_value v;
	int rc = bq274xx_sample_fetch(&_bq27427, SENSOR_CHAN_ALL);
	if ( rc ) return INT16_MIN;

	switch (type)
	{
		case AVG:
			return _bq27427_data.avg_current;
			break;

		case STBY:
			return _bq27427_data.stdby_current;
			break;

		case MAX:
			return _bq27427_data.max_load_current;
			break;
		
		default:
			return INT16_MIN;
	}
	
	return INT16_MIN;
}

// Reads and returns the specified capacity measurement
uint16_t BQ27427::capacity(capacity_measure type)
{
	struct sensor_value v;
	int rc = bq274xx_sample_fetch(&_bq27427, SENSOR_CHAN_ALL);
	if ( rc ) return UINT16_MAX;

	switch (type)
	{
	case REMAIN:
		return (_bq27427_data.remaining_charge_capacity);
		break;

	case FULL:
		return (_bq27427_data.full_charge_capacity);
		break;

	case AVAIL:
		return (_bq27427_data.nom_avail_capacity);
		break;

	case AVAIL_FULL:
		return (_bq27427_data.full_avail_capacity);
		break;

	default:
		return UINT16_MAX;
	}
}

// Reads and returns measured average power
int16_t BQ27427::power(void)
{
	struct sensor_value v;
	int rc = bq274xx_sample_fetch(&_bq27427, SENSOR_CHAN_GAUGE_AVG_POWER);
	if ( rc ) return INT16_MIN;

	return _bq27427_data.avg_power;
}

// Reads and returns specified state of charge measurement
uint16_t BQ27427::soc()
{
	struct sensor_value v;
	int rc = bq274xx_sample_fetch(&_bq27427, SENSOR_CHAN_GAUGE_STATE_OF_CHARGE);
	if ( rc ) return UINT16_MAX;

	return _bq27427_data.state_of_charge;
}

// Reads and returns specified state of health measurement
uint16_t BQ27427::soh()
{
	struct sensor_value v;
	int rc = bq274xx_sample_fetch(&_bq27427, SENSOR_CHAN_GAUGE_STATE_OF_HEALTH);
	if ( rc ) return UINT16_MAX;

	return _bq27427_data.state_of_health;
}

// Reads and returns specified temperature measurement
uint16_t BQ27427::temperature()
{
	struct sensor_value v;
	int rc = bq274xx_sample_fetch(&_bq27427, SENSOR_CHAN_GAUGE_TEMP);
	if ( rc ) return UINT16_MAX;

	return _bq27427_data.internal_temperature;
}

/*****************************************************************************
 *************************** Control Sub-Commands ****************************
 *****************************************************************************/

// Read the device type - should be 0x0421
uint16_t BQ27427::deviceType(void)
{
	uint16_t deviceID = 0;
	int rc = bq274xx_get_device_type(&_bq27427, &deviceID);
	return rc ? UINT16_MAX : deviceID;
}

