/*
class DlabsLCD
{
	private:
		int _pinSCE;
		int _pinReset;
		int _pinDC;
		int _pinDIN;
		int _pinSCLK;
		void lcdWrite (byte data, byte data);
		void lcCharacter(char caracter);
	public:
		DlabsLCD(int pinSCE, int pinReset, int pinDC, int pinDIN,int pinSCLK);
		lcdInit();		
		lcdClear();	
		lcdString(char *caracteres);
		
}
*/
#include "DlabsNokia.h"

DlabsLCD::DlabsLCD(int pinSCE, int pinReset, int pinDC, int pinDIN, int pinCLK, int pinLight)
{
	_pinSCE   = pinSCE;
	_pinReset = pinReset;
	_pinDC    = pinDC;
	_pinDIN   = pinDIN;
	_pinCLK   = pinCLK;
	_pinLight = pinLight;
}

void DlabsLCD::lcdWrite(byte dc, byte data)
{
	digitalWrite(_pinDC,dc);
	digitalWrite(_pinSCE,LOW);
	shiftOut(_pinDIN,_pinCLK, MSBFIRST, data);
	digitalWrite(_pinSCE,HIGH);
}
void DlabsLCD::lcdInit()
{
	pinMode(_pinSCE,OUTPUT);
	pinMode(_pinReset,OUTPUT);
	pinMode(_pinDC,OUTPUT);
	pinMode(_pinDIN,OUTPUT);
	pinMode(_pinCLK,OUTPUT);
	pinMode(_pinLight,OUTPUT);
	digitalWrite(_pinReset, LOW);
	digitalWrite(_pinReset, HIGH);
	digitalWrite(_pinLight,HIGH);
	lcdWrite(LCD_C, 0x21); //LCD modo comandos
	lcdWrite(LCD_C, 0xB1); // contraste
	lcdWrite(LCD_C, 0x04); //coeficiente temperartura
	lcdWrite(LCD_C, 0x14); //LCD Bias 1:48
	lcdWrite(LCD_C, 0x20); //LCD comandos basicos
	lcdWrite(LCD_C, 0x0C); //LCD modo normal

}

void DlabsLCD::lcdClear()
{
	gotoXY(0,0);
	for (int i=0;i < LCD_X * LCD_Y / 8; i++)
	{
		lcdWrite(LCD_D,0x00);
	}
}

void DlabsLCD::lcdCharacter(char caracter)
{
//Para escribir un caracter lo que hace es escribir su mapa de bits correspondiente.
	lcdWrite(LCD_D, 0x00);//primero borramos lo que tenemos en esa posición de memoria
	for (int i=0; i< 5; i++)
	{
//Se tienen que restar 0x20 para pasar de caracter decimal a la tabla de caractares que tenemos definida.
		lcdWrite(LCD_D,ASCII[caracter - 0x20][i]);//dibujamos el elemento con su mapa de caracteres
	}
}

void DlabsLCD::lcdString(char *caracteres)
{//Mientras haya caracteres por leer los va escribiendo
//Lo que hace es recorrer el vector hasta el último elemento.
	while (*caracteres)
	{
		lcdCharacter(*caracteres++);
	}
}

void DlabsLCD::LCDBitmap(char my_array[])
{//Dibuja el mapa de caracteres que ocupa toda la pantalla (84x(6x8))
  gotoXY(0,0);
  for (int i = 0 ; i < (LCD_X * LCD_Y / 8) ; i++)
    lcdWrite(LCD_DATA, my_array[i]);
}

void DlabsLCD::lcdSprite(char my_array[11],int x, int y)
{
	gotoXY(x,y);
	for (int i=0;i<11;i++)
	{
	    lcdWrite(LCD_DATA, my_array[i]);
	}
}

void DlabsLCD::lcdLogo()
{
//Manda un mapa de bits para el dibujo de una imagen, que está guardada en un mapa.
	LCDBitmap(logoDlabs2);
}



void DlabsLCD::gotoXY(int x, int y) 
{
/*
Las posiciones a las que se puede mover están dentro de las 84 columnas y 6 bloques de memoria, para acceder a un pixel dentro del bloque de 8 bits hay que indicarlo con un valor en binario.
*/
  lcdWrite(0, 0x80 | x);  // Columna
  lcdWrite(0, 0x40 | y);  // Fila recordar que se mueve por bloques
}

void DlabsLCD::lcdLight()
{
	digitalWrite(_pinLight, !digitalRead(_pinLight));
}
