#include "Suiveur.h"

static uint8_t Suiveur::_buffer[BUFFER_SIZE];

/***************************************************************
// Constructeur
//***************************************************************/
Suiveur::Suiveur(int sck, int mosi, int miso, int ss_n) : _settings(4000000, MSBFIRST, SPI_MODE0)
/*Configure le bus SPI comme suit:
	- Permet les interruptions
	- Active le bus SPI
	- Transmet le MSB en premier
	- Configure le bus SPI comme ma�tre
	- CPOL = 0 et CPHA = 0
	- freq = 4 MHz
	*/
{
	_sck = sck;
	_mosi = mosi;
	_miso = miso;
	_ss_n = ss_n;

	//Initialise les broches du bus SPI
	pinMode(_sck, OUTPUT);
	pinMode(_mosi, OUTPUT);
	pinMode(_miso, INPUT);
	pinMode(_ss_n, OUTPUT);
}

void Suiveur::begin()
{
	SPI.begin();
}

void Suiveur::refresh()
{

	SPI.beginTransaction(_settings);

	digitalWrite(_ss_n, LOW); //Active le signal de slave select du bus SPI
	for (byte i = 0; i < BUFFER_SIZE; i++)
	{
		_buffer[i] = SPI.transfer(i == BUFFER_SIZE - 1 ? 0 : 1);
		delayMicroseconds(10);
	}
	digitalWrite(_ss_n, HIGH); //Désactive le signal de slave select du bus SPI

	SPI.endTransaction();

	for (byte i = 0; i < NUMBER_OF_SENSORS; i++)
	{
		_values[i] = ((uint16_t)_buffer[i * 2]) * 256 + _buffer[i * 2 + 1];
	}

	line.s1 = _values[5];
	line.s2 = _values[4];
	line.s3 = _values[3];
	line.s4 = _values[2];
	line.s5 = _values[1];
	line.s6 = _values[0];
}

Ligne Suiveur::getValues()
{
	return line;
}
