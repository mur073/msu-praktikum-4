#include "GroupContainer.h"

size_t GroupContainer::PearsonHashing(void* elem, size_t elemSize) {
    size_t hash = 0, i, j;

    const unsigned char* str = (unsigned char*)elem;
    unsigned char h, H[HASH_LENGTH];

    for (i = 0; i < HASH_LENGTH; ++i) {
        h = T[(str[0] + i) % 256];

        for (j = 0; j < elemSize; ++j) h = T[h ^ str[j]];

        H[i] = h;

        for (j = 0; j < 8; ++j) {
            if ((H[i] >> j) & 1) hash |= 1LL << (8 * (HASH_LENGTH - i - 1) + j);
        }
    }

    return hash % m_capacity;
}

void GroupContainer::resize(size_t newCapacity) {
    List** _m_data = (List**)_memory.allocMem(sizeof(List*) * newCapacity);

    if (!_m_data) return;

    size_t oldCapacity = m_capacity;
    m_capacity = newCapacity;

    for (size_t i = 0; i < newCapacity; ++i) _m_data[i] = nullptr;

    for (size_t i = 0; i < oldCapacity; ++i) {
        if (!m_data[i]) continue;

        if (m_data[i]->empty()) {
            _memory.freeMem(m_data[i]);
            continue;
        }

        Iterator* it = m_data[i]->newIterator();

        size_t hash, elemSize;
        void* elem;
        while (!m_data[i]->empty()) {
            elem = it->getElement(elemSize);
            hash = PearsonHashing(elem, elemSize);

            if (!_m_data[hash]) _m_data[hash] = new List(_memory);

            _m_data[hash]->push_front(elem, elemSize);

            m_data[i]->remove(it);
        }

        _memory.freeMem(it);
        _memory.freeMem(m_data[i]);
    }

    _memory.freeMem(m_data);
    m_data = _m_data;
}

void GroupContainer::clear() {
    for (size_t i = 0; i < m_capacity; ++i) {
        if (m_data[i]) {
            if (!m_data[i]->empty()) m_data[i]->clear();

            _memory.freeMem(m_data[i]);
            m_data[i] = nullptr;
        }
    }

    m_size = 0;
}

void GroupContainer::cleanListsArr() {
    for (size_t i = 0; i < m_capacity; ++i) m_data[i] = nullptr;
}
