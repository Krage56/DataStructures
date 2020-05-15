#pragma once

// потом поменяем на шаблоны
#include <cstdlib>
#include <stdexcept>

//using ValueType = double;
template <typename ValueType>
class LinkedList{
    class Node{
        public:
            Node(const ValueType& value, Node* next = nullptr);
            ~Node();
            ValueType value;
            Node* next;
            friend class LinkedList;
        private:
            void insertNext(const ValueType& value);
            void insertNext(Node* node);
            void removeNext();
    };

public:
	////
	LinkedList();
	LinkedList(const LinkedList& copyList);
	LinkedList& operator=(const LinkedList& copyList);

	LinkedList(LinkedList&& moveList) noexcept;
	LinkedList& operator=(LinkedList&& moveList) noexcept;

	~LinkedList();
	////

	// доступ к значению элемента по индексу
	ValueType& operator[](const size_t pos) const;
	// доступ к узлу по индексу
	LinkedList::Node* getNode(const size_t pos) const;
	
	// вставка элемента по индексу, сначала ищем, куда вставлять (О(n)), потом вставляем (O(1))
	void insert(const size_t pos, const ValueType& value);
	// вставка элемента после узла, (O(1))
	void insertAfterNode(Node* node, const ValueType& value);
	// вставка в конец (О(n))
	void pushBack(const ValueType& value);
	// вставка в начало (О(1))
	void pushFront(const ValueType& value);

	// удаление
	/*Отсюда пишу сам*/
	void remove(const size_t pos);//+
	void removeNextNode(Node* node);//+
	void removeFront();//+
	void removeBack();//+
	
	// поиск, О(n)
	long long int findIndex(const ValueType& value) const;//+
	Node* findNode(const ValueType& value) const;//+

	// разворот списка
	void reverse();						// изменение текущего списка
	LinkedList reverse() const;			// полчение нового списка (для константных объектов)
	LinkedList getReverseList() const;	// чтобы неконстантный объект тоже мог возвращать новый развернутый список

	size_t size() const;

/*Вспомогательные методы, НЕ МЕНЯЮТ _SIZE СПИСКА*/
protected:
    void pushBackNode(Node *node);
    void pushFrontNode(Node *node);
    void insert(const size_t pos, Node *node);

private:
	Node*	_head;
	size_t	_size;

	void forceNodeDelete(Node* node);
};

template<typename ValueType>
LinkedList<ValueType>::Node::Node(const ValueType& value, Node* next)
{
    this->value = value;
    this->next = next;
}

template<typename ValueType>
LinkedList<ValueType>::Node::~Node(){}

template<typename ValueType>
void LinkedList<ValueType>::Node::insertNext(const ValueType& value)
{
    Node* newNode = new Node(value, this->next);
    this->next = newNode;
}

template<typename ValueType>
void LinkedList<ValueType>::Node::removeNext()
{
    if(this->next){
        Node* removeNode = this->next;
        Node* newNext = removeNode->next;
        this->next = newNext;
        removeNode->next = nullptr;
        delete removeNode;
    }
}

template<typename ValueType>
void LinkedList<ValueType>::Node::insertNext(LinkedList::Node *node) {
    Node* tmp = this->next;
    this->next = node;
    node->next = tmp;
}

template<typename ValueType>
LinkedList<ValueType>::LinkedList()
        : _head(nullptr), _size(0)
{

}

template<typename ValueType>
LinkedList<ValueType>::LinkedList(const LinkedList& copyList)
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

template<typename ValueType>
LinkedList<ValueType>& LinkedList<ValueType>::operator=(const LinkedList& copyList){
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
    _size = copyList._size;
    return *this;
}

template<typename ValueType>
LinkedList<ValueType>::LinkedList(LinkedList&& moveList) noexcept
{
    this->_size = moveList._size;
    this->_head = moveList._head;

    moveList._size = 0;
    moveList._head = nullptr;
}
template<typename ValueType>
LinkedList<ValueType>& LinkedList<ValueType>::operator=(LinkedList&& moveList) noexcept
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
template<typename ValueType>
LinkedList<ValueType>::~LinkedList()
{
    forceNodeDelete(_head);
}
template<typename ValueType>
ValueType& LinkedList<ValueType>::operator[](const size_t pos) const
{
    return getNode(pos)->value;
}
template<typename ValueType>
typename LinkedList<ValueType>::Node* LinkedList<ValueType>::getNode(const size_t pos) const
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

template<typename ValueType>
void LinkedList<ValueType>::insert(const size_t pos, const ValueType& value)
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

template<typename ValueType>
void LinkedList<ValueType>::insertAfterNode(Node* node, const ValueType& value)
{
    node->insertNext(value);
    this->_size += 1;
}

template<typename ValueType>
void LinkedList<ValueType>::pushBack(const ValueType& value)
{
    if (_size == 0) {
        pushFront(value);
        //return;
    }
    else{
        insert(_size, value);
    }
}

template<typename ValueType>
void LinkedList<ValueType>::pushFront(const ValueType& value)
{
    _head = new Node(value, _head);
    ++_size;
}

template<typename ValueType>
void LinkedList<ValueType>::remove(const size_t pos){
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
    else {
        Node* bufNode = this->_head;
        for (size_t i = 0; i < pos - 1; ++i) {
            bufNode = bufNode->next;
        }
        bufNode->removeNext();
        --_size;
    }
}

template<typename ValueType>
void LinkedList<ValueType>::removeNextNode(Node* node){
    node->removeNext();
    --_size;
}

template<typename ValueType>
void LinkedList<ValueType>::removeFront(){
    if(_size){
        Node* bufNode = _head->next;
        delete _head;
        _head = bufNode;
        --_size;
    }
}

template<typename ValueType>
void LinkedList<ValueType>::removeBack() {
    if(_size > 1){
        getNode(_size - 2)->removeNext();
        --_size;
    }
    else{
        removeFront();
    }
}
template<typename ValueType>
long long int LinkedList<ValueType>::findIndex(const ValueType& value) const
{
    Node* bufNode = this->_head;
    for (size_t i = 0; i < _size; ++i) {
        if(bufNode->value == value)
            return i;
        bufNode = bufNode->next;
    }
    return -1;
}
template<typename ValueType>
typename LinkedList<ValueType>::Node* LinkedList<ValueType>::findNode(const ValueType& value) const
{
    Node* bufNode = this->_head;
    for (size_t i = 0; i < _size; ++i) {
        if(bufNode->value == value)
            return bufNode;
        bufNode = bufNode->next;
    }
    return nullptr;
}

template<typename ValueType>
void LinkedList<ValueType>::reverse(){
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

template<typename ValueType>
LinkedList<ValueType> LinkedList<ValueType>::reverse() const
{
    LinkedList result = *this;
    result.reverse();
    return result;
}

template<typename ValueType>
LinkedList<ValueType> LinkedList<ValueType>::getReverseList() const{
    LinkedList result = *this;
    result.reverse();
    return result;
}

template<typename ValueType>
size_t LinkedList<ValueType>::size() const
{
    return _size;
}

template<typename ValueType>
void LinkedList<ValueType>::forceNodeDelete(Node* node)
{
    if (node == nullptr) {
        return;
    }

    Node* nextDeleteNode = node->next;
    delete node;
    forceNodeDelete(nextDeleteNode);
}

template<typename ValueType>
void LinkedList<ValueType>::insert(const size_t pos, LinkedList::Node *node) {
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

template<typename ValueType>
void LinkedList<ValueType>::pushBackNode(LinkedList::Node *node) {
    if (_size == 0) {
        pushFrontNode(node);
        return;
    }
    insert(_size, node);
}
template<typename ValueType>
void LinkedList<ValueType>::pushFrontNode(LinkedList::Node *node) {
    Node *tmp = _head;
    _head = node;
    _head->next = tmp;
}
