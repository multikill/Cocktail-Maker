/// Communication Ansteuerung                                                   ////
////////////////////////////////////////////////////////////////////////////////////
//
//  void checkMystring()
//  void ComHelp()
//  void ComPageAufrufen()
//  void ComPagePress()
//  void ComSetCocktail()
//  void ComSetTiming()
//  void ComVentil()
//  void ComMotorXspeed()
//  void ComMotorYspeed()
//
////////////////////////////////////////////////////////////////////////////////////


void checkMystring() {

  inputString.trim();                               // Lösche Leerzeichen, Zeilenumbruch (cl + cr)
  inputString.toLowerCase();                        // Scheiß auf Großbuchstaben
  Serial.println("Deine Eingabe: " + inputString);  // Eingabe wieder ausgeben


  if (inputString == "home") {
    Aufruf = 30;
  }
  else if (inputString == "settings") {
    Aufruf = 40;
  }
  else if (inputString == "help") {
    ComHelp();
  }
  else if (inputString.substring(0, 4) == "page")
  {
    ComPageAufrufen();
  }
  else if (inputString.substring(0, 5) == "press")
  {
    ComPagePress();
  }
  else if (inputString.substring(0, 11) == "setcocktail") {
    ComSetCocktail();
  }
  else if (inputString.substring(0, 9) == "settiming") {
    ComSetTiming();
  }
  else if (inputString == "startcocktail") {
    Aufruf = 11;                                    // Mit Display
  }
  else if (inputString == "starttest1") {
    ZutatenLaden(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);     // Reihenfolge der Anfahrt
    Zubereiten();                                   // Hardware beginnen
  }
  else if (inputString == "starttest2") {
    ZutatenLaden(0, 5, 1, 6, 2, 7, 3, 8, 4, 9);     // Reihenfolge der Anfahrt
    Zubereiten();                                   // Hardware beginnen
  }
  else if (inputString.substring(0, 6) == "ventil") {
    ComVentil();
  }
  else if (inputString.substring(0, 11) == "motorxspeed")
  {
    ComMotorXspeed();
  }
  else if (inputString.substring(0, 11) == "motoryspeed")
  {
    ComMotorYspeed();
  }
  else if (inputString == "motorreferenz")
  {
    if (!Grundstellung())
    {
      Referenzfahrt();
    }
  }
  else if (inputString == "volt")
  {
    ServoEinSchalten(1);
    delay(100);
    SpannungAuslesen();
    ServoEinSchalten(0);
  }
  else {
    Serial.println("Keine Uebereinstimmung");
  }

}

void ComHelp() {
  Serial.println("All verfuegbaren Befehle: ");
  Serial.println("Befehl:         Beispiel              Beschreibung\t");
  Serial.println("help                                  Diese Befehlsliste wird aufgerufen");
  Serial.println("home                                  Das Home Display wird aufgerufen");
  Serial.println("settings                              Das settings Display wird aufgerufen");
  Serial.println("pagex           page401               Zeigt am Display die Seite Drachenblut ");
  Serial.println("pressx          press10               Drueckt am Display den Home Button (1-12)");
  Serial.println("");
  Serial.println("motorxspeedx    motorxspeed800        Die Motor X Geschwindigkeit wird gesetzt (0-1023)");
  Serial.println("motoryspeedx    motoryspeed200        Die Motor Y Geschwindigkeit wird gesetzt (0-0123");
  Serial.println("motorreferenz                         Schrittmotoren fahren in Grundstellung falls Sie nicht sind");
  Serial.println("venitlx         ventil0               Loest die 1.Flasche fuer 2 Sekunden aus (0-9)");
  Serial.println("");
  Serial.println("setcocktail     setCocktail,0,1,2,3,4,5,6,7,8,9,                      Setzt die Flaschenreihenfolge");
  Serial.println("settiming       settiming,100,200,300,400,500,600,700,800,900,1000,   Setzt die Dosierzeit der Flaschen");
  Serial.println("startcocktail                                                         Startet die Cocktailzubereitung");
  Serial.println("starttest1                                                            Beginnt die Testzubereitung 0,1,2,3...");
  Serial.println("starttest2                                                            Beginnt die Testzubereitung 0,5,1,6...");
  Serial.println("");
  Serial.println("volt                                  Eingangsspannung von 24V, 12V und 7.2 V anzeigen");
}

void ComPageAufrufen() {

  int     Anfang      = 4;
  int     Ende        = inputString.length();
  String  Eingabe     = inputString.substring(Anfang, Ende);
  int     EingabeInt  = Eingabe.toInt();
  //Serial.println("Die Aufzurufende Seite ist: " + EingabeInt);

  Aufruf = EingabeInt;
}

void ComPagePress() {

  int     Anfang      = 5;
  int     Ende        = inputString.length();
  String  Eingabe     = inputString.substring(Anfang, Ende);
  int     EingabeInt  = Eingabe.toInt();
  //Serial.println("Dein Tastendruck ist: " + EingabeInt);

  SetTouchAuswahl = EingabeInt;
}

void ComSetCocktail() {

  int     Anfang      = 11;
  int     Ende        = inputString.length();
  String  Eingabe     = inputString.substring(Anfang, Ende);

  String  TempZahl    = "";
  int   Komma[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int   Zahl[16]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  Serial.println("Dein Tastendruck ist: " + Eingabe);
  // setCocktail,0,1,2,3,4,5,6,7,8,9,

  Komma[0]  = Eingabe.indexOf(',');                                                 // Finde Komma 0
  //Komma[10] = Eingabe.lastIndexOf(',');                                             // Finde das letzte Komma

  for (int i = 1; i <= 10; i++) {                                                   // Finde Komma 1-10
    Komma[i] = Eingabe.indexOf(',', Komma[i - 1] + 1);
  }

  int fail = 0;
  for (int i = 0; i <= 9; i++) {                                         // Finde Zahl 0-9
    TempZahl = "";
    if ((Komma[i + 1] == -1) || (fail == 1)) {
      //Serial.println("Fehlende Komma");
      fail = 1;
      Zahl[i] = 20;
    }
    else {
      TempZahl = Eingabe.substring(Komma[i] + 1, Komma[i + 1]); // Finde Zahl 0-9
      Zahl[i]  = TempZahl.toInt();
    }
  }

  for (int i = 0; i <= 9; i++) {                                                    // Uebergebe die Werte
    if ((Zahl[i] >= 0) && (Zahl[i] <= 20)) {
      Zutatenn[i] = Zahl[i];
    }
    else {
      Zutatenn[i] = 0;
    }
  }

  //  for(int j = 0; j <=10; j++) {                                                     // Gib Komma 0-10 aus
  //    Serial.println("Komma Nr.: " + String(j) + " Position: "+ String(Komma[j]));
  //  }
  //
  //  for(int j = 0; j <=9; j++) {                                                      // Gib Zahl 0-9 aus
  //    Serial.println("Zahl Nr.: " + String(j) + " Wert: "+ String(Zahl[j]));
  //  }
  //
  for (int j = 0; j <= 9; j++) {                                                    // Gib Zahl 0-9 aus
    Serial.println("Zutat Nr.: " + String(j) + " Flasche: " + String(Zutatenn[j]));
  }
}

void ComSetTiming() {

  int     Anfang      = 9;
  int     Ende        = inputString.length();
  String  Eingabe     = inputString.substring(Anfang, Ende);

  String  TempZahl    = "";
  int   Komma[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int   Zahl[16]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  Serial.println("Dein Tastendruck ist: " + Eingabe);
  // settiming,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
  // settiming,100,200,300,400,500,600,700,800,900,1000,
  // setcocktail,0,0,0,0,0,0,0,0,0,0,

  Komma[0]  = Eingabe.indexOf(',');                                                 // Finde Komma 0

  for (int i = 1; i <= 10; i++) {                                                   // Finde Komma 1-10
    Komma[i] = Eingabe.indexOf(',', Komma[i - 1] + 1);
  }

  for (int i = 0; i <= 9; i++) {                                                    // Finde Zahl 0-9
    TempZahl = "";
    TempZahl = Eingabe.substring(Komma[i] + 1, Komma[i + 1]); // Finde Zahl 0-9
    Zahl[i]  = TempZahl.toInt();
  }

  for (int i = 0; i <= 9; i++) {                                                    // Uebergebe die Werte
    if ((Zahl[i] >= 0) && (Zahl[i] <= 10000)) {
      Timenn[i] = Zahl[i];
    }
    else {
      Timenn[i] = 5000;
    }
  }

  //  for(int j = 0; j <=10; j++) {                                                     // Gib Komma 0-10 aus
  //    Serial.println("Komma Nr.: " + String(j) + " Position: "+ String(Komma[j]));
  //  }
  //
  //  for(int j = 0; j <=9; j++) {                                                      // Gib Zahl 0-9 aus
  //    Serial.println("Zahl Nr.: " + String(j) + " Wert: "+ String(Zahl[j]));
  //  }
  //
  for (int j = 0; j <= 9; j++) {                                                    // Gib Zahl 0-9 aus
    Serial.println("Zutat Nr.: " + String(j) + " Zeti: " + String(Timenn[j]));
  }
}

void ComVentil() {
  int     Anfang      = 6;
  int     Ende        = inputString.length();
  String  Eingabe     = inputString.substring(Anfang, Ende);
  int     EingabeInt  = Eingabe.toInt();
  //Serial.println("Dein Tastendruck ist: " + EingabeInt);

  // Rot Blau Grün
  setLED(255, 0, 0);
  ServoEinSchalten(1);                      // Servo Motoren Einschalten
  Ventil(EingabeInt);
  delay(1000);
  VentilZu();
  delay(1000);
  ServoEinSchalten(0);                      // Servo Motoren Einschalten
}

void ComMotorXspeed() {

  int     Anfang      = 11;
  int     Ende        = inputString.length();
  String  Eingabe     = inputString.substring(Anfang, Ende);
  int     EingabeInt  = Eingabe.toInt();
  //Serial.println("Die Aufzurufende Seite ist: " + String(EingabeInt));

  EEPROM.write(E_ADDR_MOTOR_X, (EingabeInt / 4));                         // Durch 4 weil EEPROM nur 1 Byte kann

  MotorInitalisieren();
}

void ComMotorYspeed() {

  int     Anfang      = 11;
  int     Ende        = inputString.length();
  String  Eingabe     = inputString.substring(Anfang, Ende);
  int     EingabeInt  = Eingabe.toInt();
  //Serial.println("Die Aufzurufende Seite ist: " + String(EingabeInt));

  EEPROM.write(E_ADDR_MOTOR_Y, (EingabeInt / 4));                         // Durch 4 weil EEPROM nur 1 Byte kann

  MotorInitalisieren();
}


