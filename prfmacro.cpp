#include <iostream>
#include <Windows.h>
#include <vector>


int BoxSizeX = 895;
int BoxSizeY = 765;

int main()
{
	HDC ScreenHdc = GetDC(NULL);

	std::cout << "Press K to Get the CenterPixel G!\n";

	while (!GetAsyncKeyState('K'))
		Sleep(100);

	const COLORREF CenterPixelG = GetPixel(ScreenHdc, 960, 540);
	int CenterRedG = GetRValue(CenterPixelG);
	int CenterBlueG = GetBValue(CenterPixelG);
	int CenterGreenG = GetGValue(CenterPixelG);



	std::cout << "Press L to Get the Second CenterPixel!\n";

	while (!GetAsyncKeyState('L'))
		Sleep(100);

	COLORREF CenterPixelR = GetPixel(ScreenHdc, 960, 540);

	int CenterRedR = GetRValue(CenterPixelR);
	int CenterBlueR = GetBValue(CenterPixelR);
	int CenterGreenR = GetGValue(CenterPixelR);



	bool Enabled = false;

	while (true)
	{
		bool Found = false;

		if (GetAsyncKeyState('2')) //Put your Shotgun Slot KeyBind Here
			Enabled = true;

		else if (GetAsyncKeyState('1') ||
			     GetAsyncKeyState('3') ||
			     GetAsyncKeyState('4') ||
			     GetAsyncKeyState('5')) //Put your Slots KeyBinds Here
			Enabled = false;

		if (Enabled == false)
			continue;

		if (!GetAsyncKeyState('X'))
			continue;



		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);


		int XUpperLeftCorner = (screenWidth - BoxSizeX) / 2;
		int YUpperLeftCorner = (screenHeight - BoxSizeY) / 2;

		HDC Screenhdc = GetDC(NULL);
		HDC Capturehdc = CreateCompatibleDC(Screenhdc);

		HBITMAP CaptureBitMap = CreateCompatibleBitmap(Screenhdc, BoxSizeX, BoxSizeY);

		SelectObject(Capturehdc, CaptureBitMap);

		BitBlt(Capturehdc, 0, 0, BoxSizeX, BoxSizeY, Screenhdc, XUpperLeftCorner, YUpperLeftCorner, SRCCOPY);



		BITMAPINFOHEADER bi;
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = BoxSizeX;
		bi.biHeight = BoxSizeY;
		bi.biPlanes = 1;
		bi.biBitCount = 32;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		std::vector<RGBQUAD> Pixels(BoxSizeX * BoxSizeY);

		GetDIBits(Capturehdc, CaptureBitMap, 0, BoxSizeY, Pixels.data(), (BITMAPINFO*)&bi, DIB_RGB_COLORS);


		for (int Size = 0; Size < BoxSizeX * BoxSizeY; Size += 40)
		{

			if (!GetAsyncKeyState('X'))
				break;

			int Red = Pixels[Size].rgbRed;
			int Green = Pixels[Size].rgbGreen;
			int Blue = Pixels[Size].rgbBlue;

			if (Red >= CenterRedG && Red <= CenterRedG + 6  //Green
				&& Green >= CenterGreenG && Green <= CenterGreenG + 10
				&& Blue >= CenterBlueG && Blue <= CenterBlueG + 5 
				|| Red >= CenterRedR && Red <= CenterRedR + 5 //Red
				&& Green >= CenterGreenR && Green <= CenterGreenR + 6
				&& Blue >= CenterBlueR && Blue <= CenterBlueR + 4
				)
			{
				Found = true;
			}
		}

		ReleaseDC(NULL, Screenhdc);
		if (GetAsyncKeyState('X') && Found == false && Enabled == true)
		{

			INPUT LeftMouseClick{};

			LeftMouseClick.type = INPUT_MOUSE;


			LeftMouseClick.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			UINT SendDown = SendInput(1, &LeftMouseClick, sizeof(INPUT));


			LeftMouseClick.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			UINT SendUp = SendInput(1, &LeftMouseClick, sizeof(INPUT));

		}
		ReleaseDC(NULL, Screenhdc);
		DeleteDC(Capturehdc);
		DeleteObject(CaptureBitMap);
	}
}
