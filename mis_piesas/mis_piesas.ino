// C++ code
//
/// переделать на управление реле  по низкому уровню

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define frequency 2250 // Время между импульсами в мксек. 1000 мксек = 1 мсек = 0.001 сек. Частота следования импульсов 1/0.001 = 1 кГц,
// Не рекомендуется устанавливать время меньше 100 мсек, т.к. частота будет 10 кГц               2250 
int toStep = 8080;// 159-коэфициэнт пересчета длинны см строка 188 при диам 80мм-один оборот 251.2 мм на 1метр 3.98 оборота на вал 31 5.07---806
int dif = 45;// прокатка блистера за корпус ножа надо гдето 34мм минимум плюс 10 на откат
int dif_rev = 10; //откат от сжимаемого блистера
int largo = 0;
int rev = 5;// откат блистера для исключения зацепа при сжатии
int ps = 0;
int tiempoNumber;
int speed; // шим сигнал проверить на оганичения
const float Tested = 50.0;//  прокрутка блистера
int x = 0;
long n = 0;
volatile bool flag = false; // флаг для кнопки пин 2
const int PinLamp = 13;  // пин лампы рабочего цикла
const int Presso = A0;   // пин реза
const int Bajo = A1;     // пин на сброс изделия
const int ReaderBtn = 4; // пин слушатель кнопки прокрутки стартинг
const int Motor = 3;     // пин шагового мотора
int rapido = A2;         // пин потенциометра
// временный
//  const int PinPauseLamp = 0;// пин сигнала паузы
const int Pauses = 2;     // пин прерывания
volatile int counter = 0; // переменная-счётчик
 //#include <LCD_1602_RUS.h> //потом посмотреть русскую озвучку

LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <Keypad.h> //клавиатура
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

byte rowPin[ROWS] = {12, 11, 10, 9};
byte colPin[COLS] = {8, 7, 6};

Keypad kpd = Keypad(makeKeymap(keys), rowPin, colPin, ROWS, COLS);

const int LampFoto = A3;//проверить необходимость

void setup()
{
//TCCR2B = TCCR2B & B11111000 | B00000001; // set timer 2 divisor to 1 for PWM frequency of 31372.55 Hz
//TCCR2B = TCCR2B & B11111000 | B00000010; // set timer 2 divisor to 8 for PWM frequency of 3921.16 Hz
//TCCR2B = TCCR2B & B11111000 | B00000011; // set timer 2 divisor to 32 for PWM frequency of 980.39 Hz
//TCCR2B = TCCR2B & B11111000 | B00000100; // set timer 2 divisor to 64 for PWM frequency of 490.20 Hz (The DEFAULT)
//TCCR2B = TCCR2B & B11111000 | B00000101; // set timer 2 divisor to 128 for PWM frequency of 245.10 Hz
//TCCR2B = TCCR2B & B11111000 | B00000110; // set timer 2 divisor to 256 for PWM frequency of 122.55 Hz
//TCCR2B = TCCR2B & B11111000 | B00000111; // set timer 2 divisor to 1024 for PWM frequency of 30.64 Hz

  Serial.begin(9600); // общение на порт или пины 0 и 1
  lcd.init();
  lcd.backlight();

  

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Mis piezas");

  lcd.setCursor(0, 1); // ввод
  lcd.print("www.katalina.ru");
  delay(1000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print((String("L= ") + largo) + (String("  PS= ") + ps));
  lcd.setCursor(0, 1);
  lcd.print("numero: ");

  pinMode(PinLamp, OUTPUT);
  
  pinMode(Presso, OUTPUT);
  digitalWrite(Presso, 1);//управление реле по низкому
  pinMode(Bajo, OUTPUT);
  digitalWrite(Bajo, 1);//управление реле по низкому
  pinMode(rev, OUTPUT);
  
  pinMode(ReaderBtn, INPUT_PULLUP);
  pinMode(Motor, OUTPUT);
  pinMode(rapido, INPUT); // пин потенциометра вход
  pinMode(Pauses, INPUT_PULLUP);
  attachInterrupt(0, MiInterrupt, CHANGE);//пауза по прерыванию

  pinMode(LampFoto, OUTPUT);//проверить необходимость
  
 
  }
//**************************************************************************************/



//*************mi pause
void MiInterrupt()
{
  counter++; // + нажатие
  Serial.println(counter);
 // читаем инвертированное значение для удобства
  volatile bool btnState = (!digitalRead(Pauses));
  if (btnState && !flag)
  { // обработчик нажатия
    flag = true;
    Serial.println("press");      // временная
    delay(50);
  }
  Serial.println(flag);
  if (!btnState && flag)
  { 
    // обработчик отпускания
    flag = false;
    Serial.println("release");     // временная(String("L= ") + largo)
    Serial.println(String("Flag = ") + "release");
  }
  Serial.println(String("Flag = ") +  flag);
  digitalWrite(LampFoto, !flag); //  сигнал паузы лампа 
}




int DetectButtons() // ввод с клавиатуры
{
  x = kpd.getKey();
  if (x > 47)
  {
    x = x - 48;
    lcd.setCursor(0, 0);
    lcd.print((String("L= ") + largo) + (String("  PS= ") + ps));
    n = n * 10 + x;
    lcd.setCursor(0, 1);
    lcd.print((String("numero: ") + String(n)));
  }

  if (x == '*') // откат последней цыфры
  {
    n = n / 10;
    lcd.setCursor(0, 1);
    lcd.print("numero:                     ");
    lcd.setCursor(0, 1);
    lcd.print((String("numero: ") + String(n)));
  }
  if (x == '#') // запоминание значения
  {
    tiempoNumber = n;
    n = 0;
    lcd.setCursor(0, 1);
    lcd.print(String("numero: ") + String(n) + String("         "));
  }
  return tiempoNumber;
}








void reader()
{
  for (int i = 1; i <= ((Tested/ 1000) * toStep) ; i++)
            {
              digitalWrite(Motor    , 1);
              delayMicroseconds(1050);
              digitalWrite(Motor    , 0);
              delayMicroseconds(1050);
            }
}


void servir(int l, int n)
{
  int lec = l - dif; //для всех кроме первого
  
  {
    digitalWrite(PinLamp, 1); // начало цикла лампа загор
    // или здесь передаем
    
   for (int i = 1; i <= n; i++)

		{
    
				if (flag != 0)//проверяем паузу
				{
					delay(5000);
				}

				if (flag == 0)
				{
					speed = analogRead(rapido);
					speed = map(speed, 10, 1023, 200, 500);/// подбираю
					speed = constrain(speed, 200, 500);
					Serial.println(speed); // порт

						if (i == 1)
						{
                           l = l;
                          Serial.println("UNO!!!!!!!!!!"); // порт
                          Serial.println(i); // порт
                          Serial.println(l); // порт
                          //l = l - dif; 
                          // для первого целое, для последующих меньше на дифирент уменьшить длинну
                        }
                  else {
                    l = lec;  // для первого целое, для последующих меньше на дифирент уменьшить длинну
                    
                    Serial.println("Dos, Tres..........!!!!!!!!!!"); // порт
                    Serial.println(i); // порт
                    Serial.println(l); // порт
                  }
    	   ///*******************************здесь надо уменьшить прокрутку на 35мм
					//l = l - dif;  // для первого целое, для последующих меньше на дифирент уменьшить длинну

								///*******************************здесь надо уменьшить прокрутку на 35мм
					int k = ((float)l * toStep) / 1000;
					for (int i = 1; i <= k; i++)
					{
						digitalWrite(Motor, 1);
						delayMicroseconds(speed);
						digitalWrite(Motor, 0);
						delayMicroseconds(speed);
					}
			reducir();
					Serial.println((String("hizo:  ") + i)); // счетчик заменить переменную
					lcd.setCursor(0, 1);
					lcd.print((String("hacen: ") + String(i))); // вывод счетчика проверить
				}
			}
		
    
    
    n = 0;                    // сброс данных
    digitalWrite(PinLamp, 0); // начало цикла лампа загорается
    largo = 0;
    ps = 0;
    tiempoNumber = 0;
    lcd.setCursor(0, 0);
    lcd.print(String("L= ") + largo + String("  PS= ") + ps + String("       "));
    lcd.setCursor(0, 1);
    lcd.print("numero:                           ");
  }
}










void reducir() // режем и сбрасываем  
/// переделать на управление реле  по низкому уровню
{
  if (flag == 0)
  {
    digitalWrite(Presso, 0);  //  сигнал 1 на реле
    Serial.println("Presso"); // порт lcd.print((String("L= ") + largo) + (String("  PS= ") + ps));
   // Serial.println((String("stuco - ") + ps));        // счетчик заменить переменную неработает нет доступа к переменной стаика идет
    delay(500);
    digitalWrite(Presso, 1);
        delay(500);
    ///*******************************здесь надо добавить прокрутку на 45мм  плюс 10мм на откат
    for (int i = 1; i <= (((float)dif + dif_rev) / 1000 ) * toStep; i++)
    {
      digitalWrite(Motor    , 1);
      delayMicroseconds(speed);
      digitalWrite(Motor    , 0);
      delayMicroseconds(speed);
    }
  //Serial.println(Sting("diffirent")+((((float)dif + dif_rev) / 1000 ) * toStep; i++))); // порт
     ///*******************************здесь надо добавить прокрутку на 35мм
   
    ////*****************************здесь надо добавить откат от сжимаемого блистера
    delay(250);
    digitalWrite(rev, 1);// включаем реверс
    //делаем откат на 10 мм
     for (int i = 1; i <= ((float)dif_rev/ 1000) * toStep ; i++)
     {
      digitalWrite(Motor    , 1);
      delayMicroseconds(speed);
      digitalWrite(Motor    , 0);
      delayMicroseconds(speed);
      }
    
    //делаем откат на 10 мм
    digitalWrite(rev, 0);// выключаем реверс
    
    ////*****************************здесь надо добавить откат от сжимаемого блистера
    
    digitalWrite(Bajo, 0);  // сигнал 1 на реле
    Serial.println("Bajo"); // порт
    delay(500);
    digitalWrite(Bajo, 1);
  }
}









//***************************************************************************************
void loop()
{


    if (digitalRead(ReaderBtn) == 0)
  {
    reader();
    }
  if (largo == 0)
  {
    largo = DetectButtons(); // назначаем длинну из функции
  }
  lcd.setCursor(0, 0);
  lcd.print((String("L= ") + largo) + (String("  PS= ") + ps));

  if (largo > 0 && ps == 0)
  {
    tiempoNumber = 0;
    ps = DetectButtons(); // получаем количество
  }
  if (largo > 0 && ps > 0)
  {
    lcd.setCursor(0, 0);
    lcd.print((String("L= ") + largo) + (String("  PS= ") + ps));
    lcd.setCursor(0, 1);
    lcd.print("hacen:    listo       ");
    
      Serial.println(String("trabajo = ") + largo + String("mm.   ") + ps + String("ps."));     // здесь передаемв ком порт

      int arr[3] = {0, largo, ps};
      {
      for (byte i = 0; i < 3; i++) 
        {
          if(i<2) {
          Serial.print(arr[i]);
          Serial.print(',');
          }
          else{
           Serial.print(arr[i]);
          }
        }     // здесь передаемв ком пор для записи
        Serial.println();
      }
    servir(largo, ps);
  }
  
  

}

/////////////////////////////////////////////////////////////////////





    