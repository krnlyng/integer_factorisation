/*
 *  A slow but simple integer factorisation algorithm.
 *  Copyright (C) 2015 Franz-Josef Anton Friedrich Haider
 *  Copyright (C) 2015 Lorenz Oberhammer
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <vector>
#include <tuple>

#if USE_GMP
#include <gmpxx.h>
typedef mpz_class number;
#else
typedef unsigned long long int number;
#endif

using namespace std;

tuple<number, number, bool> find_factors(const number &n, const int &current_digit, const number &first_factor_so_far, const number &second_factor_so_far, const number &base, const number &current_base)
{
    number first_tmp;
    number second_tmp;
    number d;
    number product;
    number product_mod;

    for(number first_factor_digit = 0;first_factor_digit < base;first_factor_digit++)
    {
        for(number second_factor_digit = 0;second_factor_digit < base;second_factor_digit++)
        {
            first_tmp = first_factor_digit * (current_base / base) + first_factor_so_far;
            second_tmp = second_factor_digit * (current_base / base) + second_factor_so_far;

            d = n % current_base;

            product = first_tmp * second_tmp;
            product_mod = product % current_base;

            if(product > n)
            {
                break;
            }

            if(d == product_mod && product != n)
            {
                tuple<number, number, bool> factors = find_factors(n, current_digit + 1, first_tmp, second_tmp, base, current_base * base);
                if(get<0>(factors) * get<1>(factors) == n && get<2>(factors)) return move(factors);
            }

            if(d == product_mod && product == n)
            {
                /* don't use trivial factorisations */
                if(first_tmp != 1 && second_tmp != 1)
                {
                    return make_tuple(first_tmp, second_tmp, true);
                }
            }
        }
    }

    return make_tuple(1, n, false);
}

pair<number, number> factorize(const number &n)
{
    if(n != 0)
    {
        tuple<number, number, bool> r = find_factors(n, 0, 0, 0, 10, 10);

        return make_pair(get<0>(r), get<1>(r));
    }
    else
    {
        return make_pair(1,0);
    }
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

    cout << "n = " << n << " can be factorized as:" << endl;
    cout << factors.first << " * " << factors.second << endl;

    return 0;
}

