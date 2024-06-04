#pragma once
#include "Container.h"

#define containerCapacity 1000003

// #include "HashMap.h"
// Базовый класс для некоторой группы абстрактных контейнеров
class GroupContainer : public Container {
   public:
    GroupContainer(MemoryManager &mem) : Container(mem) {}
    size_t hashFunction(void *key, size_t keySize) {
        size_t hash = 1;
        u_int8_t *str = static_cast<u_int8_t *>(key);
        for (size_t i = 0; i != keySize; ++i) {
            int s = hash + str[i];
            hash ^= (str[i] << 5) * 31;
            hash ^= (str[i] >> 3) * 19;
        }
        return hash % containerCapacity;
    }
};
