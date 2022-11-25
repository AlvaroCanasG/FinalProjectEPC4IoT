#include "mbed.h"
#include "SoilMoisture.h"

MoistureSensor::MoistureSensor(): moistureSensor(SOIL_MOISTURE_SENSOR_PIN){
    moisture = 0.0;
    resetValues();
}

void MoistureSensor::read(){
    moisture = moistureSensor.read()*100;
}

void MoistureSensor::printMoisture(){
    read();
    printf("Soil Moisture: %.2f%%\n", moisture);
    if(moisture < minMoisture){minMoisture = moisture;}
    if(moisture > maxMoisture){maxMoisture = moisture;}
    TotalMoistures += moisture;
    counter ++;
    meanMoisture = TotalMoistures/counter;
}
void MoistureSensor :: printMinMaxMean(){
    printf("MOISTURE\n");
    printf(" -Min: %.2f %% \n", minMoisture);
    printf(" -Max: %.2f %% \n", maxMoisture);
    printf(" -Mean: %.2f %% \n", meanMoisture);
    resetValues();
}
void MoistureSensor::resetValues(){
    minMoisture = MAXMOIST;
    maxMoisture = MINMOIST;
    meanMoisture = 0;
    TotalMoistures = 0;
    counter = 0;
}