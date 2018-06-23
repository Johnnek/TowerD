#include <iostream>
#include <Windows.h>
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

int main() 
{
	////
	return 0;
}