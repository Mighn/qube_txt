// Qube - prosta gra tekstowa.
#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include "qube.h"
#include "player.h"

// prototypy funkcji

//glowny modul gry
int	mainGame(player *p_allData, qube **p_p_matrix);
//analiza ruchu
int	move(player *p_allData, qube **p_p_matrix);
//opis pomieszczenia po wejscu
void opisQ(player *p_allData, qube **p_p_matrix);
//blok poruszania sie
int	menuQ(player *p_allData, qube **p_p_matrix);
//rozliczenie końcowe gry
int	gameOver(player *p_allData, qube **p_p_matrix);
//inicjacja i wprowadzanie danych do tablicy struktur planszy gry
void defineQ(player* p_allData, qube** p_p_matrix, int, int);
//realizacja wysokiej trudnosci gry
void hardmode(player* p_allData, qube** p_p_matrix, int, int);
//inicjacja HP i Time
void hpTimeInit(player* p_allData);

int main()
{
	srand(time(NULL));
	int xM = 10;			// deklaracja rozmiaru gry
	int yM = xM;			// deklaracja rozmiaru gry
	int end = 0; // !=0 - koniec petli pomocniczej

	do		//petla pomocnicza
	{
		qube **p_p_matrix = new qube*[xM];		//tblica obiektow qube wraz z inicjacja
		for (int i = 0; i < yM; i++)
		{
			p_p_matrix[i] = new qube[yM];
		}
		player *p_allData = new player(xM, yM);	// obiekt danych gracza

		system("cls");

		std::cout << "Witam w qube txt" << std::endl;
		std::cout << "\nzaraz wejdziesz do losowego boxu " << std::endl;
		std::cout << "twoim zadaniem bedzie wydostanie sie na zewnatrz" << std::endl;
		std::cout << "w tym celu uzyj klawiszy WSAD :" << std::endl;
		std::cout << "\n W - polnoc\n E - poludnie\n A - zachod\n D - wschod" << std::endl;
		std::cout << "spokojnie - nie musisz tego zapamietac - bedzie to podawane podczas gry :)" << std::endl;
		std::cout << "\nNa poczatek masz 10 punktow zycia.\nCzesc pokojow zadaje obrazena a odpoczynek w czesci pokojow leczy rany" << std::endl;
		std::cout << "odpoczywasz uzywajac klawisza Q" << std::endl;
		std::cout << "masz okreslona ilosc jednostek czasu a kazdy pokoj zabiera pewna ilosc czasu na ture" << std::endl;
		std::cout << "\nzycze milej zabawy, nacisnij dowolny klawisz aby zaczac." << std::endl;
		std::cout << "koncowy wynik zalezy od czasu przejscia i pozostalgo zdrowia" << std::endl;
		std::cout << "\n\nwybierz trudnosc gry: 1 - latwa 2- trudna - uwaga, gra moze byc sporadycznie nie mozliwa do ukonczenia " << std::endl;

		int tmp;
		do
		{
			tmp = 0;
			char tempMode;
			tempMode = _getch();
			if (tempMode == '1')p_allData->setHardMode(false);
			else if (tempMode == '2')p_allData->setHardMode(true);
			else tmp = 1;
		} while (tmp == 1);

		hpTimeInit(p_allData);										  //inicjacja HP i Time
		defineQ(p_allData, p_p_matrix, xM, yM);							  //uzupełnianie danych
		if (p_allData->getHardMode()) hardmode(p_allData, p_p_matrix, xM, yM); //realizacja wysokiej trudnosci gry
		p_allData->setEndgame(mainGame(p_allData, p_p_matrix));			  //pętla główna gry
		end = gameOver(p_allData, p_p_matrix);							  //koniec gry
			 // czyszczenie pamieci na koniec rozgrywki lub koniec gry
		for (int i = 0; i < yM; i++)
		{
			delete[] p_p_matrix[i];
		}
		delete[] p_p_matrix;
		delete p_allData;
	} while (end == 0);
	return 0;
}

void  defineQ(player *p_allData, qube **p_p_matrix, int xM, int yM)
{
	int  p = 1;			//numerator nazwy pokoju

	for (int i = 0; i < xM; i++)
		for (int j = 0; j < yM; j++)
		{
			p_p_matrix[i][j].qubeIni(p);								//inicjacja główmna lokacji
			p++;

			if (i == 0) p_p_matrix[i][j].setExitQ(0, false);			//dla blokady w
			if (j == (yM - 1)) p_p_matrix[i][j].setExitQ(1, false);	//dla blokady d
			if (i == (xM - 1)) p_p_matrix[i][j].setExitQ(2, false);	//dla blokady s
			if (j == 0) p_p_matrix[i][j].setExitQ(3, false);			//dla blokady a
		}
	if (p_allData->getHardMode()) hardmode(p_allData, p_p_matrix, xM, yM);
}

void hardmode(player* p_allData, qube** p_p_matrix, int xM, int yM)
{
	for (int i = 1; i < (xM - 1); i++)
		for (int j = 1; j < (yM - 1); j++)
		{
			int tmp = (std::rand() % 4);
			p_p_matrix[i][j].setExitQ(tmp, false);
		}
}

void hpTimeInit(player* p_allData)
{
	if (p_allData->getHardMode())p_allData->setHp(40);			//zdrowie gracza
	else p_allData->setHp(30);

	if (p_allData->getHardMode())p_allData->setTimer(150);		// deklaracja ilosci rund
	else p_allData->setTimer(30);
}

int mainGame(player *p_allData, qube **p_p_matrix)
{
	p_allData->setEndgame(0);	//znacznik końca gry 0-gramy | 1-zdrowie | 2-ucieczka | 3-wygrana! | 4-koniec czasu
	int door = 5;			//znacznik pokonanych ostatnio drzwi (odwrotny wdsa) 5 - pierwsze wejscie
	p_allData->setMoveP(9);		//oznaczenie wyboru gracza 0-w |1-d |2-s |3-a |4-a |5-rest |6- exit | 9 - wartosc startowa

	system("cls");
	std::cout << "\n\n\n\nZaraz wejdziesz do gry!!!" << std::endl;
	std::cout << "\nzaczniesz w miejscu oznaczonym jako " << p_p_matrix[p_allData->getActX()][p_allData->getActY()].getInfoQ() << std::endl;
	std::cout << "wyjscie znajduje sie w pokoju oznaczonym jako " << p_p_matrix[p_allData->getEndX()][p_allData->getEndY()].getInfoQ() << std::endl;
	std::cout << "\nzapamietaj to, bo od teraz podawany bedzie tylko numer pokoju w ktorym aktualnie jestes!" << std::endl;
	std::cout << "\n\nWspinasz sie po drabince do pomieszczenia od ktorego zaczniesz gre," << std::endl;
	std::cout << "celem jest dotrzec jak najszybciej do pomieszczenia z taka sama klapa w podlodze i wyjscie" << std::endl;
	std::cout << "\n\nNacisnij dowolny klawisz" << std::endl;

	_getch();

	do					//głowna pętla gry
	{
		system("cls");
		int tempHp = p_allData->getHp();
		p_allData->setHp(tempHp += p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ());  //zmiana HP gracza w nowym pokoju
		if (p_allData->getHp() <= 0) return 1;
		int tmpMov = p_allData->getMov();
		p_allData->setMov(tmpMov += p_p_matrix[p_allData->getActX()][p_allData->getActY()].getTimeQ());  //zmiana rundy gracza w nowym pokoju
		if (p_allData->getMov() >= p_allData->getTimer()) return 4;

		std::cout << "    HP  " << p_allData->getHp()
			<< "   zakonczone rundy  " << p_allData->getMov()
			<< "    Pozostalo czasu  " << p_allData->getTimer() - p_allData->getMov() << std::endl << std::endl;

		opisQ(p_allData, p_p_matrix);
		p_allData->setMoveP(menuQ(p_allData, p_p_matrix));
		p_allData->setEndgame(move(p_allData, p_p_matrix));
	} while (p_allData->getEndgame() == 0);

	return p_allData->getEndgame();
}

void opisQ(player *p_allData, qube **p_p_matrix)
{
	std::cout << "wchodzisz do pomieszczenia przez ";
	if (p_allData->getMoveP() == 9) std::cout << "klape w podlodze " << std::endl;
	if (p_allData->getMoveP() == 0) std::cout << "poludniowe drzwi " << std::endl;
	if (p_allData->getMoveP() == 1) std::cout << "zachodnie drzwi " << std::endl;
	if (p_allData->getMoveP() == 2) std::cout << "polnocne drzwi " << std::endl;
	if (p_allData->getMoveP() == 3) std::cout << "wschodnie drzwi " << std::endl;

	if (p_allData->getMoveP() == 0 && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(2) == false) std::cout << "Drzwi za toba zatrzaskuja sie" << std::endl;
	if (p_allData->getMoveP() == 1 && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3) == false) std::cout << "Drzwi za toba zatrzaskuja sie" << std::endl;
	if (p_allData->getMoveP() == 2 && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(0) == false) std::cout << "Drzwi za toba zatrzaskuja sie" << std::endl;
	if (p_allData->getMoveP() == 3 && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1) == false) std::cout << "Drzwi za toba zatrzaskuja sie" << std::endl;

	std::cout << "\nw jednolicie bialym pokoju widzisz tylko na kazdej scianie napis \n\n    \""
		<< p_p_matrix[p_allData->getActX()][p_allData->getActY()].getInfoQ() << "\"" << std::endl;
	std::cout << "\no ile pamietasz jestes w tym pokoju po raz " << p_p_matrix[p_allData->getActX()][p_allData->getActY()].getBeInsideQ();
	int tmpIn = p_p_matrix[p_allData->getActX()][p_allData->getActY()].getBeInsideQ(); //robocze beInside
	p_p_matrix[p_allData->getActX()][p_allData->getActY()].setBeInsideQ(++tmpIn);    //dodanie liczby odwiedzin w pokoju
	std::cout << "\n\nwychodza z tad drzwi na ";
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(0)) std::cout << "polnoc ";
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1)) std::cout << "wschod ";
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(2)) std::cout << "poludnie ";
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3)) std::cout << "zachod ";
	if (p_allData->getActX() == p_allData->getStartX() && p_allData->getActY() == p_allData->getStartY()) std::cout
		<< "\nw podloze widzisz klape przez ktora sie tu dostales" << std::endl;
	if (p_allData->getActX() == p_allData->getEndX() && p_allData->getActY() == p_allData->getEndY())std::cout
		<< "\n w podlodze widzisz nieznana klape, czyzby upragnione wyjscie??" << std::endl;
	std::cout << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(0)) std::cout << "  _|____*   *____|_  " << std::endl;
	else std::cout << "  _|____*___*____|_  " << std::endl;
	std::cout << "   |             |   " << std::endl;
	std::cout << "   |             |   " << std::endl;
	std::cout << "   *             *   " << std::endl;
	if ((p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1) && (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3))))
		std::cout << "                     " << std::endl;
	if ((p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3)) && !(p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1)))
		std::cout << "                 |   " << std::endl;
	if ((p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1)) && !(p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3)))
		std::cout << "   |                 " << std::endl;
	if (!(p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1)) && !(p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3)))
		std::cout << "   |             |  " << std::endl;
	std::cout << "   *             *   " << std::endl;
	std::cout << "   |             |   " << std::endl;
	std::cout << "   |             |   " << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(2)) std::cout << "  _|____     ____|_  " << std::endl;
	else std::cout << "  _|____ ___ ____|_  " << std::endl;
	std::cout << "   |    *   *    |   " << std::endl;

	std::cout << "\nJednoczesnie czujesz ze w pokoju panuje ";
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ() == 0) std::cout << "neutralna atmosfera " << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ() > 0) std::cout << "przyjazna atmosfera i orzymujesz "
		<< p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ() << " HP" << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ() < 0) std::cout << "szkodliwa atmosfera i orzymujesz "
		<< p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ() << " HP" << std::endl;
	std::cout << "\nPobyt w tym pokoju kosztuje " << p_p_matrix[p_allData->getActX()][p_allData->getActY()].getTimeQ() << " rund" << std::endl;
}

int menuQ(player *p_allData, qube **p_p_matrix)
{
	std::cout << std::endl;
	std::cout << "nacisnij E zeby odpoczac w pokoju jedna runde;  R - zrezygnuj i wyjdz z \n" << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(0))  std::cout << "nacisnij W zeby isc na polnoc" << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1))  std::cout << "nacisnij D zeby isc na wschod" << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(2))  std::cout << "nacisnij S zeby isc na poludie" << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3))  std::cout << "nacisnij A zeby isc na zachod" << std::endl;
	if (p_allData->getActX() == p_allData->getEndX() && p_allData->getActY() == p_allData->getEndY())  std::cout
		<< "\n\a\a****** nacisnij Q zeby skorzystac z klapy w podlodze *****" << std::endl;

	int tst = 0;
	char moveC;						//znak z klawiatury
	p_allData->setMoveP(9);			//oznaczenie wyboru gracza 0-w |1-d |2-s |3-a |4-a |5-rest |6- exit

	do
	{
		tst = 0;
		moveC = _getch();			//pobieranie znaku z klawiatury

		if ((moveC == 'w' || moveC == 'W') && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(0)) p_allData->setMoveP(0);
		else if ((moveC == 'd' || moveC == 'D') && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1)) p_allData->setMoveP(1);
		else if ((moveC == 's' || moveC == 'S') && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(2)) p_allData->setMoveP(2);
		else if ((moveC == 'a' || moveC == 'A') && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3)) p_allData->setMoveP(3);
		else if ((moveC == 'q' || moveC == 'Q') && (p_allData->getActX() == p_allData->getEndX() && p_allData->getActY() == p_allData->getEndY())) p_allData->setMoveP(4);
		else if (moveC == 'e' || moveC == 'E') p_allData->setMoveP(5);
		else if (moveC == 'r' || moveC == 'R') p_allData->setMoveP(6);
		else tst = 1;
	} while (tst == 1);
	return p_allData->getMoveP();
}

int move(player *p_allData, qube **p_p_matrix)
{
	int x = p_allData->getActX();
	int y = p_allData->getActY();

	if (p_allData->getMoveP() == 0) { --x; p_allData->setActX(x); }
	else if (p_allData->getMoveP() == 1) { ++y; p_allData->setActY(y); }
	else if (p_allData->getMoveP() == 2) { ++x; p_allData->setActX(x); }
	else if (p_allData->getMoveP() == 3) { --y; p_allData->setActY(y); }
	else if (p_allData->getMoveP() == 4) return 3;
	else if (p_allData->getMoveP() == 5) return 0;
	else if (p_allData->getMoveP() == 6) return 2;
	else menuQ(p_allData, p_p_matrix);

	return 0;
}

int gameOver(player *p_allData, qube **p_p_matrix)
{
	system("cls");
	std::cout << "    HP  " << p_allData->getHp() << "   zakonczone rundy  " << p_allData->getMov() << "    Pozostalo czasu  "
		<< p_allData->getTimer() - p_allData->getMov() << std::endl << std::endl;
	if (p_allData->getEndgame() == 1) std::cout << "\nNiestety twoje zdrowie spadlo do zera... umarles :) ";
	if (p_allData->getEndgame() == 2) std::cout << "\nUciekasz wyjsciem bezpieczenstwa przed zakonczeniem zadania.";
	if (p_allData->getEndgame() == 4) std::cout << "\nNiestety koniec czasu - przegrales.";
	if (p_allData->getEndgame() == 3)
	{
		std::cout << "\n\nGRATULACJE udalo ci sie wygrac !!!!";
		std::cout << "\nuzyskales " << p_allData->getHp() / 2 + (p_allData->getTimer() - p_allData->getMov()) << " punktow" << std::endl;
	}
	std::cout << "\nKoniec gry nacisnij Q\nponowna gra nacisnij E" << std::endl;
	int tst = 0;
	do
	{
		char a = 0;
		a = _getch();
		if (a == 'e' || a == 'E')return 0;
		else if (a == 'q' || a == 'Q')return 1;
		else tst = 1;
	} while (tst == 1);
	return 1;
}