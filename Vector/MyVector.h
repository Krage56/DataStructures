#pragma once

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <iterator>
// стратегия изменения capacity
enum class ResizeStrategy {
	Additive,// capacity = OldCapacity + delta
	Multiplicative // capacity = coef * OldCapacity
};

//определение сортировки для sortedSquares()
enum class SortedStrategy{
    Increase,
    Decrease
};
// тип значений в векторе
// потом будет заменен на шаблон
//using ValueType = double;
template <typename ValueType>
class MyVector
{
public:
    class VecIterator: public std::iterator<std::input_iterator_tag, ValueType>
    {
        friend class MyVector;
    private:
        VecIterator(ValueType* p);
    public:
        VecIterator(const VecIterator &it);

        bool operator!=(VecIterator const& other) const;
        bool operator==(VecIterator const& other) const; //need for BOOST_FOREACH
        typename VecIterator::reference operator*() const;
        VecIterator& operator++();
        VecIterator& operator--();
    private:
        ValueType* p;
    };


	MyVector(size_t size = 0,
	        ResizeStrategy = ResizeStrategy::Multiplicative,
	        float coef = 1.5f);
	MyVector(size_t size, ValueType value,
	        ResizeStrategy = ResizeStrategy::Multiplicative,
	        float coef = 1.5f);//Заполнить иниц-ый вектор value
	
	MyVector(const MyVector& copy);
	MyVector& operator=(const MyVector& copy);

    //Конструктор перемещения
    MyVector(MyVector&& moveVec) noexcept;
    MyVector& operator=(MyVector&& moveVec) noexcept;

	~MyVector();

	size_t capacity() const;
	size_t size() const;
	float loadFactor();

	// доступ к элементу, 
	// должен работать за O(1)
	ValueType& operator[](const size_t i) const;

	// добавить в конец,
	// должен работать за amort(O(1))
	void pushBack(const ValueType& value);
	// вставить,
	// должен работать за O(n)
	void insert(const size_t i, const ValueType& value);	// версия для одного значения
	void insert(const size_t i, const MyVector& value);		// версия для вектора

	// удалить с конца,
	// должен работать за amort(O(1))
	void popBack();
	// удалить
	// должен работать за O(n)
	void erase(const size_t i);
	void erase(const size_t i, const size_t len);			// удалить len элементов начиная с i

	// найти элемент,
	// должен работать за O(n)
	// если isBegin == true, найти индекс первого элемента, равного value, иначе последнего
	// если искомого элемента нет, вернуть -1
	long long int find(const ValueType& value, bool isBegin = true) const;

	// зарезервировать память (принудительно задать capacity)
	void reserve(const size_t capacity);

	// изменить размер
	// если новый размер больше текущего, то новые элементы забиваются дефолтными значениями
	// если меньше - обрезаем вектор
	void resize(const size_t size, const ValueType = 0.0);

	// очистка вектора, без изменения capacity
	void clear();

	/*итераторы*/
    typedef VecIterator iterator;
    typedef VecIterator const_iterator;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;


    /*Вспомогательные методы работы с памятью*/
protected:
    /*Считает новый _capacity с учётом политики выделения
    памяти и loadFactor-а*/
    size_t capCalc(size_t cap, bool forced_increase = false);
    //процедура обрезки памяти
    void cropMem();
private:
	ValueType* _data;
	size_t _size;
	size_t _capacity;
	ResizeStrategy _strategy;
	float _coef;
};
template <typename ValueType>
MyVector<ValueType> sortedSquares(const MyVector<ValueType>& vec, SortedStrategy strategy);
template <typename ValueType>
size_t MyVector<ValueType>::capacity() const {
    return _capacity;
}
template <typename ValueType>
size_t MyVector<ValueType>::size() const {
    return _size;
}

template <typename ValueType>
MyVector<ValueType>::MyVector(size_t size, ResizeStrategy strategy, float coef)
        :_data(nullptr), _capacity(0)
{
    if(strategy == ResizeStrategy::Multiplicative){
        _capacity = size == 0? 1 : std::round(coef * size);
    }
    else if(strategy == ResizeStrategy::Additive){
        _capacity = std::round(size + coef);
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

template <typename ValueType>
MyVector<ValueType>::MyVector(size_t size, ValueType value, ResizeStrategy strategy, float coef)
        :_data(nullptr), _capacity(0)
{
    if(strategy == ResizeStrategy::Multiplicative){
        _capacity = size == 0? 1 : std::round(coef * size);

    }
    else if(strategy == ResizeStrategy::Additive){
        _capacity = std::round(size + coef);
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

template <typename ValueType>
MyVector<ValueType>::~MyVector() {
    delete []_data;
}

template <typename ValueType>
MyVector<ValueType>::MyVector(const MyVector &copy)
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

template <typename ValueType>
MyVector<ValueType>& MyVector<ValueType>::operator=(const MyVector &copy) {
    if(this == &copy)
        return *this;

    auto *tmp_data = new ValueType[copy._capacity];
    delete []_data;
    _data = tmp_data;
    memcpy(_data, copy._data, sizeof(ValueType) * copy._size);
    _size = copy._size;
    _capacity = copy._capacity;
    _strategy = copy._strategy;
    _coef = copy._coef;
    return *this;
}

template <typename ValueType>
MyVector<ValueType>::MyVector(MyVector &&moveVec) noexcept {
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

template <typename ValueType>
MyVector<ValueType>& MyVector<ValueType>::operator=(MyVector &&moveVec) noexcept {
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

template <typename ValueType>
ValueType &MyVector<ValueType>::operator[](const size_t i) const {
    if(i >= _size)
        throw std::out_of_range("Index of required position is out of range\n");
    return _data[i];
}


template <typename ValueType>
float MyVector<ValueType>::loadFactor() {
    if(_capacity != 0)
        return (float)_size / (float)_capacity;
    return 0;
}

template <typename ValueType>
void MyVector<ValueType>::pushBack(const ValueType &value) {
    if(_capacity < _size + 1){
        reserve(capCalc(_capacity));
    }
    _data[_size] = value;
    ++_size;
}

template <typename ValueType>
size_t MyVector<ValueType>::capCalc(size_t cap, bool forced_increase) {
    size_t newCap = 0;
    if((std::fabs(loadFactor() - 1.f) < std::numeric_limits<float>::epsilon()) ||
       forced_increase){
        if(_strategy == ResizeStrategy::Multiplicative){
            newCap = std::round(cap * _coef);
        }
        else if(_strategy == ResizeStrategy::Additive){
            newCap = cap + _coef * 2;
        }
    }
    else{
        if(_strategy == ResizeStrategy::Multiplicative){
            newCap = std::round(cap / _coef);
        }
        else if(_strategy == ResizeStrategy::Additive){
            newCap = cap - _coef;
        }
    }
    return newCap;
}

template <typename ValueType>
void MyVector<ValueType>::insert(const size_t i, const ValueType &value) {
    if(i == _size)
        pushBack(value);
    else if(i > _size){
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
            //Здесь было красиво каставание
            for(long long j = _size; j > i; --j)
                _data[j] = _data[j - 1];
            _data[i] = value;
            ++_size;
        }
    }
}

template <typename ValueType>
void MyVector<ValueType>::insert(const size_t i, const MyVector &value){
    size_t delta = value.size();
    if(_size == 0){
        *this = value;
    }
    else if(i == _size)
        for(size_t j = 0; j < delta; ++j)
            pushBack(value[j]);
    else if(i > _size){
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

template <typename ValueType>
void MyVector<ValueType>::popBack() {
    --_size;
    /*обрежем память, если необходимо*/
    cropMem();
}

template <typename ValueType>
void MyVector<ValueType>::clear() {
    delete []_data;
    _data = new ValueType[_capacity];
    _size = 0;
}

template <typename ValueType>
void MyVector<ValueType>::reserve(const size_t capacity) {
    if(capacity < _size)
        _size = capacity;
    ValueType  *tmp_data = new ValueType[capacity];
    memcpy(tmp_data, _data, _size * sizeof(ValueType));
    delete []_data;
    _data = tmp_data;
    _capacity = capacity;
}

template <typename ValueType>
void MyVector<ValueType>::erase(const size_t i) {
    for(size_t j = i; j < _size - 1; ++j){
        _data[j] = _data[j + 1];
    }
    --_size;
    /*обрежем выделяемую память, если необходимо*/
    cropMem();
}

template <typename ValueType>
void MyVector<ValueType>::erase(const size_t i, const size_t len) {
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

template <typename ValueType>
void MyVector<ValueType>::resize(const size_t size, const ValueType default_value) {
    if(size > _size){
        if(size > _capacity){
            reserve(size);
        }
        for(size_t i = _size; i < size; ++i){
            _data[i] = default_value;
        }
    }
    else if(size < _size){
        _size = size;
        cropMem();
    }
    _size = size;
}

template <typename ValueType>
void MyVector<ValueType>::cropMem() {
    if(((std::fabs(loadFactor() - 1/pow(_coef, 2)) < std::numeric_limits<float>::epsilon())
        || (loadFactor() < 1/pow(_coef, 2))) && (_size != 0))
    {
        size_t bottom_border = _strategy == ResizeStrategy::Multiplicative? std::round((float)_size * _coef):
                               std::round((float)_size + _coef);
        reserve(bottom_border);
    }
    else if (_size == 0)
        reserve(1);
}

template <typename ValueType>
long long int MyVector<ValueType>::find(const ValueType &value, bool isBegin) const {
    if(isBegin){
        for(size_t i = 0; i < _size; ++i){
            if(_data[i] == value)
                return i;
        }
    }
    else{
        for(long long i = _size; i > 0; --i){
            if(_data[i - 1] == value)
                return i;
        }
    }

    return -1;
}
template <typename ValueType>
typename MyVector<ValueType>::iterator MyVector<ValueType>::begin()
{
    return iterator(_data);
}

template <typename ValueType>
typename MyVector<ValueType>::iterator MyVector<ValueType>::end()
{
    return iterator(_data + _size);
}

template <typename ValueType>
typename MyVector<ValueType>::const_iterator MyVector<ValueType>::begin() const
{
    return const_iterator(_data);
}

template <typename ValueType>
typename MyVector<ValueType>::const_iterator MyVector<ValueType>::end() const
{
    return const_iterator(_data + _size);
}
template <typename ValueType>
MyVector<ValueType>::VecIterator::VecIterator(ValueType *p):p(p){}


template <typename ValueType>
MyVector<ValueType>::VecIterator::VecIterator(const VecIterator& it) :
        p(it.p)
{

}

template <typename ValueType>
bool MyVector<ValueType>::VecIterator::operator!=(VecIterator const& other) const
{
    return p != other.p;
}

template <typename ValueType>
bool MyVector<ValueType>::VecIterator::operator==(VecIterator const& other) const
{
    return p == other.p;
}

template <typename ValueType>
typename MyVector<ValueType>::VecIterator::reference MyVector<ValueType>::VecIterator::operator*()
const{
    return *p;
}

template <typename ValueType>
typename MyVector<ValueType>::VecIterator &MyVector<ValueType>::VecIterator::operator++()
{
    ++p;
    return *this;
}

template <typename ValueType>
typename MyVector<ValueType>::VecIterator &MyVector<ValueType>::VecIterator::operator--()
{
    --p;
    return *this;
}

template <typename ValueType>
MyVector<ValueType> sortedSquares(const MyVector<ValueType>& vec, SortedStrategy strategy){
    MyVector<ValueType> result(vec.size());
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