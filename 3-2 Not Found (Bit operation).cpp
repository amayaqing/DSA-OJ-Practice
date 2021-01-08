#include<iostream>
#include<string>
using namespace std;

string s;

//将unsigned int以二进制形式输出
void printbinary(unsigned int x, int len) {
	for (int i = len - 1; i >= 0; i--) {
		if (x & (1 << i)) cout << "1";
		else cout << "0";
	}
	cout << endl;
}
//将string化为数字形式
unsigned int stringToBin(string str) {
	int t = 1;
	unsigned int x = 0;
	for (int i = str.length() - 1; i >= 0; i--) {
		if (str[i] - 48) x += t;
		t <<= 1;
	}
	return x;
}

//针对所存的串
unsigned int lastN(unsigned int a, int k) {
	unsigned int temp = 1;
	for (int i = 0; i < k - 1; i++) {
		temp = (temp << 1) + 1;
	}
	return a & temp;
}

//计算一个数的log值
inline int log(int len) {
	int n = 0;
	while (len >>= 1) n++;
	n++;
	return n;
}

//借助另一个串来记录每一个出现的值
bool* getBit(int k) {
	bool*b = new bool[1 << k]{ 0 };
	for (int i = 0; i < s.length() - k + 1; i++) {
		b[stringToBin(s.substr(i, k))] = 1;
	}
	return b;
}
//由b[l]得到b[l-1]串
bool* transferBit(bool*b, int k) {
	bool* c = new bool[1 << (k - 1)]{ 0 };
	for (int i = 0; i < (1 << k); i++) {
		if (b[i] == 1) {
			c[i >> 1] = 1;
			c[lastN(i, k - 1)] = 1;
		}
	}
	return c;
}


int main() {
	cin >> s;
	unsigned int x = stringToBin(s);
	int len = s.length();
	int k = log(len);

	//一次遍历得到最大的k的记录串bit
	bool* b = getBit(k);
	//循环得到其余的
	for (k; k > 1; k--) {
		bool* c = transferBit(b, k);

		//若得到全为1的串，则可以及时退出
		bool exit = 1;
		for (int i = 0; i < (1 << (k - 1)); i++) {
			if (c[i] == 0) exit = 0;
		}
		if (exit) break;

		//不退出的情况，用c替换b
		else b = c;
	}
	
	unsigned int ans = 0;
	//找到最后得到的bit串中最小的为0的地方，即为没有出现过的子串
	for (int i = 0; i < (1 << k); i++) {
		if (b[i] == 0) {
			ans = i;
			break;
		}
	}
	printbinary(ans, k);

	return 0;
}