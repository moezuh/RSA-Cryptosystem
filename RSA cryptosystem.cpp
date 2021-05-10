#include<iostream>
#include<string>
#include<cstdlib>
#include<time.h>

using namespace std;

signed long long int s = -1, t = 0; //for Ext_gcd

int Decimal(int Message[], int ); //Decimal conversion from base27

void base27(int temp[], int , int ); //base27 conversion from decimal

unsigned long long int powers(unsigned long long int, unsigned long long int); //computing powers

unsigned long long int ex_powers(unsigned long long int y, unsigned long long int z, unsigned long long int k); //modification of powers to deal with large integers

unsigned long long int modular_exponentiation(unsigned long long int, unsigned long long int, unsigned long long int); //performing modular exponentiation

bool MillerRabin(unsigned long long int n); //Primality testing

int gcd(unsigned long long int, unsigned long long int); //Normal Euclid's GCD

int Ext_gcd(unsigned long long int, unsigned long long int, int, int, int); //Extended GCD

int main(void)
{
	//Initializing variables
	unsigned long long int p, q, a1, b1, d, e, n, C, Euler_totient_func, M_decimal, P;
	bool is_true = false;
	char bearcat;
	string M_ASKII; // Message in ASKII
	int M_BCAT[M_ASKII.length()], a, b, count; // Message in Bearcat II
	int M_base27[M_ASKII.length()]; //Recovered message
	//Get the message 'M' from the user and convert it from ASCII to BEARCAT II
	while(1)
	{
	count = 2;
	s = -1, t = 0;
	cout<<"Enter the message (in capital letters or blank spaces) \n";
	getline(cin, M_ASKII);
	cout<<endl;
	system("cls");
	
	
	//string to Base27 conversion
	for (int i = 0; i < M_ASKII.length(); i++) 
	{
		if (M_ASKII[i] == ' ') //check for blank space
		M_BCAT[i] = 0;
		else
		M_BCAT[i] = (int)M_ASKII[i] - 64;
	}
	
	
	//Base27 to Decimal Conversion
	M_decimal = Decimal(M_BCAT, M_ASKII.length());
	
	
	//Finding primes p and q
	srand (time(0));
	while (true)
	{
		p = 2 + rand();
		is_true = MillerRabin(p);
		if (is_true == true) 
		{
			for (int itr = 0; itr < 1; itr ++)
			{
				is_true = MillerRabin(p);
				if (is_true == false)
					break;
			}
		}
		if (is_true == true) 
		break;
	}
	cout<<"p = "<<p<<endl;
	
	while (true)
	{
		q = 2 + rand();
		is_true = MillerRabin(q);
		if (is_true == true) 
		{
			for (int itr = 0; itr < 0; itr ++)
			{
				is_true = MillerRabin(q);
				if (is_true == false)
					break;
			}
		}
		if (is_true == true) 
		break;
	}
	cout<<"q = "<<q<<endl;
	
	
	//calculating n
	n = p * q;
	cout<<"n = "<<n<<endl;
	cout<<"\nOriginal message (M): "<<M_ASKII<<endl;
	
	
	
	//Euler totient function
	Euler_totient_func = (p - 1)* (q - 1);
	cout<<"Euler Totient function = "<<Euler_totient_func<<endl;
	
	
	//Public and Private keys
	while (s < 0) //To make sure private key comes positive as we have used unsigned
	{
		//public key
		e = count;
		while (gcd(e, Euler_totient_func) != 1)
		{
			count++;
			e = count;
		}
		count ++;
		//Private Key
		Ext_gcd(e, Euler_totient_func, 0,0,0);
	}
	d = s;
	
	
	//Encrypt
	C = modular_exponentiation(M_decimal, e, n);
	cout<<"Cyber Text (C): "<<C<<endl;
	
	
	//Decrypt
	P = modular_exponentiation(C, d, n);
	cout<<"Recovered message : "<<P<<endl;
	
	
	//Decimal to Base 27 conversion
	base27(M_base27, P, M_ASKII.length());
	
	
	//Display recovered message from base27 to string
	cout<<"\nDecrypted message (P): ";
	for (int i = 0; i < M_ASKII.length(); i++)
	{
		if (M_base27[i] == 0)
		cout<<" ";
		else
		cout<<(char)(M_base27[i] + 64);
	}
	cout<<"\n\n";
	system("pause");
	system("cls");
}
}

int Decimal(int Message[], int n) //Base27 to Decimal Conversion
{
	int dec = Message[n-1];
	int product = 1;
	for (int i = n-2; i >= 0; i--)
	{
		product = product * 27;
		dec = dec + Message[i] * product;
	}
	return dec;
}

void base27(int temp[], int m_10, int n)  //Decimal to Base 27 conversion
{
	for (int i = n-1; i >= 0; i--)
	{
		temp[i] = m_10 % 27;
		m_10 /= 27;
	}
}

unsigned long long int ex_powers(unsigned long long int y, unsigned long long int z, unsigned long long int k)  //modification of powers to deal with large numbers by including modulo
{
	if (z == 1)
	return y % k;
	if (z % 2 == 0)
	return (ex_powers(y * y % k, z/2, k) % k);
	else
	return (y % k  * ex_powers(y * y % k, (z-1)/2, k) % k);
}

unsigned long long int modular_exponentiation(unsigned long long int x, unsigned long long int p, unsigned long long int k)
{
	if (p == 1) 
	return x % k;
	if (p % 2 == 0) 
	return ( ex_powers((x % k) * (x % k) % k, p/2, k)% k);
	else 
	return (x * ex_powers((x % k) * (x % k) % k, (p-1)/2, k) % k);
}

unsigned long long int powers(unsigned long long int y, unsigned long long int z)
{
	if (z == 1)
	return y;
	if (z % 2 == 0)
	return powers(y * y, z/2);
	else
	return y * powers(y * y, (z-1)/2);
}

bool MillerRabin(unsigned long long int n)  //Test to check prime number
{
	if (n % 2 == 0) // n is even, return back
		return false;
	unsigned long long int a, b, k=0;
	int m = n-1; // m is even since n is odd
	a = (2 + rand()) % (n-2);
	
	//finding k and m
	while (m % 2 == 0)
	{
		m /= 2;
		k++;
	}
	b = modular_exponentiation(a, m, n);

	for (int j = 1; j < k; j++)
	{
		if (b != 1 && b != n-1)
			return false;
		else if (b == n-1)
			return true;
		b = modular_exponentiation(b, 2, n);
	}
	if (b != 1)
		return false;
	else
		return true;
}

int gcd(unsigned long long int a,unsigned long long int b)  //Euclid's GCD
{
	int r;
	while (b != 0)
	{
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

int Ext_gcd(unsigned long long int a, unsigned long long int b, int g, int x, int y)  //Extension of GCD
{
	int r = 0, stemp = 0, q = 0;
	
	if (b == 0)
	{
		g = a;
		s = 1;
		t = 0;
	}
	else
	{
		r = a % b;
		q = a / b;
		Ext_gcd(b, r, g, s, t);
		stemp = s;
		s = t;
		t = stemp - t * q;
	}
}
