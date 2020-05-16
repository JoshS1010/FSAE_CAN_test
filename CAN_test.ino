
/*
   Hardware used is Teensy 3.2 http://www.pjrc.com/store/teensy32.html
   WaveShare SN65HVD230 CAN Board http://www.amazon.com/gp/product/B00KM6XMXO?psc=1&redirect=true&ref_=od_aui_detailpages00

  The teensy loader comes preinstalled with a FlexCAN library that must be deleted or this code will not compile.
   The correct FlexCAN library can be found at https://github.com/teachop/FlexCAN_Library/blob/master/examples/CANtest/CANtest.ino
*/


#include <FlexCAN.h>
#include <kinetis_flexcan.h>

FlexCAN CANbus(500000);
static CAN_message_t rxmsg;

//Megasquirt data vars
byte indicator[7]; // where to store indicator data
float  BATTV, IAC, dwell, idle_tar, AFRtgt, AFR, newBATTV, oldBATTV , MAP, TPS, CLT = 0;
unsigned int SPKADV, RPM, MAT, injduty, Baro, PW1, nexAFR, nexCLT, VSS = 0;

void setup() {


  //Initialize CAN and start Serial and Serial2, Serial2 is for Nextion display
  Serial.begin(9600);
  CANbus.begin();


}


void loop(void) {

  // Gauge display function
//  Serial.println(TPS);

  //Look for CAN broadcasts
  if ( CANbus.read(rxmsg) ) {
    Serial.println(rxmsg.id);
    switch (rxmsg.id){
      case 1512:
        MAP = (float)(word(rxmsg.buf[0], rxmsg.buf[1]))/10.0;
        RPM = (float)(word(rxmsg.buf[2], rxmsg.buf[3]));
        CLT = (float)(word(rxmsg.buf[4], rxmsg.buf[5]))/10.0;
        TPS = (float)(word(rxmsg.buf[6], rxmsg.buf[7]))/10.0;
        Serial.print("MAP: ");
        Serial.println(MAP);
        Serial.print("RPM: ");
        Serial.println(RPM);
        Serial.print("CLT: ");
        Serial.println(CLT);
        Serial.print("TPS: ");
        Serial.println(TPS);
        break;
      case 1515:
        BATTV = (float)(word(rxmsg.buf[0], rxmsg.buf[1]))/10.0;
        break;
      case 1516:
        VSS = (float)(word(rxmsg.buf[0], rxmsg.buf[1]))/10.0;
      default:
        break;
    }
  }
}
