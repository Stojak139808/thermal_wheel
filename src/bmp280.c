#include "include/bmp280.h"
#include "include/i2c.h"
#include "include/uart.h"
#include "include/debug.h"

#define READ                0b00000000U
#define WRITE               0b00000001U
#define CALIB_DATA_START    0x88U
#define TEMP_DATA_START     0xFAU
#define PRES_DATA_START     0xF7U

static uint8_t bmp280_read(bmp280_dev_t * dev, uint8_t address);
static void bmp280_read_calib_data(bmp280_dev_t * dev);
static int32_t bmp280_compensate_T_int32(bmp280_dev_t * dev);
static uint32_t bmp280_compensate_P_int64(bmp280_dev_t * dev);
static uint32_t bmp280_compensate_P_int32(bmp280_dev_t * dev);
static uint32_t bmp280_get_comp_pres_32bit(bmp280_dev_t * dev);
static uint32_t bmp280_buff_to_int20(uint8_t buff[3]);
static uint16_t bmp280_buff_to_int16(uint8_t buff[2]);

static uint32_t bmp280_buff_to_int20(uint8_t buff[3]){
    uint32_t ret = 0x0;
    ret = ((uint32_t)(buff[0]) << 16) |
          ((uint32_t)(buff[1]) << 8)  |
          ((uint32_t)(buff[2]) & 0xF0U);
    return ret >> 4;
}

static uint16_t bmp280_buff_to_int16(uint8_t buff[2]){
    uint16_t ret = 0x0;
    ret = ((uint16_t)buff[0] | ((uint16_t)buff[1] << 8));
    return ret;
}

static uint8_t bmp280_read(bmp280_dev_t * dev, uint8_t address){
    uint8_t message = 0x0;
    i2c_start();
    i2c_write(dev->address | READ);
    i2c_write(address);
    i2c_stop();
    i2c_start();
    i2c_write((dev->address | WRITE));
    message = i2c_read(0);
    i2c_stop();
    return message;
}

static uint8_t bmp280_write(bmp280_dev_t * dev, uint8_t address, uint8_t value){
    uint8_t message = 0x0;
    i2c_start();
    i2c_write(dev->address | READ);
    i2c_write(address);
    i2c_write(value);
    i2c_stop();
    return message;
}

static void bmp280_read_calib_data(bmp280_dev_t * dev){
    uint8_t i = 0;
    uint8_t data_buff[sizeof(struct bmp280_calib_data)];
    struct bmp280_calib_data redout;
    for(i = 0; i < sizeof(struct bmp280_calib_data); i++){
        data_buff[i] = bmp280_read(dev, i + CALIB_DATA_START);
    }

    /* fill the structure given by BOSCH */
    redout.dig_T1 = (unsigned short)bmp280_buff_to_int16(data_buff);
    redout.dig_T2 = (signed short)bmp280_buff_to_int16(data_buff + 2);
    redout.dig_T3 = (signed short)bmp280_buff_to_int16(data_buff + 4);
    redout.dig_P1 = (unsigned short)bmp280_buff_to_int16(data_buff + 6);
    redout.dig_P2 = (signed short)bmp280_buff_to_int16(data_buff + 8);
    redout.dig_P3 = (signed short)bmp280_buff_to_int16(data_buff + 10);
    redout.dig_P4 = (signed short)bmp280_buff_to_int16(data_buff + 12);
    redout.dig_P5 = (signed short)bmp280_buff_to_int16(data_buff + 14);
    redout.dig_P6 = (signed short)bmp280_buff_to_int16(data_buff + 16);
    redout.dig_P7 = (signed short)bmp280_buff_to_int16(data_buff + 18);
    redout.dig_P8 = (signed short)bmp280_buff_to_int16(data_buff + 20);
    redout.dig_P9 = (signed short)bmp280_buff_to_int16(data_buff + 22);

    dev->calib_data = redout;
}

void setup_bmp280_sensor(bmp280_dev_t * dev){

    uint8_t reg = 0x0;

    DEBUG("setting up BMP280 at %X\n\n\r", dev->address);
    /* read calibration data and fill the struct */
    bmp280_read_calib_data(dev);

    reg = ((dev->config.t_sb & 0b111) << 5) |
          ((dev->config.filter & 0b111) << 2);
    bmp280_write(dev, 0xf5, reg);

    reg = ((dev->config.osrs_t & 0b111) << 5) |
          ((dev->config.osrs_p & 0b111) << 2) |
          ((dev->config.mode & 0b11));
    bmp280_write(dev, 0xf4, reg);

    /* read the values back to verify */
    reg = bmp280_read(dev, 0xf5);
    DEBUG("sampling delay: 0x%X\n\r", (reg & 0b11100000) >> 5);
    DEBUG("filter: 0x%X\n\r", (reg & 0b00011100) >> 2);

    reg = bmp280_read(dev, 0xf4);
    DEBUG("temperature oversampling: 0x%X\n\r", (reg & 0b11100000) >> 5);
    DEBUG("pressure oversampling: 0x%X\n\r", (reg & 0b00011100) >> 2);
    DEBUG("power mode: 0x%X\n\r", (reg & 0b0000011));

    DEBUG("\nCalibration data:\n\r");
    DEBUG("T1: %d\n\r", dev->calib_data.dig_T1);
    DEBUG("T2: %d\n\r", dev->calib_data.dig_T2);
    DEBUG("T3: %d\n\r", dev->calib_data.dig_T3);
    DEBUG("P1: %d\n\r", dev->calib_data.dig_P1);
    DEBUG("P2: %d\n\r", dev->calib_data.dig_P2);
    DEBUG("P3: %d\n\r", dev->calib_data.dig_P3);
    DEBUG("P4: %d\n\r", dev->calib_data.dig_P4);
    DEBUG("P5: %d\n\r", dev->calib_data.dig_P5);
    DEBUG("P6: %d\n\r", dev->calib_data.dig_P6);
    DEBUG("P7: %d\n\r", dev->calib_data.dig_P7);
    DEBUG("P8: %d\n\r", dev->calib_data.dig_P8);
    DEBUG("P9: %d\n\r", dev->calib_data.dig_P9);
    DEBUG("\nCalibration done\n\n\r");
}

void bmp280_read_temperature(bmp280_dev_t * dev){
    uint8_t i = 0;
    uint8_t buff[3];

    for(i = 0; i < sizeof(buff); i++){
        buff[i] = bmp280_read(dev, TEMP_DATA_START + i);
    }

    dev->temp_raw = (int32_t)bmp280_buff_to_int20(buff);
    dev->temp = bmp280_compensate_T_int32(dev);
}

void bmp280_read_pressure(bmp280_dev_t * dev){
    uint8_t i = 0;
    uint8_t buff[3];

    for(i = 0; i < sizeof(buff); i++){
        buff[i] = bmp280_read(dev, PRES_DATA_START + i);
    }

    dev->pres_raw = (int32_t)bmp280_buff_to_int20(buff);
    dev->pres = bmp280_compensate_P_int32(dev);
}

int32_t bmp280_get_temperature(bmp280_dev_t * dev){
    return dev->temp;
}

uint32_t bmp280_get_pressure(bmp280_dev_t * dev){
    return dev->pres;
}

/****** code snippets from BOSCH ******/

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123”
// equals 51.23 DegC.
static int32_t bmp280_compensate_T_int32(bmp280_dev_t * dev){
    int32_t var1, var2, T;
    var1 = ((((dev->temp_raw>>3) - ((int32_t)dev->calib_data.dig_T1<<1))) *
            ((int32_t)dev->calib_data.dig_T2)) >> 11;
    var2 = (((((dev->temp_raw>>4) -
            ((int32_t)dev->calib_data.dig_T1)) *
            ((dev->temp_raw>>4) -
            ((int32_t)dev->calib_data.dig_T1))) >> 12) *
            ((int32_t)dev->calib_data.dig_T3)) >> 14;
    dev->t_fine = var1 + var2;
    T = (dev->t_fine * 5 + 128) >> 8;
    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386”
//equals 96386 Pa = 963.86 hPa
uint32_t bmp280_compensate_P_int32(bmp280_dev_t * dev){
    int32_t var1, var2;
    uint32_t p;
    var1 = (((int32_t)dev->t_fine)>>1) - (int32_t)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)dev->calib_data.dig_P6);
    var2 = var2 + ((var1*((int32_t)dev->calib_data.dig_P5))<<1);
    var2 = (var2>>2)+(((int32_t)dev->calib_data.dig_P4)<<16);
    var1 = (((dev->calib_data.dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) +
            ((((int32_t)dev->calib_data.dig_P2) * var1)>>1))>>18;
    var1 =((((32768+var1))*((int32_t)dev->calib_data.dig_P1))>>15);
    if (var1 == 0){
        return 0; // avoid exception caused by division by zero
    }
    p = (((uint32_t)(((int32_t)1048576)-dev->pres_raw)-(var2>>12)))*3125;
    if (p < 0x80000000){
        p = (p << 1) / ((uint32_t)var1);
    }
    else{
        p = (p / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)dev->calib_data.dig_P9) *
            ((int32_t)(((p>>3) *
            (p>>3))>>13)))>>12;
    var2 = (((int32_t)(p>>2)) * ((int32_t)dev->calib_data.dig_P8))>>13;
    p = (uint32_t)((int32_t)p + ((var1 + var2 + dev->calib_data.dig_P7) >> 4));
    return p;
}
