#include <iostream>
#include <Windows.h>


#pragma comment(lib, "SDL2.lib")
//#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#include <SDL.h>
//#include <SDL_main.h>
#include <SDL_image.h>


enum GAMESTATE { TITLENEW, TITLESAFED, TITLEOPTIONS } GameState;

void HandleKeyboardInput(SDL_Keysym key, bool keydown);
void SDLCleanup(SDL_Texture* tmp, SDL_Window *tmp2, SDL_Renderer *tmp3);

int main()
{
	// SDL mit dem Grafiksystem Initialisierenx
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		// Fehler Initialisierung fehlgeschlagen
		std::cerr << "Fehler SDL konnte nicht initialisiert werden: " << SDL_GetError() << '\n';
		return 0;
	}

	// Fenster erstellen grösse 800 x 600 mit 32 bit für vollbild SDL_FULLSCREEN hinzufügen

	int screenX = GetSystemMetrics(SM_CXSCREEN);
	int screenY = GetSystemMetrics(SM_CYSCREEN);

	SDL_Window * Win = SDL_CreateWindow("TowerD", screenX / 2 - 512, screenY / 2 - 290, 1024, 580, SDL_WINDOW_SHOWN);

	//SetForegroundWindow(FindWindow(0, "Shaiya Mystra Launcher"));
	//ShowWindow(FindWindow(0, "Shaiya Philippines - Updater"), SW_HIDE);


	if (!Win)
	{
		std::cerr << "SDL Fenster konnte nicht erzeugt werden: " << SDL_GetError() << '\n';
		SDL_Quit();
		return 0;
	}

	// Renderer erstellen

	SDL_Renderer *ren = SDL_CreateRenderer(Win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (ren == nullptr)
	{
		SDL_DestroyWindow(Win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// [nach der SDL-Initialisierung] Hier laden wir unser Bild mit SDL_image.
	char maindirectory[256] = { 0 };
	char titelscreen[256] = { 0 };

	GetCurrentDirectoryA(255, maindirectory);

	strcpy(titelscreen, maindirectory);

	strcat(titelscreen, "\\data\\startmenu.png");

	SDL_Surface* image = IMG_Load(titelscreen);

	if (!image)
	{
		// Fehler!
		std::cerr << SDL_GetError();
		return -1;
	}


	SDL_Texture* titeltex = SDL_CreateTextureFromSurface(ren, image);

	if (!image)
	{
		// Fehler!
		std::cerr << SDL_GetError();
		return -1;
	}

	SDL_FreeSurface(image);

	bool rungame = true;

	while (rungame)
	{
		SDL_Event event;
		
		/* 
		int hover = CheckMouse();

		if (peter == 1)
		{
			GameState = PLAYHOVER;
		}
		else if (peter == 2) // website
		{
			GameState = REGISTERHOVER;
		}
		else if (peter == 3) // options
		{
			GameState = OPTIONSHOVER;
		}
		else
		{
			GameState = TITELNEW;
		}
		*/

		// Eventschleife vom SDL Fenster

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{

			case SDL_MOUSEMOTION:
			{
				//
				break;
			}
			case SDL_QUIT:
				rungame = false;
				break;
			case SDL_KEYDOWN:
				HandleKeyboardInput(event.key.keysym, true);
				break;
			case SDL_KEYUP:
				HandleKeyboardInput(event.key.keysym, false);
				break;
			}
		}

		//First clear the renderer
		SDL_RenderClear(ren);

		switch (GameState)
		{
		case TITLENEW:
			//Draw the texture
			SDL_RenderCopy(ren, titeltex, NULL, NULL);
			//Update the screen
			SDL_RenderPresent(ren);
			break;
		case TITLESAFED:
		{
			//Draw the texture
			SDL_Rect DestR;
			DestR.h = 200;
			DestR.w = 200;
			DestR.x = 421;
			DestR.y = 394;
			//DestR.h
			SDL_RenderCopy(ren, titeltex, NULL, NULL);
			//SDL_RenderCopy(ren, titelsafedtex, NULL, &DestR);
			//Update the screen
			SDL_RenderPresent(ren);
			break;
		}
		case TITLEOPTIONS:
		{
			//Draw the texture
			SDL_Rect DestR;
			DestR.h = 50;
			DestR.w = 110;
			DestR.x = 722;
			DestR.y = 489;
			//DestR.h
			//SDL_RenderCopy(ren, titeltex, NULL, NULL);
			//SDL_RenderCopy(ren, registerhovertext, NULL, &DestR);
			//Update the screen
			SDL_RenderPresent(ren);
			break;
		}
		default:
			//Draw the texture
			SDL_RenderCopy(ren, titeltex, NULL, NULL);
			//Update the screen
			SDL_RenderPresent(ren);
			break;
		}

		SDL_Delay(2);

		// Spielcode Grafiken etc

	} // Spiel beendet

	SDLCleanup(titeltex, Win, ren);
	SDL_Quit();
	return 0;
}

void SDLCleanup(SDL_Texture* tmp, SDL_Window *tmp2, SDL_Renderer *tmp3)
{
	SDL_DestroyTexture(tmp);
	SDL_DestroyRenderer(tmp3);
	SDL_DestroyWindow(tmp2);
}

void HandleKeyboardInput(SDL_Keysym key, bool keydown)
{
	if (keydown)
	{
		switch (key.scancode)
		{
		case SDL_SCANCODE_W:
			if( GameState == TITLESAFED )
			{
			GameState = TITLENEW;
			}
			// std::cout << "w pressed\n";
			break;
		case SDL_SCANCODE_A:
			//std::cout << "A pressed\n";
			break;
		case SDL_SCANCODE_S:
			if (GameState == TITLENEW )
			{
				GameState = TITLESAFED;
			}
			std::cout << "S pressed\n";
			break;
		case SDL_SCANCODE_D:
			std::cout << "D pressed\n";
			break;
		case SDL_SCANCODE_RETURN:
			//PlaySound(MAKEINTRESOURCE(IDR_WAVE2), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_MEMORY);
			Sleep(2750);
			std::cout << "ENTER pressed\n";
			break;
		}
	}
	else
	{
		switch (key.scancode)
		{
		case SDL_SCANCODE_W:
			std::cout << "w released\n";
			break;
		case SDL_SCANCODE_A:
			std::cout << "A released\n";
			break;
		case SDL_SCANCODE_S:
			std::cout << "S released\n";
			break;
		case SDL_SCANCODE_D:
			std::cout << "D released\n";
			break;
		case SDL_SCANCODE_RETURN:
			std::cout << "ENTER released\n";
			break;
		}
	}
}
