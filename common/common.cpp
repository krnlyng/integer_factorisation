/*
 *  This file is part of https://github.com/krnlyng/integer_factorisation.
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

#include "common.h"

using namespace std;

/* this function returns x^y */
#if USE_GMP
number my_pow(const number &x, const digit_counter &y)
{
    mpz_class r;
    mpz_pow_ui(r.get_mpz_t(), x.get_mpz_t(), y);
    return r;
}
number my_sqrt(const number &x)
{
    mpz_class r;
    mpz_sqrt(r.get_mpz_t(), x.get_mpz_t());
    return r;
}
#endif

/* this function finds the inverse of x modulo mod if it exists */
pair<bool, number> find_inverse(const number &x, const number &mod)
{
#if USE_GMP
    mpz_class r;
    int b = mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t());

    return ((b != 0) ? make_pair(true, r) : make_pair(false, 0));
#else
    for(number k = 1;k < mod;k++)
    {
        if((k * x) % mod == 1) return make_pair(true, k);
    }

    return make_pair(false, 0);
#endif
}

/* this function returns true if n is prime and false otherwise
 * this function is only intended for small n */
bool is_prime(const number &n)
{
    if(n == 1) return false;

    for(number i = 2; i <= sqrt(n);i++)
    {
        if(n % i == 0) return false;
    }

    return true;
}

/* this function sets the left-most digit of x to digit in base base
 * (if the digit corresponding to digit_base of x is zero otherwise garbage) */
void set_digit(number &x, const number &digit, const number &digit_base)
{
    x += digit * digit_base;
}

/* this function returns the digit_number-th digit of x in base base) */
number get_digit(const number &x, const digit_counter &digit_number, const number &base)
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

/* this function checks if the new digits solve the digit equation for digit number current_digit
 * and returns a pair consisting of the check and the new carry */
pair<bool, number> check_if_new_digits_solve_digit_equation(const number &n, const number &first_factor_so_far, const number &second_factor_so_far, const number &carry, const digit_counter &current_digit, const number &base)
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
bool is_odd(const number &x)
{
    return ((x & 1) == 1);
}

pair<number, number> trialdivision_factorise(const number &n)
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

pair<number, number> factorise(const number &n, const number &base)
{
    if(n != 0)
    {
        tuple<number, number, bool> r = find_next_digits(n, 0, 0, 0, base, base, 1, 0);

        return make_pair(get<0>(r), get<1>(r));
    }
    else
    {
        return make_pair(1,0);
    }
}

void usage(char *name, bool prime_base, bool trial_division)
{
    cout << "usage:" << endl;
    if(trial_division)
    {
        cout << name << " number" << endl;
        cout << "\tnumber\tis the number which shall be factorised." << endl;
        cout << "number must be positive." << endl;
    }
    else
    {
        cout << name << " [base] number" << endl;
        cout << "\tbase\tis the base with which the algorithm should calculate, if not" << endl;
        cout << "\t\tspecified base = 2 will be used." << endl;
        cout << "\tnumber\tis the number which shall be factorised." << endl;
        cout << "both arguments must be positive." << endl;
    }
    if(prime_base)
    {
        cout << "base must be prime." << endl;
    }
}

int common_main(int argc, char *argv[], bool prime_base, bool trial_division)
{
    number n;
    number base;
    pair<number, number> factors;

    if(argc != 2 && (argc != 3 || trial_division))
    {
        usage(argv[0], prime_base, trial_division);
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
    else if(!trial_division)
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
        usage(argv[0], prime_base, trial_division);
        return -3;
    }

    if(prime_base)
    {
        if(!is_prime(base))
        {
            usage(argv[0], prime_base, trial_division);
            return -3;
        }
    }

    factors = trial_division ? trialdivision_factorise(n) : factorise(n, base);

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

