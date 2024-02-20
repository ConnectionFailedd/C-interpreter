#ifndef __ANY_HPP__
#define __ANY_HPP__

#include <cstddef>
#include <new>

class Any {
private:
    size_t __size;
    std::align_val_t __alignment;

    void * __ptr;

public:
    inline Any() : __ptr(nullptr) {}
    template<class T>
    inline Any(const T & _value) : __size(sizeof(T)), __alignment(std::align_val_t(alignof(T))) {
        __ptr = operator new(__size, __alignment);
        new(__ptr) T(_value);
    }
    inline Any(Any && _other) : __size(_other.__size), __alignment(_other.__alignment), __ptr(_other.__ptr) {
        _other.__ptr = nullptr;
    }
    inline ~Any() {
        if(__ptr != nullptr) {
            operator delete(__ptr, __size, __alignment);
        }
        __ptr = nullptr;
    }

    template<class T>
    Any & operator=(const T & _value) {
        if(__ptr != nullptr) {
            operator delete(__ptr, __size, __alignment);
        }
        __size = sizeof(T);
        __alignment = std::align_val_t(alignof(T));
        __ptr = operator new(__size, __alignment);
        new(__ptr) T(_value);
        return *this;
    }
};

#endif // __ANY_HPP__