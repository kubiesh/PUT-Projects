#ifndef KLASA_PARAMETRY_HPP
#define KLASA_PARAMETRY_HPP
/*Plik Klasa_Parametry.h zawiera:
- deklaracja klasy Parametry
- deklaracja klasy ParametryIO */
//
//#include "Jadro_programu.h"

//Import bibliotek
//#include <iostream>
//
//using std::cout;
//using std::cin;

extern std::vector<std::string>DniTygodnia;

class Parametry {
	/* Klasa odpowiadajaca za przechowywanie informacji o podstawowych zalozeniach planu lekcji na podstawie
	ktorych algorytm bedzie generowac plan
	Klasa przechowuje pola statyczne, aby umozliwiac korzystanie z niektorych parametrow przez inne moduly*/

	friend class ModulParametry;

	/*Pole statyczne 'oczekiwany zysk' (wartosc PLN) przechowuje informacje o zysku oczekiwanym przez korepetytora
	Po wygenerowanu algorytmu kwota oczekiwana wyswietli sie obok kwoty: */
protected: static int oczekiwanyzysk;
	/*Pole statyczne 'jednostkaczasowa' (wartosc w minutach) przechowuje informacje o dlugosci zajec prowadzonych 
	przez korepetytora, domyslnie jest to 60 minut */
public: static int jednostkaczasowa;
	/* Pole statyczne 'czasprzerwy' (wartosc w minutach) przechowuje informacje o dlugosci przerwy pomiedzy zajeciami
	w harmonogramie, tak aby algorytm nie generowal zajec nastepujacych bezposrednio po sobie a z odpowiednim
	odstepem*/
protected: static int czasprzerwy;
	/* Pole statyczne 'zaokraglenie' przechowuje wartosc przedzialu czasowego, w ktorych porusza sie program, aby
	zapobiec marnotractwu czasu i pamieci, poniewaz optymalniej jest okreslac plan dla 5-minutowych przedzialow
	czasowych niz dla kazdej minuty
	Domyslnie 5 minut, nie przewidziano w programie zmiany tego zalozenia*/
public: static int zaokraglenie; 
	/* Dwuwymiarowa tablica liczb calkowitych 'Dostepnosc' przechowuje informacje o dostepnosci
	w 5-minutowych przedzialach czasowych kazdego dnia tygodnia
	Flaga '-1' - niedostepny
	Flaga !='-1' - dostepny
	Kazdy dzien zawiera 720 pozycji, czyli 720 przedzialow 5-minutowych(3600/5=720), tj. 0:00, 0:05, 0:10 itd...
	*/
public: static int Dostepnosc[7][720];

//////////////////////////////////////////////////////////////////////////////////////////

	/* KONSTRUKTOR DOMYŒLNY*/
public: Parametry();

public: static void wyswietlgodzine(int wartoscniezaokraglona);
public: static std::string przekazWartoscPrzedzialu(int dolnyindeks,int gornyindeks);

public: void aktualneParametry();

public: void ustaw_oczekiwanyzysk(int nowyzysk);
public: static void ustaw_jednostkaczasowa(int nowa);
public: static void ustaw_czasprzerwy(int nowa);
protected: void ustaw_dostepnosc();
protected: void wyczysc_dzien();


public: void menu();
};




class ParametryIO {
/* Pomocnicza klasa sluzaca przekazywaniu parametrow programu z pliku i do pliku. Rozdzielona miedzy klasa Parametry
z powodu istnienia w niej istotnych pol statycznych, ktore nie zostaja kopiowane do wlasciwosci obiektu*/

//Przekazanie uprawnien klasie ModulParametry
friend class ModulParametry;

//Wszystkie istotne pola zawierajace istotne wartosci przekazywane z pliku/do pliku
protected: int oczekiwanyzysk;
protected: int czasprzerwy;
protected: int jednostkaczasowa;
protected: int zaokraglenie;
protected: int Dostepnosc[7][720];
};

#endif