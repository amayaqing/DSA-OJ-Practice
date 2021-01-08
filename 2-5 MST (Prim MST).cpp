#include<iostream>
#include<cstdio>
using namespace std;

#define INT_MAX 2147483647
//read方法实现IO优化
inline int read() {
	int x = 0, f = 1; 
	char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') f = -1; c = getchar(); }
	while (c >= '0' && c <= '9') { x = x * 10 + c - '0'; c = getchar(); }
	return x * f;
}
#define in(a) a=read()

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
	T& operator[] (int r) { return element[r]; }
};

typedef enum { UNDISCOVERED, VISITED }vstatus;
struct vertex {
	int data;      //实际记录的是在v中的index
	vstatus status;
	int priority;
	int parent;
	vertex(int d) {
		data = d;
		status = UNDISCOVERED;
		priority = INT_MAX;
	}
};


int prim(int a[500][500], int n) {
	vector<vertex*> v(n);
	//初始化
	for (int i = 0; i < n; i++) {
		v[i] = new vertex(i);
		v[i]->status = UNDISCOVERED;
		v[i]->priority = INT_MAX;
		v[i]->parent = -1;
	}
	int s = 0;     //从0开始，s记录当前节点
	int sum = 0;
	v[0]->priority = 0;
	v[0]->parent = -1;

	for (int k = 0; k < n; k++) {
		v[s]->status = VISITED;
		if (-1 != v[s]->parent) {
			sum += a[v[s]->parent][s];
		}
		//更新优先级
		for (int i = 0; i<n; i++) {
			if ((i != s) && (v[i]->status == UNDISCOVERED) && (v[i]->priority) > a[s][i]) {
				v[i]->priority = a[s][i];
				v[i]->parent = v[s]->data;
			}
		}
		//找到权重最小的边
		int shortest = INT_MAX;
		for (int w = 0; w < n; w++) {
			if ((v[w]->status == UNDISCOVERED) && (v[w]->priority < shortest)) {
				shortest = v[w]->priority;
				s = w;
			}
		}
	}
	return sum;
}


int main()
{
	int n;
	int a[500][500];
	in(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) in(a[i][j]);
	}
	int sum = prim(a, n);
	printf("%d\n", sum);

	return 0;
}
