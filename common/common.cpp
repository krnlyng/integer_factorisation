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

/* this function returns true if x is odd and false otherwise */
bool is_odd(const number &x)
{
    return ((x & 1) == 1);
}

void usage(char *name, bool prime_base, bool trial_division, bool use_steps)
{
    cout << "usage:" << endl;
    if(trial_division)
    {
        cout << name << " number" << endl;
        cout << "\tnumber\tis the number which shall be factorised." << endl;
    }
    else if(use_steps)
    {
        cout << name << " [base [steps]] number" << endl;
        cout << "\tbase\tIs the base with which the algorithm should calculate, if not" << endl;
        cout << "\t\tspecified base = 2 will be used." << endl;
        cout << "\tnumber\tIs the number which shall be factorised." << endl;
        cout << "\tsteps\tIs the number of iterations for the digit determination which" << endl;
        cout << "\t\tshould be done, if not specified steps = 1 will be used." << endl;
        cout << "base must be greater than or equal to 2." << endl;
        cout << "steps must be positive." << endl;
    }
    else
    {
        cout << name << " [base] number" << endl;
        cout << "\tbase\tis the base with which the algorithm should calculate, if not" << endl;
        cout << "\t\tspecified base = 2 will be used." << endl;
        cout << "\tnumber\tis the number which shall be factorised." << endl;
        cout << "base must be greater than or equal to 2." << endl;
    }
    cout << "number must be positive." << endl;

    if(prime_base)
    {
        cout << "base must be prime." << endl;
    }
}

int common_main(int argc, char *argv[], bool prime_base, bool trial_division, bool use_steps)
{
    number n;
    number base;
    digit_counter steps = 1;
    pair<number, number> factors;

    if(argc != 2 && (argc != 3 || trial_division) && (argc != 4 || !use_steps))
    {
        usage(argv[0], prime_base, trial_division, use_steps);
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
    else if(argc == 4)
    {
#if USE_GMP
        base = argv[1];
        steps = strtoull(argv[2], NULL, 10);
        n = argv[3];
#else
        base = strtoull(argv[1], NULL, 10);
        steps = strtoull(argv[2], NULL, 10);
        n = strtoull(argv[3], NULL, 10);
#endif
    }
    else if(argc == 3)
    {
#if USE_GMP
        base = argv[1];
        n = argv[2];
#else
        base = strtoull(argv[1], NULL, 10);
        n = strtoull(argv[2], NULL, 10);
#endif
    }

    if(base < 2 || n < 1 || steps < 1)
    {
        usage(argv[0], prime_base, trial_division, use_steps);
        return -3;
    }

    if(prime_base)
    {
        if(!is_prime(base))
        {
            usage(argv[0], prime_base, trial_division, use_steps);
            return -3;
        }
    }

    factors = factorise(n, base, steps);

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

