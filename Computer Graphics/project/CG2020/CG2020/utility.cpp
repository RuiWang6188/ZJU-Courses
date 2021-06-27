#define _CRT_SECURE_NO_WARNINGS
#define FREEGLUT_STATIC

#include"utility.h"

using namespace std;

static GLint    imageWidth;  //BMP图像宽
static GLint    imageHeight; //BMP图像高
static GLint    pixelLength;  //图像数据长度
GLubyte* pixelData_INTRO;
GLubyte* pixelData_HELP;


void display_string(int x, int y, const char* string, int font)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		if (font == 1)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
		if (font == 2)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
		if (font == 3)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
		if (font == 4)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
	}

}


void ReadBmpFile(const char* fileName, GLubyte*& pixelData)
{
	// 打开文件
	FILE* pFile = fopen(fileName, "rb");
	if (pFile == 0) {
		cout << "Open File Error!" << endl;
		exit(0);
	}
	else
		cout << "Open File Success:" << fileName << endl;

	// 读取图象的宽高信息
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&imageWidth, sizeof(imageWidth), 1, pFile);
	fread(&imageHeight, sizeof(imageHeight), 1, pFile);

	// 计算像素数据长度，不足4位补齐4位
	pixelLength = imageWidth * 3;
	while (pixelLength % 4 != 0)
		++pixelLength;
	pixelLength *= imageHeight;

	// 读取像素数据
	pixelData = (GLubyte*)malloc(pixelLength);
	if (!pixelData) {
		cout << "pixel malloc error" << endl;
		exit(0);
	}

	fseek(pFile, 54, SEEK_SET);  //跳过文件头和信息头
	fread(pixelData, pixelLength, 1, pFile);

	// 关闭文件
	fclose(pFile);
}