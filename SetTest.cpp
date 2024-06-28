#include "SetTest.h"

void SetTest::testInsert(size_t testCount) {
    printf("testing insert() and find() ");

    size_t elemSize;
    int* elem;
    int e;

    for (int i = 0; i < testCount; ++i) {
        e = set->insert(&i, sizeof(i));

        if (e != 0) {
            printf(
                "-> failed.\nExpected insert() to return 0, but got 1 or 2 "
                "instead.\n\n");
            return;
        }

        Set::SetIterator* it =
            dynamic_cast<Set::SetIterator*>(set->find(&i, sizeof(i)));
        if (it) {
            elem = (int*)it->getElement(elemSize);

            if (*elem != i) {
                printf(
                    "-> failed.\nfind() returned iterator to other "
                    "element.\n\n");

                delete it;
                return;
            }

            delete it;
        } else {
            printf("-> failed.\nInserted element was not found.\n\n");
            return;
        }
    }
    printf("\t\t\t\t-> successfully passed.\n\n");
}

void SetTest::checkInsertSpeed(size_t testCount) {
    set->clear();

    printf("testing insert() speed with %zu elements (", testCount);
    steady_clock::time_point begin = steady_clock::now();

    for (int i = 0; i < testCount; ++i) set->insert(&i, sizeof(i));

    steady_clock::time_point end = steady_clock::now();
    cout << duration_cast<milliseconds>(end - begin).count()
         << "ms)\t-> successfully passed.\n\n";
}

void SetTest::removeOdd(size_t testCount) {
    set->clear();
    fill(testCount);

    vector<bool> v(testCount, false);

    printf("testing remove() by deleting odd elements ");

    Set::SetIterator* it = dynamic_cast<Set::SetIterator*>(set->newIterator());
    int *elem, elemCopy;
    size_t size;

    while (it->hasNext()) {
        elem = (int*)(it->getElement(size));
        if ((*elem) % 2) {
            elemCopy = *elem;
            set->remove(it);

            Set::SetIterator* tmp =
                dynamic_cast<Set::SetIterator*>(set->find(&elemCopy, size));
            if (tmp) {
                printf("-> failed.\nRemoved element (%d) was found.\n\n",
                       elemCopy);

                delete it;

                return;
            }

        } else {
            it->goToNext();
        }
    }

    elem = (int*)(it->getElement(size));
    if ((*elem) % 2) {
        elemCopy = *elem;
        set->remove(it);

        Set::Iterator* tmp = set->find(&elemCopy, size);
        if (tmp) {
            printf("-> failed.\nRemoved element (%d) was found.\n\n", elemCopy);

            delete it;

            return;
        }
    }

    delete it;

    it = dynamic_cast<Set::SetIterator*>(set->newIterator());
    while (it->hasNext()) {
        elem = (int*)it->getElement(size);
        v[*elem] = true;

        it->goToNext();
    }

    elem = (int*)it->getElement(size);
    v[*elem] = true;

    delete it;

    for (int i = 0; i < testCount; ++i) {
        if (v[i] == i % 2) {
            if (v[i]) {
                printf("-> failed.\nOdd number (%d) was not removed.", i);
            } else {
                printf("-> failed.\nEven element (%d) was not found.\n", i);
            }
            return;
        }
    }

    printf("\t\t-> successfully passed.\n\n");
}

void numberToBin(char* arr, int number, int bits) {
    for (int i = 0; i < bits; ++i) {
        arr[i] = ((number >> (bits - i - 1)) & 1) ? '1' : '0';
    }
}

void SetTest::testInsertString(size_t testCount) {
    set->clear();

    printf("testing by inserting strings ");

    int bits = (int)log2(testCount);
    char arr[bits + 1];
    arr[bits] = '\0';

    for (int i = 0; i < testCount; ++i) {
        numberToBin(arr, i, bits);

        int err = set->insert(arr, bits + 1);

        if (err != 0) {
            printf("-> failed.\nString was not inserted.\n");
        }
    }

    for (int i = 0; i < testCount; ++i) {
        numberToBin(arr, i, bits);

        Set::SetIterator* it =
            dynamic_cast<Set::SetIterator*>(set->find(arr, bits + 1));

        if (!it) {
            printf("-> fialed.\nString: %s was not found.\n", arr);
        }

        delete it;
    }

    printf("\t\t\t\t-> successfully passed.\n\n");
}

void SetTest::fill(size_t testCount) {
    for (int i = 0; i < testCount; ++i) set->insert(&i, sizeof(i));
}