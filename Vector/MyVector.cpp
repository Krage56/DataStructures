
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
        //_capacity += round(coef);
        do{
            _capacity += round(coef);
        }while(_capacity < size);
    }
    else{
        //assert("Unidentified strategy");
        throw std::invalid_argument("Unidentified strategy\n");
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
        //assert(strategy);
        throw std::invalid_argument("Unidentified strategy\n");
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
    delete []_data;
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

    ValueType *tmp_data = new ValueType[copy._capacity];
    delete []_data;
    _data = tmp_data;
    memcpy(_data, copy._data, sizeof(ValueType) * copy._size);
    _size = copy._size;
    _capacity = copy._capacity;
    _strategy = copy._strategy;
    _coef = copy._coef;
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
    delete []_data;
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
        //assert(i >= _size);
        throw std::out_of_range("Index of required position is out of range\n");
    return _data[i];
}



float MyVector::loadFactor() {
    if(_capacity != 0)
        return (float)_size / (float)_capacity;
    return 0;
}

void MyVector::pushBack(const ValueType &value) {
    if(_capacity < _size + 1){
        reserve(capCalc(_capacity));
    }
    _data[_size] = value;
    ++_size;
}

size_t MyVector::capCalc(size_t cap, bool forced_increase) {
    return std::fabs(loadFactor() - 1.f) < std::numeric_limits<float>::epsilon()
    || forced_increase ?
           _strategy == ResizeStrategy::Multiplicative?
        std::round(cap * _coef): cap + _coef :
           _strategy == ResizeStrategy::Multiplicative?
           std::round(cap / _coef): cap - _coef;
}


void MyVector::insert(const size_t i, const ValueType &value) {
    if(i == _size)
        pushBack(value);
    else if(i > _size){
        //assert(i > _size);
        throw std::out_of_range("Index of required position is out of range\n");
    }
    else{
        if(_capacity < _size + 1){
            _capacity = capCalc(_capacity);
            ValueType  *tmp_data = new ValueType[_capacity];
            tmp_data[i] = value;
            for(size_t j = 0; j < i; ++j)
                tmp_data[j] = _data[j];
            for(size_t j = i + 1; j < _size + 1; ++j)
                tmp_data[j] = _data[j - 1];
            delete []_data;
            _data = tmp_data;
            ++_size;
        }
        else{
            //Без этого каставания будет больно
            for(long long j = _size - 1; j >= (const long long)i; --j)
                _data[j + 1] = _data[j];
            _data[i] = value;
            ++_size;
        }
    }
}

void MyVector::insert(const size_t i, const MyVector &value){
    size_t delta = value.size();
    if(_size == 0){
        *this = value;
    }
    else if(i == _size)
        for(size_t j = 0; j < delta; ++j)
            pushBack(value[j]);
    else if(i > _size){
        //assert(i > _size);
        throw std::out_of_range("Index of required position is out of range\n");
    }
    else{
        if(_capacity < _size + delta){
            while(_capacity < _size + delta)
                _capacity = capCalc(_capacity,true);
            ValueType  *tmp_data = new ValueType[_capacity];
            for(size_t j = i; j < delta + i; j++){
                tmp_data[j] = value[j - i];
            }
            for(size_t j = 0; j < i; ++j)
                tmp_data[j] = _data[j];
            for(size_t j = i + delta; j < _size + delta; ++j)
                tmp_data[j] = _data[j - delta];
            delete []_data;
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

void MyVector::popBack() {
    --_size;
    /*обрежем память, если необходимо*/
    cropMem();
}

void MyVector::clear() {
    delete []_data;
    _data = new ValueType[_capacity];
    _size = 0;
}

void MyVector::reserve(const size_t capacity) {
    if(capacity < _size)
        _size = capacity;
    ValueType  *tmp_data = new ValueType[capacity];
    memcpy(tmp_data, _data, _size * sizeof(ValueType));
    delete []_data;
    _data = tmp_data;
    _capacity = capacity;
}

void MyVector::erase(const size_t i) {
    for(size_t j = i; j < _size - 1; ++j){
        _data[j] = _data[j + 1];
    }
    --_size;
    /*обрежем выделяемую память, если необходимо*/
    cropMem();
}

void MyVector::erase(const size_t i, const size_t len) {
    if (i + len > _size)
        //assert(i + len >= _size);
        throw std::out_of_range("Index of required position is "
                                "out of range\n");
    else if (i + len == _size - 1)
        for (size_t j = i; j < len; ++j){
            popBack();
        }
    else{
        /*сколько элементов от последнего удалённого до конца
        вектора*/
        size_t delta = _size - (i + len /*+ 1*/);
        /*перенесём delta элементов с конца массива
         на освободившиеся позиции*/
        for(size_t j = i; j < i + delta; ++j){
            _data[j] = _data[j + len];
        }
        _size -= len;
    }
    /*обрежем выделяемую память, если необходимо*/
    cropMem();
}

void MyVector::resize(const size_t size, const ValueType default_value) {
    if(size > _size){
        if(size > _capacity){
            reserve(size);
        }
        for(size_t i = _size; i < size; ++i){
            _data[i] = default_value;
        }
    }
    _size = size;
    //cropMem();
}

void MyVector::cropMem() {
    if(((std::fabs(loadFactor() - 1/pow(_coef, 2)) < std::numeric_limits<float>::epsilon())
        || (loadFactor() < 1/pow(_coef, 2))) && (_size != 0))
    {
        size_t min_capacity = _capacity;
        while(capCalc(min_capacity) > _size && (float)_size/(float)min_capacity < 0.5)
        {
            min_capacity = capCalc(min_capacity);
        }
        reserve(min_capacity);
    }
    else if (_size == 0)
        reserve(1);
}

long long int MyVector::find(const ValueType &value, bool isBegin) const {
    if(isBegin){
        for(size_t i = 0; i < _size; ++i){
            if(_data[i] == value)
                return i;
        }
    }
    else{
        for(long long i = _size - 1; i >= 0; --i){
            if(_data[i] == value)
                return i;
        }
    }

    return -1;
}

MyVector::iterator MyVector::begin()
{
    return iterator(_data);
}

MyVector::iterator MyVector::end()
{
    return iterator(_data + _size);
}

MyVector::const_iterator MyVector::begin() const
{
    return const_iterator(_data);
}

MyVector::const_iterator MyVector::end() const
{
    return const_iterator(_data + _size);
}

MyVector::VecIterator::VecIterator(ValueType *p) :
        p(p)
{

}


MyVector::VecIterator::VecIterator(const VecIterator& it) :
        p(it.p)
{

}


bool MyVector::VecIterator::operator!=(VecIterator const& other) const
{
    return p != other.p;
}


bool MyVector::VecIterator::operator==(VecIterator const& other) const
{
    return p == other.p;
}


typename MyVector::VecIterator::reference MyVector::VecIterator::operator*()
const{
    return *p;
}


MyVector::VecIterator &MyVector::VecIterator::operator++()
{
    ++p;
    return *this;
}
MyVector::VecIterator &MyVector::VecIterator::operator--()
{
    --p;
    return *this;
}

MyVector sortedSquares(const MyVector& vec, SortedStrategy strategy){
    MyVector result(vec.size());
    size_t top = vec.size() - 1, bottom = 0;
    size_t count = vec.size();
    for(int i = 0; i < vec.size(); ++i){
        if(pow(vec[top], 2) > pow(vec[bottom], 2)){
            result[strategy == SortedStrategy::Decrease? i: count - i - 1] = pow(vec[top], 2);
            top -= 1;
        }
        else{
            result[strategy == SortedStrategy::Decrease? i: count - i - 1] = pow(vec[bottom], 2);
            bottom += 1;
        }
    }
    return result;
}
