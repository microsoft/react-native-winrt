#include <iostream>
#include <type_traits>
#include <cstdint>

int main() {
    std::cout << "is uint32_t arithmetic? " << std::is_arithmetic_v<uint32_t> << std::endl;
    std::cout << "is int arithmetic? " << std::is_arithmetic_v<int> << std::endl;
    std::cout << "is double arithmetic? " << std::is_arithmetic_v<double> << std::endl;
    return 0;
}
