#include <iostream>
#include <vector>
#include<time.h>
using namespace std;

clock_t start_time;
clock_t end_time;

int partion(vector<vector<int>>& rec, int left, int right) {
	vector<int> pivot = rec[left];
	while (left < right) {
		while (left < right && rec[right][0] <= pivot[0])
			--right;
		rec[left] = rec[right];
		while (left < right && rec[left][0] >= pivot[0])
			++left;
		rec[right] = rec[left];
	}
	rec[left] = pivot;
	return left;
}

void QuickSort(vector<vector<int>>& rec, int left, int right) {
	if (left >= right)
		return;
	int pivot = partion(rec, left, right);
	QuickSort(rec, left, pivot - 1);
	QuickSort(rec, pivot + 1, right);
}
int TexturePacking(vector<vector<int>>& rec, int width, vector<vector<int>>& recpos) {
	int N = rec.size();
	QuickSort(rec, 0, N - 1);
	int height = 0;
	vector<vector<int>> pack;
	for (int i = 0; i < N; i++) {
		int j;
		for (j = 0; j < pack.size(); j++) {
			if (pack[j][1] + rec[i][1] <= width)
				break;
		}
		if (j < pack.size()) {
			recpos[i][0] = pack[j][0];
			recpos[i][1] = pack[j][1];
			pack[j][1] += rec[i][1];
		}
		else {
			pack.push_back(vector<int>{height, rec[i][1]});
			height += rec[i][0];
			recpos[i][0] = pack[j][0];
			recpos[i][1] = 0;
		}
	}
	return height;
}

int main() {
	int N;
	cin >> N;
	int width;
	cin >> width;

	vector<vector<int>> rectangles(N, vector<int>(2, 0));
	vector<vector<int>> recpos(N, vector<int>(2, 0));

	for (int i = 0; i < N; i++) {
		cin >> rectangles[i][0] >> rectangles[i][1];
	}

	start_time = clock();
	int min_height;
	min_height = TexturePacking(rectangles, width, recpos);

	end_time = clock();
	cout << "The Time: " << (float)(end_time - start_time) / CLOCKS_PER_SEC << endl;

	cout << min_height;

	system("pause");
}