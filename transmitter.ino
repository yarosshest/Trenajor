
// data[1]=2 кнопка нажата
// data[1]=1 ждать нажатие кнопки 
// data[1]=3 включить звук 

#include <SPI.h>                                          // Подключаем библиотеку для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>     // Подключаем библиотеку для работы с nRF24L01+
RF24           radio(8, 9);                              // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)
int nom= 1;                                               // номер устройства
boolean sound=false;                                              //звук
int data [2];
// Создаём массив для передачи данных

void setup(){
    pinMode(6,INPUT);
    pinMode(4,OUTPUT);
    data[0]=1;                                             // Номер устройства
    radio.begin();                                        // Инициируем работу nRF24L01+
    radio.setChannel(5);                                  // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
    radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
    radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openReadingPipe(1,0x1234567890LL);              //хотим слушать трубу 0
    //radio.openWritingPipe (0x1234567890LL);               // Открываем трубу с идентификатором 0x1234567890 для передачи данных (на ожном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
    radio.powerUp(); //начать работу
    radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
}

void wait(){
  boolean Out=false;
  while (Out= false){
    if (digitalRead(6)== HIGH){
      delay(10);
      if(digitalRead(6)== HIGH){
        data[0]=nom;
        data[1]=2;
        Out=true;
        radio.write(&data, sizeof(data));
        if(sound){
          digitalWrite(4,HIGH);
          delay(500);
          digitalWrite(4,LOW);
        }
      }
    }
  }
}




void loop(){ 
    data[0]=0;
    
    radio.read(&data, sizeof(data));                     // Отправляем данные из массива data указывая сколько байт массива мы хотим отправить

    if ((data[0]== nom) and (data[1]==1)){
      wait();
    }
    
}
