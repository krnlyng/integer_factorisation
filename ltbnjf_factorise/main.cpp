/*
 * factorisation algorithm for LTBNJF numbers with x=0
 *  Copyright (C) 2015 Franz-Josef Anton Friedrich Haider
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
#include <cmath>

#include "../common/common.h"

using namespace std;

struct ltbnjf_representation {
    ltbnjf_representation() : ltbnjf_representation(0,0) {}
    ltbnjf_representation(number i, number b)
    {
        this->index = i;
        this->base = b;
    }
    number index;
    number base;
};

void usage(char *name)
{
    cout << "usage:" << endl;
    cout << name << " base number" << endl;
    cout << "\tbase\tis the base of the LTBNJF number which will be factorised" << endl;
    cout << "\tnumber\tis the index of the LTBNJF which will be factorised" << endl;
    cout << "base must be greater or equal to 2" << endl;
    cout << "number must be greater than 0" << endl;
}

/* factor, multiplicity */
std::vector<std::pair<number, unsigned long int>> get_prime_factors(number n)
{
    std::vector<std::pair<number, unsigned long int>> result;

    for(number i = 2; i <= n;i++)
    {
        if(n % i == 0)
        {
            if(is_prime(i))
            {
                unsigned long int multiplicity;
                number tmp_number = n / i;

                for(multiplicity = 1;;multiplicity++)
                {
                    if(tmp_number == 0) break;

                    if(tmp_number % i != 0)
                    {
                        break;
                    }

                    tmp_number = tmp_number / i;
                }

                result.push_back(make_pair(i, multiplicity));
            }
        }
    }

    return result;
}

std::vector<ltbnjf_representation> ltbnjf_factorise(ltbnjf_representation ltbnjf_number)
{
    std::vector<ltbnjf_representation> result;
    std::vector<std::pair<number, unsigned long int>> prime_factors_of_ltbnjf_index = get_prime_factors(ltbnjf_number.index);

    // loop over prime divisors
    for(std::vector<std::pair<number, unsigned long int>>::size_type i = 0;i < prime_factors_of_ltbnjf_index.size();i++)
    {
        // multiply prime factors ^ multiplicity until the i-th one
        unsigned long int prod_l = 1;
        for(std::vector<std::pair<number, unsigned long int>>::size_type l = 0;l < i;l++)
        {
            prod_l *= (my_pow(prime_factors_of_ltbnjf_index[l].first, prime_factors_of_ltbnjf_index[l].second)).get_ui();
        }
        // loop over multiplicity of current prime factor
        for(unsigned long int j_i = 1;j_i <= prime_factors_of_ltbnjf_index[i].second;j_i++)
        {
            unsigned long prod_j_i = (my_pow(prime_factors_of_ltbnjf_index[i].first, prime_factors_of_ltbnjf_index[i].second - j_i).get_ui());
            result.emplace_back(prime_factors_of_ltbnjf_index[i].first, (number)my_pow(ltbnjf_number.base, prod_l * prod_j_i));
        }
    }

    return result;
}

ostream& operator<<(ostream& os, const ltbnjf_representation ltbnjf_number)
{
    os << "I_{0," << ltbnjf_number.index << "," << ltbnjf_number.base << "}";
    return os;
}

int main(int argc, char *argv[])
{
    ltbnjf_representation ltbnjf_number;
    std::vector<ltbnjf_representation> result;

    if(argc != 3)
    {
        usage(argv[0]);
        return -1;
    }

#if USE_GMP
    ltbnjf_number.base = argv[1];
    ltbnjf_number.index = argv[2];
#else
    ltbnjf_number.base = strtoull(argv[1], NULL, 10);
    ltbnjf_number.index = strtoull(argv[2], NULL, 10);
#endif
    
    if(ltbnjf_number.base < 2 || ltbnjf_number.index <= 0)
    {
        usage(argv[1]);
        return -2;
    }

    result = ltbnjf_factorise(ltbnjf_number);

    cout << ltbnjf_number << " can be factorised as:" << endl;
    for(std::vector<ltbnjf_representation>::size_type i = 0;i < result.size();i++)
    {
        cout << result[i] << " ";
    }
    cout << endl;
}

