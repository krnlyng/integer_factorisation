/*
 *  Altered version of our second multiplication algorithm.
 *  Copyright (C) 2015 Franz-Josef Anton Friedrich Haider (idea, implementation)
 *  Copyright (C) 2015 Lorenz Oberhammer (proofs)
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

#include "../common/common.h"

using namespace std;

tuple<number, number, bool> find_next_digits(const number &n, const digit_counter &current_digit, const number &first_factor_so_far, const number &second_factor_so_far, const number &base, const number &current_base, const number &previous_base, const number &carry)
{
    number a;
    number b;
    number product;
    number tmp;
    number new_carry;
    number second_factor_digit;
    number a_0th_digit;
    pair<bool, number> inverse;

    for(number first_factor_digit = 0;first_factor_digit < base;first_factor_digit++)
    {
        a = first_factor_so_far;
        set_digit(a, first_factor_digit, previous_base);
        a_0th_digit = get_digit(a, 0, base);

        if(a_0th_digit == 0)
        {
            pair<bool, number> check;
            for(second_factor_digit = 0;second_factor_digit < base;second_factor_digit++)
            {
                b = second_factor_so_far;
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
        else
        {
            b = second_factor_so_far;
            tmp = first_factor_digit * get_digit(b, 0, base);

            for(digit_counter i = 1;i < current_digit;i++)
            {
                tmp += get_digit(a, current_digit - i, base) * get_digit(b, i, base);
            }

            inverse = find_inverse(a_0th_digit, base);
    
            if(inverse.first)
            {
                tmp += carry;
                second_factor_digit = (inverse.second * (get_digit(n, current_digit, base) - tmp)) % base;

                while(second_factor_digit < 0) second_factor_digit += base;

                set_digit(b, second_factor_digit, previous_base);
                new_carry = (tmp + a_0th_digit * second_factor_digit) / base;
               
                product = a * b;

                if(product > n)
                {
                    continue;
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
                    tuple<number, number, bool> factors = find_next_digits(n, current_digit + 1, a, b, base, current_base * base, current_base, new_carry);
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

int main(int argc, char *argv[])
{
    return common_main(argc, argv, true, false);
}

