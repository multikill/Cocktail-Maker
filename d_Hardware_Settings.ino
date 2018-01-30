////////////////////////////////////////////////////////////////////////////////////
/// Hardware Ansteuerung  PROGRAMM CODE 2                                        ////
////////////////////////////////////////////////////////////////////////////////////
//
//  void ZutatenLaden(int Z0, int Z1, int Z2, int Z3, int Z4, int Z5, int Z6, int Z7, int Z8, int Z9)
//  void Koordinaten(int Flasche)
//  int Flaschenposition_X(int xy_pos)
//  int Flaschenposition_Y(int xy_pos)
//  void Unterbrech()
//  void setLED(int R, int B, int G)
//  bool GlasVorhanden()
//  void eepromWriteInt(int adr, int wert)
//  int eepromReadInt(int adr)
//
//
////////////////////////////////////////////////////////////////////////////////////


void ZutatenLaden(int Z0, int Z1, int Z2, int Z3, int Z4, int Z5, int Z6, int Z7, int Z8, int Z9) {
  Zutatenn[ 0] = Z0;
  Zutatenn[ 1] = Z1;
  Zutatenn[ 2] = Z2;
  Zutatenn[ 3] = Z3;
  Zutatenn[ 4] = Z4;
  Zutatenn[ 5] = Z5;
  Zutatenn[ 6] = Z6;
  Zutatenn[ 7] = Z7;
  Zutatenn[ 8] = Z8;
  Zutatenn[ 9] = Z9;
}

void Koordinaten(int Flasche) {
  positions[0] = Flaschenposition_X(Flasche);

  long randNumber;
  randomSeed(analogRead(PIN_RANDOM));   // Generate Random Seed with Pin A9 (muss offen bleiben)
  randNumber = random(0, 2);            //a random number between min and max-1 (long)
  //Serial.print(randNumber);


  // Bist du hinten? Nächste Flasche Hinten? => Bleib hinten
  // Bist du vorne ? Nächste Flasche Hinten? => Random 0,1 hinten
  // Bist du start ? Nächste Flasche Hinten? => Random 0,1 hinten
  // Rest Position ?                         => Setzte Position
  if (MOTOR_Y_HINTEN == Flaschenposition_Y(Flasche)) {
    if (stepperY.currentPosition() == (MOTOR_Y_VORNE + MOTOR_Y_HINTEN)) {
      positions[1] = stepperY.currentPosition();
    }
    else if (stepperY.currentPosition() == (MOTOR_Y_VORNE - MOTOR_Y_HINTEN)) {
      positions[1] = stepperY.currentPosition();
    }
    else if ((stepperY.currentPosition() == MOTOR_Y_VORNE) && (randNumber == 0)) {
      positions[1] = MOTOR_Y_VORNE + Flaschenposition_Y(Flasche);
    }
    else if ((stepperY.currentPosition() == MOTOR_Y_VORNE) && (randNumber == 1)) {
      positions[1] = MOTOR_Y_VORNE - Flaschenposition_Y(Flasche);
    }
    else if ((stepperY.currentPosition() == MOTOR_Y_START) && (randNumber == 0)) {
      positions[1] = MOTOR_Y_VORNE + Flaschenposition_Y(Flasche);
    }
    else if ((stepperY.currentPosition() == MOTOR_Y_START) && (randNumber == 1)) {
      positions[1] = MOTOR_Y_VORNE - Flaschenposition_Y(Flasche);
    }
  }
  else {
    positions[1] = Flaschenposition_Y(Flasche);
  }

}

int Flaschenposition_X(int xy_pos) {
  if (xy_pos == 0 || xy_pos == 5)             // Pos 0 &  5
  {
    return -800;
  }

  else if (xy_pos == 1 || xy_pos == 6)        // Pos 1 &  6
  {
    return -2300;
  }

  else if (xy_pos == 2 || xy_pos == 7)        // Pos 2 &  7
  {
    return -3800;
  }

  else if (xy_pos == 3 || xy_pos == 8)        // Pos 3 &  8
  {
    return -5300;
  }

  else if (xy_pos == 4 || xy_pos == 9)        // Pos 4 &  9
  {
    return -6800;
  }

  else if (xy_pos == 20)                     // Grundstellung
  {
    return 0;
  }

  else
  {
    return 0;
  }


}

int Flaschenposition_Y(int xy_pos) {

  if (xy_pos == 0 || xy_pos == 1 || xy_pos == 2 || xy_pos == 3 || xy_pos == 4 )       // Pos 0..4   Vorne
  {
    return MOTOR_Y_VORNE; // Offset zu Startposition
  }

  else if (xy_pos == 5 || xy_pos == 6 || xy_pos == 7 || xy_pos == 8 || xy_pos == 9)   // Pos 5...9  Hinten
  {
    return MOTOR_Y_HINTEN; // Offset zu Vorne
  }

  else if (xy_pos == 20)                                                              // Grundstellung
  {
    return MOTOR_Y_START;
  }

  else
  {
    return MOTOR_Y_START;
  }
}

void Unterbrech() {
  Serial.println("Unterbrechung");
  delay(100);
}

void SpannungAuslesen() {

  int i_24V = analogRead(PIN_DIAG_24V);
  int i_12V = analogRead(PIN_DIAG_12V);
  int i_6V  = analogRead(PIN_DIAG_6V );

  //  Serial.println("  - 24V Spannung hat: " + String(i_24V));
  //  Serial.println("  - 12V Spannung hat: " + String(i_12V));
  //  Serial.println("  -  6V Spannung hat: " + String(i_6V));

  float f_24V = i_24V * (25.05 / 1023.00);
  float f_12V = i_12V * (11.95 / 876.00);
  float f_6V  = i_6V  * ( 7.22 / 920.00);

  Serial.println("  - 24V Spannung hat: " + String(f_24V));
  Serial.println("  - 12V Spannung hat: " + String(f_12V));
  Serial.println("  -  6V Spannung hat: " + String(f_6V));

  if (f_24V <= 23.00) {
    analogWrite(PIN_LED_STATUS, 1);
  }
  if (f_12V <= 11.00) {
    analogWrite(PIN_LED_STATUS, 1);
  }
  if (f_6V  <=  6.00) {
    analogWrite(PIN_LED_STATUS, 1);
  }
}

void setLED(int R, int B, int G) {
  analogWrite(PIN_LED_R, R);
  analogWrite(PIN_LED_B, B);
  analogWrite(PIN_LED_G, G);

  pwm.setPWM(PIN_EXP_LED_ROT, 0, R * 16);
  pwm.setPWM(PIN_EXP_LED_BLAU, 0, B * 16);
  pwm.setPWM(PIN_EXP_LED_GRUEN, 0, G * 16);
}

bool GlasVorhanden() {

  for (int i = 0; i <= 20; i++)
  {
    if (analogRead (PIN_GLAS) < ANALOG_LOW)
    {
      Serial.println("Glas vorhanden");
      delay(1000);                                // Verzoegerung fuer Bediener
      return true;
    }

    Serial.println("Glas NICHT vorhanden");
    delay(100);
  }

  return false;
}

void eepromWriteInt(int adr, int wert) {
  byte low, high;
  low = wert & 0xFF;
  high = (wert >> 8) & 0xFF;
  EEPROM.write(adr, low); // dauert 3,3ms
  EEPROM.write(adr + 1, high);
  return;
} //eepromWriteInt

int eepromReadInt(int adr) {

  byte low, high;
  low = EEPROM.read(adr);
  high = EEPROM.read(adr + 1);
  return low + ((high << 8) & 0xFF00);
} //eepromReadInt


