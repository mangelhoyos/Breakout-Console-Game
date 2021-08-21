#include "olcConsoleGameEngine.h"
#include <iostream>

using namespace std;

class Breakout : public olcConsoleGameEngine {

public:
	Breakout() {

	}

	string level;
	int nWidth = 16;
	int nHeight = 15;
	int nBlock = 8;

	float fBat = 64.0f;

	float fBallX = 64.0f;
	float fBallY = 64.0f;
	float fBallDX, fBallDY; //Deltas de la pelota


public:
	bool OnUserCreate() {

		//Level string

		level += "################";
		level += "#..............#";
		level += "#...11111111...#";
		level += "#...11111111...#";
		level += "#...11111111...#";
		level += "#..............#";
		level += "#..............#";
		level += "#..............#";
		level += "#..............#";
		level += "#..............#";
		level += "#..............#";
		level += "#..............#";
		level += "#..............#";
		level += "#..............#";
		level += "#..............#";

		//Angulo de empujo inicial a la pelota
		float fAng = ((float)rand() / (float)(RAND_MAX)) * 3.14159f * 2.0f;
		fAng = 0.7f;
		fBallDX = cosf(fAng);
		fBallDY = sinf(fAng);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) {

		//Preferencias de gameplay
		int nBatWidth = 10;
		float playerVelocity = 65.0f;
		float fBallSpeed = 55.0f;

		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

		//Movimiento de la barra del jugador
		if (GetKey(VK_LEFT).bHeld) {
			fBat -= playerVelocity * fElapsedTime;
		}
		if (GetKey(VK_RIGHT).bHeld) {
			fBat += playerVelocity * fElapsedTime;
		}

		//Limitar el movimiento de la barra del jugador para que no traspase los muros
		if (fBat - nBatWidth < nBlock) {
			fBat = nBlock + nBatWidth;
		}
		if (fBat + nBatWidth > (nWidth-1) * nBlock) {
			fBat = (nWidth - 1) * nBlock - nBatWidth;
		}

		//Guardar la posición anterior para checkear las colisiones mas adelante
		float fOldX = fBallX;
		float fOldY = fBallY;

		//Mover la pelota
		fBallX += fBallDX * fElapsedTime * fBallSpeed;
		fBallY += fBallDY * fElapsedTime * fBallSpeed;


		//Detectar collisiones de la pelota

		//Posición dada en bloques de la pelota
		int nCellOldX = (int)fOldX / nBlock;
		int nCellOldY = (int)fOldY / nBlock;

		int nCellNewX = (int)fBallX / nBlock;
		int nCellNewY = (int)fBallY / nBlock;

		//Se interpreta la posición de la pelota en el arreglo de 1 dimensión del nivel para saber con que esta colisionando la pelota
		char newcell = level[nCellNewY * nWidth + nCellNewX];
		char oldcell = level[nCellOldY * nWidth + nCellOldX];

		//Verificar si la colisión es con la pared o uno de los bloques destruibles
		if (newcell != '.') {

			if (newcell == '1') level[nCellNewY * nWidth + nCellNewX] = '.';

			if (nCellNewX != nCellOldX) fBallDX *= -1;
			if (nCellNewY != nCellOldY) fBallDY *= -1;
		
		}

		//Colision con la barra del jugador
		if (fBallY > nHeight * nBlock - 2) {
			if (fBallX > (fBat - nBatWidth) && (fBallX < fBat + nBatWidth)) {
				fBallDY *= -1;
			}
			else {
				//Game Over
				fBallX = (nWidth / 2.0f) * nBlock;
				fBallY = (nHeight / 2.0f) * nBlock;

				//Angulo de lanzamiento de pelota inicial después de reaparecer en el medio
				float fAng = ((float)rand() / (float)(RAND_MAX)) * 3.14159f * 2.0f;
				fAng = 0.7f;
				fBallDX = cosf(fAng);
				fBallDY = sinf(fAng);
			}
		}



		//Dibujar nivel con respecto al string de nivel

		for (int y = 0; y < nHeight; y++) {
			for (int x = 0; x < nWidth; x++) {
				switch (level[y * nWidth + x])
				{
				case '#':
					Fill(x * nBlock, y * nBlock, (x + 1) * nBlock, (y + 1) * nBlock, PIXEL_SOLID, FG_WHITE);
					break;
				case '.':
					Fill(x * nBlock, y * nBlock, (x + 1) * nBlock, (y + 1) * nBlock, PIXEL_SOLID, FG_BLACK);
					break;
				case '1':
					Fill(x * nBlock, y * nBlock, (x + 1) * nBlock, (y + 1) * nBlock, PIXEL_SOLID, FG_RED);
					break;
				}

			}
		}

		//Dibujar pelota
		FillCircle(fBallX, fBallY, 2.0f, PIXEL_SOLID, FG_YELLOW);

		//Dibujar el controlador del jugador
		
		DrawLine(fBat - nBatWidth, nHeight * nBlock - 2, fBat + nBatWidth, nHeight * nBlock - 2, PIXEL_SOLID, FG_WHITE);

		return true;
	}

};


int main() {

	Breakout game;

	game.ConstructConsole(256,240,4,4);
	game.Start();

	return 0;
}