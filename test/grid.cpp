#include "../grid2d.h"

using namespace Utility;

int main(){
    Grid2D<int> grid(5, 3, 10);
    grid.print();

    grid.foreach([&](size_t y, size_t x){
        grid.at(y, x) = x + y;
    });
    grid.print();

    grid.push_back_column(100);
    grid.push_back_row(200);
    grid.print();

    grid.resize(4, 6, 777);
    grid.print();

    return 0;
}
