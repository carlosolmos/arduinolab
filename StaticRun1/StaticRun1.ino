
// Motors
#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

// PWM motor control settings applied to ENABLE1, ENABLE2

#define STOP 0
#define SLOW 150
#define MEDIUM 200
#define FAST 255

// run motor at this speed - set to a number between 128 and 255
// less than 128 any the motors may not have enough torque to move
// (of course you could enhance the speed control to accelerate/decelerate when more/less clear space available as indicated by ping)
int throttle = MEDIUM;

// setup
void setup() {
    
    // setup motor
    motor1.setSpeed(SLOW);
    motor2.setSpeed(SLOW);
   
    motor1.run(RELEASE);
    motor2.run(RELEASE);    

}

// main loop - note that the loop itself does not block (no waiting in the loop)
// functions that need to block (complete before continuing the main loop) need to manage their own delays
//
// the algorithm used is designed to be simple to understand - you can go on to make this much more sophisticated
// guiding the steering to veer away before getting into danger for example
// and accelerating from a stop and decelerating as you get closer to objects
// and of course you could use more ultrasonic detectors, IR sensors etc. to really fine tune your robot's operation

void loop() {
    
                
        drive_forward();
        delay(2000);
        brake();

        u_turn();
        brake();
        delay(2000);
        
        
        drive_forward();
        delay(2000);
        brake();
        
       
    
}

// You may need to vary the delays in the drive functions to enable the function to complete

void freewheel(){
    motor1.run(RELEASE);
    motor2.run(RELEASE);
}

void brake(){
    motor1.run(BRAKE);
    motor2.run(BRAKE);
    delay(100); // braking time
}

void drive_forward(){
    motor1.run(FORWARD);
    motor2.run(FORWARD);
}

void drive_backward(){
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
}

void turn_left(){
    motor1.run(RELEASE);
    motor2.run(FORWARD);
    delay(600);
}

void turn_right(){
    motor2.run(RELEASE);
    motor1.run(FORWARD);
    delay(600);
}

void rotate_left(){
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    delay(375);
}

void rotate_right(){
    motor2.run(BACKWARD);
    motor1.run(FORWARD);
    delay(375);
}

void u_turn(){
    motor2.run(BACKWARD);
    motor1.run(FORWARD);
    delay(700); // twice as long as rotate right to end up 180 degrees around
}








