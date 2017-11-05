/*
* Blink
* Turns on an LED on for one second,
* then off for one second, repeatedly.
*/

#include "Arduino.h"




// #include <ps2.h>
// /*
// * an arduino sketch to interface with a ps/2 mouse.
// * Also uses serial protocol to talk back to the host
// * and report what it finds.
// */
// /*
// * Pin 5 is the mouse data pin, pin 6 is the clock pin
// * Feel free to use whatever pins are convenient.
// */
// PS2 mouse(6, 5);
// /*
// * initialize the mouse. Reset it, and place it into remote
//
// * mode, so we can get the encoder data on demand.
// */
// void mouse_init()
// {
//   mouse.write(0xff); // reset
//   mouse.read(); // ack byte
//   mouse.read(); // blank */
//   mouse.read(); // blank */
//   mouse.write(0xf0); // remote mode
//   mouse.read(); // ack
//   delayMicroseconds(100);
// }
// void setup()
// {
//   Serial.begin(115200);
//   Serial.print("hello");
//   mouse_init();
//   Serial.print("WORLD");
//   pinMode(13, OUTPUT);
// }
// /*
// * get a reading from the mouse and report it back to the
// * host via the serial line
// */
//
// void loop()
// {
//   char mstat;
//   char mx;
//   char my;
//   static int mx_tot;
//   static int my_tot;
//
//   /* get a reading from the mouse */
//   mouse.write(0xeb); // give me data!
//   mouse.read(); // ignore ack
//   mstat = mouse.read();
//   mx = mouse.read();
//   my = mouse.read();
//
//   if(mstat == 0b1100)
//   {
//     mx_tot = 0;
//     my_tot = 0;
//   }
//   mx_tot += mx;
//   my_tot += my;
//   /* send the data back up */
//   Serial.print(mstat, BIN);
//   Serial.print("\tX=");
//   Serial.print(mx, DEC);
//   Serial.print("\tY=");
//   Serial.print(my, DEC);
//
//   Serial.print("\ttX=");
//   Serial.print(mx_tot, DEC);
//   Serial.print("\ttY=");
//   Serial.print(my_tot, DEC);
//
//   Serial.println();
//   delay(10);
//   analogWrite(13, my*10);
//   // delay(20); /* twiddle */
// }
//
//

//both libs are behaving same
#include <PS2Mouse.h>

#define PS2_TWO_MOUSES

#define MOUSE_1_DATA 5
#define MOUSE_1_CLOCK 6

PS2Mouse mouse_1(MOUSE_1_CLOCK, MOUSE_1_DATA, STREAM);

#if defined (PS2_TWO_MOUSES)
#define MOUSE_2_DATA 2
#define MOUSE_2_CLOCK 3

PS2Mouse mouse_2(MOUSE_2_CLOCK, MOUSE_2_DATA, STREAM);
#endif // #if defined (PS2_TWO_MOUSES)

void setup()
{

  Serial.begin(115200);
  Serial.print("hello");

  // initialize LED digital pin as an output.
  //pinMode(LED_BUILTIN, OUTPUT);

  mouse_1.initialize();  
  
  #if defined (PS2_TWO_MOUSES)
  mouse_2.initialize();
  #endif // #if defined (PS2_TWO_MOUSES)


  //delay(3000);

  Serial.print("World");
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  //digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  //delay(100);
  // turn the LED off by making the voltage LOW
  //digitalWrite(LED_BUILTIN, LOW);
  // wait for a second
  //delay(100);


  int data_1[3], data_2[3];
  mouse_1.report(data_1);
  
  #if defined (PS2_TWO_MOUSES)
  mouse_2.report(data_2);
  #endif // #if defined (PS2_TWO_MOUSES)

  char mstat_1 = data_1[0];
  char mx_1 = data_1[1];
  char my_1 = data_1[2];
  static int mx_tot_1;
  static int my_tot_1;
  float ds_1 ;
  
  #if defined (PS2_TWO_MOUSES)
  char mstat_2 = data_2[0];
  char mx_2 = data_2[1];
  char my_2 = data_2[2];
  static int mx_tot_2;
  static int my_tot_2;
  float ds_2 ;
  #endif // #if defined (PS2_TWO_MOUSES)

  if(mstat_1 == 0b1100)
  {
    mx_tot_1 = 0;
    my_tot_1 = 0;
    ds_1 = 0;
  }
  else
  {
    mx_tot_1 += mx_1;
    my_tot_1 += my_1;

    ds_1 = sqrtf(mx_tot_1*mx_tot_1 + my_tot_1*my_tot_1);
  }
  
  #if defined (PS2_TWO_MOUSES)
  if(mstat_2 == 0b1100)
  {
    mx_tot_2 = 0;
    my_tot_2 = 0;
    ds_2 = 0;
  }
  else
  {
    mx_tot_2 += mx_2;
    my_tot_2 += my_2;

    ds_2 = sqrtf(mx_tot_2*mx_tot_2 + my_tot_2*my_tot_2);
  }
  #endif // #if defined (PS2_TWO_MOUSES)
  
  /* send the data back up */
  Serial.print("M1 ");
  Serial.print(mstat_1, BIN);
  Serial.print("\tX=");
  Serial.print(mx_1, DEC);
  Serial.print("\tY=");
  Serial.print(my_1, DEC);

  Serial.print("\ttX=");
  Serial.print(mx_tot_1, DEC);
  Serial.print("\ttY=");
  Serial.print(my_tot_1, DEC);

  Serial.print("\tds=");
  Serial.print(ds_1,5);
 
  #if defined (PS2_TWO_MOUSES) 
  Serial.print("\t|\tM2 ");
  
  Serial.print(mstat_2, BIN);
  Serial.print("\tX=");
  Serial.print(mx_2, DEC);
  Serial.print("\tY=");
  Serial.print(my_2, DEC);

  Serial.print("\ttX=");
  Serial.print(mx_tot_2, DEC);
  Serial.print("\ttY=");
  Serial.print(my_tot_2, DEC);

  Serial.print("\tds=");
  Serial.print(ds_2,5);
  
  #endif // #if defined (PS2_TWO_MOUSES)
  
  Serial.println();
  delay(10);

  //Serial.println();
}
