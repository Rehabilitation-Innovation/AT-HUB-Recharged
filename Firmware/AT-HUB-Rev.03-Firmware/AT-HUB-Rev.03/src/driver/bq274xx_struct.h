#pragma once

#include <stdint.h>
#include <stdbool.h>

struct i2c_dt_spec {
	const struct device *bus;
	uint16_t addr;
};

/* Hold the register offset for a device variant. */
struct bq274xx_regs {
	uint8_t dm_design_capacity;
	uint8_t dm_design_energy;
	uint8_t dm_terminate_voltage;
	uint8_t dm_taper_rate;
};

struct bq274xx_data {
	const struct bq274xx_regs *regs;
	bool configured;
	uint16_t voltage;
	int16_t avg_current;
	int16_t stdby_current;
	int16_t max_load_current;
	int16_t avg_power;
	uint16_t state_of_charge;
	int16_t state_of_health;
	uint16_t internal_temperature;
	uint16_t full_charge_capacity;
	uint16_t remaining_charge_capacity;
	uint16_t nom_avail_capacity;
	uint16_t full_avail_capacity;

#ifdef CONFIG_BQ274XX_TRIGGER
	const struct device *dev;
	struct gpio_callback ready_callback;
	sensor_trigger_handler_t ready_handler;
	const struct sensor_trigger *ready_trig;

#ifdef CONFIG_BQ274XX_TRIGGER_OWN_THREAD
	struct k_sem sem;
#endif

#ifdef CONFIG_BQ274XX_TRIGGER_GLOBAL_THREAD
	struct k_work work;
#endif
#endif /* CONFIG_BQ274XX_TRIGGER */
};

struct bq274xx_config {
	struct i2c_dt_spec i2c;
	uint16_t i2c_device_id;
	uint16_t design_voltage;
	uint16_t design_capacity;
	uint16_t taper_current;
	uint16_t terminate_voltage;
#if defined(CONFIG_BQ274XX_PM) || defined(CONFIG_BQ274XX_TRIGGER)
	struct gpio_dt_spec int_gpios;
#endif
	uint16_t chemistry_id;
	bool lazy_loading;
};