#include <iostream>

#include "../common/common.h"

using namespace std;

tuple<number, number, bool> find_next_digits(const number &n, const digit_counter &current_digit, const number &first_factor_so_far, const number &second_factor_so_far, const number &base, const number &current_base, const number &previous_base, const number &carry)
{
    for(number x = 2;x <= my_sqrt(n);x++)
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
    common_main(argc, argv, false, true);
}

