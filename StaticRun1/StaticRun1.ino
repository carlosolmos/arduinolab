
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

int flag = 0;

// setup
void setup() {
    Serial.begin(9600);           // set up Serial library at 9600 bps
    Serial.println("Motor test!\n");
  
    // setup motor
    motor1.setSpeed(SLOW);
    motor2.setSpeed(SLOW);
   
    motor1.run(RELEASE);
    motor2.run(RELEASE);    

    flag = 0;

}


void loop() {
    
    if(flag == 0){
      Serial.print("go!\n");
      flag = 1;
      rotate_left();
  
     // turn_right();
      freewheel();
      Serial.print("end!\n");
    }         
    
}

// You may need to vary the delays in the drive functions to enable the function to complete

void freewheel(){
    Serial.print("freewheel\n");
    motor1.run(RELEASE);
    motor2.run(RELEASE);
}


void drive_forward(){
    Serial.print("forward\n");
    motor1.run(FORWARD);
    motor2.run(FORWARD);
}

void drive_backward(){
    Serial.print("backward\n");
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
}

void turn_left(){
    Serial.print("left\n");
    motor1.run(RELEASE);
    motor2.run(FORWARD);
    delay(600);
}

void turn_right(){
    Serial.print("right\n");
    motor2.run(RELEASE);
    motor1.run(FORWARD);
    delay(600);
}

void rotate_left(){
    Serial.print("rotate left\n");
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    delay(375);
}

void rotate_right(){
    Serial.print("rotate right\n");
    motor2.run(BACKWARD);
    motor1.run(FORWARD);
    delay(375);
}

void u_turn(){
    Serial.print("uturn\n");
    motor2.run(BACKWARD);
    motor1.run(FORWARD);
    delay(700); // twice as long as rotate right to end up 180 degrees around
}








