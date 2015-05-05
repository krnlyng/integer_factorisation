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

tuple<number, number, bool> find_next_digits(const number &n, const digit_counter &current_digit, const number &first_factor_so_far, const number &second_factor_so_far, const number &base, const number &current_base, const number &previous_base, const number &carry)
{
    vector<number> possible_first_digits;
    vector<number> increments;
    vector<number>::size_type current_increment = 0;

    if(n % base == 0)
    {
        cout << "hint: n modulo base == 0, cannot skip numbers, try another base!" << endl;
        possible_first_digits.push_back(2);
        increments.push_back(1);
        goto trial_division;
    }

    if(!is_prime(base))
    {
        number product, product_mod;

        for(number a_digit = 1;a_digit < base;a_digit++)
        {
            for(number b_digit = 1;b_digit < base;b_digit++)
            {
                product = a_digit * b_digit;
                product_mod = product % base;

                if(product_mod == get_digit(n, 0, base))
                {
                    possible_first_digits.push_back(a_digit);
                    possible_first_digits.push_back(b_digit);
                }
            }
        }
    }
    else
    {
        cout << "hint: base is prime, cannot skip numbers, try another base!" << endl;
        possible_first_digits.push_back(2);
        increments.push_back(1);
        goto trial_division;
    }

    sort(possible_first_digits.begin(), possible_first_digits.end());

    possible_first_digits.erase(unique(possible_first_digits.begin(), possible_first_digits.end()), possible_first_digits.end());

    for(vector<number>::size_type i = 0;i < possible_first_digits.size();i++)
    {
        number increment = (possible_first_digits[(i+1) % possible_first_digits.size()] - possible_first_digits[i]) % base;
        while(increment < 0) increment += base;

        increments.push_back(increment);
    }

    while(current_increment < possible_first_digits.size() && possible_first_digits[current_increment] < 2)
    {
        current_increment++;
    }

    cout << "calculated increments." << endl;

trial_division:
    for(number x = possible_first_digits[current_increment];x <= my_sqrt(n);x+=increments[current_increment % (increments.size() + 1)], current_increment++)
    {
        if(n % x == 0)
        {
            return make_tuple(x, n / x, true);
        }
    }

    return make_tuple(1, n, false);
}

int main(int argc, char *argv[])
{
    return common_main(argc, argv, false, false);
}

