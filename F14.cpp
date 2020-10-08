#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>


using namespace std;

int fun(const void* a, const void* b) {
	return *((char*)(a)) - *((char*)(b));
}

int main(int argc, char** argv) {
	if (argc < 3) {
		cerr << "Error of arguments" << endl;
		return 1;
	}
	int left = atoi(argv[2]);
	int right = atoi(strchr(argv[2], '-') + 1);
	
	ifstream file(argv[1]);

	if (!file.is_open()) {
		cerr << "Error of file opening" << endl;
		return 2;
	}

	string s;
	getline(file, s, '\0');
	qsort((void*)s.c_str(), s.length(), sizeof(char), fun);
	file.close();

	int max_count = 0;
	char max_value;
	for (int i = 0, ch, k; i < s.length();) {
		ch = s[i];
		k = 1;
		while (s[++i] == ch) {
			++k;
		}
		if((ch<left) || (ch>right)){
			continue;
		}
		if (k > max_count) {
			max_count = k;
			max_value = ch;
		}

	}

	int cod = int(max_value);

	cerr << "The most used symbol is \'" << max_value << "\'\n";
	cerr << "Ascii cod is \'" << cod << "\'\n";
	cerr << "Used " << max_count << " times" << endl;
	
	return 0;
}
