//#include "timer-api.h"
#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля
RF24 radio(8, 9); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги
int data [2];
boolean NoOtv;
int TimeOtv;
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

byte counter;


void setup() {
  randomSeed(analogRead(2));
  Serial.begin(57600); //открываем порт для связи с ПК
  //timer_init_ISR_1Hz(TIMER_DEFAULT);
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
}
//void timer_handle_interrupts(int timer) {
//  if (NoOtv){
//    TimeOtv=TimeOtv+1;
//    if (TimeOtv==2){
//      TimeOtv=0;
//      Serial.print("P");
//      radio.write(&data,sizeof(data));
//      Serial.print("K");
//    }
//}
//}

void Prov(){
  radio.startListening();
  radio.openReadingPipe(1,address[0]);
  int Knop=data[0];
  boolean Out=false;
  NoOtv=true;
  while (Out== false){
      byte pipeNo;                  
      while( radio.available(&pipeNo)){    // слушаем эфир со всех труб
        radio.read( &data, sizeof(data) );         // чиатем входящий сигнал
      }
      //Serial.print("OTVET: "); Serial.println(data[1]);
      if((data[0]== Knop) and (data[1]==0)){
        data[1]=-1;
        Out=true;
       }
  }
  NoOtv=false;
  TimeOtv=0;
  radio.stopListening();
  radio.openWritingPipe(address[0]);
}

void wait(){
  radio.startListening();
  radio.openReadingPipe(1,address[0]);
  int Knop=data[0];
  boolean Out=false;
  while (Out== false){
      byte pipeNo;                  
      while( radio.available(&pipeNo)){    // слушаем эфир со всех труб
        radio.read( &data, sizeof(data) );         // чиатем входящий сигнал
        Serial.print("Recieved: "); Serial.println(data[1]);
      }
        if((data[0]== Knop) and (data[1]==2)){
          data[1]=-1;
          Out=true;
    }
  }
  radio.stopListening();
  radio.openWritingPipe(address[0]);
}

void Classik(){
  delay(1000);
  data[0]=random(1,6);
  if (data[0]==3){
    data[0]=random(1,3);
  }
  //data[0]=3;
  data[1]=0;
  while (radio.write(&data,sizeof(data))){
    radio.write(&data,sizeof(data));
  }
  Serial.print("Prov: ");
  Prov();
  Serial.println("OK");
  data[1]=1;
  Serial.print("Wait: "); Serial.println(data[0]);
  wait();
}

void loop() {
  
  Classik();
}
