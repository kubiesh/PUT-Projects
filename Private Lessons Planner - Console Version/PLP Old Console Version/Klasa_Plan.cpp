#include "stdafx.h"

Plan::Plan() {
	this->przychod = 0;
	this->iloscZajec = 0;
	for (int i = 0; i < 7; i++) {
		czySaZajeciaDzien[i] = false;
	}
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 720; j++) {
			this->rozkladZajec[i][j] = Parametry::Dostepnosc[i][j];
		}
	}


}

Plan::Plan(int rozkladZajec[7][720]) {
	this->przychod = 0;
	this->iloscZajec = 0;
	for (int i = 0; i < 7; i++) {
		czySaZajeciaDzien[i] = false;
	}
	//rozkladZajec
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 720; j++) {
			this->rozkladZajec[i][j] = rozkladZajec[i][j];
		}
	}

}
