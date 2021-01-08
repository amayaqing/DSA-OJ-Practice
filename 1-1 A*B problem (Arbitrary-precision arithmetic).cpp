#include<iostream>
#include<string>
#include<cstdio>
using namespace std;

int exp(int n) {
	int m = 1;
	for (int i = 0; i < n; i++) m *= 10;
	return m;
}


long long* mul(string s1, string s2, int*len) {
	long long a[2][10001]{ 0 }, re[10001]{ 0 };
	len[0] = s1.size() / 6;
	len[1] = s2.size() / 6;
	len[2] = 0;

	//写入数组
	//将s1写为10^6进制的数组
	int i = s1.size() - 1, j = 0;
	while (j<len[0]) {
		a[0][j] = 0;
		int k = 5;
		while (k >= 0) {
			a[0][j] += ((int)s1[i - k] - 48)*exp(k);
			k--;
		}
		j++;
		i -= 6;
	}
	if (s1.size() % 6 != 0) {
		len[0]++;
		int rest = 0;
		while (rest <= i) {
			a[0][j] += ((int)s1[rest] - 48)*exp(i - rest);
			rest++;
		}
	}

	//s2
	i = s2.size() - 1; j = 0;
	while (j<len[1]) {
		a[1][j] = 0;
		int k = 5;
		while (k >= 0) {
			a[1][j] += ((int)s2[i - k] - 48)*exp(k);
			k--;
		}
		j++;
		i -= 6;
	}
	if (s2.size() % 6 != 0) {
		len[1]++;
		int rest = 0;
		while (rest <= i) {
			a[1][j] += ((int)s2[rest] - 48)*exp(i - rest);
			rest++;
		}
	}

	//运算
	for (int i = 0; i < len[1]; i++) {
		int addjw = 0;
		int k = i;
		int muljw = 0;
		for (int j = 0; j < len[0]; j++) {
			long long temp = a[0][j] * a[1][i];
			re[k] += (temp + muljw) % exp(6) + addjw;
			addjw = re[k] / exp(6);
			muljw = (temp + muljw) / exp(6);
			re[k] = re[k] % exp(6);
			k++;
		}
		if (muljw != 0 || addjw!=0) {
			re[k] += muljw + addjw;
			len[2] = k + 1;
		}
		else len[2] = k;
	}

	return re;
}


int main()
{
	std::ios::sync_with_stdio(false);
	int n;
	cin >> n;

	string s1, s2;
	int len[3];
	long long** result = new long long*[n];
	int* length = new int[n];
	for (int i = 0; i < n; i++) {
		cin >> s1 >> s2;
		long long*temp = mul(s1, s2, len);
		length[i] = len[2];
		result[i] = new long long[length[i]];
		for (int j = 0; j < len[2]; j++) result[i][j] = temp[j];
	}

	for (int i = 0; i < n; i++) {
		cout << result[i][length[i] - 1];
		for (int k = length[i] - 2; k>=0; k--) {
			int m = 5;
			while (m >= 0 && result[i][k] / exp(m) == 0) {
				cout << "0";
				m--;
			}
			if(m >= 0) cout << result[i][k];
		}
		cout << endl;
	}
	return 0;
}