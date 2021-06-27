#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include <iostream>
using namespace std;

const double PI = 3.1415926535;

void ellipseScanConvert(int x0, int y0, int Ra, int Rb, double theta);
void ppmWrite(const char* filename, unsigned char* data, int w, int h);
void drawPixelSet(unsigned char* data, int x, int y, int x0, int y0);
void drawSinglePixel(unsigned char* data, int x, int y, int x0, int y0);


//backgournd: width = 400, height = 300

int main() {
	/*unsigned char data[400 * 300 * 3] = { 0, 0,0, 255, 255, 255, 255,255,255 };
	ppmWrite("test.ppm", data, 400, 300);*/
	int x0, y0, Ra, Rb;
	double theta;
	cin >> x0 >> y0 >> Ra >> Rb;
	cout << "Please input the radian value of theta: (ÒÔPI½áÎ²)";
	cin >> theta;
	theta *= PI;

	ellipseScanConvert(x0, y0, Ra, Rb, theta);
	return 0;
}

void ppmWrite(const char* filename, unsigned char* data, int w, int h) {
	FILE* fp;
	fp = fopen(filename, "wb");

	fprintf(fp, "P6\n%d %d\n255\n", w, h);
	fwrite(data, w * h * 3, 1, fp);

	fclose(fp);
}

void ellipseScanConvert(int x0, int y0, int Ra, int Rb, double theta)
{
	double c = sqrt(Ra * Ra - Rb * Rb);
	double xc = c * cos(theta), yc = c * sin(theta);
	double A = Ra * Ra - xc * xc, B = Ra * Ra - yc * yc, C = -xc * yc, F = -Ra * Ra * Rb * Rb;

	int xt = -C / sqrt(A), yt = sqrt(A);
	int xtr = (B - C) / sqrt(A + B - 2 * C), ytr = (A - C) / sqrt(A + B - 2 * C);
	int xr = sqrt(B), yr = -C / sqrt(B);
	int xbr = (B + C) / sqrt(A + B + 2 * C), ybr = -(A + C) / sqrt(A + B + 2 * C);
	int xb = C / sqrt(A), yb = -sqrt(A);

	unsigned char data[400 * 300 * 3] = { 0 };

	int x = xt, y = yt;
	double d = A * (x + 1) * (x + 1) + B * (y - 0.5) * (y - 0.5) + 2 * C * (x + 1) * (y - 0.5) + F;

	drawPixelSet(data, x, y, x0, y0);
	while (x > xtr) {
		if (d < 0) {
			d += 2 * A * x + 2 * C * y + 3 * A - C;
		}
		else {
			d += 2 * (A - C) * x + (3 * C - 2 * B) * y + 3 * A + 2 * B - 5 * C;
			y--;
		}
		x++;
		drawPixelSet(data, x, y, x0, y0);
	}

	d = A * (x + 0.5) * (x + 0.5) + B * (y - 1) * (y - 1) + 2 * C * (x + 0.5) * (y - 1) + F;	//Same as upper
	while (y > yr) {
		if (d < 0) {
			d += 2 * (A - C) * x + 2 * (C - B) * y + 2 * A + 3 * B - 5 * c;
			x++;
		}
		else {
			d += (-2 * C * x - 2 * B * y + 3 * B - C);
		}
		y--;
		drawPixelSet(data, x, y, x0, y0);
	}

	d = A * (x - 0.5) * (x - 0.5) + B * (y - 1) * (y - 1) + 2 * C * (x - 0.5) * (y - 1) + F;	//error
	while (y > ybr) {
		if (d < 0) {
			d += (-2 * C * x - 2 * B * y + 3 * B + C);
		}
		else {
			d += (-2 * (A + C) * x - 2 * (B + C) * y + 2 * A + 3 * B + 5 * C);
			x--;
		}
		y--;
		drawPixelSet(data, x, y, x0, y0);
	}

	d = A * (x - 1) * (x - 1) + B * (y - 0.5) * (y - 0.5) + 2 * C * (x - 1) * (y - 0.5) + F;
	while (x > xb) {
		if (d < 0) {
			d += (-2 * (A + C) * x - 2 * (B + C) * y + 3 * A + 2 * B + 5 * C);
			y--;
		}
		else {
			d += (-2 * A * x - 2 * C * y + 3 * A + C);
		}
		x--;
		drawPixelSet(data, x, y, x0, y0);
	}

	ppmWrite("ellipse.ppm", data, 400, 300);
}

void drawPixelSet(unsigned char* data, int x, int y, int x0, int y0)
{
	drawSinglePixel(data, x, y, x0, y0);
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


