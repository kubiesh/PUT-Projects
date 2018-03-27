#include "stdafx.h"
//#include "Jadro_programu.hpp"

Program::Program()
{
}

 Program::~Program() {}

void Program::menuglowne() {
	int wybor = -1;
	bool warunekpetli = true;
	while (warunekpetli) {
		cout << "::MENU GLOWNE::\n";
		cout << "1 - Parametry glowne\n2 - Prowadzone zajecia \n3 - Klienci i ich dyspozycyjnosc\n"
			<< "4 - Preferencje klientow \n5 - Generowanie planu\n0 - Zakoncz program\n";
		cin >> wybor;
		switch (wybor) {
		case 1:
			//Wyswietlenie interfejsu parametrow programu:
			this->ModulParametry_programu.menu();
			break;
		case 2:
			//Wyswietlenie interfejsu do zarzadzania lista przedmiotow:
			this->Lista_przedmiotow.menu();
			break;
		case 3:
			//Wyswietlenie interfejsu do zarzadzania lista klientow:
			this->Lista_klientow.menu();
			break;
		case 4:
			this->Lista_zamowien.menu();
			break;
		case 5:
			if (this->Lista_przedmiotow.Lista.size() == 0 || this->Lista_klientow.Lista.size() == 0 || this->Lista_klientow.Lista_dostepnosc.size() == 0 || this->Lista_zamowien.Lista.size() == 0) {
				cout << "Algorytm nie wygeneruje planu, poniewaz w ktorejs z baz brakuje danych.\n";
			}
			else {
				std::vector<Generator> TablicaWygenerowanychPlanow;
				
				for (int i = 0; i < 20; i++) {
					Generator GeneratorPlanu;
					GeneratorPlanu.FunkcjaGlowna();
					TablicaWygenerowanychPlanow.push_back(GeneratorPlanu);
					cout << "Numer generacji: " << i << '\n';
				}
				std::sort(TablicaWygenerowanychPlanow.begin(), TablicaWygenerowanychPlanow.end(), [](const Generator &a, const Generator &b) ->bool {return a.WygenerowanyPlan.przychod < b.WygenerowanyPlan.przychod; });
				TablicaWygenerowanychPlanow[TablicaWygenerowanychPlanow.size() - 1].PlanDoPliku();
			}
			break;
		default:
			warunekpetli = false;
			break;
		}
	}
}
