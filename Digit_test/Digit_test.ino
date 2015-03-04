#include <dht11.h>

dht11 DHT11;

#define DHT11PIN A4

#define DIGITPIN6 A3
#define DIGITPIN8 A0
#define DIGITPIN9 A1
#define DIGITPIN12 A2

#define DIGITPIN1 5
#define DIGITPIN2 6
#define DIGITPIN3 7
#define DIGITPIN4 8
#define DIGITPIN5 9
#define DIGITPIN7 4
#define DIGITPIN10 3
#define DIGITPIN11 2

#define TIMER_CLOCK_FREQ 2000000.0 //2MHz for /8 prescale from 16MHz

int counter = 0;
int chk = 0;
int lastTemp = 0;
int timerLoadValue;
int currentReg=0;
int lock=0;

unsigned char SetupTimer2(float timeoutFrequency){
  unsigned char result; //Начальное значение таймера.
   
  //Подсчет начального значения таймера
  result=(int)((257.0-(TIMER_CLOCK_FREQ/timeoutFrequency))+0.5);
  //257 на самом деле должно быть 256, но я получил лучшие результаты с 257.
   
  //Установки Таймер2: Делитель частоты /8, режим 0
  //Частота = 16MHz/8 = 2Mhz или 0.5 мкс
  //Делитель /8 дает нам хороший рабочий диапазон
  //так что сейчас мы просто жестко запрограммируем это.
  TCCR2A = 0;
  TCCR2B = 0<<CS22 | 1<<CS21 | 0<<CS20;
   
  //Подключение прерывания по переполнению Timer2
  TIMSK2 = 1<<TOIE2;
   
  //загружает таймер для первого цикла
  TCNT2=result;
     //Serial.println("Timer started");
  return(result);
}

ISR(TIMER2_OVF_vect) {
  if (lock == 0){
    //Переключение IO-вывода в другое состояние.
    //digitalWrite(13,!digitalRead(13));
    //printTemp(lastTemp);
    //Serial.println("T");
    //Захват текущего значения таймера. Это величина ошибки
    //из-за задержки обработки прерывания и работы этой функции
    //latency=TCNT2;
    
    //Перезагрузка таймера и коррекция по задержке
    //TCNT2=latency+timerLoadValue;
    reset();
    currentReg++;
    if (currentReg > 4){
      currentReg = 1;
    }
    switch (currentReg){
      case 4:
        printDigit(4, lastTemp / 10);
        //printDigit(4, 4);
        break;
     case 3:
       printDigit(3, lastTemp % 10);
       break;
     case 2:
       printGrad();
       break;
     case 1:
       printC();
       break;
    }
    //printDigit(currentReg,currentReg);
    
    TCNT2=TCNT2+timerLoadValue;
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("DIGIT LED TEST PROGRAM ");
  Serial.println();
  
  //pinMode(13,OUTPUT);
  
  //timerLoadValue=SetupTimer2(44100);
  timerLoadValue=SetupTimer2(5);
  
  pinMode(DIGITPIN1, OUTPUT);
  pinMode(DIGITPIN2, OUTPUT);
  pinMode(DIGITPIN3, OUTPUT);
  pinMode(DIGITPIN4, OUTPUT);
  pinMode(DIGITPIN5, OUTPUT);
  pinMode(DIGITPIN6, OUTPUT);
  pinMode(DIGITPIN7, OUTPUT);
  pinMode(DIGITPIN8, OUTPUT);
  pinMode(DIGITPIN9, OUTPUT);
  pinMode(DIGITPIN10, OUTPUT);
  pinMode(DIGITPIN11, OUTPUT);
  pinMode(DIGITPIN12, OUTPUT);
}

void loop()
{
  //counter++;
  //if (counter > 500){
    //counter = 0;
    Serial.println("In loop: trying to read value");
    lock = 1;
    //chk = DHT11.read(DHT11PIN);
    //lastTemp = (int)DHT11.temperature;
    lastTemp++;
    if (lastTemp > 99){
      lastTemp=0;
    }
    lock = 0;
    Serial.println(lastTemp);
  //}
  //printTemp(lastTemp);
  delay(2000);
  
  //printTemp(lastTemp);
  //delay(5);
  
  //printTemp(26);
//  delay(1000);
}

void reset(){
  digitalWrite(DIGITPIN6, LOW);
  digitalWrite(DIGITPIN8, LOW);
  digitalWrite(DIGITPIN9, LOW);
  digitalWrite(DIGITPIN12, LOW);
  //digitalWrite(DIGITPIN6, HIGH);
  //digitalWrite(DIGITPIN8, HIGH);
  //digitalWrite(DIGITPIN9, HIGH);
  //digitalWrite(DIGITPIN12, HIGH);
  digitalWrite(DIGITPIN1, HIGH);
  digitalWrite(DIGITPIN2, HIGH);
  digitalWrite(DIGITPIN3, HIGH);
  digitalWrite(DIGITPIN4, HIGH);
  digitalWrite(DIGITPIN5, HIGH);
  digitalWrite(DIGITPIN7, HIGH);
  digitalWrite(DIGITPIN10, HIGH);
  digitalWrite(DIGITPIN11, HIGH);
}

void printDigit(int reg, int digit){
  //Serial.println("DIGIT Printed");
  //reset(); 
  switch(reg){
    case 1:
      digitalWrite(DIGITPIN6, HIGH);
      //digitalWrite(DIGITPIN6, LOW);
    break;
    case 2:
      digitalWrite(DIGITPIN8, HIGH);
      //digitalWrite(DIGITPIN8, LOW);
    break;
    case 3:
      digitalWrite(DIGITPIN9, HIGH);
      //digitalWrite(DIGITPIN9, LOW);
    break;
    case 4:
      digitalWrite(DIGITPIN12, HIGH);
      //digitalWrite(DIGITPIN12, LOW);
    break;
  }
  switch(digit){
    case 0:
      digitalWrite(DIGITPIN1, LOW);
      digitalWrite(DIGITPIN2, LOW);
      digitalWrite(DIGITPIN4, LOW);
      digitalWrite(DIGITPIN7, LOW);
      digitalWrite(DIGITPIN10, LOW);
      digitalWrite(DIGITPIN11, LOW);
    break;
    case 1:
      digitalWrite(DIGITPIN4, LOW);
      digitalWrite(DIGITPIN7, LOW);
    break;
    case 2:
      digitalWrite(DIGITPIN1, LOW);
      digitalWrite(DIGITPIN2, LOW);
      digitalWrite(DIGITPIN5, LOW);
      digitalWrite(DIGITPIN7, LOW);
      digitalWrite(DIGITPIN11, LOW);
    break;
    case 3:
      digitalWrite(DIGITPIN2, LOW);
      digitalWrite(DIGITPIN4, LOW);
      digitalWrite(DIGITPIN5, LOW);
      digitalWrite(DIGITPIN7, LOW);
      digitalWrite(DIGITPIN11, LOW);
    break;
    case 4:
      digitalWrite(DIGITPIN4, LOW);
      digitalWrite(DIGITPIN5, LOW);
      digitalWrite(DIGITPIN7, LOW);
      digitalWrite(DIGITPIN10, LOW);
    break;
    case 5:
      digitalWrite(DIGITPIN2, LOW);
      digitalWrite(DIGITPIN4, LOW);
      digitalWrite(DIGITPIN5, LOW);
      digitalWrite(DIGITPIN10, LOW);
      digitalWrite(DIGITPIN11, LOW);
    break;
    case 6:
      digitalWrite(DIGITPIN1, LOW);
      digitalWrite(DIGITPIN2, LOW);
      digitalWrite(DIGITPIN4, LOW);
      digitalWrite(DIGITPIN5, LOW);
      digitalWrite(DIGITPIN10, LOW);
      digitalWrite(DIGITPIN11, LOW);
    break;
    case 7:
      digitalWrite(DIGITPIN4, LOW);
      digitalWrite(DIGITPIN7, LOW);
      digitalWrite(DIGITPIN11, LOW);
    break;
    case 8:
      digitalWrite(DIGITPIN1, LOW);
      digitalWrite(DIGITPIN2, LOW);
      digitalWrite(DIGITPIN4, LOW);
      digitalWrite(DIGITPIN5, LOW);
      digitalWrite(DIGITPIN7, LOW);
      digitalWrite(DIGITPIN10, LOW);
      digitalWrite(DIGITPIN11, LOW);
    break;
    case 9:
      digitalWrite(DIGITPIN2, LOW);
      digitalWrite(DIGITPIN4, LOW);
      digitalWrite(DIGITPIN5, LOW);
      digitalWrite(DIGITPIN7, LOW);
      digitalWrite(DIGITPIN10, LOW);
      digitalWrite(DIGITPIN11, LOW);
    break;
  }
  //delay(1);
}

void printGrad(){
  //grad
  digitalWrite(DIGITPIN8, HIGH);
  digitalWrite(DIGITPIN11, LOW);
  digitalWrite(DIGITPIN10, LOW);
  digitalWrite(DIGITPIN5, LOW);
  digitalWrite(DIGITPIN7, LOW);
  //delay(1);
}
void printC(){
  // C
  digitalWrite(DIGITPIN6, HIGH);
  digitalWrite(DIGITPIN11, LOW);
  digitalWrite(DIGITPIN10, LOW);
  digitalWrite(DIGITPIN1, LOW);
  digitalWrite(DIGITPIN2, LOW);
  //delay(1);
}

//
// END OF FILE
//



