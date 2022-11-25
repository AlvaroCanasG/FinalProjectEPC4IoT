#include "mbed.h"
#include "LightSensor.h"

LightSensor::LightSensor(): lightSensor(LIGHT_SENSOR_PIN){
    light = 0.0;
    resetValues();
}

void LightSensor::read(){
    light = lightSensor.read()*100;
}

void LightSensor::printLight(){
    read();
    printf("Ambient light: %.2f %%\n", light);
    if(light < minLight){minLight = light;}
    if(light > maxLight){maxLight = light;}
    TotalLights = TotalLights + light;
    counter ++;
    meanLight = TotalLights/counter;
}

void LightSensor :: printMinMaxMean(){
    printf("LIGHT\n");
    printf(" -Min: %.2f %% \n", minLight);
    printf(" -Max: %.2f %% \n", maxLight);
    printf(" -Mean: %.2f %% \n", meanLight);
    resetValues();
}
void LightSensor::resetValues(){
    minLight = MAXLIGHT;
    maxLight = MINLIGHT;
    meanLight = 0;
    TotalLights = 0;
    counter = 0;
}