
#include <SPI.h> //librairie du MCP42100 
#include <Adafruit_SSD1306.h> //librairie de l'ecran OLED
#define encoder0PinA  2  //CLK de l'encodeur
#define encoder0PinB  4  //DT de l'encodeur
#define Switch 5 // bouton de l'encodeur

const int CaptPin = A0; // Pin de lecture de la valeur du capteur

// Parametrage capteur industriel
const int flexPin = A1;      // Pin du capteur industriel
const float VCC = 5;      
const float R_DIV = 10000.0;  // resistance du diviseur de tension
const float flatResistance = 30000.0; // resistance du capteur industriel quand il est plat
const float bendResistance = 1000.0;  // resistance du capteur industriel quand il est à 90 deg


volatile int PosiPot = 0; //regle la valeur initial de l'encodeur
int potPos = 0;

bool valider = false;

bool Pot = false;

bool blue = true;

int Position = 1;

char bullshit[16];

const byte csPin           = 10;      // Pin pour regler la valeur du potentiometre MCP42100 
const int  maxPositions    = 256;     // défini les valeurs max pouvant etre inscrite sur le potentiometre
const long rAB             = 52200;   // 100k pot resistance between terminals A and B, 
                                      // mais pour ajuster au multimètre, je mets 92500
const byte rWiper          = 125;     // 125 ohms pot wiper resistance
const byte pot0            = 0x11;    // pot0 addr // B 0001 0001
const byte pot0Shutdown    = 0x21;    // pot0 shutdown // B 0010 0001

//parametrage de l'ecran OLED
#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

void setPotWiper(int addr, int pos) {

  ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
  ecranOLED.setCursor(0, 0);

  pos = constrain(pos, 0, 255);            // limite les parametre du potentiometre entre 0 et 255
  digitalWrite(csPin, LOW);                // Activation de l'ecriture
  SPI.transfer(addr);                      // positionne le potentiometre à la valeur defini
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // Desactivation de l'ecriture

  // ecrit la resistance du potentiometre resistance utiliser par notre circuit
  long resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
  ecranOLED.setTextColor(SSD1306_WHITE); //Calcul de la valeur de la resistance correspondant à la position     
  ecranOLED.print("Position: ");
  ecranOLED.println(pos);
  ecranOLED.println(" Resistance correspondante: ");
  ecranOLED.print(resistanceWB);
  ecranOLED.println(F(" ohms"));
  ecranOLED.print(analogRead(CaptPin)*(5.0/1023.0)*1000.0);
  ecranOLED.println(" mV");
  ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  ecranOLED.println("Retour");

}

void setup() { 
  Serial.begin (9600); // defini le Baud rate de l'arduina pour la lecture
  
  pinMode(encoder0PinA, INPUT); // Configuration de la PIN CLK de l'encodeur pour permetre la lecture
  digitalWrite(encoder0PinA, HIGH);

  pinMode(encoder0PinB, INPUT); // Configuration de la PIN DT de l'encodeur pour permetre la lecture
  digitalWrite(encoder0PinB, HIGH); 

  digitalWrite(Switch, HIGH);         //Permet de traiter le switch comme un vrai bouton avec une résistance pull Up
  
  attachInterrupt(0,Encoder, RISING); //Creer une interruption pour activer la lecture de la postion de l'encodeur. Pourrait etre remplacer par une fonction de detection de front montant mais cela alourdi le code de manière inutile
  
  digitalWrite(csPin, HIGH);        // Desactivation par defaut de l'écriture du potentiometre
  pinMode(csPin, OUTPUT);           // Configure la Pin du potentiometre en mode ecriture
  SPI.begin(); // librairie permettant de manipuler les adresse du potentiometre. Pourrait surement être alléger mais cela necessiterait de regarder plus en détaille le MCP42100 
  ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED);

  pinMode(flexPin, INPUT);
} 

void loop(){
  
  Control_OLED();
  Bouton();
}

void Encoder() {
  if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==HIGH) {
    
    if (Pot){
    PosiPot++;
    }
    else if(!valider){
    Position++;
    }  
      
  } else if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==LOW) {
    if (Pot){
    PosiPot--;
    } 
    else if(!valider){
      Position--;      
    }     
  }
  //Permet de faire boucler les valeurs entre 4 et 1. Cela permet de faire un menu déroulant
  if(Position > 4){
    Position = 1;
  }else if(Position < 1){
    Position = 4;
  }

  if(PosiPot >= 256){
    PosiPot = 0;
  }else if(PosiPot < 0){
    PosiPot = 255;
  }
  Pot = false;
}

void Bouton() {
  if (digitalRead(Switch)==LOW) {
    valider=!valider;
    delay(500);
    Serial.println(valider);
  }
}


void Menu_OLED(){
  //initialisation ecran
    ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
    ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
    ecranOLED.setCursor(0, 0);  
    switch(Position){
      case 1: 
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      ecranOLED.println(F("Graphite "));
      ecranOLED.setTextColor(SSD1306_WHITE);
      ecranOLED.println(F("Industriel "));
      ecranOLED.println(F("Bluetooth "));
      ecranOLED.println(F("Potentio "));
      break;
      case 2:
      ecranOLED.setTextColor(SSD1306_WHITE);
      ecranOLED.println(F("Graphite "));      
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      ecranOLED.println(F("Industriel "));
      ecranOLED.setTextColor(SSD1306_WHITE);
      ecranOLED.println(F("Bluetooth "));
      ecranOLED.println(F("Potentio "));
      break;
      case 3:
      ecranOLED.setTextColor(SSD1306_WHITE);
      ecranOLED.println(F("Graphite "));
      ecranOLED.println(F("Industriel "));
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      ecranOLED.println(F("Bluetooth "));
      ecranOLED.setTextColor(SSD1306_WHITE);
      ecranOLED.println(F("Potentio "));
      break;
      case 4:
      ecranOLED.setTextColor(SSD1306_WHITE);
      ecranOLED.println(F("Graphite "));
      ecranOLED.println(F("Industriel "));
      ecranOLED.println(F("Bluetooth "));
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      ecranOLED.println(F("Potentio "));
      break;
    }                               
    ecranOLED.display();                            // Transfert le buffer à l'écran
    delay(200);
  }


void Graph_OLED(){
    setPotWiper(pot0, PosiPot);
    Serial.println(analogRead(CaptPin));
    ecranOLED.display();
}

void Flex_OLED(){
  ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
  ecranOLED.setCursor(0, 0);
   int ADCflex = analogRead(flexPin);
  float Vflex = ADCflex * VCC / 1023.0;
  float Rflex = R_DIV * (VCC / Vflex - 1.0);
  Serial.println("Resistance: " + String(Rflex) + " ohms");

  // Use the calculated resistance to estimate the sensor's bend angle:
  float angle = map(Rflex, flatResistance, bendResistance, 0, 90.0);
  ecranOLED.setTextColor(SSD1306_WHITE);  
  ecranOLED.println("Bend: " + String(angle) + " degrees");
  ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  ecranOLED.println("Retour");
  ecranOLED.display();

  delay(500);
}

void Blue_OLED(){
  ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE);  
  if(blue){
    ecranOLED.println("BLUETOOTH ON");
  }
  else{
    ecranOLED.println("BLUETOOTH OFF");
  }

  ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  ecranOLED.println("Retour");
  ecranOLED.display();
}

void Pot_OLED(int addr, int pos){
  Pot = true;
  ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
  ecranOLED.setCursor(0, 0);

  PosiPot = constrain(PosiPot, 0, 255);            // limite les parametre du potentiometre entre 0 et 255
  digitalWrite(csPin, LOW);                // Activation de l'ecriture
  SPI.transfer(addr);                      // positionne le potentiometre à la valeur defini
  SPI.transfer(PosiPot);
  digitalWrite(csPin, HIGH);               // Desactivation de l'ecriture

  // ecrit la resistance du potentiometre resistance utiliser par notre circuit
  long resistanceWB = ((rAB * PosiPot) / maxPositions ) + rWiper;
  ecranOLED.setTextColor(SSD1306_WHITE); //Calcul de la valeur de la resistance correspondant à la position     
  ecranOLED.print("Position: ");
  ecranOLED.println(PosiPot);
  ecranOLED.println(" Resistance correspondante: ");
  ecranOLED.print(resistanceWB);
  ecranOLED.println(" ohms");
  ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  ecranOLED.println("Retour");
  ecranOLED.display();
  

}

void Control_OLED(){
  ecranOLED.clearDisplay(); 
if (valider){
  switch(Position){
    case 1:
  Graph_OLED();
  break;
  case 2:
  Flex_OLED();
  break;
  case 3:
  Blue_OLED();
  break;
  case 4:
  Pot_OLED(pot0, potPos);
  break;
  }
  }
  else{
    Menu_OLED();
  }
}

