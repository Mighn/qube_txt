#include "stdafx.h"
#include "qube.h"
#include <iostream>

qube::qube()
{
	beInsideQ = 1;
	exitQ[0] = true;
	exitQ[1] = true;
	exitQ[2] = true;
	exitQ[3] = true;
}

void qube::qubeIni(int p)						//inicjacja g��wmna lokacji
{
	infoQ = "pokoj numer  ";					//opis pokoju - moze z pliku
	infoQ += std::to_string(p); p++;			//numer pokoju
	hpQ = (std::rand() % 5) - 2;				//losowe okreslenie obrazem lub leczenia w zakresie +/-2
	timeQ = (std::rand() % 4) + hpQ;			//Im bezpieczniej, tym czas szybciej idzie
}

qube::~qube()
{
}