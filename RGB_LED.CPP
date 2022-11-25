#include "mbed.h"
#include "RGB_LED.h"

LedRGB::LedRGB():Red(RED_LED), Green(GREEN_LED), Blue(BLUE_LED){
    Red = Blue = Green = 1;
    WarningActivated = false;
}

void LedRGB::setColor(int Color){
    switch (Color) {
        case 0:
            Red = 0;
            Green = 1;
            Blue = 1;
            break;
        case 1:
            Red = 1;
            Blue = 0;
            Green = 1;
            break;

        case 2:
            Red = 1;
            Blue = 1;
            Green = 0;
            break;
        
    }
}

int LedRGB::setLimits(float measurement, int color, float max, float min){//Method use to check if a meausrement has exceeded the prviously set limits,and activate the Led accordingly
    if(!WarningActivated){// Check if a warning has already been activated. If it is the case don't do anything
        if((measurement < min)|(measurement > max)){
            switch (color) {
            case 0: //Red
                Red = 0;
                Green = 1;
                Blue = 1;
                break;
            case 1: //Blue
                Red = 1;
                Blue = 0;
                Green = 1;
                break;
            case 2: //Green
                Red = 1;
                Blue = 1;
                Green = 0;
                break;
            case 3: // Purple
                Red = 0;
                Blue = 0;
                Green = 1;
                break;
            case 4: //Yellow
                Red = 0;
                Blue = 1;
                Green = 0;
                break;
            }
            return 1;// Return 1 == WarningActivated = true

        } else {
            Red = Green = Blue = 1;
            return 0; //Return 0 == WarningActivated = false
        }
    } else
        return 1;
}