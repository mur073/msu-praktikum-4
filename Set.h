#pragma once
#include "List_1.h"
#include "SetAbstract.h"
#include "iostream"

const size_t initCapacity = 1000000;

class Set : public AbstractSet {
   private:
    size_t longestListSize;  // temp

    size_t m_capacity;
    size_t m_size;
    List **m_data;

    size_t findNextList(size_t startPos);

    List::Iterator *newListIterator(size_t &listPos, bool fromBegin);

    void cleanContainer();

   public:
    size_t longestList() const { return longestListSize; }

    Set(MemoryManager &mem)
        : AbstractSet(mem), m_size(0), m_capacity(initCapacity) {
        m_data = (List **)_memory.allocMem(sizeof(List *) * m_capacity);

        if (!m_data)
            throw Error(
                "SET-ERR: Could not create new Set because of memory error.");

        cleanContainer();
    }

    ~Set() {
        clear();
        _memory.freeMem(m_data);
    }

    class SetIterator : public Container::Iterator {
       private:
        Iterator *listIt;
        Set *set;
        size_t listPos;

       public:
        SetIterator(Set *set, Iterator *it, size_t listPos)
            : listIt(it), set(set), listPos(listPos) {}

        SetIterator(Set *set)
            : set(set), listIt(set->newListIterator(listPos, true)) {}

        void *getElement(size_t &size);

        void goToNext();

        bool hasNext();
        bool equals(Iterator *right);

        friend class Set;

        ~SetIterator() {
            if (listIt) set->_memory.freeMem(listIt);
        }
    };

    inline size_t max_bytes() { return _memory.size(); }
    inline Iterator *newIterator() { return new SetIterator(this); }
    inline int size() { return m_size; };
    inline bool empty() { return m_size == 0; }

    Iterator *find(void *elem, size_t size);

    int insert(void *elem, size_t size);

    void remove(Iterator *iter);
    void clear();
};