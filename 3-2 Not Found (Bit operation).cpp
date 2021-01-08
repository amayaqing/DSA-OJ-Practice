#include<iostream>
#include<string>
using namespace std;

string s;

//��unsigned int�Զ�������ʽ���
void printbinary(unsigned int x, int len) {
	for (int i = len - 1; i >= 0; i--) {
		if (x & (1 << i)) cout << "1";
		else cout << "0";
	}
	cout << endl;
}
//��string��Ϊ������ʽ
unsigned int stringToBin(string str) {
	int t = 1;
	unsigned int x = 0;
	for (int i = str.length() - 1; i >= 0; i--) {
		if (str[i] - 48) x += t;
		t <<= 1;
	}
	return x;
}

//�������Ĵ�
unsigned int lastN(unsigned int a, int k) {
	unsigned int temp = 1;
	for (int i = 0; i < k - 1; i++) {
		temp = (temp << 1) + 1;
	}
	return a & temp;
}

//����һ������logֵ
inline int log(int len) {
	int n = 0;
	while (len >>= 1) n++;
	n++;
	return n;
}

//������һ��������¼ÿһ�����ֵ�ֵ
bool* getBit(int k) {
	bool*b = new bool[1 << k]{ 0 };
	for (int i = 0; i < s.length() - k + 1; i++) {
		b[stringToBin(s.substr(i, k))] = 1;
	}
	return b;
}
//��b[l]�õ�b[l-1]��
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

	//һ�α����õ�����k�ļ�¼��bit
	bool* b = getBit(k);
	//ѭ���õ������
	for (k; k > 1; k--) {
		bool* c = transferBit(b, k);

		//���õ�ȫΪ1�Ĵ�������Լ�ʱ�˳�
		bool exit = 1;
		for (int i = 0; i < (1 << (k - 1)); i++) {
			if (c[i] == 0) exit = 0;
		}
		if (exit) break;

		//���˳����������c�滻b
		else b = c;
	}
	
	unsigned int ans = 0;
	//�ҵ����õ���bit������С��Ϊ0�ĵط�����Ϊû�г��ֹ����Ӵ�
	for (int i = 0; i < (1 << k); i++) {
		if (b[i] == 0) {
			ans = i;
			break;
		}
	}
	printbinary(ans, k);

	return 0;
}