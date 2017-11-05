
#include <Arduino.h>
#include <Servo.h>

Servo servo ;

#define RANGE (15) // for one servo this is the limit
#define BIAS_CCW  (0)
#define BIAS_CW  (0) //(-20)

#define MIN_RANGE (45-RANGE+BIAS_CW)
#define MAX_RANGE (135+RANGE-BIAS_CCW)

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  servo.attach(D0);
  servo.write(90);
  delay(2000);
  servo.write(MIN_RANGE);
  delay(2000);
  servo.write(MAX_RANGE);
  delay(2000);
  servo.write(90);
  delay(2000);
  while(1);
}

void loop()
{

return;
  #if ( (MIN_RANGE<0) || (MAX_RANGE>180) )
  #error chech range
  #endif

  servo.write(MIN_RANGE);
delay(500);
  servo.write(MAX_RANGE);
  delay(500);
}
