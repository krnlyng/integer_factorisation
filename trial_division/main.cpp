/*
 * Trial division.
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

#include "../common/common.h"

using namespace std;

pair<number, number> factorise(const number &n, const number &base, const digit_counter &steps)
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

int main(int argc, char *argv[])
{
    common_main(argc, argv, false, true, false);
}

