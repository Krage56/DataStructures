#pragma once
// Уровень абстракции
// клиентский код подключает именно этот хедер

// тип значений в стеке
#include <cstddef>
#include "StackImplementation.h"
#include "ListStack.h"
#include "VectorStack.h"

enum class StackContainer {
	Vector = 0,
	List,
	// можно дополнять другими контейнерами
};

template <typename ValueType>
class Stack{
public:
	// Большая пятерка
	Stack(StackContainer container = StackContainer::Vector);
	// элементы массива последовательно подкладываются в стек
	Stack(const ValueType* valueArray, const size_t arraySize, 
		  StackContainer container = StackContainer::Vector);

	Stack(const Stack& copyStack);
	Stack& operator=(const Stack& copyStack);

	Stack(Stack&& moveStack) noexcept;
	Stack& operator=(Stack&& moveStack) noexcept;

	~Stack();

	// добавление в хвост
	void push(const ValueType& value);
	// удаление с хвоста
	void pop();
	// посмотреть элемент в хвосте
	const ValueType& top() const;
	// проверка на пустоту
	bool isEmpty() const;
	// размер 
	size_t size() const;
private:
	// указатель на имплементацию (уровень реализации)
	StackImplementation<ValueType>* _pimpl = nullptr;
	// тип контейнера, наверняка понадобится
	StackContainer _containerType;
};

#include <stdexcept>

template <typename ValueType>
Stack<ValueType>::Stack(StackContainer container)
        : _containerType(container)
{
    switch (container)
    {
        case StackContainer::List: {
            _pimpl = new ListStack<ValueType>();
            break;
        }
        case StackContainer::Vector: {
            _pimpl = new VectorStack<ValueType>();
            break;
        }
        default:
            throw std::runtime_error("Неизвестный тип контейнера");
    }
}

template <typename ValueType>
Stack<ValueType>::Stack(const ValueType* valueArray, const size_t arraySize, StackContainer container)
{
    switch (container)
    {
        case StackContainer::List: {
            _pimpl = new ListStack<ValueType>();
            break;
        }
        case StackContainer::Vector: {
            _pimpl = new VectorStack<ValueType>();
            break;
        }
        default:
            throw std::runtime_error("Неизвестный тип контейнера");
    }
    for(size_t i = 0; i < arraySize; ++i){
        _pimpl->push(valueArray[i]);
    }
}

template <typename ValueType>
Stack<ValueType>::Stack(const Stack& copyStack)
{
    delete(_pimpl);
    switch (copyStack._containerType) {
        case StackContainer::List: {
            _pimpl = new ListStack<ValueType>(*(dynamic_cast<ListStack<ValueType>*>(copyStack._pimpl)));
            break;
        }
        case StackContainer::Vector: {
            _pimpl = new VectorStack<ValueType>(
                    *(dynamic_cast<VectorStack<ValueType>*>(copyStack._pimpl)));
            break;
        }
    }
    _containerType = copyStack._containerType;
}

template <typename ValueType>
Stack<ValueType>& Stack<ValueType>::operator=(const Stack& copyStack)
{
    if(this == &copyStack){
        return *this;
    }
    delete(_pimpl);
    switch (copyStack._containerType) {
        case StackContainer::List: {
            _pimpl = new ListStack<ValueType>(*(dynamic_cast<ListStack<ValueType>*>(copyStack._pimpl)));
            break;
        }
        case StackContainer::Vector: {
            _pimpl = new VectorStack<ValueType>(
                    *(dynamic_cast<VectorStack<ValueType>*>(copyStack._pimpl)));
            break;
        }
    }
    _containerType = copyStack._containerType;
    return *this;
}

template <typename ValueType>
Stack<ValueType>::~Stack()
{
    delete _pimpl;
}

template <typename ValueType>
void Stack<ValueType>::push(const ValueType& value)
{
    _pimpl->push(value);
}

template <typename ValueType>
void Stack<ValueType>::pop()
{
    _pimpl->pop();
}

template <typename ValueType>
const ValueType& Stack<ValueType>::top() const
{
    return _pimpl->top();
}

template <typename ValueType>
bool Stack<ValueType>::isEmpty() const
{
    return _pimpl->isEmpty();
}

template <typename ValueType>
size_t Stack<ValueType>::size() const
{
    return _pimpl->size();
}

template <typename ValueType>
Stack<ValueType>::Stack(Stack &&moveStack) noexcept {
    delete(_pimpl);
    _pimpl = moveStack._pimpl;
    moveStack._pimpl = nullptr;
    _containerType = moveStack._containerType;

}

template <typename ValueType>
Stack<ValueType> &Stack<ValueType>::operator=(Stack &&moveStack) noexcept {
    delete(_pimpl);
    _pimpl = moveStack._pimpl;
    moveStack._pimpl = nullptr;
    _containerType = moveStack._containerType;
    return *this;
}


