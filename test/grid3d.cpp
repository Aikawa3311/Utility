#include "../grid3d.h"

using namespace Utility;

int main(){
    Grid3D<int> grid(3, 2, 4, 10);
    grid.foreach([&](int const z, int const y, int const x){
        grid.at(z, y, x) = (x + y + z) % 10;
    });

    grid.at(2, 1, 1) = 0;
    grid.print();
    std::cout << "---" << std::endl;

    grid.insert_column(1, 9);
    grid.print();
    std::cout << "---" << std::endl;

    grid.insert_row(1, 7);
    grid.print();
    std::cout << "---" << std::endl;

    grid.insert_depth(1, 5);
    grid.print();
    std::cout << "---" << std::endl;

    grid.remove_column(2);
    grid.print();
    std::cout << "---" << std::endl;

    grid.remove_row(0);
    grid.print();
    std::cout << "---" << std::endl;

    grid.remove_depth(2);
    grid.print();
    std::cout << "---" << std::endl;

    grid.resize(5, 5, 5, 8);
    grid.print();
    grid.print_size();
    std::cout << "---" << std::endl;

    grid.resize(7, 3, 2, 0);
    grid.print();
    grid.print_size();
    std::cout << "---" << std::endl;

    return 0;
}
