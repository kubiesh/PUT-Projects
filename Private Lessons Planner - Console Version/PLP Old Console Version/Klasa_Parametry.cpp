#include "stdafx.h"
#include <sstream>
//#include "Klasa_Parametry.hpp"

//Ustawienie domyslnych wartosci pol statycznych 'jednostkaczasowa' oraz 'zaokraglenie':
int Parametry::jednostkaczasowa = 60; //60 minut
int Parametry::zaokraglenie = 5; //5 minut
int Parametry::czasprzerwy = 10;
int Parametry::oczekiwanyzysk = 0;
int Parametry::Dostepnosc[7][720];

Parametry::Parametry() {
	/*Petla for wypelniajaca tablice 'Dostepnosc' flagami '-1', czyli domyslnie korepetytor nie jest
	dostepny w zadnym terminie*/
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 720; j++) {
			Dostepnosc[i][j] = -1; //Flaga '-1'
		}
	}
}

  void Parametry::wyswietlgodzine(int wartoscniezaokraglona) {
	/*Funkcja odpowiedzialna za wyswietlenie godziny na podstawie indeksu tablicy reprezentujacej przedzial 5-minutowy */
	int pom = wartoscniezaokraglona * Parametry::zaokraglenie;
	cout << pom / 60 << ":";
	if (pom % 60 <10) {
		cout << '0'<<pom%60;
	}
	else {
		cout << pom % 60;
	}
}

  std::string Parametry::przekazWartoscPrzedzialu(int dolnyindeks, int gornyindeks)
  {
	  int pom = dolnyindeks*Parametry::zaokraglenie;
	  int godzinaDolnyPrzedzial = pom / 60;
	  int minutaDolnyPrzedzial = pom % 60;
	  std::ostringstream dolnaCzescPrzedzialu;
	  if (minutaDolnyPrzedzial <10) {
		  dolnaCzescPrzedzialu<<godzinaDolnyPrzedzial <<":0"<< minutaDolnyPrzedzial;
	  }
	  else {
		  dolnaCzescPrzedzialu << godzinaDolnyPrzedzial <<':'<<minutaDolnyPrzedzial;
	  }
	  pom = gornyindeks*Parametry::zaokraglenie;
	  int godzinaGornyPrzedzial = pom / 60;
	  int minutaGornyPrzedzial = pom % 60;
	  std::ostringstream gornaCzescPrzedzialu;
	  if (minutaGornyPrzedzial <10) {
		  gornaCzescPrzedzialu << godzinaGornyPrzedzial << ":0" << minutaGornyPrzedzial;
	  }
	  else {
		  gornaCzescPrzedzialu << godzinaGornyPrzedzial << ':' << minutaGornyPrzedzial;
	  }
	  return dolnaCzescPrzedzialu.str()+'-'+gornaCzescPrzedzialu.str();
  }

  void Parametry::aktualneParametry() {
	/* Funkcja odpowiadajaca za wyswietlenie informacji o aktualnych parametrach programu */

	//Wyswietlenie danych
	cout << "::AKTUALNE PARAMETRY::\n"
		<< "Jednostka czasowa: " << Parametry::jednostkaczasowa << " min.\n"
		<< "Czas na przerwe pomiedzy zajeciami: " << this->czasprzerwy << " min.\n"
		<< "Oczekiwany zysk za tydzien pracy: " << oczekiwanyzysk << " PLN.\n";
	cout << "Dostepnosc:\n";
	//Specjalna petla for interpretujaca dane i wypisujaca dyspozycyjnosc w przedzialach czasowych
	//na podstawie tablicy 'Dostepnosc'
	for (int i = 0; i < 7; i++) {	
		//Flaga 'wystapildzien' zapobiega wielokrotnemu wypisywaniu nazwy danego dnia
		bool wystapildzien = false;
		//Flaga 'dolngr' sluzy okresleniu poczatku danego przedzialu czasowego (dolnej granicy)
		//Poczatkowe ustawienie 'false', wartosc sie nie zmieni dopoki nie znajdzie poczatku przedzialu
		bool dolngr = false;

		for (int j = 0; j < 720; j++) {
			//Sprawdzenie czy flaga dostepnosci jest rozna od '-1' czyli czy korepetytor jest dostepny
			//w danym przedziale czasowym
			if (Dostepnosc[i][j] != -1) {
				if (!wystapildzien) {
					//Wyswietla nazwe dnia jesli do tej pory nie zostala wyswietlona (flaga 'wystapildzien')
					cout << DniTygodnia[i] << ": ";
				}

				if (dolngr == true && j == 719) {
					//Sprawdzamy warunek bardzo malo prawdopobny, czyli ze korepetytor pracuje do 23:55
					//Wypisujemy koniec tego przedzialu
					Parametry::wyswietlgodzine(j);
				}

				//Sprawdzenie czy otwarto przedzial czasowy (flaga 'dolngr')
				if (!dolngr) {
					if (wystapildzien) {
						//Jezeli w danym dniu jest wiecej przedzialow niz jeden, zostaja one wypisane po przecinku
						cout << ", ";
					}
					//Korzystamy z funkcji pomocniczej wyswietlajacej godzine na podstawie indeksu przedzialu
					Parametry::wyswietlgodzine(j);

					cout << "-"; //Pierwsza czesc przedzialu

								 //Zmieniamy flage 'dolngr' dajac znak, ze wyswietlono dolna granice przedzialu
					dolngr = true;
				}

				//Zmiana tej flagi wystepuje na koncu bloku instrukcji warunkowej, aby nie wyswietlac przecinka przy pierwszym przedziale czasowym
				wystapildzien = true;
			}

			//Sprawdzamy czy przypadkiem nie skonczyl sie przedzial czasowy dyspozycyjnosci korepetytora
			if (Dostepnosc[i][j] == -1 && dolngr == true) {
				//Ustawiamy flage 'dolngr' na false dajac znac ze przedzial zostal zamkniety i wyswietlony
				dolngr = false;
				//Wyswietlamy gorna granice przedzialu
				//Nie odejmujemy 1 od indeksu, poniewaz przedzial czasowy musi sie zamknac
				Parametry::wyswietlgodzine(j);
			}

		}

		//Zakonczylismy iterowanie po dyspozycyjnosci korepetytora, dodajemy znak konca linii jesli jakiekolwiek przedzialy zostaly wypisane
		//Mowi nam o tym flaga 'wystapildzien'
		if (wystapildzien) {
			cout << '\n';
		}
	}
}

  void Parametry::ustaw_oczekiwanyzysk(int nowyzysk) {
	//Setter pola 'oczekiwanyzysk'
	//Sprawdzenie czy wartosc wprowadzona przez uzytkownika jest prawidlowa
	if (nowyzysk > 0) {
		Parametry::oczekiwanyzysk = nowyzysk;
	}
	else {
		//Jesli wartosc nieprawidlowa wyrzuc wyjatek
		throw (std::out_of_range("Oczekiwany zysk nie moze byc mniejszy od 0."));
	}
}

  void Parametry::ustaw_jednostkaczasowa(int nowa) {
	//Setter pola 'jednostkaczasowa'
	//Sprawdzenie czy wartosc wprowadzona przez uzytkownika jest prawidlowa
	if (nowa > 0) {
		Parametry::jednostkaczasowa = nowa;
	}
	else {
		//Jesli wartosc nieprawidlowa wyrzuc wyjatek
		throw (std::out_of_range("Jednostka czasowa nie moze byc mniejsza od 0."));
	}
}

  void Parametry::ustaw_czasprzerwy(int nowa) {
	//Setter pola 'czasprzerwy'
	//Sprawdzenie czy wartosc wprowadzona przez uzytkownika jest prawidlowa
	if (nowa >= Parametry::zaokraglenie) {
		Parametry::czasprzerwy = nowa;
	}
	else {
		//Jesli wartosc nieprawidlowa wyrzuc wyjatek
		throw (std::out_of_range("Przerwa nie moze byc mniejsza od 5 minut."));
	}
}



  void Parametry::ustaw_dostepnosc() {
	/* Funkcja odpowiedzialna za wyswietlenie interfejsu do zarzadzania dyspozycyjnoscia korepetytora */
	//Wyswietlenie komunikatu o tym ze znalezlismy sie w tym interfejsie
	cout << "::USTALANIE TWOICH GODZIN PRACY::\n";

	//Interfejs dziala w petli dopoki nie zdecydujemy sie z niego wyjsc
	while (true) {
		//Wyswietlenie sposobu poslugiwania sie interfejsem
		cout << "Wybierz dzien:\n1 - Poniedzialek\n2 - Wtorek\n3 - Sroda\n"
			<< "4 - Czwartek\n5 - Piatek\n6 - Sobota\n7 - Niedziela\n13-Wyczysc dany dzien z danych\n0 - Wyjdz z menu\n";
		//Zmienna przechowujaca nasz wybor
		int wybor{};
		//Przekazujemy wybor ze strumienia wejsciowego do zmiennej
		cin >> wybor;
		//Zmienna ustawiajaca wskaznik, a dokladniej indeks tablicy 'Dostepnosc' odpowiadajacej danemu dniu
		int wskaznik{};
		//Instrukcja warunkowa ustawiajaca wskaznik
		switch (wybor) {
		case 1: //Poniedzialek
			wskaznik = 0;
			break;
		case 2: //Wtorek
			wskaznik = 1;
			break;
		case 3: //Sroda
			wskaznik = 2;
			break;
		case 4: //Czwartek
			wskaznik = 3;
			break;
		case 5: //Piatek
			wskaznik = 4;
			break;
		case 6: //Sobota
			wskaznik = 5;
			break;
		case 7: //Niedziela
			wskaznik = 6;
			break;
		case 13: //Czyszczenie
			this->wyczysc_dzien();
			break;
		}
		//Jezeli wybor nie padl na zaden z dni nie mozemy kontynuowac dzialania funkcji i musimy ja przerwac
		if (wybor <1 || wybor >7) {
			//Przerwanie petli
			break;
		}
		//Jesli korepetytor nie zdecydowal sie wyjsc z interfejsu to ustala przedzial czasowy dla dnia, ktory wybral
		//Wyswietlamy komunikat z instrukcja
		cout << ":Dolna granica przedzialu dyspozycyjnosci:\n";
		//Zmienne przechowujace dane o godzinie i minucie dolnego przedzialu czasowego
		int dolna_godzina{};
		int dolna_minuta{};
		//Petla do-while
		do {
			//Stosowne komunikaty i wczytywanie ze strumienia wejsciowego
			cout << "Wpisz godzine dla dolnej granicy:\n";
			cin >> dolna_godzina;
			cout << "Wpisz minute dla dolnej granicy:\n";
			cin >> dolna_minuta;

		}
		//Sprawdzamy czy wprowadzone wartosci sa poprawne, jesli nie bedziemy czekac az korepetytor wprowadzi poprawne dane
		while (dolna_godzina > 25 && dolna_godzina < 0 && dolna_minuta < 0 && dolna_minuta>60);

		//Wyswietlamy komunikat z instrukcja
		cout << ":Gorna granica:\n";
		//Zmienne przechowujace dane o godzinie i minucie gornego przedzialu czasowego
		int gorna_godzina{};
		int gorna_minuta{};

		//Petla do-while
		do {
			//Stosowne komunikaty i wczytywanie ze strumienia wejsciowego
			cout << "Wpisz godzine dla gornej granicy:\n";
			cin >> gorna_godzina;
			cout << "Wpisz minute dla gornej granicy:\n";
			cin >> gorna_minuta;

		}
		//Sprawdzamy czy wprowadzone wartosci sa poprawne, jesli nie bedziemy czekac az korepetytor wprowadzi poprawne dane
		while (dolna_godzina > 25 && dolna_godzina < 0 && dolna_minuta < 0 && dolna_minuta>60);

		//Obliczamy indeks otwierajacego przedzialu czasowego
		int dolny_indeks = (dolna_godzina * 60 + dolna_minuta) / Parametry::zaokraglenie;
		//Zamykajacego
		int gorny_indeks = (gorna_godzina * 60 + gorna_minuta) / Parametry::zaokraglenie;

		//Petla for do zapisania dyspozycyjnosci korepetytora w tablicy 'Dostepnosc'
		for (int i = dolny_indeks; i < gorny_indeks; i++) {
			this->Dostepnosc[wskaznik][i] = 0; //Ustawienie flagi '0' oznaczajacej ze w tym przedziale korepetytor jest dostepny
		}

		//Powiadomienie korepetytora o tym jaki przedzial dostepnosci zostal ustawiony
		//Jest to tez sprawdzenie poprawnosci dzialania programu
		//Wyswietlenie odpowiedniego dnia:
		cout << DniTygodnia[wskaznik] << " ";
		//Wyswietlenie godziny dolnej granicy przedzialu
		Parametry::wyswietlgodzine(dolny_indeks);
		cout << "-";
		//Wyswietlenie godziny gornej granicy przedzialu
		Parametry::wyswietlgodzine(gorny_indeks);
		cout << '\n';
	}
}

void Parametry::wyczysc_dzien() {

	/* Funkcja odpowiedzialna za wyswietlenie interfejsu do zarzadzania dyspozycyjnoscia korepetytora */
	//Wyswietlenie komunikatu o tym ze znalezlismy sie w tym interfejsie
	cout << "::USTALANIE TWOICH GODZIN PRACY::\n";

	//Interfejs dziala w petli dopoki nie zdecydujemy sie z niego wyjsc
	while (true) {

		//Wyswietlenie sposobu poslugiwania sie interfejsem
		cout << "Wybierz dzien ktoremu chcesz 'wyzerowac' godziny pracy:\n1 - Poniedzialek\n2 - Wtorek\n3 - Sroda\n"
			<< "4 - Czwartek\n5 - Piatek\n6 - Sobota\n7 - Niedziela\n0 - Wyjdz z menu\n";
		//Zmienna przechowujaca nasz wybor
		int wybor{};
		//Przekazujemy wybor ze strumienia wejsciowego do zmiennej
		cin >> wybor;
		//Zmienna ustawiajaca wskaznik, a dokladniej indeks tablicy 'Dostepnosc' odpowiadajacej danemu dniu
		int wskaznik{};
		//Instrukcja warunkowa ustawiajaca wskaznik
		switch (wybor) {
		case 1: //Poniedzialek
			wskaznik = 0;
			break;
		case 2: //Wtorek
			wskaznik = 1;
			break;
		case 3: //Sroda
			wskaznik = 2;
			break;
		case 4: //Czwartek
			wskaznik = 3;
			break;
		case 5: //Piatek
			wskaznik = 4;
			break;
		case 6: //Sobota
			wskaznik = 5;
			break;
		case 7: //Niedziela
			wskaznik = 6;
			break;
		}
		//Jezeli wybor nie padl na zaden z dni nie mozemy kontynuowac dzialania funkcji i musimy ja przerwac
		if (wybor <1 || wybor >7) {
			//Przerwanie petli
			break;
		}
		else {
			for (int j = 0; j < 720; j++) {
				this->Dostepnosc[wskaznik][j] = -1;
			}
			cout << "Dzien " << DniTygodnia[wskaznik] << " zostal wyczyszczony z godzin pracy.\n";

		}

	}
}

  void Parametry::menu() {
	/* Funkcja odpowiedzialna za wyswietlenie glownego menu do zarzadzania parametrami programu
	z poziomu aplikacji*/

	//Zmienna do ustawiania nowych wartosci za pomoca strumienia wejsciowego i przekazywana do setterow
	int nowawartosc;
	//Flaga 'warunekpetli' daje sygnal czy menu ma zostac wyswietlane czy jednak uzytkownik zdecydowal sie wyjsc z interfejsu
	bool warunekpetli = true;
	//Petla while dla menu wyswietlajacego sie dopoki uzytkownik z niego nie zdecyduje wyjsc
	while (warunekpetli) {
		//Wyseitlenie komunikatu o dostepnych elementach menu
		cout << "\n::PARAMETRY GLOWNE - MENU::\n1 - Ustaw jednostke czasowa(domyslnie 60min)\n"
			<< "2 - Ustal czas przerwy(domyslnie 10min)\n3 - Ustaw oczekiwany zysk\n"
			<< "4 - Ustaw ograniczenia czasowe\n5 - Aktualne parametry\n0 - Wyjdz z menu i zapisz zmiany\n";
		//Zmienna przechowujaca wybor pozycji menu ze strumienia wejsciowego
		int wybor = -1;
		//Strumien wejsciowy
		cin >> wybor;
		//Instrukcja przelaczajaca miedzy wyborami uzytkownika
		switch (wybor) {
		case 1: //Ustawienia jednostki czasowej
			cout << "Podaj w minutach nowa jednostke czasowa:\n";
			std::cin >> nowawartosc;
			try {
			ustaw_jednostkaczasowa(nowawartosc);
			cout << "Jednostka czasowa to teraz: " << jednostkaczasowa << " min.\n";
			}
			catch (std::exception &a) {
				cout << a.what();
			}
			break;
		case 2: //Ustawienia czasu przerwy
			cout << "Podaj w minutach czas przerwy:\n";
			std::cin >> nowawartosc;
			try {
			ustaw_czasprzerwy(nowawartosc);
			cout << "Czas przerwy to teraz: " << czasprzerwy << " min.\n";
			}
			catch (std::exception &a) {
				cout << a.what();
			}
			break;
		case 3: //Ustawienia oczekiwanej kwoty zysku z korepetycji
			cout << "Podaj oczekiwany zysk z korepetycji:\n";
			std::cin >> nowawartosc;
			try {
				ustaw_oczekiwanyzysk(nowawartosc);
			cout << "Oczekiwany zysk to teraz: " << oczekiwanyzysk << " PLN.\n";
			}
			catch (std::exception &a) {
				cout << a.what();
			}
			break;
		case 4: //Ustawienia dostepnosci korepetytora, przekierowania do kolejnego interfejsu
			this->ustaw_dostepnosc();
			break;
		case 5: //Wyswietlenie aktualnych parametrow programu
			this->aktualneParametry();
			break;
		case 0: //Wyjscie z interfejsu
			warunekpetli = false;
			break;
		}
	}
}
