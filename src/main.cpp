#include <Map.h>
#include <iostream>
int main() {

    Map<> map(3,2, {
            5,1,1,
            1,1,1
    });

    std::cout << map(0,0) << std::endl;

	return 0;
}
