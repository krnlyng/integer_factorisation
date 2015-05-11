#include <iostream>
#include <random>
#include <functional>
#include <chrono>
#include <cmath>

#include "../common/common.h"

using namespace std;

void usage(char *name)
{
    cout << "usage:" << endl;
    cout << name << " [p] number" << endl;
    cout << "p\tIf p is specified the p-norm will be used in calculations, must be" << endl;
    cout << "\tgreater or equal to 1." << endl;
    cout << "number\tIs the number which shall be factorised, must be greater or equal to 1." << endl;
}

pair<number, number> rand_factorise(const number &n, const digit_counter &p)
{
    number s = 0, t = 0;
    number prev_s = 0, prev_t = 0;
 
#if USE_GMP
    gmp_randstate_t rand_state;
    gmp_randinit_default(rand_state);
    gmp_randseed_ui(rand_state, std::chrono::system_clock::now().time_since_epoch().count());

    auto dice = bind(my_rand, rand_state, 2, sqrt(n));
#else
    default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

    uniform_int_distribution<number> distribution(2, sqrt(n));

    auto dice = bind(distribution, generator);
#endif

    goto roll_dice;

    while(s * t != n || s == 1 || t == 1)
    {
        prev_s = s;
        prev_t = t;

        s = 1.f / (1.f / static_cast<float_number>(my_pow(s, p)) + 1.f / static_cast<float_number>(my_pow(t, p)));

        if(s == 0)
        {
#if DEBUG
            cout << "s == 0" << endl;
#endif
            goto roll_dice;
        }

        t = n / s;

        if(t == prev_t || s == prev_s)
        {
#if DEBUG
            cout << "loop" << endl;
#endif
            goto roll_dice;
        }

        continue;

roll_dice:
#if DEBUG
        cout << "rolling dice" << endl;
#endif
        s = dice();
        t = n / s;
    }

    return make_pair(s, t);
}

std::pair<number, number> factorise(const number &n, const number &base, const digit_counter &steps)
{
    return rand_factorise(n, 1);
}

int main(int argc, char *argv[])
{
    number n;
    digit_counter p;
    pair<number, number> factors;

    if(argc != 2 && argc != 3)
    {
        usage(argv[0]);
        return -1;

    }

    if(argc == 2)
    {
        p = 1;
#if USE_GMP
        n = argv[1];
#else
        n = strtoull(argv[1], NULL, 10);
#endif
    }
    else if(argc == 3)
    {
#if USE_GMP
        p = strtoull(argv[1], NULL, 10);
        n = argv[2];
#else
        p = strtoull(argv[1], NULL, 10);
        n = strtoull(argv[2], NULL, 10);
#endif
    }

    if(n < 1 || p < 1)
    {
        usage(argv[0]);
        return -2;
    }

    factors = rand_factorise(n, p);

    cout << "n = " << n << " can be factorised as:" << endl;
    cout << factors.first << " * " << factors.second << endl;

    return 0;
}

