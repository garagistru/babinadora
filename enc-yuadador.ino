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

#define TicksA 3    // пин энкодера TickA
#define TicksB 10    // пин энкодера
#define ENC_TYPE 1 // тип энкодера, 0 или 1

volatile boolean states0, lastStates, turnsFlag;
volatile int encsCounter;

int Ubutton = 11;
bool flagUbutton = false;
uint32_t btnTimer = 0;  /// на месте
void setup()
{
  Serial.begin(9600);
  attachInterrupt(1, ints0, CHANGE);
  pinMode(Ubutton, INPUT_PULLUP);
  encsCounter = EEPROM.get(address, lento);
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
void asignar()
{
encsCounter = constrain(encsCounter, 0, 600);
  // int coco=map(encCounter,-300,300,0, 600);  ///длинна
  // coco= constrain(coco,0,600);
  //   Serial.println("coco - "+String(coco));

  Serial.println("encsCounter = " + String(encsCounter));
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
void loop()
{
  asignar( );
}