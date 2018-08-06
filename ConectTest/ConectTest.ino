#include <StackList.h>
#include "timer-api.h"
StackList <int> ButList;
#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля
boolean training = false;
boolean StopTraining=false;
String message;
int reaction=0;
RF24 radio(48, 49); // "создать" модуль на пинах 9 и 10 Для Уно
int data [2];
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

byte counter;


void setup() {
  Serial.begin(9600); //открываем порт для связи с ПК
  radio.begin(); //активировать модуль
  timer_init_ISR_1Hz(TIMER_DEFAULT);
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
void timer_handle_interrupts(int timer) {
  reaction++;
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
        Serial.print("Recieved: "); Serial.println(data[0]);
      }
        if((data[0]== Knop) and (data[1]==2)){
          Out=true;
          digitalWrite(13,155);
    }
  }
  digitalWrite(3,LOW);
  radio.stopListening();
  radio.openWritingPipe(address[0]);
}

void Classik(){
  Serial.print("Go");
  while (!ButList.isEmpty ()){
    data[0]=ButList.pop();
    //data[0]=3;
    data[1]=1;
  
    while (radio.write(&data,sizeof(data))){
      radio.write(&data,sizeof(data));
    }
    Serial.print("Wait: "); Serial.println(data[0]);
    wait();
  }
}
void chatHC06(){
  if (message.equalsIgnoreCase("стоп") ){
    StopTraining = true;
    training= false;
    Serial.println("Для завершения тренеровкеи, нажмите на последнию горящию кнопку");
  }
  else if( training){
    Serial.println("Тренеровка выполняется, дождитесь завершения тренеровки.Или напишите Стоп для завершения");
  }
  else if(message.equalsIgnoreCase("Старт") ){
    Serial.print("Gog");
    training=true;
    Serial.print("Gog");
    Classik();
  }
  else{
  //if(!message.equalsIgnoreCase("Старт")){
    Serial.print("pos");
    message.trim();
    int STStr;
    int ENStr;
    boolean StrOn=false;
    int t=message.length();
    for(int i =0;i<=t;i++){
      
      if ((!message.charAt(i)==',')&&(!StrOn)){
        StrOn=true;
        STStr=i;
      }
      if (message.charAt(i)==','){
        StrOn=false;
        ENStr=i;
      }
      if (!StrOn){
        ButList.push(message.substring(STStr,ENStr).toInt());
      }
  }
  message="";
  Serial.println("Напишите старт для начала тренеровки");
 }
 Serial.print("no");
}

void loop() {
  while(Serial.available())
  {
    message+=char(Serial.read());
    delay(2);
  }
  if(!Serial.available())
  {
    if(message!="")
    {
      chatHC06();
      message="";
    }
  }
}
