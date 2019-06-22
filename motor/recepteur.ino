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
      
     if(vw_get_message(buf, &buflen))
      {
       if(buf[0] == id) {
          
        int i;
        Serial.print("Reçu: ");
        for(i = 0; i < buflen - 1; ++i)
        {
        conversion = (int) buf[i+1] - key;
        message[i] = (char) conversion;
          Serial.print((char) message[i]);
          Serial.print(" ");
        }
        Serial.println("");
      }
      }  
      delay(100);
    }
