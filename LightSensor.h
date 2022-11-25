#include "mbed.h"
#define LIGHT_SENSOR_PIN PA_4
#define MINLIGHT 1.0
#define MAXLIGHT 90.0
class LightSensor{//Class to define the Light Sensor
    public: 
        LightSensor();
        float light;
        AnalogIn lightSensor;
        void read();
        void printLight();
        float minLight;
        float maxLight;
        float meanLight;
        int counter;
        float TotalLights;
        void printMinMaxMean();
        void resetValues();
};