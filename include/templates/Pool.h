#ifndef POOL_H
#define POOL_H

#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>


template <typename T>
class Pool {
public:

    Pool(size_t initialSize = 0, size_t maxSize = 100);

    ~Pool();

    std::unique_ptr<T> acquire();

    void release(std::unique_ptr<T> object);

    size_t getSize() const;
    size_t getActiveCount() const;
    size_t getAvailableCount() const;
    

    void addObject(std::unique_ptr<T> object);
    

    template <typename... Args>
    void populate(size_t count, Args&&... args);
    

    void clear();
    
private:

    std::vector<std::unique_ptr<T>> m_availableObjects;

    size_t m_activeCount;

    size_t m_maxSize;

    template <typename... Args>
    std::unique_ptr<T> createObject(Args&&... args);
};


template <typename T>
Pool<T>::Pool(size_t initialSize, size_t maxSize)
    : m_activeCount(0)
    , m_maxSize(maxSize)
{

    m_availableObjects.reserve(initialSize);
}

template <typename T>
Pool<T>::~Pool() {

}

template <typename T>
std::unique_ptr<T> Pool<T>::acquire() {
    if (m_availableObjects.empty()) {

        return nullptr;
    }

    std::unique_ptr<T> object = std::move(m_availableObjects.back());
    m_availableObjects.pop_back();

    ++m_activeCount;
    
    return object;
}

template <typename T>
void Pool<T>::release(std::unique_ptr<T> object) {

    if (!object) {
        return;
    }

    if (m_availableObjects.size() >= m_maxSize) {

        return;
    }

    m_availableObjects.push_back(std::move(object));
    

    if (m_activeCount > 0) {
        --m_activeCount;
    }
}

template <typename T>
size_t Pool<T>::getSize() const {
    return m_availableObjects.size() + m_activeCount;
}

template <typename T>
size_t Pool<T>::getActiveCount() const {
    return m_activeCount;
}

template <typename T>
size_t Pool<T>::getAvailableCount() const {
    return m_availableObjects.size();
}

template <typename T>
void Pool<T>::addObject(std::unique_ptr<T> object) {

    if (!object) {
        return;
    }

    if (m_availableObjects.size() >= m_maxSize) {

        return;
    }

    m_availableObjects.push_back(std::move(object));
}

template <typename T>
template <typename... Args>
void Pool<T>::populate(size_t count, Args&&... args) {

    size_t spaceLeft = m_maxSize - (m_availableObjects.size() + m_activeCount);
    size_t objectsToCreate = std::min(count, spaceLeft);
    

    for (size_t i = 0; i < objectsToCreate; ++i) {
        m_availableObjects.push_back(createObject(std::forward<Args>(args)...));
    }
}

template <typename T>
void Pool<T>::clear() {

    m_availableObjects.clear();

    m_activeCount = 0;
}

template <typename T>
template <typename... Args>
std::unique_ptr<T> Pool<T>::createObject(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

#endif