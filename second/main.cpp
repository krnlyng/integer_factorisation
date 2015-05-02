/*
 *  Another slow but simple integer factorisation algorithm.
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

using namespace std;

#if USE_GMP
#include <gmpxx.h>
typedef mpz_class number;
typedef unsigned long int digit_counter;
#else
typedef unsigned long long int number;
typedef unsigned long int digit_counter;
#endif

/* this function returns x^y */
#if USE_GMP
inline number my_pow(const number &x, const digit_counter &y)
{
    mpz_class r;
    mpz_pow_ui(r.get_mpz_t(), x.get_mpz_t(), y);
    return r;
}
#else
#include <cmath>
#define my_pow(...) (number)pow(__VA_ARGS__)
#endif

/* this function returns the digit_number-th digit of x in base base) */
inline number get_digit(const number &x, const digit_counter &digit_number, const number &base)
{
    if(base == 2)
    {
        return (x >> digit_number) & 1;
    }
    else
    {
        return (x / my_pow(base, digit_number)) % base;
    }
}

/* this function sets the left-most digit of x to digit in base base
 * (if the digit corresponding to digit_base of x is zero otherwise garbage) */
inline void set_digit(number &x, const number &digit, const number &digit_base)
{
    x += digit * digit_base;
}

/* this function checks if the new digits solve the digit equation for digit number current_digit
 * and returns a pair consisting of the check and the new carry */
inline pair<bool, number> check_if_new_digits_solve_digit_equation(const number &n, const number &first_factor_so_far, const number &second_factor_so_far, const number &carry, const digit_counter &current_digit, const number &base)
{
    number tmp = 0;
    number new_carry = 0;

    /* current_digit == 0 optimisations: one factor has to be even if the product is even and both must be odd if the product is odd */
    for(digit_counter i = 0;i <= current_digit;i++)
    {
        tmp += get_digit(first_factor_so_far, i, base) * get_digit(second_factor_so_far, current_digit - i, base);
    }

    tmp += carry;

    new_carry = tmp / base;
    
    if(tmp % base == get_digit(n, current_digit, base))
    {
        return make_pair(true, new_carry);
    }

    return make_pair(false, 0);
}

/* this function returns true if x is odd and false otherwise */
inline bool is_odd(const number &x)
{
    return ((x & 1) == 1);
}

tuple<number, number, bool> find_next_digits(const number &n, const digit_counter &current_digit, const number &first_factor_so_far, const number &second_factor_so_far, const number &base, const number &current_base, const number &previous_base, const number &carry)
{
    number a;
    number b;
    number product;
    pair<bool, number> check;

    for(number first_factor_digit = (current_digit == 0 && is_odd(n)) ? 1 : 0;first_factor_digit < base;first_factor_digit += (current_digit == 0) ? 2 : 1)
    {
        for(number second_factor_digit = (current_digit == 0 && is_odd(n)) ? 1 : 0;second_factor_digit < base;second_factor_digit += (current_digit == 0 && is_odd(n)) ? 2 : 1)
        {
            a = first_factor_so_far;
            b = second_factor_so_far;
            set_digit(a, first_factor_digit, previous_base);
            set_digit(b, second_factor_digit, previous_base);

            check = check_if_new_digits_solve_digit_equation(n, a, b, carry, current_digit, base);

            if(check.first)
            {
                product = a * b;

                if(product > n)
                {
                    break;
                }

                if(product == n)
                {
                    /* don't use trivial factorisations */
                    if(a != 1 && b != 1)
                    {
                        return make_tuple(a, b, true);
                    }
                }
                else
                {
                    tuple<number, number, bool> factors = find_next_digits(n, current_digit + 1, a, b, base, current_base * base, current_base, check.second);
                    if(get<2>(factors)) return move(factors);
                }
            }
        }
    }

    return make_tuple(1, n, false);
}

pair<number, number> factorise(const number &n, const number &base)
{
    if(n != 0)
    {
        tuple<number, number, bool> r = find_next_digits(n, 0, 0, 0, base, base, 1, 0);

        return make_pair(get<0>(r), get<1>(r));
    }
    else
    {
        return make_pair(1, 0);
    }
}

void usage(char *name)
{
    cout << "usage:" << endl;
    cout << name << " [base] number" << endl;
    cout << "\tbase\tis the base with which the algorithm should calculate, if not" << endl;
    cout << "\t\tspecified base = 2 will be used." << endl;
    cout << "\tnumber\tis the number which shall be factorised." << endl;
    cout << "both arguments must be positive." << endl;
}

int main(int argc, char *argv[])
{
    number n;
    number base;
    pair<number, number> factors;

    if(argc != 2 && argc != 3)
    {
        usage(argv[0]);
        return -1;
    }

    if(argc == 2)
    {
        base = 2;
#if USE_GMP
        n = argv[1];
#else
        n = strtoull(argv[1], NULL, 10);
#endif
    }
    else
    {
#if USE_GMP
        base = argv[1];
        n = argv[2];
#else
        base = strtoull(argv[1], NULL, 10);
        n = strtoull(argv[2], NULL, 10);
#endif
    }

    if(base < 0 || n < 0)
    {
        usage(argv[0]);
        return -3;
    }

    factors = factorise(n, base);

    if((factors.first == 1 || factors.second == 1) && !(factors.first == factors.second))
    {
        cout << "n = " << n << " is prime." << endl;
    }
    else
    {
        cout << "n = " << n << " can be factorised as:" << endl;
        cout << factors.first << " * " << factors.second << endl;
    }

    return 0;
}

