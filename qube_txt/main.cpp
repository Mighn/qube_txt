/*Qube - a simple text game.
* Created at the beginning as a simple structural porogram, and then as you acquire knowledge
* transformed into an increasingly advanced object program.
* at https://github.com/Mighn/qube_txt/commits/master I put all the development of the code.
* I also use this program to learn the basic functionalities of Github.
*/
#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include "qube.h"
#include "player.h"
#include "windows.h"
//#include "eventQ.h"			 //during development

//main loop of the game
int startGame(player* p_allData, qube** p_p_matrix, int, int);
//basic game module
int mainGame(player* p_allData, qube** p_p_matrix);
//description of the room after the entrance
void opisQ(player* p_allData, qube** p_p_matrix);
//block of movement
int menuQ(player* p_allData, qube** p_p_matrix);
//initiation and entering data into the board of game structures
void defineQ(player* p_allData, qube** p_p_matrix, int, int);
//implementation high difficulty of game
void hardmode(qube** p_p_matrix, int, int);

int main()
{
	//randomizer initialization
	srand(static_cast<unsigned int>(time(nullptr)));

	//changing the size of the console window ......... copy-paste, I do not cover it myself yet - but it works (windows.h lib);)
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); // save the console dimensions
	MoveWindow(console, r.left, r.top, 900, 600, TRUE); // width, height

	//maybe in the future a menu with a selection of board size and difficulty level (hardmode) moved from startGame ()
	const int xM = 10; //declaration of the size of the game
	const int yM = xM; //declaration of the size of the game
	int end;	 //! = 0 - end of auxiliary loop

	do // helper game loop (life cycle of objects and indicators)
	{
		const auto p_p_matrix = new qube*[xM]; //array of qube objects with initiation
		for (int i = 0; i < yM; i++)
			p_p_matrix[i] = new qube[yM];

		const auto p_allData = new player(xM, yM); // player data object

		//start of the game
		end = startGame(p_allData, p_p_matrix, xM, yM);

		//clearing the memory at the end of the tour of game or ending the game
		for (int i = 0; i < yM; i++)
			delete[] p_p_matrix[i];
		delete[] p_p_matrix;
		delete p_allData;
	} while (end == 0);
	return 0;
}
int startGame(player* p_allData, qube** p_p_matrix, const int xM, const int yM)
{
	system("cls");
	std::cout << "Witam w qube txt" << std::endl;
	std::cout << "\nzaraz wejdziesz do losowego boxu " << std::endl;
	std::cout << "twoim zadaniem bedzie wydostanie sie na zewnatrz" << std::endl;
	std::cout << "w tym celu uzyj klawiszy WSAD :" << std::endl;
	std::cout << "\n W - polnoc\n E - poludnie\n A - zachod\n D - wschod" << std::endl;
	std::cout << "spokojnie - nie musisz tego zapamietac - bedzie to podawane podczas gry :)" << std::endl;
	std::cout << "\nNa poczatek masz 10 punktow zycia.\nCzesc pokojow zadaje obrazena a odpoczynek w czesci pokojow leczy rany"
		<< std::endl;
	std::cout << "odpoczywasz uzywajac klawisza Q" << std::endl;
	std::cout << "masz okreslona ilosc jednostek czasu a kazdy pokoj zabiera pewna ilosc czasu na ture" << std::endl;
	std::cout << "\nzycze milej zabawy, nacisnij dowolny klawisz aby zaczac." << std::endl;
	std::cout << "koncowy wynik zalezy od czasu przejscia i pozostalgo zdrowia" << std::endl;
	std::cout <<
		"\n\nwybierz trudnosc gry: 1 - latwa 2- trudna - uwaga, gra moze byc sporadycznie nie mozliwa do ukonczenia " 	//after adding the key functionality will always be able to win
		<< std::endl;

	int tmp;
	do
	{
		tmp = 0;
		char tempMode = _getch();
		if (tempMode == '1')p_allData->setHardMode(false);
		else if (tempMode == '2')p_allData->setHardMode(true);
		else tmp = 1;
	} while (tmp == 1);

	p_allData->hpTimeInit(); //HP and Time initiation
	defineQ(p_allData, p_p_matrix, xM, yM); //supplementing data
	if (p_allData->getHardMode()) hardmode(p_p_matrix, xM, yM); //implementation of the high difficulty of the game
	p_allData->setEndgame(mainGame(p_allData, p_p_matrix)); // the main loop of the game

	return p_allData->gameOver(); //end of the game & return to main()
}

void defineQ(player* p_allData, qube** p_p_matrix, const int xM, const int yM)
{
	int p = 1; //rom name numerator

	for (int i = 0; i < xM; i++)
		for (int j = 0; j < yM; j++)
		{
			p_p_matrix[i][j].qubeIni(p); //main initiation of the site
			p++;
			//blockade of external doors
			if (i == 0) p_p_matrix[i][j].setExitQ(0, false); //for lock in w
			if (j == (yM - 1)) p_p_matrix[i][j].setExitQ(1, false); //for lock in d
			if (i == (xM - 1)) p_p_matrix[i][j].setExitQ(2, false); //for lock in s
			if (j == 0) p_p_matrix[i][j].setExitQ(3, false); //for lock in a
		}
	if (p_allData->getHardMode()) hardmode(p_p_matrix, xM, yM);
}

void hardmode(qube** p_p_matrix, const int xM, const int yM)
//we close one random door in each room, except for the extreme rooms (after implementation keys funcionality maybe I change this to: without excrptions)
{
	for (int i = 1; i < (xM - 1); i++)
		for (int j = 1; j < (yM - 1); j++)
		{
			int tmp = (rand() % 4);
			p_p_matrix[i][j].setExitQ(tmp, false);
		}
}

int mainGame(player* p_allData, qube** p_p_matrix)
{
	p_allData->setEndgame(0);// end marker of the game 0-continue | 1-health 2-escape 3-win! | 4-end of time | 9-loading error
	p_allData->setMoveP(9); //player selection 0-w | 1-d | 2-s | 3-a | 4-a | 5-rest | 6- exit | 9 - start value

	system("cls");
	std::cout << "\n\n\n\nZaraz wejdziesz do gry!!!" << std::endl;
	std::cout << "\nzaczniesz w miejscu oznaczonym jako " << p_p_matrix[p_allData->getActX()][p_allData->getActY()].
		getInfoQ() << std::endl;
	std::cout << "wyjscie znajduje sie w pokoju oznaczonym jako " << p_p_matrix[p_allData->getEndX()][p_allData->getEndY()]
		.getInfoQ() << std::endl;
	std::cout << "\nzapamietaj to, bo od teraz podawany bedzie tylko numer pokoju w ktorym aktualnie jestes!" << std::endl;
	std::cout << "\n\nWspinasz sie po drabince do pomieszczenia od ktorego zaczniesz gre," << std::endl;
	std::cout << "celem jest dotrzec jak najszybciej do pomieszczenia z taka sama klapa w podlodze i wyjscie" << std::endl;
	std::cout << "\n\nNacisnij dowolny klawisz" << std::endl;
	_getch();

	do //main loop of the game
	{
		system("cls");
		int tempHp = p_allData->getHp();
		p_allData->setHp(tempHp += p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ());//change player HP in a new room                     * add as a method !!!!!!!!!!!!!
		if (p_allData->getHp() <= 0) return 1; //check Health Pionts
		int tmpMov = p_allData->getMov();
		p_allData->setMov(tmpMov += p_p_matrix[p_allData->getActX()][p_allData->getActY()].getTimeQ());//change the player's round in the new room       * add as a method !!!!!!!!!!!!!
		if (p_allData->getMov() >= p_allData->getTimer()) return 4; //check timer
		// top info bar
		std::cout << "    HP  " << p_allData->getHp()
			<< "   zakonczone rundy  " << p_allData->getMov()
			<< "    Pozostalo czasu  " << p_allData->getTimer() - p_allData->getMov() << std::endl << std::endl;

		opisQ(p_allData, p_p_matrix);
		p_allData->setMoveP(menuQ(p_allData, p_p_matrix));
		p_allData->setEndgame(p_allData->move());
		if (p_allData->getEndgame() == 9) menuQ(p_allData, p_p_matrix); //re-loading the player's movement after the error
	} while (p_allData->getEndgame() == 0);
	return p_allData->getEndgame();
}

void opisQ(player* p_allData, qube** p_p_matrix)
{
	std::cout << "wchodzisz do pomieszczenia przez ";
	if (p_allData->getMoveP() == 9) std::cout << "klape w podlodze " << std::endl;
	if (p_allData->getMoveP() == 0) std::cout << "poludniowe drzwi " << std::endl;
	if (p_allData->getMoveP() == 1) std::cout << "zachodnie drzwi " << std::endl;
	if (p_allData->getMoveP() == 2) std::cout << "polnocne drzwi " << std::endl;
	if (p_allData->getMoveP() == 3) std::cout << "wschodnie drzwi " << std::endl;

	if (p_allData->getMoveP() == 0 && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(2) == false) std::
		cout << "Drzwi za toba zatrzaskuja sie" << std::endl;
	if (p_allData->getMoveP() == 1 && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3) == false) std::
		cout << "Drzwi za toba zatrzaskuja sie" << std::endl;
	if (p_allData->getMoveP() == 2 && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(0) == false) std::
		cout << "Drzwi za toba zatrzaskuja sie" << std::endl;
	if (p_allData->getMoveP() == 3 && p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1) == false) std::
		cout << "Drzwi za toba zatrzaskuja sie" << std::endl;

	std::cout << "\nw jednolicie bialym pokoju widzisz tylko na kazdej scianie napis \n\n    \""
		<< p_p_matrix[p_allData->getActX()][p_allData->getActY()].getInfoQ() << "\"" << std::endl;
	std::cout << "\no ile pamietasz jestes w tym pokoju po raz " << p_p_matrix[p_allData->getActX()][p_allData->getActY()].
		getBeInsideQ();
	int tmpIn = p_p_matrix[p_allData->getActX()][p_allData->getActY()].getBeInsideQ(); //temp beInside
	p_p_matrix[p_allData->getActX()][p_allData->getActY()].setBeInsideQ(++tmpIn); //change the player's round in the new room         * add as a method !!!!!!!!!!!!!
	std::cout << "\n\nwychodza z tad drzwi na ";
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(0)) std::cout << "polnoc ";
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1)) std::cout << "wschod ";
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(2)) std::cout << "poludnie ";
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3)) std::cout << "zachod ";
	if (p_allData->getActX() == p_allData->getStartX() && p_allData->getActY() == p_allData->getStartY())
		std::cout
		<< "\nw podloze widzisz klape przez ktora sie tu dostales" << std::endl;
	if (p_allData->getActX() == p_allData->getEndX() && p_allData->getActY() == p_allData->getEndY())
		std::cout
		<< "\n w podlodze widzisz nieznana klape, czyzby upragnione wyjscie??" << std::endl;
	std::cout << std::endl;
	//text graphics showing the room with the state of the door
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(0)) std::cout << "  _|____*   *____|_  " << std::
		endl;
	else std::cout << "  _|____*___*____|_  " << std::endl;
	std::cout << "   |             |   " << std::endl;
	std::cout << "   |             |   " << std::endl;
	std::cout << "   *             *   " << std::endl;
	if ((p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1) && (p_p_matrix[p_allData->getActX()][p_allData
		->getActY()].getExitQ(3))))
		std::cout << "                     " << std::endl;
	if ((p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3)) && !(p_p_matrix[p_allData->getActX()][
		p_allData->getActY()].getExitQ(1)))
		std::cout << "                 |   " << std::endl;
		if ((p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1)) && !(p_p_matrix[p_allData->getActX()][
			p_allData->getActY()].getExitQ(3)))
			std::cout << "   |                 " << std::endl;
			if (!(p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1)) && !(p_p_matrix[p_allData->getActX()][
				p_allData->getActY()].getExitQ(3)))
				std::cout << "   |             |  " << std::endl;
				std::cout << "   *             *   " << std::endl;
				std::cout << "   |             |   " << std::endl;
				std::cout << "   |             |   " << std::endl;
				if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(2)) std::cout << "  _|____     ____|_  " << std::
					endl;
				else std::cout << "  _|____ ___ ____|_  " << std::endl;
				std::cout << "   |    *   *    |   " << std::endl;
				std::cout << "\nJednoczesnie czujesz ze w pokoju panuje ";
				if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ() == 0) std::cout << "neutralna atmosfera " << std::
					endl;
				if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ() > 0)
					std::cout << "przyjazna atmosfera i orzymujesz "
					<< p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ() << " HP" << std::endl;
				if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ() < 0)
					std::cout << "szkodliwa atmosfera i orzymujesz "
					<< p_p_matrix[p_allData->getActX()][p_allData->getActY()].getHpQ() << " HP" << std::endl;
				std::cout << "\nPobyt w tym pokoju kosztuje " << p_p_matrix[p_allData->getActX()][p_allData->getActY()].getTimeQ() <<
					" rund" << std::endl;
}

int menuQ(player* p_allData, qube** p_p_matrix)
{
	std::cout << std::endl;
	std::cout << "nacisnij E zeby odpoczac w pokoju jedna runde;  R - zrezygnuj i wyjdz z \n" << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(0)) std::cout << "nacisnij W zeby isc na polnoc" << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(1)) std::cout << "nacisnij D zeby isc na wschod" << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(2)) std::cout << "nacisnij S zeby isc na poludie" << std::endl;
	if (p_p_matrix[p_allData->getActX()][p_allData->getActY()].getExitQ(3)) std::cout << "nacisnij A zeby isc na zachod" << std::endl;
	if (p_allData->getActX() == p_allData->getEndX() && p_allData->getActY() == p_allData->getEndY())
		std::cout << "\n\a\a****** nacisnij Q zeby skorzystac z klapy w podlodze *****" << std::endl;

	int tst;
	p_allData->setMoveP(9); // player selection 0-w | 1-d | 2-s | 3-a | 4-a | 5-rest | 6- exit

	do
	{
		tst = 0;
		char moveC = _getch(); //retrieve a character from the keyboard                   * add key support

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