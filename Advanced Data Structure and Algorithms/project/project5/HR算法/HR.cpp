#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <time.h>
using namespace std;


clock_t start_time;
clock_t end_time;


/*******************************
*类定义
********************************
*
*Rectangle：矩形类
*	
*UnboundArea：无边界区域类
*
*BoundArea：有边界区域类
*
*******************************/

//声明
class Rectangle {
public:
	Rectangle() :height(0), width(0), area(0), x(0), y(0) {}
	Rectangle(int h, int w) {
		int min = h < w ? h : w;
		int max = h > w ? h : w;
		height = min;
		width = max;
		area = height * width;
		x = 0;
		y = 0;
	}
	void SetPosition(int x, int y) {
		this->x = x;
		this->y = y;
	}
	int height;
	int width;
	int area;
	int x;
	int y;
};

class BoundArea;
class UnboundArea {
public:
	UnboundArea() :x(0), y(0), width(0) {}
	~UnboundArea() {}

	void Set(int x, int y, int w);

	void Pack(Rectangle& item, UnboundArea& S1, BoundArea& S2);

	int x;
	int y;
	int width;
};

class BoundArea {
public:
	//functions
	BoundArea() :x(0), y(0), height(0), width(0), area(0) {}
	~BoundArea() {}

	void Set(int x, int y, int h, int w);

	bool CanPack(Rectangle& item);

	void Pack(Rectangle& item, BoundArea& S3, BoundArea& S4);


	//varibles
	int x;
	int y;
	int height;
	int width;
	int area;
};

//定义
void UnboundArea::Set(int x, int y, int w) {
	this->x = x;
	this->y = y;
	this->width = w;
}

void UnboundArea::Pack(Rectangle& item, UnboundArea& S1, BoundArea& S2) {
	//Pack item
	item.SetPosition(x, y);

	//Divided into S1 and S2
	S1.Set(x, y + item.height, width);
	S2.Set(x + item.width, y, item.height, width - item.width);
}

void BoundArea::Set(int x, int y, int h, int w) {
	this->x = x;
	this->y = y;
	this->height = h;
	this->width = w;
	this->area = h * w;
}

bool BoundArea::CanPack(Rectangle& item) {
	int h = item.height;
	int w = item.width;
	if (h <= height && w <= width) {
		return true;
	}
	if (w <= height && h <= width) {
		item.height = w;
		item.width = h;
		return true;
	}
	return false;
}

void BoundArea::Pack(Rectangle& item, BoundArea& S3, BoundArea& S4) {
	//Pack item
	item.SetPosition(x, y);

	//Divide
	S3.Set(x, y + item.height, height - item.height, item.width);
	S4.Set(x + item.width, y, height, width - item.width);
}




/******************************
*一些工具函数
*******************************
*
*myCompare: 用于sort的自定义比较函数
*
*Swap: 用于交换两个矩形
*
*******************************/
bool myCompare(Rectangle& a, Rectangle& b) {
	return a.area < b.area;
}

void Swap(Rectangle& a, Rectangle& b) {
	Rectangle temp;
	temp = a;
	a = b;
	b = temp;
}




/******************************
*主要的功能函数
*******************************
*
*RecursivePacking:用于递归Pack有边界空间
*
*Packing:用于Pack无边界空间
*
*******************************/
void RecursivePacking(vector<Rectangle>& unpacked_recs, BoundArea& S2, vector<Rectangle>& packed_recs) {
	int can = -1;
	for (int i = 0; i < unpacked_recs.size(); i++) {
		if (S2.CanPack(unpacked_recs[i]))
			can = i;
		if (S2.area < unpacked_recs[i].area)
			break;
	}
	if (can == -1)
		return;
	BoundArea S3, S4;
	S2.Pack(unpacked_recs[can], S3, S4);
	packed_recs.push_back(unpacked_recs[can]);
	unpacked_recs.erase(unpacked_recs.begin() + can);
	if (S3.area > S4.area) {
		RecursivePacking(unpacked_recs, S3, packed_recs);
		RecursivePacking(unpacked_recs, S4, packed_recs);
	}
	else {
		RecursivePacking(unpacked_recs, S4, packed_recs);
		RecursivePacking(unpacked_recs, S3, packed_recs);
	}
}

void Packing(vector<Rectangle>& unpacked_recs, UnboundArea& S, vector<Rectangle>& packed_recs) {

	while (unpacked_recs.size() != 0) {
		UnboundArea S1;
		BoundArea S2;
		S.Pack(unpacked_recs[unpacked_recs.size() - 1], S1, S2);
		packed_recs.push_back(unpacked_recs[unpacked_recs.size() - 1]);
		unpacked_recs.pop_back();
		S = S1;
		RecursivePacking(unpacked_recs, S2, packed_recs);
	}
}

/******************************
*主函数
*******************************
*
*输入：以h w的格式输入待装箱矩形的高和宽，初始化矩形并放入unpacked_recs数组中
*
*排序：按矩形面积对unpacked_recs中的元素由小到大排序
*
*装箱：调用Packing函数对初始化的无边界区域S和待装箱矩形unpacked_recs装箱
*
*输出1：可选择将装好的矩形的位置信息和大小信息以x y h w的格式输出到out.txt文件中
*输出2：也可选择在控制台输出装箱后的高度
*
*/

int main() {
	int N;
	cin >> N;
	int area_width;
	cin >> area_width;

	//输入
	vector<Rectangle> unpacked_recs, packed_recs;
	int h, w;
	for (int i = 0; i < N; i++) {
		cin >> h >> w;
		unpacked_recs.push_back(Rectangle(h, w));
	}
	UnboundArea S;
	S.Set(0, 0, area_width);


	start_time = clock();

	//按面积排序
	sort(unpacked_recs.begin(), unpacked_recs.end(), myCompare);


	//装箱
	Packing(unpacked_recs, S, packed_recs);


	end_time = clock();
	cout << "The Time: " << (float)(end_time - start_time) / CLOCKS_PER_SEC << endl;
	

	/********输出位置信息到文件********
	*ofstream outfile(".\\out.txt");
	*for (int i = 0; i < N; i++) {
	*	outfile << packed_recs[i].x << " " << packed_recs[i].y << " " << packed_recs[i].height << " " << packed_recs[i].width << endl;
	*}
	*outfile.close();
	********************************/

	//输出最小高度到控制台
	int height = 0;
	for (int i = 0; i < N; i++) {
		if (packed_recs[i].y + packed_recs[i].height > height)
			height = packed_recs[i].y + packed_recs[i].height;
	}
	cout << "The packed height: " << height << endl;


	system("pause");
}