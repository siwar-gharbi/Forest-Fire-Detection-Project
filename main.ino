
#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(10,11);
#include <String.h>
#include <DHT.h> 
#define MQ2_PIN A1
 int buzzer =9;
#define DHTPIN A0
DHT dht(DHTPIN, DHT11);
 
void setup()
{
  gprsSerial.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);    // the GPRS baud rate 
   dht.begin();
 pinMode(buzzer,OUTPUT);
 digitalWrite(buzzer,LOW);
 delay(100); 
}
 
void loop()
{
      float h = dht.readHumidity();
      float t = dht.readTemperature();
       float mq2_val = analogRead(MQ2_PIN); 
      float seuil_MQ2 =10.0;
      delay(100);   
         
      Serial.print("Temperature = ");
      Serial.print(t);
      Serial.println(" °C");
      Serial.print("Humidity = ");
      Serial.print(h);
      Serial.println(" %");   
      mq2_val = mq2_val/1023*100;
      Serial.println("MQ-2 Data: " + String(mq2_val)); 
     
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());
 
  gprsSerial.println("AT");
  delay(1000);
 
  gprsSerial.println("AT+CPIN?");
  delay(1000);
 
  gprsSerial.println("AT+CREG?");
  delay(1000);
 
  gprsSerial.println("AT+CGATT?");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CSTT=\"internet\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=IQB4UYWDEMRCZ9KI&field1=" + String(t) +"&field2="+String(h)+"&field3="+String(mq2_val);
  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
  
  if ( mq2_val>seuil_MQ2)
  {
  tone (buzzer,500);
  delay(500);
  MakeCall();
  delay(5);
  } 
  else
  {
   noTone  (9);
  }
} 

void MakeCall()
{
  gprsSerial.println("ATD+21624634648;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(100);
}
void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000);        
}
