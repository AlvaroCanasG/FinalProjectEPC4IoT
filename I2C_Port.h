#include "mbed.h"

#define COLOR_ADDR 0x29
#define COLOR_REG_ENABLE 0x00
#define COLOR_REG_CONTROL 0x0F
#define COLOR_REG_CDATAL 0x14
#define COLOR_REG_RDATAL 0x16
#define COLOR_REG_GDATAL 0X18
#define COLOR_REG_BDATAL 0x1A
#define COLOR_REG_ATIME 0x01
#define TEMP_ADRESS 0x40
#define SDA PB_9
#define SCL PB_8
#define READ_TEMP_CMD 0xE3
#define READ_HUM_CMD 0xE5
#define LED_C PB_7
#define MINTEMP 10.0
#define MAXTEMP 30.0
#define MINHUM  30.0
#define MAXHUM 70.0

#define MINACC  -10.0
#define MAXACC 10.0


#define REG_WHO_AM_I 0x0D
#define REG_SYSMOD 0x0B
#define REG_CTRL_REG1 0x2A
#define REG_OUT_X_MSB 0x01
#define REG_OUT_X_LSB 0x02
#define REG_OUT_Y_MSB 0x03
#define REG_OUT_Y_LSB 0x04
#define REG_OUT_Z_MSB 0x05
#define REG_OUT_Z_LSB 0x06

#define ACCEL_ADDRS 0x1C
#define STANDARD_GRAVITY 9.8066

class TempHum{//Class to define the Temperature-Humidity Sensor
    
    public: 

        I2C I2CPort;
        TempHum ();
        void readTemp();
        void readHum();
        void printTemp();
        void printHum();
        void PrintMinMaxMean();
        void ResetValues();

        float Temperature;
        float minTemperature;
        float maxTemperature;
        float meanTemperature;
        float TotalTemps;
        
        float Humidity;
        float minHumidity;
        float maxHumidity;
        float meanHumidity;
        float TotalHums;
        int counter;
};


class ColorSensor{//Class to define the Color Sensor 
    public: 
        I2C I2CPort;
        ColorSensor();
        void configColorsensor();
        void readColor();
        void printColor();
        void printHourlyDominant();
        void resetValues();

        float red;
        float blue;
        float green;
        float clear;
        DigitalOut ledColor;
        int dominantColor;// 0 red, 1 blue, 2 green
        int counterRed;//Each counter counts how many reads that color has been dominant in
        int counterBlue;
        int counterGreen;

};

class Accelerometer{ //Class to define the Acceleromter
    public:
        I2C I2CPort;
        Accelerometer();
        void configAccelerometer();
        void readRegs(char * addr, char * data, int len) ;
        

        void readAcc_x ();
        void readAcc_y ();
        void readAcc_z ();

        void printAccelerometer();
        
        void printMinMax();
        void resetValues();

        float acc_x;
        float acc_y;
        float acc_z;

        float min_acc_x;
        float min_acc_y;
        float min_acc_z;

        float max_acc_x;
        float max_acc_y;
        float max_acc_z;

      
};