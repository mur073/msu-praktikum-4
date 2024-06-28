#include <iostream>

#include "Mem.h"
#include "Set.h"
#include "SetTest.h"

int main() {
    Mem mm(0);

    SetTest Test(new Set(mm));
    size_t size = 1'000'000;

    steady_clock::time_point begin = steady_clock::now();

    Test.testInsert(size);
    Test.checkInsertSpeed(size);
    Test.removeOdd(size);

    steady_clock::time_point end = steady_clock::now();
    cout << "overall time: ";
    cout << duration_cast<milliseconds>(end - begin).count() << "ms\n";

    return 0;
}