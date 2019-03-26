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
        BP.set_motor_power(PORT_B, 10);
        BP.set_motor_power(PORT_C, 11);
        usleep(500000);
        //BP.set_motor_power(PORT_B, 40);
        //BP.set_motor_power(PORT_C, 40);
        //usleep(500000);
        //BP.set_motor_power(PORT_B, 50);
        //BP.set_motor_power(PORT_C, 50);
        sleep(2);
        stop();
}
//move left
void leftcorrectie(void){
        BP.set_motor_position_relative(PORT_B, 20);
        BP.set_motor_position_relative(PORT_C, -20);
}

//Move Right
void rightcorrectie(void){
        BP.set_motor_position_relative(PORT_B, -20);
        BP.set_motor_position_relative(PORT_C, 20);
}

void measure(sensor_color_t Color1, sensor_color_t Color4){
    int richting = 0;
    if(BP.get_sensor(PORT_1, Color1) == 0){
        //cout << "Color sensor (S1): detected  " << (int) Color1.color;
        //cout << " ambient" << setw(4) << Color1.ambient << endl;
        cout << "Color 1 " << Color1.ambient << endl;
    }
    else if(BP.get_sensor(PORT_4, Color4) == 0){
        //cout << "Color sensor (S1): detected  " << (int) Color1.color;
        //cout << " ambient" << setw(4) << Color4.ambient << endl;
        cout << "Color 4 " << Color4.ambient << endl;
    }
    while(true){
        sleep(2);
        if(BP.get_sensor(PORT_1, Color1) == 0){0;
            //cout << "Color sensor (S1): detected  " << (int) Color1.color;
            //cout << " ambient" << setw(4) << Color1.ambient << endl;
        }
        
        else if(BP.get_sensor(PORT_4, Color4) == 0){
            //cout << "Color sensor (S1): detected  " << (int) Color4.color;
            //cout << " ambient" << setw(4) << Color4.ambient << endl;
        }
        else if(Color1.ambient >= 80 && Color4.ambient < 80){ //rechts wit(1 is rechts)
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
    //einde while
    }
        
}

int main(){
    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_COLOR_FULL);

    sensor_color_t      Color1;
    sensor_color_t      Color3;
    
    measure(Color1,Color3);
    while(true){
        measure(Color1, Color3);
            cout << "....." << endl;
            sleep(1);
        
}
}
void exit_signal_handler(int signo){
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}