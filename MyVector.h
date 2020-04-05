#pragma once

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
// стратегия изменения capacity
enum class ResizeStrategy {
	Additive,// capacity = OldCapacity + delta
	Multiplicative // capacity = coef * OldCapacity
};

// тип значений в векторе
// потом будет заменен на шаблон
using ValueType = double;

class MyVector
{
public:
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

	/*Вспомогательные методы работы с памятью*/
protected:
    //Изменяет _data в соответсвии с новым _capacity
    void resize_data(size_t newCap);
    /*Считает новый _capacity с учётом политики выделения
    памяти и loadFactor-а*/
    size_t capCalc();
private:
	ValueType* _data;
	size_t _size;
	size_t _capacity;
	ResizeStrategy _strategy;
	float _coef;
};

