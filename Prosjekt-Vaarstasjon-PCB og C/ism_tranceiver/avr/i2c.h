/*
 * Copied from https://github.com/g4lvanix/I2C-master-lib
 *
 * MODIFIED BY ELAB
 */

#ifndef I2C_H
#define I2C_H

#define I2C_READ	0x01
#define I2C_WRITE	0x00

int i2c_init(void);
int i2c_write_reg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint8_t length);
int i2c_read_reg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint8_t length);
int i2c_read_reg_10ms_delay(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint8_t length);

#endif /* I2C_H */