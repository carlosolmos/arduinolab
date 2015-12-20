
//
// DETECCION DE OBSTACULOS CON SENSOR ULTRASONICO Y SERVO.
//
// Motor Shield Connections            
//          
// Servo Connections          
//  Brown SER1 -      
//  Red SER1 +      
//  Orange  SER1 S      
//          
// HC-SR04 Ultrasonic Module          
//          
//  Vcc +5V     
//  Trig  A4      
//  Echo  A5      
//  GND     
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

Servo theServo;  // create servo object to control a servo 
 
int currentPos = 0;    // variable to store the servo position 
int flag = 0;

// setup
void setup() {
    Serial.begin(9600); 
    
    // setup servo
    theServo.attach(10,650,1075);  // attaches the servo on pin 5 to the servo object
   
    
}


void loop() {

    if(flag == 0){
      Serial.print("Center");
      flag = 1;
      //test sweep
      servo_180();
      delay(25);
      servo_180();
    }

    
   
}



// servo control


void servo_180(){
   Serial.print("doing a 180 from ");
   int currentPos = theServo.read();
   
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
  theServo.write(startingPos);                  // tell servo to go to position in variable 'pos' 
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
    
    theServo.write(pos); // tell servo to go to position in variable 'pos' 
    pos = pos + increment; // in steps of 1 degree 
    delay(30); // waits 15ms for the servo to reach the position 
      
  }
   
}


//sweep from 0 to newPos
void servo_sweep(int newPos){
    if(newPos < 0)
      newPos = 0;
      
    if(newPos > 180)
      newPos = 180;
        
    for(int pos=0; pos < newPos; pos += 1)  // goes from 0 degrees to 180 degrees 
    {                                                // in steps of 1 degree 
        theServo.write(pos);                  // tell servo to go to position in variable 'pos' 
        delay(15);                                   // waits 15ms for the servo to reach the position 
    }  
}

//go to position
void servo_position(int newPos){
  theServo.write(newPos);
  delay(25);
}


// ultrasonic ping

float ping(){
    delay(60);                        // Wait 60ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    unsigned int uS = sonar.ping();   // Send ping, get ping time in microseconds (uS).
    if (uS == 0)                      // out of range (0 = outside set distance range, no ping echo)
        return MAX_DISTANCE;
    else                              // in range
        return uS / US_ROUNDTRIP_CM;  // Convert ping time to distance: uS / 58 = centimeters
}





