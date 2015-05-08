/*
 * A slightly improved trial division algorithm which skips integers which can
 * never be a factor of the starting number.
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

inline void find_possible_factor_residuals(const number &n, const digit_counter &current_digit, vector<number> &possible_factor_residuals, const number &base, const number &first_factor_so_far, const number &second_factor_so_far, const digit_counter &steps, const number &carry, const number &previous_base)
{
    number a, b;
    pair<bool, number> check;

    /* a_digit == 0 and b_digit == 0 cases are already done */
    for(number a_digit = 1;a_digit < base;a_digit++)
    {
        for(number b_digit = 1;b_digit < base;b_digit++)
        {
            a = first_factor_so_far;
            b = second_factor_so_far;
            set_digit(a, a_digit, previous_base);
            set_digit(b, b_digit, previous_base);

            if(a * b > n)
            {
                break;
            }

            check = check_if_new_digits_solve_digit_equation(n, a, b, carry, current_digit, base, previous_base);

            if(check.first)
            {
                possible_factor_residuals.push_back(a);
                possible_factor_residuals.push_back(b);

                if(current_digit + 1 < steps)
                {
                    find_possible_factor_residuals(n, current_digit + 1, possible_factor_residuals, base, a, b, steps, check.second, previous_base * base);
                }
            }
        }
    }
}

pair<number, number> factorise(const number &n, const number &base, const digit_counter &steps)
{
    vector<number> possible_factor_residuals;
    vector<number> increments;
    vector<number>::size_type current_increment = 0;

    if(n % base == 0)
    {
#if DEBUG
        cout << "hint: n modulo base == 0, cannot skip numbers, try another base!" << endl;
#endif
        possible_factor_residuals.push_back(2);
        increments.push_back(1);
        goto trial_division;
    }

    if(!is_prime(base))
    {
        find_possible_factor_residuals(n, 0, possible_factor_residuals, base, 0, 0, steps, 0, 1);
    }
    else
    {
#if DEBUG
        cout << "hint: base is prime, cannot skip numbers, try another base!" << endl;
#endif
        possible_factor_residuals.push_back(2);
        increments.push_back(1);
        goto trial_division;
    }

    sort(possible_factor_residuals.begin(), possible_factor_residuals.end());

    possible_factor_residuals.erase(unique(possible_factor_residuals.begin(), possible_factor_residuals.end()), possible_factor_residuals.end());

    for(vector<number>::size_type i = 0;i < possible_factor_residuals.size();i++)
    {
        number increment = (possible_factor_residuals[(i+1) % possible_factor_residuals.size()] - possible_factor_residuals[i]) % base;
        while(increment < 0) increment += base;

        increments.push_back(increment);
    }

    while(current_increment < possible_factor_residuals.size() && possible_factor_residuals[current_increment] < 2)
    {
        current_increment++;
    }

#if DEBUG
    cout << "calculated increments." << endl;
#endif

trial_division:
    for(number x = possible_factor_residuals[current_increment];x <= my_sqrt(n);)
    {
        if(n % x == 0)
        {
            return make_pair(x, n / x);
        }

        x += increments[current_increment];

        current_increment++;

        if(current_increment >= increments.size())
        {
            current_increment = 0;
        }
    }

    return make_pair(1, n);
}

int main(int argc, char *argv[])
{
    return common_main(argc, argv, false, false, true);
}

