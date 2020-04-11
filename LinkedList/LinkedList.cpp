#include "LinkedList.h"

#include <iostream>

LinkedList::Node::Node(const ValueType& value, Node* next)
{
	this->value = value;
	this->next = next;
}

LinkedList::Node::~Node()
{
	// ничего не удаляем, т.к. агрегация
}

void LinkedList::Node::insertNext(const ValueType& value)
{
	Node* newNode = new Node(value, this->next);
	this->next = newNode;
}

void LinkedList::Node::removeNext()
{
	Node* removeNode = this->next;
	Node* newNext = removeNode->next;
	this->next = newNext;
	removeNode->next = nullptr;
	delete removeNode;
}

void LinkedList::Node::insertNext(LinkedList::Node *node) {
    Node* tmp = this->next;
    this->next = node;
    node->next = tmp;
}

LinkedList::LinkedList()
	: _head(nullptr), _size(0)
{

}

LinkedList::LinkedList(const LinkedList& copyList)
{
	this->_size = copyList._size;
	if (this->_size == 0) {
		this->_head = nullptr;
		return;
	}

	this->_head = new Node(copyList._head->value);

	Node* currentNode = this->_head;
	Node* currentCopyNode = copyList._head;

	while (currentCopyNode->next) {
		currentNode->next = new Node(currentCopyNode->next->value);//fixed
		currentCopyNode = currentCopyNode->next;
		currentNode = currentNode->next;
	}
    currentNode->next = nullptr;//на всякий случай
}

LinkedList& LinkedList::operator=(const LinkedList& copyList){
	if (this == &copyList) {
		return *this;
	}
	forceNodeDelete(_head);//удалить весь текущий список
    this->_head = new Node(copyList._head->value);//копирование головы
    Node* currentNode = this->_head;
    Node* currentCopyNode = copyList._head;
    while (currentCopyNode->next) {
        currentNode->next = new Node(currentCopyNode->next->value);
        currentCopyNode = currentCopyNode->next;
        currentNode = currentNode->next;
    }
    currentNode->next = nullptr;//на всякий случай
	return *this;
}

LinkedList::LinkedList(LinkedList&& moveList) noexcept
{
	this->_size = moveList._size;
	this->_head = moveList._head;

	moveList._size = 0;
	moveList._head = nullptr;
}

LinkedList& LinkedList::operator=(LinkedList&& moveList) noexcept
{
	if (this == &moveList) {
		return *this;
	}
	forceNodeDelete(_head);
	this->_size = moveList._size;
	this->_head = moveList._head;

	moveList._size = 0;
	moveList._head = nullptr;

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

	Node* bufNode = this->_head;
	for (int i = 0; i < pos; ++i) {
		bufNode = bufNode->next;
	}

	return bufNode;
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
	else {
		Node* bufNode = this->_head;
		for (size_t i = 0; i < pos-1; ++i) {
			bufNode = bufNode->next;
		}
		bufNode->insertNext(value);
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
		//return;
	}
	else{
        insert(_size, value);
	}
}

void LinkedList::pushFront(const ValueType& value)
{
	_head = new Node(value, _head);
	++_size;
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
    else {
        Node* bufNode = this->_head;
        for (size_t i = 0; i < pos - 1; ++i) {
            bufNode = bufNode->next;
        }
        bufNode->removeNext();
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
    Node* bufNode = this->_head;
    for (size_t i = 0; i < _size; ++i) {
        bufNode = bufNode->next;
    }
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
        Node *tmp;
        for(int i = 0; i < _size; ++i){
            tmp = getNode(_size - 1);
            this->insert(i, tmp);
        }
        //иначе петля в последнем элементе
        tmp->next = nullptr;
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
    else {
        Node* bufNode = this->_head;
        for (size_t i = 0; i < pos - 1; ++i) {
            bufNode = bufNode->next;
        }
        bufNode->insertNext(node);

    }
}

void LinkedList::pushBackNode(LinkedList::Node *node) {
    if (_size == 0) {
        pushFrontNode(node);
        return;
    }
    insert(_size, node);
}

void LinkedList::pushFrontNode(LinkedList::Node *node) {
    Node *tmp = _head;
    _head = node;
    _head->next = tmp;
}






