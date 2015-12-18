/* Laser transmitter module test project
 * tutorial and circuit url http://osoyoo.com/?p=640
*/
char val;
void setup()
{
  Serial.begin(9600); 
  pinMode(13, OUTPUT);
  Serial.print("Start\n");  
}
void loop() {
  val=Serial.read();
  Serial.print(val);
  Serial.print("\n");
  
   if(val=='1')
  {
     digitalWrite(13, HIGH);  
     Serial.print("1===================\n");
   }
  if(val=='0')
  {
    digitalWrite(13, LOW); 
    Serial.print("0+++++++++++++++++++\n"); 
   } 

  
   
  
}
