#include "pico/stdlib.h"

#include <stdio.h>
#include <string.h>
#include <sstream>

#include "AnalogSensor.h"
#include "DFRobot_PH_Analog.h"
#include "DFRobot_EC_Analog.h"
#include "DFRobot_CalibrationManager.h"
#include "DS18B20_onewire.h"
#include "DFRobot_A02YYUW_Uart.h"



// #include "hardware/watchdog.h"
// #include "hardware/adc.h"
// #include "hardware/i2c.h"


const uint8_t ADC1_PH_PIN = 27;
const uint8_t adc_ph_channel = 1;
const uint8_t ONEWIRE_PIN = 1;
const uint8_t UART_TX_PIN = 4;
const uint8_t UART_RX_PIN = 5;

#define UART_ID uart1



const uint8_t ADC2_EC_PIN = 28;
const uint8_t adc_ec_channel = 2;

#define CMD_BUFFER_SIZE 64

bool read_serial_command(char* out_cmd) {
    static char buffer[CMD_BUFFER_SIZE];
    static int index = 0;

    int ch = getchar_timeout_us(0);  // non-blocking read

    if (ch == PICO_ERROR_TIMEOUT) {
        return false;
    }

    // User pressed Enter
    if (ch == '\n' || ch == '\r') {
        if (index == 0) {
            return false;
        }

        buffer[index] = '\0';

        strcpy(out_cmd, buffer);

        index = 0;
        memset(buffer, 0, sizeof(buffer));

        return true;
    }

    // Normal typed character
    if (index < CMD_BUFFER_SIZE - 1) {
        buffer[index++] = (char)ch;
    }

    return false;
}

std::string make_json_payload(const std::map<std::string,float>& data){
    std::ostringstream ss;
    ss << "{";
    bool first = true;

    for (const auto &[key, value] : data){
        if (!first) ss<< ", ";
        ss << "\"" << key << "\": " << value;
        first = false;
    }

    ss << "}";

    return ss.str();
}

DFR_PH_Analog ph_converter;
DFR_CalibrationManager ph_cal(ph_converter);


auto phMapping = [](float voltage) -> std::map<std::string,float> {
    
    float voltage_mv = voltage * 1000.0f ;

    float ph = ph_converter.readPH(voltage_mv);
    
    return {{"ph", ph},{"ph_voltage_mv", voltage_mv}};
};


DFR_EC_Analog ec_converter;
DFR_CalibrationManager ec_cal(ec_converter);


auto ecMapping = [](float voltage) -> std::map<std::string,float> {
    
    float voltage_mv = voltage * 1000.0f ;

    float ec = ec_converter.readEC(voltage_mv);
    
    return {{"ec_ms/cm", ec},{"ec_voltage_mv", voltage_mv}};
};


int main(){
    stdio_init_all();
    adc_init();
    uart_init(UART_ID,9600);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    gpio_init(ONEWIRE_PIN);



    sleep_ms(1000);

    ph_converter.begin();
    ec_converter.begin();

    AnalogSensor ph_sensor(adc_ph_channel, ADC1_PH_PIN, phMapping);
    AnalogSensor ec_sensor(adc_ec_channel, ADC2_EC_PIN, ecMapping);
    DS18B20_onewire temp_sensor(ONEWIRE_PIN);
    DFR_A02YYUW_Ultrasonic ultrasonic_sensor(UART_ID);



    ph_sensor.init();
    ec_sensor.init();
    temp_sensor.init();
    ultrasonic_sensor.init();



    sleep_ms(1000);

    std::map<std::string,float> allData;
    std::string payload;


    uint64_t last_log = time_us_64();


    // Main Loop
    while(true){

        char command[64];
        if (read_serial_command(command)) {
            printf("Command received: %s\n", command);
            ph_cal.handleCommand(command);
        }

        if(ph_sensor.read()){
            auto ph_data = ph_sensor.getData();
            for (const auto &[key,value] : ph_data){
                allData[key] = value;
            }

            ph_cal.task(ph_data["ph_voltage_mv"]);

        }

        if(ec_sensor.read()){
            auto ec_data = ec_sensor.getData();
            for (const auto &[key,value] : ec_data){
                allData[key] = value;
            }

            ec_cal.task(ec_data["ec_voltage_mv"]);

        }

        if(temp_sensor.read()){
            auto temp_data = temp_sensor.getData();
            for (const auto &[key,value] : temp_data){
                allData[key] = value;
            }
        }

        if(ultrasonic_sensor.read()){
            auto ultrasonic_data = ultrasonic_sensor.getData();
            for (const auto &[key,value] : ultrasonic_data){
                allData[key] = value;
            }
        }

        if((time_us_64() - last_log) > 1 * 1000000){
            last_log = time_us_64();

            payload = make_json_payload(allData);
            printf("%s\n", payload.c_str());

            if (ph_cal.isCalibrating()) {
                printf("Calibrating PH");
            }

            if (ec_cal.isCalibrating()) {
                printf("Calibrating EC");
            }
        
        }


        sleep_ms(1);

    }

}