/*
  Максимально быстрый универсальный код для обработки энкодера
  Работает на перывании (используется одно)
  Тут код построен на digitalRead, что делает его универсальным для всех плат Arduino
  const int TickA = 2;  // пин счетчик А
const int TickB = 3;  // пин счетчик B
*/

#include <EEPROM.h>
int address = 10;
long lento;
//-------------------------
 int speedAddress= 250;
long spider;

#define TicksA 3    // пин энкодера TickA
#define TicksB 10    // пин энкодера
#define ENC_TYPE 1 // тип энкодера, 0 или 1
//----------------------------------------
#define TickSA 2    // пин энкодера TickA
#define TickSB 9    // пин энкодера
//#define ENC_TYPE 1 // тип энкодера, 0 или 1
//------------------

volatile boolean states0, lastStates, turnsFlag;
volatile int encsCounter;
//------------------------------
volatile boolean stateS0, lastStateS, turnSFlag;
volatile int encSCounter;


int Ubutton = 11;
bool flagUbutton = false;
uint32_t btnTimer = 0;  /// на месте
//--------------------------------
int UbuttonS = 8;
bool flagUbuttonS = false;
uint32_t btnTimerS = 0;  /// на месте////

void setup()
{
  Serial.begin(9600);
  attachInterrupt(1, ints0, CHANGE);
  pinMode(Ubutton, INPUT_PULLUP);
  encsCounter = EEPROM.get(address, lento);
  //-------------------
  attachInterrupt(0, intS0, CHANGE);
  pinMode(UbuttonS, INPUT_PULLUP);
  encSCounter = EEPROM.get(speedAddress, spider);

}



void ints0()
{
  states0 = digitalRead(TicksA);
  if (states0 != lastStates)
  {
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
//------------------------------------------------------
void intS0()
{
  stateS0 = digitalRead(TickSA);
  if (stateS0 != lastStateS)
  {
#if (ENC_TYPE == 1)
    turnSFlag = !turnSFlag;
    if (turnSFlag)
      encSCounter += (digitalRead(TickSB) != lastStateS) ? -1 : 1;
#else
    encSCounter += (digitalRead(TickSB) != lastStateS) ? -1 : 1;
#endif
    lastStateS = stateS0;
  }
}


void asignar()
{
encsCounter = constrain(encsCounter, 0, 600);
  // int coco=map(encCounter,-300,300,0, 600);  ///длинна
  // coco= constrain(coco,0,600);
  //   Serial.println("coco - "+String(coco));

  Serial.println("encsCounter-Largo = " + String(encsCounter));
  delay(200);
  int beta=(EEPROM.get(address, lento));
  Serial.println(String(beta));
  delay(200);
  Serial.println("*EEPROM = " + String(EEPROM.get(address, lento)));
  delay(600);

  bool btnsState = !digitalRead(Ubutton);
  if (btnsState && !flagUbutton && millis() - btnTimer > 100)
  {
    flagUbutton = true;
    btnTimer = millis();
    // Serial.println("press");
  }
  if (btnsState && flagUbutton && millis() - btnTimer > 500)
  {
    btnTimer = millis();
    EEPROM.put(address, encsCounter);
    Serial.println("press hold");
  }
  if (!btnsState && flagUbutton && millis() - btnTimer > 500)
  {
    flagUbutton = false;
    btnTimer = millis();
    // Serial.println("release");
  }
}
//---------------
void asignarSpeed()
{
encSCounter = constrain(encSCounter, 1000, 2400);
  // int coco=map(encCounter,-300,300,0, 600);  ///длинна
  // coco= constrain(coco,0,600);
  //   Serial.println("coco - "+String(coco));

  Serial.println(" sPEED encSCounter = " + String(encSCounter));
  delay(200);
  int betaS=(EEPROM.get(speedAddress, spider));
  Serial.println(String(betaS));
  delay(200);
  Serial.println("*EEPROM = " + String(EEPROM.get(speedAddress, spider)));
  delay(600);

  bool btnSState = !digitalRead(UbuttonS);
  if (btnSState && !flagUbuttonS && millis() - btnTimerS > 100)
  {
    flagUbuttonS = true;
    btnTimerS = millis();
    // Serial.println("press");
  }
  if (btnSState && flagUbuttonS && millis() - btnTimerS > 500)
  {
    btnTimerS = millis();
    EEPROM.put(speedAddress, encSCounter);
    Serial.println("press sPEED");
  }
  if (!btnSState && flagUbuttonS && millis() - btnTimerS > 500)
  {
    flagUbuttonS = false;
    btnTimerS = millis();
     Serial.println("release");
  }
}
//---------------


void loop()
{
  //asignar( );
  delay(400);
  asignarSpeed();
}