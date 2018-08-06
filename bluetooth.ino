
#include <StackList.h>
//#include "timer-api.h"
StackList <int> ButList;
boolean training = false;
boolean StopTraining=false;
String message;
int reaction=0;
int actionBut;
void setup() {
  Serial.begin(9600);
  //timer_init_ISR_1Hz(TIMER_DEFAULT);
}
//void timer_handle_interrupts(int timer) {
//  reaction++;
//}
void pushBut(){
  char gameBut;
  while (!ButList.isEmpty ()){
    actionBut=ButList.pop();
    reaction=0;
    Serial.print("Нажмите кнопку: ");
    Serial.println(actionBut);
    gameBut= Serial.read();
    while (actionBut!=gameBut){
      gameBut= Serial.read();
    }
    Serial.println("Нужная кнопка нажата");
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
    training=true;
    pushBut();
  }
  else{
  //if(!message.equalsIgnoreCase("Старт")){
    Serial.println("Нало записи последовательности");
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
  Serial.println("Конец записи последовательности.Напишите старт для начала тренеровки");
 }
}

void loop() {
  while(Serial.available())
  {//while there is data available on the serial monitor
    message+=char(Serial.read());//store string from serial command
    delay(2);
  }
  if(!Serial.available())
  {
    if(message!="")
    {//if data is available
      Serial.println(message.equalsIgnoreCase("Старт")); //show the data
      chatHC06();
      message=""; //clear the data
    }
  }
}
