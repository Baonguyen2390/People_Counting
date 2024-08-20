#include <REGX51.H>
#include <Thu_Vien_LCD.h>

#define R1 P2_0
#define R2 P2_1
#define R3 P2_2
#define R4 P2_3
#define C1 P2_4
#define C2 P2_5
#define C3 P2_6

#define CBVAO P1_0
#define CBRA P1_1
#define BUZZ P1_2
int Input = 0, GioiHan = 50, SoLuong = 0, preGioiHan = 0, preSoLuong = 0, Count = 0, flag = 0; 

void delay(int ms)
{
	int i, j;
	for (i= 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}
void Reset();
void Set();
int Quet_Phim()
{
	int InputPhim = -1;
	R1 = 0;
	if (C1 == 0)
	{
		InputPhim = 1;
		delay(300);
	}
	if (C2 == 0)
	{
		InputPhim = 2;
		delay(300);
	}
	if (C3 == 0)
	{
		InputPhim = 3;
		delay(300);
	}
	R1 = 1;
	
	R2 = 0;
	if (C1 == 0)
	{
		InputPhim = 4;
		delay(300);
	}
	if (C2 == 0)
	{
		InputPhim = 5;
		delay(300);
	}
	if (C3 == 0)
	{
		InputPhim = 6;
		delay(300);
	}
	R2 = 1;
	
	R3 = 0;
	if (C1 == 0)
	{
		InputPhim = 7;
		delay(300);
	}
	if (C2 == 0)
	{
		InputPhim = 8;
		delay(300);
	}
	if (C3 == 0)
	{
		InputPhim = 9;
		delay(300);
	}
	R3 = 1;
	
	R4 = 0;
	if (C1 == 0)
	{
		InputPhim = 100; //Reset
		delay(300);
	}
	if (C2 == 0)
	{
		InputPhim = 0;
		delay(300);
	}
	if (C3 == 0)
	{
		InputPhim = 200; //Set
		delay(300);
	}
	R4 = 1;
	return InputPhim;
}
void SetSoLuong()
{
	LCD_Gotoxy(0,0);
	preSoLuong = SoLuong;
	SoLuong = 0;
	LCD_Puts("SET QA: ");
	delay(300);
	Count = 0;
	while (Count < 4)
	{
		Input = Quet_Phim();
		if (Input == 200 && Count == 0)
		{
			SoLuong = preSoLuong;
			break;
		}
		else if (Input == 200)
			break;
		else if (Input == 100 || Input == -1)
			continue;
		SoLuong = SoLuong*10 + Input;
		LCD_PutNumber(Input);
		Count++;
		delay(300);
	};
}
void SetGioiHan()
{
	LCD_Gotoxy(0,1);
	preGioiHan = GioiHan;
	GioiHan = 0;
	LCD_Puts("SET CA: ");
	delay(300);
	Count = 0;
	while(Count < 4)
	{
		Input = Quet_Phim();
		if (Input == 200 && Count == 0)
		{
			GioiHan = preGioiHan;
			break;
		}
		else if (Input == 200)
			break;
		else if (Input == 100 || Input == -1)
			continue;
		GioiHan = GioiHan*10 + Input;
		LCD_PutNumber(Input);
		Count++;
		delay(300);
	};
}

void Set()
{
	LCD_Clear();
	SetSoLuong();
	SetGioiHan();
	while(GioiHan < SoLuong)
	{
		LCD_Clear();
		LCD_Gotoxy(0,1);
		LCD_Puts("UPDATE FAIL!");
		delay(1000);
		LCD_Clear();
		SetSoLuong();
		SetGioiHan();
	}
	LCD_Clear();
	LCD_Gotoxy(0,1);
	LCD_Puts("UPDATE SUCCESS!");
	delay(1000);
}
void Reset()
{
	SoLuong = 0;
	GioiHan = 50;
}
void HienThi()
{
	LCD_Clear();
	LCD_Gotoxy(0,0); 
	LCD_Puts("QUANTITY: "); 
	LCD_PutNumber(SoLuong);
	LCD_Gotoxy(0,1);
	LCD_Puts("VACANCY : ");
	LCD_PutNumber(GioiHan - SoLuong);
	delay(300);
}
void CamBienTang()
{
	while(1)
	{
		if(CBRA == 0)
		{
			while(CBRA == 0);
			if (SoLuong <= GioiHan)
				SoLuong++;
			if (SoLuong <= GioiHan)
				HienThi();
			break;
		}
	}
}
	
void CamBienGiam()
{
	while(1)
	{
		if(CBVAO == 0)
		{
			while(CBVAO == 0);
			if (SoLuong > 0)
			SoLuong--;
			HienThi();
			break;
		}
	}
}

void main()
{
	BUZZ = 0;
	LCD_Init(); // Khoi tao LCD
	HienThi();
	while(1)
	{
		if (CBVAO == 0)
		{
			if (SoLuong == GioiHan)
			{
				BUZZ = 1;
				LCD_Clear();
				LCD_Gotoxy(0,1);
				LCD_Puts("OVERLOAD!");
				delay(10000);
				BUZZ = 0;
				HienThi();
				continue;
			}
			while(CBVAO == 0);
			CamBienTang();
			continue;
		}
		if (CBRA == 0)
		{
			while(CBRA == 0);
			CamBienGiam();
			continue;
		}
		Input = Quet_Phim();
		if (Input == 100)
		{
			Reset();
			HienThi();
		}
		else if(Input == 200)
		{
			Set();
			HienThi();
		}
	}
}