#include "mbed.h"
#include "I2C_Port.h"
#include <cstdio>

TempHum::TempHum(): I2CPort(SDA, SCL){// Contructor. Declare the I2C Port and Set all the values to 0.
    
    Temperature = 0.0;
    Humidity = 0.0;
    ResetValues();
}


void TempHum::readHum(){

    char command[1];
    char hum[2];
    command[0] = READ_HUM_CMD;

    I2CPort.write(TEMP_ADRESS << 1, command, 1);
    I2CPort.read(TEMP_ADRESS << 1, hum, 2);
    float aux = (hum[0]<<8) + hum[1];
    Humidity = (125*aux)/65536 - 6;
    if(Humidity < minHumidity){minHumidity = Humidity;}
    if(Humidity > maxHumidity){maxHumidity = Humidity;}
    TotalHums += Humidity;
    meanHumidity = TotalHums/counter;
}

void TempHum::readTemp(){

    char command[1];
    char temp[2];
    command[0] = READ_TEMP_CMD;

    I2CPort.write(TEMP_ADRESS << 1, command, 1);
    I2CPort.read(TEMP_ADRESS << 1, temp, 2);
    float aux = (temp[0]<<8) + temp[1];
    Temperature = (175.72*aux)/65536 - 46.85;
    if(Temperature < minTemperature){minTemperature = Temperature;}
    if(Temperature > maxTemperature){maxTemperature = Temperature;}
    counter ++;
    TotalTemps = TotalTemps + Temperature;
    meanTemperature = TotalTemps/counter;
}

void TempHum ::printTemp(){
    readTemp();
    printf("Temperature: %.2f ºC\n", Temperature);
} 

void TempHum ::printHum(){
    readHum();
    printf("Relative Humidity: %.2f %%\n", Humidity);
}

void TempHum :: ResetValues(){
    minTemperature = MAXTEMP;
    maxTemperature = MINTEMP;
    meanTemperature = 0.0;
    minHumidity = MAXHUM;
    maxHumidity = MINHUM;
    meanHumidity = 0.0;
    counter = 0;
    TotalHums = 0;
    TotalTemps = 0;
}

void TempHum :: PrintMinMaxMean(){
    printf("IN THE LAST HOUR: \n");
    printf("TEMPERATURE:\n");
    printf(" -Min: %.2f ºC \n", minTemperature);
    printf(" -Max: %.2f ºC \n", maxTemperature);
    printf(" -Mean: %.2f ºC \n", meanTemperature);
    printf("HUMIDITY:\n");
    printf(" -Min: %.2f %% \n", minHumidity);
    printf(" -Max: %.2f %% \n", maxHumidity);
    printf(" -Mean: %.2f %% \n", meanHumidity);
    ResetValues();
}


ColorSensor::ColorSensor():I2CPort(SDA, SCL), ledColor(LED_C){
    red = 0.0;
    green = 0.0;
    blue = 0.0;
    clear = 0.0;
    resetValues();
}

void ColorSensor::configColorsensor(){
    ledColor=1;
    char cmd[2];

     // ENABLE PON
  cmd[0] = COLOR_REG_ENABLE | 0X80;
  cmd[1] = 0x01;
  I2CPort.write(COLOR_ADDR << 1, cmd, 2);
  
  // Activate AEN
    cmd[1] = 0x01 | 0x02;
    I2CPort.write(COLOR_ADDR << 1, cmd, 2);

     //1xGAIN
    cmd[0] = COLOR_REG_CONTROL | 0X80;
    cmd[1] = 0x01;
    I2CPort.write(COLOR_ADDR << 1, cmd, 2);
    wait_us(3000);


    //ATIME 101 ms
    cmd[0] = COLOR_REG_ATIME | 0X80;
    cmd[1] = 0xD5;
    I2CPort.write(COLOR_ADDR << 1, cmd, 2);

}

void ColorSensor::readColor(){

    configColorsensor();
    uint16_t r, g, b, c;
    char cr[1] = {COLOR_REG_CDATAL | 0x80};
    char color[8];
    I2CPort.write(COLOR_ADDR << 1, cr, 1);
    I2CPort.read(COLOR_ADDR << 1, color, 8);
    c = (color[1] << 8 | color[0]);
    r = (color[3] << 8 | color[2]);
    g = (color[5] << 8 | color[4]);
    b = (color[7] << 8 | color[6]);
    clear = c;

    //rgb values from 0 to 255
    uint32_t sum = c;
    if (c == 0) {
        red = green = blue = 0.0;
    } else {
        red = (float)r / sum * 255.0;
        green = (float)g / sum * 255.0;
        blue = (float)b / sum * 255.0;
    }

    if(red > green){
        if(red > blue){ 
        dominantColor = 0;//red dominant
        counterRed ++;
        } else {
            dominantColor = 1; //blue dominant
            counterBlue ++;
        }
    } else if (green > blue) {
        dominantColor = 2; //green dominant
        counterGreen ++;
        } else {
                dominantColor = 1; //blue dominant
                counterBlue ++;
            }
}

void ColorSensor::printColor(){
    
    readColor();
    switch (dominantColor) {
    case 0: 
        printf("Color Sensor: Clear: %.2f, Red: %.2f, Blue: %.2f, Green: %.2f. Dominat Color: RED\n",clear, red, blue, green);
        break;
    case 1: 
        printf("Color Sensor: Clear: %.2f, Red: %f, Blue: %f, Green: %.2f. Dominat Color: BLUE\n",clear, red, blue, green);
        break;
    case 2:
        printf("Color Sensor: Clear: %.2f, Red: %.2f, Blue: %.2f, Green: %.2f. Dominat Color: GREEN\n",clear, red, blue, green);
        break;
    }
    
    
}

void ColorSensor::printHourlyDominant(){
    if(counterRed > counterGreen){
        if(counterRed > counterBlue){
            dominantColor = 0;
        } else {
            dominantColor = 1;
        }
    } else if (counterGreen > counterBlue) {
                dominantColor = 2;
            } else {
                dominantColor = 1;
            }
    switch (dominantColor) {
    case 0: 
        printf("COLOR: RED\n");
        break;
    case 1: 
        printf("COLOR: BLUE\n");
        break;
    case 2:
        printf("COLOR: GREEN\n");
        break;
    }
    resetValues();
}

void ColorSensor::resetValues(){
    counterBlue = 0;
    counterGreen = 0;
    counterRed = 0;
}


Accelerometer::Accelerometer():I2CPort(SDA, SCL){
    acc_x = 0.0;
    acc_y = 0.0;
    acc_z = 0.0;
    resetValues();
    configAccelerometer();
}
void Accelerometer::configAccelerometer(){// Configuration to initialize the Acc.
    char ctrlReg[2];
    ctrlReg[0] = REG_CTRL_REG1;
    ctrlReg[1] = 0X01;
    I2CPort.write(0x1c <<1, ctrlReg,2);
}

void Accelerometer::readRegs(char * addr, char * data, int len) {// Auxiliar function to read a specific register in the Acc.
    I2CPort.write(0x1c << 1, addr, 1, true);
    I2CPort.read(0x1c << 1, data, len);
}

void Accelerometer::readAcc_x (){
    char x_regaddr[1] = {REG_OUT_X_MSB};
    char x_data[2];
    int16_t acc_1;
    readRegs( x_regaddr , x_data, 2);
    
      acc_1 = (x_data[0] << 6) | (x_data[1] >> 2);
        if (acc_1 > 16383/2) {//Check if negative or positive
        acc_1 -= 16384; //Transform negative numbers from Ca2
      }
    
    acc_x = float(acc_1 / 4096.0) * STANDARD_GRAVITY;
    if(acc_x < min_acc_x){min_acc_x = acc_x;}
    if(acc_x > max_acc_x){max_acc_x = acc_x;}
}

void Accelerometer::readAcc_y (){
    char y_regaddr[1] = {REG_OUT_Y_MSB};
    char y_data[2];
    int16_t acc_1;
    readRegs( y_regaddr , y_data, 2);
    
      acc_1 = (y_data[0] << 6) | (y_data[1] >> 2);
       
        if (acc_1 > 16383/2) { 
        acc_1 -= 16384;        
      }
    
      acc_y = float(acc_1 / 4096.0) * STANDARD_GRAVITY; //Normalize: Each count corresponds to 1g/4096
      if(acc_y < min_acc_y){min_acc_y = acc_y;}
      if(acc_y > max_acc_y){max_acc_y = acc_y;}
}

void Accelerometer::readAcc_z (){
    char z_regaddr[1] = {REG_OUT_Z_MSB};
    char z_data[2];
    int16_t acc_1;
    readRegs( z_regaddr , z_data, 2);
    
      acc_1 = (z_data[0] << 6) | (z_data[1] >> 2);
        
        if (acc_1 > 16383/2) {
        acc_1 -= 16384; // change how to convert the CA2
      }
    
      acc_z = float(acc_1 / 4096.0) * STANDARD_GRAVITY;
      if(acc_z < min_acc_z){min_acc_z = acc_z;}
      if(acc_z > max_acc_z){max_acc_z = acc_z;}
}

void Accelerometer::resetValues(){
    min_acc_x = MAXACC;
    max_acc_x = MINACC;
    min_acc_y = MAXACC;
    max_acc_y = MINACC;
    min_acc_z = MAXACC;
    max_acc_z = MINACC;
}

void Accelerometer::printAccelerometer(){
    readAcc_x();
    readAcc_y();
    readAcc_z();
    printf("Accel (x,y,z) m/s2 = (%.2f,%.2f,%.2f)\n", acc_x, acc_y, acc_z);
}

void Accelerometer::printMinMax(){
    printf("ACCELEROMETER:\n");
    printf(" -Min X: %.2f  \n", min_acc_x);
    printf(" -Max X: %.2f  \n", max_acc_x);
    printf(" -Min Y: %.2f  \n", min_acc_y);
    printf(" -Max Y: %.2f  \n", max_acc_y);
    printf(" -Min Z: %.2f  \n", min_acc_z);
    printf(" -Max Z: %.2f  \n", max_acc_z);
    resetValues();
}

