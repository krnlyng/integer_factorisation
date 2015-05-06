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

#include <iostream>
#include <tuple>

#if USE_GMP
#include <gmpxx.h>
typedef mpz_class number;
typedef unsigned long int digit_counter;
#else
typedef unsigned long long int number;
typedef unsigned long int digit_counter;
#endif

bool is_prime(const number &n);

bool is_odd(const number &x);

extern std::pair<number, number> factorise(const number &n, const number &base = 2, const digit_counter &steps = 1);

void usage(char *name, bool prime_base);

int common_main(int argc, char *argv[], bool prime_base, bool trial_division, bool use_steps);

/* this function returns x^y */
#if USE_GMP
inline number my_pow(const number &x, const digit_counter &y)
{
    mpz_class r;
    mpz_pow_ui(r.get_mpz_t(), x.get_mpz_t(), y);
    return r;
}
inline number my_sqrt(const number &x)
{
    mpz_class r;
    mpz_sqrt(r.get_mpz_t(), x.get_mpz_t());
    return r;
}
#else
#include <cmath>
#define my_pow(...) (number)pow(__VA_ARGS__)
#define my_sqrt(...) (number)sqrt(__VA_ARGS__)
#endif

/* this function finds the inverse of x modulo mod if it exists */
inline std::pair<bool, number> find_inverse(const number &x, const number &mod)
{
#if USE_GMP
    mpz_class r;
    int b = mpz_invert(r.get_mpz_t(), x.get_mpz_t(), mod.get_mpz_t());

    return ((b != 0) ? std::make_pair(true, r) : std::make_pair(false, 0));
#else
    for(number k = 1;k < mod;k++)
    {
        if((k * x) % mod == 1) return std::make_pair(true, k);
    }

    return std::make_pair(false, 0);
#endif
}

/* this function sets the left-most digit of x to digit in base base
 * (if the digit corresponding to digit_base of x is zero otherwise garbage) */
inline void set_digit(number &x, const number &digit, const number &digit_base)
{
    x += digit * digit_base;
}

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

/* this function checks if the new digits solve the digit equation for digit number current_digit
 * and returns a pair consisting of the check and the new carry */
inline std::pair<bool, number> check_if_new_digits_solve_digit_equation(const number &n, const number &first_factor_so_far, const number &second_factor_so_far, const number &carry, const digit_counter &current_digit, const number &base)
{
    number tmp = 0;
    number new_carry = 0;

    for(digit_counter i = 0;i <= current_digit;i++)
    {
        tmp += get_digit(first_factor_so_far, i, base) * get_digit(second_factor_so_far, current_digit - i, base);
    }

    tmp += carry;

    new_carry = tmp / base;
    
    if(tmp % base == get_digit(n, current_digit, base))
    {
        return std::make_pair(true, new_carry);
    }

    return std::make_pair(false, 0);
}

