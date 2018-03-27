#ifndef KLASA_MODULPARAMETRY_HPP
#define KLASA_MODULPARAMETRY_HPP

/*Plik Klasa_Parametry.h zawiera:
- deklaracja klasy ModulParametry (Caly modul obslugi parametrow programu) */

//Import plikow naglowkowych
//#include "stdafx.h"


//Import bibliotek
//#include <iostream>
//#include <string>
//#include <fstream>
//using std::cout;
//using std::cin;

//
//Klasa odpowiadajaca za obsluge obiektu parametrow w jadrze programu
//Powstala rozdzielona od jadra, aby ulatwic czytelnosc klasy jadra
class ModulParametry {
	//Udzielamy dostepu do modulu parametrow dla jadra programu
	friend class Program;

protected: ParametryIO Parametry_obiektIO;
	//Obiekt klasy Parametry:
public: Parametry Parametry_programu; 
	//Obiekt obslugujacy plik binarny przechowujacy wartosci parametrow:
protected:	std::fstream Plik_Parametry;
	//Lancuch znakow przechowujacy nazwe pliku z parametrami:
protected:	std::string Plik_Parametry_Nazwa = "Plik_Parametry.bin";

//////////////////////////////////////////////////////////////////////////////////////////

	/*KONSTRUKTOR MODULU PARAMETRY */
public: ModulParametry();

protected: void wczytajparametry();


public: void zapiszparametry();


public: void menu();
};




#endif