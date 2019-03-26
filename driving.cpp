#include "BrickPi3.h"   // for BrickPi3
#include <iostream>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iomanip>
#include <conio.h>      // keystroke input

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
        BP.set_motor_power(PORT_B, 50);
        BP.set_motor_power(PORT_C, 55);
        usleep(500000);

}
//move left
void leftcorrectie(void){
        BP.set_motor_power(PORT_B, 30);
        BP.set_motor_power(PORT_C, -33);
}

//Move Right
void rightcorrectie(void){
        BP.set_motor_power(PORT_B, -30);
        BP.set_motor_power(PORT_C, 33);
}

void measure(sensor_color_t Color1, sensor_color_t Color4){
    
    
    if(Color1.ambient >= 80 && Color4.ambient < 80){ //rechts wit(1 is rechts)
        cout << "Turn left" << endl;
        leftcorrectie();
    }
    else if(Color1.ambient < 80 && Color4.ambient >= 80){//rechts zwart
        cout << "Turn right" << endl;
        rightcorrectie();
    }
    else if(Color1.ambient > 100 && Color4.ambient > 100){//rechts zwart
        cout << "LOST" << endl;
    }
    else{
        fwd();
    }
    cout << "Color 1 " << Color1.ambient << endl;   
    
        
}

int main(){
    while(true){
        signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

        BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

        BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
        BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_COLOR_FULL);

        sensor_color_t Color1;
        sensor_color_t Color4;
        
        measure(Color1, Color4);
    }
}

void exit_signal_handler(int signo){
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}