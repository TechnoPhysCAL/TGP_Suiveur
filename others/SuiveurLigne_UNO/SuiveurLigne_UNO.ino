/*
  Nom du programme : SuiveurLigne_UNO.ino
  Auteur : Richard Milette / Claude Bouchard
  Version : 2.0.0
  Plateforme : Arduino Uno
  Date : mars 2021

  Description :  Ce programme permet de lire six capteurs de réflexion lumineuse
  d'un suiveur de ligne. Ce programme est conçu pour être sur la carte Suiveur_de_Ligne
  et communiquer via le SPI.
*/

//Appel de la bibliothèque SPI
#include <SPI.h>
//Appel de la bibliothèque pour lire les photodétecteurs du module de Pololu
#include <QTRSensors.h>

#define NOMBRE_CAPTEURS 6
#define  RX_BUF_SIZE 12

//Définition des broches pour chacun des capteurs
#define BROCHE_CAPT0  A0
#define BROCHE_CAPT1  A1
#define BROCHE_CAPT2  A2
#define BROCHE_CAPT3  A3
#define BROCHE_CAPT4  A4
#define BROCHE_CAPT5  A5

//Indique la broche utilisée pour activer tous les photodétecteurs
#define PHOTO_EN  9
//Définition des broches de l'interface SPI (carte Arduino UNO)
#define BROCHE_SCK  13
#define BROCHE_MOSI 11
#define BROCHE_MISO  12
#define BROCHE_CS  10
//Définition des broches pour les interrupteurs et le voyant lumineux
#define BROCHE_DEL  6

//Variable pour l'état du voyant lumineux
boolean etatDEL = LOW;

//Déclare une variable qtr du type QTRsensors
QTRSensors qtr;


//Déclaration d'un tableau pour lire l'état de chacun des photodétecteur
uint16_t valeursCapteurs[NOMBRE_CAPTEURS];
//Tableau pour envoyer les données
uint8_t donnees[NOMBRE_CAPTEURS * 2];
//Variables pour recevoir des données non nulle du maître SPI
volatile int buffer_pos = 0;


//Variable et constante pour le délai d'acquisition des photodétecteurs
unsigned long minuteriePhoto = millis();
const unsigned long DELAI_PHOTO = 20;

//Variable et constante pour le délai d'affichage en mode debug
unsigned long minuterieAffichage = millis();
const unsigned long DELAI_AFFICHAGE = 1000;

void setup() {
  //Configure le port série à 115200 bits par seconde
  Serial.begin(115200);
  //Affiche le nom du programme au démarrage
  Serial.println("Programme du suiveur de ligne. Version 2.0.0");
  //Initialise les photodétecteurs du module de Pololu
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]) {
    BROCHE_CAPT0, BROCHE_CAPT1, BROCHE_CAPT2, BROCHE_CAPT3, BROCHE_CAPT4, BROCHE_CAPT5
  }, NOMBRE_CAPTEURS);
  qtr.setEmitterPin(PHOTO_EN);

  //Initialise les broches du bus SPI
  pinMode(BROCHE_SCK, INPUT);
  pinMode(BROCHE_MOSI, INPUT);
  pinMode(BROCHE_MISO, OUTPUT);
  pinMode(BROCHE_CS, INPUT);
  /*Configure le bus SPI comme suit:
    - Permet les interruptions
    - Active le bus SPI
    - Transmet le MSB en premier
    - Configure le bus SPI comme esclave
    - CPOL = 0 et CPHA = 0
    - freq = 4 MHz
  */
  SPCR = 0b11000000;
  //Signal pour le voyant lumineux
  pinMode(BROCHE_DEL, OUTPUT);
  digitalWrite(BROCHE_DEL, etatDEL);
}

void loop() {
  if (millis() - minuteriePhoto > DELAI_PHOTO) {
    minuteriePhoto = millis();
    qtr.read(valeursCapteurs);

    for (int i = 0; i < NOMBRE_CAPTEURS; i++) {
      donnees[i * 2] = (valeursCapteurs[i] & 0xFF00) >> 8;
      donnees[i * 2 + 1] = valeursCapteurs[i] & 0x00FF;
    }
  }

  if (millis() - minuterieAffichage > DELAI_AFFICHAGE) {
    minuterieAffichage = millis();
    Serial.print("Valeurs : ");
    for (int i = 0; i < NOMBRE_CAPTEURS; i++) {
      Serial.print(valeursCapteurs[i]);
      Serial.print(" ");
    }
    Serial.println("");
  }
}

/*
  Nom de la routine : routine d'interruption SPI (esclave)
  Auteur : Richard Milette / Claude Bouchard
  Date : mars 2021
  Version : 2.0.0
  Description :  Cette routine permet de transmettre les octets des donnees sur le bus SPI,
  sur demande du maître.
*/
ISR (SPI_STC_vect) {
  //Lit l'octet recu du maître
  uint8_t RX_char = SPDR;

  //Indexe le tableau à partir de 0 quand nous recevons une valeur nulle :
  if (RX_char == 0x00 ) {
    buffer_pos = 0;
  }

  if (buffer_pos >= 0 && buffer_pos < (NOMBRE_CAPTEURS * 2)) {
    SPDR  = donnees[buffer_pos++];
  }
  else {
    SPDR = 255;
  }

}
