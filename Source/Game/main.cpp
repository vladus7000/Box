#include <stdio.h>
#include <System/Engine.hpp>

int main(int argc, char** argv)
{
	{
		(void)argc;
		(void)argv;

		box::Engine();
		printf("The game is started\n");
		printf("The game is ended\n");
	}
	
}