#pragma once

// потом поменяем на шаблоны
#include <cstdlib>
#include <iostream>

template <typename ValueType>
class TwoLinkedList{
	class Node {
        public:
            Node(const ValueType& value, Node* next = nullptr, Node* prev = nullptr);
            ~Node();
            ValueType value;
            Node* next;
            Node* previous;
            friend class TwoLinkedList;
	    private:
            void insertNext(const ValueType& value);
            void insertNext(Node* node);
            void removeNext();
    };

public:
	////
	TwoLinkedList();
	TwoLinkedList(const TwoLinkedList& copyList);
	TwoLinkedList& operator=(const TwoLinkedList& copyList);

	TwoLinkedList(TwoLinkedList&& moveList) noexcept;
	TwoLinkedList& operator=(TwoLinkedList&& moveList) noexcept;

	~TwoLinkedList();
	////

	// доступ к значению элемента по индексу
	ValueType& operator[](const size_t pos) const;
	// доступ к узлу по индексу
	TwoLinkedList::Node* getNode(const size_t pos) const;
	
	// вставка элемента по индексу, сначала ищем, куда вставлять (О(n)), потом вставляем (O(1))
	void insert(const size_t pos, const ValueType& value);
	// вставка элемента после узла, (O(1))
	void insertAfterNode(Node* node, const ValueType& value);
	// вставка в конец (О(n))
	void pushBack(const ValueType& value);
	// вставка в начало (О(1))
	void pushFront(const ValueType& value);

	// удаление
	void remove(const size_t pos);//+
	void removeNextNode(Node* node);//+
	void removeFront();//+
	void removeBack();//+
	
	// поиск, О(n)
	long long int findIndex(const ValueType& value) const;//+
	Node* findNode(const ValueType& value) const;//+

	// разворот списка
	void reverse();						// изменение текущего списка
	TwoLinkedList reverse() const;			// полчение нового списка (для константных объектов)
	TwoLinkedList getReverseList() const;	// чтобы неконстантный объект тоже мог возвращать новый развернутый список

	size_t size() const;

/*Вспомогательные методы, НЕ МЕНЯЮТ РАЗМЕР СПИСКА*/
protected:
    void pushBackNode(Node *node);
    void pushFrontNode(Node *node);
    void insert(const size_t pos, Node *node);

private:
	Node* _head;
	Node* _tail;
	size_t	_size;

	void forceNodeDelete(Node* node);
};

template <typename ValueType>
TwoLinkedList<ValueType>::Node::Node(const ValueType& value, Node* next, Node* prev){
    this->value = value;
    this->previous = prev;
    this->next = next;
}


template <typename ValueType>
TwoLinkedList<ValueType>::Node::~Node(){}

template <typename ValueType>
void TwoLinkedList<ValueType>::Node::insertNext(const ValueType& value){
    Node* newNode = new Node(value, this->next, this);
    this->next = newNode;
}

template <typename ValueType>
void TwoLinkedList<ValueType>::Node::removeNext(){
    if(this->next){
        Node* removeNode = this->next;
        Node* newNext = removeNode->next;
        this->next = newNext;
        this->next->previous = this;
        delete removeNode;
    }
}

template <typename ValueType>
void TwoLinkedList<ValueType>::Node::insertNext(TwoLinkedList::Node *node) {
    Node* tmp = this->next;
    this->next = node;
    node->previous = this;
    node->next = tmp;
    tmp->previous = node;
}

template <typename ValueType>
TwoLinkedList<ValueType>::TwoLinkedList()
        : _head(nullptr), _size(0), _tail(nullptr){}


template <typename ValueType>
TwoLinkedList<ValueType>::TwoLinkedList(const TwoLinkedList& copyList){
    this->_size = copyList._size;
    if (this->_size == 0) {
        this->_head = nullptr;
        this->_tail = nullptr;
        return;
    }

    this->_head = new Node(copyList._head->value);
    Node* currentNode = this->_head;
    Node* currentCopyNode = copyList._head;

    while (currentCopyNode->next) {
        //копия текущей ноды
        currentNode->next = new Node(currentCopyNode->next->value);
        currentNode->next->previous = currentNode;
        //прокрутка
        currentCopyNode = currentCopyNode->next;
        currentNode = currentNode->next;
    }
    this->_tail = currentNode;
}
template <typename ValueType>
TwoLinkedList<ValueType>& TwoLinkedList<ValueType>::operator=(const TwoLinkedList& copyList){
    this->_size = copyList._size;
    if (this == &copyList) {
        return *this;
    }
    forceNodeDelete(_head);//удалить весь текущий список
    this->_head = new Node(copyList._head->value);//копирование головы
    Node* currentNode = this->_head;
    Node* currentCopyNode = copyList._head;
    while (currentCopyNode->next) {
        //копия текущей ноды
        currentNode->next = new Node(currentCopyNode->next->value);
        currentNode->next->previous = currentNode;
        //прокрутка
        currentCopyNode = currentCopyNode->next;
        currentNode = currentNode->next;
    }

    this->_tail = currentNode;
    return *this;
}

template <typename ValueType>
TwoLinkedList<ValueType>::TwoLinkedList(TwoLinkedList&& moveList) noexcept{
    this->_size = moveList._size;
    this->_head = moveList._head;
    this->_tail = moveList._tail;
    moveList._size = 0;
    moveList._head = nullptr;
    moveList._tail = nullptr;
}

template <typename ValueType>
TwoLinkedList<ValueType>& TwoLinkedList<ValueType>::operator=(TwoLinkedList&& moveList) noexcept{
    if (this == &moveList) {
        return *this;
    }
    forceNodeDelete(_head);
    this->_size = moveList._size;
    this->_head = moveList._head;
    this->_tail = moveList._tail;
    moveList._size = 0;
    moveList._head = nullptr;
    moveList._tail = nullptr;
    return *this;
}

template <typename ValueType>
TwoLinkedList<ValueType>::~TwoLinkedList(){
    forceNodeDelete(_head);
}

template <typename ValueType>
ValueType& TwoLinkedList<ValueType>::operator[](const size_t pos) const{
    return getNode(pos)->value;
}

template <typename ValueType>
typename TwoLinkedList<ValueType>::Node* TwoLinkedList<ValueType>::getNode(const size_t pos) const{
    if (pos < 0) {
        throw std::out_of_range("Index of required node is "
                                "out of range\n");
    }
    else if (pos >= this->_size) {
        throw std::out_of_range("Index of required node is"
                                "out of range\n");
    }
    else{
        Node* bufNode;
        if(pos > _size / 2){
            bufNode = this->_tail;
            for(size_t i = 0; i < _size - pos - 1; ++i){
                bufNode = bufNode->previous;
            }
        }
        else{
            bufNode = this->_head;
            for(size_t i = 0; i < pos; ++i){
                bufNode = bufNode->next;
            }
        }
        return bufNode;
    }
}

template <typename ValueType>
void TwoLinkedList<ValueType>::insert(const size_t pos, const ValueType& value){
    if (pos < 0) {
        throw std::out_of_range("Index of required position is "
                                "out of range\n");
    }
    else if (pos > this->_size) {
        throw std::out_of_range("Index of required position is "
                                "out of range\n");
    }

    if (pos == 0) {
        pushFront(value);
    }
    else if(pos == _size){
        pushBack(value);
    }
    else {
        getNode(pos - 1)->insertNext(value);
        ++_size;
    }
}

template <typename ValueType>
void TwoLinkedList<ValueType>::insertAfterNode(Node* node, const ValueType& value){
    node->insertNext(value);
    ++_size;
}

template <typename ValueType>
void TwoLinkedList<ValueType>::pushBack(const ValueType& value){
    if (_size == 0) {
        pushFront(value);
    }
    else{
        _tail->next = new Node(value);
        _tail->next->previous = _tail;
        _tail = _tail->next;
        ++_size;
    }
}

template <typename ValueType>
void TwoLinkedList<ValueType>::pushFront(const ValueType& value){
    ++_size;
    if(_size == 1){
        _head = new Node(value, _head);
        _tail = _head;
    }
    else{
        Node* bufNode = _head;
        _head = new Node(value);
        _head->next = bufNode;
        _head->next->previous = _head;
    }
}

template <typename ValueType>
void TwoLinkedList<ValueType>::remove(const size_t pos){
    if (pos < 0) {
        throw std::out_of_range("Index of required position is "
                                "out of range\n");
    }
    else if (pos >= this->_size) {
        throw std::out_of_range("Index of required position is "
                                "out of range\n");
    }

    if (pos == 0) {
        removeFront();
    }
    else if (pos == _size - 1){
        removeBack();
    }
    else {
        getNode(pos - 1)->removeNext();
        --_size;
    }
}

template <typename ValueType>
void TwoLinkedList<ValueType>::removeNextNode(Node* node){
    node->removeNext();
}

template <typename ValueType>
void TwoLinkedList<ValueType>::removeFront(){
    Node* bufNode = _head->next;
    delete _head;
    _head = bufNode;
    --_size;
}

template <typename ValueType>
void TwoLinkedList<ValueType>::removeBack() {
    Node* bufNode = _tail;
    _tail = bufNode->previous;
    _tail->next = nullptr;
    delete bufNode;
    --_size;
}
template <typename ValueType>
long long int TwoLinkedList<ValueType>::findIndex(const ValueType& value) const{
    Node* bufNode = this->_head;
    for (size_t i = 0; i < _size; ++i) {
        if(bufNode->value == value)
            return i;
        bufNode = bufNode->next;
    }
    return -1;
}
template <typename ValueType>
typename TwoLinkedList<ValueType>::Node* TwoLinkedList<ValueType>::findNode(const ValueType& value) const{
    Node* bufNode = this->_head;
    for (size_t i = 0; i < _size; ++i) {
        if(bufNode->value == value)
            return bufNode;
        bufNode = bufNode->next;
    }
    return nullptr;
}

template <typename ValueType>
void TwoLinkedList<ValueType>::reverse(){
    if(_size > 1){
        Node* bufNode = _tail;
        //Поменяем местами хвост и голову
        _tail->next = _tail->previous;
        _tail->previous = nullptr;
        _tail = _head;
        _head = bufNode;
        while(bufNode->next){
            bufNode->next->next = bufNode->next->previous;
            bufNode->next->previous = bufNode;
            bufNode = bufNode->next;
        }
    }
}

template <typename ValueType>
TwoLinkedList<ValueType> TwoLinkedList<ValueType>::reverse() const{
    TwoLinkedList result = *this;
    result.reverse();
    return result;
}

template <typename ValueType>
TwoLinkedList<ValueType> TwoLinkedList<ValueType>::getReverseList() const{
    TwoLinkedList result = *this;
    result.reverse();
    return result;
}

template <typename ValueType>
size_t TwoLinkedList<ValueType>::size() const{
    return _size;
}

template <typename ValueType>
void TwoLinkedList<ValueType>::forceNodeDelete(Node* node){
    if (node == nullptr) {
        return;
    }

    Node* nextDeleteNode = node->next;
    delete node;
    forceNodeDelete(nextDeleteNode);
}


template <typename ValueType>
void TwoLinkedList<ValueType>::insert(const size_t pos, TwoLinkedList::Node *node) {
    if (pos < 0) {
        throw std::out_of_range("Index of required position is "
                                "out of range\n");
    }
    else if (pos > this->_size) {
        throw std::out_of_range("Index of required position is "
                                "out of range\n");
    }

    if (pos == 0) {
        pushFrontNode(node);
    }
    else if(pos == _size){
        pushBackNode(node);
    }
    else {
        getNode(pos - 1)->insertNext(node);
    }
}

template <typename ValueType>
void TwoLinkedList<ValueType>::pushBackNode(TwoLinkedList::Node *node) {
    if (_size == 0) {
        pushFrontNode(node);
    }
    else{
        _tail->next = node;
        node->previous = _tail;
        node->next = nullptr;
        _tail = node;
    }
}

template <typename ValueType>
void TwoLinkedList<ValueType>::pushFrontNode(TwoLinkedList::Node *node) {
    Node *tmp = _head;
    _head = node;
    _head->next = tmp;
    _head->previous = nullptr;
}




