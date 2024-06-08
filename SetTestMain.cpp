#include <iostream>

#include "Mem.h"
#include "Set.h"
#include "SetTest.h"

// g++ -std=c++17 -o out GroupContainer.cpp GroupList.cpp List.cpp Set.cpp
// SetTest.cpp SetTestMain.cpp g++ SetTestMain.cpp SetTest.cpp Set.cpp
// GroupContainer.cpp -o a -std=c++17 leaks --atExit -- ./a
// g++ List_1.cpp GroupList.cpp GroupContainer.cpp Set.cpp SetTest.cpp
// SetTestMain.cpp -o out -std=c++17
//  valgrind --leak-check=yes myprog arg1 arg2

int main() {
    Mem mm(0);

    SetTest Test(new Set(mm));
    size_t size = 1000000;

    // steady_clock::time_point begin = steady_clock::now();

    // Test.testInsert(size);
    // Test.checkInsertSpeed(size);
    // Test.removeOdd(size);

    // steady_clock::time_point end = steady_clock::now();
    // cout << "\noverall time: ";
    // cout << duration_cast<milliseconds>(end - begin).count() << "\n";

    Set *set = new Set(mm);
    for (int i = 0; i < 100; ++i) set->insert(&i, sizeof(i));

    Set::SetIterator *_50thIt;

    Set::SetIterator *it = dynamic_cast<Set::SetIterator *>(set->newIterator());
    int counter = 0;
    while (counter < 50) {
        _50thIt = it;

        it->goToNext();
        counter++;
    }

    it = dynamic_cast<Set::SetIterator *>(
        set->find(_50thIt->getElement(size), size));

    while (counter < 60) {
        it->goToNext();
        counter++;
    }

    while (it->hasNext()) {
        set->remove(it);
    }

    while (_50thIt->hasNext()) {
        size_t size;
        int *elem = (int *)_50thIt->getElement(size);
        printf("elem: %d\n", *elem);

        _50thIt->goToNext();
    }

    return 0;
}