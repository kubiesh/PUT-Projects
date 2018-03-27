#include "stdafx.h"
//#include "Klasa_ListaKlientow.hpp"


//#include "Klasa_Klient.hpp"

ListaKlientow::ListaKlientow() {
	// 1. WCZYTYWANIE BAZY KLIENTOW Z PLIKU
	this->wczytajLista();
	// 2. WCZYTYWANIE BAZY DOSTEPNOSCI KLIENTOW Z PLIKU
	this->wczytajDostepnoscKlientow();
}

  void ListaKlientow::menu() {
	/* Funkcja odpowiedzialna za obsluge glownego menu do interfejsu zarzadzania klientami*/
	//Flaga ktora warunkuje wyswietlanie sie glownego menu modulu
	bool warunekpetli = true;
	while (warunekpetli) {
		//Wyswietlenie dostepnych opcji
		cout << "\n::BAZA KLIENTOW - MENU::\n1 - Wyswietl liste klientow\n"
			<< "2 - Dodaj klienta\n3 - Usun klienta\n4 - Modyfikacja danych osobowych\n5 - Dyspozycyjnosc klientow\n0 - Wroc do menu glownego\n";
		int wybor = -1;
		cin >> wybor;
		switch (wybor) {
		case 1: //Wyswietl liste klientow
			this->wyswietlliste();
			break;
		case 2: //Dodaj klienta
			this->dodajklienta();
			break;
		case 3: //Usun klienta
			this->usunklienta();
			break;
		case 4: //Modyfikacja danych osobowych
			this->modyfikacjadaneosobowe();
			break;
		case 5: //Dyspozycyjnosc klienta
				//Przejscie do menu zarzadzania dyspozycyjnoscia klienta
			this->dyspozycyjnosc();
			break;
		default: //Wyjscie z menu
			warunekpetli = false;
			break;
		}
	}
}

  void ListaKlientow::wczytajLista() {
	/*Funkcja odpowiedzialna za wczytywanie danych z pliku binarnego do kolekcji bazy klientow*/
	//Proba otwarcia pliku binarnego z baza danych klientow
	this->Plik_Lista.open(this->Plik_Lista_Nazwa, std::ios::binary | std::ios::in);
	//Sprawdzenie czy plik udalo sie otworzyc
	if (this->Plik_Lista.is_open()) {
		//Utworzenie obiektu klasy Klient, ktory bedzie sluzyl wczytywaniu danych z pliku
		Klient odczytany;
		while (this->Plik_Lista.read(reinterpret_cast<char*>(&odczytany), sizeof(odczytany))) {
			this->Lista.push_back(odczytany); //Dodajemy rekord do bazy danych
		}
		//Sprawdzamy czy byly w pliku jakiekolwiek dane aby ustalic jakie bylo ostatnie ID klienta
		if (this->Lista.size() > 0) {
			//Jezeli w pliku byly dane ustawiamy w polu statycznym ostatnie ID klienta aby zachowac unikalnosc kluczy
			int ostatnie_id = this->Lista[this->Lista.size() - 1].id_klienta;
			Klient::ostatnie_id = ostatnie_id;
		}
	}
	else {
		//Komunikat na wypadek niepowodzenia
		cout << "Nie udalo sie otworzyc Plik_ListaKlientow\n";
	}
	//Zamkniecie pliku
	this->Plik_Lista.close();
}

  void ListaKlientow::zapiszzmiany() {
	/*Funkcja odpowiedzialna za zapisywanie zmian w bazie danych klientow do pliku binarnego*/

	//Proba otwarcia pliku binarnego z baza danych klientow
	this->Plik_Lista.open(this->Plik_Lista_Nazwa, std::ios::binary | std::ios::out);
	//Sprawdzenie czy plik udalo sie otworzyc
	if (this->Plik_Lista.is_open()) {
		for (auto it = this->Lista.begin(); it != this->Lista.end(); ++it) {
			//Iterujac po calej bazie danych zapisujemy rekord po rekordzie do pliku binarnego
			this->Plik_Lista.write(reinterpret_cast<char*>(&(*it)), sizeof((*it)));
		}
		cout << "Zapisano zmiany w pliku ListaKlientow\n";
	}
	else {
		//Komunikat na wypadek niepowodzenia
		cout << "Nie udalo sie zapisac Plik_ListaKlientow\n";
	}
	//Zamkniecie pliku
	this->Plik_Lista.close();
}

  void ListaKlientow::wyswietlliste() {
	/*Funkcja odpowiedzialna za wyswietlanie listy klientow w porzadku alfabetycznym
	Utworzona jest tymczasowa kopia danych, ktora zostaje sortowana*/

	//Sprawdzamy czy w bazie danych klientow sa zapisane jakies dane, inaczej nie ma sensu probowac ich wyswietlac
	if (Lista.size()>0) {
		//Kopia bazy danych klientow
		std::vector<Klient> KopiaListy = Lista;
		//Funkcja z wyrazeniem lambda sortujaca dane w porzadku alfabetycznym
		std::sort(KopiaListy.begin(), KopiaListy.end(), [](const Klient &a, const Klient &b) -> bool { if (std::strcoll(a.nazwisko, b.nazwisko) != 0) { if (std::strcoll(a.nazwisko, b.nazwisko) < 0) return true; else return false; } else { if (std::strcoll(a.imie, b.imie) < 0) return true; else return false; }  });

		//Naglowek tabeli opisujacy zawartosc kolumn
		cout << "::LISTA KLIENTOW (alfabetycznie)::\n";
		cout << "ID\tNazwisko\tImie\tNr telefonu\tEmail\n";

		//Petla iterujaca po calej bazie danych i wyswietlajaca
		for (auto element : KopiaListy) {
			cout << element.id_klienta << '\t' << element.nazwisko << '\t' << element.imie << '\t' << element.nrtelefonu << '\t' << element.email << '\n';
		}
		cout << '\n';
		cout << "Lacznie: " << KopiaListy.size() << " klientow w bazie.\n";
	}
	else {
		//Komunikat na wypadek niepowodzenia
		cout << "Aktualnie nie ma zadnych danych w bazie klientow.\n";
	}
}

  std::vector<Klient>::iterator ListaKlientow::znajdz(int id_klienta) {
	/*Funkcja pomocnicza zwracajaca iterator klienta o podanym numerze ID
	W przypadku gdy nie ma klienta o podanym numerze ID funkcja zwraca iterator Lista.end()*/

	//Ustawienie iteratora na poczatek bazy danych
	auto it = Lista.begin();
	//Iteracja po calej bazie danych w celu znalezienia rekordu o szukanym numerze ID
	while (it != Lista.end()) {
		if ((*it).id_klienta == id_klienta) {
			//W razie powodzenia przerwij dzialanie petli
			break;
		}
		++it;
	}
	//Zwroc iterator
	return it;
}

  void ListaKlientow::dodajklienta() {
	/*Funkcja odpowiedzialna za dodanie nowego rekordu z danymi klienta do bazy danych*/


	cout << "::DODAWANIE KLIENTA::\n";
	//Zignorowanie bialego znaku/znaku konca linii na wypadek jego pojawienia sie w strumieniu wejsciowym
	cin.ignore();

	//Nazwisko
	cout << "Wprowadz nazwisko:\n";
	std::string nazwisko;
	std::getline(cin, nazwisko);

	//Imie
	std::string imie;
	cout << "Wprowadz imie:\n";
	std::getline(cin, imie);

	//Numer telefonu
	std::string nrtelefonu;
	cout << "Wprowadz numer telefonu:\n";
	std::getline(cin, nrtelefonu);

	//Adres email
	std::string email;
	cout << "Wprowadz adres email:\n";
	std::getline(cin, email);

	//Tworzenie nowego obiektu, ktory zostanie dodany do bazy
	Klient Nowy(imie, nazwisko, nrtelefonu, email);
	//Dodanie obiektu do bazy
	Lista.push_back(Nowy);
	//Stosowny komunikat
	cout << "Pomyslnie dodano klienta do bazy.\n";

	//Wywolanie funkcji zapisujacej zmiany w bazie danych
	this->zapiszzmiany();
}

  void ListaKlientow::usunklienta() {
	/*Funkcja odpowiedzialna za usuwanie klienta z bazy danych*/

	//Wyswietlenie instrukcji uzytkownikowi
	cout << "::USUWANIE KLIENTA Z BAZY::\n";
	cout << "Wprowadz ID klienta, ktory chcesz usunac(Wpisz 0 jesli chcesz sie wycofac):\n";

	//Zmienna przechowujaca wpisane ID
	int id_dousuniecia;
	//Wczytanie ID ze strumienia wejsciowego
	cin >> id_dousuniecia;
	//Sprawdzamy czy korepetytor wycofal sie z usuwania czy tez nie
	if (id_dousuniecia != 0) {
		//Wywolanie funkcji pomocniczej zwracajacej iterator klienta o podanym numerze id
		auto it = this->znajdz(id_dousuniecia);
		//Jezeli istnieje rekord o podanym numerze id to usuwamy klienta z bazy
		//Sprawdzamy czy iterator nam nie zwrocil konca kolekcji
		if (it != Lista.end()) {
			//Jezeli numer ID jest prawidlowy wyswietlamy komunikat z nazwiskiem i imieniem usunietego klienta
			cout << "Usunieto klienta: " << (*it).nazwisko << " " << (*it).imie << '\n';
			//Usuwamy wszystkie wpisy z danym ID klienta z listy zamowien
			ListaZamowien::usunwszystkiewpisy_idklienta(id_dousuniecia);
			//Usuniecie z bazy klientow
			it = Lista.erase(it);

			//Jezeli jednak uzytkownik sie pomylil i nie chcialby dokonac usuniecia trzeba dac mu szanse wyboru
			//Wyswietlenie komunikatu z zapytaniem o zapisanie zmian
			cout << "Czy zapisac zmiany? (Usuniecie klienta spowoduje usuniecie wszystkich zwiazanych"
				<< "z nim informacji w bazach danych klientow, dyspozycyjnosci i zamowien)\n1 - Tak\n0 - Nie\n";
			//Zmienna przechowujaca wybor zapisu zmian
			int czyzapisaczmiany = 0;
			//Wczytanie wyboru ze strumienia wejsciowego
			cin >> czyzapisaczmiany;
			if (czyzapisaczmiany == 1) {
				//Jezeli uzytkownik zgadza sie zapisac zmiany wywolujemy funkcje zapisujace zmiany:
				ListaZamowien::zapiszzmiany();
				this->zapiszzmiany();

			}
			else {
				//Jezeli uzytkownik nie chce zapisac zmian informujemy go wyswietleniem komunikatu
				cout << "Nie zapisano zmian.\n";
				//Wczytujemy ostatnia zapisana wersje danych
				ListaZamowien::wczytajdane();
				this->wczytajDostepnoscKlientow();
			}
		}
		else {
			//W przypadku zle wpisanego numeru ID wyswietlamy komunikat:
			cout << "Nie znaleziono klienta o podanym numerze ID.\n";
		}
	}
}

  void ListaKlientow::modyfikacjadaneosobowe() {
	/* Funkcja odpowiedzialna za wyswietlenie interfejsu do zmiany danych osobowych klienta */

	//Wyswietlenie komunikatu
	cout << "Wpisz ID klienta, ktorego dane osobowe chcesz zmodyfikowac:\n";
	//Zmienna przechowujaca numer ID rekordu, ktory ma zostac poddany modyfikacji
	int szukane_id;
	//Wczytanie numeru ID ze strumienia wejsciowego
	cin >> szukane_id;
	//Wykorzystanie funkcji pomocniczej 'znajdz' i przekazanie wyniku do iteratora
	auto it = this->znajdz(szukane_id);

	//Jezeli iterator jest rozny od Lista.end() to oznacza ze istnieje rekord o podanym numerze ID
	if (it != Lista.end()) {
		//Wyswietlenie danych o rekordzie aby uzytkownik mial swiadomosc kogo poddaje modyfikacjom
		cout << "Znaleziono: " << (*it).id_klienta << ' ' << (*it).nazwisko << ' ' << (*it).imie << ' ' << (*it).nrtelefonu << ' ' << (*it).email << '\n';
		//Zmienna wykorzystana do instrukcji przelaczajacej przechowujaca wybor
		int wybor = -1;
		//Wyswietlenie dostepnych opcji
		cout << "1 - Zmien nazwisko\n2 - Zmien imie\n3 - Zmien numer telefonu\n4 - Zmien email\n0 - Wyjdz\n";
		//Wczytanie wyboru uzytkownika ze strumienia wejsciowego
		cin >> wybor;
		//Wywolanie metody ignorujacej bialy znak/znak konca linii
		cin.ignore();
		//Zmienna do przechowywania lancucha znakow nowych wartosci w razie modyfikacji
		std::string nowawartosc;
		switch (wybor) {
		case 1: //Zmien nazwisko
			cout << "Wprowadz nazwisko:\n";
			std::getline(cin, nowawartosc);
			(*it).ustawNazwisko(nowawartosc);
			break;
		case 2: //Zmien imie
			cout << "Wprowadz imie:\n";
			std::getline(cin, nowawartosc);
			(*it).ustawImie(nowawartosc);
			break;
		case 3: //Zmien nr telefonu
			cout << "Wprowadz nr telefonu:\n";
			std::getline(cin, nowawartosc);
			(*it).ustawNrTel(nowawartosc);
			break;
		case 4: //Zmien adres email
			cout << "Wprowadz adres email:\n";
			std::getline(cin, nowawartosc);
			(*it).ustawEmail(nowawartosc);
			break;
		default:
			break;
		}
		/*Wywolujemy metode zapisania zmian, nie potrzebujemy tutaj pytac uzytkownika "Czy na pewno?",
		poniewaz w kazdej chwili mozna wrocic wpisac stara wartosc*/
		this->zapiszzmiany();

	}
	else {
		//Wyswietlenie stosownego komunikatu w przypadku niepowodzenia
		cout << "Nie znaleziono klienta o podanym numerze ID.\n";
	}
}

  std::string ListaKlientow::getNazwiskoKlienta(int id_klienta) {
	/* Getter nazwiska klienta na podstawie 'id_klienta'*/
	auto it = Lista.begin();
	for (; it != Lista.end(); ++it) {
		if ((*it).id_klienta == id_klienta) {
			break;
		}
	}
	return (*it).nazwisko;
}

  std::string ListaKlientow::getImieKlienta(int id_klienta) {
	/* Getter imienia klienta na podstawie 'id_klienta'*/
	auto it = Lista.begin();
	for (; it != Lista.end(); ++it) {
		if ((*it).id_klienta == id_klienta) {
			break;
		}
	}
	return (*it).imie;
}

  bool ListaKlientow::czyistniejrekord(int id_klienta) {
	/* Funkcja sprawdzajaca istnienie klienta w bazie na podstawie 'id_klienta'*/
	for (auto it = Lista.begin(); it != Lista.end(); ++it) {
		if ((*it).id_klienta == id_klienta) {
			return true;
		}
	}
	return false;
}

  void ListaKlientow::wczytajDostepnoscKlientow() {
	/*Funkcja odpowiedzialna za wczytywanie danych dostepnosci klientow z pliku binarnego*/
	//Proba otwarcia pliku binarnego z baza dostepnoscilientow
	this->Plik_DostepnoscKlientow.open(this->Plik_DostepnoscKlientow_Nazwa, std::ios::binary | std::ios::in);
	//Sprawdzenie czy plik udalo sie otworzyc
	if (this->Plik_DostepnoscKlientow.is_open()) {
		//Utworzenie obiektu klasy DostepnoscKlienta, ktory bedzie sluzyl wczytywaniu danych z pliku
		DostepnoscKlienta odczytany;
		while (Plik_DostepnoscKlientow.read(reinterpret_cast<char*>(&odczytany), sizeof(odczytany))) {
			this->Lista_dostepnosc.push_back(odczytany); //Dodajemy rekord do bazy danych
		}

		//Sprawdzamy czy byly w pliku jakiekolwiek dane aby ustalic jakie bylo ostatnie ID rekordu
		if (Lista_dostepnosc.size() > 0) {
			//Jezeli w pliku byly dane ustawiamy w polu statycznym ostatnie ID rekordu aby zachowac unikalnosc kluczy
			int ostatnie_id = Lista_dostepnosc[Lista_dostepnosc.size() - 1].id_dostepnosci;
			DostepnoscKlienta::ostatnie_id = ostatnie_id;
		}
	}
	else {
		//Komunikat na wypadek niepowodzenia
		cout << "Nie udalo sie otworzyc Plik_DostepnoscKlientow\n";
	}
	//Zamkniecie pliku
	this->Plik_DostepnoscKlientow.close();
}

  void ListaKlientow::zapiszzmiany_dostepnosc() {
	/*Funkcja odpowiedzialna za zapisywanie zmian w bazie danych dostepnosci klientow do pliku binarnego*/
	//Proba otwarcia pliku binarnego z baza dostepnosci klientow
	this->Plik_DostepnoscKlientow.open(this->Plik_DostepnoscKlientow_Nazwa, std::ios::binary | std::ios::out);
	//Sprawdzenie czy plik udalo sie otworzyc
	if (this->Plik_DostepnoscKlientow.is_open()) {
		for (auto it = Lista_dostepnosc.begin(); it != Lista_dostepnosc.end(); ++it) {
			//Iterujac po calej bazie danych zapisujemy rekord po rekordzie do pliku binarnego
			Plik_DostepnoscKlientow.write(reinterpret_cast<char*>(&(*it)), sizeof((*it)));
			
		}
		cout << "Zapisano zmiany w pliku DostepnoscKlientow\n";
	}
	else {
		//Komunikat na wypadek niepowodzenia
		cout << "Nie udalo sie zapisac Plik_DostepnoscKlientow\n";
	}
	//Zamkniecie pliku
	Plik_DostepnoscKlientow.close();
}

  void ListaKlientow::usunprzedzial(int id_dostepnosci) {
	/* Funkcja pomocnicza pozwalajaca usunac konkretny przedzial czasowy dyspozycyjnosci danego klienta*/
	//Iterator bazy danych dostepnosci
	auto it = Lista_dostepnosc.begin();
	//Flaga zapobiegajaca wyswietleniu falszywego komunikatu w sytuacji gdy usuwany rekord jest ostatni w bazie
	//i metoda erase() zwraca identyfikator konca, czyli Lista_dostepnosc.end()
	bool usunieto = false;
	//Petla iterujaca cala baza az do napotkania rekordu o podanym ID
	while (it != Lista_dostepnosc.end()) {
		if ((*it).id_dostepnosci == id_dostepnosci) {
			//Jezeli udalo sie "namierzyc" rekord, powiadamiamy o tym uzytkownika komunikatem
			cout << "Usunieto przedzial czasowy.\n";
			//Usuwamy rekord z bazy danych
			it = Lista_dostepnosc.erase(it);
			//Ustawiamy flage na wartosc prawdziwa
			usunieto = true;
			break;
		}
		//Jezeli nie odnieslismy sukcesu iterujemy dalej:
		++it;
	}
	if (it == Lista_dostepnosc.end() && usunieto == false) {
		//W razie niepowodzenia wyswietlamy stosowny komunikat
		cout << "Wprowadzone ID dostepnosci jest nieprawidlowe.\n";
	}
}

  void ListaKlientow::dodajprzedzial(int id_klienta) {
	/* Funkcja odpowiedzialna z interfejs dodawania przedzialu dyspozycyjnosci dla danego klienta*/

	//Zmienna przechowujaca iterator rekordu danego klienta, wiemy ze klient na pewno istnieje, bo
	//wynika to z nadinterfejsu prowadzacego do tego interfejsu, wiec nie ma potrzeby tworzenia
	//dodatkowych instrukcji warunkowych
	auto it = znajdz(id_klienta);
	//Wyswietlenie dostepnych opcji
	cout << "Wybierz dzien:\n1 - Poniedzialek\n2 - Wtorek\n3 - Sroda\n4 - Czwartek\n5 - Piatek\n6 - Sobota\n7 - Niedziela\n0 - Wyjdz z menu\n";
	//Zmienna przechowujaca wybor opcji menu
	int wybor{};
	//Wczytanie wyboru ze standardowego wejscia
	cin >> wybor;
	//Zmienna przechowujaca numer indeksu odpowiadajemu danemu dniu
	int wskaznik{};
	//Flaga warunku wyjscia z interfejsu
	bool czywyjsc = false;
	switch (wybor) {
		//Wybor ktorejs z instrukcji ustawia wartosc zmiennej 'wskaznik'
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

	//Jezeli wybor nie padl na zadna z wartosci program nie przechodzi do dalszej czesci funkcji, poniewaz
	//spowodowaloby to wystapienie bledow
	if (wybor <1 || wybor >7) {
		//Ustawienie flagi
		czywyjsc = true;
	}

	//Jezeli flaga 'czywyjsc' ma wartosc falszywa, program kontynuuje wykonywanie instrukcji
	if (!czywyjsc) {
		cout << "Wybrales dzien: " << DniTygodnia[wskaznik] << '\n';
		//DOLNA GRANICA
		//Wyswietlamy komunikat z instrukcja
		cout << ":Dolna granica:\n";
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

		//GORNA GRANICA
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

		//Obliczenie wartosci indeksu dolnej i gornej granicy przedzialu czasowego
		int dolny_indeks = (dolna_godzina * 60 + dolna_minuta) / Parametry::zaokraglenie;
		int gorny_indeks = (gorna_godzina * 60 + gorna_minuta) / Parametry::zaokraglenie;
		gorny_indeks= gorny_indeks - 1;//Przedzial zamyka sie wtedy gdy inny sie otwiera
		//Zmienna przechowujaca dlugosc przedzialu czasowego
		int warunekistnienia = gorny_indeks - dolny_indeks+1;

		//Sprawdzenie czy dlugosc przedzialu czasowego przypadkiem nie jest mniejsza od czasu trwania lekcji,
		//co spowodowaloby bezsens istnienia rekordu o takim przedziale w bazie
		if (warunekistnienia < Parametry::jednostkaczasowa / Parametry::zaokraglenie) {
			//Wyswietlenie stosownego komunikatu w przypadku niepowodzenia
			cout << "Przedzial czasowy jest mniejszy od jednostki czasowej. Sprobuj jeszcze raz.\n";
		}
		else {
			//Przedzial czasowy spelnia warunki
			//Nowy rekord bazy dostepnosci klientow
			DostepnoscKlienta Nowy(id_klienta, wskaznik, dolny_indeks, gorny_indeks);
			//Dodajemy rekord do bazy
			this->Lista_dostepnosc.push_back(Nowy);
			//Wyswietlenie dodanego przedzialu czasowego, aby uzytkownik widzial czy nie zostaly popelnione zadne bledy
			cout << DniTygodnia[wskaznik] << " ";
			Parametry::wyswietlgodzine(dolny_indeks);
			cout << "-";
			Parametry::wyswietlgodzine(gorny_indeks+1);
			cout << '\n';
		}
	}
}

  void ListaKlientow::wyswietldyspozycyjnosc(int id_klienta) {
	/* Funkcja odpowiedzialna za wyswietlanie rekordow o dyspozycyjnosci dla danego klienta
	Nie musimy sprawdzac poprawnosci numeru ID przekazanego do funkcji, poniewaz bledny numer ID nie
	spowoduje bledow dzialania*/

	//Kolekcja bedaca kopia tylko tych rekordow z bazy, ktore sa przypisane danemu klientowi
	std::vector<DostepnoscKlienta>MiniLista;
	//Petla for iterujaca po calej bazie w szukaniu rekordow przypisanych danemu klientow
	for (auto it = Lista_dostepnosc.begin(); it != Lista_dostepnosc.end(); ++it) {
		if ((*it).id_klienta == id_klienta) {
			//W razie spelnienia warunku rekord dodajemu do naszej mini-kolekcji
			MiniLista.push_back((*it));
		}
	}

	//Sprawdzamy czy jakiekolwiek rekordy zostaly dodane dla klienta o podanym numerze ID
	if (MiniLista.size() > 0) {
		//Wyswietlamy kolumny tabeli
		cout << "ID\tDzien\tPrzedzial czasowy\n";
		//Funkcja sortujaca mini-kolekcje w porzadku dni tygodnia (od poniedzialku do niedzieli)
		std::sort(MiniLista.begin(), MiniLista.end(), [](const DostepnoscKlienta &a, const DostepnoscKlienta &b) -> bool { return a.dzien > b.dzien;  });
		for (auto it = MiniLista.begin(); it != MiniLista.end(); ++it) {
			//Wyswietlenie ID rekordu
			cout << (*it).id_dostepnosci << '\t';
			//Wyswietlenie nazwy dnia
			cout << DniTygodnia[(*it).dzien] << '\t';
			//Wyswietlenie przedzialu czasowego
			Parametry::wyswietlgodzine((*it).start);
			cout << '-';
			Parametry::wyswietlgodzine((*it).koniec+1);
			cout << '\n';
		}

	}
	else {
		//Wyswietlenie komunikatu na wypadek niepowodzenia
		cout << "BRAK DANYCH dla klienta o podanym numerze ID.\n";
	}

}

  void ListaKlientow::dyspozycyjnosc() {
	/* Funkcja odpowiedzialna za interfejs dyspozycyjnosci klienta*/

	//Komunikat z instrukcja
	cout << "Wprowadz ID klienta, ktoremu zmienic dyspozycyjnosc:\n";
	//Zmienna przechowujaca ID danego klienta
	int szukane_id;
	//Wprowadzenie ID klienta ze strumienia wejsciowego
	cin >> szukane_id;
	//Iterator odpowiadajacy rekordowi bazy odpowiadajacemu szukanemu klientu
	auto it = this->znajdz(szukane_id);

	//Sprawdzamy czy znaleziono klienta o podanym ID
	if (it != Lista.end()) {
		//Flaga warunku wykonywania sie petli wyswietlajacej mini-interfejs zarzadzania dyspozycyjnoscia
		bool warunekpetli = true;
		while (warunekpetli) {
			//Wyswietlamy dotychczas dodane przedzialy czasowe danego klienta, aby uzytkownik mial swiadomosc
			//co juz jest dodane, co moze dodac lub usunac
			cout << "::Dotychczasowa dyspozycyjnosc klienta "<<(*it).nazwisko<<' '<< (*it).imie <<" [ID: "<<(*it).id_klienta<<"]::\n";
			this->wyswietldyspozycyjnosc(szukane_id);

			//Wyswietlenie instrukcji obslugi menu
			cout << "1 - Dodaj przedzial czasowy\n2 - Usun przedzial czasowy\n0 - Wroc do poprzedniego menu\n";
			//Zmienna przechowujaca wybor pozycji menu
			int wybor = -1;
			//Wczytanie wyboru uzytkownika ze strumienia wejsciowego
			cin >> wybor;
			//Zmienna do przechowywania ID dostepnosci w razie potrzeby usuniecia przedzialu z bazy
			int id_dostepnosci;

			switch (wybor) {
			case 1: //Dodaj przedzial czasowy
				this->dodajprzedzial(szukane_id);
				this->zapiszzmiany_dostepnosc();
				break;
			case 2: //Usun przedzial czasowy
				cout << "Podaj ID dostepnosci do usuniecia(widoczne wyzej)\n";
				cin >> id_dostepnosci;
				this->usunprzedzial(id_dostepnosci);
				this->zapiszzmiany_dostepnosc();
				break;
			default: //Wybor powrotu do poprzedniego menu
				warunekpetli = false;
				break;
			}
			//Zapisanie zmian
			
		}
	}
	else {
		//Wyswietlenie stosownego komunikatu w razie niepowodzenia
		cout << "Nie znaleziono klienta o podanym numerze ID.\n";
	}

}

  void ListaKlientow::usunwszystkiedostepnosci_idklienta(int id_klienta) {
	/* Funkcja przeznaczona do usuwania wszystkich rekordow z bazy od danym 'id_klienta'
	Funkcji uzywa modul ListaKlientow do usuwania wszystkich zamowien dla danego klienta jezeli klient istnieje*/
	for (auto it = Lista_dostepnosc.begin(); it != Lista_dostepnosc.end(); ++it) {
		if ((*it).id_klienta == id_klienta) {
			it = Lista_dostepnosc.erase(it);
		}
	}
}
std::vector<Klient>ListaKlientow::Lista;
std::vector<DostepnoscKlienta>ListaKlientow::Lista_dostepnosc;