#include<cstdio>
#include<iostream>
#include<cstring>
#include<cctype>

using namespace std;

class Alpha
{
private:
	unsigned bin;
public:
	Alpha operator+(Alpha &);
	Alpha operator~();
	Alpha() { bin = 0; }
	Alpha(char*);
	friend ostream & operator<<(ostream&, const Alpha&);
};

Alpha::Alpha(char* s)
{
	bin = 0;
	while (*s) 
	{
		bin |= (1 << (tolower(*s) - 'a'));
		s++;
	}
}

Alpha Alpha :: operator + (Alpha &y)
{
	Alpha z;
	z.bin = bin | y.bin; 
	return z; 
}

Alpha Alpha::operator~()
{
	Alpha z;
	z.bin = ~this->bin;
	return (z);
}
ostream & operator << (ostream &out, const Alpha &z)
{
	unsigned bit = 1 << 25;
	int i;
	for (i = 25; i >= 0; i--)
	{
		if ((z.bin & bit) > 0)	
			out << (char)('a' + i);
		bit = bit >> 1; 	
	}	
	return out;
}


int main(int argc, char* argv[])
{
	Alpha x(argv[1]);
	Alpha y(argv[2]);
	Alpha z = ~( x + y);
	cout << z << endl;
	return 0;
}