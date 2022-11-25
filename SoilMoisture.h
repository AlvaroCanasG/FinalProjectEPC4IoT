#include "mbed.h"
#define SOIL_MOISTURE_SENSOR_PIN PA_0
#define MINMOIST 10.0
#define MAXMOIST 90.0
class MoistureSensor{ //Class to define the Soil Moisture sensor
    public: 
        MoistureSensor();
        float moisture;
        AnalogIn moistureSensor;
        void read();
        void printMoisture();
        float minMoisture;
        float maxMoisture;
        float meanMoisture;
        int counter;
        float TotalMoistures;
        void printMinMaxMean();
        void resetValues();
};