#include <stdio.h>
#include <windows.h>
#include <time.h>
#define scount 80
#define screen_x 80
#define screen_y 25
HANDLE wHnd;
HANDLE rHnd;
DWORD fdwMode;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
COORD star[scount];

int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}

char cursor(int x, int y) 
{
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

		return '\0';
	else
		return buf[0];

}


int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void draw_ship(int x, int y, int color)
{
	
	consoleBuffer[x + screen_x * y].Char.AsciiChar = 'A';
	consoleBuffer[x + 1 + screen_x * y].Char.AsciiChar = '-';
	consoleBuffer[x + 2 + screen_x * y].Char.AsciiChar = '>';
	consoleBuffer[x - 1 + screen_x * y].Char.AsciiChar = '-';
	consoleBuffer[x - 2 + screen_x * y].Char.AsciiChar = '<';
	consoleBuffer[x + screen_x * y].Attributes = color;
	consoleBuffer[x + 1 + screen_x * y].Attributes = color;
	consoleBuffer[x - 1 + screen_x * y].Attributes = color;
	consoleBuffer[x + 2 + screen_x * y].Attributes = color;
	consoleBuffer[x - 2 + screen_x * y].Attributes = color;

	//characterPos[x + screen_x * y].Char.AsciiChar = 'A';
}

void clear_ship(int x, int y)
{

	consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + 1 + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + 2 + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x - 1 + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x - 2 + screen_x * y].Char.AsciiChar = ' ';
	//consoleBuffer[x + screen_x * y].Attributes = 7;

}

void clear_buffer()
{
	/*for (int y = 0; y < screen_y; ++y)
	{
		for (int x = 0; x < screen_x; ++x)
		{

			consoleBuffer[x + screen_x * y].Char.AsciiChar = 0;
			consoleBuffer[x + screen_x * y].Attributes = 9;
		}
	}*/

	for (int i = 0; i < scount; i++)
	{

		consoleBuffer[star[i].X + screen_x * (star[i].Y - 1)].Char.AsciiChar = ' ';
		consoleBuffer[star[i].X + screen_x * (star[i].Y - 1)].Attributes = 7;




	}

}



void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos, &windowSize);
}



void init_star()
{
	for (int i = 0; i < scount; i++)
	{
		star[i] = { (rand() % screen_x) , (rand() % screen_y) };
	}
}

void star_fall()
{
	//Sleep(200);
	int i;
	for (i = 0; i < scount; i++)
	{
		if (star[i].Y >= screen_y - 1)
		{
			consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = ' ';
			star[i] = { (rand() % screen_x),1 };
		}
		else if ( cursor(star[i].X, star[i].Y+1) == 'A' || cursor(star[i].X + 1 , star[i].Y + 1) == '-' || cursor(star[i].X + 2, star[i].Y + 1) == '>' || cursor(star[i].X - 1, star[i].Y + 1) == '-' || cursor(star[i].X - 2, star[i].Y + 1) == '<')
		{
			consoleBuffer[star[i].X + screen_x * (star[i].Y)].Char.AsciiChar = ' ';
			star[i] = { (rand() % screen_x) , (rand() % screen_y) };
			consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '*';
			consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 5;

		}
		else
		{
			star[i] = { star[i].X,star[i].Y + 1 };
		}
	}


}

void fill_star_to_buffer()
{
	for (int i = 0; i < scount; i++)
	{
		consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '*';
		consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 9;


	}

}



int main()
{
	setcursor(0);
	struct star
	{
		int Y;
		int X;

	}; star star[scount];

	struct ship
	{
		int X;
		int Y;
	};

	int i;
	int color = 7, hit=0;
	int posx, posy, oldposx=1, oldposy=1;
	srand(time(NULL));
	setConsole(screen_x, screen_y);
	init_star();
	i = 0;

	bool play = true;
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	//setConsole(screen_x, screen_y);
	setMode();

	while (play)
	{
		star_fall();
		clear_buffer();
		fill_star_to_buffer();
		fill_buffer_to_console();
		Sleep(200);


		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0)
		{

			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);

			for (DWORD i = 0; i < numEventsRead; ++i)
			{


					if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true )
					{
						if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
						{
							play = false;
						}
					}
	
				 posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
				 posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
				if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					color = rand() % 9;
				}
				/*else if (eventBuffer[i].Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
				{
					printf("right click\n");
				}*/
				if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED)
				{
					
					if (posx != oldposx || posy != oldposy)
					{
						clear_ship(oldposx, oldposy);
					}
					draw_ship(posx, posy, color);
					oldposx = posx;
					oldposy = posy;


				}
				

				//	}
			}
			

			


			delete[] eventBuffer;
		}
		if (cursor(posx, posy-1) == '*' || cursor(posx+1, posy-1) == '*' || cursor(posx + 2, posy-1) == '*' || cursor(posx - 1, posy-1) == '*' || cursor(posx - 2, posy-1) == '*')
		{
			hit += 1;
			if (hit == 10)
			{
				play = false;
			}
		
		}

		


	}


	return 0;
}