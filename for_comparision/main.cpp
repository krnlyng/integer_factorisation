#include <iostream>

using namespace std;

#if USE_GMP
#include <gmpxx.h>
typedef mpz_class number;
#else
typedef unsigned long long int number;
#endif

#if USE_GMP
number my_sqrt(const number &n)
{
    mpz_class r;
    mpz_sqrt(r.get_mpz_t(), n.get_mpz_t());
    return r;
}
#else
#include <cmath>
#define my_sqrt(...) (number)sqrt(__VA_ARGS__)
#endif

pair<number, number> factorize(const number &n)
{
    for(number x = 2;x <= my_sqrt(n);x++)
    {
        if(n % x == 0)
        {
            return make_pair(x, n / x);
        }
    }

    return make_pair(1, n);
}

int main(int argc, char *argv[])
{
    number n;
    pair<number, number> factors;

    if(argc != 2)
    {
        cerr << "please specify one (positive integer) argument to this program" << endl;
        return -1;
    }

#if USE_GMP
    n = argv[1];
#else
    n = strtoll(argv[1], NULL, 10);
#endif

    factors = factorize(n);

    if((factors.first == 1 || factors.second == 1) && !(factors.first == factors.second))
    {
        cout << "n = " << n << " is prime." << endl;
    }
    else
    {
        cout << "n = " << n << " can be factorized as:" << endl;
        cout << factors.first << " * " << factors.second << endl;
    }

    return 0;
}

