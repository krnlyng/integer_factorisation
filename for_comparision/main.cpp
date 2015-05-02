#include <iostream>

#include "../common/common.h"

using namespace std;

tuple<number, number, bool> find_next_digits(const number &n, const digit_counter &current_digit, const number &first_factor_so_far, const number &second_factor_so_far, const number &base, const number &current_base, const number &previous_base, const number &carry)
{
    cout << "find_next_digits unimplemented" << endl;
    return make_tuple(0, 0, false);
}

int main(int argc, char *argv[])
{
    common_main(argc, argv, false, true);
}

