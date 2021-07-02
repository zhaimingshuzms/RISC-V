#include <iostream>
#include "ISA.h"
int main() {
    ISA test;
    test.read();
    //test.mem->print_all();
    test.run();
    return 0;
}
