#ifndef Suiveur_h
#define Suiveur_h

#include "Arduino.h"
#include "SPI.h"

#define BUFFER_SIZE 12
#define NUMBER_OF_SENSORS 6

//Définition des broches de l'interface SPI (carte Arduino MEGA 1280/2560)
#define BROCHE_SPI_SCK 52
#define BROCHE_SPI_MOSI 51
#define BROCHE_SPI_MISO 50
#define BROCHE_SPI_SS_N 53

extern "C" void SPI_STC_vect(void) __attribute__((signal));

struct Ligne
{
	uint16_t s1;
	uint16_t s2;
	uint16_t s3;
	uint16_t s4;
	uint16_t s5;
	uint16_t s6;
};

class Suiveur
{
public:
	Suiveur(int sck = BROCHE_SPI_SCK, int mosi = BROCHE_SPI_MOSI, int miso = BROCHE_SPI_MISO, int ss_n = BROCHE_SPI_SS_N);
	void begin();
	void refresh();
	Ligne getValues();

private:
	SPISettings _settings;
	int _sck, _mosi, _miso, _ss_n;
	Ligne line;
	uint16_t _values[NUMBER_OF_SENSORS];
	static uint8_t Suiveur::_buffer[BUFFER_SIZE];

};
#endif