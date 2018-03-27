#include "stdafx.h"
//#include "Klasa_ModulParametry.hpp"

ModulParametry::ModulParametry() {
	//Wczytanie parametrow za pomoca funkcji pomocniczej
	this->wczytajparametry();
}

  void ModulParametry::wczytajparametry() {
	/*Funkcja odpowiedzialna za wczytywanie parametrow z pliku*/
	//Proba otwarcia pliku binarnego z parametrami programu
	this->Plik_Parametry.open(Plik_Parametry_Nazwa, std::ios::binary | std::ios::in);
	//Sprawdzenie czy plik udalo sie otworzyc
	if (this->Plik_Parametry.is_open()) {
		//Wczytanie danych
		this->Plik_Parametry.read(reinterpret_cast<char*>(&Parametry_obiektIO), sizeof(Parametry_obiektIO));

		//Kopiowanie danych z pliku do pol statycznych klasy Parametry
		Parametry::czasprzerwy = Parametry_obiektIO.czasprzerwy;
		Parametry::jednostkaczasowa = Parametry_obiektIO.jednostkaczasowa;
		Parametry::oczekiwanyzysk = Parametry_obiektIO.oczekiwanyzysk;
		Parametry::zaokraglenie = Parametry_obiektIO.zaokraglenie;

		//Kopiowanie danych z pliku do obiektu Parametry_programu - Tablica 'Dostepnosc'
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 720; j++) {
				this->Parametry_programu.Dostepnosc[i][j] = Parametry_obiektIO.Dostepnosc[i][j];
			}
		}

	}
	else {
		//Komunikat na wypadek niepowodzenia
		cout << "Nie udalo sie otworzyc Plik_Parametry\n";
	}
	//Zamkniecie pliku
	this->Plik_Parametry.close();
}

  void ModulParametry::zapiszparametry() {
	/*
	Funkcja odpowiadajaca za zapisanie parametrow programu za kazdym razem gdy nastapia jakies zmiany
	*/
	//Proba otwarcia pliku
	this->Plik_Parametry.open(Plik_Parametry_Nazwa, std::ios::binary | std::ios::out);

	if (Plik_Parametry.is_open()) {//Sprawdzenie czy plik udalo sie otworzyc
								   //Kopiowanie danych z pol statycznych klasy Parametry do obiektu klasy pomocniczej ParametryIO
		Parametry_obiektIO.czasprzerwy = Parametry::czasprzerwy;
		Parametry_obiektIO.jednostkaczasowa = Parametry::jednostkaczasowa;
		Parametry_obiektIO.oczekiwanyzysk = Parametry::oczekiwanyzysk;
		Parametry_obiektIO.zaokraglenie = Parametry::zaokraglenie;

		//Kopiowanie danych z tablicy 'Dostepnosc' do obiektu klasy pomocniczej ParametryIO
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 720; j++) {
				Parametry_obiektIO.Dostepnosc[i][j] = this->Parametry_programu.Dostepnosc[i][j];
			}
		}

		//Zapisanie danych w pliku binarnym
		Plik_Parametry.write(reinterpret_cast<char*>(&Parametry_obiektIO), sizeof(Parametry_obiektIO));
	}
	else {
		//Komunikat na wypadek niepowodzenia
		cout << "Nie udalo sie zapisac Plik_Parametry\n";
	}
	//Zamkniecie pliku
	Plik_Parametry.close();
}

  void ModulParametry::menu() {
	/* Funkcja pomocnicza odpowiedzialna za wyswietlenie interfejsu ustawien parametrow */
	this->Parametry_programu.menu();
	//Funkcja zapisuje zmiany w pliku po wyjsciu z interfejsu
	this->zapiszparametry();
}
