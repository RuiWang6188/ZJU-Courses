#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

void ellipseScanConvert(int x0, int y0, int Ra, int Rb);
void ppmWrite(const char* filename, unsigned char* data, int w, int h);
void drawPixelSet(unsigned char* data, int x, int y, int x0, int y0);
void drawSinglePixel(unsigned char* data, int x, int y, int x0, int y0);


//backgournd: width = 400, height = 300

int main() {
	int x0, y0, Ra, Rb;
	cin >> x0 >> y0 >> Ra >> Rb;

	ellipseScanConvert(x0, y0, Ra, Rb);
	return 0;
}

void ppmWrite(const char* filename, unsigned char* data, int w, int h) {
	FILE* fp;
	fp = fopen(filename, "wb");

	fprintf(fp, "P6\n%d %d\n255\n", w, h);
	fwrite(data, w * h * 3, 1, fp);

	fclose(fp);
}

void ellipseScanConvert(int x0, int y0, int Ra, int Rb)
{
	if (x0 - Ra < 0 || y0 - Rb < 0) {
		cout << "Invalid input: causing clipping!" << endl;
		return;
	}

	unsigned char data[400 * 300 * 3] = { 0 };

	int sqrRa = Ra * Ra, sqrRb = Rb * Rb;
	int x = 0, y = Rb;
	double d = Rb * Rb + Ra * Ra * 0.25 - Ra * Ra * Rb;

	drawPixelSet(data, x, y, x0, y0);
	while (Rb * Rb * (x + 1) < Ra * Ra * (y - 0.5)) {
		if (d >= 0) {
			d += Rb * Rb * (3 + 2 * x) + Ra * Ra * (2 - 2 * y);
			y--;
		}
		else
			d += Rb * Rb * (3 + x << 1);
		
		x++;
		drawPixelSet(data, x, y, x0, y0);
	}

	d = Ra * Ra * (y - 1) * (y - 1) + Rb * Rb * (x + 0.5) * (x + 0.5) - Ra * Ra * Rb * Rb;
	while (y >= 0) {
		if (d >= 0) {
			d += Ra * Ra * (3 - 2 * y);
		}
		else {
			d += Ra * Ra * (3 - 2 * y) + 2 * Rb * Rb * (x + 1);
			x++;
		}
		y--;
		drawPixelSet(data, x, y, x0, y0);
	}

	ppmWrite("ellipse.ppm", data, 400, 300);
}

void drawPixelSet(unsigned char* data, int x, int y, int x0, int y0)
{
	drawSinglePixel(data, x, y, x0, y0);
	drawSinglePixel(data, x, -y, x0, y0);
	drawSinglePixel(data, -x, y, x0, y0);
	drawSinglePixel(data, -x, -y, x0, y0);
}

void drawSinglePixel(unsigned char* data, int x, int y, int x0, int y0)
{	
	int w = 400, h = 300;
	int finalX = x + x0, finalY = y + y0;

	int position = (finalY * w + finalX) * 3;
	for (int i = 0; i < 3; i++)
		data[position + i] = 255;
}


