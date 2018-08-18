#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(8,9); // "создать" модуль на пинах 9 и 10 Для Уно
int nom= 3;                                               // номер устройства
boolean sound=false;                                              //звук
int data [2];

byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  //возможные номера труб

void setup(){
  //Serial.begin(57600); //открываем порт для связи с ПК
  pinMode(3,OUTPUT);
  pinMode(7,INPUT);
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0,15);     //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(sizeof(data));     //размер пакета, в байтах

  radio.openReadingPipe(1,address[0]);      //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  
  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
}

void wait(){
  radio.stopListening();
  radio.openWritingPipe(address[0]);
  data[0]=nom;
  data[1]=2;
  boolean Out=false;
  digitalWrite(3,HIGH);
  while (Out== false){
    if (digitalRead(7)== HIGH){
      delay(50);
      if(digitalRead(7)== HIGH){
        digitalWrite(3,LOW);
       // Serial.println("Nachato");
        Out=true;
        while (radio.write(&data,sizeof(data))){
          radio.write(&data, sizeof(data));
        }
      }
    }
  }
  radio.startListening();
  radio.openReadingPipe(1,address[0]);
}

void loop() {
    byte pipeNo;                  
    while( radio.available(&pipeNo)){    // слушаем эфир со всех труб
      radio.read( &data, sizeof(data) );         // чиатем входящий сигнал

    //  Serial.print("Recieved: ");Serial.println(data[0]); Serial.println(data[1]);
   }
   if (data[0]== nom){
      data[0]=nom;
      data[1]=0;
      radio.stopListening();
      radio.openWritingPipe(address[0]);
      while (radio.write(&data,sizeof(data))){
        radio.write(&data, sizeof(data));
      }
      radio.startListening();
      radio.openReadingPipe(1,address[0]);
      delay(100);
     // Serial.print("Prov: ");
      wait();
      digitalWrite(3,LOW);
    //  Serial.println("OK");
      data[0]=0;
   }

   
}
