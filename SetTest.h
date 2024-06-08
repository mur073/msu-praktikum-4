#pragma once

#include <math.h>

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "Mem.h"
#include "Set.h"

using namespace std::chrono;

class SetTest {
   private:
    Set* set;

    void fill(size_t size);

   public:
    SetTest(Set* set) : set(set) {}
    ~SetTest() { delete set; }

    void checkInsertSpeed(size_t testCount);
    void testInsert(size_t testCount);

    void removeOdd(size_t testCount);

    void testInsertString(size_t testCount);
};