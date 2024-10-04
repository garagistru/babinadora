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

#define TickA 3    // пин энкодера TickA
#define TickB 5    // пин энкодера
#define ENC_TYPE 1 // тип энкодера, 0 или 1

volatile boolean state0, lastState, turnFlag;
volatile int encCounter;

int Ubutton = 10;
bool flagUbutton = false;
uint32_t btnTimer = 0;

void setup()
{
  Serial.begin(9600);
  attachInterrupt(1, int0, CHANGE);
  pinMode(Ubutton, INPUT_PULLUP);
  encCounter = EEPROM.get(address, lento);
}

void int0()
{
  state0 = digitalRead(TickA);
  if (state0 != lastState)
  {
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

void loop()
{
  encCounter = constrain(encCounter, 0, 600);
  // int coco=map(encCounter,-300,300,0, 600);  ///длинна
  // coco= constrain(coco,0,600);
  //   Serial.println("coco - "+String(coco));

  Serial.println("encCounter = " + String(encCounter));
  delay(200);
  int beta=(EEPROM.get(address, lento));
  Serial.println(String(beta));
  delay(200);
  Serial.println("*EEPROM = " + String(EEPROM.get(address, lento)));
  delay(600);

  bool btnState = !digitalRead(Ubutton);
  if (btnState && !flagUbutton && millis() - btnTimer > 100)
  {
    flagUbutton = true;
    btnTimer = millis();
    // Serial.println("press");
  }
  if (btnState && flagUbutton && millis() - btnTimer > 500)
  {
    btnTimer = millis();
    EEPROM.put(address, encCounter);
    Serial.println("press hold");
  }
  if (!btnState && flagUbutton && millis() - btnTimer > 500)
  {
    flagUbutton = false;
    btnTimer = millis();
    // Serial.println("release");
  }
}