/* Simple temperature webpage implementation. 
   Tested hardware: Arduino Uno, ENC28J60 ethernet Shield, DS1820 sensors
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <EtherCard.h>
#include<stdlib.h>

//DS18B20
#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//DS18B20 addresses
DeviceAddress thermo1 = { 0x28, 0x88, 0x42, 0x6F, 0x03, 0x00, 0x00, 0xD1 };
DeviceAddress thermo2 = { 0x28, 0x43, 0x55, 0x2C, 0x03, 0x00, 0x00, 0xF3 };
DeviceAddress thermo3 = { 0x28, 0x1F, 0x20, 0x6F, 0x03, 0x00, 0x00, 0x77 };

//Ethernetshield configzzz
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
byte Ethernet::buffer[500]; // tcp/ip send and receive buffer
static byte myip[] = { 192,168,2,40 };
static byte gwip[] = { 192,168,2,1 };

BufferFiller bfill;

void setup()  
{
  Serial.begin(57600);
  ether.begin(sizeof Ethernet::buffer, mymac, 10);
  ether.staticSetup(myip, gwip);
  sensors.begin();
  sensors.setResolution(thermo1, 10);
  sensors.setResolution(thermo2, 10);
  sensors.setResolution(thermo3, 10);
}
 
float readTempFrom1820(int number) {
    sensors.requestTemperatures();
    switch (number) 
    {
      case 1:  return sensors.getTempC(thermo1); break;
      case 2:  return sensors.getTempC(thermo2); break;
      case 3:  return sensors.getTempC(thermo3); break;
      default: return -1; break;
    }
}

static word homePage() {
  char temp1[10];
  char temp2[10];
  char temp3[10];
  
  dtostrf(readTempFrom1820(1),4,1,temp1);
  dtostrf(readTempFrom1820(2),4,1,temp2);
  dtostrf(readTempFrom1820(3),4,1,temp3);
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='10'/>"
    "<title>Temp server</title>" 
    "<h1>Temp1: $S</h1>"
    "<h1>Temp2: $S</h1>"
    "<h1>Temp3: $S</h1>"),
      temp1,temp2,temp3);
  return bfill.position();
}

void loop(){
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  if (pos)  // check if valid tcp data is received
    ether.httpServerReply(homePage()); // send web page data
}