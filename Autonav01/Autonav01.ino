
//
// Copyright oddWires 2013
// May be freely used by any purchaser of an oddWires Arduino Kit for their sole purpose
//
// Motor Shield Connections						
//					
// Servo Connections					
//	Brown	SER1 -			
//	Red	SER1 +			
//	Orange	SER1 S			
//					
// HC-SR04 Ultrasonic Module					
//					
//	Vcc	+5V			
//	Trig	A4			
//	Echo	A5			
//	GND			
//
// Motor 1
// LEFT motor (looking from rear to the robot
//
// This motor will need the inline connector and some wire extensions to reach the terminals
//
// Black wire to left M1 terminal
// Red wire to right motor terminal
//
// Motor 2
// RIGHT motor (looking from rear to the robot
//
// This motor will need the inline connector and some wire extensions to reach the terminals
//
// Black wire to RIGHT M1 terminal
// Red wire to LEFT motor terminal
//
// 6V 4 x AA Battery Box 
//
// Connect to Blue ternminal marked EXT_PWR
// Be very careful - do NOT connect the RED wire to GND or you could destroy the motor board
// +6 V (Red wire)   M+
// GND (Black wire)  GND
//



// Ultrasonic sensor

#include <NewPing.h>

#define TRIG_PIN  A4  
#define ECHO_PIN  A5  

#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pin and maximum distance.

float dangerThreshold = 20.0; // 20 cm - depends on the floor surface and speed setting
 
// Servo

#include <Servo.h> 

#define LEFT 180
#define CENTER 90
#define RIGHT 0

Servo ultrasonicServo;  // create servo object to control a servo 
 
int currentPos = 0;    // variable to store the servo position 
int bestAngle = 90; //front.
int bestDistance = 0;
   
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
    Serial.begin(9600); 
    
    // setup motor
    motor1.setSpeed(SLOW);
    motor2.setSpeed(SLOW);
   
    motor1.run(RELEASE);
    motor2.run(RELEASE);    

    // setup servo
    ultrasonicServo.attach(10,650,1075);  // attaches the servo on pin 5 to the servo object
   
    // move servo to center 
    servo_position(CENTER);  
}


void loop() {
    Serial.print("LOOP");       // prints a label
    Serial.print("\n");   
    float distanceForward = ping();

    Serial.print("PING: "); 
    Serial.print(distanceForward);
    Serial.print("\n");   
    
    if (distanceForward > dangerThreshold) //if path is clear
    {
        Serial.print("FORWARD -> "); 
        Serial.print("\n"); 
        drive_forward();
    }
    else // if path is blocked
    {
        Serial.print("BLOCKED ||"); 
        Serial.print("\n"); 
        
        freewheel();  // see if we can implment faster stop

        //look for a better direction.
        servo_sweep();
        
        // re-center the ultrasonic
        Serial.print("LOOK CENTER\n");
        servo_position(CENTER);
        
        // go the least obstructed way
               
        if (bestAngle > 90 && bestDistance > dangerThreshold)       //if left is less obstructed 
        {
            Serial.print("GO LEFT\n");
            rotate_left();
        }
        else if (bestAngle < 90 && bestDistance > dangerThreshold) //if right is less obstructed 
        {
           Serial.print("GO RIGHT\n");
           rotate_right();
        }
        else // equally blocked or less than danger threshold left or right
        {
            Serial.print("UTURN\n");
            u_turn();
        }       
    }
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

// servo control

void servo_sweep(){

   Serial.print("doing a sweep from ");
   int currentPos = ultrasonicServo.read();   
   Serial.print(currentPos);
   Serial.print("\n");
   
   int startingPos = 0;
   int increment = 1;
   
   if(currentPos <= 90){
     startingPos = 0;
     increment = 1;
   }else{
     startingPos = 180;
     increment = -1;
   }

  Serial.print("Starting pos ");
  Serial.print(startingPos);
  Serial.print("\n");
  
  //go to starting pos
  ultrasonicServo.write(startingPos); 
  delay(25);

  int endPos = startingPos + (increment * 180);
  int pos = startingPos;

  Serial.print("End pos ");
  Serial.print(endPos);
  Serial.print("\n");
  
  while(pos != endPos){
    Serial.print("pos ");
    Serial.print(pos);
    Serial.print("\n");
    
    float distance = ping();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print("\n");
    if(distance > bestDistance){
      bestDistance = distance;
      bestAngle = pos;
      Serial.print("best\n");
    }
    
    if(pos == 70 && increment == 1){
      pos = 110;  
      servo_position(pos);
    }
    
    if(pos == 110 && increment == -1){
      pos = 70; 
      servo_position(pos);
    }
    
    ultrasonicServo.write(pos); // tell servo to go to position in variable 'pos' 
    pos = pos + increment; // in steps of 1 degree 
    delay(30); // waits 15ms for the servo to reach the position 
      
  }
  
  Serial.print("Best pos: ");
  Serial.print(bestAngle);
  Serial.print("\nBest distance: ");
  Serial.print(bestDistance);
  Serial.print("\n");

}

void servo_position(int newPos){
  ultrasonicServo.write(newPos);
  delay(800);
}


// ultrasonic ping

float ping(){
    delay(50);                        // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    unsigned int uS = sonar.ping();   // Send ping, get ping time in microseconds (uS).
    if (uS == 0)                      // out of range (0 = outside set distance range, no ping echo)
        return MAX_DISTANCE;
    else                              // in range
        return uS / US_ROUNDTRIP_CM;  // Convert ping time to distance 
}








