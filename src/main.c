#include "include/mcu.h"
#include "include/gpio.h"
#include "include/fan.h"
#include "include/smc.h"
#include "include/uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "include/i2c.h"
#include "include/bmp280.h"
#include "include/control.h"

#include "include/debug.h"

#include <util/delay.h>

#include <stdlib.h>
#include <string.h>

#define SENSOR_1_ADDRESS 0b11101100U
#define SENSOR_2_ADDRESS 0b11101110U
#define READ             0b00000000U
#define WRITE            0b00000001U

struct control_vector {
    uint8_t fan_0_duty_cycle;
    uint8_t fan_1_duty_cycle;
    uint16_t smc_period;
};

struct sensor_vector {
    int32_t sensor_0_temp;
    uint32_t sensor_0_pres;
    int32_t sensor_1_temp;
    uint32_t sensor_1_pres;
};

struct control_vector control_val;
struct sensor_vector  sensor_val;

const bmp280_config_t sensor_config = {
    .t_sb = BMP280_STANDBY_500US,
    .filter = BMP280_FILTER_16,
    .osrs_t = BMP280_OVSP_2,
    .osrs_p = BMP280_OVSP_16,
    .mode = BMP280_NORMAL
};

bmp280_dev_t sensor0 = {
        .address = SENSOR_1_ADDRESS
};

bmp280_dev_t sensor1 = {
        .address = SENSOR_2_ADDRESS
};

static void init(void);
static void core_process(void);
static void set_control_vector(struct control_vector * control_values);
static void get_sensor_vector(struct sensor_vector * sensor_values);

static void init(){
    /* init all peripherials */
    init_gpio();
    init_uart();

    /* indicate a start of the program */
    send_uart("Start\n\r",1);

    init_i2c();

    sensor0.config = sensor_config;
    sensor1.config = sensor_config;
    init_bmp280_sensor(&sensor0);
    init_bmp280_sensor(&sensor1);

    init_fan();
    set_fan_prescaler(FAN_DIV8);
    control_val.fan_0_duty_cycle = 100;
    control_val.fan_1_duty_cycle = 100;
    set_fan_dutycycle(FAN0, control_val.fan_0_duty_cycle);
    set_fan_dutycycle(FAN1, control_val.fan_1_duty_cycle);

    init_smc();
    //IO_SMC_REG |= (1 << IO_SMC_EN);
    set_smc_prescaler(SMC_DIV1024);
    //set_smc_period(0x0u);
    control_val.smc_period = 15;

    set_syscounter(3125, SYS_DIV1024); /* 200ms */
    set_syscallback(core_process);

    init_control();
    ENABLE_TX_ISR();
    sei();
}

/* callback for the control module, all periodic jobs habe to be in here */
void core_process(){
    static char uart_buff[128] = {0};
    const uint8_t message_delay = 50;
    static uint8_t message_delay_counter = 0;
    /* read sensor values */
    bmp280_read_pressure(&sensor0);
    bmp280_read_temperature(&sensor0);
    bmp280_read_pressure(&sensor1);
    bmp280_read_temperature(&sensor1);

    get_sensor_vector(&sensor_val);

    //control_val.smc_period = 16400;
    set_control_vector(&control_val);

    message_delay_counter++;
    if(message_delay_counter >= message_delay){
        message_delay_counter = 0;
        sprintf(uart_buff, "T0: %ld T1: %ld P0: %lu P1: %lu\n\r",
            sensor_val.sensor_0_temp,
            sensor_val.sensor_1_temp,
            sensor_val.sensor_0_pres,
            sensor_val.sensor_1_pres
        );
        send_uart(uart_buff, 1);
    }
}

static void set_control_vector(struct control_vector * control_values){
    set_fan_dutycycle(FAN0, control_values->fan_0_duty_cycle);
    set_fan_dutycycle(FAN1, control_values->fan_1_duty_cycle);
    set_smc_period(control_values->smc_period);
}

static void get_sensor_vector(struct sensor_vector * sensor_values){
    sensor_values->sensor_0_pres = bmp280_get_pressure(&sensor0);
    sensor_values->sensor_1_pres = bmp280_get_pressure(&sensor1);
    sensor_values->sensor_0_temp = bmp280_get_temperature(&sensor0);
    sensor_values->sensor_1_temp = bmp280_get_temperature(&sensor1);
}

static void process_message(char * message_buff){

    uint8_t msg_pointer = 0;
    uint8_t msg_len = strlen(message_buff);
    int32_t value = 0;

    if(msg_len < 3){
        return;
    }

    while(message_buff[msg_pointer] == ' '){
        msg_pointer++;
    }

    switch (message_buff[msg_pointer]){
    case 'F': /* fan speed */
        switch (message_buff[msg_pointer + 1]){
        case '0':
            value = strtol(message_buff + msg_pointer + 2, NULL, 0);
            control_val.fan_0_duty_cycle = (uint8_t)value;
            DEBUG("GOT FAN0\r\n");
            break;
        case '1':
            value = strtol(message_buff + msg_pointer + 2, NULL, 0);
            control_val.fan_1_duty_cycle = (uint8_t)value;
            DEBUG("GOT FAN1\r\n");
            break;
        default:
            return;
        }
        break;
    case 'M': /* stepper motor speed */
        value = strtol(message_buff + msg_pointer + 1, NULL, 0);
        control_val.smc_period = (uint16_t)value;
        DEBUG("GOT SMC\r\n");
    default:
        break;
    }
    DEBUG("got message %s, got value: %d\r\n", message_buff, value);
}

int main(){

    int32_t temperature = 0x0;
    uint32_t pressure = 0x0;
    uint8_t duty_cycle = 0x0;

    char message[64] = {0};

    init();

    for(;;) {
        /*
        temperature = bmp280_get_temperature(&sensor0);
        pressure = bmp280_get_pressure(&sensor0);
        DEBUG("sensor 1: temp: %ld    press: %ld\n\r", temperature, pressure);

        pressure = bmp280_get_pressure(&sensor1);
        temperature = bmp280_get_temperature(&sensor1);
        DEBUG("sensor 2: temp: %ld    press: %ld\n\r\n", temperature, pressure);

        /* check if there is a message to process /

        DEBUG("status counter: %ld\n\r", get_syscounter());
        _delay_ms(1000);

        set_fan_dutycycle(FAN0, duty_cycle);
        set_fan_dutycycle(FAN1, UINT8_MAX - duty_cycle);

        duty_cycle+=8;
        */
        if(0 != uart_rx_state()) {
            read_uart_buff(message);
            process_message(message);
        }
    }
}
