#include<iostream>
#include <cstdlib>
#include "temperature.h"
using namespace std;

#define lc(i) i*2+1
#define rc(i) i*2+2
#define parent(i) ((i-1)>>1)
#define isLc(i) (i-1)%2==0?1:0
#define isRc(i) (i-1)%2==1?1:0

template<typename T>
class vector {
private:
	int size;
	int capacity;
	T* element;
public:
	vector(int c) {
		element = new T[c];
		capacity = c;
		size = c;
	}
	vector() {
		element = new T[3];
		capacity = 3;
		size = 0;
	}
	void resize(int n) {
		element = new T[n];
		capacity = n;
		size = n;
	}
	int getSize() { return size; }
	T& operator[] (int r) { return element[r]; }
};

vector<int> merge(vector<int> a, vector<int> b) {//num表示几个叶子的合并
	vector<int> n(a.getSize() + b.getSize());
	for (int i = 0; i < a.getSize(); i++) {
		n[i] = a[i];
		n[i + a.getSize()] = b[i];
	}
	return n;
}
void mergeLayer(vector<vector<int>>&y, int start, int end) {
	for (int i = start; i <= end; i++) {
		y[i] = merge(y[lc(i)], y[rc(i)]);
	}
}

int yrange(vector<int>y, int y1, int y2, int m) {
	int sum = 0;
	int lo = 0, hi = y.getSize();
	while (lo < hi) {
		int mi = (lo + hi) >> 1;
		y2 < mi ? hi = mi : lo = mi + 1;
	}
	int start = y1, end = --lo;
	int n = y.getSize() / m;      //merge了几个叶子
	for (int i = end; i >= start; i--) {
		for (int k = 0; k < n; k++) {
			sum += y[i + k * m];
		}
	}
	return sum;
}

//构造一棵树
vector<int> tree;
//构造一个同步的vector
vector<vector<int>> y;

void init(int n, int m, int**a) {
	int fullsize = 0;
	if ((n&(n - 1)) != 0) {
		int count = 0;
		int ntemp = n;
		while (ntemp >>= 1) count++;
		count++;
		fullsize = 1;
		while (count--) fullsize <<= 1;
	}
	else fullsize = n;
	//构造一棵树
	tree.resize(2 * fullsize - 1);      //后n个元素，前n-1个元素
										//构造一个同步的vector
	y.resize(2 * fullsize - 1);
	for (int i = fullsize - 1, j = 0; i < 2 * fullsize - 1; i++, j++) {
		//填充叶子节点
		tree[i] = j;
		y[i].resize(m);
		for (int k = 0; k < m; k++) y[i][k] = (j < n) ? a[j + 1][k + 1] : 0;
		//填充树结构的前x-1个节点
		int temp = i;
		while (isRc(temp) && parent(temp) > 0) temp = parent(temp);
		tree[parent(temp)] = j;
	}
	tree[tree.getSize() - 1] = fullsize - 1;    //最后一个单独写入
	y[2 * fullsize - 2].resize(m);
	for (int k = 0; k < m; k++) y[2 * fullsize - 2][k] = (fullsize - 1 < n) ? a[fullsize][k + 1] : 0;    

	int start = fullsize - 1;
	int end = tree.getSize() - 1;
	while (parent(start) != parent(end)) {     //parent(start)==parent(end)时到达root
		mergeLayer(y, parent(start), parent(end));
		start = parent(start);
		end = parent(end);
	}
	y[0] = merge(y[1], y[2]);   //root单独提出

}

int query(int x1, int y1, int x2, int y2) {
	x1--; x2--; y1--; y2--;
	//初步深入搜索
	int i = 0;
	int lca = 0;
	int sum = 0;
	int num = ((x2 - x1) + 1)*((y2 - y1) + 1);
	int average = 0;
	//找到lca
	while (rc(i) <= tree.getSize()) {
		if ((x1 <= tree[i]) && (x2 <= tree[i])) {
			i = lc(i);
		}
		else if ((x1 > tree[i]) && (x2 > tree[i])) i = rc(i);
		else {
			break;
		}
	}
	lca = i;
	int report[100]{ 0 };    //记录需要一篮子提出的index
	int index = 0;
	if (rc(lca) > tree.getSize()) {      //x1=x2的特殊情况
		report[index++] = lca;
	}
	else {
		int a1 = lc(lca);
		int a2 = rc(lca);

		while (a1 < tree.getSize() && a2 < tree.getSize()) {
			if (x1 <= tree[a1]) {
				report[index++] = (rc(a1) < tree.getSize()) ? rc(a1) : a1;
				a1 = lc(a1);
			}
			else a1 = rc(a1);

			if (x2 <= tree[a2]) {
				if (x2 == tree[a2] && lc(a2)>tree.getSize()) report[index++] = a2;    //最后一步的判断
				a2 = lc(a2);
			}
			else {
				report[index++] = (lc(a2) < tree.getSize()) ? lc(a2) : a2;
				a2 = rc(a2);
			}
		}
	}

	//对y进行检索
	for (int j = 0; j < index; j++) {
		sum += yrange(y[report[j]], y1, y2, y[y.getSize() - 1].getSize());
		average = int(sum / num);
	}
	return average;
}

void change(int x1, int y1, int temp) {
	x1--; y1--;
	int leaf = (y.getSize() + 1) / 2 - 1 + x1;        //leaf从这里开始
	y[leaf][y1] = temp;
	//更新其祖先中存的数据
	while (parent(leaf) >= 0) {
		if (isLc(leaf)) y[parent(leaf)][y1] = temp;
		else y[parent(leaf)][y1 += y[leaf].getSize()] = temp;
		leaf = parent(leaf);
	}
}