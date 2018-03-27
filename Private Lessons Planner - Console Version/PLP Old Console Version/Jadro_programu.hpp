#ifndef JADRO_PROGRAMU_HPP
#define JADRO_PROGRAMU_HPP

//#include <iostream>
//#include <string>
//#include <vector>
//#include <fstream>
//using std::cout;
//using std::cin;

//
//extern class ListaPrzedmiotow;
//extern class ListaKlientow;
//extern class ListaZamowien;

//extern ListaPrzedmiotow;
//extern ListaKlientow;

class Program {
	//
	//MODUL: PARAMETRY PROGRAMU
	//
	//prot
public: ModulParametry ModulParametry_programu;
	//
	//MODUL: BAZA PRZEDMIOTOW
	//
protected: ListaPrzedmiotow Lista_przedmiotow;
	//
	//MODUL: BAZA KLIENTOW ORAZ BAZA DOSTEPNOSCI KLIENTOW
	//
protected: ListaKlientow Lista_klientow;
	//Zamowienia klientow
protected: ListaZamowien Lista_zamowien;
	



//////////////////////////////////////////////////////////////////////////////////////////


	/*KONSTRUKTOR JADRA PROGRAMU */
public: Program();
	/*DESTRUKTOR J¥DRA PROGRAMU */
public: ~Program();

	//
	//MENU GLOWNE
	//
public: void menuglowne();
};


#endif