#include "BrickPi3.h"   // for BrickPi3
#include <iostream>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iomanip>

using namespace std;

BrickPi3 BP;

struct rgbSensorValue{
    int sensor1;
    int sensor4;
};

void exit_signal_handler(int signo);

int masterRotate = 150;
int masterForward = 200;

//Stop
void stop(void){
        BP.set_motor_power(PORT_B, 0);
        BP.set_motor_power(PORT_C, 0);
}

void stopforobject(void){
BP.set_motor_power(PORT_B, 0);
BP.set_motor_power(PORT_C, 0);        
usleep(35000);
}
//forward
void fwd(int &powerA, int &powerB){
    if      (powerA > masterForward){powerA -= masterRotate;}
    else if (powerA < masterForward){powerA += masterRotate;}
    if      (powerB > masterForward){powerB -= masterRotate;}
    else if (powerB < masterForward){powerB += masterRotate;}

    BP.set_motor_dps(PORT_B, powerA);
    BP.set_motor_dps(PORT_C, powerB);
}
//move left
void leftcorrectie(int &powerA, int &powerB){
    powerA += masterRotate;
    powerB -= masterRotate;

    BP.set_motor_dps(PORT_B, powerA);
    BP.set_motor_dps(PORT_C, powerB);
}

//Move Right
void rightcorrectie(int &powerA, int &powerB){
    powerA -= masterRotate;
    powerB += masterRotate;

    BP.set_motor_dps(PORT_B, powerA);
    BP.set_motor_dps(PORT_C, powerB);
}  

void checkObstacleInRange(sensor_ultrasonic_t ultrasonic, bool & obstacle){
    
    while(true){
    int detectRange = 5;

        cout << "cm " << ultrasonic.cm << endl;

        if(detectRange >= ultrasonic.cm) + 10.0 && ultrasonic.cm > 0.0){
            obstakel = 1;
        }
    
        else if(detectRange < ultrasonic.cm && ultrasonic.cm >= 0.0){
            obstacle = 0;
        }
        
        if(BP.get_sensor(PORT_2, ultrasonic) == 0){
            if(ultrasonic.cm <= detectRange + 10.0 && ultrasonic.cm > 0.0){
                stopforobject();
                
            }

            else if(detectRange < ultrasonic.cm+1 && ultrasonic.cm+2 > 0){
                fwd(powerA, powerB); break;
                
            }

        else{
            cout << "Error -7: Ultrasonic sensor.";
        }
        usleep(35000);
    }
}      

void crossroad(int &powerA, int &powerB){
    BP.set_motor_dps(PORT_B, 0);
    BP.set_motor_dps(PORT_C, 0);
    char keuze;
    cout << "Links: A, Rechts: D, vooruit: W " << endl;
    cin >> keuze;
    if(keuze == 'A' || keuze == 'a'){
        BP.set_motor_dps(PORT_B, 180);
        BP.set_motor_dps(PORT_C, -180);
    }
    if(keuze == 'D' || keuze == 'd'){
        BP.set_motor_dps(PORT_B, -180);
        BP.set_motor_dps(PORT_C, 180);
    }
    if(keuze == 'W' || keuze == 'w'){
        fwd(powerA, powerB);
    }
    usleep(50000);
}

rgbSensorValue measure(sensor_color_t Color1, sensor_color_t Color4){

    if((BP.get_sensor(PORT_1, Color1) == 0)&&(BP.get_sensor(PORT_4, Color4) == 0)){
        rgbSensorValue rgb;
        rgb.sensor1 = Color1.color;
        rgb.sensor4 = Color4.color;
        return rgb;
    }
    else{
        cout << "Error: rgb sensors.";
    }
    
}

// void measure(sensor_color_t Color1, sensor_color_t Color4, int powerA, int powerB, sensor_ultrasonic_t ultrasonic, int &ticker, bool & obstacle){
//     //if(ticker == 500){checkObstacleInRange(ultrasonic, powerA, powerB); ticker = 0;}
//     if(obstacle){
//         stop();
//     }
//     else{
//         if((BP.get_sensor(PORT_1, Color1) == 0)&&(BP.get_sensor(PORT_4, Color4) == 0)){
//             cout << "Color1 " << (int) Color11.color << " Color4 " << (int) Color44.color << endl;
//             if     (Color11.color == 1 && Color44.color == 6) {rightcorrectie(powerA, powerB);}        //rechts wit links zwart
//             else if(Color11.color == 6 && Color44.color == 1) {leftcorrectie(powerA, powerB);}         //rechts zwart links wit
//             else if(Color11.color == 6 && Color44.color == 6) {fwd(powerA, powerB);}                   //rechts zwart 
//             else if(Color11.color == 1 && Color44.color == 1) {crossroad(powerA, powerB);} 
            
//             //ticker++;
//             //cout << "ticker: " << ticker << endl;
//         }
//     }
    
// }

void movement(sensor_color_t Color1, sensor_color_t Color4, int powerA, int powerB, bool obstacle){
    rgbSensorValue rgb = measure(Color1, Color4);
    
    if(obstacle){
        stop();
    }
    else{
        if(rgb.sensor1 == 1 && rgb.sensor4 == 6){
            rightcorrectie(powerA, powerB);
        }        //rechts wit links zwart
        else if(rgb.sensor1 == 6 && rgb.sensor4 == 1){
            leftcorrectie(powerA, powerB);
        }         //rechts zwart links wit
        else if(rgb.sensor1 == 6 && rgb.sensor4 == 6){
            fwd(powerA, powerB);
        }                   //rechts zwart 
        else if(rgb.sensor1 == 1 && rgb.sensor4 == 1){
            crossroad(powerA, powerB);
        } 
    } 
}

int main(){
   signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);

    sensor_color_t      Color1;
    sensor_color_t      Color4;
    sensor_ultrasonic_t ultrasonic;

    BP.set_motor_power(PORT_B, 40);
    BP.set_motor_power(PORT_C, 44);

    int powerA = masterForward;
    int powerB = masterForward;

    int ticker = 0;
    bool obstacle = 0;

    measure(Color1,Color4);
    thread thread1 (checkObstacleInRange, obstacle);  
    while(true){
        thread thread2 (movement, Color1, Color4, powerA, powerB, obstacle);
    }
    
    //measure(Color1,Color4, powerA, powerB, ultrasonic, ticker);
    //while(true){
    //    measure(Color1, Color4, powerA, powerB, ultrasonic, ticker);
    //}
}

void exit_signal_handler(int signo){
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}
