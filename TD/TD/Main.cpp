#include <Windows.h>
#include <iostream>
#include <Tlhelp32.h>
#include <process.h>

#ifdef WIN32
#undef main
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")

#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL/SDL.h>
#endif
#ifdef _WIN32
#undef main
#endif 

enum GAMESTATE { TITELNEW, TITELSAFED, TITELOPTION, TITELEND };
void HandleKeyboardInput(SDL_Keysym key, bool keydown);
int CheckMouse();

/* game.exe updater.exe notice.txt tips.txt data.sah data.saf */

GAMESTATE GameState = TITELNEW;

void SDLCleanup(SDL_Texture* tmp, SDL_Window *tmp2, SDL_Renderer *tmp3);
void SetDebugPrivilege();


int main()
{
	char dllpath[256] = { 0 };
	GetCurrentDirectory(255, dllpath);

	strcat_s<256>(dllpath, "\\libpng16-16.dll");

	if (!LoadLibraryA(dllpath))
	{
		memset(dllpath, 0, 255);
		sprintf_s<256>(dllpath, "LoadLibraryA Error: %X", GetLastError());
		std::cout << dllpath << '\n';
		system("PAUSE");
	}

	// PlaySound( soundpath, NULL, SND_ASYNC );
	// SDL mit dem Grafiksystem Initialisieren
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		// Fehler Initialisierung fehlgeschlagen
		std::cerr << "Fehler SDL konnte nicht initialisiert werden: " << SDL_GetError() << '\n';
		system("PAUSE");
		return 0;
	}

	//
	// Fenster erstellen grösse 1024 x 580 mit 32 bit für vollbild SDL_FULLSCREEN hinzufügen

	int screenX = GetSystemMetrics(SM_CXSCREEN);
	int screenY = GetSystemMetrics(SM_CYSCREEN);

	SDL_Window * Win = SDL_CreateWindow("TowerD", screenX / 2 - 512, screenY / 2 - 290, 1024, 580, SDL_WINDOW_SHOWN );

	SetForegroundWindow(FindWindow(0, "TowerD"));

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
	char titeldirectory[256] = { 0 };
	char saveddirectory[256] = { 0 };
	char optiondirectory[256] = { 0 };
	char enddirectory[256] = { 0 };

	GetCurrentDirectory(255, maindirectory);


	strcpy(titeldirectory, maindirectory);
	strcpy(saveddirectory, maindirectory);
	strcpy(optiondirectory, maindirectory);
	strcpy(enddirectory, maindirectory);

	strcat(titeldirectory, "\\data\\startbild.png");
	strcat(saveddirectory, "\\data\\save.png");
	strcat(optiondirectory, "\\data\\options.png");
	strcat(enddirectory, "\\data\\end.png");


	SDL_Surface* image = IMG_Load(titeldirectory);
	SDL_Surface* imagesaved = IMG_Load(saveddirectory);
	SDL_Surface* imageoption = IMG_Load(optiondirectory);
	SDL_Surface* imageend = IMG_Load(enddirectory);

	if ( !image || !imagesaved || !imageoption || !imageend)
	{
		// Fehler!
		std::cerr << SDL_GetError();
		MessageBox(0, titeldirectory, saveddirectory, MB_OK); // Mach mal feddich du Jude
		return -1;
	}


	SDL_Texture* titeltex = SDL_CreateTextureFromSurface(ren, image);
	SDL_Texture* savedtex = SDL_CreateTextureFromSurface(ren, imagesaved);
	SDL_Texture* optiontex = SDL_CreateTextureFromSurface(ren, imageoption);
	SDL_Texture* endtex = SDL_CreateTextureFromSurface(ren, imageend);

	if ( !titeltex || !savedtex || !imageoption || !imageend)
	{
		// Fehler!
		std::cerr << SDL_GetError();
		MessageBox(0, titeldirectory, saveddirectory, MB_OK); // same du Mongo
		return -1;
	}

	SDL_FreeSurface(image);
	SDL_FreeSurface(imagesaved);
	SDL_FreeSurface(imageoption);
	SDL_FreeSurface(imageend);

	bool rungame = true;

	while (rungame)
	{
		SDL_Event event;
		
		/*
		int peter = CheckMouse();

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
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			switch (peter)
			{
			case 1: // Start game
			{
				if (timer == 0)
				{
					timer = GetTickCount();
					if (GetTickCount() - timer < 200)
					{
						std::cout << "starting\n";
						PlaySound(MAKEINTRESOURCE(IDR_WAVE2), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_MEMORY);
						Sleep(2000);
						StartGame();
						SDL_Quit();
						return 0;
					}
				}
				else
				{
					if (GetTickCount() - timer > 200)
					{
						std::cout << "starting\n";
						PlaySound(MAKEINTRESOURCE(IDR_WAVE2), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_MEMORY);
						Sleep(2000);
						StartGame();
						SDL_Quit();
						return 0;
					}
				}
				break;
			}
			case 2: // Register
			{
				if (!website)
				{
					OpenWebsite("http://shaiyamystra.com/accounts/register.php");
					website = true;
				}
				break;
			}
			case 3: // Change Options in ini file
			{
				// Draw Menu - open winapi file ? :pppp
				StartConfig();
			}
			}
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
		case TITELNEW:
			//Draw the texture
			SDL_RenderCopy(ren, titeltex, NULL, NULL);
			//Update the screen
			SDL_RenderPresent(ren);
			break;
		case TITELSAFED:
			//Draw the texture
			SDL_RenderCopy(ren, savedtex, NULL, NULL);
			//Update the screen
			SDL_RenderPresent(ren);
			break;
		case TITELOPTION:
			//Draw the texture
			SDL_RenderCopy(ren, optiontex, NULL, NULL);
			//Update the screen
			SDL_RenderPresent(ren);
			break;
		case TITELEND:
			//Draw the texture
			SDL_RenderCopy(ren, endtex, NULL, NULL);
			//Update the screen
			SDL_RenderPresent(ren);
			break;
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
			switch (GameState)
			{
			case TITELNEW:
				GameState = TITELEND;
				break;
			case TITELSAFED:
				GameState = TITELNEW;
				break;
			case TITELOPTION:
				GameState = TITELSAFED;
				break;
			case TITELEND:
				GameState = TITELOPTION;
				break;
			}
			std::cout << "w pressed\n";
			break;
		case SDL_SCANCODE_A:
			std::cout << "A pressed\n";
			break;
		case SDL_SCANCODE_S:
			switch (GameState)
			{
			case TITELNEW:
				GameState = TITELSAFED;
				break;
			case TITELSAFED:
				GameState = TITELOPTION;
				break;
			case TITELOPTION:
				GameState = TITELEND;
				break;
			case TITELEND:
				GameState = TITELNEW;
				break;
			}
			std::cout << "S pressed\n";
			break;
		case SDL_SCANCODE_D:
			std::cout << "D pressed\n";
			break;
		case SDL_SCANCODE_RETURN:
			std::cout << "ENTER pressed\n";
			//test();
			/* 449 449 - 602 579well

			*/
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

int CheckMouse()
{
	RECT wd = { 0 };
	POINT mouse = { 0 };

	//GetWindow
	GetWindowRect(FindWindow(0, "Shaiya Mystra Launcher"), &wd);

	char buffer[100] = { 0 };

	GetCursorPos(&mouse);

	/* debug
	sprintf_s<100>( buffer, "a: %d b: %d c: %d d: %d\n", wd.left, wd.top, mouse.x, mouse.y );

	MessageBox( 0, buffer, 0, MB_OK );
	*/

	mouse.x -= wd.left;
	mouse.y -= wd.top;

	if (mouse.x > 449 && mouse.x < 602 && mouse.y > 449 && mouse.y < 579) // Play button
	{
		std::cout << "starting Game\n";
		return 1;
	}
	else if (mouse.x > 626 && mouse.x < 716 && mouse.y > 525 && mouse.y < 545) // Options
	{
		std::cout << "Options\b";
		return 3;
	}
	else if (mouse.x > 733 && mouse.x < 825 && mouse.y > 526 && mouse.y < 545) // Register
	{
		std::cout << "opening Website\n";
		return 2; // normally 2 to open website
	}
	else
		return 0;

}

void SetDebugPrivilege()
{
	HANDLE hProcess = GetCurrentProcess(), hToken;
	TOKEN_PRIVILEGES priv;
	LUID luid;

	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);//fnet unser Prozess token zum "anpassen"(adjust) und speichert ein handle zum verwalten in hToken ab.
	LookupPrivilegeValue(0, "seDebugPrivilege", &luid);
	/*speichert den LUID wert von "seDebugPrivilege" in die LUID-Variable. 0 heiﾟt,
	dass es die LUID von unserem System und nicht etwa die eines remote-systems sein soll*/

	priv.PrivilegeCount = 1;
	priv.Privileges[0].Luid = luid;
	priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;//Da wir das Privileg aktivieren wollen und nicht deaktivieren(was keinen effekt h舩te), setzen wir als Attribut "SE_PRIVILEGE_ENABLED"
	AdjustTokenPrivileges(hToken, false, &priv, 0, 0, 0);/*Parameter 1: wir wollen eine Privilegie in eben DIESEM token anpassen; Parameter 2: Alle anderen Privilegien sollen bleiben, wie sie sind;
														 Parameter 3: Dass ist unser neues Privileg; Parameter 4-6 sind nur relevant, wenn wir noch das "alte" privileg zur・kbekommen und speichern wollen -> wollen wir aber nicht, also NULL
														 /*wer das vielleicht trotzdem mal muss kriegt hier mehr auskunft ・er die funktion: http://msdn.microsoft.com/en-us/library/aa375202%28VS.85%29.aspx     */

	CloseHandle(hToken); //Schlieﾟt das Token wieder -> es m・ste nun wieder mit OpenProcessToken gefnet werden.
	CloseHandle(hProcess);//Schlieﾟt das Prozess-Objekt, ・er dass auf Prozess-Resourcen zugegriffen werden kann wieder.
}