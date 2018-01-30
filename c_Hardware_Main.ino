////////////////////////////////////////////////////////////////////////////////////
/// Hardware Ansteuerung  PROGRAMM CODE                                         ////
////////////////////////////////////////////////////////////////////////////////////
//
//  void Zubereiten()
//  void Abfuellen()
//  void BewegDich()
//  void MotorInitalisieren()
//  bool Grundstellung()
//  void Referenzfahrt()
//  void MotorZustand()
//  void MotorenEinSchalten(int EIN)
//  void Ventil(int Flasche)
//  void VentilZu()
//  void ServoEinSchalten(int EIN)
//
//
////////////////////////////////////////////////////////////////////////////////////


void Zubereiten() {
  Serial.println("Zubereiten beginnen");

  FehlerBeimProduzieren = false;
  setLED(0, 0, 255); // Gruen

  if (MotorFehler)
  {
    Serial.println("Motor Fehler");
  }
  else
  {

    if (!Grundstellung())
    {
      Referenzfahrt();
    }
    if (!GlasVorhanden()) {
      FehlerBeimProduzieren = true;
      return;
    }


    MotorenEinSchalten(1);
    ServoEinSchalten(1);


    //CocktailLaden();

    //    interrupts();
    Abfuellen();
    //    noInterrupts();

    MotorenEinSchalten(0);
    ServoEinSchalten(0);


    if (FehlerBeimProduzieren == true) {
      setLED(255, 0, 0); // ROT
    }
    else {
      setLED(0, 0, 255); // Grün

      long Zaehler;
      Zaehler = eepromReadInt(E_ADDR_ZAEHLER);
      Zaehler = Zaehler + 1;
      eepromWriteInt(E_ADDR_ZAEHLER, Zaehler);
      //Serial.println(Zaehler);
    }
    Serial.println("Zubereiten beendet");
    delay(100);
  }

}

void Abfuellen() {
  Serial.println("Abfuellen bgeinnen");

  for (int i = 0; i <= 10; i++)
  {
    Serial.print("Anfahren Flasche: ");
    Serial.println(Zutatenn[i]);

    Koordinaten(Zutatenn[i]);
    Serial.print("Aktuelle Position X: ");
    Serial.println(stepperX.currentPosition());
    Serial.print("Aktuelle Position Y: ");
    Serial.println(stepperY.currentPosition());
    Serial.print("Neue Position X: ");
    Serial.println(positions[0]);
    Serial.print("Neue Position Y: ");
    Serial.println(positions[1]);
    BewegDich();

    SpannungAuslesen();
    MotorZustand();
    if (MotorFehler) {
      MotorenEinSchalten(0);
      Serial.println("Motor Fehler Abfuellen");
    }
    else {
      if (Zutatenn[i] == 20) {
        // do nothing
      }
      else {

        Ventil(Zutatenn[i]);
        delay(Timenn[i]);
        VentilZu();
        if (Zutatenn[i] == Zutatenn[i + 1])
        {
          delay(DOSIER_SAMEBOTTLE);
        }
        else
        {
          delay(DOSIER_AFTERTIME);
        }

      }
    }
  }

}

void BewegDich() {
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
  Serial.println("Beweg Dich in Ordnung");
}

void MotorInitalisieren() {
  float MotorX_Speed = (float) (EEPROM.read(E_ADDR_MOTOR_X));;
  float MotorY_Speed = (float) (EEPROM.read(E_ADDR_MOTOR_Y));;
  MotorX_Speed *= 4;
  MotorY_Speed *= 4;

  if ((MotorX_Speed >= 0) && (MotorX_Speed <= 1000)) {  // Setzte Motor X Speed
    stepperX.setMaxSpeed(MotorX_Speed);                 // Geschwindigkeit aus EEPROM
  }
  else {
    stepperX.setMaxSpeed(400);                          // Standard Geschwindikgkeit
  }

  if ((MotorY_Speed >= 0) && (MotorY_Speed <= 1000)) {  // Setzte Motor Y Speed
    stepperY.setMaxSpeed(MotorY_Speed);                 // Geschwindigkeit aus EEPROM
  }
  else {
    stepperY.setMaxSpeed(50);                          // Standard Gewschindigkeit
  }

  stepperX.setAcceleration(10);                        // Maximale Beschleunigung fuer X und Y
  stepperY.setAcceleration(10);
  // speed 100 acc 5

  Serial.println("  - Motor X Speed: " + String(stepperX.maxSpeed()));
  Serial.println("  - Motor Y Speed: " + String(stepperY.maxSpeed()));
}

bool Grundstellung() {
  if ((stepperX.currentPosition() == 0) && (stepperY.currentPosition() == 0))
  {
    Serial.println("  - Grundstellung Software vorhanden");
  }
  else
  {
    Serial.println("  - Grundstellung  Software NICHT vorhanden");
    return false;
  }

  if ((analogRead (PIN_X_ACHSE) > ANALOG_HIGH) && (analogRead (PIN_Y_ACHSE) > ANALOG_HIGH))
  {
    Serial.println("  - Grundstellung Hardware vorhanden");
    return true;
  }
  else
  {
    Serial.println("  - Grundstellung Hardware NICHT vorhanden");
    return false;
  }
}

void Referenzfahrt() {
  Serial.print("  - Referenzfahrt X laeuft: ");
  MotorenEinSchalten(1);

  //*** Referenzfahr X_Achse Kleines Stück nach rechts fahren*//
  stepperX.setMaxSpeed(-200);
  stepperX.setSpeed(200);                  // Rechts fahren
  //stepperX.runSpeed();
  //delay(2000);

  for (int i = 0; i <= 1000; i++)
  {
    stepperX.runSpeed();
    delay(1);
  }
  stepperX.stop();

  //*** Referenzfahr X_Achse ***//
  stepperX.setMaxSpeed(-200);              // Links fahren mit "-xx"
  stepperX.setSpeed(-200);                 // Beide sind notwendig!

  while (!(analogRead (PIN_X_ACHSE) > ANALOG_HIGH))
  {
    stepperX.runSpeed();
    Serial.print(".");
  }

  delay(20);                                // Das er Mittig drauf steht
  stepperX.stop();
  stepperX.setCurrentPosition(0);
  Serial.println();
  Serial.println("  - Referenzfahrt X erfolgreich");

  Serial.print("  - Referenzfahrt Y laeuft: ");

  //*** Referenzfahr Y_Achse ***//
  stepperY.setMaxSpeed(400);              // Links fahren mit "-xx"
  stepperY.setSpeed(-400);                 // Beide sind notwendig!

  while (!(analogRead (PIN_Y_ACHSE) > ANALOG_HIGH))
  {
    stepperY.runSpeed();
    Serial.print(".");
  }

  stepperY.stop();
  stepperY.setCurrentPosition(0);
  Serial.println();
  Serial.println("  - Referenzfahrt Y erfolgreich");

  MotorInitalisieren();
  MotorenEinSchalten(0);
}

void MotorZustand() {

  if (analogRead(PIN_TREIBER_FAIL) < ANALOG_LOW) {
    MotorFehler = true;
    FehlerBeimProduzieren = true;
    analogWrite(PIN_LED_STATUS, 1);
    Serial.println("Treiber Fehler erkannt");
    Serial.println("MotorZustand PIN_TREIBER_FAIL(sollte A6) ist LOW!");
  }

  if (analogRead(PIN_MOTOR_RESET) < ANALOG_LOW) {
    MotorFehler = false;
    Serial.println("Treiber Fehler zurückgesetzt");
    Serial.println("MotorZustand PIN_MOTOR_RESET (sollte A8) ist LOW!");

    digitalWrite(PIN_TREIBER_RESET, LOW);        // Treiber Reset (Low Active)
    delay(100);
    digitalWrite(PIN_TREIBER_RESET, HIGH);
  }
}

void MotorenEinSchalten(int EIN) {
  if (EIN == true)
  {
    pwm.setPWM(PIN_EXP_LUEFTER, 0, 3000);
    digitalWrite(PIN_MOTOR_EIN, LOW);
  }
  else
  {
    pwm.setPWM(PIN_EXP_LUEFTER, 0, 3900);
    digitalWrite(PIN_MOTOR_EIN, HIGH);
  }
}

void Ventil(int Flasche) {
  Serial.println(" Servo Flaschen Ansteuerung Nr: " + String(Flasche));

  switch (Flasche) {
    case 0:
      pwm.setPWM(PIN_EXP_SERVO1, 0, SERVO_AL);
      break;
    case 1:
      pwm.setPWM(PIN_EXP_SERVO2, 0, SERVO_AL);
      break;
    case 2:
      pwm.setPWM(PIN_EXP_SERVO3, 0, SERVO_AL);
      break;
    case 3:
      pwm.setPWM(PIN_EXP_SERVO4, 0, SERVO_AL);
      break;
    case 4:
      pwm.setPWM(PIN_EXP_SERVO5, 0, SERVO_AL);
      break;
    case 5:
      pwm.setPWM(PIN_EXP_SERVO1, 0, SERVO_AR);
      break;
    case 6:
      pwm.setPWM(PIN_EXP_SERVO2, 0, SERVO_AR);
      break;
    case 7:
      pwm.setPWM(PIN_EXP_SERVO3, 0, SERVO_AR);
      break;
    case 8:
      pwm.setPWM(PIN_EXP_SERVO4, 0, SERVO_AR);
      break;
    case 9:
      pwm.setPWM(PIN_EXP_SERVO5, 0, SERVO_AR);
      break;
  }

  //    if(Flasche <= 4)
  //      pwm.setPWM(Flasche, 0, SERVO_AL);
  //
  //    if(Flasche >= 5)
  //      pwm.setPWM(Flasche-5, 0, SERVO_AR);
}

void VentilZu() {

  pwm.setPWM(PIN_EXP_SERVO1, 0, SERVO_MITTE);
  pwm.setPWM(PIN_EXP_SERVO2, 0, SERVO_MITTE);
  pwm.setPWM(PIN_EXP_SERVO3, 0, SERVO_MITTE);
  pwm.setPWM(PIN_EXP_SERVO4, 0, SERVO_MITTE);
  pwm.setPWM(PIN_EXP_SERVO5, 0, SERVO_MITTE);

  //    for(int i = 0; i <= 4; i++)
  //      {pwm.setPWM(i, 0, SERVO_MITTE);}        // Mittelstellung
}

void ServoEinSchalten(int EIN) {

  if (EIN == 1) {
    digitalWrite(PIN_EXP_EIN, LOW);           // PWM Expander Einschalten
    delay(100);
  }
  else {
    digitalWrite(PIN_EXP_EIN, HIGH);           // PWM Expander Ausschalten
  }

}

