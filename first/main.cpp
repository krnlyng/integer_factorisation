#include <iostream>
#include <vector>
#include <tuple>

#if USE_GMP
#include <gmpxx.h>
typedef mpz_class number;
#else
typedef unsigned long long int number;
#endif

std::tuple<number, number, bool> find_factors(number n, int current_digit, number first_factor_so_far, number second_factor_so_far, number b, number current_base)
{
    for(number first_factor_digit = 0;first_factor_digit < b;first_factor_digit++)
    {
        for(number second_factor_digit = 0;second_factor_digit < b;second_factor_digit++)
        {
            number first_tmp = first_factor_digit * (current_base / b) + first_factor_so_far;
            number second_tmp = second_factor_digit * (current_base / b) + second_factor_so_far;
            number d = n % current_base;
            number product = first_tmp * second_tmp;
            number product_mod = product % current_base;

            if(product > n)
            {
                break;
            }

            if(d == product_mod && product != n)
            {
                std::tuple<number, number, bool> factors = find_factors(n, current_digit + 1, first_tmp, second_tmp, b, current_base * b);
                if(std::get<0>(factors) * std::get<1>(factors) == n && std::get<2>(factors)) return std::move(factors);
            }

            if(d == product_mod && product == n)
            {
                if(first_tmp != 1 && second_tmp != 1)
                {
                    return std::make_tuple(first_tmp, second_tmp, true);
                }
            }
        }
    }

    return std::make_tuple(1, n, false);
}

std::pair<number, number> factorize(number n)
{
    std::vector<number> factors;

    if(n != 0)
    {
        std::tuple<number, number, bool> r = find_factors(n, 0, 0, 0, 10, 10);

        return std::make_pair(std::get<0>(r), std::get<1>(r));
    }
    else
    {
        return std::make_pair(1,0);
    }
}

int main(int argc, char *argv[])
{
    number n;
    std::pair<number, number> factors;

    if(argc != 2)
    {
        std::cerr << "please specify one (positive integer) argument to this program" << std::endl;
        return -1;
    }

#if USE_GMP
    n = argv[1];
#else
    n = strtoll(argv[1], NULL, 10);
#endif

    factors = factorize(n);

    std::cout << "n = " << n << " can be factorized as:" << std::endl;
    std::cout << factors.first << " * " << factors.second << std::endl;

    return 0;
}

