#include <iostream>
#include <string>
#include <future>
#include <ctime>
#include "SHA256.h"
#include <vector>
#include <fstream>
#include <Windows.h>
using namespace std;

bool brut(string key) {
	char* str = new char[6];
	for (str[0] = 97; str[0] < 123; str[0]++)
	for (str[1] = 97; str[1] < 123; str[1]++)
	for (str[2] = 97; str[2] < 123; str[2]++)
	for (str[3] = 97; str[3] < 123; str[3]++)
	for (str[4] = 97; str[4] < 123; str[4]++)
	{
		str[5] = '\0';
		if (sha256(string(str)) == key) {
			cout << "hash: " << key << ", password: " << str << '\n';
			return true;
		}
	};

	delete[] str;
	return false;
}

int main() {

	ifstream fin("base.txt", ios_base::in);
	vector<string> keys;
	while (fin.good()) {
		string temp;
		getline(fin, temp, '\n');
		keys.push_back(temp);
	}
	clock_t start = clock();
	for (int i = 0; i < keys.size(); ++i) brut(keys[i]);
	clock_t end = clock();
	cout << "single threaded mode: " << (end - start) / CLOCKS_PER_SEC << " seconds\n\n";

	start = clock();
	int k = keys.size();
	future<bool>* rez = new future<bool>[k];
	for (int i = 0; i < keys.size(); ++i) { rez[i] = async(launch::async, brut, keys[i]); };
	for (int i = 0; i < keys.size(); ++i) { rez[i].get(); };
	end = clock();
	cout << "multithreaded mode: " << (end - start) / CLOCKS_PER_SEC << " seconds\n";
	delete[] rez;

	return 0;
}

