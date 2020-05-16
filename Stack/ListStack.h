#pragma once

#include "StackImplementation.h"

#include "../LinkedList/LinkedList.h"
template <typename ValueType>
class ListStack: virtual public StackImplementation<ValueType>,
        public LinkedList<ValueType>
{
public:
    ListStack();
    explicit ListStack(const ListStack& copyList);
    ListStack& operator=(const ListStack& copyVec);

    // Конструктор копирования присваиванием
    ListStack(ListStack&& moveVec) noexcept;
    ListStack& operator=(ListStack&& moveVec) noexcept;
    // добавление в конец
    void push(const ValueType& value) override;
    // удаление с хвоста
    void pop() override;
    // посмотреть элемент в хвосте
    const ValueType& top() const override;
    // проверка на пустоту
    bool isEmpty() const override;
    // размер
    size_t size() const override;
    // деструктор
    ~ListStack()override ;

};

template <typename ValueType>
ListStack<ValueType>::ListStack():
        LinkedList<ValueType>(){}

template <typename ValueType>
ListStack<ValueType>::~ListStack() {}

template <typename ValueType>
ListStack<ValueType>::ListStack(const ListStack<ValueType> &copyList):LinkedList<ValueType>(copyList){}

template <typename ValueType>
ListStack<ValueType> &ListStack<ValueType>::operator=(const ListStack<ValueType> &copyList) {
    if(this == &copyList){
        return *this;
    }
    LinkedList<ValueType>::operator=(copyList);
    return *this;
}

template <typename ValueType>
ListStack<ValueType>::ListStack(ListStack<ValueType> &&moveList) noexcept:LinkedList<ValueType>(std::move(moveList)){}

template <typename ValueType>
ListStack<ValueType> &ListStack<ValueType>::operator=(ListStack<ValueType> &&moveList) noexcept {
    if(this == &moveList){
        return *this;
    }
    LinkedList<ValueType>::operator=(std::move(moveList));
    return *this;
}

template <typename ValueType>
void ListStack<ValueType>::push(const ValueType &value) {
    (*this).pushBack(value);
}

template <typename ValueType>
void ListStack<ValueType>::pop() {
    this->removeBack();
}

template <typename ValueType>
const ValueType &ListStack<ValueType>::top() const {
    return (*this)[this->size() - 1];
}

template <typename ValueType>
bool ListStack<ValueType>::isEmpty() const {
    return this->LinkedList<ValueType>::size() == 0;
}

template <typename ValueType>
size_t ListStack<ValueType>::size() const {
    return LinkedList<ValueType>::size();
}
