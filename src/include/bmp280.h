#ifndef BMP280_H
#define BMP280_H

#include <stdint.h>

/*
 * https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf
 * Table 17: Compensation parameter storage, naming and data type
 */
struct bmp280_calib_data {  /* LSB  / MSB  */
    unsigned short  dig_T1; /* 0x88 / 0x89 */
    signed short    dig_T2; /* 0x8A / 0x8B */
    signed short    dig_T3; /* 0x8C / 0x8D */
    unsigned short  dig_P1; /* 0x8E / 0x8F */
    signed short    dig_P2; /* 0x90 / 0x91 */
    signed short    dig_P3; /* 0x92 / 0x93 */
    signed short    dig_P4; /* 0x94 / 0x95 */
    signed short    dig_P5; /* 0x96 / 0x97 */
    signed short    dig_P6; /* 0x98 / 0x99 */
    signed short    dig_P7; /* 0x9A / 0x9B */
    signed short    dig_P8; /* 0x9C / 0x9D */
    signed short    dig_P9; /* 0x9E / 0x9F */
    /* reserved                0xA0 / 0xA1 */
};

/* standby time between measuremenet bursts */
enum bmp280_standby_time {
    BMP280_STANDBY_500US,
    BMP280_STANDBY_62500US,
    BMP280_STANDBY_125MS,
    BMP280_STANDBY_250MS,
    BMP280_STANDBY_500MS,
    BMP280_STANDBY_1000MS,
    BMP280_STANDBY_2000MS,
    BMP280_STANDBY_4000MS
};


/* coefficient for pressure LPF, chapter 3.3.3 in documentation for reference */
enum bmp280_filter_coefficient {
    BMP280_FILTER_OFF,
    BMP280_FILTER_2,
    BMP280_FILTER_4,
    BMP280_FILTER_8,
    BMP280_FILTER_16
};

/* number of samples per measurement */
enum bmp280_oversampling {
    BMP280_OVSP_SKIPPED,
    BMP280_OVSP_1,
    BMP280_OVSP_2,
    BMP280_OVSP_4,
    BMP280_OVSP_8,
    BMP280_OVSP_16
};

/* bmp280 power mode */
enum bmp280_power_mode {
    BMP280_SLEEP,
    BMP280_FORCED,
    BMP280_RESERVED,
    BMP280_NORMAL
};

typedef struct {
    uint8_t t_sb;       /* sampling downtime */
    uint8_t filter;     /* IIR filter time coefficient */
    uint8_t osrs_t;     /* temperature oversampling */
    uint8_t osrs_p;     /* pressure oversampling */
    uint8_t mode;       /* power mode */
}bmp280_config_t;

typedef struct {
    /* i2c address */
    uint8_t address;

    /* temperature values */
    int32_t temp_raw;
    int32_t temp;

    /* pressure values */
    int32_t pres_raw;
    uint32_t pres;

    /* temperature value for pressure calibration */
    int32_t t_fine;

    /* sensor configuration */
    bmp280_config_t config;

    /* calibration data */
    struct bmp280_calib_data calib_data;
}bmp280_dev_t;

void setup_bmp280_sensor(bmp280_dev_t * dev);
void bmp280_read_temperature(bmp280_dev_t * dev);
void bmp280_read_pressure(bmp280_dev_t * dev);

int32_t bmp280_get_temperature(bmp280_dev_t * dev);
uint32_t bmp280_get_pressure(bmp280_dev_t * dev);

#endif
