#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

using std::cout;          
using std::endl;
int euclide(int, int);    

class Fraction{
  private:
    int nom;
    int den;
    char* buf = NULL;           
  public:
    Fraction(char*);
    Fraction(int n=0, int m=1) : nom(n), den(m) {};
    ~Fraction();
    Fraction operator*(int mul);    
    operator char*();        
    void reduce();          
};

Fraction::Fraction(char* s){
  char* p = strchr(s,'/');        
  if(p != NULL){
    *p='\0';
    p++;
    den=atoi(p);
    }
  else {
    den=1;                          
    }  
  nom=atoi(s);
}

Fraction::~Fraction() {      
  if (buf != NULL) {
    delete buf; 
    }
}

Fraction Fraction::operator* (int mul)
{      
  return Fraction(nom*mul,den);
}

Fraction::operator char*(){
  buf = new char[32];
  //s << nom << "/" << den;  
  sprintf(buf, "%d/%d", nom, den);
  return buf;
}

void Fraction::reduce(){
  int gcd;
  gcd=euclide(abs(nom),abs(den));
  nom /= gcd;
  den /= gcd;

  if(den < 0){       
    den *= -1;
    nom *= -1; 
  }
  return;
}

int euclide(int n,int m){
  int r=1;
  while(n != 0){
    r=m%n;
    m=n;
    n=r;
  }
  return(m);
}

int main(int argc, char* argv[]){
  if(argc != 3){
    cout << "Usage: multiplier nomenator/denomenator" << endl;
    return(-1);
  }
  Fraction x(argv[2]);
  int y = atoi(argv[1]);
  x = x*y;
  x.reduce();
  cout << (char*)x << endl;
  return(0);
}

