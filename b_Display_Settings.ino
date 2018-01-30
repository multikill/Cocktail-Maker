////////////////////////////////////////////////////////////////////////////////////
/// Display  Ansteuerung  PROGRAMM  Settings                                    ////
////////////////////////////////////////////////////////////////////////////////////
//
//  void EntwicklerModus()
//  void Farbe( char FarbeSchrift, char FarbeHintergrund)
//  void LadeScreenKopfFussZeile(int Kopf, int Fuss, int pLinks, int pRechts)
//  void LadeScreenNamen()
//  void strNamen()
//  void strName(int iNummer, int iCocktailNamen)
//
//
//
////////////////////////////////////////////////////////////////////////////////////

void EntwicklerModus() {
  // Touch Eingaben auf dem Bildschirm ausgeben

  int x = 0.00;
  int y = 0.00;

  Touch.read();
  x = Touch.getX();                        // X Koordinaten, wo am Bildschirm gedrÃ¼ckte wurde
  y = Touch.getY();                        // Y Koordinaten, wo am Bildschirm gedrÃ¼ckte wurde

  LCD.setFont(BigFont);
  LCD.print("Touch Press: " + String(Touch.dataAvailable()), CENTER, 10);
  LCD.print("Position: " + String(x) + " " + String(y) + String("     "), CENTER, 80);

}

void Farbe( char FarbeSchrift, char FarbeHintergrund) {
  int k;
  k = EEPROM.read(E_ADDR_FARBEN);


  if ( (k > 3) || (k < 0) )             // Check ob es eine gültige Farbe ist
  {
    Serial.println("Setze Farbe auf Standard!");

    EEPROM.write(E_ADDR_FARBEN, 0);
    k = EEPROM.read(E_ADDR_FARBEN);
  }

  int FarbMatrix[3][3][3] = {0, 0, 0, 255, 255, 255, 96, 96, 96,
                             8, 49, 107, 192, 192, 192, 125, 35, 47,
                             255, 255, 255, 0, 0, 0, 96, 96, 96
                            };

  // Zeilen sind Farbkombinationen 3x3x3
  // Hintergrund, Schrift, Balken
  // 0 = Schwarz, 255 = WeiÃŸ, 96 = Grau

  // [1] [2] [3]
  // [1] = Farbkombination
  // [2] = Farbe in Gruppen
  // [3] = Farbe in RGB Werten
  switch (FarbeSchrift)
  {
    case 'b':
      LCD.setColor(FarbMatrix[k][0][0], FarbMatrix[k][0][1], FarbMatrix[k][0][2]);
      break;
    case 'w':
      LCD.setColor(FarbMatrix[k][1][0], FarbMatrix[k][1][1], FarbMatrix[k][1][2]);
      break;
    case 'g':
      LCD.setColor(FarbMatrix[k][2][0], FarbMatrix[k][2][1], FarbMatrix[k][2][2]);
      break;
    default:
      break;
  }

  switch (FarbeHintergrund)
  {
    case 'b':
      LCD.setBackColor(FarbMatrix[k][0][0], FarbMatrix[k][0][1], FarbMatrix[k][0][2]);
      break;
    case 'w':
      LCD.setBackColor(FarbMatrix[k][1][0], FarbMatrix[k][1][1], FarbMatrix[k][1][2]);
      break;
    case 'g':
      LCD.setBackColor(FarbMatrix[k][2][0], FarbMatrix[k][2][1], FarbMatrix[k][2][2]);
      break;
    default:
      break;
  }
}

void LadeScreenKopfFussZeile(int Kopf, int Fuss, int pLinks, int pRechts) {
  //Hintergrund
  //LCD.clrScr();                  // Nicht mÃ¶glich wegen Farbcombi
  Farbe('b', 'w');
  LCD.fillRect(DISPLAY_ZERO, DISPLAY_ZERO, DISPLAY_Y_MAX, DISPLAY_X_MAX);

  // Kopfzeile
  if (Kopf == 1)
  {
    Farbe('g', 'g');
    LCD.fillRect(DISPLAY_ZERO, DISPLAY_ZERO, DISPLAY_Y_MAX, 96);
    if (!ProduzierenErlaubt)
    {
      Farbe('w', 'g');
      LCD.fillCircle(460, 20, 5);
    }
  }

  // Fusszeile
  if (Fuss == 1)
  {
    //Fusszeile Hintergrund
    Farbe('g', 'g');
    LCD.fillRect(DISPLAY_ZERO, DISPLAY_X_MAX - 96, DISPLAY_Y_MAX, DISPLAY_X_MAX);

    // Fusszeile Schrift
    int Unten;
    Unten = 724;
    LCD.setFont(Grotesk24x48);
    Farbe('w', 'g');
    LCD.print(String("Home"), CENTER, Unten);

    // Fusszeile Pfeile
    if (pLinks == 1)
      LCD.print(String("<"), 80, Unten);
    if (pRechts == 1)
      LCD.print(String(">"), 400 - 24, Unten);
  }
}

void LadeScreenNamen() {
  //Kopfzeile Namen
  LCD.setFont(Grotesk24x48);
  Farbe('w', 'g');
  LCD.print(CocktailNamen[0], CENTER, 24);    //Ãœberschrift

  //Farbe('b', 'w');                            // Zahlen in Punkte umgewandelt
  //LCD.print(String(l),34 ,i);

  Farbe('w', 'b');
  LCD.setFont(Grotesk16x32);
  // i = Y-Abstand zwischen den Namen
  for (int kreis = 137, cN = 1, i = 121.5; i <= 700; i = i + 75, cN++)
  {
    if (CocktailNamen[cN] != String(""))       // Wenn keine Namen, dann zeichne keinen Kreis
      LCD.fillCircle(50, kreis, 10);

    kreis = kreis + 75;
    LCD.print(CocktailNamen[cN], 100 , i);
  }
}

void strNamen() {
  // Suchen Namen mit Aufruf Wert

  char buf [MAX_SIZE];
  for (int p = 0; p <= 8; p = p + 1)
  {
    strcpy_P (buf, (const char *) & descriptions [Aufruf + p]);
    CocktailNamen[p] = buf;
  }
}

void strName(int iNummer, int iCocktailNamen) {
  char buf [MAX_SIZE];
  strcpy_P (buf, (const char *) & descriptions [iNummer]);
  CocktailNamen[iCocktailNamen] = buf;

}

void ZutatenNamenLade(int SeiteZwei) {
  if (!SeiteZwei)
  {
    // 1. Zutaten NummerAnzeige 2. Globales Char Array
    strName(Zutatenn[ 0], 1);
    strName(Zutatenn[ 1], 2);
    strName(Zutatenn[ 2], 3);
    strName(Zutatenn[ 3], 4);
    strName(Zutatenn[ 4], 5);
  }

  if (SeiteZwei)
  {
    strName(Zutatenn[ 5], 1);
    strName(Zutatenn[ 6], 2);
    strName(Zutatenn[ 7], 3);
    strName(Zutatenn[ 8], 4);
    strName(Zutatenn[ 9], 5);
  }

  if (Aufruf > 99)
    strName(10, 7); //Mischen Anzeigen
}


