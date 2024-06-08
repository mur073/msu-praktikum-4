#pragma once

#include "SetAbstract.h"
#include "iostream"

class Set : public AbstractSet {
   private:
    size_t findNextList(size_t startPos);

    List::Iterator *newListIterator(size_t listPos, bool fromBegin);

   public:
    Set(MemoryManager &mem) : AbstractSet(mem) {}

    ~Set() {
        clear();
        _memory.freeMem(m_data);
    }

    class SetIterator : public Container::Iterator {
       private:
        Set *set;
        Iterator *listIt;

        size_t getHashCode();

        SetIterator(Set *set, Iterator *it, size_t listPos)
            : set(set), listIt(it) {}

        SetIterator(Set *set)
            : set(set), listIt(set->newListIterator(0, true)) {}

       public:
        friend class Set;

        void *getElement(size_t &size);

        void goToNext();

        bool hasNext();
        bool equals(Iterator *right);

        ~SetIterator() {
            if (listIt) set->_memory.freeMem(listIt);
        }
    };

    inline Iterator *newIterator() { return new SetIterator(this); }

    Iterator *find(void *elem, size_t size);

    int insert(void *elem, size_t size);

    void remove(Iterator *iter);
};