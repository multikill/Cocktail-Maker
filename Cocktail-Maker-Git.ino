
////////////////////////////////////////////////////////////////////////////////////
//
//   Programm:            Cocktail Roboter (Display + Hardware)
//   Version:             5.0
//   Erstellungsdatum:    17.01.2017
//   Autor:               Adrian, Benedikt
//
////////////////////////////////////////////////////////////////////////////////////
//
//   Changelog:
//
//   Datum:                Beschreibung:
//   12.12.2016            Zusammenfuegen von Display und Hardware
//   21.12.2017            Erste Anlauefe
//   06.01.2018            Recode Main
//                         - PIN Liste eingefuegt
//                         - Globale Variable Reduziert
//                         - Lesbarkeit verbessert
//                         - Treiber Fehler und Reset fix
//                         - Startcolour eingefuegt
//                         - Hardware Start ohne Display integriert
//                         - Servo Motoren abschalten
//  13.01.2018             - Referenzfahrt X Motor kleines Stueck rechts
//                         - Dosierwartezeiten beim Dosier ueber Timenn[]
//                         - Dosierwartezeit gleiche Flasche und andere Flasche
//                         - Help Befehl eingebaut
//                         - Spannungsueberwachung eingebaut
//                         - Randomizer der Tellerdrehrichtung
//                         - Fehlerseite am Display einprogrammiert
//                         - Offset für Y-Achse eingestellt
//                         - Cocktailzaehler 2 Byte im EEPROM somit 4000 Cocktails moeglich
//                         - Luefter PWM eingestellt
//
//
////////////////////////////////////////////////////////////////////////////////////
//
//   ToDo Liste:
//
//   Status:                Beschreibung:
//
//   Planning:
//
//   Denied:                - Display Genauigkeit kann nicht durch Float erhöht werden (nur mit setPrecision())
//                          - SD-Karten Cocktail Speicherung
//                          - LED PWM expander sind teilweise defekt
//                          - Interrupt auskommentiert. Nicht zwingend noetig fuer das Projekt
//                            (Routine ist zu lang und bereitet generell Probleme)
//                          - Mehrere Farben der LEDs gleichzeitig ansteuern geht nicht wenn die Servo angesteuert werden!!!
//
////////////////////////////////////////////////////////////////////////////////////
//
//  Display Ansteuerung:
//  UTFT Libary
//  http://www.rinkydinkelectronics.com/library.php?id=51
//
//  Touch Ansteuerung:
//  URTouch Add-on Libary
//  http://www.rinkydinkelectronics.com/library.php?id=92
//
//  SD-Karten Ansteuerung: (nicht aktive)
//  UTFT_tinyFAT Add-on Libary
//  http://www.rinkydinkelectronics.com/library.php?id=53
//
//  Display Font:
//  SmallFont (eingebunden ueber UTFT Libary)
//  BigFont   (eingebunden ueber UTFT Libary)
//  Grotesk16x32
//  Grotesk24x48
//  http://www.rinkydinkelectronics.com/r_fonts.php
//
//  Motoren Ansteuerung:
//  Accelstepper Libary
//  http://www.airspayce.com/mikem/arduino/AccelStepper/index.html
//
//  Servo Motoren Ansteuerung:
//  Adafruit PWM Servo Driver Libary 
//  https://learn.adafruit.com/16-channel-pwm-servo-driver/using-the-adafruit-library
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////
///  Display, Bibliotheken einbinden   ///
//////////////////////////////////////////

#include <EEPROM.h>                     // Speichern von Einstellungen und Zaehler
#include <UTFT.h>                       // Ansteuerung vom Display         
#include <URTouch.h>                    // Toucheinehit Anteuerung Allgemein
#include <URTouchCD.h>                  // Spezielle Toucheinheit Unterstuetzung fuer 5" TFT
//#include <UTFT_tinyFAT.h>             // SD-Karte auf Display
//#include <tinyFAT.h>                  // SD-Karte auf Shield

extern uint8_t Grotesk24x48[];          // Schriftart Standard
extern uint8_t Grotesk16x32[];          // Schriftart fuer Unterpunkte
extern uint8_t BigFont[];               // Schriftart fuer Startbildschirm
extern uint8_t SmallFont[];             // Schriftart fuer Starrtbildschirm

//////////////////////////////////////////
///  Hardware, Bibliotheken einbinden  ///
//////////////////////////////////////////

#include <AccelStepper.h>               // Schrittmotoren Ansteuerung( fuer Referenzfahrt)
#include <MultiStepper.h>               // Gleichzeitige Ansteuerung von Schrittmotoren
#include <Wire.h>                       // Kommunikation Ueber I2C Schnittstelle ermoeglichen
#include <Adafruit_PWMServoDriver.h>    // Kommunikation mit dem PWM Expander fuer die Servomotoren

//////////////////////////////////////////
///  PIN und Variablen Belegung        ///
//////////////////////////////////////////

//#define PIN_COM_RX        0           // Com      Input   I2C               Initialisierung ueber Wire.h
//#define PIN_COM_TX        1           // Com      Output  I2C               Initialisierung ueber Wire.h

#define   PIN_TOUCH_1       2           // PWM      Output  Ansteuerung       Touch
#define   PIN_TOUCH_2       3           // PWM      Output  Ansteuerung       Touch
#define   PIN_TOUCH_3       4           // PWM      Output  Ansteuerung       Touch
#define   PIN_TOUCH_4       5           // PWM      Output  Ansteuerung       Touch
#define   PIN_TOUCH_5       6           // PWM      Output  Ansteuerung       Touch
//#define PIN_TOUCH_LCD     7           // PWM      Output  Ansteuerung       Touchdisplay
#define   PIN_MOTOR_STEP_Y  8           // PWM      Output  Motor Step        Schritt Y_Achse
#define   PIN_MOTOR_STEP_X  9           // PWM      Output  Motor Step        Schritt X_Achse
#define   PIN_LED_STATUS    10          // PWM      Output  Ansteuerung       Led Status
#define   PIN_LED_R         11          // PWM      Output  Ansteuerung       Led Farbe Rot
#define   PIN_LED_B         12          // PWM      Output  Ansteuerung       Led Farbe Blau
#define   PIN_LED_G         13          // PWM      Output  Ansteuerung       Led Farbe Gruen

//#define PIN_TOUCH_LCD     14-21       // Com      Output  Ansteuerung       Touchdisplay
//#define PIN_TOUCH_LCD     22-37       // Digital  Output  Ansteuerung       Touchdisplay
#define   PIN_LCD_1         38          // Digital  Output  Ansteuerung       Display
#define   PIN_LCD_2         39          // Digital  Output  Ansteuerung       Display
#define   PIN_LCD_3         40          // Digital  Output  Ansteuerung       Display
#define   PIN_LCD_4         41          // Digital  Output  Ansteuerung       Display
#define   PIN_EXP_EIN       42          // Digital  Output  PWM Expander      Einschalten
#define   PIN_MOTOR_DIR_Y   43          // Digital  Output  Motor Direction   Richtung Y_Achse
//#define PIN_TOUCH_LCD     44-45       // Digital  Output  Ansteuerung       Touchdisplay
#define   PIN_MOTOR_DIR_X   46          // Digital  Output  Motor Direction   Richtung X_Achse
//#define PIN_TOUCH_LCD     47          // Digital  Output  Ansteuerung       Touchdisplay
#define   PIN_MOTOR_EIN     48          // Digital  Output  Signal            Motoren Ein schalten
#define   PIN_TREIBER_RESET 49          // Digital  Output  Signal Treiber    3. Fehlermeldung vom Treiber zurueksetzen
//#define PIN_TOUCH_LCD     50-53       // Digital  Output  Ansteuerung       Touchdisplay

//#define PIN               A0          // Analog   Input   Defekt
#define   PIN_X_ACHSE       A1          // Analog   Input   Naeherungssensor  X_Achse
#define   PIN_Y_ACHSE       A2          // Analog   Input   Naeherungssensor  Y_Achse
#define   PIN_DIAG_24V      A3          // Analog   Input   Diagnose          VCC 24V Ueberwachung
#define   PIN_DIAG_6V       A4          // Analog   Input   Diagnose          6V Ueberwachung
#define   PIN_DIAG_12V      A5          // Analog   Input   Diagnose          12V Ueberwachung
#define   PIN_TREIBER_FAIL  A6          // Analog   Input   Signal Treiber    1. Fehlermeldung vom Treiber erkennen
#define   PIN_MOTOR_RESET   A7          // Analog   Input   Signal(Taster)    2. Fehlermeldung vom Benutzer reseten
#define   PIN_START         A8          // Analog   Input   Taster            Starten ohne Display

#define   PIN_RANDOM        A9          // Analog   Input   Randomize         Drehtellerrichtung
#define   PIN_GLAS          A10         // Analog   Input   Infrarot Sensor   Glas erkennung
//#define PIN               A11         // Analog   Input
//#define PIN               A12         // Analog   Input
//#define PIN               A13         // Analog   Input
//#define PIN               A14         // Analog   Input
//#define PIN               A15         // Analog   Input


#define   PIN_EXP_SERVO1    0           // PWM Exp  Output  Servomotor        Flasche 0 und 5
#define   PIN_EXP_SERVO2    1           // PWM Exp  Output  Servomotor        Flasche 1 und 6
#define   PIN_EXP_SERVO3    2           // PWM Exp  Output  Servomotor        Flasche 2 und 7
#define   PIN_EXP_SERVO4    3           // PWM Exp  Output  Servomotor        Flasche 3 und 8
#define   PIN_EXP_SERVO5    4           // PWM Exp  Output  Servomotor        Flasche 4 und 9
//#define PIN_EXP_          5           // PWM Exp  Output
//#define PIN_EXP_          6           // PWM Exp  Output
//#define PIN_EXP_          7           // PWM Exp  Output
//#define PIN_EXP_          8           // PWM Exp  Output
#define   PIN_EXP_LUEFTER   9           // PWM Exp  Output  Luefter           12V 2 Stueck
#define   PIN_EXP_LED_BLAU  10          // PWM Exp  Output
#define   PIN_EXP_LED_GRUEN 11          // PWM Exp  Output
#define   PIN_EXP_LED_ROT   12          // PWM Exp  Output
//#define PIN_EXP_          13          // PWM Exp  Output
//#define PIN_EXP_          14          // PWM Exp  Output
//#define PIN_EXP_          15          // PWM Exp  Output
//#define PIN_EXP_          16          // PWM Exp  Output

#define   ANALOG_LOW        100
#define   ANALOG_HIGH       900
#define   DISPLAY_ZERO      0           // Touchdisplay Startposition
#define   DISPLAY_X_MAX     800         // Touchdisplay Querformat X Max
#define   DISPLAY_Y_MAX     480         // Touchdisplay Querformat Y Max
#define   DOSIER_AFTERTIME  200         // Wartezeit nach dem Dosieren andere Flasche
#define   DOSIER_SAMEBOTTLE 2000        // Wartezeit nach dem Dsieren gleiche Flasche

#define   E_ADDR_FARBEN     2           // EEPROM Addresse fuer Farben Einstellung
#define   E_ADDR_MOTOR_X    3           // EEPROM Addresse fuer Motor X Max Speed
#define   E_ADDR_MOTOR_Y    4           // EEPROM Addresse fuer Motor X Max Speed
#define   E_ADDR_ZAEHLER    10          // EEPROM Addresse fuer Cocktail Zaehler

//#define   SERVO_AL          340         // SERVO Motor Anschlag Links (von 0 bis 4096)
//#define   SERVO_AR          200         // SERVO Motor Anschlag Rechts
//#define   SERVO_MITTE       270         // SERVO Motor Mitte (Grundstellung)

#define   SERVO_AL          300         // SERVO Motor Anschlag Links (von 0 bis 4096)
#define   SERVO_AR          160         // SERVO Motor Anschlag Rechts
#define   SERVO_MITTE       230         // SERVO Motor Mitte (Grundstellung)

#define   MOTOR_Y_START     0           // Position Glas Start
#define   MOTOR_Y_VORNE     10          // Position Glas Vorne // + Offset nach Links // - Offset nach Rechts
#define   MOTOR_Y_HINTEN    500         // Position Glas Hinten

////////////////////////////////////////////////////////////////////////////////////
/// Display  Ansteuerung  INITALISIEREN                                         ////
////////////////////////////////////////////////////////////////////////////////////

UTFT LCD(CTE50, PIN_LCD_1, PIN_LCD_2, PIN_LCD_3, PIN_LCD_4);                        // SSD1963  (16bit) 800x480
URTouch  Touch( PIN_TOUCH_5, PIN_TOUCH_4, PIN_TOUCH_3, PIN_TOUCH_2, PIN_TOUCH_1);   // PWM Pins fuer Daten
//UTFT_tinyFAT myFiles(&LCD);                                                       // SD-Karten Zugriff

////////////////////////////////////////////////////////////////////////////////////
/// Initialisieren Allgemeine Globale Variablen                                 ////
////////////////////////////////////////////////////////////////////////////////////

int PageNummer = 0;                                                               // Gibt die aktuelle Seite an.  Setzte 0 damit StartPage 1 geladen wird.
int Aufruf = 0;                                                                   // Gibt die aufzurufende Seite an. Setzte 0 fuer Home Screen
int Auswahl = 0;                                                                  // Gibt die Auswahl Nummer auf der aktuellen Seite an.
int ZuProCock = 0;                                                                // Gibt die Nummer des Zu Produzierenden Cocktail an.
int SetTouchAuswahl = 0;                                                          // Setzte die Touchauswahl ueber Communication

bool FehlerBeimProduzieren = false;                                               // Hier wird festgestellt ob ein Fehler aufgetreten ist
bool ProduzierenErlaubt = true;                                                   // Allgemeine Freigabe beim Cocktail erstellen
bool MotorFehler = false;                                                         // Ist ein Treiber Fehler vorhanden?
// Treiber Fehler erkennen mit        PIN_TREIBER_FAIL
// Benutzer Fehler Zuruecksetzten mit PIN_MOTOR_FAIL
// Treiber Fehler zuruecksetzen mit   PIN_TREIBER_RESET

int Zutatenn[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 20};                              // Standard Zutaten und und Reihenfolge (Wird vom Cocktail ueberschrieben)
int Timenn[]   = {3000, 3000, 3000, 3000, 3000,
                  3000, 3000, 3000, 3000, 3000 };                                 // Standard Dosierzeiten und ihre Reihenfolge

String CocktailNamen[9] = {};                                                     // Die Ausgabewerte am Bildschirm. Ueberschrift und Unterpunkte
//String CocktailNamen[9]={"Touchdown", "Cuba Libre", "Gin Fizz", "Wodka Energie", "Sex on the Beach", "Cocktail mit ganz langem Wort", "Spezial", "Tequilla Sunrise"};

long positions[2];                                                                // Array fuer Positionen der X und Y Motoren

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


////////////////////////////////////////////////////////////////////////////////////
/// Initialisieren Globale Namen Variablen                                      ////
////////////////////////////////////////////////////////////////////////////////////

const int NUMBER_OF_ELEMENTS = 803;
const int MAX_SIZE = 30;

const char descriptions [NUMBER_OF_ELEMENTS] [MAX_SIZE] PROGMEM = {

  //  0 -  9 Flaschen Namen
  {"Ginger Ale"}, {"Coooola"}, {"Vodka Gorwi"}, {"Sprite"}, {"Havana Club"}, {"Coke"}, {"Hugo"}, {"Soda"}, {"Jin Tonic"}, {"Grenadine"},

  // 10- 19 Allgemeine Namen
  {"Mischen"}, {}, {}, {}, {}, {}, {}, {}, {}, {},

  // 20 - 29
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},

  // 30 - 39 Home Screen
  {"Home"}, {"Favoriten"}, {"Beliebte"}, {"Eigene"}, {"Einstellungen"}, {"Flaschen Vorne"}, {"Flaschen Hinten"}, {}, {}, {},

  // 40 - 49 Einstellungen
  {"Einstellungen"}, {"Farbekombination 1"}, {"Farbekombination 2"}, {"Farbekombination 3"}, {"Produzieren EIN"}, {"Produzieren AUS"}, {"Helligkeit Hoch"}, {"Helligkeit Niedrig"}, {"Refresh"}, {},

  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},


  //100
  {"1 Favoriten"}, {"Drachenblut"}, {"Moscov Mule"}, {"Havana Cola"}, {"Havana Sprite"}, {"Graffl"}, {"Tami"}, {"Mexicaner"}, {"Pisco Sour"}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {"2 Favoriten"}, {"Drachenblut"}, {"Orangen Delight"}, {"Goldener Herbst"}, {"Sau Zeig"}, {"Graffl"}, {"Double Apple Smash"}, {"Mexicaner"}, {"Pisco Sour"}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {"3 Favoriten"}, {"Drachenblut"}, {"Orangen Delight"}, {"Goldener Herbst"}, {"Sau Zeig"}, {"Graffl"}, {"Double Apple Smash"}, {"Mexicaner"}, {"Pisco Sour"}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},

  //200
  {"1 Beliebte"}, {"Kamikaze"}, {"Halt die Flasche"}, {"Ex die Molen"}, {"Goenn dir hart"}, {"Graffl"}, {"Weinbrand Cola"}, {"Berenzen"}, {"Wisky on Ice"}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {"2 Beliebte"}, {"Kamikaze X"}, {"Halt die Flasche"}, {"Ex die Molen"}, {"Goenn dir hart"}, {"Graffl"}, {"Weinbrand Cola"}, {"Berenzen"}, {"Wisky on Ice"}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {"3 Beliebte"}, {"Kamikaze"}, {"Halt die Flasche"}, {"Ex die Molen"}, {"Goenn dir hart"}, {"Graffl"}, {"Weinbrand Cola"}, {"Berenzen"}, {"Wisky on Ice"}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},

  //300
  {"1 Eigene"}, {"Der Zerstoerer 1"}, {"Einfach mal was anderes"}, {"Wodka Hart"}, {"WIESO NUR??"}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {"2 Eigene"}, {"Der Zerstoerer 1"}, {"Einfach mal was anderes"}, {"Wodka Hart"}, {"WIESO NUR??"}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},

  //400
  {}, {"Drachenblut Page 1"}, {"Moscov Mule"}, {"Havana Cola"}, {"Havana Sprite"}, {}, {}, {}, {}, {},
  {}, {"Drachenblut Page 2"}, {"Moscov Mule"}, {"Havana Cola"}, {"Havana Sprite"}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},   {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},   {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},

  //500
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {"KAMIKAZE"}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {"KMAIKAZE 2"}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},   {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},   {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},

  //600
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},   {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},   {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},   {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},

  //700
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},   {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},   {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},   {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
  {}, {}, {}, {}, {}, {}, {}, {}, {}, {},

  // 800
  {"Test1"},
  {"Test2"},
  {"Test3"},
};


////////////////////////////////////////////////////////////////////////////////////
/// Initialisieren Motoren                                                      ////
////////////////////////////////////////////////////////////////////////////////////

AccelStepper stepperX(1, PIN_MOTOR_STEP_X, PIN_MOTOR_DIR_X);                      // X_Achse InterfaceTyp: Driver, Step, Direction
AccelStepper stepperY(1, PIN_MOTOR_STEP_Y, PIN_MOTOR_DIR_Y);                      // Y_Achse InterfaceTyp: Driver, Step, Direction
MultiStepper steppers;                                                            // X und Y Achse


////////////////////////////////////////////////////////////////////////////////////
/// Initialisieren PWM Expander                                                 ////
////////////////////////////////////////////////////////////////////////////////////

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();                          //I2C Adresse 0x40


////////////////////////////////////////////////////////////////////////////////////
/// Ardunio void setup()                                                        ////
////////////////////////////////////////////////////////////////////////////////////
void setup()
{

  ////////////////////////////////////////////////
  ///  1. Allgemein Setup                      ///
  ////////////////////////////////////////////////

  Serial.begin(115200);                       // Baudrate zur Kommunikation
  Serial.println("1. SETUP BEGIN");         // Kommuniziere Setup Schritt
  inputString.reserve(200);                 // Reserviere 200 Bytes fuer inputString

  Serial1.begin(115200);                    // Baudrate für ESP
//  Serial1.begin("Hallo ESP");               // Kommuniziere zum ESP

  ////////////////////////////////////////////////
  ///  2. PIN Mode Setup                       ///
  ////////////////////////////////////////////////

  Serial.println("2. SETUP Pin Mode");        // Kommuniziere Setup Schritt

  pinMode(PIN_MOTOR_EIN,    OUTPUT);          // Motoren Ein/Aus schalten
  pinMode(PIN_TREIBER_RESET, OUTPUT);         // 3. Fehlermeldung vom Treiber Reseten mit HIGH Signal
  pinMode(PIN_LED_STATUS,   OUTPUT);          // LED Status Output
  pinMode(PIN_LED_R,        OUTPUT);          // LED Farbe Rot
  pinMode(PIN_LED_B,        OUTPUT);          // LED Farbe Blau
  pinMode(PIN_LED_G,        OUTPUT);          // LED Farbe Gruen

  pinMode(PIN_EXP_EIN,      OUTPUT);          // PWM Expander Einschalten
  pinMode(PIN_MOTOR_EIN,    OUTPUT);          // Motoren Einschalten
  pinMode(PIN_X_ACHSE,      INPUT);           // Endlage X_Achse
  pinMode(PIN_Y_ACHSE,      INPUT);           // Endlage Y_Achse
  pinMode(PIN_DIAG_24V,     INPUT);           // Diagnose 24V Ueberwachung
  pinMode(PIN_DIAG_6V,      INPUT);           // Diagnose  6V Ueberwachung
  pinMode(PIN_DIAG_12V,     INPUT);           // Diagnose 12V Ueberwachung
  pinMode(PIN_TREIBER_FAIL, INPUT);           // 1. Fehlermeldung vom Treiber erknnen mit LOW Signal
  pinMode(PIN_MOTOR_RESET,  INPUT_PULLUP);    // 2. Fehlermeldung Reset vom Benutzer ekennen mit HIGH Signal
  pinMode(PIN_START,        INPUT_PULLUP);    // Start Cocktail ohne Display

  pinMode(PIN_RANDOM,       INPUT);           // Randomize Drehteller
  pinMode(PIN_GLAS,         INPUT);           // Glas Erkennung


  ////////////////////////////////////////////////
  ///  3. Touchdisplay Setup                   ///
  ////////////////////////////////////////////////

  Serial.println("3. SETUP Touchdisplay");    // Kommuniziere Setup Schritt

  LCD.InitLCD(PORTRAIT);                      // Hochformat
  LCD.clrScr();                               // Bildschirm loeschen
  Touch.InitTouch();                          // Touch initalisieren
  Touch.setPrecision(PREC_HI);                // Genauigkeit festlegen
  // PREC_LOW, PREC_MEDIUM, PREC_HI, PREC_EXTREME

  Serial.print("  - Display Size X: ");       // Sollte 800 Px sein
  Serial.println(LCD.getDisplayXSize());
  Serial.print("  - Display Size Y: ");       // Sollte 480 Px sein
  Serial.println(LCD.getDisplayYSize());

  //LCD.fillScr(VGA_WHITE);                   // Testausgabe
  LCD.setBrightness(16);                      // Helligkeit (wird nichts unterstuetzt)
  LCD.setContrast(64);                        // Kontrast   (wird nichts unsterstuezt)

  //file.initFAT();                           // SD-Karte initalisieren

  LadeScreen(10);                             // Start bildschirm

  ////////////////////////////////////////////////
  ///  4. Setup  Servo Motoren                 ///
  ////////////////////////////////////////////////

  Serial.println("4. SETUP Servo Motoren");  // Kommuniziere Setup Schritt

  ServoEinSchalten(1);                      // Servo Motoren Einschalten

  pwm.begin();                              // Initalisierung. Wenn er hier stehen bleibt ist der PWM Expander Chip defekt!
  Serial.println("  - I2C Kommunikation ist in Ordnung!");

  pwm.setPWMFreq(51);                       // 51 fuer 50Hz NEUE SERVO
  yield();                                  // Kommunikation zum PWM Expander
  VentilZu();                               // Ventile in Grundstellung
  delay(100);
  ServoEinSchalten(0);                      // Servo Motoren Ausschalten


  ////////////////////////////////////////////////
  ///  5. Setup Schritt Motoren                ///
  ////////////////////////////////////////////////

  Serial.println("5. SETUP Schritt Motoren"); // Kommuniziere Setup Schritt

  Serial.print("  - Digital Wert PIN_TREIBER_FAIL (Low fuer Fehler):");
  Serial.println(String(analogRead(PIN_TREIBER_FAIL)));

  Serial.print("  - Analog  Wert PIN_MOTOR_RESET  (High fuer Reset): ");
  Serial.println(String(analogRead(PIN_MOTOR_RESET)));

  digitalWrite(PIN_TREIBER_RESET, HIGH);      // Treiber Reset (LOW Active)

  steppers.addStepper(stepperX);              // Stepper X hinzufuegen
  steppers.addStepper(stepperY);              // Stepper Y hinzufuegen

  MotorInitalisieren();                       // Geschwindigkeit und SPeed einstellen
  MotorenEinSchalten(0);                      // Motoren ausschalten
  if (!Grundstellung()) {
    Referenzfahrt();
  }

  ////////////////////////////////////////////////
  ///  6. Setup Diverses                       ///
  ////////////////////////////////////////////////

  Serial.println("6. SETUP Diverses");        // Kommuniziere Setup Schritt

  //pinMode(18, INPUT_PULLUP);                // Interrupt Unterbrechung
  //attachInterrupt(digitalPinToInterrupt(18), Unterbrech, LOW);

  //eepromWriteInt(E_ADDR_ZAEHLER, 300);      // EEPROM Cocktail Zaehler Reset

  Serial.println("  - LED GRUEN Gruen");
  setLED(0, 0, 255);                          // LED Farbe auf Gruen setzen

  analogWrite(PIN_LED_STATUS, 0);

  ServoEinSchalten(1);
  delay(100);
  SpannungAuslesen();
  ServoEinSchalten(0);

  ////////////////////////////////////////////////
  ///  7. Setup Glas Sensor                    ///
  ////////////////////////////////////////////////

  Serial.println("7. SETUP Glas");            // Kommuniziere Setup Schritt

  Serial.println("  - Analog Wert: " +        // Gib Analog Wert aus
    String(analogRead(PIN_GLAS)));

  if (analogRead (PIN_GLAS) < ANALOG_LOW) {   // Check ob das Glas vorhanden ist
    Serial.println("  - Glas vorhanden");
  }
  else {
    Serial.println("  - Glas nicht vorhanden");
  }

  ////////////////////////////////////////////////
  ///  8. Setup Ende                           ///
  ////////////////////////////////////////////////

  Serial.println("8. SETUP Ende");           // Kommuniziere Setup Schritt
  Serial.println("");

}


////////////////////////////////////////////////////////////////////////////////////
/// Ardunio void loop()                                                         ////
////////////////////////////////////////////////////////////////////////////////////
void loop() {
  ////////////////////////////////////////////////
  ///  Loop mit Display                        ///
  ////////////////////////////////////////////////

  if (true) {

    //EntwicklerModus();                      // Anzeige der Koordinaten vom Touchscreen
    LadeScreen(Aufruf);                       // Seite laden
    TouchAuslesen();                          // Touch Werte auslesen
    AuswertungBearbeiten();                   // Ausgelesene Werte bearbeiten
  }

  ////////////////////////////////////////////////
  ///  Loop Hardware Ansteuerung allein        ///
  ////////////////////////////////////////////////

  if (false) {

    if (analogRead(PIN_START) < ANALOG_LOW) { // Startbefehl

      Serial.println("Loop Hardware Test");   // Kommuniziere Loop Schritt
      ZutatenLaden(0, 1, 2, 3, 4, 5, 6, 7, 8, 9); // Reihenfolge der Anfahrt
      Zubereiten();                           // Hardware beginnen
      delay(1000);                            // Zusaetzliche Wartezeit
    }
  }

  ////////////////////////////////////////////////
  ///  Loop Communication                      ///
  ////////////////////////////////////////////////


  if (true) {
    if (stringComplete) {
      checkMystring();
      inputString = "";                       // Loesche den String
      stringComplete = false;
    }
  }
  
//  // read from port 1, send to port 0:
//  if (Serial1.available()) {
//    int inByte = Serial1.read();
//    Serial.write(inByte);    
//  }

}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void serialEvent1() {
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

