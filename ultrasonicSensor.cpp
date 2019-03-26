/*
 *  https://www.dexterindustries.com/BrickPi/
 *  https://github.com/DexterInd/BrickPi3
 *
 *  Copyright (c) 2017 Dexter Industries
 *  Released under the MIT license (http://choosealicense.com/licenses/mit/).
 *  For more information, see https://github.com/DexterInd/BrickPi3/blob/master/LICENSE.md
 *
 *  This code is an example for reading the encoders of motors connected to the BrickPi3.
 *
 *  Hardware: Connect NXT sensors to the sensor ports. Color sensor to PORT_1. Ultrasonic sensor to PORT_2. Light sensor to PORT_3. Touch sensor to PORT_4 (EV3 or NXT touch sensor).
 *
 *  Results:  When you run this program, you should see the values for each sensor.
 *
 *  Example compile command:
 *    g++ -o program senxor_nxt.cpp BrickPi3.cpp
 *  Example run command:
 *    ./program
 *
 */

#include "BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>      // for setw and setprecision

using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);




void printUltrasonicValue(){
    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
 
    BP.set_sensor_type(PORT_3,SENSOR_TYPE_NXT_ULTRASONIC);

    sensor_ultrasonic_t ultrasonic;
    if(BP.get_sensor(PORT_3, ultrasonic) == 0){
        cout << "Ultrasonic sensor (S3): "   << ultrasonic.cm << "cm" << endl;
    }
    else{
        cout << "Error -3: Ultrasonic sensor not properly connected or initialized.";
    }
}

float getUltrasonicSensor(){
    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
 
    BP.set_sensor_type(PORT_3,SENSOR_TYPE_NXT_ULTRASONIC);

    sensor_ultrasonic_t ultrasonic;
    if(BP.get_sensor(PORT_3, ultrasonic) == 0){
        return ultrasonic.cm;
    }
    else{
        cout << "Error -3: Ultrasonic sensor not properly connected or initialized.";
    }
}

bool CheckObstacleInRange(const float detectRange){
    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
 
    BP.set_sensor_type(PORT_3,SENSOR_TYPE_NXT_ULTRASONIC);

    sensor_ultrasonic_t ultrasonic;
    if(BP.get_sensor(PORT_3, ultrasonic) == 0){
        
        if(detectRange >= ultrasonic.cm){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        cout << "Error -3: Ultrasonic sensor not properly connected or initialized.";
    }
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
}

int main(){
    BrickPi3 BP;
    BP.set_sensor_type(PORT_3,SENSOR_TYPE_NXT_ULTRASONIC);
    sensor_ultrasonic_t ultrasonic;
    printUltrasonicValue();
    float afstand = getUltrasonicSensor();
    cout << "Afstand" << afstand << endl;
    bool result = CheckObstacleInRange(afstand);
    cout << "Result" << result << endl;
    }