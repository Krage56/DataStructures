#include "Two-LinkedList.h"

#include <iostream>

LinkedList::Node::Node(const ValueType& value, Node* next, Node* prev)
{
	this->value = value;
    this->previous = prev;
	this->next = next;
}

LinkedList::Node::~Node()
{
	// ничего не удаляем, т.к. агрегация
}

void LinkedList::Node::insertNext(const ValueType& value)
{
	Node* newNode = new Node(value, this->next, this);
	this->next = newNode;
}

void LinkedList::Node::removeNext()
{
	Node* removeNode = this->next;
	Node* newNext = removeNode->next;
	this->next = newNext;
	this->next->previous = this;
	delete removeNode;
}

void LinkedList::Node::insertNext(LinkedList::Node *node) {
    Node* tmp = this->next;
    this->next = node;
    node->previous = this;
    node->next = tmp;
    tmp->previous = node;
}

LinkedList::LinkedList()
	: _head(nullptr), _size(0), _tail(nullptr)
{

}

LinkedList::LinkedList(const LinkedList& copyList)
{
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

LinkedList& LinkedList::operator=(const LinkedList& copyList){
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

LinkedList::LinkedList(LinkedList&& moveList) noexcept
{
	this->_size = moveList._size;
	this->_head = moveList._head;
    this->_tail = moveList._tail;
	moveList._size = 0;
	moveList._head = nullptr;
    moveList._tail = nullptr;
}

LinkedList& LinkedList::operator=(LinkedList&& moveList) noexcept
{
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

LinkedList::~LinkedList()
{
	forceNodeDelete(_head);
}

ValueType& LinkedList::operator[](const size_t pos) const
{
	return getNode(pos)->value;
}

LinkedList::Node* LinkedList::getNode(const size_t pos) const
{
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

void LinkedList::insert(const size_t pos, const ValueType& value)
{
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

void LinkedList::insertAfterNode(Node* node, const ValueType& value)
{
	node->insertNext(value);
}

void LinkedList::pushBack(const ValueType& value)
{
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

void LinkedList::pushFront(const ValueType& value)
{
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

void LinkedList::remove(const size_t pos){
    if (pos < 0) {
        throw std::out_of_range("Index of required position is "
                                "out of range\n");
    }
    else if (pos > this->_size) {
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

void LinkedList::removeNextNode(Node* node){
    node->removeNext();
}

void LinkedList::removeFront(){
    Node* bufNode = _head->next;
    delete _head;
    _head = bufNode;
    --_size;
}

void LinkedList::removeBack() {
    Node* bufNode = _tail;
    _tail = bufNode->previous;
    _tail->next = nullptr;
    delete bufNode;
    --_size;
}
long long int LinkedList::findIndex(const ValueType& value) const
{
    Node* bufNode = this->_head;
    for (size_t i = 0; i < _size; ++i) {
        if(bufNode->value == value)
            return i;
        bufNode = bufNode->next;
    }
	return -1;
}

LinkedList::Node* LinkedList::findNode(const ValueType& value) const
{
    Node* bufNode = this->_head;
    for (size_t i = 0; i < _size; ++i) {
        if(bufNode->value == value)
            return bufNode;
        bufNode = bufNode->next;
    }
	return nullptr;
}

void LinkedList::reverse(){
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

LinkedList LinkedList::reverse() const
{
    LinkedList result = *this;
    result.reverse();
	return result;
}

LinkedList LinkedList::getReverseList() const{
    LinkedList result = *this;
    result.reverse();
    return result;
}

size_t LinkedList::size() const
{
	return _size;
}

void LinkedList::forceNodeDelete(Node* node)
{
	if (node == nullptr) {
		return;
	}

	Node* nextDeleteNode = node->next;
	delete node;
	forceNodeDelete(nextDeleteNode);
}

void LinkedList::insert(const size_t pos, LinkedList::Node *node) {
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

void LinkedList::pushBackNode(LinkedList::Node *node) {
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

void LinkedList::pushFrontNode(LinkedList::Node *node) {
    Node *tmp = _head;
    _head = node;
    _head->next = tmp;
    _head->previous = nullptr;
}






