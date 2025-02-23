#include "Set.h"

size_t Set::findNextList(size_t startPos) {
    size_t i = startPos;

    for (; i < m_capacity; ++i)
        if (m_data[i] && !m_data[i]->empty()) break;

    return i;
}

List::Iterator* Set::newListIterator(size_t listPos, bool fromBegin) {
    listPos = findNextList(listPos + 1 - fromBegin);

    return (listPos < m_capacity) ? m_data[listPos]->newIterator() : nullptr;
}

void* Set::SetIterator::getElement(size_t& size) {
    return listIt ? listIt->getElement(size) : nullptr;
}

void Set::SetIterator::goToNext() {
    if (!listIt) return;

    if (listIt->hasNext()) {
        listIt->goToNext();
        return;
    }

    size_t hash = getHashCode();

    set->_memory.freeMem(listIt);
    listIt = set->newListIterator(hash, false);
}

bool Set::SetIterator::hasNext() {
    if (!listIt) return false;
    if (listIt->hasNext()) return true;

    return set->findNextList(getHashCode() + 1) < set->m_capacity;
}

bool Set::SetIterator::equals(Iterator* right) {
    SetIterator* rightIt = dynamic_cast<SetIterator*>(right);

    if (!listIt && !rightIt->listIt) return true;
    if (set != rightIt->set || !listIt || !rightIt->listIt) return false;

    return listIt->equals(rightIt->listIt);
}

size_t Set::SetIterator::getHashCode() {
    size_t elemSize;
    void* elem = getElement(elemSize);

    return set->PearsonHashing(elem, elemSize);
}

Container::Iterator* Set::find(void* elem, size_t size) {
    size_t hash = PearsonHashing(elem, size);

    if (!m_data[hash] || m_data[hash]->empty()) return nullptr;

    Iterator* listIt = m_data[hash]->find(elem, size);

    if (!listIt) return nullptr;

    SetIterator* it = new SetIterator(this, listIt, hash);

    return it;
}

void Set::remove(Iterator* iter) {
    if (!iter) throw Error("SET-ERR:find: Trying to access to null iterator.");
    if (m_size == 0) return;

    SetIterator* it = dynamic_cast<SetIterator*>(iter);

    if (this != it->set) return;

    bool isLastInList = !it->listIt->hasNext();
    size_t _listPos = it->getHashCode();

    m_data[it->getHashCode()]->remove(it->listIt);
    m_size--;

    if (isLastInList) {
        _memory.freeMem(it->listIt);
        it->listIt = newListIterator(_listPos, 0);
    }
}

int Set::insert(void* elem, size_t size) {
    if (!elem) throw Error("SET-ERR:insert: Trying to access to null pointer.");

    size_t hash = PearsonHashing(elem, size);
    Iterator* tmp;

    if (m_data[hash] && !m_data[hash]->empty() &&
        (tmp = m_data[hash]->find(elem, size))) {
        _memory.freeMem(tmp);
        return 1;
    }

    if (!m_data[hash]) m_data[hash] = new List(_memory);

    int error = m_data[hash]->push_front(elem, size);

    if (error) return 2;

    if (m_data[hash]->size() >= MAX_ALLOWED_LIST_LENGTH) resize(m_capacity * 2);

    m_size++;

    return 0;
}
