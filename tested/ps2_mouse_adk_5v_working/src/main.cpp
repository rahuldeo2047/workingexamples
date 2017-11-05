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

#ifdef ESP8266
extern "C" {
  #include "user_interface.h"
  #define MOUSE_DATA D3 //5
  #define MOUSE_CLOCK D4 //6
}
#else

#define MOUSE_DATA 5
#define MOUSE_CLOCK 6

#endif

PS2Mouse mouse(MOUSE_CLOCK, MOUSE_DATA, STREAM);

void setup()
{

  Serial.begin(115200);
  Serial.print("hello");

#ifdef ESP8266
  ESP.wdtEnable(20000);
#endif

  // initialize LED digital pin as an output.
  //pinMode(LED_BUILTIN, OUTPUT);

  mouse.initialize();


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


  int16_t data[3];
  mouse.report(data);

  int mstat = data[0];
  int mx = (unsigned int)data[1];
  int my = (unsigned int)data[2];
  static int mx_tot;
  static int my_tot;
  float ds ;

  if(mstat == 0b1100)
  {
    mx_tot = 0;
    my_tot = 0;
    ds = 0;
  }
  else
  {
    mx_tot += mx;
    my_tot += my;

    ds = sqrtf(mx_tot*mx_tot + my_tot*my_tot);
  }
  /* send the data back up */

  #ifdef ESP8266

  Serial.printf("M1 st=0x%x\tX=%d\tY=%d\tTX=%d\tTY=%d", mstat, mx, my, mx_tot, my_tot);

  #else
  Serial.print("M1 ");
  Serial.print(mstat, BIN);
  Serial.print("\tX=");
  Serial.print(mx, DEC);
  Serial.print("\tY=");
  Serial.print(my, DEC);

  Serial.print("\ttX=");
  Serial.print((int)mx_tot, DEC);
  Serial.print("\ttY=");
  Serial.print((int)my_tot, DEC);

  #endif //#ifdef ESP8266

  Serial.print("\tds=");
  Serial.print(ds,5);

  Serial.println();
  delay(200);

  //Serial.println();
}
