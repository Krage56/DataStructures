
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

size_t MyVector::capCalc(bool forced_increase) {
    return std::fabs(loadFactor() - 1.f) < std::numeric_limits<float>::epsilon()
    || forced_increase ?
           _strategy == ResizeStrategy::Multiplicative?
        std::round(_capacity * _coef): _capacity + _coef :
           _strategy == ResizeStrategy::Multiplicative?
           std::round(_capacity / _coef): _capacity - _coef;
}

void MyVector::insert(const size_t i, const ValueType &value) {
    if(i == _size)
        pushBack(value);
    else if(i > _size){
        assert(i > _size);
    }
    else{
        if(_capacity < _size + 1){
            _capacity = capCalc();
            ValueType  *tmp_data = new ValueType[_capacity];
            tmp_data[i] = value;
            for(size_t j = 0; j < i; ++j)
                tmp_data[j] = _data[j];
            for(size_t j = i + 1; j < _size + 1; ++j)
                tmp_data[j] = _data[j - 1];
            delete _data;
            _data = tmp_data;
            ++_size;
        }
        else{
            //Без этого касатавания будет больно
            for(long long j = _size - 1; j >= (const long long)i; --j)
                _data[j + 1] = _data[j];
            _data[i] = value;
            ++_size;
        }
    }
}

void MyVector::insert(const size_t i, const MyVector &value){
    size_t delta = value.size();
    if(i == _size)
        for(size_t j = 0; j < delta; ++j)
            pushBack(value[j]);
    else if(i > _size){
        assert(i > _size);
    }
    else{
        if(_capacity < _size + delta){
            while(_capacity < _size + delta)
                _capacity = capCalc(true);
            ValueType  *tmp_data = new ValueType[_capacity];
            for(size_t j = i; j < delta + i; j++){
                tmp_data[j] = value[j - i];
            }
            for(size_t j = 0; j < i; ++j)
                tmp_data[j] = _data[j];
            for(size_t j = i + delta; j < _size + delta; ++j)
                tmp_data[j] = _data[j - delta];
            delete _data;
            _data = tmp_data;
        }
        else{
            for(long long j = (long long)(_size + delta) - 1; j >= (const long long)i + delta; --j){
                _data[j] = _data[j - delta];
            }
            for(size_t j = i; j < i + delta; ++j)
                _data[j] = value[j - i];
        }
        _size += delta;
    }
}

