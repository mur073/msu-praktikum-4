#include "SetTest.h"

int SetTest::testInsert(size_t testCount) {
    printf("testing insert()...\n");

    int err = 0;
    for (int i = 0; i < testCount; ++i) {
        err = set->insert(&i, sizeof(i));

        if (err == 1) {
            cout << "ERROR: Element " << i << " already exists in set.\n";
            return 1;
        }
        if (err == 2) {
            cout << "ERROR: Element " << i << " was not inserted.\n";
            return 1;
        }
    }

    return 0;
}

int SetTest::testFind(size_t sizeCount) {
    printf("testing find()...\n");

    Set::Iterator* it;

    for (int i = 0; i < sizeCount; ++i) {
        it = set->find(&i, sizeof(i));

        if (!it) {
            cout << "ERROR: Element " << i << " was not found.\n";
            return 1;
        }

        delete dynamic_cast<Set::SetIterator*>(it);
    }

    return 0;
}

int SetTest::removeOdd(size_t testCount) {
    printf("testing remove() by deleting odd elements...\n");

    Set::Iterator* it = set->newIterator();

    int *elem, elemCopy;
    size_t size;

    while (it->hasNext()) {
        elem = (int*)(it->getElement(size));

        if ((*elem) % 2) {
            elemCopy = *elem;
            set->remove(it);

            Set::Iterator* tmp = set->find(&elemCopy, size);
            if (tmp) {
                cout << "Element " << elemCopy << " was not deleted\n";
                return 1;
            }
        } else {
            it->goToNext();
        }
    }

    delete dynamic_cast<Set::SetIterator*>(it);

    return 0;
}

std::string numberToBin(int number, int bits) {
    std::string s(bits, '0');

    for (int i = 0; i < bits; ++i) {
        s[i] = ((number >> (bits - i - 1)) & 1) ? '1' : '0';
    }

    return s;
}

int SetTest::testInsertString(size_t testCount) {
    printf("testing insert() and find() with strings...\n");
    set->clear();

    int bits = (int)log2(testCount);

    for (int i = 0; i < testCount; ++i) {
        string s = numberToBin(i, bits);

        int err = set->insert(&s, s.size());

        if (err != 0) {
            cout << "ERROR: Unexpected behaviour.\n";
            return 1;
        }
    }

    for (int i = 0; i < testCount; ++i) {
        string s = numberToBin(i, bits);

        Set::Iterator* it = set->find(&s, s.size());

        if (!it) {
            cout << "ERROR: String: " << s << " was not found.\n";
            return 1;
        }

        delete dynamic_cast<Set::SetIterator*>(it);
    }

    return 0;
}