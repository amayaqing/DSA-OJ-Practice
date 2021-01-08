#include<iostream>
#include<cstdio>
using namespace std;
#define lc(x) ((x*2)+1)
#define rc(x) ((x*2)+2)

inline long read() {
	long x = 0, f = 1; char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') f = -1; c = getchar(); }
	while (c >= '0' && c <= '9') { x = x * 10 + c - '0'; c = getchar(); }
	return x * f;
}
#define in(a) a=read()

int exp(int n) {
	int m = 1;
	for (int i = 0; i < n; i++) m *= 10;
	return m;
}

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
	void resize(int n) {size = n;}
	void expand() {
		//��expand�ڲ�����飬���Ƿ���Ҫ����
		if (size <= capacity) return;
		T*temp = element;
		element = new T[capacity *= 2]; 
		for (int i = 0; i < capacity; i++) element[i] = temp[i];
		delete[]temp;
	}
	bool shrink() {
		//��expand�ڲ�����飬���Ƿ���Ҫ����
		if (size * 2 > capacity) return false;
		T*temp = element;
		element = new T[capacity /= 2];  
		for (int i = 0; i < capacity; i++) element[i] = temp[i];
		delete[]temp;
		return true;
	}
	void insert(T num) {
		size++;
		expand();
		element[size - 1] = num;
	}

	int getSize() { return size; }
	T& operator[] (int r) { return element[r]; }
	void operator()(int n) {
		for (int i = 0; i < size; i++) element[i] = n;
	}

	bool operator>(vector<T> b) {
		if (size > b.getSize()) return true;
		else if (size < b.getSize()) return false;
		else {
			for (int i = size - 1; i >= 0; i--) {
				if (element[i] > b[i]) return true;
				else if (element[i] < b[i]) return false;
			}
			return false;        //�˴�ӦΪ��ȫ��ȵ����
		}
	}
	void show() {
		cout << element[size - 1];     //���żǣ����������ǵ��Ŵ���
		if (element[size - 1] != 0 && size - 2 >= 0) {
			for (int k = size - 2; k >= 0; k--) {
				int m = 5;
				while (m >= 0 && element[k] / exp(m) == 0) {
					printf("%d", 0);
					//cout << "0";
					m--;
				}
				if (m >= 0) printf("%d", element[k]);
			}
		}
		printf("\n");
	}
};

class heap {
private:
	int size;
	vector<long> items[2000];
public:
	heap() { size = 0; }
	void insert(vector<long> key);
	vector<long> removeMin();
	void show();
	int getSize() { return size; }
};

void heap::insert(vector<long> key) {
	items[size] = key;
	//�Խṹ���е���
	int i = size;
	while (items[(i - 1) / 2] > items[i] && i != 0) {
		swap(items[i], items[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
	size++;
}

vector<long> heap::removeMin() {
	vector<long> value = items[0];
	swap(items[0], items[size - 1]);
	size--;
	int hole = 0;
	int child = 0;
	//�Խṹ���е���
	while (lc(hole) < size) {
		if (rc(hole) < size && items[lc(hole)]>items[rc(hole)]) child = rc(hole);
		else child = lc(hole);
		if (items[hole] > items[child]) {
			swap(items[hole], items[child]);
			hole = child;
		}
		else break;
	}
	return value;
}


vector<long> mul(vector<long>&a, vector<long>&b) {
	int len[3];
	len[0] = a.getSize();
	len[1] = b.getSize();
	len[2] = 0;
	vector<long> re(100000);
	re(0);     //��ʼ��
	for (int i = 0; i < len[1]; i++) {
		for (int j = 0; j < len[0]; j++) {
			long long temp = a[j] * b[i];
			re[i + j] += temp;        //�������
		}
	}
	//��λ
	int k = 0;
	for (k; k < len[0] + len[1]; k++) {
		if (re[k] / exp(6) > 0) {
			re[k + 1] += re[k] / exp(6);
			re[k] %= exp(6);
		}
	}
	k++;
	for (k; re[k] / exp(6) > 0; k++) {
		re[k + 1] += re[k] / exp(6);
		re[k] %= exp(6);
	}
	//ȥ����λ��1�����
	while (re[k] == 0 && k>0) k--;
	len[2] = k + 1;
	//resize,shrink
	re.resize(len[2]);
	while (re.shrink());

	return re;
}

int main() {
	int n;
	long num[2000];
	heap h;

	cin >> n;
	for (int i = 0; i < n; i++) {
		//���벢ת��Ϊ10^6����
		in(num[i]);
		vector<long> a;
		int j = 0;
		while (num[i] > 0) {
			a.insert(num[i] % exp(6));
			num[i] /= exp(6);
		}
		if (a.getSize() == 0) a.insert(0);     //�ر��ж�0�����

       //����heap
		h.insert(a);
	}

	//�˷�
	vector<long> result; result.insert(1);  
	while (h.getSize() > 1) {
		vector<long> x1 = h.removeMin();
		vector<long> x2 = h.removeMin();
		vector<long> temp = mul(x1, x2);
		h.insert(temp);
		result = mul(result, temp);       //result*=x1*x2;
	}

	//show
	result.show();
	return 0;
}
