#pragma once
#include "StackImplementation.h"
#include "../Vector/MyVector.h"
#include <iostream>

template <class ValueType>
class VectorStack : public MyVector<ValueType>, virtual public StackImplementation<ValueType>
{
public:
    //конструктор
    VectorStack();
    //конструктор копированием
    VectorStack(const VectorStack& copyVec);
    VectorStack& operator=(const VectorStack& copyVec);
    // Конструктор копирования присваиванием
    VectorStack(VectorStack&& moveVec) noexcept;
    VectorStack& operator=(VectorStack&& moveVec) noexcept;
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
    ~VectorStack();

};

template <typename ValueType>
VectorStack<ValueType>::VectorStack()
:MyVector<ValueType>(0, ResizeStrategy::Multiplicative, 2){}

template <typename ValueType>
VectorStack<ValueType>::~VectorStack() {}

template <typename ValueType>
VectorStack<ValueType>::VectorStack(const VectorStack &copyVec):MyVector<ValueType>(copyVec){}

template <typename ValueType>
VectorStack<ValueType>& VectorStack<ValueType>::operator=(const VectorStack &copyVec){
    if(this == &copyVec){
        return *this;
    }
    MyVector<ValueType>::operator=(copyVec);
    return *this;
}

template <typename ValueType>
VectorStack<ValueType>::VectorStack(VectorStack &&moveVec) noexcept :MyVector<ValueType>(std::move(moveVec)){}

template <typename ValueType>
VectorStack<ValueType> &VectorStack<ValueType>::operator=(VectorStack &&moveVec) noexcept {
    if(this == &moveVec){
        return *this;
    }
    MyVector<ValueType>::operator=(std::move(moveVec));
    return *this;
}

template <typename ValueType>
void VectorStack<ValueType>::push(const ValueType &value) {
    (*this).pushBack(value);
}

template <typename ValueType>
void VectorStack<ValueType>::pop() {
    if(this->size()){
        (*this).popBack();
    }
}

template <typename ValueType>
const ValueType &VectorStack<ValueType>::top() const {
    if(this->MyVector<ValueType>::size()){
        return (*this)[this->MyVector<ValueType>::size() - 1];
    }
}

template <typename ValueType>
bool VectorStack<ValueType>::isEmpty() const {
    return this->MyVector<ValueType>::size() == 0;
}

template <typename ValueType>
size_t VectorStack<ValueType>::size() const {
    return MyVector<ValueType>::size();
}



