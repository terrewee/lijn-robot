

#include "BrickPi3.h"   // for BrickPi3
#include <iostream>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iomanip>

using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

int masterRotate = 150;
int masterForward = 200;

//Stop
void stop(void){
        BP.set_motor_power(PORT_B, 0);
        BP.set_motor_power(PORT_C, 0);
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

bool checkObstacleInRange(sensor_ultrasonic_t ultrasonic, int &powerA, int &powerB){
    
    while(true){
    int detectRange = 8;

        cout << "cm " << ultrasonic.cm << endl;

        if(detectRange <= ultrasonic.cm){
        stop();
        }
        else{
        fwd(powerA, powerB);
    }
        if(BP.get_sensor(PORT_2, ultrasonic) == 0){
            if(ultrasonic.cm <= detectRange+1 && ultrasonic.cm > 0){stop();}
            else if(detectRange < ultrasonic.cm+1 && ultrasonic.cm+2 > 0){fwd(powerA, powerB); break;}
    }
        else{cout << "Error -7: Ultrasonic sensor not properly connected or initialized.";}
    }
}     

void crossroad(int &powerA, int &powerB, sensor_color_t Color1, sensor_color_t Color4){

    BP.set_motor_dps(PORT_B, 0);
    BP.set_motor_dps(PORT_C, 0);
    char keuze;
    cout << "Links: A, Rechts: D, vooruit: W " << endl;
    cin >> keuze;
    if(keuze == 'A' || keuze == 'a'){
        BP.set_motor_dps(PORT_B, 30);
        BP.set_motor_dps(PORT_C, 30);
        sleep(2);
        stop();
        while(true){
            BP.set_motor_dps(PORT_B, 180); 
            BP.set_motor_dps(PORT_C, -180);
            if((BP.get_sensor(PORT_1, Color1) == 0)&&(BP.get_sensor(PORT_4, Color4) == 0)){
                if(Color4.color == 1){cout << "x" << endl; fwd(powerA, powerB); break;}
            }
        }
    }
    if(keuze == 'D' || keuze == 'd'){
        BP.set_motor_dps(PORT_B, 30);
        BP.set_motor_dps(PORT_C, 30);
        sleep(2);
        stop();
        while(true){
            BP.set_motor_dps(PORT_B, -180); 
            BP.set_motor_dps(PORT_C, 180);
            if((BP.get_sensor(PORT_1, Color1) == 0)&&(BP.get_sensor(PORT_4, Color4) == 0)){
                if(Color1.color == 1){cout << "x" << endl; fwd(powerA, powerB); break;}
            }
        }
    }
    if(keuze == 'W' || keuze == 'w'){
        BP.set_motor_dps(PORT_B, 30);
        BP.set_motor_dps(PORT_C, 30);
        sleep(2);
        stop();
        fwd(powerA, powerB);
    }
    usleep(50000);
}
void measure(sensor_color_t Color1, sensor_color_t Color4, int powerA, int powerB, sensor_ultrasonic_t ultrasonic, int &ticker){
    if(ticker == 1000){checkObstacleInRange(ultrasonic, powerA, powerB); ticker = 0;}
    if((BP.get_sensor(PORT_1, Color1) == 0)&&(BP.get_sensor(PORT_4, Color4) == 0)){
    cout << "Color1 " << (int) Color1.color << " Color4 " << (int) Color4.color << endl;
        if     (Color1.color == 1 && Color4.color == 6) {rightcorrectie(powerA, powerB);}        //rechts wit links zwart
        else if(Color1.color == 6 && Color4.color == 1) {leftcorrectie(powerA, powerB);}         //rechts zwart links wit
        else if(Color1.color == 6 && Color4.color == 6) {fwd(powerA, powerB);}                   //rechts zwart 
        else if(Color1.color == 1 && Color4.color == 1) {crossroad(powerA, powerB, Color1, Color4);} 
        
        ticker++;
        //cout << "ticker: " << ticker << endl;
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

    measure(Color1,Color4, powerA, powerB, ultrasonic, ticker);
    while(true){measure(Color1, Color4, powerA, powerB, ultrasonic, ticker);}
}

void exit_signal_handler(int signo){
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}
