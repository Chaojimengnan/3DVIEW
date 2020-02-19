#include "pch.h"
#include "space.h"
#include <iostream>
#include <graphics.h>
#include <Windows.h>
#include <conio.h>


Space_Creator *newspace;
Camera *mycamera;
tObject *cube1;
tObject *cube2;
void start()
{
	initgraph(PMX, PMY);
	newspace = new Space_Creator();
	mycamera = new Camera(1, PMX/2);
	cube1 = new tObject(1, 4, 0, 0);
	cube2 = new tObject(1, -6, 0, 0);
	newspace->add_object(*cube1);
	newspace->add_object(*cube2);
}
DWORD WINAPI display(LPVOID lpParamter)
{
	float pi = 0;
	while (1)
	{
		pi += 0.01;
		cube1->move(sin(pi) / 10, sin(pi)/10, 0, 1);
		cube1->rotate(1, 0.01);
		cube1->rotate(2, 0.01);
		cube1->rotate(3, 0.01);
		cube2->move(sin(pi) / 10, cos(pi) / 10, 0, 1);
		cube2->rotate(1, 0.01);
		cube2->rotate(2, 0.01);
		cube2->rotate(3, 0.01);
		mycamera->draw(newspace->theObjects);
		Sleep(10);
	}
}

DWORD WINAPI control(LPVOID lpParamter)
{
	char a;
	while (1)
	{
		a = _getch();
		switch (a)
		{
		case 'w':
			mycamera->move(1, 0, 0, 2);
			break;
		case 's':
			mycamera->move(-1, 0, 0, 2);
			break;
		case 'a':
			mycamera->move(0, -1, 0, 2);
			break;
		case 'd':
			mycamera->move(0, 1, 0, 2);
			break;
		}
	}
}


int main()
{
	start();
	HANDLE hThread1 = CreateThread(NULL, 0, display, NULL, 0, NULL);
	CloseHandle(hThread1);
	HANDLE hThread2 = CreateThread(NULL, 0, control, NULL, 0, NULL);
	CloseHandle(hThread2);
	MOUSEMSG m;
	int msg_x=PMX/2;
	int msg_y=PMY/2;

	while (true)
	{
		m = GetMouseMsg();

		switch (m.uMsg)
		{
		case WM_MOUSEMOVE:
			mycamera->rotate(1, (m.x - msg_x)*0.01);
			mycamera->rotate(2, -(m.y - msg_y)*0.01);
			msg_x = m.x;
			msg_y = m.y;
			break;

		case WM_RBUTTONUP:
			return 0;
		}
	}

	closegraph();
	return 0;
}
