const int magneto = 1;  // колисество сигналов от вала для сдвига flagProgreso
const int Kit=10;  //перевод в метры должно быть 1000

 long contFin = 1000;  //                                                     значение временное, нужно брать из памяти
// testo  Trabajo counterTick fin


long dataSt;  ///значение которое надо ввести
long fiasco = 0;  // значение которое надо вывести
  //long fin = 0;
long finSt=0;

int address = 10;
long lento;
const long St = 387096;  //пробное дл

//коэфициэнтпересчета при счетчике на 600- 6.45161
//коэфициэнтпересчета при счетчике на 200- 2.150537
//коэфициэнтпересчета при счетчике на 360- 3.8709677
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <EEPROM.h>
int cn = 1;
volatile uint32_t debounce;
/*
RISING (рост) - срабатывает при изменении сигнала с LOW на HIGH
FALLING (падение) - срабатывает при изменении сигнала с HIGH на LOW
CHANGE (изменение) - срабатывает при изменении сигнала (с LOW на HIGH и наоборот)
LOW (низкий) - срабатывает постоянно при сигнале LOW
(не поддерживается на ESP8266)
*/


//long fin =EEPROM.get(address, lento);
#define TicksA 3    // пин энкодера TickA
#define TicksB 10   // пин энкодера
#define ENC_TYPE 1  // тип энкодера, 0 или 1

volatile boolean states0, lastStates, turnsFlag;
volatile long encsCounter;

int Ubutton = 11;
bool flagUbutton = false;
uint32_t btnTimer = 0;  /// на месте


//счетчик 2
#define TickA 2     // пин счетчик А
#define TickB 5     // пин счетчик B
#define ENC_TYPE 1  // тип энкодера, 0 или 1
volatile long encCounter;
volatile long  counterSt;
volatile boolean state0, lastState, turnFlag;

/* сдвиг после считывания оборота вала со шпулейBtnPausa

struct Data {
  
  int largo = 15;
 
};
// глобальный экземпляр для личного использования
Data data;
long timpoPas = 0;      // временная
int ps = 0;             // временная
long largos = 0;        // временная
long x = 0;             // нажатая цифра
long n = 0;             // число на вводе
long tiempoNumber = 0;  // наборная из клавиатуры
*/

int progreso = 45;  /// сдвиг на 4,5мм
// Логический флаг для сдвига
bool flagProgreso = 0;


/*
сдвиг после считывания оборота вала со шпулей

Тлп = Sшд*Fшд/Pр*Nшк

где:

Тлп - точность линейного перемещения, шаг/мм
Sшд — количество шагов на оборот для двигателя (в наших примерах 200)
Fшд — микрошаг (1, 2, 4, 8 и т. д.)
Pр — шаг ремня (например, 2 мм)
Nшк — количество зубьев на шкиве, на валу двигателя.
Попробуем посчитать для примера с такими параметрами, двигатель 200 шагов на оборот,
с 2-кратным микрошагом, 2-миллиметровыми ремнями GT2 и шкивом с 20 зубцами даст нам 10 шагов на мм.

200*2
/2*20=10 шагов/мм.
4,5 мм это 45 шагов
полный ход это 45/2 = 22,5


*/
const int BtnPausa=12;  ///Pausa
/////////////////////////////////////////////////////////////////////const int BtnPausa=12;  ///Pausa

const long stepMotoApilador = 9;  // Steppin мотор укладчика

const int dereccion = 7;  // DirPin направление мотор укладчика

const int EnablePin = 8;  // EnablePin   остановка мотора
#define frequency 2250    // Время между импульсами в мксек. 1000 мксек = 1 мсек = 0.001 сек. Частота следования импульсов 1/0.001 = 1 кГц,
// Не рекомендуется устанавливать время меньше 100 мсек, т.к. частота будет 10 кГц

// Логический флаг для рабочего режима
bool flagTrabajadora = 0;

const int disposicion = A1;  // конечное положение
const int izcuierda = A2;    // левая точка
const int derecha = A3;      // правая точка

LiquidCrystal_I2C lcd(0x27, 16, 2);  // экземп4ляр жк экрана

bool puntoIzcuierdaFlag = false;  // флаг левого положения
bool puntoDerechaFlag = false;    // флаг правого положения
//uint32_t btnTimer = 0;            // таймер кнопок
//**************///**************///*****************

volatile int counterTick = 0;       // переменная-счётчик
volatile bool intFlagTick = false;  // флаг!!!!!!!!!!!!!!!!!!!!!
bool intFlagTick2 = false;          // флаг!!!!!!!!!!!!!!!!!!!!!


volatile int counter = 0;    // переменная-счётчик

volatile bool intFlag = false;  // флаг!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int houser = 0;                 // сделать память
bool status;
// const int PinLamp = 5;      // заменить на цикл укладчика
const int recalculo = A0;    // кнопка пересчета длинны
bool recalculoFlag = false;  // флаг
const int moto = 6;          // motor rele

const int SensorPin = 4;  // пин датчика катушки
bool intFlagSensor = false;
long counterSensor = 0;

void setup() {
  pinMode(13, OUTPUT);

  /* EEPROM.get(0, data); // прочитать из адреса 0
                              //  меняем
         data.largo = 12;
         data.fvalue = 3.14;
         EEPROM.put(0, data); // поместить в EEPROM по адресу 0
     */
  Serial.begin(9600);  // открыли порт для связи
                       // Keyboard.begin(); // открыли клавиатуры
  lcd.init();

  lcd.backlight();

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(String("BOBINADORA"));
  delay(1000);
  lcd.setCursor(0, 1);  // ввод
  lcd.print("www.katalina.ru");
  delay(2000);

 /////////////////////////////////////////////////////////////////////////pinMode(BtnPausa, INPUT_PULLUP);    //кнопка пауза
  //pinMode(pinProgreso, INPUT_PULLUP); // кнопка оборота шпинделя
  pinMode(disposicion, INPUT_PULLUP);  // кнопка дома
  pinMode(EnablePin, OUTPUT);          // stop  уладчик
  digitalWrite(EnablePin, HIGH);       // состояние выключен мотор чтобы не грелся
  pinMode(dereccion, OUTPUT);          // направление уладчик
  digitalWrite(dereccion, 0);          // состояние
  pinMode(stepMotoApilador, OUTPUT);   // уладчик
  digitalWrite(stepMotoApilador, 0);   // состояние

  pinMode(izcuierda, INPUT_PULLUP);  // точка лево
  pinMode(derecha, INPUT_PULLUP);    // точка право

  pinMode(moto, OUTPUT);  // мотор намотки реле
  digitalWrite(moto, 1);  // выход надо проверить + или -

  pinMode(recalculo, INPUT_PULLUP);  // пин кнопки входа в память
  // подключили кнопку на D2 и GND

  // pinMode(PinLamp, OUTPUT);
  digitalWrite(moto, 1);
  //pinMode(testDiod, OUTPUT);  /////////////////////////////
  pinMode(TicksA, INPUT_PULLUP);

  pinMode(SensorPin, INPUT_PULLUP);  // Установим вывод 4 как вход
  //pinMode(rotor, INPUT_PULLUP);
  pinMode(TicksB, INPUT_PULLUP);

  //счетчик 2
  pinMode(TickA, INPUT_PULLUP);
  pinMode(TickB, INPUT_PULLUP);

  //  pinMode(2, INPUT);
  // FALLING - при нажатии на кнопку будет сигнал 0, его и ловим
  // attachInterrupt(0, btnIsr, FALLING);
  // digitalWrite(PinLamp, 1); //  сигнал 0 на реле
  // attachInterrupt(0, buttonTick, RISING); //+
  // attachInterrupt(0, buttonTick, FALLING);//+

  // attachInterrupt(1, buttonTick2, FALLING);  //++
  // attachInterrupt(0, buttonTick, LOW);

  //attachInterrupt(0, int0, CHANGE);// при использовании счетчика
  attachInterrupt(0, buttonTickSt, CHANGE);  //++
    counterSt =0;
  attachInterrupt(1, ints0, CHANGE);
  pinMode(Ubutton, INPUT_PULLUP);
  encsCounter = EEPROM.get(address, lento);
  pinMode (BtnPausa, INPUT_PULLUP);
  // attachInterrupt(0, buttonTick, LOW);
  lcd.clear();
}
//***счетчик**************************************************************

void ints0() {
  states0 = digitalRead(TicksA);
  if (states0 != lastStates) {
#if (ENC_TYPE == 1)
    turnsFlag = !turnsFlag;
    if (turnsFlag)
      encsCounter += (digitalRead(TicksB) != lastStates) ? -1 : 1;
#else
    encsCounter += (digitalRead(TicksB) != lastStates) ? -1 : 1;
#endif
    lastStates = states0;
  }
}

//****************************************************************

///--------------------------
/*void MiEeprom(long a) {
  EEPROM.get(10, data);  // прочитать из адреса 0
    data.largo = a;
  EEPROM.put(10, data);  // поместить в EEPROM по адресу 0
}
*/
void Recalculo() {
  bool btnState = !digitalRead(recalculo);
  if (btnState && !recalculoFlag) {  // обработчик нажатия
    recalculoFlag = true;
    Serial.println("press");
    counter = 0;                  /////////////////////////////////------------------
    counterTick = counterSt = 0;  ////////////////????????????????777777

    lcd.clear();
    lcd.setCursor(0, 0);



    
    lcd.print(String(counterSt) + " mm          ");
    delay(2000);
    Serial.println(String("Trabajo "));
    // обнуление счетчика

    digitalWrite(moto, 0);
   
    flagTrabajadora = 1;

    lcd.clear();
  }
  if (!btnState && recalculoFlag) {  // обработчик отпускания
    recalculoFlag = false;
    ///////выйти в перезагрузку
  }
  
}

void buttonTickSt()  // сработка от прерывания енкодер учета
{
  {
    state0 = digitalRead(TickA);
    if (state0 != lastState) {
#if (ENC_TYPE == 1)
      turnFlag = !turnFlag;
      if (turnFlag)
        encCounter += (digitalRead(TickB) != lastState) ? -1 : 1;
#else
      encCounter += (digitalRead(TickB) != lastState) ? -1 : 1;
#endif
      lastState = state0;
    }
  }



  counterSt = (encCounter*100000)/St;
 //counterSt = encCounter;
  if (counterSt < 0) {
    counterSt = counterSt * (-1);
  }
  //contFin////////////////////////////////////////здесь коефициент Kit перевода длинны мм в метры пка 10 
  //finSt
  //if (counterSt > finSt) {
     if (counterSt > fiasco*Kit){

    Serial.println("Fin!!!!!!!!!!!!!!!");

    digitalWrite(moto, 1);  // отключаем реле мотора и останавливае укладчик
    // digitalWrite(stepMotoApilador, 0); // стоп мотор
    flagTrabajadora = 0;
   // digitalWrite(dereccion, 0);                                                    // сбрасываем направление

    Serial.println("Fin");

    //counter = 0;
    encCounter = 0;
    counterSt = 0;
  }
}

void Derecha() {

  // читаем инвертированное значение для удобства
  bool btnState1 = !digitalRead(derecha);
  if (btnState1 && !puntoDerechaFlag && millis() - btnTimer > 100) {
    puntoDerechaFlag = true;
    btnTimer = millis();
    Serial.println("press puntoDerechaFlag");

    // digitalWrite(EnablePin, HIGH);  //  выключаем мотор
    // delay(50);
    digitalWrite(dereccion, 1);  // мотор меняем направление
                                 // digitalWrite(EnablePin, LOW);  //  включаем мотор
  }
  if (!btnState1 && puntoDerechaFlag) {
    puntoDerechaFlag = false;
    btnTimer = millis();
    // Serial.println("release");
  }
  ///
  /****************/  ///
  bool btnState2 = !digitalRead(izcuierda);
  if (btnState2 && !puntoIzcuierdaFlag && millis() - btnTimer > 100) {
    puntoIzcuierdaFlag = true;
    btnTimer = millis();
    Serial.println("press puntoIzcuierdaFlag");

    // digitalWrite(EnablePin, HIGH);  //  выключаем мотор
    delay(50);
    digitalWrite(dereccion, 0);  // мотор меняем направление
    //digitalWrite(EnablePin, LOW);  //  включаем мотор
  }
  if (!btnState2 && puntoIzcuierdaFlag && millis() - btnTimer > 100) {
    puntoIzcuierdaFlag = false;
    btnTimer = millis();
    // Serial.println("release");
  }
}

///// после сработки датчика шпули запустить работу
void Trabajadora() {
  if (flagTrabajadora && flagProgreso) {
//Serial.println("to es !!!!!!!!!!!!!!!!!!sdvig");
//lcd.setCursor(9, 0);
  //  lcd.print(String(counterSensor));
    digitalWrite(EnablePin, LOW);         //  включаем мотор
    for (int n = 0; n < progreso; n++)  ////// ghjmmm
    {
      digitalWrite(stepMotoApilador, HIGH);  // мотор
      delayMicroseconds(frequency);
      digitalWrite(stepMotoApilador, LOW);  // стоп мотор
    }
    digitalWrite(EnablePin, HIGH);  //  вsключаем мотор
    flagProgreso = false;
    // flagTrabajadora =0;
  } else {
    digitalWrite(stepMotoApilador, LOW);  // стоп мотор
    digitalWrite(EnablePin, HIGH);  //  выключаем мотор
    
  }
  
}

void Disposicion() {
  bool btnStateDisposicion = !digitalRead(disposicion);
  if (btnStateDisposicion && digitalRead(izcuierda) && millis() - btnTimer > 500) {
    // puntoDerechaFlag = true;
    btnTimer = millis();
    Serial.println("press Disposicion");

    // digitalWrite(EnablePin, HIGH);  //  выключаем мотор
    //  delay(50);
    digitalWrite(dereccion, 1);    // мотор меняем направление
    digitalWrite(EnablePin, LOW);  //  включаем мотор
    // переделать условия под концевик
    while (!digitalRead(disposicion) && digitalRead(izcuierda)) {
      digitalWrite(stepMotoApilador, HIGH);  // мотор
      delayMicroseconds(frequency);
      digitalWrite(stepMotoApilador, LOW);  // стоп мотор
    }
    digitalWrite(EnablePin, HIGH);  //  выключаем мотор
  }
  if (!btnStateDisposicion && puntoIzcuierdaFlag && millis() - btnTimer > 100) {
    puntoDerechaFlag = false;
    btnTimer = millis();
    // Serial.println("release");
  }
}


//*************************************************************
void loop() {
  {


    //fin = 10000;

     EEPROM.get(address, fiasco);
    //finSt=fin;
    //
    // Serial.print(String("coco - "));
    //Serial.println(String(lento));
    //

    // lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print( String(counterSt)+"mm   de         ");

    lcd.setCursor(0, 1);
    //int asco = EEPROM.get(address, lento);
    lcd.print(String(encsCounter) + "     " + String(fiasco)+"       ");
    //   Serial.println(encsCounter);
    delay(300);
  }

  /* if (counter > 0) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print(String(counter));
    // delay(100);
  }
*/
  ////--------------------*******
  Derecha();
  Recalculo();
  //Trabajadora();
  Disposicion();
  //toSdvig();
 btnPausa();//
 ///************************************ ****
{
   bool motionDetected = digitalRead(SensorPin);  // Считываем показания
    int ciu=digitalRead(SensorPin);
    
   motionDetected = !motionDetected;
  if (motionDetected && !intFlagSensor) {
    intFlagSensor = true;
    
    counterSensor++; 
    //Serial.println(String(ciu) + " "+ String(counterSensor) );
  Serial.println("to es Pausa");  //  включаем мотор
    //Serial.print("moto\t");
    //Serial.println(String(counterSensor));
    if (counterSensor >= magneto && flagTrabajadora) {
     Serial.println("to es sdvig");  //  включаем мотор

  //lcd.setCursor(9, 0);
    //lcd.print(String(counterSensor));
    digitalWrite(EnablePin, LOW);         //  включаем мотор
    for (int n = 0; n < progreso; n++)  ////// ghjmmm
    {
      digitalWrite(stepMotoApilador, HIGH);  // мотор
      delayMicroseconds(frequency);
      digitalWrite(stepMotoApilador, LOW);  // стоп мотор
    }
    digitalWrite(EnablePin, HIGH);  //  вsключаем мотор
    Serial.println("to es sdvig 2");  //  включаем мотор

   // flagProgreso = false;
    // flagTrabajadora =0;
     
        //   flagProgreso = true;
      counterSensor = 0;
     // Serial.println("magneto");
    }
  
  }
  if (!motionDetected && intFlagSensor) {

    intFlagSensor = false;
  }
}
   ///***********************************************
  encsCounter = constrain(encsCounter, 0, 600);
  // int coco=map(encCounter,-300,300,0, 600);  ///длинна
  // coco= constrain(coco,0,600);
  // Serial.println("coco - "+String(coco));

  // Serial.println("encsCounter = " + String(encsCounter));
  // delay(300);
  /*int beta=(EEPROM.get(address, lento));
  Serial.println(String(beta));
  delay(600);
  Serial.println("*EEPROM = " + String(EEPROM.get(address, lento)));
  delay(600);
*/
  bool btnsState = !digitalRead(Ubutton);
  if (btnsState && !flagUbutton && millis() - btnTimer > 100) {
    flagUbutton = true;
    btnTimer = millis();
    // Serial.println("press");
  }
  if (btnsState && flagUbutton && millis() - btnTimer > 500) {
    btnTimer = millis();

    ////////////////////////////////////////////////умножить на 1000 получим ммльлиметры
   // long tiempoST=encsCounter*1000;
    /////////////////////////////////////////////////////////////////////////////////////////////EEPROM.put(address, encsCounter);
    EEPROM.put(address, encsCounter);
    Serial.println("   encsCounter!!! " + String(encsCounter));
    Serial.println("press hold \n");
   


  }
  if (!btnsState && flagUbutton && millis() - btnTimer > 500) {
    flagUbutton = false;
    btnTimer = millis();
    // Serial.println("release");
  }
  ///********************
}


void toSdvig()
{
   bool motionDetected = digitalRead(SensorPin);  // Считываем показания
    int ciu=digitalRead(SensorPin);
    
   motionDetected = !motionDetected;
  if (motionDetected && !intFlagSensor) {
    intFlagSensor = true;
    
    counterSensor++; 
    Serial.println(String(ciu) + " "+ String(counterSensor) );
  Serial.println("to es Pausa");  //  включаем мотор
    //Serial.print("moto\t");
    //Serial.println(String(counterSensor));
    if (counterSensor >= magneto && flagTrabajadora) {
     Serial.println("to es sdvig");  //  включаем мотор

  lcd.setCursor(9, 0);
    lcd.print(String(counterSensor));
    digitalWrite(EnablePin, LOW);         //  включаем мотор
    for (int n = 0; n < progreso; n++)  ////// ghjmmm
    {
      digitalWrite(stepMotoApilador, HIGH);  // мотор
      delayMicroseconds(frequency);
      digitalWrite(stepMotoApilador, LOW);  // стоп мотор
    }
    digitalWrite(EnablePin, HIGH);  //  вsключаем мотор
    Serial.println("to es sdvig 2");  //  включаем мотор

   // flagProgreso = false;
    // flagTrabajadora =0;
     
        //   flagProgreso = true;
      counterSensor = 0;
     // Serial.println("magneto");
    }
  
  }
  if (!motionDetected && intFlagSensor) {

    intFlagSensor = false;
  }
}


void btnPausa()
{
  bool flagStatePausa= false;
  bool StateBtnPausa= !digitalRead(BtnPausa);
  if(StateBtnPausa && !flagStatePausa)
  {
    flagStatePausa=true;
    //delay(10000);
    Serial.println("Pausa");  //  выключаем мотор
    digitalWrite(moto, 1);  // отключаем реле мотора и останавливае укладчик
    // digitalWrite(stepMotoApilador, 0); // 
    
  }
  if(!StateBtnPausa && !flagStatePausa && flagTrabajadora)
  {
    flagStatePausa=false;
      digitalWrite(moto, 0);  // отключаем реле мотора и останавливае укладчик
    // digitalWrite(stepMotoApilador, 0); // 
  }
}
