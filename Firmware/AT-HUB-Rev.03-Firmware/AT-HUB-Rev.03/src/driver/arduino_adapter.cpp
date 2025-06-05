#include "bq274xx_struct.h"
#include "arduino_adapter.h"

int i2c_burst_read_dt (const struct i2c_dt_spec* spec, uint8_t start_addr, uint8_t* buf, uint32_t num_bytes) {
    // Start the transmission to the device with the given address
    
    if ( spec == NULL || buf == NULL ) return -1;
    
    Wire.beginTransmission(spec->addr);
    
    // Send the start address from which to begin reading
    Wire.write(start_addr);
    
    // End the transmission to prepare for a read
    int result = Wire.endTransmission(false);  // 'false' to not release the bus, so we can follow up with a read
    
    // If there was an error starting the transmission, return the error code
    if (result != 0) {
        return result;
    }
    
    // Request the specified number of bytes from the device
    uint8_t bytesRead = 0;
    Wire.requestFrom(spec->addr, num_bytes);
    
    // Read the requested bytes into the buffer
    while (Wire.available()) {
        if (bytesRead < num_bytes) {
            buf[bytesRead++] = Wire.read();
        }
    }
    
    // Return 0 for success, or number of bytes read (error handling can be expanded)
    return (bytesRead == num_bytes) ? 0 : -1;  // Returns 0 if all bytes are read, otherwise error code
}

int i2c_write_dt ( const struct i2c_dt_spec* spec, const uint8_t* buf, uint32_t num_bytes ) {
    // Start the transmission to the device with the given address
    
    if ( spec == NULL || buf == NULL ) return -1;
        
    Wire.beginTransmission(spec->addr);

    // Write each byte in the buffer to the I2C bus
    for (uint32_t i = 0; i < num_bytes; i++) {
        Wire.write(buf[i]);  // Write each byte from the buffer
    }

    // End the transmission and send the data
    int result = Wire.endTransmission();  // 0 means success, non-zero means failure

    // Return the result of the transmission (0 = success, other = error code)
    return result;
}


int i2c_reg_write_byte_dt ( const struct i2c_dt_spec* spec, uint8_t reg_addr, uint8_t value ) {
    // Start the transmission to the device with the given address
    
    if ( spec == NULL ) return -1;
    
    Wire.beginTransmission(spec->addr);
    
    // Write the register address to which the data will be written
    Wire.write(reg_addr);
    
    // Write the data byte to the register
    Wire.write(value);
    
    // End the transmission and send the data to the device
    int result = Wire.endTransmission();  // 0 means success, non-zero means failure
    
    // Return the result of the transmission (0 = success, other = error code)
    return result;  
}

int i2c_reg_read_byte_dt (const struct i2c_dt_spec* spec, uint8_t reg_addr, uint8_t* value ) {
    // Start the transmission to the device with the given address
    
    if ( spec == NULL || value == NULL ) return -1;
    
    Wire.beginTransmission(spec->addr);
    
    // Write the register address from which to read
    Wire.write(reg_addr);
    
    // End the transmission and prepare for a read
    int result = Wire.endTransmission(false);  // false keeps the bus active for a subsequent read
    
    // If there was an error starting the transmission, return the error code
    if (result != 0) {
        return result;
    }
    
    // Request 1 byte from the device
    Wire.requestFrom(spec->addr, (uint8_t)1);
    
    // If there's at least 1 byte available to read
    if (Wire.available()) {
        *value = Wire.read();  // Read the byte and store it in the value pointer
        return 0;  // Success
    }
    
    // If no byte was read, return an error code
    return -1;  // Failed to read byte
}


uint16_t sys_get_le16(const uint8_t *buf)
{
    if ( buf == NULL ) return 0;
    return (uint16_t)(buf[0] | ((uint16_t)buf[1] << 8));
}

void sys_put_le16(uint16_t val, uint8_t *buf)
{
    if ( buf == NULL ) return;
    buf[0] = (uint8_t)(val & 0xFF);          // Write LSB first
    buf[1] = (uint8_t)((val >> 8) & 0xFF);   // Write MSB second
}

uint16_t sys_get_be16(const uint8_t *buf)
{
    if ( buf == NULL ) return 0;
    return (uint16_t)((buf[0] << 8) | buf[1]);
}

void sys_put_be16(uint16_t val, uint8_t *buf)
{
    if ( buf == NULL ) return;
    buf[0] = (uint8_t)((val >> 8) & 0xFF);   // Write MSB first
    buf[1] = (uint8_t)(val & 0xFF);          // Write LSB second
}

bool device_is_ready (const struct device* dev	) {
    if ( !dev ) return false;
    if ( !dev->config ) return false;
    if ( !dev->config->i2c.bus ) return false;
    Wire.beginTransmission(dev->config->i2c.addr);
    return (Wire.endTransmission() == 0);
}   
