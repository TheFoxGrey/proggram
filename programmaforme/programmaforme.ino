#include <DHT.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Servo.h>
Servo window;
Servo door;
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x4C, 0xFB };
byte ip[] = { 192, 168, 1, 36 };

byte server[] = { 192, 168, 1, 35 };

EthernetClient client;
char c;
String msg;
//ДАТЧИК УЛЬТРАЗВУКА НА КУХНЕ
int kitecho= 9; 
int kittrig = 8;
//ДАТЧИКИ УЛЬТРАЗВУКА У ДВЕРИ
  //ВЕРХНИЙ
int hecho= 9; 
int htrig = 8;
  //НИЖНИЙ
int lecho= 9; 
int ltrig = 8;

#define street 3
#define vytag_yvlag 3
#define cool_hot 3
#define gaz 3
#define dogd 3
#define dvig_room 3
#define svet_room 3
#define svet_kitchen 3
#define fire 3
#define DHTPIN_1 3 //пиин датчика в комнате
#define DHTPIN_2 3 //пиин датчика на кухне
#define DHTTYPE DHT11   
DHT dht_1(DHTPIN_1, DHTTYPE);
DHT dht_2(DHTPIN_2, DHTTYPE);
void setup()
{
  window.attach(2);
  door.attach(2);
  Ethernet.begin(mac, ip);
  dht_1.begin();
  dht_2.begin();
  Serial.begin(9600);
  pinMode(kittrig, OUTPUT); 
  pinMode(kitecho, INPUT);
   pinMode(htrig, OUTPUT); 
  pinMode(hecho, INPUT);
   pinMode(ltrig, OUTPUT); 
  pinMode(lecho, INPUT);
  pinMode(fire, INPUT);
  pinMode(svet_kitchen, OUTPUT);
  pinMode(svet_room, OUTPUT);
  pinMode(dvig_room, INPUT);
  pinMode(dogd, INPUT);
  pinMode(gaz, INPUT);
  pinMode(cool_hot, OUTPUT);
  pinMode(vytag_yvlag, OUTPUT);
   pinMode(street, INPUT);
}
////////////////////////////////
void loop(){///////////////////
//////////////////////////////

////////////////////  
//ОТКРЫТИЕ ДВЕРЕЙ//
//////////////////
  //СНЯТИЕ ПОКАЗАНИЙ С ВЕРХНЕГО ДАТЧИКА
  int duration_dh, cm_dh;
digitalWrite(htrig, LOW);
delayMicroseconds(2);
digitalWrite(htrig, HIGH);
delayMicroseconds(10);
digitalWrite(htrig, LOW);
duration_dh = pulseIn(hecho, HIGH);
cm_dh = duration_dh / 58; 
  //СНЯТИЕ ПОКАЗАНИЙ СС НИЖНЕГО ДАТЧИКА
  int duration_dl, cm_dl;
digitalWrite(ltrig, LOW);
delayMicroseconds(2);
digitalWrite(ltrig, HIGH);
delayMicroseconds(10);
digitalWrite(ltrig, LOW);
duration_dl = pulseIn(lecho, HIGH);
cm_dl = duration_dl / 58; 
  //УПРАВЛЕНИЕ ДВЕРЬЮ
if (cm_dl < 9 & cm_dh < 17){
msg = "GET /objects/?op=set&object=door&p=status&v=1";
Serial.println(msg);
client.connect(server, 80);
client.println(msg);
delay (1000);
client.stop();
delay(100);
msg = "GET /objects/?op=set&object=door&p=otvet";
Serial.println(msg);
client.connect(server, 80);
client.println(msg);
while (client.available()) {
    c = client.read();
    Serial.write(c);
  }
client.stop();
if (c == 'S'){
  door.write(90);
  delay(5000);
  door.write(0);
}
if (c == 'O'){
  door.write(0);
}

delay (1000);
}
/////////////////////////////
//*******КОНЕЦ************//
///////////////////////////

///////////////////////////////////////////////////////  
// ИЗМЕРЕНИЕ РАССТОЯНИЯ НА КУХНЕ ДЛЯ ВКЛЮЧЕНИЯ СВЕТА//
/////////////////////////////////////////////////////
  int duration, cm;
digitalWrite(kittrig, LOW);
delayMicroseconds(2);
digitalWrite(kittrig, HIGH);
delayMicroseconds(10);
digitalWrite(kittrig, LOW);
duration = pulseIn(kitecho, HIGH);
cm = duration / 58; 
if (cm < 9){
  analogWrite(svet_kitchen, 1024);
  msg = "GET /objects/?op=set&object=ultraSensor01&p=sm&v=1";
Serial.println(msg);
client.connect(server, 80);
client.println(msg);
delay (1000);
client.stop();
delay (1000);
}
else {
  analogWrite(svet_kitchen, 0);
  msg = "GET /objects/?op=set&object=ultraSensor01&p=sm&v=0";
Serial.println(msg);
client.connect(server, 80);
client.println(msg);
delay (1000);
client.stop();
delay (1000);
}
Serial.println (" cm  на кухне:");
Serial.print (cm);
delay(100);
/////////////////////////////
//*******КОНЕЦ************//
///////////////////////////

///////////////////////////////////////////////////////////////////////////////
//ДАТЧИКИ ДВИЖЕНИЯ И ОСВЕЩЕННОСТИ И ДАТЧИК ДОЖДЯ - ВКЛЮЧЕНИЯ СВЕТА В КОМНАТЕ//
/////////////////////////////////////////////////////////////////////////////
int dvig = digitalRead(dvig_room);
int osvet = analogRead(street);

if(analogRead(dogd) <300){
  window.write(0);}
  
else{
  if (dvig == 1){
    if(osvet > 90){
      analogWrite(svet_room, 0);
      window.write(90);
     msg = "GET /objects/?op=set&object=rainSensor01&p=status&v=1";
    Serial.println(msg);
    client.connect(server, 80);
    client.println(msg);
    delay (1000);
    client.stop();
    delay (1000);
    msg = "GET /objects/?op=set&object=MotionSensor01&p=status&v=0";
    Serial.println(msg);
    client.connect(server, 80);
    client.println(msg);
    delay (1000);
    client.stop();
    delay (1000);
    }
    else{
      analogWrite(svet_room, 1024);
      window.write(0);
      msg = "GET /objects/?op=set&object=rainSensor01&p=status&v=0";
    Serial.println(msg);
    client.connect(server, 80);
    client.println(msg);
    delay (1000);
    client.stop();
    delay (1000);
    msg = "GET /objects/?op=set&object=MotionSensor01&p=status&v=1";
    Serial.println(msg);
    client.connect(server, 80);
    client.println(msg);
    delay (1000);
    client.stop();
    delay (1000);
    }
  }
  else{
  analogWrite(svet_room, 0);
  window.write(0);
   msg = "GET /objects/?op=set&object=rainSensor01&p=status&v=0";
    Serial.println(msg);
    client.connect(server, 80);
    client.println(msg);
    delay (1000);
    client.stop();
    delay (1000);
    msg = "GET /objects/?op=set&object=MotionSensor01&p=status&v=0";
    Serial.println(msg);
    client.connect(server, 80);
    client.println(msg);
    delay (1000);
    client.stop();
    delay (1000);}
}

/////////////////////////////
//*******КОНЕЦ************//
///////////////////////////

///////////////////////////////////////
//ДАТЧИК ГАЗА - ОТПРАВКА УВЕДОМЛЕНИЯ//
/////////////////////////////////////
int g = digitalRead(gaz);
if(g == 1){
msg = "GET /objects/?op=set&object=gazSensor1&p=status&v=1";
client.connect(server, 80);
client.println(msg);
delay (1000);
while (client.available()) {
    c = client.read();
    Serial.write(c);
  }
  Serial.println();
client.stop();
delay (1000);
}
else{
msg = "GET /objects/?op=set&object=gazSensor1&p=status&v=0";
client.connect(server, 80);
client.println(msg);
delay (1000);
while (client.available()) {
    c = client.read();
    Serial.write(c);
  }
  Serial.println();
client.stop();
delay (1000);
}

/////////////////////////////
//*******КОНЕЦ************//
///////////////////////////

/////////////////////////////////////////
//ДАТЧИК ТЕМПЕРАТУРЫ - УПРАВЛЕНИЕ РЕЛЕ//
///////////////////////////////////////
float tk = dht_1.readTemperature();
if (tk > 22.50){
  analogWrite(cool_hot, 1024);
   msg = "GET /objects/?op=set&object=TempSensor01&p=status&v=1";
    Serial.println(msg);
    client.connect(server, 80);
    client.println(msg);
    delay (1000);
    client.stop();
    delay (1000);
    }
else{
  analogWrite(cool_hot, 0);
  msg = "GET /objects/?op=set&object=TempSensor01&p=status&v=0";
    Serial.println(msg);
    client.connect(server, 80);
    client.println(msg);
    delay (1000);
    client.stop();
    delay (1000);
}

/////////////////////////////
//*******КОНЕЦ************//
///////////////////////////

///////////////////////////////////////
//ДАТЧИК ВЛАЖНОСТИ - УПРАВЛЕНИЕ РЕЛЕ//
/////////////////////////////////////
float hk = dht_1.readHumidity();
 if (hk > 50.50){
  analogWrite(vytag_yvlag, 1024);
  msg = "GET /objects/?op=set&object=HumiSensor01&p=status&v=1";
    Serial.println(msg);
    client.connect(server, 80);
    client.println(msg);
    delay (1000);
    client.stop();
    delay (1000);
}
else{
  analogWrite(vytag_yvlag, 0);
  msg = "GET /objects/?op=set&object=HumiSensor01&p=status&v=0";
    Serial.println(msg);
    client.connect(server, 80);
    client.println(msg);
    delay (1000);
    client.stop();
    delay (1000);
}

/////////////////////////////
//*******КОНЕЦ************//
///////////////////////////

//////////////////////////////////////////////////////
//ДАТЧИКИ ОГНЯ И ТЕМПЕРАТУРЫ - ОТПРАВКА УВЕДОМЛЕНИЯ//
////////////////////////////////////////////////////
  int f = analogRead(fire);
  float tf = dht_2.readTemperature();
  float hf = dht_2.readHumidity();
if(f==1 & tf > 50){
msg = "GET /objects/?op=set&object=fireSensor1&p=status&v=1";
client.connect(server, 80);
client.println(msg);
delay (1000);
while (client.available()) {
    c = client.read();
    Serial.write(c);
  }
  Serial.println();
client.stop();
delay (1000);
}
else{
msg = "GET /objects/?op=set&object=fireSensor1&p=status&v=0";
client.connect(server, 80);
client.println(msg);
delay (1000);
while (client.available()) {
    c = client.read();
    Serial.write(c);
  }
  Serial.println();
client.stop();
delay (1000);
}

/////////////////////////////
//*******КОНЕЦ************//
///////////////////////////
//вывод температуры
msg = "GET /objects/?op=set&object=TempSensor01&p=temp&v=";
Serial.println(msg);
client.connect(server, 80);
client.println(msg);
client.println(tk);
delay (1000);
client.stop();
delay (1000);
msg = "GET /objects/?op=set&object=HumiSensor01&p=humi&v=";
Serial.println(msg);
client.connect(server, 80);
client.println(msg);
client.println(hk);
delay (1000);
client.stop();
delay (1000);
  }
