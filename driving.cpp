#include "BrickPi3.h"   // for BrickPi3
#include <iostream>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iomanip>

using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

//Stop
void stop(void){
        BP.set_motor_power(PORT_B, 0);
        BP.set_motor_power(PORT_C, 0);
}
//forward
void fwd(void){
        BP.set_motor_dps(PORT_B, 360);
        BP.set_motor_dps(PORT_C, 360);
        usleep(500000);

}
//move left
void leftcorrectie(void){
        BP.set_motor_dps(PORT_B, 180);
        BP.set_motor_dps(PORT_C, -180);
}

//Move Right
void rightcorrectie(void){
        BP.set_motor_dps(PORT_B, -180);
        BP.set_motor_dps(PORT_C, 180);
}

void measure(sensor_color_t Color1, sensor_color_t Color4){
    if((BP.get_sensor(PORT_1, Color1) == 0)&&(BP.get_sensor(PORT_4, Color4) == 0)){
        if((Color1.color == 1 )&&(Color4.color == 6)){rightcorrectie();}       //rechts wit links zwart
        if((Color1.color == 6)&&(Color4.color == 1)){leftcorrectie();}         //rechts zwart links wit
        if((Color1.color == 6)&&(Color4.color == 6)){fwd();}                   //rechts zwart 
        cout << "Color1 " << (int) Color1.color << " Color4 " << (int) Color4.color << endl;
    }
}

int main(){
   signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_COLOR_FULL);

    sensor_color_t      Color1;
    sensor_color_t      Color4;

    BP.set_motor_power(PORT_B, 20);
    BP.set_motor_power(PORT_C, 22);

    measure(Color1,Color4);
    while(true){
        measure(Color1, Color4);
    }
}

void exit_signal_handler(int signo){
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}
