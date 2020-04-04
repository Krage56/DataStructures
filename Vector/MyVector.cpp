
#include <iostream>

#include "MyVector.h"

size_t MyVector::capacity() const {
    return _capacity;
}

size_t MyVector::size() const {
    return _size;
}

MyVector::MyVector(size_t size, ResizeStrategy strategy, float coef)
    :_data(nullptr), _capacity(0)
{
    if(strategy == ResizeStrategy::Multiplicative){
        _capacity = size == 0? 1 : round(coef * size);
    }
    else if(strategy == ResizeStrategy::Additive){
        _capacity += round(coef);
    }
    else{
        assert("Unidentified strategy");
    }
    _data = new ValueType[_capacity];
    _size = size;
    _strategy = strategy;
    _coef = coef;
}

MyVector::MyVector(size_t size, ValueType value, ResizeStrategy strategy, float coef)
        :_data(nullptr), _capacity(0)
{
    if(strategy == ResizeStrategy::Multiplicative){
        _capacity = size == 0? 1 : round(coef * size);

    }
    else if(strategy == ResizeStrategy::Additive){
        _capacity += round(coef);
    }
    else{
        assert("Unidentified strategy");
    }
    _data = new ValueType[_capacity];
    for(size_t i = 0; i < size; ++i){
        *(_data + i) = value;
    }
    _size = size;
    _strategy = strategy;
    _coef = coef;
}

MyVector::~MyVector() {
    delete _data;
}

MyVector::MyVector(const MyVector &copy)
:_data(nullptr), _capacity(0)
{
    _data = new ValueType[copy._capacity];
    _size = copy._size;
    _capacity = copy._capacity;
    _strategy = copy._strategy;
    _coef = copy._coef;
    for(size_t i = 0; i < _size; ++i)
        *(_data + i) = *(copy._data + i);
}

MyVector& MyVector::operator=(const MyVector &copy) {
    if(this == &copy)
        return *this;
    MyVector tmp(copy);
    delete _data;
    _data = tmp._data;
    _size = tmp._size;
    _capacity = tmp._capacity;
    _strategy = tmp._strategy;
    _coef = tmp._coef;
    return *this;
}

MyVector::MyVector(MyVector &&moveVec) noexcept {
    _data = moveVec._data;
    _size = moveVec._size;
    _capacity = moveVec._capacity;
    _strategy = moveVec._strategy;
    _coef = moveVec._coef;
    moveVec._data = nullptr;
    moveVec._size = 0;
    moveVec._capacity = 0;
    moveVec._coef = 0;
}

MyVector &MyVector::operator=(MyVector &&moveVec) noexcept {
    if(this == &moveVec)
        return *this;
    delete _data;
    _size = moveVec._size;
    _capacity = moveVec._capacity;
    _data = moveVec._data;
    _strategy = moveVec._strategy;
    _coef = moveVec._coef;
    moveVec._data = nullptr;
    moveVec._size = 0;
    moveVec._capacity = 0;
    moveVec._coef = 0;
    return *this;
}

ValueType &MyVector::operator[](const size_t i) const {
    if(i >= _size)
        assert(i >= _size);
    return _data[i];
}

void MyVector::resize_data(size_t newCap) {
    if(newCap < _size)
        _size = newCap;
    ValueType  *tmp_data = new ValueType[newCap];
    memcpy(tmp_data, _data, _size * sizeof(ValueType));
    delete _data;
    _data = tmp_data;
    _capacity = newCap;
}

float MyVector::loadFactor() {
    if(_capacity != 0)
        return _size / _capacity;
    return 0;
}

void MyVector::pushBack(const ValueType &value) {
    if(_capacity < _size + 1){
        resize_data(capCalc());
    }
    _data[_size] = value;
    ++_size;
}

size_t MyVector::capCalc() {
    // loadFactor примерно равен единице, тогда увеличим capacity
    size_t result = 0;
    if(std::fabs(loadFactor() - 1.f)
    < std::numeric_limits<float>::epsilon()){
        _strategy == ResizeStrategy::Multiplicative?
        result = round(_capacity * _coef):
                result = _capacity + _coef;
    }
    //Иначе уменьшаем capacity
    else{
        _strategy == ResizeStrategy::Multiplicative?
                result = round(_capacity / _coef):
                result = _capacity - _coef;
    }

    return result;
}






