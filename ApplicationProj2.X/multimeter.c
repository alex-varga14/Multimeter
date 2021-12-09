#include <xc.h>
#include <p24F16KA101.h>
#include "multimeter.h"
#include "ADC.h"
#include "UART2.h"
#include "TimeDelay.h"

//Function to average the ADC samples
float average_ADC(int samples, int pin) {
    
    int i = 0;
    uint32_t total = 0;
    while (i < samples) {
        uint16_t adcbuf = do_ADC(pin);
        total += adcbuf;
        i++;
    }
    return total/samples;
}

//Measure frequency function
float measure_frequency(){
    /* pseudocode:
     * start timer
     * enable comparator events
     * number of events = 0
     * while number of events is less than 1000 {
     * if theres a comparator event, number of events ++
     * }
     * stop the timer
     * disable comparator interrupts
     * period = timer value/1000
     * frequency = 1/period
     */
    start_timer(); //start timer
    CM1CONbits.CEVT = 0; //compartor events set to 0
    int number_of_events = 0;
    //count events till 50 to get a good estimate
    while(number_of_events < 50) {
        //if the current time is over 500, stop timer, set idle mode and return
        if(get_current_ms() > 500) {
            stop_timer();
            return 0;
            //frequency too low or no frequency signal detected
        }
        //Disp2Dec(number_of_events);
        if(CM1CONbits.CEVT == 1) {
            number_of_events++;
            CM1CONbits.CEVT = 0;
        }
    }
    //stop timer
    int ms = stop_timer();
    //calculate frequency
    float frequency = 1.0/(((float)ms/1000.0)/50.0);
    return frequency;
}
//function to measure voltage
float measure_voltage(){
    //get samples from adc for voltage (pin 5)
    int measurement = average_ADC(500, 5);
    //volatage calculation
    float voltage = (measurement/1023.0)*3.25;
    return voltage;
}
//function to measure resistance
float measure_resistance(){
    //get samples from adc for resistance (pin 11)
    int measurement = average_ADC(500, 11);
    float voltage = (measurement/1023.0)*3.25;
    //resistance formula calculation
    float resistance = (voltage*1000)/(3.25 - voltage);
    return resistance;
}
//function to measure capacitance
float measure_capacitance(){
    /*pseudocode:
     * start timer
     * set RA6 to high
     * convert AN12 to a digital signal (between 0 and 1023)
     * wait until digital input is 645 (63% of max voltage)
     *  if it's taking too long, discharge capacitor and return 0
     * stop timer
     * capacitance = (elapsed time)/(resistor value = 10000) * (1000 microFarads)
     * discharge capacitor by setting RA6 and RB9 to low and waiting until AN12 is 0-ish
     */
    //start timer
    start_timer();
    LATAbits.LATA6 = 1; //start charging capacitor
    int capacitor_voltage = average_ADC(100, 12);
    //charge up capacitor to around 60%
    while (capacitor_voltage < 500) {
        capacitor_voltage = average_ADC(100, 12);
    }
    int ms_passed = stop_timer();
    //calculate capacitance
    float capacitance_uF = (((float)ms_passed)/10000.0) * 1000.0;
    //discharge capacitor:
    LATAbits.LATA6 = 0;
    //discharge capacitor
    while(average_ADC(100, 12) > 0) {}
    return capacitance_uF;
}