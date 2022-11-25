#include "mbed.h"
#include <cstdio>
#include "SerialGPS.h"
#include "I2C_Port.h"
#include "LightSensor.h"
#include "SoilMoisture.h"
#include "RGB_LED.h"

//Definition of sensors and Actuators
Thread gpsThread;
SerialGPS myGPS(PA_9, PA_10,9600);
TempHum myTempHum;
LightSensor myLightSensor;
MoistureSensor myMoistureSensor;
ColorSensor myColorSensor;
LedRGB myLedRGB;
Accelerometer myAccelerometer;



Ticker TwoSeconds;
Ticker ThirtySeconds;
int Counter;
DigitalOut LED(LED3);
InterruptIn userButton(USER_BUTTON);
bool Flag = true;
bool NormalMode = false;
bool PrintMode = true;


void tick_isr(void){ //The ticker activates the Flag to activate the reads, and increments the Counter used in Normal Mode
    Counter ++;
    Flag = true;
    
}

void button(void){// Function that sets up the change of mode when the button is pressed
    Flag = true;
    Counter = 0;
    PrintMode = true;
    if(NormalMode){// Switch from normal to Test Mode
        NormalMode = false;
        ThirtySeconds.detach();
        TwoSeconds.attach(tick_isr, 2.0);
        LED = 0;
    } else { //Switch from Test to Normal Mode
        NormalMode = true;
        TwoSeconds.detach();
        ThirtySeconds.attach(tick_isr, 5.0);
        LED = 1;
        //Reset all the values to calculate min, max and mean values
        myTempHum.ResetValues();
        myColorSensor.resetValues();
        myLightSensor.resetValues();
        myMoistureSensor.resetValues();
        myAccelerometer.resetValues();
    }
}

void pressButton(void){
    button();
}




int main()
{   
    LED = 0;
    userButton.rise(pressButton);
    gpsThread.start(gps_task);
    TwoSeconds.attach(tick_isr, 2.0);// By default, start in TestMode
    while (true) {
        if(!NormalMode){//TEST MODE

            if(Flag){// 2 seconds
                    if(PrintMode){// If it is the First iteration, print the mode
                        printf("TEST MODE\n");
                        printf("----------------\n");
                        PrintMode = false;
                    }

                    Flag = false;

                    //Use the print methods of each class
                    myTempHum.printTemp();
                    myTempHum.printHum();
                    myLightSensor.printLight();
                    myMoistureSensor.printMoisture();
                    myColorSensor.printColor();
                    myLedRGB.setColor(myColorSensor.dominantColor);
                    myAccelerometer.printAccelerometer();
                    myGPS.PrintTime();
                    myGPS.PrintLocation();
                    printf("\n");        
                 }

        } else {  // NORMAL MODE
            if(Flag){ //30 seconds

                    if(PrintMode){
                        printf("NORMAL MODE\n");
                        printf("----------------\n");
                        PrintMode = false;
                    }
                    Flag = false;

                    //Other than printing the measurements, for each sensor we check if the value exceeds the limits set.
                    //If it is the case, the Warning flag is activated once
                    myTempHum.printTemp();
                    myLedRGB.WarningActivated = myLedRGB.setLimits(myTempHum.Temperature, 0, MAXTEMP, MINTEMP);// Store the value returned by the setLimitfunction in the WarningActivated flag
                                                                                                                //If the flag is true, it will disable the method in the rest of the checkings
                    myTempHum.printHum();                                                                       // 0 Red
                    myLedRGB.WarningActivated = myLedRGB.setLimits(myTempHum.Humidity, 1, MAXHUM, MINHUM);// 1 Blue

                    myLightSensor.printLight();
                    myLedRGB.WarningActivated = myLedRGB.setLimits(myLightSensor.light, 2, MAXLIGHT, MINLIGHT);//2 Green

                    myMoistureSensor.printMoisture();
                    myLedRGB.WarningActivated = myLedRGB.setLimits(myMoistureSensor.moisture, 3, MAXMOIST, MINMOIST);//3 Purple
                    
                    myAccelerometer.printAccelerometer();
                    myLedRGB.WarningActivated = myLedRGB.setLimits(myAccelerometer.acc_x, 4, MAXACC, MINACC);//4 Yellow
                    myLedRGB.WarningActivated = myLedRGB.setLimits(myAccelerometer.acc_y, 4, MAXACC, MINACC);
                    myLedRGB.WarningActivated = myLedRGB.setLimits(myAccelerometer.acc_z, 4, MAXACC, MINACC);

                    myColorSensor.printColor();

                    myGPS.PrintTime();
                    myGPS.PrintLocation();
                    printf("\n");
                    myLedRGB.WarningActivated = 0;

                    if(Counter == 2){// 1 hour
                        Counter = 0;
                        myTempHum.PrintMinMaxMean();
                        myLightSensor.printMinMaxMean();
                        myMoistureSensor.printMinMaxMean();
                        myColorSensor.printHourlyDominant();
                        myAccelerometer.printMinMax();
                        printf("\n");
                    }
            }
        }
    }   
}

