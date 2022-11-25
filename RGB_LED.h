#include "mbed.h"

#define RED_LED PH_0
#define GREEN_LED PH_1
#define BLUE_LED PB_13

class LedRGB{ //Class to efine the RGB Led
    public: 
        LedRGB();
        DigitalOut Red;
        DigitalOut Green;
        DigitalOut Blue;
        void setColor(int Color);
        int setLimits(float measurement, int color, float max, float min);
        bool WarningActivated;// This flag is activated when any of the measurements received exceeds the limits
                                // The Warning signal can only be activated once (one sensor) in each read.
};