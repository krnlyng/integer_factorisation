/*
 * Automatic (dirty) test for the two trial division algorithms (assumes that
 * the simple trial_division algorithm is correct).
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

#define main main_trialdivision
#define factorise factorise_trial_division
#include "../trial_division/main.cpp"
#undef factorise
#undef main

#define main main_enhanced_trial_division
#include "../enhanced_trial_division/main.cpp"
#undef main

#include "../common/common.h"

void compare_usage(char *name)
{
    cout << "usage:" << endl;
    cout << name << " steps base maxnum" << endl;
    cout << "steps\tis the maximum number of steps of finding digits in the enhanced" << endl;
    cout << "\ttrial division algorithm should be done must be greater or equal to 1." << endl;
    cout << "base\tis the base with which the enhanced trial division algorithm should work" << endl;
    cout << "\tmust be greater or equal to 2." << endl;
    cout << "maxnum\tis the number until which the trial divisions should be compared must be" << endl;
    cout << "\tgreater than zero." << endl;
}

int main(int argc, char *argv[])
{
    number x = 1;
    number base;
    number maxnum;
    digit_counter steps;

    if(argc != 4)
    {
        compare_usage(argv[0]);
        return -1;
    }

#if USE_GMP
    base = argv[1];
    maxnum = argv[3];
#else
    base = strtoull(argv[1], NULL, 10);
    maxnum = strotull(argv[2], NULL, 10);
#endif
    steps = strtoull(argv[2], NULL, 10);

    if(base < 2 || steps < 1 || maxnum < 0)
    {
        compare_usage(argv[0]);
        return -2;
    }

    for(x = 1;x < maxnum;x++)
    {
        if(factorise(x, base, steps).first != factorise_trial_division(x).first)
        {
            cout << "found error in enhanced trial division, failed to factorise " << x << " properly." << endl;
            return -1;
        }

        if(x % 1000 == 0)
        {
            cout << "reached x = " << x << endl;
        }
    }

    cout << "no errors found." << endl;

    return 0;
}

