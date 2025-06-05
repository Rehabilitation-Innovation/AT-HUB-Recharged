#pragma once

#include "arduino_adapter.h"
#include <stdint.h>
#include <stdbool.h>
#include "bq274xx_struct.h"

// #include <zephyr/drivers/i2c.h>
// #include <zephyr/drivers/gpio.h>
#include "dt-bindings/sensor/bq274xx.h"

/*** General Constant ***/
#define BQ274XX_UNSEAL_KEY_A 0x8000 /* Unseal code one on BQ27441-G1A and similar */
#define BQ274XX_UNSEAL_KEY_B 0x8000 /* Unseal code two on BQ27441-G1A and similar */
#define BQ27421_DEVICE_ID  0x0421
#define BQ27427_DEVICE_ID  0x0427

/*** Standard Commands ***/
#define BQ274XX_CMD_CONTROL        0x00 /* Control() register */
#define BQ274XX_CMD_TEMP           0x02 /* Temperature() */
#define BQ274XX_CMD_VOLTAGE        0x04 /* Voltage() */
#define BQ274XX_CMD_FLAGS          0x06 /* Flags() */
#define BQ274XX_CMD_NOM_CAPACITY   0x08 /* NominalAvailableCapacity() */
#define BQ274XX_CMD_AVAIL_CAPACITY 0x0A /* FullAvailableCapacity() */
#define BQ274XX_CMD_REM_CAPACITY   0x0C /* RemainingCapacity() */
#define BQ274XX_CMD_FULL_CAPACITY  0x0E /* FullChargeCapacity() */
#define BQ274XX_CMD_AVG_CURRENT    0x10 /* AverageCurrent() */
#define BQ274XX_CMD_STDBY_CURRENT  0x12 /* StandbyCurrent() */
#define BQ274XX_CMD_MAX_CURRENT    0x14 /* MaxLoadCurrent() */
#define BQ274XX_CMD_AVG_POWER      0x18 /* AveragePower() */
#define BQ274XX_CMD_SOC            0x1C /* StateOfCharge() */
#define BQ274XX_CMD_INT_TEMP       0x1E /* InternalTemperature() */
#define BQ274XX_CMD_SOH            0x20 /* StateOfHealth() */
#define BQ274XX_CMD_REM_CAP_UNFL   0x28 /* RemainingCapacityUnfiltered() */
#define BQ274XX_CMD_REM_CAP_FIL    0x2A /* RemainingCapacityFiltered() */
#define BQ274XX_CMD_FULL_CAP_UNFL  0x2C /* FullChargeCapacityUnfiltered() */
#define BQ274XX_CMD_FULL_CAP_FIL   0x2E /* FullChargeCapacityFiltered() */
#define BQ274XX_CMD_SOC_UNFL       0x30 /* StateOfChargeUnfiltered() */

/*** Control Sub-Commands ***/
#define BQ274XX_CTRL_STATUS          0x0000
#define BQ274XX_CTRL_DEVICE_TYPE     0x0001
#define BQ274XX_CTRL_FW_VERSION      0x0002
#define BQ274XX_CTRL_DM_CODE         0x0004
#define BQ274XX_CTRL_PREV_MACWRITE   0x0007
#define BQ274XX_CTRL_CHEM_ID         0x0008
#define BQ274XX_CTRL_BAT_INSERT      0x000C
#define BQ274XX_CTRL_BAT_REMOVE      0x000D
#define BQ274XX_CTRL_SET_HIBERNATE   0x0011
#define BQ274XX_CTRL_CLEAR_HIBERNATE 0x0012
#define BQ274XX_CTRL_SET_CFGUPDATE   0x0013
#define BQ274XX_CTRL_SHUTDOWN_ENABLE 0x001B
#define BQ274XX_CTRL_SHUTDOWN        0x001C
#define BQ274XX_CTRL_SEALED          0x0020
#define BQ274XX_CTRL_PULSE_SOC_INT   0x0023
#define BQ274XX_CTRL_RESET           0x0041
#define BQ274XX_CTRL_SOFT_RESET      0x0042
#define BQ274XX_CTRL_EXIT_CFGUPDATE  0x0043
#define BQ274XX_CTRL_EXIT_RESIM      0x0044

/* BQ27427 */
#define BQ27427_CTRL_CHEM_A 0x0030
#define BQ27427_CTRL_CHEM_B 0x0031
#define BQ27427_CTRL_CHEM_C 0x0032

/*** Extended Data Commands ***/
#define BQ274XX_EXT_OPCONFIG                   0x3A /* OpConfig() */
#define BQ274XX_EXT_CAPACITY                   0x3C /* DesignCapacity() */
#define BQ274XX_EXT_DATA_CLASS                 0x3E /* DataClass() */
#define BQ274XX_EXT_DATA_BLOCK                 0x3F /* DataBlock() */
#define BQ274XX_EXT_BLKDAT_START               0x40 /* BlockData_start() */
#define BQ274XX_EXT_BLKDAT_END                 0x5F /* BlockData_end() */
#define BQ274XX_EXT_CHECKSUM                   0x60 /* BlockDataCheckSum() */
#define BQ274XX_EXT_DATA_CONTROL               0x61 /* BlockDataControl() */
#define BQ274XX_EXT_BLKDAT(off)                (BQ274XX_EXT_BLKDAT_START + off)


// int bq274xx_trigger_mode_init(const struct device *dev);
// int bq274xx_trigger_set(const struct device *dev,
// 			const struct sensor_trigger *trig,
// 			sensor_trigger_handler_t handler);


int bq274xx_get_device_type(const struct device *dev, uint16_t *val);
int bq274xx_gauge_configure(const struct device *dev);
int bq274xx_gauge_init(const struct device *dev);
int bq274xx_sample_fetch(const struct device *dev, enum sensor_channel chan);
int bq274xx_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val);