#pragma once

// потом поменяем на шаблоны
#include <cstdlib>

using ValueType = double;

class LinkedList{
	// класс узла списка
	// по своей сути, может содержать любые данные,
	// можно реализовать и ассоциативный массив, просто добавив 
	// поле с ключем в узел и, с учетом этого, поменять методы LinkedList 
	// (доступ по ключу, поиск по ключу и т.д.)
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

