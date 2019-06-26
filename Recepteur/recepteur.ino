#include <VirtualWire.h>

int RF_RX_PIN = 2;

void setup()
{
  Serial.begin(9600);
  Serial.println("setup");
  vw_set_rx_pin(RF_RX_PIN);
  vw_setup(2000);
  vw_rx_start(); 
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  char message[VW_MAX_MESSAGE_LEN];
  int conversion = 0;
  int key = 5;
  int id = 155;

  String instruction = "node";
  /* Instructions:
  *  node => Send the current node
  *  nextNode => Send the next node
  *  state => Send the state (start, end)
  *  path => Send the full path
  *  placeStart => Send a signal of staring parking space
  *  placeEnd => Send a signal of ending parking space
  *  0 => Other (will not be read by the Java interface)
  */

 /* Serial.print(instruction);
  Serial.print(":");
  Serial.println(1);*/
  
 if(vw_get_message(buf, &buflen))
  {
   if(buf[0] == id) {
    int i;
    //Serial.print("Reçu: ");
    for(i = 0; i < buflen - 1; ++i)
    {
    conversion = (int) buf[i+1] - key;
    message[i] = (char) conversion;
      Serial.print((char) message[i]);
    }
    Serial.println("");
  }
  }  
  //delay(100);
}
