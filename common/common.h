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

#if USE_GMP
number my_pow(const number &x, const digit_counter &y);
number my_sqrt(const number &x);
#else
#include <cmath>
#define my_pow(...) (number)pow(__VA_ARGS__)
#define my_sqrt(...) (number)sqrt(__VA_ARGS__)
#endif

std::pair<bool, number> check_if_new_digits_solve_digit_equation(const number &n, const number &first_factor_so_far, const number &second_factor_so_far, const number &carry, const digit_counter &current_digit, const number &base);

std::pair<bool, number> find_inverse(const number &x, const number &mod);

bool is_prime(const number &n);

void set_digit(number &x, const number &digit, const number &digit_base);
number get_digit(const number &x, const digit_counter &digit_number, const number &base);

std::pair<bool, number> check_if_new_digits_solve_digit_equation(const number &n, const number &first_factor_so_far, const number &second_factor_so_far, const number &carry, const digit_counter &current_digit, const number &base);

bool is_odd(const number &x);

extern std::pair<number, number> factorise(const number &n, const number &base = 2, const digit_counter &steps = 1);

void usage(char *name, bool prime_base);

int common_main(int argc, char *argv[], bool prime_base, bool trial_division, bool use_steps);

