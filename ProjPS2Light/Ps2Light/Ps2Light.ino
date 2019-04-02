#include <ps2.h>

/*
 * an arduino sketch to interface with a ps/2 mouse.
 * Also uses serial protocol to talk back to the host
 * and report what it finds.
 */

/*
 * Pin 5 is the mouse data pin, pin 6 is the clock pin
 * Feel free to use whatever pins are convenient.
 */
#define LED D6
#define CLOCK D1
#define DATA D2
PS2 mouse(CLOCK, DATA);

int ledValue = 0;

/*
 * initialize the mouse. Reset it, and place it into remote
 * mode, so we can get the encoder data on demand.
 */
void mouse_init()
{
  mouse.write(0xff);  // reset
  mouse.read();  // ack byte
  mouse.read();  // blank */
  mouse.read();  // blank */
  mouse.write(0xf0);  // remote mode
  mouse.read();  // ack
  delayMicroseconds(100);
}

void setup()
{
  Serial.begin(115200);
  mouse_init();
  pinMode(LED, OUTPUT);
}

/*
 * get a reading from the mouse and report it back to the
 * host via the serial line.
 */
void loop()
{
  char mstat;
  char mx;
  char my;

  /* get a reading from the mouse */
  mouse.write(0xeb);  // give me data!
  mouse.read();      // ignore ack
  mstat = mouse.read();
  mx = mouse.read();
  my = mouse.read();

  /* send the data back up */
  Serial.print(mstat, BIN);
  Serial.print("\tX=");
  Serial.print(mx, DEC);
  Serial.print("\tY=");
  Serial.print(my, DEC);
  Serial.println();
  if(moves_up(my)) {
    ledValue += 7;
    analogWrite(LED, ledValue);
  }
  if(moves_down(my)) {
    ledValue -= 7;
    analogWrite(LED, ledValue);
  }
  delay(40);  
}

int moves_up(char my){
  if(my > 0 & my < 100){
      return 1;
  }
  return 0;
}

int moves_down(char my){
  if(my > 200 & my < 255){
      return 1;
  }
  return 0;
}






