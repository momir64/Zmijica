#include <windows.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <fcntl.h>
#include <thread>
#include <string>
#include <random>
#include <queue>
#include <mutex>
#include <io.h>
#include "resource.h"
#pragma comment(lib, "winmm")
#pragma warning(disable: 6031)

#define IVICAU               10
#define IVICAR               38
#define IVICAL               10
#define IVICAD               20

#define GLAVAS             L'■'
#define GLAVAG             L'▀'
#define GLAVAD             L'▄'
#define TELOX              L'━'
#define TELOY              L'┃'
#define TELO1              L'┏'
#define TELO2              L'┓'
#define TELO3              L'┛'
#define TELO4              L'┗'
#define TELODX             L'═'
#define TELODY             L'║'
#define TELOD1             L'╔'
#define TELOD2             L'╗'
#define TELOD3             L'╝'
#define TELOD4             L'╚'
#define NIŠTA              L' '
#define HRANAO             L'○'

#define PRAZNO                0
#define TELO                  1
#define TELOD                 2
#define HRANA                 4
#define NAGRADA               5

#define B0                    1
#define B1                    2
#define B2                    3
#define B3                    4
#define B4                    5
#define B5                    6
#define B6                    7
#define B7                    8
#define B8                    9
#define B9                   10
#define LINL                 11
#define LINR                 12
#define SLUS                 13
#define ESC                  14
#define UP                   15
#define RIGHT                16
#define DOWN                 17
#define LEFT                 18
#define ENTER                19
#define MUTE                 20
#define MENU                21

#define DGM                   0
#define SRO                   0
#define RAM                  10
#define RAMO                  0
#define TAST                  7
#define TASTT                12
#define TASTO                 2
#define NOKIA                 8
#define EKRAN                11
#define EKRANO                0
#define EKRAN1                9
#define EKRAN2                6
#define LINIJAC               3
#define LINIJAP              15
#define SLUSALICAZ           13
#define SLUSALICAC           14

#define XX(x) ((x)-(9))
#define YY(y) ((y)-(10))
#define XXX(x) ((x)+(9))
#define YYY(y) ((y)+(10))
#define color(a, b) (SetConsoleTextAttribute(ConsoleOutputHandle, 16 * (b) + (a)))
#define underline() (std::wcout << L"\033[4m")

using namespace std;

HANDLE ConsoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE ConsoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);
mutex mtx;

int level = 0;
int score = 0;
int tajmer = 0;
int speeds = 5;
int speed = 300;
int curlevel = 0;
int pojedeni = 0;
int tabla[31][11];
int speedcur = 300;
int exittime = 700;
int smer = RIGHT;
int smerold = RIGHT;
COORD special = { -1, -1 };
COORD hranaxy = { -1, -1 };
wstring hrana = L"₷¥ᵯ¥ᶘ₷֍₷¥ᴥ₷¥֏֎¥₷";
vector<COORD> zmija;
vector<int> input;
queue<wchar_t> prosireni;
wchar_t hranaspecial;
bool unos = true;
bool klik = false;
bool mute = false;
bool levels = true;
bool restart = false;
bool interupt = false;
bool speedauto = true;
bool mousewait = false;
bool nextlevel = false;
bool keyboardwait = false;

void MouseInput();
void KeyboardInput();
void SakriKursor();
void gotoxy(int, int);
void Mute();
void CentrirajKonzolu();
void PodesiKonzolu();
void PrintNokia();
void Animacija();
void PrintConsole(bool);
void Init(int);
COORD Hrana(bool);
void ScorePlus(int);
void SpecialTajmer(int);
void Level(bool);

void SpecialTajmer(int tajmer) {
	mtx.lock();
	color(EKRANO, EKRAN);
	underline();
	gotoxy(25, 8);  std::wcout << setfill(L'0') << setw(2) << tajmer;
	mtx.unlock();
}

void IspisiZmiju() {
	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(zmija.back().X, zmija.back().Y);
	if (zmija.back().Y == zmija[zmija.size() - 2].Y) {
		std::wcout << GLAVAS;
		if (zmija.back().X == zmija[zmija.size() - 2].X + 1)
			gotoxy(zmija.back().X - 1, zmija.back().Y);
		std::wcout << TELOX;
	}
	else if (zmija.back().Y == zmija[zmija.size() - 2].Y + 1)
		std::wcout << GLAVAG;
	else
		std::wcout << GLAVAD;
	for (int i = zmija.size() - 2; i > 0; i--) {
		gotoxy(zmija[i].X, zmija[i].Y);
		if (tabla[XX(zmija[i].X)][YY(zmija[i].Y)] == TELOD) {
			if (zmija[i].X == zmija[i + 1].X && zmija[i].X == zmija[i - 1].X)
				std::wcout << TELODY;
			else if (zmija[i].Y == zmija[i + 1].Y && zmija[i].Y == zmija[i - 1].Y)
				std::wcout << TELODX;
			else if (zmija[i].X == zmija[i + 1].X + 1 && zmija[i].Y == zmija[i - 1].Y + 1)
				std::wcout << TELOD3;
			else if (zmija[i].X == zmija[i + 1].X - 1 && zmija[i].Y == zmija[i - 1].Y + 1)
				std::wcout << TELOD4;
			else if (zmija[i].X == zmija[i + 1].X - 1 && zmija[i].Y == zmija[i - 1].Y - 1)
				std::wcout << TELOD1;
			else if (zmija[i].X == zmija[i + 1].X + 1 && zmija[i].Y == zmija[i - 1].Y - 1)
				std::wcout << TELOD2;
			else if (zmija[i].X == zmija[i - 1].X + 1 && zmija[i].Y == zmija[i + 1].Y + 1)
				std::wcout << TELOD3;
			else if (zmija[i].X == zmija[i - 1].X - 1 && zmija[i].Y == zmija[i + 1].Y + 1)
				std::wcout << TELOD4;
			else if (zmija[i].X == zmija[i - 1].X - 1 && zmija[i].Y == zmija[i + 1].Y - 1)
				std::wcout << TELOD1;
			else if (zmija[i].X == zmija[i - 1].X + 1 && zmija[i].Y == zmija[i + 1].Y - 1)
				std::wcout << TELOD2;
		}
		else {
			if (zmija[i].X == zmija[i + 1].X && zmija[i].X == zmija[i - 1].X)
				std::wcout << TELOY;
			else if (zmija[i].Y == zmija[i + 1].Y && zmija[i].Y == zmija[i - 1].Y)
				std::wcout << TELOX;
			else if (zmija[i].X == zmija[i + 1].X + 1 && zmija[i].Y == zmija[i - 1].Y + 1)
				std::wcout << TELO3;
			else if (zmija[i].X == zmija[i + 1].X - 1 && zmija[i].Y == zmija[i - 1].Y + 1)
				std::wcout << TELO4;
			else if (zmija[i].X == zmija[i + 1].X - 1 && zmija[i].Y == zmija[i - 1].Y - 1)
				std::wcout << TELO1;
			else if (zmija[i].X == zmija[i + 1].X + 1 && zmija[i].Y == zmija[i - 1].Y - 1)
				std::wcout << TELO2;
			else if (zmija[i].X == zmija[i - 1].X + 1 && zmija[i].Y == zmija[i + 1].Y + 1)
				std::wcout << TELO3;
			else if (zmija[i].X == zmija[i - 1].X - 1 && zmija[i].Y == zmija[i + 1].Y + 1)
				std::wcout << TELO4;
			else if (zmija[i].X == zmija[i - 1].X - 1 && zmija[i].Y == zmija[i + 1].Y - 1)
				std::wcout << TELO1;
			else if (zmija[i].X == zmija[i - 1].X + 1 && zmija[i].Y == zmija[i + 1].Y - 1)
				std::wcout << TELO2;
		}
	}
	gotoxy(zmija[0].X, zmija[0].Y);
	if (zmija[0].X == zmija[1].X)
		if (tabla[XX(zmija[0].X)][YY(zmija[0].Y)] == TELOD)
			std::wcout << TELODY;
		else
			std::wcout << TELOY;
	else
		if (tabla[XX(zmija[0].X)][YY(zmija[0].Y)] == TELOD)
			std::wcout << TELODX;
		else
			std::wcout << TELOX;

	mtx.unlock();
}

void PrintItem(int br, bool underline) {
	while (input[0])Sleep(1);
	mtx.lock();
	color(EKRANO, EKRAN);
	if (underline)
		underline();
	if (br == 1) {
		if (restart) {
			gotoxy(12, 10); std::wcout << L"Restart ";
		}
		else {
			gotoxy(12, 10); std::wcout << L"Continue";
		}
	}
	else if (br == 2) {
		gotoxy(12, 12); std::wcout << L"Speed";
	}
	else if (br == 3) {
		gotoxy(12, 14); std::wcout << L"Maze";
	}
	else if (br == 4) {
		gotoxy(12, 16); std::wcout << L"Sound";
	}
	SakriKursor();
	mtx.unlock();
}

void PrintValue(int br, wstring value) {
	mtx.lock();
	color(EKRANO, EKRAN);
	if (br == 2)
		gotoxy(21, 12);
	else if (br == 3)
		gotoxy(21, 14);
	else if (br == 4)
		gotoxy(21, 16);
	std::wcout << std::right << std::setfill(L' ') << std::setw(16) << L"◄ " + value + L" ►";
	SakriKursor();
	mtx.unlock();
}

wstring LevelName(int i) {
	if (i == 0)
		return L"None";
	else if (i == 1)
		return L"Box";
	else if (i == 2)
		return L"Tunnel";
	else if (i == 3)
		return L"Spiral";
	else if (i == 4)
		return L"Blockade";
	else if (i == 5)
		return L"Twisted";
	else if (i == 6)
		return L"Adventure";
}

void Menu() {
	bool once = true;
	bool once2 = true;
	restart = false;
	int speedtmp = speed;
	int leveltmp = level;
	bool speedautotmp = speedauto;
	bool levelstmp = levels;
	int selected = 1;
	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(8, 8);  std::wcout << L"┌───────────────────────────────┐";
	gotoxy(8, 9);  std::wcout << L"│                               │";
	gotoxy(8, 10); std::wcout << L"│                               │";
	gotoxy(8, 11); std::wcout << L"│                               │";
	gotoxy(8, 12); std::wcout << L"│                               │";
	gotoxy(8, 13); std::wcout << L"│                               │";
	gotoxy(8, 14); std::wcout << L"│                               │";
	gotoxy(8, 15); std::wcout << L"│                               │";
	gotoxy(8, 16); std::wcout << L"│                               │";
	gotoxy(8, 17); std::wcout << L"│                               │";
	gotoxy(8, 18); std::wcout << L"│                               │";
	gotoxy(8, 19); std::wcout << L"│                               │";
	gotoxy(8, 20); std::wcout << L"│                               │";
	gotoxy(8, 21); std::wcout << L"└───────────────────────────────┘";
	SakriKursor();
	mtx.unlock();
	PrintItem(1, true);
	PrintItem(2, false);
	if (speedauto)
		PrintValue(2, L"Auto");
	else
		PrintValue(2, to_wstring(speeds));

	PrintItem(3, false);
	if (levels)
		PrintValue(3, LevelName(6));
	else
		PrintValue(3, LevelName(level));

	PrintItem(4, false);
	if (mute)
		PrintValue(4, L"Off");
	else
		PrintValue(4, L"On");

	while (1) {
		if (input[0] == ESC || input[0] == LINR) {
			while (input[0] == ESC || input[0] == LINR)Sleep(1);
			break;
		}
		else if (input[0] == B8 || input[0] == B0 || input[0] == DOWN) {
			PrintItem(selected, false);
			selected++;
			if (selected > 4)
				selected = 1;
			PrintItem(selected, true);
			while (input[0] == B8 || input[0] == B0 || input[0] == DOWN)Sleep(1);
		}
		else if (input[0] == B2 || input[0] == UP) {
			PrintItem(selected, false);
			selected--;
			if (selected < 1)
				selected = 4;
			PrintItem(selected, true);
			while (input[0] == B2 || input[0] == UP)Sleep(1);
		}
		else if (input[0] == ENTER) {
			while (input[0] == ENTER)Sleep(1);
			if (selected == 1)
				break;
			else if (selected == 4)
				if (!mute) {
					PrintValue(4, L"Off");
					PlaySoundW(NULL, NULL, SND_ASYNC);
					mute = true;
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TASTT);
					std::wcout << L"♪";
					SakriKursor();
					mtx.unlock();
					while (input[0] == MUTE)Sleep(50);
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TAST);
					std::wcout << L"♪";
					SakriKursor();
					mtx.unlock();
				}
				else {
					PrintValue(4, L"On");
					mute = false;
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TASTT);
					std::wcout << L"♫";
					SakriKursor();
					mtx.unlock();
					while (input[0] == MUTE)Sleep(50);
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TAST);
					std::wcout << L"♫";
					SakriKursor();
					mtx.unlock();
				}
			else if (selected == 2) {
				if (speedauto && speeds != 10) {
					speedauto = false;
					PrintValue(2, to_wstring(speeds));
				}
				else {
					speedauto = true;
					PrintValue(2, L"Auto");
				}
			}
			else if (selected == 3) {
				if (levels && level != 6) {
					levels = false;
					PrintValue(3, LevelName(level));
				}
				else {
					levels = true;
					PrintValue(3, LevelName(6));
				}
			}
		}
		else if (input[0] == LEFT || input[0] == B4) {
			while (input[0] == LEFT || input[0] == B4)Sleep(1);
			if (selected == 4)
				if (!mute) {
					PrintValue(4, L"Off");
					PlaySoundW(NULL, NULL, SND_ASYNC);
					mute = true;
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TASTT);
					std::wcout << L"♪";
					SakriKursor();
					mtx.unlock();
					while (input[0] == MUTE)
						Sleep(50);
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TAST);
					std::wcout << L"♪";
					SakriKursor();
					mtx.unlock();
				}
				else {
					PrintValue(4, L"On");
					mute = false;
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TASTT);
					std::wcout << L"♫";
					SakriKursor();
					mtx.unlock();
					while (input[0] == MUTE)
						Sleep(50);
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TAST);
					std::wcout << L"♫";
					SakriKursor();
					mtx.unlock();
				}
			else if (selected == 2) {
				speedauto = false;
				speeds--;
				if (speeds < 1) {
					speeds = 10;
					speedauto = true;
				}
				if (speedauto)
					PrintValue(2, L"Auto");
				else
					PrintValue(2, to_wstring(speeds));
				speed = 350 - 30 * speeds;
			}
			else if (selected == 3) {
				levels = false;
				level--;
				if (level < 0) {
					level = 6;
					levels = true;
				}
				PrintValue(3, LevelName(level));
			}
		}
		else if (input[0] == RIGHT || input[0] == B6) {
			while (input[0] == RIGHT || input[0] == B6)Sleep(1);
			if (selected == 4)
				if (!mute) {
					PrintValue(4, L"Off");
					PlaySoundW(NULL, NULL, SND_ASYNC);
					mute = true;
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TASTT);
					std::wcout << L"♪";
					SakriKursor();
					mtx.unlock();
					while (input[0] == MUTE)
						Sleep(50);
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TAST);
					std::wcout << L"♪";
					SakriKursor();
					mtx.unlock();
				}
				else {
					PrintValue(4, L"On");
					mute = false;
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TASTT);
					std::wcout << L"♫";
					SakriKursor();
					mtx.unlock();
					while (input[0] == MUTE)
						Sleep(50);
					mtx.lock();
					gotoxy(36, 37);
					color(DGM, TAST);
					std::wcout << L"♫";
					SakriKursor();
					mtx.unlock();
				}
			else if (selected == 2) {
				speedauto = false;
				speeds++;
				if (speeds == 10)
					speedauto = true;
				else if (speeds > 10)
					speeds = 1;
				if (speedauto)
					PrintValue(2, L"Auto");
				else
					PrintValue(2, to_wstring(speeds));
				speed = 350 - 30 * speeds;
			}
			else if (selected == 3) {
				levels = false;
				level++;
				if (level == 6) {
					levels = true;
				}
				else if (level > 6)
					level = 0;
				PrintValue(3, LevelName(level));
			}
		}

		if (!mute && once2) {
			PrintValue(4, L"On");
			once2 = false;
		}
		else if (mute && !once2) {
			PrintValue(4, L"Off");
			once2 = true;
		}

		if (((speedtmp != speed && !speedauto) || speedauto != speedautotmp || (leveltmp != level && !levels) || levels != levelstmp) && once) {
			once = false;
			restart = true;
			if (selected == 1)
				PrintItem(1, true);
			else
				PrintItem(1, false);
		}
		else if ((((speedtmp == speed && speedauto == speedautotmp) || (speedautotmp && speedauto == speedautotmp)) && ((leveltmp == level && levels == levelstmp) || (levelstmp && levels == levelstmp))) && !once) {
			once = true;
			restart = false;
			if (selected == 1)
				PrintItem(1, true);
			else
				PrintItem(1, false);
		}

		Sleep(1);
	}

	curlevel = level;
	if (levels)
		curlevel = 0;

	speedcur = speed;
	if (speedauto)
		speedcur = 300;

	if (!restart) {
		Level(false);
		ScorePlus(0);
		if (tajmer) {
			mtx.lock();
			color(EKRANO, EKRAN);
			gotoxy(24, 8);
			underline();
			std::wcout << hranaspecial;
			color(EKRANO, EKRAN);
			gotoxy(special.X, special.Y);
			std::wcout << hranaspecial;
			mtx.unlock();
			SpecialTajmer(tajmer);
			tajmer--;
		}
		mtx.lock();
		color(EKRANO, EKRAN);
		gotoxy(hranaxy.X, hranaxy.Y);
		std::wcout << HRANAO;
		mtx.unlock();
		IspisiZmiju();
		Sleep(speed);
	}
}

void Level(bool snake) {
	if (curlevel == 1) {
		if (snake)
			Init(0);
		for (int i = 1; i < 30; i++)
			for (int j = 0; j < 11; j++)
				if (i == 1 || j == 0 || i == 29 || j == 10)
					tabla[i][j] = TELO;
		PrintConsole(false);
		mtx.lock();
		color(EKRANO, EKRAN);
		gotoxy(10, 10); std::wcout << L"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
		gotoxy(10, 11); std::wcout << L"┃                           ┃";
		gotoxy(10, 12); std::wcout << L"┃                           ┃";
		gotoxy(10, 13); std::wcout << L"┃                           ┃";
		gotoxy(10, 14); std::wcout << L"┃                           ┃";
		if (snake) {
			gotoxy(10, 15); std::wcout << L"┃   ━━━━━━━━━━■             ┃";
		}
		else {
			gotoxy(10, 15); std::wcout << L"┃                           ┃";
		}
		gotoxy(10, 16); std::wcout << L"┃                           ┃";
		gotoxy(10, 17); std::wcout << L"┃                           ┃";
		gotoxy(10, 18); std::wcout << L"┃                           ┃";
		gotoxy(10, 19); std::wcout << L"┃                           ┃";
		gotoxy(10, 20); std::wcout << L"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
		SakriKursor();
		mtx.unlock();
	}
	else if (curlevel == 2) {
		if (snake)
			Init(0);
		for (int i = 1; i < 30; i++)
			for (int j = 0; j < 11; j++)
				if ((i == 1 || i == 29) && (j < 3 || j > 7) || (j == 0 || j == 10) && (i < 7 || i > 23))
					tabla[i][j] = TELO;
		for (int i = 10; i < 21; i++) {
			tabla[i][4] = TELO;
			tabla[i][6] = TELO;
		}
		PrintConsole(false);
		mtx.lock();
		color(EKRANO, EKRAN);
		gotoxy(10, 10); std::wcout << L"┏━━━━━                 ━━━━━┓";
		gotoxy(10, 11); std::wcout << L"┃                           ┃";
		gotoxy(10, 12); std::wcout << L"┃                           ┃";
		gotoxy(10, 13); std::wcout << L"                             ";
		gotoxy(10, 14); std::wcout << L"         ━━━━━━━━━━━         ";
		if (snake) {
			gotoxy(10, 15); std::wcout << L"    ━━━━━━━━━━■              ";
		}
		else {
			gotoxy(10, 15); std::wcout << L"                             ";
		}
		gotoxy(10, 16); std::wcout << L"         ━━━━━━━━━━━         ";
		gotoxy(10, 17); std::wcout << L"                             ";
		gotoxy(10, 18); std::wcout << L"┃                           ┃";
		gotoxy(10, 19); std::wcout << L"┃                           ┃";
		gotoxy(10, 20); std::wcout << L"┗━━━━━                 ━━━━━┛";
		SakriKursor();
		mtx.unlock();
	}
	else if (curlevel == 3) {
		if (snake)
			Init(1);
		for (int i = 0; i < 6; i++) {
			tabla[11][i] = TELO;
			tabla[19][i + 5] = TELO;
			tabla[2 * i][8] = TELO;
			tabla[2 * i + 1][8] = TELO;
			tabla[2 * i + 19][2] = TELO;
			tabla[2 * i + 20][2] = TELO;
		}
		tabla[12][8] = TELO;
		tabla[13][8] = TELO;
		tabla[17][2] = TELO;
		tabla[18][2] = TELO;
		PrintConsole(false);
		mtx.lock();
		color(EKRANO, EKRAN);
		gotoxy(9, 10); std::wcout << L"           ┃                   ";
		gotoxy(9, 11); std::wcout << L"           ┃                   ";
		gotoxy(9, 12); std::wcout << L"           ┃     ━━━━━━━━━━━━━━";
		gotoxy(9, 13); std::wcout << L"           ┃                   ";
		gotoxy(9, 14); std::wcout << L"           ┃                   ";
		gotoxy(9, 15); std::wcout << L"           ┃       ┃           ";
		if (snake) {
			gotoxy(9, 16); std::wcout << L"     ━━━━━━━━━━■   ┃           ";
		}
		else {
			gotoxy(9, 16); std::wcout << L"                   ┃           ";
		}
		gotoxy(9, 17); std::wcout << L"                   ┃           ";
		gotoxy(9, 18); std::wcout << L"━━━━━━━━━━━━━━     ┃           ";
		gotoxy(9, 19); std::wcout << L"                   ┃           ";
		gotoxy(9, 20); std::wcout << L"                   ┃           ";
		SakriKursor();
		mtx.unlock();
	}
	else if (curlevel == 4) {
		if (snake)
			Init(3);
		for (int i = 1; i < 30; i++)
			for (int j = 0; j < 11; j++)
				if ((i == 1 || j == 0 || i == 29 || j == 10) && (j < 4 || j > 6))
					tabla[i][j] = TELO;
		for (int i = 3; i < 8; i++) {
			tabla[9][i] = TELO;
			tabla[21][i] = TELO;
		}
		PrintConsole(false);
		mtx.lock();
		color(EKRANO, EKRAN);
		gotoxy(10, 10); std::wcout << L"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
		gotoxy(10, 11); std::wcout << L"┃                           ┃";
		gotoxy(10, 12); std::wcout << L"┃                           ┃";
		gotoxy(10, 13); std::wcout << L"┃       ┃           ┃       ┃";
		gotoxy(10, 14); std::wcout << L"        ┃           ┃        ";
		gotoxy(10, 15); std::wcout << L"        ┃           ┃        ";
		gotoxy(10, 16); std::wcout << L"        ┃           ┃        ";
		gotoxy(10, 17); std::wcout << L"┃       ┃           ┃       ┃";
		if (snake) {
			gotoxy(10, 18); std::wcout << L"┃   ━━━━━━━━━━■             ┃";
		}
		else {
			gotoxy(10, 18); std::wcout << L"┃                           ┃";
		}
		gotoxy(10, 19); std::wcout << L"┃                           ┃";
		gotoxy(10, 20); std::wcout << L"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
		SakriKursor();
		mtx.unlock();
	}
	else if (curlevel == 5) {
		if (snake)
			Init(0);
		for (int i = 0; i < 11; i++) {
			tabla[i][3] = TELO;
			tabla[i + 20][3] = TELO;
		}
		tabla[11][3] = TELO;
		tabla[12][3] = TELO;
		tabla[19][3] = TELO;
		for (int i = 1; i < 4; i++)
			tabla[13][i] = TELO;
		for (int i = 10; i < 26; i++)
			tabla[i][0] = TELO;
		for (int i = 0; i < 31; i++)
			tabla[i][7] = TELO;
		for (int i = 8; i < 11; i++)
			tabla[19][i] = TELO;
		tabla[1][1] = TELO;
		tabla[1][0] = TELO;
		tabla[2][0] = TELO;
		tabla[3][0] = TELO;
		tabla[4][0] = TELO;
		PrintConsole(false);
		mtx.lock();
		color(EKRANO, EKRAN);
		gotoxy(9, 10); std::wcout << L" ┏━━━     ━━━┳━━━━━━━━━━━━     ";
		gotoxy(9, 11); std::wcout << L" ┃           ┃                 ";
		gotoxy(9, 12); std::wcout << L"             ┃                 ";
		gotoxy(9, 13); std::wcout << L"━━━━━━━━━━━━━┛     ━━━━━━━━━━━━";
		gotoxy(9, 14); std::wcout << L"                               ";
		if (snake) {
			gotoxy(9, 15); std::wcout << L"     ━━━━━━━━━━■               ";
		}
		else {
			gotoxy(9, 15); std::wcout << L"                               ";
		}
		gotoxy(9, 16); std::wcout << L"                               ";
		gotoxy(9, 17); std::wcout << L"━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━";
		gotoxy(9, 18); std::wcout << L"                   ┃           ";
		gotoxy(9, 19); std::wcout << L"                   ┃           ";
		gotoxy(9, 20); std::wcout << L"                   ┃           ";
		SakriKursor();
		mtx.unlock();
	}
	else {
		if (snake)
			Init(0);
		PrintConsole(snake);
	}
}


int main() {
	PodesiKonzolu();
	PrintNokia();

	Init(0);
	input.insert(input.begin(), 0);
	thread mouse(MouseInput);
	mouse.detach();
	thread keyboard(KeyboardInput);
	keyboard.detach();
	thread mut(Mute);
	mut.detach();
	thread exit([]() {
		while (1) {
			while (!input.size() || !count(input.begin(), input.end(), ESC))Sleep(10);
			clock_t begin = clock();
			clock_t current;
			bool exit = true;
			do {
				if (!input.size() || !count(input.begin(), input.end(), ESC))
					exit = false;
				Sleep(1);
				current = (clock() - begin) / (CLOCKS_PER_SEC / 1000);
			} while (current < exittime);
			if (exit) {
				mtx.lock();
				color(EKRANO, EKRAN);
				gotoxy(7, 8);  std::wcout << L"                                   ";
				gotoxy(7, 9);  std::wcout << L"                                   ";
				gotoxy(7, 10); std::wcout << L"                                   ";
				gotoxy(7, 11); std::wcout << L"                                   ";
				gotoxy(7, 12); std::wcout << L"                                   ";
				gotoxy(7, 13); std::wcout << L"     ██  █   ██   █ █  █    █      ";
				gotoxy(7, 14); std::wcout << L"     █ █ █  █  █  ██   █   █‗█     ";
				gotoxy(7, 15); std::wcout << L"     █  ██   ██   █ █  █  █   █    ";
				gotoxy(7, 16); std::wcout << L"                                   ";
				gotoxy(7, 17); std::wcout << L"                                   ";
				gotoxy(7, 18); std::wcout << L"                                   ";
				gotoxy(7, 19); std::wcout << L"                                   ";
				gotoxy(7, 20); std::wcout << L"                                   ";
				gotoxy(7, 21); std::wcout << L"                                   ";
				Sleep(30);
				Beep(1500, 110);
				Beep(800, 40);
				Sleep(800);
				Beep(2000, 80);
				_exit(0);
				mtx.unlock();
			}
		}
	});
	exit.detach();
	Animacija();

	while (true) {
		Sleep(speed);
		if (levels && nextlevel)
			curlevel++;
		else if (levels)
			curlevel = 0;
		if (!nextlevel)
			score = 0;
		else if (speedauto && !nextlevel)
			speedcur = 300;
		Level(true);
		nextlevel = false;
		restart = false;
		special = { -1, -1 };
		pojedeni = 0;
		tajmer = 0;
		smer = RIGHT;
		smerold = RIGHT;
		hranaxy = Hrana(false);
		while (true) {
			bool preskoci = false;
			clock_t begin = clock();
			clock_t current;

			if (tajmer) {
				SpecialTajmer(tajmer);
				tajmer--;
			}
			else if (special.X != -1 && special.Y != -1) {
				mtx.lock();
				color(EKRANO, EKRAN);
				gotoxy(special.X, special.Y);
				std::wcout << NIŠTA;
				underline();
				gotoxy(24, 8);  std::wcout << L"   ";
				mtx.unlock();
				tabla[XX(special.X)][YY(special.Y)] = PRAZNO;
				special.X = -1;
				special.Y = -1;
			}

			do {
				if ((input[0] == B2 || input[0] == UP) && smerold != DOWN && smerold != UP)
					smer = UP;
				else if ((input[0] == B6 || input[0] == RIGHT) && smerold != LEFT && smerold != RIGHT)
					smer = RIGHT;
				else if ((input[0] == B4 || input[0] == LEFT) && smerold != RIGHT && smerold != LEFT)
					smer = LEFT;
				else if ((input[0] == B8 || input[0] == B0 || input[0] == DOWN) && smerold != UP && smerold != DOWN)
					smer = DOWN;
				else if (input[0] == MENU || input[0] == ENTER || input[0] == LINR || input[0] == LINL)
					Menu();
				Sleep(1);
				current = (clock() - begin) / (CLOCKS_PER_SEC / 1000);
			} while (current < speedcur);

			if (restart)
				break;

			if (tabla[XX(zmija[1].X)][YY(zmija[1].Y)] == TELOD && !prosireni.empty()) {
				mtx.lock();
				color(EKRANO, EKRAN);
				gotoxy(zmija[1].X, zmija[1].Y);
				std::wcout << prosireni.front();
				prosireni.pop();
				mtx.unlock();
				tabla[XX(zmija[1].X)][YY(zmija[1].Y)] = TELO;
			}
			else if (tabla[XX(zmija[2].X)][YY(zmija[2].Y)] == TELOD && (zmija[2].Y == zmija[3].Y || zmija[2].Y == zmija[1].Y) && !prosireni.empty()) {
				mtx.lock();
				color(EKRANO, EKRAN);
				gotoxy(zmija[2].X, zmija[2].Y);
				std::wcout << prosireni.front();
				prosireni.pop();
				mtx.unlock();
				tabla[XX(zmija[2].X)][YY(zmija[2].Y)] = TELO;
			}
			else if (tabla[XX(zmija[0].X)][YY(zmija[0].Y)] == TELOD && !prosireni.empty()) {
				mtx.lock();
				color(EKRANO, EKRAN);
				gotoxy(zmija[0].X, zmija[0].Y);
				std::wcout << prosireni.front();
				prosireni.pop();
				mtx.unlock();
				tabla[XX(zmija[0].X)][YY(zmija[0].Y)] = TELO;
			}
			if (smer == UP) {
				if ((zmija.back().Y == IVICAU && (tabla[XX(zmija.back().X)][YY(IVICAD)] == TELO || tabla[XX(zmija.back().X)][YY(IVICAD)] == TELOD)) || (zmija.back().Y != IVICAU && (tabla[XX(zmija.back().X)][YY(zmija.back().Y - 1)] == TELO || tabla[XX(zmija.back().X)][YY(zmija.back().Y - 1)] == TELOD)))
					break;  //crko
				mtx.lock();
				color(EKRANO, EKRAN);
				gotoxy(zmija.back().X, zmija.back().Y);
				if (tabla[XX(zmija.back().X)][YY(zmija.back().Y)] == TELOD) {
					if (smerold == UP) {
						std::wcout << TELODY;
						prosireni.push(TELOY);
					}
					else if (smerold == LEFT) {
						std::wcout << TELOD4;
						prosireni.push(TELO4);
					}
					else if (smerold == RIGHT) {
						std::wcout << TELOD3;
						prosireni.push(TELO3);
					}
				}
				else {
					if (smerold == UP)
						std::wcout << TELOY;
					else if (smerold == LEFT)
						std::wcout << TELO4;
					else if (smerold == RIGHT)
						std::wcout << TELO3;
					tabla[XX(zmija.back().X)][YY(zmija.back().Y)] = TELO;
				}
				mtx.unlock();
				if (zmija.back().Y == IVICAU) {
					mtx.lock();
					color(EKRANO, EKRAN);
					gotoxy(zmija.back().X, IVICAD);
					std::wcout << GLAVAD;
					mtx.unlock();
					if (tabla[XX(zmija.back().X)][YY(IVICAD)] == HRANA) {
						tabla[XX(zmija.back().X)][YY(IVICAD)] = TELOD;
						if (zmija.back().X == special.X && IVICAD == special.Y) {
							ScorePlus(tajmer + 1);
							tajmer = 0;
							mtx.lock();
							color(EKRANO, EKRAN);
							underline();
							gotoxy(24, 8);  std::wcout << L"   ";
							mtx.unlock();
							special.X = -1;
							special.Y = -1;
						}
						else {
							pojedeni++;
							preskoci = true;
							if (speedauto && speedcur > 60 && pojedeni % 3 == 0)
								speedcur -= 30 - ((2 * pojedeni > 20) ? 20 : (2 * pojedeni));
							ScorePlus(9);
							if (levels && curlevel < 5 && pojedeni % 8 == 0) {
								nextlevel = true;
								restart = true;
								break;
							}
							hranaxy = Hrana(false);
							if (pojedeni % 4 == 0 && (pojedeni % 8 || !levels) && special.X == -1 && special.Y == -1) {
								special = Hrana(true);
								tajmer = 20;
							}
						}
					}
					else
						tabla[XX(zmija.back().X)][YY(IVICAD)] = TELO;
					zmija.push_back({ zmija.back().X, IVICAD });
				}
				else {
					mtx.lock();
					color(EKRANO, EKRAN);
					gotoxy(zmija.back().X, zmija.back().Y - 1);
					std::wcout << GLAVAD;
					mtx.unlock();
					if (tabla[XX(zmija.back().X)][YY(zmija.back().Y - 1)] == HRANA) {
						tabla[XX(zmija.back().X)][YY(zmija.back().Y - 1)] = TELOD;
						if (zmija.back().X == special.X && zmija.back().Y - 1 == special.Y) {
							ScorePlus(tajmer + 1);
							tajmer = 0;
							mtx.lock();
							color(EKRANO, EKRAN);
							underline();
							gotoxy(24, 8);  std::wcout << L"   ";
							mtx.unlock();
							special.X = -1;
							special.Y = -1;
						}
						else {
							pojedeni++;
							preskoci = true;
							if (speedauto && speedcur > 60 && pojedeni % 3 == 0)
								speedcur -= 30 - ((2 * pojedeni > 20) ? 20 : (2 * pojedeni));
							ScorePlus(9);
							if (levels && curlevel < 5 && pojedeni % 8 == 0) {
								nextlevel = true;
								restart = true;
								break;
							}
							hranaxy = Hrana(false);
							if (pojedeni % 4 == 0 && (pojedeni % 8 || !levels) && special.X == -1 && special.Y == -1) {
								special = Hrana(true);
								tajmer = 20;
							}
						}
					}
					else
						tabla[XX(zmija.back().X)][YY(zmija.back().Y - 1)] = TELO;
					zmija.push_back({ zmija.back().X, zmija.back().Y - 1 });
				}
			}
			else if (smer == RIGHT) {
				if ((zmija.back().X == IVICAR && (tabla[XX(IVICAL)][YY(zmija.back().Y)] == TELO || tabla[XX(IVICAL)][YY(zmija.back().Y)] == TELOD)) || (zmija.back().X != IVICAR && (tabla[XX(zmija.back().X + 2)][YY(zmija.back().Y)] == TELO || tabla[XX(zmija.back().X + 2)][YY(zmija.back().Y)] == TELOD)))
					break;  //crko
				mtx.lock();
				color(EKRANO, EKRAN);
				gotoxy(zmija.back().X, zmija.back().Y);
				if (tabla[XX(zmija.back().X)][YY(zmija.back().Y)] == TELOD) {
					if (smerold == RIGHT) {
						std::wcout << TELODX;
						prosireni.push(TELOX);
					}
					else if (smerold == UP) {
						std::wcout << TELOD1;
						prosireni.push(TELO1);
					}
					else if (smerold == DOWN) {
						std::wcout << TELOD4;
						prosireni.push(TELO4);
					}
				}
				else {
					if (smerold == RIGHT)
						std::wcout << TELOX;
					else if (smerold == UP)
						std::wcout << TELO1;
					else if (smerold == DOWN)
						std::wcout << TELO4;
					tabla[XX(zmija.back().X)][YY(zmija.back().Y)] = TELO;
				}
				mtx.unlock();
				if (zmija.back().X == IVICAR) {
					mtx.lock();
					color(EKRANO, EKRAN);
					gotoxy(IVICAL, zmija.back().Y);
					std::wcout << GLAVAS;
					mtx.unlock();
					if (tabla[XX(IVICAL)][YY(zmija.back().Y)] == HRANA) {
						tabla[XX(IVICAL)][YY(zmija.back().Y)] = TELOD;
						if (IVICAL == special.X && zmija.back().Y == special.Y) {
							ScorePlus(tajmer + 1);
							tajmer = 0;
							mtx.lock();
							color(EKRANO, EKRAN);
							underline();
							gotoxy(24, 8);  std::wcout << L"   ";
							mtx.unlock();
							special.X = -1;
							special.Y = -1;
						}
						else {
							pojedeni++;
							preskoci = true;
							if (speedauto && speedcur > 60 && pojedeni % 3 == 0)
								speedcur -= 30 - ((2 * pojedeni > 20) ? 20 : (2 * pojedeni));
							ScorePlus(9);
							if (levels && curlevel < 5 && pojedeni % 8 == 0) {
								nextlevel = true;
								restart = true;
								break;
							}
							hranaxy = Hrana(false);
							if (pojedeni % 4 == 0 && (pojedeni % 8 || !levels) && special.X == -1 && special.Y == -1) {
								special = Hrana(true);
								tajmer = 20;
							}
						}
					}
					else
						tabla[XX(IVICAL)][YY(zmija.back().Y)] = TELO;
					zmija.push_back({ IVICAL, zmija.back().Y });
				}
				else {
					mtx.lock();
					color(EKRANO, EKRAN);
					gotoxy(zmija.back().X + 1, zmija.back().Y);
					std::wcout << TELOX;
					gotoxy(zmija.back().X + 2, zmija.back().Y);
					std::wcout << GLAVAS;
					mtx.unlock();
					if (tabla[XX(zmija.back().X + 2)][YY(zmija.back().Y)] == HRANA) {
						tabla[XX(zmija.back().X + 2)][YY(zmija.back().Y)] = TELOD;
						if (zmija.back().X + 2 == special.X && zmija.back().Y == special.Y) {
							ScorePlus(tajmer + 1);
							tajmer = 0;
							mtx.lock();
							color(EKRANO, EKRAN);
							underline();
							gotoxy(24, 8);  std::wcout << L"   ";
							mtx.unlock();
							special.X = -1;
							special.Y = -1;
						}
						else {
							pojedeni++;
							preskoci = true;
							if (speedauto && speedcur > 60 && pojedeni % 3 == 0)
								speedcur -= 30 - ((2 * pojedeni > 20) ? 20 : (2 * pojedeni));
							ScorePlus(9);
							if (levels && curlevel < 5 && pojedeni % 8 == 0) {
								nextlevel = true;
								restart = true;
								break;
							}
							hranaxy = Hrana(false);
							if (pojedeni % 4 == 0 && (pojedeni % 8 || !levels) && special.X == -1 && special.Y == -1) {
								special = Hrana(true);
								tajmer = 20;
							}
						}
					}
					else
						tabla[XX(zmija.back().X + 2)][YY(zmija.back().Y)] = TELO;
					tabla[XX(zmija.back().X + 1)][YY(zmija.back().Y)] = TELO;
					zmija.push_back({ zmija.back().X + 1, zmija.back().Y });
					zmija.push_back({ zmija.back().X + 1, zmija.back().Y });
				}
			}
			else if (smer == LEFT) {
				if ((zmija.back().X == IVICAL && (tabla[XX(IVICAR)][YY(zmija.back().Y)] == TELO || tabla[XX(IVICAR)][YY(zmija.back().Y)] == TELOD)) || (zmija.back().X != IVICAL && (tabla[XX(zmija.back().X - 2)][YY(zmija.back().Y)] == TELO || tabla[XX(zmija.back().X - 2)][YY(zmija.back().Y)] == TELOD)))
					break;  //crko
				mtx.lock();
				color(EKRANO, EKRAN);
				gotoxy(zmija.back().X, zmija.back().Y);
				if (tabla[XX(zmija.back().X)][YY(zmija.back().Y)] == TELOD) {
					if (smerold == LEFT) {
						std::wcout << TELODX;
						prosireni.push(TELOX);
					}
					else if (smerold == UP) {
						std::wcout << TELOD2;
						prosireni.push(TELO2);
					}
					else if (smerold == DOWN) {
						std::wcout << TELOD3;
						prosireni.push(TELO3);
					}
				}
				else {
					if (smerold == LEFT)
						std::wcout << TELOX;
					else if (smerold == UP)
						std::wcout << TELO2;
					else if (smerold == DOWN)
						std::wcout << TELO3;
					tabla[XX(zmija.back().X)][YY(zmija.back().Y)] = TELO;
				}
				mtx.unlock();
				if (zmija.back().X == IVICAL) {
					mtx.lock();
					color(EKRANO, EKRAN);
					gotoxy(IVICAR, zmija.back().Y);
					std::wcout << GLAVAS;
					mtx.unlock();
					if (tabla[XX(IVICAR)][YY(zmija.back().Y)] == HRANA) {
						tabla[XX(IVICAR)][YY(zmija.back().Y)] = TELOD;
						if (IVICAR == special.X && zmija.back().Y == special.Y) {
							ScorePlus(tajmer + 1);
							tajmer = 0;
							mtx.lock();
							color(EKRANO, EKRAN);
							underline();
							gotoxy(24, 8);  std::wcout << L"   ";
							mtx.unlock();
							special.X = -1;
							special.Y = -1;
						}
						else {
							pojedeni++;
							preskoci = true;
							if (speedauto && speedcur > 60 && pojedeni % 3 == 0)
								speedcur -= 30 - ((2 * pojedeni > 20) ? 20 : (2 * pojedeni));
							ScorePlus(9);
							if (levels && curlevel < 5 && pojedeni % 8 == 0) {
								nextlevel = true;
								restart = true;
								break;
							}
							hranaxy = Hrana(false);
							if (pojedeni % 4 == 0 && (pojedeni % 8 || !levels) && special.X == -1 && special.Y == -1) {
								special = Hrana(true);
								tajmer = 20;
							}
						}
					}
					else
						tabla[XX(IVICAR)][YY(zmija.back().Y)] = TELO;
					zmija.push_back({ IVICAR, zmija.back().Y });
				}
				else {
					mtx.lock();
					color(EKRANO, EKRAN);
					gotoxy(zmija.back().X - 1, zmija.back().Y);
					std::wcout << TELOX;
					gotoxy(zmija.back().X - 2, zmija.back().Y);
					std::wcout << GLAVAS;
					mtx.unlock();
					if (tabla[XX(zmija.back().X - 2)][YY(zmija.back().Y)] == HRANA) {
						tabla[XX(zmija.back().X - 2)][YY(zmija.back().Y)] = TELOD;
						if (zmija.back().X - 2 == special.X && zmija.back().Y == special.Y) {
							ScorePlus(tajmer + 1);
							tajmer = 0;
							mtx.lock();
							color(EKRANO, EKRAN);
							underline();
							gotoxy(24, 8);  std::wcout << L"   ";
							mtx.unlock();
							special.X = -1;
							special.Y = -1;
						}
						else {
							pojedeni++;
							preskoci = true;
							if (speedauto && speedcur > 60 && pojedeni % 3 == 0)
								speedcur -= 30 - ((2 * pojedeni > 20) ? 20 : (2 * pojedeni));
							ScorePlus(9);
							if (levels && curlevel < 5 && pojedeni % 8 == 0) {
								nextlevel = true;
								restart = true;
								break;
							}
							hranaxy = Hrana(false);
							if (pojedeni % 4 == 0 && (pojedeni % 8 || !levels) && special.X == -1 && special.Y == -1) {
								special = Hrana(true);
								tajmer = 20;
							}
						}
					}
					else
						tabla[XX(zmija.back().X - 2)][YY(zmija.back().Y)] = TELO;
					tabla[XX(zmija.back().X - 1)][YY(zmija.back().Y)] = TELO;
					zmija.push_back({ zmija.back().X - 1, zmija.back().Y });
					zmija.push_back({ zmija.back().X - 1, zmija.back().Y });
				}
			}
			else if (smer == DOWN) {
				if ((zmija.back().Y == IVICAD && (tabla[XX(zmija.back().X)][YY(IVICAU)] == TELO || tabla[XX(zmija.back().X)][YY(IVICAU)] == TELOD)) || (zmija.back().Y != IVICAD && (tabla[XX(zmija.back().X)][YY(zmija.back().Y + 1)] == TELO || tabla[XX(zmija.back().X)][YY(zmija.back().Y + 1)] == TELOD)))
					break;  //crko
				mtx.lock();
				color(EKRANO, EKRAN);
				gotoxy(zmija.back().X, zmija.back().Y);
				if (tabla[XX(zmija.back().X)][YY(zmija.back().Y)] == TELOD) {
					if (smerold == DOWN) {
						std::wcout << TELODY;
						prosireni.push(TELOY);
					}
					else if (smerold == LEFT) {
						std::wcout << TELOD1;
						prosireni.push(TELO1);
					}
					else if (smerold == RIGHT) {
						std::wcout << TELOD2;
						prosireni.push(TELO2);
					}
				}
				else {
					if (smerold == DOWN)
						std::wcout << TELOY;
					else if (smerold == LEFT)
						std::wcout << TELO1;
					else if (smerold == RIGHT)
						std::wcout << TELO2;
					tabla[XX(zmija.back().X)][YY(zmija.back().Y)] = TELO;
				}
				mtx.unlock();
				if (zmija.back().Y == IVICAD) {
					mtx.lock();
					color(EKRANO, EKRAN);
					gotoxy(zmija.back().X, IVICAU);
					std::wcout << GLAVAG;
					mtx.unlock();
					if (tabla[XX(zmija.back().X)][YY(IVICAU)] == HRANA) {
						tabla[XX(zmija.back().X)][YY(IVICAU)] = TELOD;
						if (zmija.back().X == special.X && IVICAU == special.Y) {
							ScorePlus(tajmer + 1);
							tajmer = 0;
							mtx.lock();
							color(EKRANO, EKRAN);
							underline();
							gotoxy(24, 8);  std::wcout << L"   ";
							mtx.unlock();
							special.X = -1;
							special.Y = -1;
						}
						else {
							pojedeni++;
							preskoci = true;
							if (speedauto && speedcur > 60 && pojedeni % 3 == 0)
								speedcur -= 30 - ((2 * pojedeni > 20) ? 20 : (2 * pojedeni));
							ScorePlus(9);
							if (levels && curlevel < 5 && pojedeni % 8 == 0) {
								nextlevel = true;
								restart = true;
								break;
							}
							hranaxy = Hrana(false);
							if (pojedeni % 4 == 0 && (pojedeni % 8 || !levels) && special.X == -1 && special.Y == -1) {
								special = Hrana(true);
								tajmer = 20;
							}
						}
					}
					else
						tabla[XX(zmija.back().X)][YY(IVICAU)] = TELO;
					zmija.push_back({ zmija.back().X, IVICAU });
				}
				else {
					mtx.lock();
					color(EKRANO, EKRAN);
					gotoxy(zmija.back().X, zmija.back().Y + 1);
					std::wcout << GLAVAG;
					mtx.unlock();
					if (tabla[XX(zmija.back().X)][YY(zmija.back().Y + 1)] == HRANA) {
						tabla[XX(zmija.back().X)][YY(zmija.back().Y + 1)] = TELOD;
						if (zmija.back().X == special.X && zmija.back().Y + 1 == special.Y) {
							ScorePlus(tajmer + 1);
							tajmer = 0;
							mtx.lock();
							color(EKRANO, EKRAN);
							underline();
							gotoxy(24, 8);  std::wcout << L"   ";
							mtx.unlock();
							special.X = -1;
							special.Y = -1;
						}
						else {
							pojedeni++;
							preskoci = true;
							if (speedauto && speedcur > 60 && pojedeni % 3 == 0)
								speedcur -= 30 - ((2 * pojedeni > 20) ? 20 : (2 * pojedeni));
							ScorePlus(9);
							if (levels && curlevel < 5 && pojedeni % 8 == 0) {
								nextlevel = true;
								restart = true;
								break;
							}
							hranaxy = Hrana(false);
							if (pojedeni % 4 == 0 && (pojedeni % 8 || !levels) && special.X == -1 && special.Y == -1) {
								special = Hrana(true);
								tajmer = 20;
							}
						}
					}
					else
						tabla[XX(zmija.back().X)][YY(zmija.back().Y + 1)] = TELO;
					zmija.push_back({ zmija.back().X, zmija.back().Y + 1 });
				}
			}
			if (!preskoci) {
				mtx.lock();
				color(EKRANO, EKRAN);
				gotoxy(zmija[0].X, zmija[0].Y);
				std::wcout << NIŠTA;
				mtx.unlock();
				tabla[XX(zmija[0].X)][YY(zmija[0].Y)] = PRAZNO;
				if (zmija[0].Y == zmija[1].Y && zmija[1].X != IVICAL && zmija[1].X != IVICAR) {
					mtx.lock();
					color(EKRANO, EKRAN);
					gotoxy(zmija[1].X, zmija[1].Y);
					std::wcout << NIŠTA;
					mtx.unlock();
					tabla[XX(zmija[1].X)][YY(zmija[1].Y)] = PRAZNO;
					zmija.erase(zmija.begin());
				}
				zmija.erase(zmija.begin());
			}
			smerold = smer;
		}
		if (!restart) {
			mtx.lock();
			color(EKRANO, EKRAN);
			underline();
			gotoxy(29, 8);
			std::wcout << L"GAME OVER";
			mtx.unlock();
			Sleep(500);
			while (!input[0])Sleep(1);
		}
	}

	return 0;
}

void ScorePlus(int value) {
	score += value;
	mtx.lock();
	color(EKRANO, EKRAN);
	underline();
	gotoxy(11, 8);  std::wcout << score / 1000 << L" " << (score % 1000) / 100 << L" " << (score % 100) / 10 << L" " << score % 10;
	mtx.unlock();
}

int random(int start, int end) {
	random_device rnd;
	mt19937 mt(rnd());
	uniform_int_distribution<mt19937::result_type> rang(start, end);
	return rang(mt);
}

COORD Hrana(bool special) {
	while (true) {
		int x = random(0, 29);
		int y = random(0, 10);
		if (!tabla[x][y] && x % 2) {
			tabla[x][y] = HRANA;
			mtx.lock();
			color(EKRANO, EKRAN);
			gotoxy(XXX(x), YYY(y));
			if (special) {
				hranaspecial = hrana[random(0, hrana.size() - 1)];
				std::wcout << hranaspecial;
				gotoxy(24, 8);
				underline();
				std::wcout << hranaspecial;
			}
			else
				std::wcout << HRANAO;
			mtx.unlock();
			return { (short)XXX(x), (short)YYY(y) };
		}
	}
}

void Init(int p) {
	for (int i = 0; i < 31; i++)
		for (int j = 0; j < 11; j++)
			tabla[i][j] = PRAZNO;

	tabla[5][5 + p] = TELO;
	tabla[6][5 + p] = TELO;
	tabla[7][5 + p] = TELO;
	tabla[8][5 + p] = TELO;
	tabla[9][5 + p] = TELO;
	tabla[10][5 + p] = TELO;
	tabla[11][5 + p] = TELO;
	tabla[12][5 + p] = TELO;
	tabla[13][5 + p] = TELO;
	tabla[14][5 + p] = TELO;
	tabla[15][5 + p] = TELO;
	zmija.clear();
	zmija.push_back({ 14, 15 + (short)p });
	zmija.push_back({ 15, 15 + (short)p });
	zmija.push_back({ 16, 15 + (short)p });
	zmija.push_back({ 17, 15 + (short)p });
	zmija.push_back({ 18, 15 + (short)p });
	zmija.push_back({ 19, 15 + (short)p });
	zmija.push_back({ 20, 15 + (short)p });
	zmija.push_back({ 21, 15 + (short)p });
	zmija.push_back({ 22, 15 + (short)p });
	zmija.push_back({ 23, 15 + (short)p });
	zmija.push_back({ 24, 15 + (short)p });
	prosireni = queue<wchar_t>();
}

void PrintConsole(bool snake) {
	mtx.lock();
	color(EKRANO, EKRAN);
	underline();
	gotoxy(8, 8);  std::wcout << L"   0 0 0 0                       ";
	color(EKRANO, EKRAN);
	gotoxy(8, 9);  std::wcout << L"┌───────────────────────────────┐";
	gotoxy(8, 10); std::wcout << L"│                               │";
	gotoxy(8, 11); std::wcout << L"│                               │";
	gotoxy(8, 12); std::wcout << L"│                               │";
	gotoxy(8, 13); std::wcout << L"│                               │";
	gotoxy(8, 14); std::wcout << L"│                               │";
	if (snake) {
		gotoxy(8, 15); std::wcout << L"│     ━━━━━━━━━━■               │";
	}
	else {
		gotoxy(8, 15); std::wcout << L"│                               │";
	}
	gotoxy(8, 16); std::wcout << L"│                               │";
	gotoxy(8, 17); std::wcout << L"│                               │";
	gotoxy(8, 18); std::wcout << L"│                               │";
	gotoxy(8, 19); std::wcout << L"│                               │";
	gotoxy(8, 20); std::wcout << L"│                               │";
	gotoxy(8, 21); std::wcout << L"└───────────────────────────────┘";

	SakriKursor();
	mtx.unlock();
	ScorePlus(0);
}

void PrintNokia() {
	mtx.lock();
	color(RAMO, RAM);
	gotoxy(2, 1);  std::wcout << L"╭──┬─────────────────────────────────────┬──╮";
	gotoxy(2, 1);  std::wcout << L"│  │                                     │  │";
	gotoxy(2, 2);  std::wcout << L"│  │                                     │  │";
	gotoxy(2, 3);  std::wcout << L"│  │                ▀▀▀▀▀                │  │";
	gotoxy(2, 4);  std::wcout << L"│  │                                     │  │";
	gotoxy(2, 5);  std::wcout << L"│  │                                     │  │";
	gotoxy(2, 6);  std::wcout << L"│  │              N O K I A              │  │";
	gotoxy(2, 7);  std::wcout << L"│  │                                     │  │";
	gotoxy(2, 8);  std::wcout << L"│  │                                     │  │";
	gotoxy(2, 9);  std::wcout << L"│  │                                     │  │";
	gotoxy(2, 10); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 11); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 12); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 13); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 14); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 15); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 16); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 17); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 18); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 19); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 20); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 21); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 22); std::wcout << L"│  │                                     │  │";
	gotoxy(2, 23); std::wcout << L"│  ├─────────────────────────────────────┤  │";
	gotoxy(2, 24); std::wcout << L"│  │           ╭─────────────╮           │  │";
	gotoxy(2, 25); std::wcout << L"│  │    ━━━    │      ▲      │    ━━━    │  │";
	gotoxy(2, 26); std::wcout << L"│  ├───────────┤ ◄         ► ├───────────┤  │";
	gotoxy(2, 27); std::wcout << L"│  │     C     │      ▼      │     ₵     │  │";
	gotoxy(2, 28); std::wcout << L"│  │           ╰─────────────╯           │  │";
	gotoxy(2, 29); std::wcout << L"│  ├────────────┬───────────┬────────────┤  │";
	gotoxy(2, 30); std::wcout << L"│  │      1     │     2     │     3      │  │";
	gotoxy(2, 31); std::wcout << L"│  ├────────────┼───────────┼────────────┤  │";
	gotoxy(2, 32); std::wcout << L"│  │      4     │     5     │     6      │  │";
	gotoxy(2, 33); std::wcout << L"│  ├────────────┼───────────┼────────────┤  │";
	gotoxy(2, 34); std::wcout << L"│  │      7     │     8     │     9      │  │";
	gotoxy(2, 35); std::wcout << L"│  ├────────────┼───────────┼────────────┤  │";
	gotoxy(2, 36); std::wcout << L"│  │      *     │     0     │     #      │  │";
	gotoxy(2, 37); std::wcout << L"│  │      +     │     ˽     │     ♫      │  │";
	gotoxy(2, 38); std::wcout << L"│  ╰────────────┴───────────┴────────────╯  │";
	gotoxy(2, 39); std::wcout << L"│                                           │";
	gotoxy(2, 40); std::wcout << L"│                                           │";
	gotoxy(2, 41); std::wcout << L"╰───────────────────────────────────────────╯";

	color(SRO, EKRAN2);
	gotoxy(5, 1);  std::wcout << L"┬─────────────────────────────────────┬";
	gotoxy(5, 1);  std::wcout << L"│                                     │";
	gotoxy(5, 2);  std::wcout << L"│                                     │";
	gotoxy(5, 3);  std::wcout << L"│                ▀▀▀▀▀                │";
	gotoxy(5, 4);  std::wcout << L"│                                     │";
	gotoxy(5, 5);  std::wcout << L"│                                     │";
	gotoxy(5, 6);  std::wcout << L"│              N O K I A              │";
	gotoxy(5, 7);  std::wcout << L"│                                     │";
	gotoxy(5, 8);  std::wcout << L"│                                     │";
	gotoxy(5, 9);  std::wcout << L"│                                     │";
	gotoxy(5, 10); std::wcout << L"│                                     │";
	gotoxy(5, 11); std::wcout << L"│                                     │";
	gotoxy(5, 12); std::wcout << L"│                                     │";
	gotoxy(5, 13); std::wcout << L"│                                     │";
	gotoxy(5, 14); std::wcout << L"│                                     │";
	gotoxy(5, 15); std::wcout << L"│                                     │";
	gotoxy(5, 16); std::wcout << L"│                                     │";
	gotoxy(5, 17); std::wcout << L"│                                     │";
	gotoxy(5, 18); std::wcout << L"│                                     │";
	gotoxy(5, 19); std::wcout << L"│                                     │";
	gotoxy(5, 20); std::wcout << L"│                                     │";
	gotoxy(5, 21); std::wcout << L"│                                     │";
	gotoxy(5, 22); std::wcout << L"│                                     │";
	gotoxy(5, 23); std::wcout << L"├─────────────────────────────────────┤";

	color(SRO, TAST);
	gotoxy(5, 24); std::wcout << L"│           ╭─────────────╮           │";
	gotoxy(5, 25); std::wcout << L"│    ━━━    │      ▲      │    ━━━    │";
	gotoxy(5, 26); std::wcout << L"├───────────┤ ◄         ► ├───────────┤";
	gotoxy(5, 27); std::wcout << L"│     C     │      ▼      │     ₵     │";
	gotoxy(5, 28); std::wcout << L"│           ╰─────────────╯           │";
	gotoxy(5, 29); std::wcout << L"├────────────┬───────────┬────────────┤";
	gotoxy(5, 30); std::wcout << L"│      1     │     2     │     3      │";
	gotoxy(5, 31); std::wcout << L"├────────────┼───────────┼────────────┤";
	gotoxy(5, 32); std::wcout << L"│      4     │     5     │     6      │";
	gotoxy(5, 33); std::wcout << L"├────────────┼───────────┼────────────┤";
	gotoxy(5, 34); std::wcout << L"│      7     │     8     │     9      │";
	gotoxy(5, 35); std::wcout << L"├────────────┼───────────┼────────────┤";
	gotoxy(5, 36); std::wcout << L"│      *     │     0     │     #      │";
	gotoxy(5, 37); std::wcout << L"│      +     │     ˽     │     ♫      │";
	gotoxy(5, 38); std::wcout << L"╰────────────┴───────────┴────────────╯";

	color(NOKIA, EKRAN1);
	gotoxy(6, 5);  std::wcout << L"                                     ";
	gotoxy(6, 6);  std::wcout << L"              N O K I A              ";
	gotoxy(6, 7);  std::wcout << L"                                     ";
	gotoxy(6, 8);  std::wcout << L"                                     ";
	gotoxy(6, 9);  std::wcout << L"                                     ";
	gotoxy(6, 10); std::wcout << L"                                     ";
	gotoxy(6, 11); std::wcout << L"                                     ";
	gotoxy(6, 12); std::wcout << L"                                     ";
	gotoxy(6, 13); std::wcout << L"                                     ";
	gotoxy(6, 14); std::wcout << L"                                     ";
	gotoxy(6, 15); std::wcout << L"                                     ";
	gotoxy(6, 16); std::wcout << L"                                     ";
	gotoxy(6, 17); std::wcout << L"                                     ";
	gotoxy(6, 18); std::wcout << L"                                     ";
	gotoxy(6, 19); std::wcout << L"                                     ";
	gotoxy(6, 20); std::wcout << L"                                     ";
	gotoxy(6, 21); std::wcout << L"                                     ";
	gotoxy(6, 22); std::wcout << L"                                     ";

	color(TASTO, TAST);
	gotoxy(6, 24); std::wcout << L"           ╭─────────────╮           ";
	gotoxy(6, 25); std::wcout << L"    ━━━    │      ▲      │    ━━━    ";
	gotoxy(6, 26); std::wcout << L"───────────┤ ◄         ► ├───────────";
	gotoxy(6, 27); std::wcout << L"     C     │      ▼      │     ₵     ";
	gotoxy(6, 28); std::wcout << L"           ╰─────────────╯           ";
	gotoxy(6, 29); std::wcout << L"────────────┬───────────┬────────────";
	gotoxy(6, 30); std::wcout << L"      1     │     2     │     3      ";
	gotoxy(6, 31); std::wcout << L"────────────┼───────────┼────────────";
	gotoxy(6, 32); std::wcout << L"      4     │     5     │     6      ";
	gotoxy(6, 33); std::wcout << L"────────────┼───────────┼────────────";
	gotoxy(6, 34); std::wcout << L"      7     │     8     │     9      ";
	gotoxy(6, 35); std::wcout << L"────────────┼───────────┼────────────";
	gotoxy(6, 36); std::wcout << L"      *     │     0     │     #      ";
	gotoxy(6, 37); std::wcout << L"      +     │     ˽     │     ♫      ";


	color(LINIJAC, EKRAN2);
	gotoxy(22, 3);  std::wcout << L"▀▀▀▀▀";

	color(DGM, TAST);
	gotoxy(12, 30); std::wcout << L"1";
	gotoxy(12, 32); std::wcout << L"4";
	gotoxy(12, 34); std::wcout << L"7";
	gotoxy(12, 36); std::wcout << L"*";
	gotoxy(12, 37); std::wcout << L"+";

	gotoxy(24, 30); std::wcout << L"2";
	gotoxy(24, 32); std::wcout << L"5";
	gotoxy(24, 34); std::wcout << L"8";
	gotoxy(24, 36); std::wcout << L"0";
	gotoxy(24, 37); std::wcout << L"˽";

	gotoxy(36, 30); std::wcout << L"3";
	gotoxy(36, 32); std::wcout << L"6";
	gotoxy(36, 34); std::wcout << L"9";
	gotoxy(36, 36); std::wcout << L"#";
	gotoxy(36, 37); std::wcout << L"♫";

	color(LINIJAP, TAST);
	gotoxy(10, 25); std::wcout << L"━━━";
	gotoxy(36, 25); std::wcout << L"━━━";

	color(SLUSALICAZ, TAST);
	gotoxy(11, 27); std::wcout << L"C";

	color(SLUSALICAC, TAST);
	gotoxy(37, 27); std::wcout << L"₵";

	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L"                                   ";
	gotoxy(7, 10); std::wcout << L"                                   ";
	gotoxy(7, 11); std::wcout << L"                                   ";
	gotoxy(7, 12); std::wcout << L"                                   ";
	gotoxy(7, 13); std::wcout << L"                                   ";
	gotoxy(7, 14); std::wcout << L"                                   ";
	gotoxy(7, 15); std::wcout << L"                                   ";
	gotoxy(7, 16); std::wcout << L"                                   ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"                                   ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";

	SakriKursor();
	mtx.unlock();
}

void CentrirajKonzolu() {
	RECT rectClient, rectWindow;
	HWND hWnd = GetConsoleWindow();
	GetClientRect(hWnd, &rectClient);
	GetWindowRect(hWnd, &rectWindow);
	int posx, posy;
	posx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2;
	posy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2;

	SetWindowPos(GetConsoleWindow(), HWND_TOP, posx, posy, 0, 0, SWP_NOSIZE);
}

void PodesiKonzolu() {
	ios_base::sync_with_stdio(false);
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	SetConsoleTitleW(L"Snake");
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLongW(consoleWindow, GWL_STYLE, GetWindowLongW(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_VSCROLL);
	SetConsoleMode(ConsoleInputHandle, ENABLE_EXTENDED_FLAGS);

	_CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(info);
	GetConsoleScreenBufferInfoEx(ConsoleOutputHandle, &info);
	info.dwSize = { 49, 43 };
	info.ColorTable[0] = RGB(0, 0, 0);
	info.ColorTable[1] = RGB(32, 32, 32);
	info.ColorTable[2] = RGB(64, 64, 64);
	info.ColorTable[3] = RGB(96, 96, 96);
	info.ColorTable[4] = RGB(128, 128, 128);
	info.ColorTable[5] = RGB(160, 160, 160);
	info.ColorTable[6] = RGB(192, 192, 192);
	info.ColorTable[7] = RGB(224, 224, 224);
	info.ColorTable[8] = RGB(255, 255, 255);

	info.ColorTable[12] = RGB(210, 210, 210);

	info.ColorTable[9] = RGB(135, 135, 135);

	info.ColorTable[10] = RGB(40, 75, 123);
	info.ColorTable[11] = RGB(163, 165, 35);

	info.ColorTable[13] = RGB(5, 136, 65);
	info.ColorTable[14] = RGB(230, 9, 22);
	info.ColorTable[15] = RGB(36, 127, 183);
	SetConsoleScreenBufferInfoEx(ConsoleOutputHandle, &info);
	system("MODE 49, 43");

	CONSOLE_FONT_INFOEX infof = { 0 };
	infof.cbSize = sizeof(infof);
	infof.dwFontSize.X = 6;
	infof.dwFontSize.Y = 16;
	infof.FontWeight = FW_NORMAL;
	wcscpy_s(infof.FaceName, 32, L"Consolas");
	SetCurrentConsoleFontEx(ConsoleOutputHandle, NULL, &infof);

	DWORD tmp;
	FillConsoleOutputAttribute(ConsoleOutputHandle, 136, 49 * 43, { 0, 0 }, &tmp);

	CentrirajKonzolu();
	CentrirajKonzolu();
}

void SakriKursor() {
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(ConsoleOutputHandle, &info);
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(ConsoleOutputHandle, coord);
}

bool Foreground() {
	HWND hwnd = GetForegroundWindow();
	if (hwnd == NULL)
		return false;
	HWND curhwnd = GetConsoleWindow();
	if (curhwnd == NULL)
		return false;

	DWORD pid;
	if (GetWindowThreadProcessId(hwnd, &pid) == 0)
		return false;
	DWORD curpid;
	if (GetWindowThreadProcessId(curhwnd, &curpid) == 0)
		return false;

	return (pid == curpid);
}

void KeyboardInput() {
	while (true) {
		while (!Foreground())Sleep(1);
		if (GetAsyncKeyState(VK_UP) && (!input.size() || !count(input.begin(), input.end(), UP))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB2), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), UP);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(TASTO, TASTT);
			gotoxy(21, 25); std::wcout << L"   ▲   ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_UP))Sleep(1);

			mtx.lock();
			color(TASTO, TAST);
			gotoxy(21, 25); std::wcout << L"   ▲   ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), UP), input.end());
			break;
		}
		else if ((GetAsyncKeyState('W') || GetAsyncKeyState('2') || GetAsyncKeyState(VK_NUMPAD8)) && (!input.size() || !count(input.begin(), input.end(), B2))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB2), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), B2);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(20, 30); std::wcout << L"    2    ";
			mtx.unlock();

			while (GetAsyncKeyState('W') || GetAsyncKeyState('2') || GetAsyncKeyState(VK_NUMPAD8))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(20, 30); std::wcout << L"    2    ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), B2), input.end());
			break;
		}
		else if (GetAsyncKeyState(VK_LEFT) && (!input.size() || !count(input.begin(), input.end(), LEFT))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB4), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), LEFT);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(TASTO, TASTT);
			gotoxy(18, 25); std::wcout << L"   ";
			gotoxy(18, 26); std::wcout << L" ◄ ";
			gotoxy(18, 27); std::wcout << L"   ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_LEFT))Sleep(1);

			mtx.lock();
			color(TASTO, TAST);
			gotoxy(18, 25); std::wcout << L"   ";
			gotoxy(18, 26); std::wcout << L" ◄ ";
			gotoxy(18, 27); std::wcout << L"   ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), LEFT), input.end());
			break;
		}
		else if ((GetAsyncKeyState('A') || GetAsyncKeyState('4') || GetAsyncKeyState(VK_NUMPAD4)) && (!input.size() || !count(input.begin(), input.end(), B4))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB4), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), B4);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(7, 32); std::wcout << L"     4    ";
			mtx.unlock();

			while (GetAsyncKeyState('A') || GetAsyncKeyState('4') || GetAsyncKeyState(VK_NUMPAD4))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(7, 32); std::wcout << L"     4    ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), B4), input.end());
			break;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && (!input.size() || !count(input.begin(), input.end(), RIGHT))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB6), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), RIGHT);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(TASTO, TASTT);
			gotoxy(28, 25); std::wcout << L"   ";
			gotoxy(28, 26); std::wcout << L" ► ";
			gotoxy(28, 27); std::wcout << L"   ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_RIGHT))Sleep(1);

			mtx.lock();
			color(TASTO, TAST);
			gotoxy(28, 25); std::wcout << L"   ";
			gotoxy(28, 26); std::wcout << L" ► ";
			gotoxy(28, 27); std::wcout << L"   ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), RIGHT), input.end());
			break;
		}
		else if ((GetAsyncKeyState('D') || GetAsyncKeyState('6') || GetAsyncKeyState(VK_NUMPAD6)) && (!input.size() || !count(input.begin(), input.end(), B6))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB6), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), B6);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(32, 32); std::wcout << L"    6     ";
			mtx.unlock();

			while (GetAsyncKeyState('D') || GetAsyncKeyState('6') || GetAsyncKeyState(VK_NUMPAD6))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(32, 32); std::wcout << L"    6     ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), B6), input.end());
			break;
		}
		else if (GetAsyncKeyState(VK_DOWN) && (!input.size() || !count(input.begin(), input.end(), DOWN))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB8), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), DOWN);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(TASTO, TASTT);
			gotoxy(21, 27); std::wcout << L"   ▼   ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_DOWN))Sleep(1);

			mtx.lock();
			color(TASTO, TAST);
			gotoxy(21, 27); std::wcout << L"   ▼   ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), DOWN), input.end());
			break;
		}
		else if ((GetAsyncKeyState(VK_NUMPAD0) || GetAsyncKeyState('0')) && (!input.size() || !count(input.begin(), input.end(), B0))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB0), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), B0);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(20, 36); std::wcout << L"    0    ";
			gotoxy(20, 37); std::wcout << L"    ˽    ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_NUMPAD0) || GetAsyncKeyState('0'))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(20, 36); std::wcout << L"    0    ";
			gotoxy(20, 37); std::wcout << L"    ˽    ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), B0), input.end());
			break;
		}
		else if ((GetAsyncKeyState('S') || GetAsyncKeyState(VK_NUMPAD2) || GetAsyncKeyState('8')) && (!input.size() || !count(input.begin(), input.end(), B8))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB8), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), B8);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(20, 34); std::wcout << L"    8    ";
			mtx.unlock();

			while (GetAsyncKeyState('S') || GetAsyncKeyState(VK_NUMPAD2) || GetAsyncKeyState('8'))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(20, 34); std::wcout << L"    8    ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), B8), input.end());
			break;
		}
		else if ((GetAsyncKeyState(VK_RETURN) || GetAsyncKeyState(VK_SPACE)) && (!input.size() || !count(input.begin(), input.end(), ENTER))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB5), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), ENTER);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(TASTO, TASTT);
			gotoxy(21, 26); std::wcout << L"       ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_RETURN) || GetAsyncKeyState(VK_SPACE))Sleep(1);

			mtx.lock();
			color(TASTO, TAST);
			gotoxy(21, 26); std::wcout << L"       ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), ENTER), input.end());
			break;
		}
		else if (GetAsyncKeyState(VK_TAB) && (!input.size() || !count(input.begin(), input.end(), LINL))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEL), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), LINL);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(LINIJAP, TASTT);
			gotoxy(7, 25); std::wcout << L"   ━━━   ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_TAB))Sleep(1);

			mtx.lock();
			color(LINIJAP, TAST);
			gotoxy(7, 25); std::wcout << L"   ━━━   ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), LINL), input.end());
			break;
		}
		else if (GetAsyncKeyState(VK_BACK) && (!input.size() || !count(input.begin(), input.end(), LINR))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVER), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), LINR);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(LINIJAP, TASTT);
			gotoxy(33, 25); std::wcout << L"   ━━━   ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_BACK))Sleep(1);

			mtx.lock();
			color(LINIJAP, TAST);
			gotoxy(33, 25); std::wcout << L"   ━━━   ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), LINR), input.end());
			break;
		}
		else if ((GetAsyncKeyState(VK_RSHIFT) || GetAsyncKeyState(VK_RCONTROL)) && (!input.size() || !count(input.begin(), input.end(), MUTE))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVER), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), MUTE);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(32, 36); std::wcout << L"    #     ";
			gotoxy(32, 37); std::wcout << L"    ♫     ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_RSHIFT) || GetAsyncKeyState(VK_RCONTROL))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(32, 36); std::wcout << L"    #     ";
			gotoxy(32, 37); std::wcout << L"    ♫     ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), MUTE), input.end());
			break;
		}
		else if ((GetAsyncKeyState(VK_LSHIFT) || GetAsyncKeyState(VK_LCONTROL)) && (!input.size() || !count(input.begin(), input.end(), MENU))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEL), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), MENU);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(7, 36); std::wcout << L"     *    ";
			gotoxy(7, 37); std::wcout << L"     +    ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_LSHIFT) || GetAsyncKeyState(VK_LCONTROL))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(7, 36); std::wcout << L"     *    ";
			gotoxy(7, 37); std::wcout << L"     +    ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), MENU), input.end());
			break;
		}
		else if (GetAsyncKeyState(VK_ESCAPE) && (!input.size() || !count(input.begin(), input.end(), ESC))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB9), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), ESC);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(SLUSALICAC, TASTT);
			gotoxy(33, 27); std::wcout << L"    ₵    ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_ESCAPE))Sleep(1);

			mtx.lock();
			color(SLUSALICAC, TAST);
			gotoxy(33, 27); std::wcout << L"    ₵    ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), ESC), input.end());
			break;
		}
		else if (GetAsyncKeyState(VK_CAPITAL) && (!input.size() || !count(input.begin(), input.end(), SLUS))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB7), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), SLUS);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(SLUSALICAZ, TASTT);
			gotoxy(7, 27); std::wcout << L"    C    ";
			mtx.unlock();

			while (GetAsyncKeyState(VK_CAPITAL))Sleep(1);

			mtx.lock();
			color(SLUSALICAZ, TAST);
			gotoxy(7, 27); std::wcout << L"    C    ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), SLUS), input.end());
			break;
		}
		else if ((GetAsyncKeyState('1') || GetAsyncKeyState(VK_NUMPAD7)) && (!input.size() || !count(input.begin(), input.end(), B1))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB1), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), B1);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(7, 30); std::wcout << L"     1    ";
			mtx.unlock();

			while (GetAsyncKeyState('1') || GetAsyncKeyState(VK_NUMPAD7))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(7, 30); std::wcout << L"     1    ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), B1), input.end());
			break;
		}
		else if ((GetAsyncKeyState('3') || GetAsyncKeyState(VK_NUMPAD9)) && (!input.size() || !count(input.begin(), input.end(), B3))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB3), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), B3);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(32, 30); std::wcout << L"    3     ";
			mtx.unlock();

			while (GetAsyncKeyState('3') || GetAsyncKeyState(VK_NUMPAD9))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(32, 30); std::wcout << L"    3     ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), B3), input.end());
			break;
		}
		else if ((GetAsyncKeyState('5') || GetAsyncKeyState(VK_NUMPAD5)) && (!input.size() || !count(input.begin(), input.end(), B5))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB5), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), B5);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(20, 32); std::wcout << L"    5    ";
			mtx.unlock();

			while (GetAsyncKeyState('5') || GetAsyncKeyState(VK_NUMPAD5))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(20, 32); std::wcout << L"    5    ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), B5), input.end());
			break;
		}
		else if ((GetAsyncKeyState('7') || GetAsyncKeyState(VK_NUMPAD1)) && (!input.size() || !count(input.begin(), input.end(), B7))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB7), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), B7);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(7, 34); std::wcout << L"     7    ";
			mtx.unlock();

			while (GetAsyncKeyState('7') || GetAsyncKeyState(VK_NUMPAD1))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(7, 34); std::wcout << L"     7    ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), B7), input.end());
			break;
		}
		else if ((GetAsyncKeyState('9') || GetAsyncKeyState(VK_NUMPAD3)) && (!input.size() || !count(input.begin(), input.end(), B9))) {
			if (!mute)
				PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB9), NULL, SND_RESOURCE | SND_ASYNC);
			input.insert(input.begin(), B9);
			thread th(KeyboardInput);
			th.detach();
			mtx.lock();
			color(DGM, TASTT);
			gotoxy(32, 34); std::wcout << L"    9     ";
			mtx.unlock();

			while (GetAsyncKeyState('9') || GetAsyncKeyState(VK_NUMPAD3))Sleep(1);

			mtx.lock();
			color(DGM, TAST);
			gotoxy(32, 34); std::wcout << L"    9     ";
			mtx.unlock();
			input.erase(remove(input.begin(), input.end(), B9), input.end());
			break;
		}
		Sleep(100);
	}
}

void MouseInput() {
	while (true) {
		while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
		SetConsoleMode(ConsoleInputHandle, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
		int x, y;
		DWORD tmp;
		INPUT_RECORD minput;

		GetNumberOfConsoleInputEvents(ConsoleInputHandle, &tmp);
		if (tmp)
			ReadConsoleInputW(ConsoleInputHandle, &minput, 1, &tmp);

		do {
			do {
				Sleep(1);
				GetNumberOfConsoleInputEvents(ConsoleInputHandle, &tmp);
			} while (!tmp);
			ReadConsoleInputW(ConsoleInputHandle, &minput, 1, &tmp);
		} while (minput.Event.MouseEvent.dwButtonState != 1);
		x = minput.Event.MouseEvent.dwMousePosition.X;
		y = minput.Event.MouseEvent.dwMousePosition.Y;

		if (y == 30) {
			if (x > 6 && x < 17) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB1), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(7, 30); std::wcout << L"     1    ";
				mtx.unlock();
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				mtx.lock();
				color(DGM, TAST);
				gotoxy(7, 30); std::wcout << L"     1    ";
				mtx.unlock();
			}
			else if (x > 19 && x < 30) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB2), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(20, 30); std::wcout << L"    2    ";
				mtx.unlock();

				input.insert(input.begin(), B2);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), B2), input.end());

				mtx.lock();
				color(DGM, TAST);
				gotoxy(20, 30); std::wcout << L"    2    ";
				mtx.unlock();
			}
			else if (x > 31 && x < 43) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB3), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(32, 30); std::wcout << L"    3     ";
				mtx.unlock();
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				mtx.lock();
				color(DGM, TAST);
				gotoxy(32, 30); std::wcout << L"    3     ";
				mtx.unlock();
			}
		}
		else if (y == 32) {
			if (x > 6 && x < 17) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB4), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(7, 32); std::wcout << L"     4    ";
				mtx.unlock();

				input.insert(input.begin(), B4);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), B4), input.end());

				mtx.lock();
				color(DGM, TAST);
				gotoxy(7, 32); std::wcout << L"     4    ";
				mtx.unlock();
			}
			else if (x > 19 && x < 30) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB5), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(20, 32); std::wcout << L"    5    ";
				mtx.unlock();
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				mtx.lock();
				color(DGM, TAST);
				gotoxy(20, 32); std::wcout << L"    5    ";
				mtx.unlock();
			}
			else if (x > 31 && x < 43) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB6), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(32, 32); std::wcout << L"    6     ";
				mtx.unlock();

				input.insert(input.begin(), B6);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), B6), input.end());

				mtx.lock();
				color(DGM, TAST);
				gotoxy(32, 32); std::wcout << L"    6     ";
				mtx.unlock();
			}
		}
		else if (y == 34) {
			if (x > 6 && x < 17) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB7), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(7, 34); std::wcout << L"     7    ";
				mtx.unlock();
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				mtx.lock();
				color(DGM, TAST);
				gotoxy(7, 34); std::wcout << L"     7    ";
				mtx.unlock();
			}
			else if (x > 19 && x < 30) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB8), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(20, 34); std::wcout << L"    8    ";
				mtx.unlock();

				input.insert(input.begin(), B8);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), B8), input.end());

				mtx.lock();
				color(DGM, TAST);
				gotoxy(20, 34); std::wcout << L"    8    ";
				mtx.unlock();
			}
			else if (x > 31 && x < 43) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB9), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(32, 34); std::wcout << L"    9     ";
				mtx.unlock();
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				mtx.lock();
				color(DGM, TAST);
				gotoxy(32, 34); std::wcout << L"    9     ";
				mtx.unlock();
			}
		}
		else if (y > 50 && y < 38) {
			if (x > 6 && x < 17) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEL), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(7, 36); std::wcout << L"     *    ";
				gotoxy(7, 37); std::wcout << L"     +    ";
				mtx.unlock();

				input.insert(input.begin(), MENU);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), MENU), input.end());

				mtx.lock();
				color(DGM, TAST);
				gotoxy(7, 36); std::wcout << L"     *    ";
				gotoxy(7, 37); std::wcout << L"     +    ";
				mtx.unlock();
			}
			else if (x > 19 && x < 30) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB0), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(20, 36); std::wcout << L"    0    ";
				gotoxy(20, 37); std::wcout << L"    ˽    ";
				mtx.unlock();

				input.insert(input.begin(), B0);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), B0), input.end());

				mtx.lock();
				color(DGM, TAST);
				gotoxy(20, 36); std::wcout << L"    0    ";
				gotoxy(20, 37); std::wcout << L"    ˽    ";
				mtx.unlock();
			}
			else if (x > 31 && x < 43) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVER), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(DGM, TASTT);
				gotoxy(32, 36); std::wcout << L"    #     ";
				gotoxy(32, 37); std::wcout << L"    ♫     ";
				mtx.unlock();

				input.insert(input.begin(), MUTE);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), MUTE), input.end());

				mtx.lock();
				color(DGM, TAST);
				gotoxy(32, 36); std::wcout << L"    #     ";
				gotoxy(32, 37); std::wcout << L"    ♫     ";
				mtx.unlock();
			}
		}
		else if (y == 25) {
			if (x > 6 && x < 16) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEL), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(LINIJAP, TASTT);
				gotoxy(7, 25); std::wcout << L"   ━━━   ";
				mtx.unlock();

				input.insert(input.begin(), LINL);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), LINL), input.end());

				mtx.lock();
				color(LINIJAP, TAST);
				gotoxy(7, 25); std::wcout << L"   ━━━   ";
				mtx.unlock();
			}
			else if (x > 32 && x < 42) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVER), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(LINIJAP, TASTT);
				gotoxy(33, 25); std::wcout << L"   ━━━   ";
				mtx.unlock();

				input.insert(input.begin(), LINR);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), LINR), input.end());

				mtx.lock();
				color(LINIJAP, TAST);
				gotoxy(33, 25); std::wcout << L"   ━━━   ";
				mtx.unlock();
			}
			else if (x > 20 && x < 28) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB2), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(TASTO, TASTT);
				gotoxy(21, 25); std::wcout << L"   ▲   ";
				mtx.unlock();

				input.insert(input.begin(), UP);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), UP), input.end());

				mtx.lock();
				color(TASTO, TAST);
				gotoxy(21, 25); std::wcout << L"   ▲   ";
				mtx.unlock();
			}
			else if (x > 17 && x < 21) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB4), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(TASTO, TASTT);
				gotoxy(18, 25); std::wcout << L"   ";
				gotoxy(18, 26); std::wcout << L" ◄ ";
				gotoxy(18, 27); std::wcout << L"   ";
				mtx.unlock();

				input.insert(input.begin(), LEFT);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), LEFT), input.end());

				mtx.lock();
				color(TASTO, TAST);
				gotoxy(18, 25); std::wcout << L"   ";
				gotoxy(18, 26); std::wcout << L" ◄ ";
				gotoxy(18, 27); std::wcout << L"   ";
				mtx.unlock();
			}
			else if (x > 27 && x < 31) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB6), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(TASTO, TASTT);
				gotoxy(28, 25); std::wcout << L"   ";
				gotoxy(28, 26); std::wcout << L" ► ";
				gotoxy(28, 27); std::wcout << L"   ";
				mtx.unlock();

				input.insert(input.begin(), RIGHT);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), RIGHT), input.end());

				mtx.lock();
				color(TASTO, TAST);
				gotoxy(28, 25); std::wcout << L"   ";
				gotoxy(28, 26); std::wcout << L" ► ";
				gotoxy(28, 27); std::wcout << L"   ";
				mtx.unlock();
			}
		}
		else if (y == 26) {
			if (x > 17 && x < 21) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB4), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(TASTO, TASTT);
				gotoxy(18, 25); std::wcout << L"   ";
				gotoxy(18, 26); std::wcout << L" ◄ ";
				gotoxy(18, 27); std::wcout << L"   ";
				mtx.unlock();

				input.insert(input.begin(), LEFT);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), LEFT), input.end());

				mtx.lock();
				color(TASTO, TAST);
				gotoxy(18, 25); std::wcout << L"   ";
				gotoxy(18, 26); std::wcout << L" ◄ ";
				gotoxy(18, 27); std::wcout << L"   ";
				mtx.unlock();
			}
			else if (x > 20 && x < 28) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB5), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(TASTO, TASTT);
				gotoxy(21, 26); std::wcout << L"       ";
				mtx.unlock();

				input.insert(input.begin(), ENTER);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), ENTER), input.end());

				mtx.lock();
				color(TASTO, TAST);
				gotoxy(21, 26); std::wcout << L"       ";
				mtx.unlock();
			}
			else if (x > 27 && x < 31) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB6), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(TASTO, TASTT);
				gotoxy(28, 25); std::wcout << L"   ";
				gotoxy(28, 26); std::wcout << L" ► ";
				gotoxy(28, 27); std::wcout << L"   ";
				mtx.unlock();

				input.insert(input.begin(), RIGHT);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), RIGHT), input.end());

				mtx.lock();
				color(TASTO, TAST);
				gotoxy(28, 25); std::wcout << L"   ";
				gotoxy(28, 26); std::wcout << L" ► ";
				gotoxy(28, 27); std::wcout << L"   ";
				mtx.unlock();
			}
		}
		else if (y == 27) {
			if (x > 6 && x < 16) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB7), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(SLUSALICAZ, TASTT);
				gotoxy(7, 27); std::wcout << L"    C    ";
				mtx.unlock();
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				mtx.lock();
				color(SLUSALICAZ, TAST);
				gotoxy(7, 27); std::wcout << L"    C    ";
				mtx.unlock();
			}
			else if (x > 32 && x < 42) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB9), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(SLUSALICAC, TASTT);
				gotoxy(33, 27); std::wcout << L"    ₵    ";
				mtx.unlock();

				input.insert(input.begin(), ESC);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), ESC), input.end());

				mtx.lock();
				color(SLUSALICAC, TAST);
				gotoxy(33, 27); std::wcout << L"    ₵    ";
				mtx.unlock();
			}
			else if (x > 20 && x < 28) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB8), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(TASTO, TASTT);
				gotoxy(21, 27); std::wcout << L"   ▼   ";
				mtx.unlock();

				input.insert(input.begin(), DOWN);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), DOWN), input.end());

				mtx.lock();
				color(TASTO, TAST);
				gotoxy(21, 27); std::wcout << L"   ▼   ";
				mtx.unlock();
			}
			else if (x > 17 && x < 21) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB4), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(TASTO, TASTT);
				gotoxy(18, 25); std::wcout << L"   ";
				gotoxy(18, 26); std::wcout << L" ◄ ";
				gotoxy(18, 27); std::wcout << L"   ";
				mtx.unlock();

				input.insert(input.begin(), LEFT);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), LEFT), input.end());

				mtx.lock();
				color(TASTO, TAST);
				gotoxy(18, 25); std::wcout << L"   ";
				gotoxy(18, 26); std::wcout << L" ◄ ";
				gotoxy(18, 27); std::wcout << L"   ";
				mtx.unlock();
			}
			else if (x > 27 && x < 31) {
				if (!mute)
					PlaySoundW(MAKEINTRESOURCEW(IDR_WAVEB6), NULL, SND_RESOURCE | SND_ASYNC);
				mtx.lock();
				color(TASTO, TASTT);
				gotoxy(28, 25); std::wcout << L"   ";
				gotoxy(28, 26); std::wcout << L" ► ";
				gotoxy(28, 27); std::wcout << L"   ";
				mtx.unlock();

				input.insert(input.begin(), RIGHT);
				while (GetAsyncKeyState(VK_LBUTTON))Sleep(1);
				input.erase(remove(input.begin(), input.end(), RIGHT), input.end());

				mtx.lock();
				color(TASTO, TAST);
				gotoxy(28, 25); std::wcout << L"   ";
				gotoxy(28, 26); std::wcout << L" ► ";
				gotoxy(28, 27); std::wcout << L"   ";
				mtx.unlock();
			}
		}
	}
}

void Mute() {
	while (true) {
		while (!input.size() || input[0] != MUTE)Sleep(1);

		if (!mute) {
			PlaySoundW(NULL, NULL, SND_ASYNC);
			mute = true;
			mtx.lock();
			gotoxy(36, 37);
			color(DGM, TASTT);
			std::wcout << L"♪";
			SakriKursor();
			mtx.unlock();
			while (input[0] == MUTE)Sleep(1);
			mtx.lock();
			gotoxy(36, 37);
			color(DGM, TAST);
			std::wcout << L"♪";
			SakriKursor();
			mtx.unlock();
		}
		else {
			mute = false;
			mtx.lock();
			gotoxy(36, 37);
			color(DGM, TASTT);
			std::wcout << L"♫";
			SakriKursor();
			mtx.unlock();
			while (input[0] == MUTE)Sleep(1);
			mtx.lock();
			gotoxy(36, 37);
			color(DGM, TAST);
			std::wcout << L"♫";
			SakriKursor();
			mtx.unlock();
		}

	}
}

#define FPS 80

void Animacija() {
	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L"                                   ";
	gotoxy(7, 10); std::wcout << L"                                   ";
	gotoxy(7, 11); std::wcout << L"                                   ";
	gotoxy(7, 12); std::wcout << L"                                   ";
	gotoxy(7, 13); std::wcout << L"                                   ";
	gotoxy(7, 14); std::wcout << L"                                   ";
	gotoxy(7, 15); std::wcout << L"                                   ";
	gotoxy(7, 16); std::wcout << L"                                   ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"                                   ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS * 10);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L"..`                                ";
	gotoxy(7, 10); std::wcout << L":-`                                ";
	gotoxy(7, 11); std::wcout << L"                                   ";
	gotoxy(7, 12); std::wcout << L":-`                                ";
	gotoxy(7, 13); std::wcout << L"-`                                 ";
	gotoxy(7, 14); std::wcout << L"                                   ";
	gotoxy(7, 15); std::wcout << L"                                   ";
	gotoxy(7, 16); std::wcout << L"                                   ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"                                   ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	PlaySoundW(MAKEINTRESOURCEW(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L"                                   ";
	gotoxy(7, 10); std::wcout << L"```````                            ";
	gotoxy(7, 11); std::wcout << L"///+/-.                            ";
	gotoxy(7, 12); std::wcout << L"/-`                                ";
	gotoxy(7, 13); std::wcout << L"+/:-```                            ";
	gotoxy(7, 14); std::wcout << L"++//:-.                            ";
	gotoxy(7, 15); std::wcout << L"++/-`                              ";
	gotoxy(7, 16); std::wcout << L"--`                                ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"                                   ";
	gotoxy(7, 19); std::wcout << L"                                 .-";
	gotoxy(7, 20); std::wcout << L"                                /ss";
	gotoxy(7, 21); std::wcout << L"                                .`.";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L"                                   ";
	gotoxy(7, 10); std::wcout << L"                                   ";
	gotoxy(7, 11); std::wcout << L".``.......`                        ";
	gotoxy(7, 12); std::wcout << L"///:/:::-.                         ";
	gotoxy(7, 13); std::wcout << L"oo+/:`                             ";
	gotoxy(7, 14); std::wcout << L"/++//::.``                         ";
	gotoxy(7, 15); std::wcout << L":://+///:-`                        ";
	gotoxy(7, 16); std::wcout << L"+/++//:.`                          ";
	gotoxy(7, 17); std::wcout << L":++-.`                       .////:";
	gotoxy(7, 18); std::wcout << L"                            .++++++";
	gotoxy(7, 19); std::wcout << L"                             `.//:/";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L"                                   ";
	gotoxy(7, 10); std::wcout << L"                                   ";
	gotoxy(7, 11); std::wcout << L"         ````                      ";
	gotoxy(7, 12); std::wcout << L".-----:::///-                      ";
	gotoxy(7, 13); std::wcout << L"/++++/:-```                        ";
	gotoxy(7, 14); std::wcout << L"::/++//:-`                `-//:-.` ";
	gotoxy(7, 15); std::wcout << L"+:::://///:-.`            //:/++/:-";
	gotoxy(7, 16); std::wcout << L"oo/////++/:.``            ``:///+os";
	gotoxy(7, 17); std::wcout << L" `:+++/:-`                   ````-:";
	gotoxy(7, 18); std::wcout << L"   ```                             ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L"                                   ";
	gotoxy(7, 10); std::wcout << L"                                   ";
	gotoxy(7, 11); std::wcout << L"                                   ";
	gotoxy(7, 12); std::wcout << L"-.``.-..--:://:.        `-::-`     ";
	gotoxy(7, 13); std::wcout << L"//:://///:....`        `::://+/-`` ";
	gotoxy(7, 14); std::wcout << L"s+:::////:-`             ..-//++++/";
	gotoxy(7, 15); std::wcout << L"+s+/::::::::-..``         `::-:/++o";
	gotoxy(7, 16); std::wcout << L" ./++:::///+/:-.`                 `";
	gotoxy(7, 17); std::wcout << L"   `./+++//-.`                     ";
	gotoxy(7, 18); std::wcout << L"      `.`                          ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L"                                   ";
	gotoxy(7, 10); std::wcout << L"                                   ";
	gotoxy(7, 11); std::wcout << L".`              ``    ```          ";
	gotoxy(7, 12); std::wcout << L":--.``.-----::///:  `.-://:-.``   `";
	gotoxy(7, 13); std::wcout << L"+///::://///-````     `...:++/////+";
	gotoxy(7, 14); std::wcout << L"oooo/:::///::.`           .+++++++o";
	gotoxy(7, 15); std::wcout << L"`.-+o+:--::::::---.      ::.``   ``";
	gotoxy(7, 16); std::wcout << L"    .///::://///:-.                ";
	gotoxy(7, 17); std::wcout << L"      `.////::.`                   ";
	gotoxy(7, 18); std::wcout << L"         ``                        ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L"                                   ";
	gotoxy(7, 10); std::wcout << L"`                                  ";
	gotoxy(7, 11); std::wcout << L"-..``        ```````               ";
	gotoxy(7, 12); std::wcout << L":::---...----::://:------......---:";
	gotoxy(7, 13); std::wcout << L"o++///::://+//:`       .-+++++oooos";
	gotoxy(7, 14); std::wcout << L".-/++o+:::://::-.```     /o+/-...--";
	gotoxy(7, 15); std::wcout << L"   ``-++/:--::://///:  `:/.`       ";
	gotoxy(7, 16); std::wcout << L"       .://::////:-``  ``          ";
	gotoxy(7, 17); std::wcout << L"          .-:...`                  ";
	gotoxy(7, 18); std::wcout << L"                                   ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L"`                                  ";
	gotoxy(7, 10); std::wcout << L"..``                               ";
	gotoxy(7, 11); std::wcout << L"---..```   ````.......         ``..";
	gotoxy(7, 12); std::wcout << L"+/:::::----:::::/:::::.....--:://++";
	gotoxy(7, 13); std::wcout << L"/++++///:::////::...-://++++++++++o";
	gotoxy(7, 14); std::wcout << L"  `.-:/o+:-::::::::-:-``/o+:`` ````";
	gotoxy(7, 15); std::wcout << L"       `:+/::-://++//: .+:`        ";
	gotoxy(7, 16); std::wcout << L"         `.:///:/:..   /.          ";
	gotoxy(7, 17); std::wcout << L"            ```                    ";
	gotoxy(7, 18); std::wcout << L"                                   ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L".`                                 ";
	gotoxy(7, 10); std::wcout << L"-..``                              ";
	gotoxy(7, 11); std::wcout << L":---...``` ``....-...--`    ```..-:";
	gotoxy(7, 12); std::wcout << L"++//:::::--::://:::-----..-::/++++o";
	gotoxy(7, 13); std::wcout << L".-://++//:-://+/++//////++++///////";
	gotoxy(7, 14); std::wcout << L"    ``.:/o/:-://+o++++++o+-`       ";
	gotoxy(7, 15); std::wcout << L"         .://:://++//-:+:`         ";
	gotoxy(7, 16); std::wcout << L"           `-:/:---`` /.           ";
	gotoxy(7, 17); std::wcout << L"                      `            ";
	gotoxy(7, 18); std::wcout << L"                                   ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"`                                  ";
	gotoxy(7, 9);  std::wcout << L"..`                                ";
	gotoxy(7, 10); std::wcout << L"-...```                          ``";
	gotoxy(7, 11); std::wcout << L"/:----...````....----:::`  ``..--:/";
	gotoxy(7, 12); std::wcout << L"+++///::::--:://///:::::--:://++ooo";
	gotoxy(7, 13); std::wcout << L"``.-://++/:-::////::::://++///:::::";
	gotoxy(7, 14); std::wcout << L"      `.-:+/:::/+ooo+o+oo/`        ";
	gotoxy(7, 15); std::wcout << L"          `:///+++/+::++.          ";
	gotoxy(7, 16); std::wcout << L"             .::-..``-/`           ";
	gotoxy(7, 17); std::wcout << L"                     `             ";
	gotoxy(7, 18); std::wcout << L"                                   ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"`                                  ";
	gotoxy(7, 9);  std::wcout << L"..`                                ";
	gotoxy(7, 10); std::wcout << L"-...```                          ``";
	gotoxy(7, 11); std::wcout << L"/:----...````....----:::`  ``..--:/";
	gotoxy(7, 12); std::wcout << L"+++///::::--:://///:::::--:://++ooo";
	gotoxy(7, 13); std::wcout << L"``.-://++/:-::////::::://++///:::::";
	gotoxy(7, 14); std::wcout << L"      `.-:+/:::/+ooo+o+oo/`        ";
	gotoxy(7, 15); std::wcout << L"          `:///+++/+::++.          ";
	gotoxy(7, 16); std::wcout << L"             .::-..``-/`           ";
	gotoxy(7, 17); std::wcout << L"                     `             ";
	gotoxy(7, 18); std::wcout << L"                                   ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"``                                 ";
	gotoxy(7, 9);  std::wcout << L"..``                               ";
	gotoxy(7, 10); std::wcout << L"--...```                         ``";
	gotoxy(7, 11); std::wcout << L"//:---.....``..---:::///:  ``..-::/";
	gotoxy(7, 12); std::wcout << L"/+++//:::::-::/+++//::::--::/+++ooo";
	gotoxy(7, 13); std::wcout << L" ``.-://++/:-://///://://++///////:";
	gotoxy(7, 14); std::wcout << L"       `.-/+:::///+o+o+oo:`        ";
	gotoxy(7, 15); std::wcout << L"           .://+++//::++-          ";
	gotoxy(7, 16); std::wcout << L"             `--.``` //`           ";
	gotoxy(7, 17); std::wcout << L"                     `             ";
	gotoxy(7, 18); std::wcout << L"                                   ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"``                                 ";
	gotoxy(7, 9);  std::wcout << L"..``                               ";
	gotoxy(7, 10); std::wcout << L"--....``                         ``";
	gotoxy(7, 11); std::wcout << L"//::---....``.----:::////` ```.--:/";
	gotoxy(7, 12); std::wcout << L"//++///::::-://++////://:-:://+++oo";
	gotoxy(7, 13); std::wcout << L" ``.--://++:-:/+++o+o////++////////";
	gotoxy(7, 14); std::wcout << L"        `.:+/::/oooo+ooso:`        ";
	gotoxy(7, 15); std::wcout << L"           `-:/++//:-:o+-          ";
	gotoxy(7, 16); std::wcout << L"             `.``    /-`           ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"     ``  `   ``   ` `  `    `      ";
	gotoxy(7, 19); std::wcout << L"     ` ` `  `  `  ``   `   `.`     ";
	gotoxy(7, 20); std::wcout << L"     `  ``   ``   ` `  `  `   `    ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"``                                 ";
	gotoxy(7, 9);  std::wcout << L"..```                              ";
	gotoxy(7, 10); std::wcout << L"--....``                          `";
	gotoxy(7, 11); std::wcout << L"+/::------....----::::/::`  ``..-::";
	gotoxy(7, 12); std::wcout << L"//+++///:::-://++://::/+/--::/+++oo";
	gotoxy(7, 13); std::wcout << L"  ``.-://++:-:/++++++++//++++++++++";
	gotoxy(7, 14); std::wcout << L"        ``-+/::+o+ooososo/```      ";
	gotoxy(7, 15); std::wcout << L"           `-://++//:+++:          ";
	gotoxy(7, 16); std::wcout << L"              ```   `-.`           ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"     :-  :   --   : -  :    -      ";
	gotoxy(7, 19); std::wcout << L"     : - :  :  :  :-   :   -_-     ";
	gotoxy(7, 20); std::wcout << L"     :  -:   --   : -  :  -   -    ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L".`                                 ";
	gotoxy(7, 9);  std::wcout << L"..```                              ";
	gotoxy(7, 10); std::wcout << L"---...```                          ";
	gotoxy(7, 11); std::wcout << L"+//::-----....----:::::::`   ``..-:";
	gotoxy(7, 12); std::wcout << L"//+++////::-:/+++//:::/+/---://+++o";
	gotoxy(7, 13); std::wcout << L"  ``.-://++:::////+++o+//+o+++++++o";
	gotoxy(7, 14); std::wcout << L"        ``-+/::/+oooososo+.````````";
	gotoxy(7, 15); std::wcout << L"           `-:/+++//:o++:          ";
	gotoxy(7, 16); std::wcout << L"              ````  `.`            ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"     ••  •   ••   • •  •    •      ";
	gotoxy(7, 19); std::wcout << L"     • • •  •  •  ••   •   •…•     ";
	gotoxy(7, 20); std::wcout << L"     •  ••   ••   • •  •  •   •    ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"``                                 ";
	gotoxy(7, 9);  std::wcout << L"..```                              ";
	gotoxy(7, 10); std::wcout << L"---...``                           ";
	gotoxy(7, 11); std::wcout << L"+//::-----....----:::::::`   ``..-:";
	gotoxy(7, 12); std::wcout << L"//+++////::::/+++/::://+/---://+++o";
	gotoxy(7, 13); std::wcout << L"  `..-://++:::///:+/+o+//+o++++++oo";
	gotoxy(7, 14); std::wcout << L"        ``-+/::/+oooososs+.````````";
	gotoxy(7, 15); std::wcout << L"           `-:/+++/+/s++-          ";
	gotoxy(7, 16); std::wcout << L"              ..``  ```            ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"     wl  w   aa   w a  w    a      ";
	gotoxy(7, 19); std::wcout << L"     w l w  w  w  wk   w   a_a     ";
	gotoxy(7, 20); std::wcout << L"     w  lw   vv   w v  w  a   a    ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"``                                 ";
	gotoxy(7, 9);  std::wcout << L"..```                              ";
	gotoxy(7, 10); std::wcout << L"---...``                           ";
	gotoxy(7, 11); std::wcout << L"+/:::----..``.----:::::::`   ``..-:";
	gotoxy(7, 12); std::wcout << L"/++++////::::/+++/::::/+/---://+++o";
	gotoxy(7, 13); std::wcout << L" ``.--:/+++:::///:+/+o+//+o++++++oo";
	gotoxy(7, 14); std::wcout << L"        `.:+/::/+oooososs+.````````";
	gotoxy(7, 15); std::wcout << L"           `://++o/+/o++-          ";
	gotoxy(7, 16); std::wcout << L"              ..``  ```            ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"     MA  M   AA   M A  M    A      ";
	gotoxy(7, 19); std::wcout << L"     M A M  M  M  MK   M   A_A     ";
	gotoxy(7, 20); std::wcout << L"     M  AM   VV   M V  M  A   A    ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"``                                 ";
	gotoxy(7, 9);  std::wcout << L"..``                               ";
	gotoxy(7, 10); std::wcout << L"--....``                           ";
	gotoxy(7, 11); std::wcout << L"//:::----..``.----::--:::`    `..-:";
	gotoxy(7, 12); std::wcout << L"/++++/////:::/++++/:::/+/-.-://+++o";
	gotoxy(7, 13); std::wcout << L" ``.-://+++:::/++:+/+o+//+o+ooo+ooo";
	gotoxy(7, 14); std::wcout << L"       ``.:+/::/+oooososs+.````````";
	gotoxy(7, 15); std::wcout << L"           `://++o/+/o++:`         ";
	gotoxy(7, 16); std::wcout << L"              ...`` ```            ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"     ▒▒  ▒   ▒▒   ▒ ▒  ▒    ▒      ";
	gotoxy(7, 19); std::wcout << L"     ▒ ▒ ▒  ▒  ▒  ▒▒   ▒   ▒˽▒     ";
	gotoxy(7, 20); std::wcout << L"     ▒  ▒▒   ▒▒   ▒ ▒  ▒  ▒   ▒    ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"``                                 ";
	gotoxy(7, 9);  std::wcout << L"..``                               ";
	gotoxy(7, 10); std::wcout << L"--...```                           ";
	gotoxy(7, 11); std::wcout << L"//::----...``...---:-----`    ``.-:";
	gotoxy(7, 12); std::wcout << L"+++++/////::://+++/:::/+/..-://+++o";
	gotoxy(7, 13); std::wcout << L"``..-://++/:::/++:+/+o+//+ooooooooo";
	gotoxy(7, 14); std::wcout << L"       ``-:+/:://o+oososso-````````";
	gotoxy(7, 15); std::wcout << L"           .://++o/o/o++:`         ";
	gotoxy(7, 16); std::wcout << L"             `.-.``````            ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"     ▓▒  ▓   ▒▒   ▓ ▓  ▓    ▓      ";
	gotoxy(7, 19); std::wcout << L"     ▓ ▒ ▓  ▓  ▓  ▓▒   ▓   ▒˽▒     ";
	gotoxy(7, 20); std::wcout << L"     ▓  ▒▓   ▒▒   ▓ ▓  ▓  ▓   ▓    ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"``                                 ";
	gotoxy(7, 9);  std::wcout << L"..``                               ";
	gotoxy(7, 10); std::wcout << L"--...```                           ";
	gotoxy(7, 11); std::wcout << L"//::----..````....-------`    ``.--";
	gotoxy(7, 12); std::wcout << L"+++++/////::://+++/:/:/+/..--://++o";
	gotoxy(7, 13); std::wcout << L"``.-://+++/:::/++/+/++///+ooooooooo";
	gotoxy(7, 14); std::wcout << L"       `.-/+/:://o+oososso-.```````";
	gotoxy(7, 15); std::wcout << L"           .:///+o+o/o++:`         ";
	gotoxy(7, 16); std::wcout << L"             `.-..```.`            ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"     ▓▓  ▓   ▓▓   ▓ ▓  ▓    ▓      ";
	gotoxy(7, 19); std::wcout << L"     ▓ ▓ ▓  ▓  ▓  ▓▓   ▓   ▓‗▓     ";
	gotoxy(7, 20); std::wcout << L"     ▓  ▓▓   ▓▓   ▓ ▓  ▓  ▓   ▓    ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"``                                 ";
	gotoxy(7, 9);  std::wcout << L"..``                               ";
	gotoxy(7, 10); std::wcout << L"--...```                           ";
	gotoxy(7, 11); std::wcout << L"//::----..````....--..-..`    ``..-";
	gotoxy(7, 12); std::wcout << L"+++++/////:-://+++/:///++...-://++o";
	gotoxy(7, 13); std::wcout << L"``.-://+++/::/+++/+/++/://+oooooooo";
	gotoxy(7, 14); std::wcout << L"      ``.:/+/:://o+oososso:........";
	gotoxy(7, 15); std::wcout << L"           .////+o+o/s+o/`         ";
	gotoxy(7, 16); std::wcout << L"             `-:..```.``           ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"     ██  █   ██   █ █  █    █      ";
	gotoxy(7, 19); std::wcout << L"     █ █ █  █  █  ██   █   █‗█     ";
	gotoxy(7, 20); std::wcout << L"     █  ██   ██   █ █  █  █   █    ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
	Sleep(FPS * 30);

	mtx.lock();
	color(EKRANO, EKRAN);
	gotoxy(7, 8);  std::wcout << L"                                   ";
	gotoxy(7, 9);  std::wcout << L"                                   ";
	gotoxy(7, 10); std::wcout << L"                                   ";
	gotoxy(7, 11); std::wcout << L"                                   ";
	gotoxy(7, 12); std::wcout << L"                                   ";
	gotoxy(7, 13); std::wcout << L"                                   ";
	gotoxy(7, 14); std::wcout << L"                                   ";
	gotoxy(7, 15); std::wcout << L"                                   ";
	gotoxy(7, 16); std::wcout << L"                                   ";
	gotoxy(7, 17); std::wcout << L"                                   ";
	gotoxy(7, 18); std::wcout << L"                                   ";
	gotoxy(7, 19); std::wcout << L"                                   ";
	gotoxy(7, 20); std::wcout << L"                                   ";
	gotoxy(7, 21); std::wcout << L"                                   ";
	mtx.unlock();
}