/*
 * TGPSuiveur_demo.ino
 * Par : Claude Bouchard
 * Ce programme montre comment utiliser la librairie Suiveur.h
 * pour lire les capteurs du suiveur de ligne.
 */

#include <Suiveur.h>

//Déclaration d'un objet monSuiveur du type Suiveur
Suiveur suiveur;
//Suiveur suiveur(52, 51, 50, 53) //Pour utiliser d'autres broches pour le SPI (dans l'ordre : SCK, MOSI, MISO et SS_M) .

void setup()
{
  Serial.begin(115200);
  suiveur.begin();
}

void loop()
{
  //Transmet une demande de lecture au suiveur de ligne
  suiveur.refresh();

  //Lit les valeurs retournés par le suiveur de ligne
  Ligne ligne = suiveur.getValues();

  //Affiche le résultat dans le moniteur série
  Serial.print(ligne.s1);
  Serial.print(' ');
  Serial.print(ligne.s2);
  Serial.print(' ');
  Serial.print(ligne.s3);
  Serial.print(' ');
  Serial.print(ligne.s4);
  Serial.print(' ');
  Serial.print(ligne.s5);
  Serial.print(' ');
  Serial.println(ligne.s6);

  delay(20);
}