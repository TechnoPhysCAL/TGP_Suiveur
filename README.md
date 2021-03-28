# Librairie TGP Suiveur

Permet le contrôle simplifié du suiveur de ligne sur la plateforme TGP Robot. Permet la lecture des 6 photodétecteurs du suiveur de ligne, via une communication SPI.

Les photodétecteurs doivent être commandées via un microcontrôleur UNO utilisant entre autre la librairie "QTRSensors" (voir code dans le répertoire /others ).


## Notes de version

### 2.0.0
- Version pour le nouveau programme en 2021. Permet maintenant de recevoir les valeurs brutes complètes des 6 capteurs.
 
## Utilisation

```cpp
#include <Suiveur.h> 

Suiveur suiveur;
 
void setup()
{
  suiveur.begin();
}

void loop()
{
  suiveur.refresh(); 

  Ligne line = suiveur.getValues();
  line.s1;
  line.s2;
  line.s3;
  line.s4;
  line.s5;
  line.s6;
```

## Constructeurs
```cpp
Suiveur()
Suiveur(int sck, int miso, int mosi, int ss_n)
```
Permet d'initialiser la communication SPI nécessaire pour recevoir les valeurs. Par défaut,  sck=52, miso=51, mosi=50 et int ss_n=53.

## Propriétés disponibles

```cpp
void begin()
```
Cette méthode doit être placée dans la boucle setup(). Elle permet d'initialiser la communication SPI.

---
```cpp
void refresh()
```
Cette méthode doit être placée en début de boucle loop(): elle permet de faire la communication via SPI pour obtenir les valeurs les plus récentes.

---
```cpp
Ligne void getValues()
```
Permet de retourner une structure Ligne qui contient les six valeurs des photodétecteurs. La structure est la suivante:

```cpp
struct Ligne
{
	uint16_t s1;
	uint16_t s2;
	uint16_t s3;
	uint16_t s4;
	uint16_t s5;
	uint16_t s6;
};
```
