#pragma once

#include <chrono>
#include <conio.h>
#define CUR_TIME std::chrono::system_clock::now()
#define ELAPSED_TIME(x,y) ((std::chrono::duration<double>)(y - x)).count()
#define LOG _cprintf
#define PI	3.141592653589793238462643383279502884197169399375105820974944592308
#define _B 0
#define _G 1
#define _R 2
#define _A 3


enum FLIP
{
	VERTICAL,
	HORIZONTAL,
	BOTH
};

enum TYPE
{
	BMP,
	JPG,
	PNG,
	GIF
};

#ifdef DLL_EXPORT
#define IMAGE_API __declspec(dllexport)
#else
#define IMAGE_API __declspec(dllimport)
#endif
class IMAGE_API MWNAM_Image
{
private:
	static MWNAM_Image *m_pInst;

public:
	static MWNAM_Image* GetInstance() {
		if (m_pInst == nullptr)
			m_pInst = new MWNAM_Image;
		return m_pInst;
	}
	static void DestroyInstance() {
		if (m_pInst) {
			delete m_pInst;
			m_pInst = nullptr;
		}
	}
private:
	char* GetExtension(const char *path);

public:
	MWNAM_Image();
	~MWNAM_Image();

	int CalcBitmapSize(int w, int h, int ch);
	bool GetSize(const char* path, unsigned int *size);
	bool GetResolution(const char* path, int *width, int *height);
	void Resize(unsigned char* src, unsigned char* dst, int w, int h, int neww, int newh, int ch);
	bool Rotate(double rotate, unsigned char *src, unsigned char *dst, int w, int h, int neww, int newh, int ch);
	bool Flip(FLIP mode, unsigned char *src, unsigned char *dst, int w, int h, int ch);
	bool Crop(unsigned char *src, unsigned char *dst, int w, int h, int ch, int x, int y, int neww, int newh);

};

