//================== распиновка =============================================
// Скетч для управления станком плавной намотки и укладки цепочки на катушку.
// ресет - стоп или сброс программы.
// D2 - Dir драйвера шагового мотора каретки.
// D3 - Step драйвера шагового мотора каретки.
// D4 - концевик коретки левый.
// D5 - концевик коретки правый.
// D6 - датчик движения катушки.
// D7 - доводка коретки в ручную влево.
// D8 - доводка коретки в ручную вправо.
//================== материалы ==============================================
// ардуино нано атмега328, драйвеер шаг.мотора А4988 или HW135, концевики.

 long stepn23 = 0;
 long spedn23 = 0;
const long TICK=23;////////////////////////69/////////////////////////////////////////////////////////////////////
 const long DelayTick= 200;//////////////////////////////////////////////////////////////////////////////////////
const long SpendTick= 100;//////////////////////////////////////////////////////////////////////////////////////

void my_1() {
  // доводка каретки. --------------------------------------------------------
  if (digitalRead(7) == 0) {  // pin D7
    stepn23 = 1;              // шаги каретки от 1 - 1000
     spedn23 = 1;             // скорость от 1 - 150
       if (stepn23 < 0 && spedn23 > 0){
         stepn23 = stepn23*-1;
         for (int count = 0; count < stepn23; count++) {
           digitalWrite(2, 0);
           digitalWrite(3, 1);
           delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
           digitalWrite(3, 0);
           delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
         }
        stepn23 = stepn23*-1;
          }
       if (stepn23 > 0 && spedn23 > 0){
         for (int count = 0; count < stepn23; count++) {
           digitalWrite(2, 1);
           digitalWrite(3, 1);
           delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
           digitalWrite(3, 0);
           delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
         }
          }
       if (stepn23 == 0 | spedn23 == 0){
           digitalWrite(2, 0);
           digitalWrite(3, 0);
          }
// ============================================================================
  }
  // доводка каретки. ---------------------------------------------------------
  if (digitalRead(8) == 0) {  // pin D8
       stepn23 = (-1);        // шаги каретки от 1 - 1000
     spedn23 = 1;             // скорость от 1 - 150
       if (stepn23 < 0 && spedn23 > 0){
         stepn23 = stepn23*-1;
         for (int count = 0; count < stepn23; count++) {
           digitalWrite(2, 0);
           digitalWrite(3, 1);
           delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
           digitalWrite(3, 0);
           delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
         }
        stepn23 = stepn23*-1;
          }
       if (stepn23 > 0 && spedn23 > 0){
         for (int count = 0; count < stepn23; count++) {
           digitalWrite(2, 1);
           digitalWrite(3, 1);
           delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
           digitalWrite(3, 0);
           delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
         }
          }
       if (stepn23 == 0 | spedn23 == 0){
           digitalWrite(2, 0);
           digitalWrite(3, 0);
          }
// =============================================================================
  }
}

void setup() {
  pinMode(4, INPUT_PULLUP);  // D4 - концевик коретки левый.
  pinMode(6, INPUT_PULLUP);  // D6 - датчик движения катушки.
  pinMode(2, OUTPUT);        // D2 - Dir драйвера шагового мотора каретки.
  pinMode(3, OUTPUT);        // D3 - Step драйвера шагового мотора каретки.
  pinMode(5, INPUT_PULLUP);  // D5 - концевик коретки правый.
  pinMode(7, INPUT_PULLUP);  // D7 - доводка коретки в ручную влево.
  pinMode(8, INPUT_PULLUP);  // D8 - доводка коретки в ручную вправо.
}

void loop() {
    while (digitalRead(4) == 1){
      my_1();
      // концевик каретки. -------------------------------------------------------
      if (digitalRead(6) == 0) {
           stepn23 = TICK;        // шаги каретки от 1 - 1000
           delay(DelayTick);//////////////////////////////////////////////////////////////////////////////////////
         spedn23 = SpendTick;          // скорость каретки от 1 - 150/////////////////////////////////////////////////////////
           if (stepn23 < 0 && spedn23 > 0){
             stepn23 = stepn23*-1;
             for (int count = 0; count < stepn23; count++) {
               digitalWrite(2, 0);
               digitalWrite(3, 1);
               delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
               digitalWrite(3, 0);
               delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
             }
            stepn23 = stepn23*-1;
              }
           if (stepn23 > 0 && spedn23 > 0){
             for (int count = 0; count < stepn23; count++) {
               digitalWrite(2, 1);
               digitalWrite(3, 1);
               delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
               digitalWrite(3, 0);
               delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
             }
              }
           if (stepn23 == 0 | spedn23 == 0){
               digitalWrite(2, 0);
               digitalWrite(3, 0);
              }
// ==============================================================================
      }
    }
    while (digitalRead(5) == 1){
      my_1();
      // концевик каретки. ------------------------------------------------------
            if (digitalRead(6) == 0) {
           stepn23 = (-TICK);      // шаги каретки от 1 - 1000
            delay(DelayTick);//////////////////////////////////////////////////////////////////////////////////////
         spedn23 = SpendTick;           // скорость каретки от 1 - 150
           if (stepn23 < 0 && spedn23 > 0){
             stepn23 = stepn23*-1;
             for (int count = 0; count < stepn23; count++) {
               digitalWrite(2, 0);
               digitalWrite(3, 1);
               delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
               digitalWrite(3, 0);
               delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
             }
            stepn23 = stepn23*-1;
              }
           if (stepn23 > 0 && spedn23 > 0){
             for (int count = 0; count < stepn23; count++) {
               digitalWrite(2, 1);
               digitalWrite(3, 1);
               delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
               digitalWrite(3, 0);
               delayMicroseconds(map(spedn23, 100, 1, 4000, 10000));
             }
              }
           if (stepn23 == 0 | spedn23 == 0){
               digitalWrite(2, 0);
               digitalWrite(3, 0);
              }
// ================================================================================
      }
    }
}