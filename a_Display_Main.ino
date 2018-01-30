////////////////////////////////////////////////////////////////////////////////////
/// Display  Ansteuerung  PROGRAMM  CODE                                        ////
////////////////////////////////////////////////////////////////////////////////////
//
//  void LadeScreen(int Nummer)
//  void TouchAuslesen()
//  void AuswertungBearbeiten()
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////


void LadeScreen(int Nummer) {
  if (PageNummer == Nummer)                   // Gleiche Page = Doppelaufruf verhindern
    return;

  for (int k = 0; k <= 8; k = k + 1)         // Cocktail Namen lÃ¶schen
    CocktailNamen[k] = "";


  ////////////////////////////////////
  //     Haupt Auswahl MenÃ¼         //
  ////////////////////////////////////
  // 30. Home                       //
  // 40. Einstellungen              //
  // 50. Einstellungen Refresh      //
  //  6. Flaschenposition Vorne     //
  //  7. Flaschenposition Hinten    //
  // 10. Start Bildschirm           //
  // 11. Cocktail wird produziert   //
  // 12. Cocktail ist Fertig        //
  ////////////////////////////////////

  switch (Nummer)
  {
    case 30:   // Home
      LadeScreenKopfFussZeile(1, 0, 0, 0);

      strNamen();

      LadeScreenNamen();
      PageNummer = Nummer;

      setLED(0, 255, 0); // Blau
      break;

    case 40:   // Einstellungen
      LadeScreenKopfFussZeile(1, 1, 0, 0);

      strNamen();
      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 50:  // Einstellungen
      // Refresh Funktion
      PageNummer = Nummer;
      break;

    case 6:   // Flaschen Vorne
      LadeScreenKopfFussZeile(1, 1, 0, 0);

      CocktailNamen[0] = "Flaschen Vorne";
      ZutatenLaden(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
      ZutatenNamenLade(0);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 7:   // Flaschen Hinten
      LadeScreenKopfFussZeile(1, 1, 0, 0);

      CocktailNamen[0] = "Flaschen Hinten";
      ZutatenLaden(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
      ZutatenNamenLade(1);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;


    case 10:  // Start Bildschirm

      LCD.clrScr();
      LCD.setBackColor(0, 0, 0);
      LCD.setColor(255, 255, 255);
      LCD.setFont(Grotesk24x48);
      LCD.print(String("Cocktail Roboter"), CENTER, 80);

      LCD.setFont(BigFont);
      LCD.print(String("PRESS START TO BEGIN"), CENTER, CENTER);

      LCD.setFont(SmallFont);
      LCD.print(String("  Version:         5.00"), LEFT, 740);
      LCD.print(String("  Datum:           17.01.2017"), LEFT, 760);
      LCD.print(String("  Autor:           Adrian, Benedikt, Andreas"), LEFT, 780);

      LCD.setColor(255, 0, 0);
      LCD.fillRect(30, 135, 450, 137);

      PageNummer = Nummer;
      break;

    case 11: // Produzieren
      LadeScreenKopfFussZeile(1, 0, 0, 0);

      CocktailNamen[0] = "Zubereiten";
      CocktailNamen[2] = "Bei der Arbeit";

      LadeScreenNamen();                        // Einstellungen mit einbauen

      if (ProduzierenErlaubt)
        Zubereiten();
      //delay(5000);
      else
        delay(1000);

      ZuProCock = 0;
      PageNummer = Nummer;
      break;

    case 12:  // Fertig gestellt
      LadeScreenKopfFussZeile(1, 0, 0, 0);

      long Zaehler;
      Zaehler = (eepromReadInt(E_ADDR_ZAEHLER));

      CocktailNamen[0] = "Zubereiten";
      CocktailNamen[3] = "Cocktail Nummer";
      CocktailNamen[4] = String(Zaehler);
      CocktailNamen[5] = "wurde erstellt";

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 13:  // Fehler bei der Erstellung
      LadeScreenKopfFussZeile(1, 0, 0, 0);

      CocktailNamen[0] = "Fehler";
      CocktailNamen[3] = "Zomg";
      CocktailNamen[4] = "Irgendwas doofes";
      CocktailNamen[5] = "ist passiert!";

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    /////////////////////////////////////
    //      Cocktail Auswahl MenÃ¼      //
    /////////////////////////////////////
    // 100. Favoriten                  //
    // 120. Favoriten 2                //
    // 140. Favoriten 3                //
    //                                 //
    // 200. Alle Cocktail's            //
    // 220. Alle Cocktail's 2          //
    // 240. Alle Cocktail's 3          //
    //                                 //
    // 300. Eigene Kreationen          //
    // 320. Eigene Kreationen 2        //
    // 340. Eigene Kreationen 3        //
    /////////////////////////////////////

    case 100:
      LadeScreenKopfFussZeile(1, 1, 0, 1);
      strNamen();
      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 120:
      LadeScreenKopfFussZeile(1, 1, 1, 1);
      strNamen();
      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 140:
      LadeScreenKopfFussZeile(1, 1, 1, 0);
      strNamen();
      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 200:
      LadeScreenKopfFussZeile(1, 1, 0, 1);
      strNamen();
      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 220:
      LadeScreenKopfFussZeile(1, 1, 1, 0);
      strNamen();
      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 300:
      LadeScreenKopfFussZeile(1, 1, 0, 1);
      strNamen();
      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 320:
      LadeScreenKopfFussZeile(1, 1, 1, 0);
      strNamen();
      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    /////////////////////////////////////
    //      Cocktail Auswahl MenÃ¼      //
    /////////////////////////////////////
    // 401. Drachenblut Seite 1        //
    // 411. Drachenblut Seite 2        //
    // 402. Orange                     //
    // 412. Orange 2                   //
    //                                 //
    // 521. Kamikaze Hart              //
    // 531. Kamikaze Hart 2            //
    //                                 //
    /////////////////////////////////////

    case 401:
      LadeScreenKopfFussZeile(1, 1, 0, 1);
      strName(Nummer, 0);

      //ZutatenLaden(2,6,8,9,20,20,20,20,20,20);
      ZutatenLaden(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
      ZutatenNamenLade(0);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 411:
      LadeScreenKopfFussZeile(1, 1, 1, 0);
      strName(Nummer, 0);

      ZutatenNamenLade(1);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 402:
      LadeScreenKopfFussZeile(1, 1, 0, 1);
      strName(Nummer, 0);

      ZutatenLaden(0, 2, 0, 0, 0, 0, 2, 0, 20, 20);
      ZutatenNamenLade(0);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 412:
      LadeScreenKopfFussZeile(1, 1, 1, 0);
      strName(Nummer, 0);

      ZutatenNamenLade(1);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 403:
      LadeScreenKopfFussZeile(1, 1, 0, 1);
      strName(Nummer, 0);

      ZutatenLaden(1, 4, 1, 4, 1, 5, 1, 4, 1, 20);
      ZutatenNamenLade(0);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 413:
      LadeScreenKopfFussZeile(1, 1, 1, 0);
      strName(Nummer, 0);

      ZutatenNamenLade(1);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 404:
      LadeScreenKopfFussZeile(1, 1, 0, 1);
      strName(Nummer, 0);

      ZutatenLaden(3, 4, 3, 3, 4, 3, 3, 3, 20, 20);
      ZutatenNamenLade(0);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 414:
      LadeScreenKopfFussZeile(1, 1, 1, 0);
      strName(Nummer, 0);

      ZutatenNamenLade(1);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 405:
      LadeScreenKopfFussZeile(1, 1, 0, 1);
      strName(Nummer, 0);

      ZutatenLaden(2, 7, 20, 20, 20, 20, 20, 20, 20, 20);
      ZutatenNamenLade(0);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 415:
      LadeScreenKopfFussZeile(1, 1, 1, 0);
      strName(Nummer, 0);

      ZutatenNamenLade(1);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 406:
      LadeScreenKopfFussZeile(1, 1, 0, 1);
      strName(Nummer, 0);

      ZutatenLaden(3, 3, 3, 3, 3, 20, 20, 20, 20, 20);
      ZutatenNamenLade(0);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 416:
      LadeScreenKopfFussZeile(1, 1, 1, 0);
      strName(Nummer, 0);

      ZutatenNamenLade(1);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 521:
      LadeScreenKopfFussZeile(1, 1, 0, 1);
      strName(Nummer, 0);

      ZutatenLaden(1, 4, 1, 4, 1, 5, 1, 4, 1, 20);
      ZutatenNamenLade(0);

      LadeScreenNamen();
      PageNummer = Nummer;
      break;

    case 531:
      LadeScreenKopfFussZeile(1, 1, 1, 0);
      strName(Nummer, 0);

      ZutatenNamenLade(1);


      LadeScreenNamen();
      PageNummer = Nummer;
      break;
  }
  return;

}

void TouchAuslesen() {
  if (SetTouchAuswahl >= 1) {
    Auswahl = SetTouchAuswahl;                  // Communication ueber Seriell
    SetTouchAuswahl = 0;                        // Reset Auswahl ueber Communication
  }
  else {
    Auswahl = 0;                                // Fruehere Auswahl zurueck setzen
  }

  if (Touch.dataAvailable())                  // Nur reagieren wenn Touch gedrueckt wurde
  {
    int x;
    int y;

    Touch.read();
    x = Touch.getX();                        // X Koordinaten, wo am Bildschirm gedrÃ¼ckte wurde
    y = Touch.getY();                        // Y Koordinaten, wo am Bildschirm gedrÃ¼ckte wurde

    // Umrechnung auf Display Pixel
    int x1, y1;
    x1 = x * 1.5;
    y1 = y * 3.347;

    //Testausgaben
    //Serial.print("X ist: ");
    //Serial.println(x1);
    //Serial.print("Y ist: ");
    //Serial.println(y1);

    // Erstmal Auswahl 0 setzen und anschlieÃŸend Ã¼berschreiben
    if ((y1 >= 0) && (y1 <= 840))
      Auswahl = 0;

    // Abfrage Auswahl 1 - 8
    for (int i = 1, k = 100; k <= 700; i = i + 1, k = k + 75)
    {
      if ((y1 >= k) && (y1 <= k + 75))
      {
        Auswahl = i;
      }
    }

    // Abfrage Links, Home, Rechts
    if ((y1 >= 710) && (y1 <= DISPLAY_X_MAX))
    {
      if ((x1 >= 320) && (x1 <= DISPLAY_Y_MAX))
        Auswahl = 9;    // Links

      if ((x1 >= 160) && (x1 <= 320))
        Auswahl = 10;   // Home

      if ((x1 >=  DISPLAY_ZERO) && (x1 <= 160))
        Auswahl = 11;   // Rechts
    }

    // Abfrage Kopfzeile
    if ((y1 >= DISPLAY_ZERO) && (y1 <= 100))
      Auswahl = 12;

    // Testausgabe
    //LCD.print("Position: " + String(x1) + " " + String(y1)+String("     "),CENTER,120);
  }
}

void AuswertungBearbeiten() {
  // Aufruf ohne Eingabe vom Bediener
  if (PageNummer == 11)   // Zubereiten laeuft
  {
    if (FehlerBeimProduzieren == true)
    {
      Aufruf = 13;
    }
    else
    {
      Aufruf  = 12;
    }
  }


  if (PageNummer == 50)   // Einstellungen
    Aufruf = 40;           // Refresh Funktion


  if (Auswahl == 0)       // Keine Auswahl
    return;


  // Aufruf mit Eingabe vom Bediener
  if (PageNummer == 30)    // Home
  {
    if (Auswahl == 1)
      Aufruf = 100;       // Favoriten
    if (Auswahl == 2)
      Aufruf = 200;       // Beliebte
    if (Auswahl == 3)
      Aufruf = 300;       // Eigene
    if (Auswahl == 4)
      Aufruf = 40;        // Einstellungen
    if (Auswahl == 5)
      Aufruf = 6;         // Flaschen Vorne
    if (Auswahl == 6)
      Aufruf = 7;         // Flaschen Hinten
  }

  if (PageNummer == 40 && Auswahl <= 8)   // Einstellungen
  {
    if (Auswahl == 1)
      EEPROM.write(E_ADDR_FARBEN, 0);
    if (Auswahl == 2)
      EEPROM.write(E_ADDR_FARBEN, 1);
    if (Auswahl == 3)
      EEPROM.write(E_ADDR_FARBEN, 2);
    if (Auswahl == 4)
      ProduzierenErlaubt = 1;
    if (Auswahl == 5)
      ProduzierenErlaubt = 0;
    if (Auswahl == 6)
      LCD.setBrightness(4);
    if (Auswahl == 7)
      LCD.setBrightness(16);

    Aufruf = 50;
  }


  if (PageNummer == 12)   // Cocktail ist Fertig
    Aufruf  = 30;

  if (PageNummer == 13)   // Fehler bei der Erstellung
    Aufruf  = 30;

  if (PageNummer == 10)   // Start Bildschirm
    Aufruf = 30;

  // Cocktail Auswahl MenÃ¼
  if (PageNummer >= 100 && PageNummer <= 379)
  {
    if (Auswahl <= 8)   // Cocktail Seite = AusgewÃ¤hlter Cocktail + 300
      Aufruf = Auswahl + 300 + PageNummer;

    if (Auswahl == 9)   // Gehe eine Seite nach Links (solange MÃ¶glich)
      Aufruf = PageNummer - 20;

    if (Auswahl == 11) // Gehe eine Seite nach Rechts (solange MÃ¶glich)
      Aufruf = PageNummer + 20;
  }


  // Cocktail Unter Menue
  String sGerade = String(PageNummer);
  int iGerade;
  iGerade = !((sGerade.substring(1, 2)).toInt() % 2) ;     // Wenn 2te Ziffer Gerade dann True
  //Serial.println(String("TEST: ") + iGerade);

  if (PageNummer >= 400)
  {
    if (Auswahl == 7)      // Zubereiten Starten
    {
      if (iGerade)      // Cocktail Unter MenÃ¼ Erste Seite
        ZuProCock = PageNummer;

      if (!iGerade)    // Cocktail Unter MenÃ¼ Zweite Seite
        ZuProCock = PageNummer - 10;

      Aufruf = 11;    // Zubereiten
    }

    if (Auswahl == 9  && !iGerade)  // Gehe eine Seite nach Links (solange MÃ¶glich)
      Aufruf = PageNummer - 10;

    if (Auswahl == 11  && iGerade)  // Gehe eine Seite nach Rechts (solange MÃ¶glich)
      Aufruf = PageNummer + 10;
  }

  if (Auswahl == 10)  // Home
    Aufruf = 30;

  // Wichtige Testausgaben
  Serial.println(String("PageNummer: ") + String(PageNummer));
  Serial.println(String("Aufruf: ") + String(Aufruf));
  Serial.println(String("WAS DARF'S SEIN: ") + String(ZuProCock));
}


