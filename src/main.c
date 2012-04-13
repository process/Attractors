#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#include "GL/gl.h"

#include "resource.h"

#define PI 3.14159265

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DrawProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void initGL(HWND hwnd);

HWND hwndWindow;
HWND hwndDraw;

HGLRC hRC;
HDC hDC;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
						LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndCls;
	MSG msg;
	
	hwndWindow = CreateDialog(
		hInstance,
		MAKEINTRESOURCE(MAINWINDOW),
		NULL, DlgProc);
		
	SendDlgItemMessage(hwndWindow, SliderA, TBM_SETRANGE, 1, (6000 << 16));
	SendDlgItemMessage(hwndWindow, SliderB, TBM_SETRANGE, 1, (6000 << 16));
	SendDlgItemMessage(hwndWindow, SliderC, TBM_SETRANGE, 1, (6000 << 16));
	SendDlgItemMessage(hwndWindow, SliderD, TBM_SETRANGE, 1, (6000 << 16));
	
	SendDlgItemMessage(hwndWindow, SliderA, TBM_SETPOS, 1, 3000 + (0.56 * 1000));
	SendDlgItemMessage(hwndWindow, SliderB, TBM_SETPOS, 1, 3000 + (0.53 * 1000));
	SendDlgItemMessage(hwndWindow, SliderC, TBM_SETPOS, 1, 3000 + (-0.274 * 1000));
	SendDlgItemMessage(hwndWindow, SliderD, TBM_SETPOS, 1, 3000 + (0.67 * 1000));

	ZeroMemory(&wndCls, sizeof(WNDCLASSEX));
	wndCls.cbSize = sizeof(WNDCLASSEX);
	wndCls.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndCls.hInstance = hInstance;
	wndCls.lpfnWndProc = DrawProc;
	wndCls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndCls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndCls.hbrBackground = CreateSolidBrush(0xFFFFFF);
	wndCls.lpszClassName = "Winny";
	wndCls.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	RegisterClassEx(&wndCls);
	
	hwndDraw = CreateWindowEx(
		0, "Winny", "GL", WS_CHILD | WS_BORDER,
		270, 10, 500, 500,
		hwndWindow, NULL, hInstance, NULL);
		
	ShowWindow(hwndDraw, 1);
	UpdateWindow(hwndDraw);
	
	initGL(hwndDraw);
	
	SendMessage(hwndWindow, WM_HSCROLL, 0, 0);
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void initGL(HWND hwnd)
{
	hDC = GetDC(hwnd);	

	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		16,0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		16, 0, 0, PFD_MAIN_PLANE,
		0, 0, 0, 0
	};

	unsigned int PixelFormat = ChoosePixelFormat(hDC,&pfd);
	SetPixelFormat(hDC,PixelFormat,&pfd);

	hRC = wglCreateContext(hDC);

	wglMakeCurrent(hDC,hRC);

	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2,2,-2,2,0,1);
	glViewport(0,0,500,500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(255,255,255,255);
	glColor3f(0,0,0);

	glClear(GL_COLOR_BUFFER_BIT);
	SwapBuffers(hDC);
}

void PutPixel(float x, float y)
{
	glVertex2f(x, y);
	glVertex2f(x, y + .008);
	glVertex2f(x + .008, y + .008);
	glVertex2f(x + .008, y);
}

void Draw()
{
	double x;
	double y;
	
	double a = (SendDlgItemMessage(hwndWindow, SliderA, TBM_GETPOS, 0, 0) - 3000) / 1000.0;
	double b = (SendDlgItemMessage(hwndWindow, SliderB, TBM_GETPOS, 0, 0) - 3000) / 1000.0;
	double c = (SendDlgItemMessage(hwndWindow, SliderC, TBM_GETPOS, 0, 0) - 3000) / 1000.0;
	double d = (SendDlgItemMessage(hwndWindow, SliderD, TBM_GETPOS, 0, 0) - 3000) / 1000.0;
	
	char buf[20];
	
	sprintf(buf, "a = %.3f", a);
	SendDlgItemMessage(hwndWindow, TextA, WM_SETTEXT, 0,(int)buf);
	sprintf(buf, "b = %.3f", b);
	SendDlgItemMessage(hwndWindow, TextB, WM_SETTEXT, 0,(int)buf);
	sprintf(buf, "c = %.3f", c);
	SendDlgItemMessage(hwndWindow, TextC, WM_SETTEXT, 0,(int)buf);
	sprintf(buf, "d = %.3f", d);
	SendDlgItemMessage(hwndWindow, TextD, WM_SETTEXT, 0,(int)buf);
	
	double x0, y0;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	
	srand((int)hwndWindow);
	x = (double) rand() / (double) RAND_MAX;
	y = (double) rand() / (double) RAND_MAX;
	
	int i;
	for(i = 0; i < 150000; ++i)
	{
		x0 = x;
		y0 = y;
	
		x = sin(a * PI * y0) - cos(b * PI * x0);
		y = sin(c * PI * x0) - cos(d * PI * y0);
		
		PutPixel(x, y);
	}
	
	glEnd();
	SwapBuffers(hDC);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch(message)
	{
		case WM_DESTROY:
		case WM_CLOSE:
			ReleaseDC(WindowFromDC(hDC), hDC);
			wglDeleteContext(hRC);
			PostQuitMessage(0);
			return 1;
	}
	return 0;
}

LRESULT CALLBACK DrawProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	return DefWindowProc(hwnd, message, wParam, lParam);
}
