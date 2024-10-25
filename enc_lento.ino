const int magneto = 2;  // колисество сигналов от вала для сдвига

// testo  Trabajo counterTick fin

const double St= 3.8709677;//пробное дл

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
int address=10;
long lento;
#define TicksA 3    // пин энкодера TickA
#define TicksB 10    // пин энкодера
#define ENC_TYPE 1 // тип энкодера, 0 или 1

volatile boolean states0, lastStates, turnsFlag;
volatile int encsCounter;

int Ubutton = 11;
bool flagUbutton = false;
uint32_t btnTimer = 0;  /// на месте


//счетчик 2
#define TickA  2  // пин счетчик А
#define TickB  5  // пин счетчик B
#define ENC_TYPE 1    // тип энкодера, 0 или 1
volatile int encCounter;
volatile boolean state0, lastState, turnFlag;

/* сдвиг после считывания оборота вала со шпулей*/

struct Data {
  int DiametroMini = 2.5;
  int DiametroMaxi = 4.5;
  int largo = 15;
  float fvalue = 4.5;
};
// глобальный экземпляр для личного использования
Data data;
long timpoPas = 0;      // временная
int ps = 0;             // временная
long largos = 0;        // временная
long x = 0;             // нажатая цифра
long n = 0;             // число на вводе
long tiempoNumber = 0;  // наборная из клавиатуры


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

LiquidCrystal_I2C lcd(0x27, 16, 2);  // экземпляр жк экрана

bool puntoIzcuierdaFlag = false;  // флаг левого положения
bool puntoDerechaFlag = false;    // флаг правого положения
//uint32_t btnTimer = 0;            // таймер кнопок 
//**************///**************///*****************

volatile int counterTick = 0;       // переменная-счётчик
volatile bool intFlagTick = false;  // флаг!!!!!!!!!!!!!!!!!!!!!
volatile int counter = 0; // переменная-счётчик
volatile int counterSt = 0; // переменная-счётчик на коэфициенте
          
volatile bool intFlag = false;      // флаг!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int houser = 0;                     // сделать память
bool status;
// const int PinLamp = 5;      // заменить на цикл укладчика
const int recalculo = A0;    // кнопка пересчета длинны
bool recalculoFlag = false;  // флаг
const int moto = 6;          // motor rele

const int rotor = 4;  // пин датчика катушки




void setup() {
  Serial.begin(9600);
  // attachInterrupt(1, ints0, CHANGE);
  pinMode(TickA, INPUT_PULLUP);
  pinMode(TickB, INPUT_PULLUP);
  attachInterrupt(0, buttonTickSt, CHANGE);
  //pinMode(Ubutton, INPUT_PULLUP);
  // encsCounter = EEPROM.get(address, lento);
}


void buttonTickSt()  // сработка от прерывания счетчик
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


counterSt = encCounter/3.8709677;

   if (counterSt > 600){

Serial.println("Fin!!!!!!!!!!!!!!!");

    digitalWrite(moto, 1);  // отключаем реле мотора и останавливае укладчик
    // digitalWrite(stepMotoApilador, 0); // стоп мотор
    flagTrabajadora = 0;
    digitalWrite(dereccion, 0);  // сбрасываем направление

    Serial.println("Fin");

    //counter = 0;
    encCounter=0;
    counterSt=0;
  }
}

void loop() {


  Serial.println("encCounter = " + String(encCounter));
                       

  Serial.println("counterSt = " + String(counterSt));
                        
  delay(600);
}