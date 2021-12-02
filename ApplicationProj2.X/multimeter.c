#include <xc.h>
#include <p24F16KA101.h>
#include "multimeter.h"
#include "ADC.h";
#include "TimeDelay.h"

float average_ADC(int samples) {
    int i = 0;
    uint32_t total = 0;
    while (i < samples) {
        uint16_t adcbuf = do_ADC();
        total += adcbuf;
        i++;
    }   
    return total/samples;
}

float measure_frequency(){
    return 0;
}
float measure_voltage(){
    int measurement = average_ADC(500);
    float voltage = (measurement/930.0)*3;
    return voltage;
}
float measure_resistance(){
    return 0;
}
float measure_capacitance(){
    return 0;
}