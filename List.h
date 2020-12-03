#ifndef List_H
#define List_H
#include <assert.h>
#include <iostream>

template <typename T>
struct Node
{
    Node<T>* next = NULL;
    T value;
};

template <typename T>
class List
{
    private:

        Node<T>* head = NULL;
    public:

        List(){}

        void append(T value)
        {   
            Node<T>* newElement = new Node<T>;

            newElement->next = NULL;
            newElement->value = value;

            if (head == NULL)
            {
                head = newElement;

                return;
            }
            
            Node<T>* currentElement = head;

            while(currentElement->next != NULL)
            {
                currentElement = currentElement->next;
            }

            currentElement->next = newElement;
        }

        bool contains(T value) 
        {
            assert(head);

            Node<T>* currentElement = head;

            while(currentElement)
            {
                if (currentElement->value == value)
                {
                    return true;
                }

                currentElement = currentElement->next;
            }

            return false;
        }

        void operator+=(List<T> list)
        {
            if (head == NULL)
            {
                head = list.head;

                return;
            }
            
            Node<T>* currentElement = head;

            while(currentElement->next != NULL)
            {
                currentElement = currentElement->next;
            }

            currentElement->next = list.head;
        }

        void pushFront (T value)
        {
            Node<T>* newElement = new Node<T>;

            newElement->next = NULL;
            newElement->value = value;

            if (head == NULL)
            {
                head = newElement;

                return;
            }

            newElement->next = head;
            head = newElement;
        }

        void removeByValue(T value)
        {
            assert(head);

            if (not contains(value))
            {
                return;
            }

            if (head->value == value)
            {
                popFront();

                return;
            }

            Node<T>* currentElement = head;

            while(currentElement->next->value != value)
            {
                currentElement = currentElement->next;
            }

            Node<T>* newNext = currentElement->next->next;
            
            delete currentElement->next;

            currentElement->next = newNext;
        }

        void removeByIndex(int index)
        {
            assert(head);

            if (index >= length())
            {
                return;
            }

            if (index == 0)
            {
                popFront();

                return;
            }

            Node<T>* currentElement = head;
            int currentNum = 1;

            while(currentElement && currentNum != index)
            {
                currentNum++;

                currentElement = currentElement->next;
            }

            Node<T>* newNext = currentElement->next->next;
        
            delete currentElement->next;

            currentElement->next = newNext;
        }

        T popFront()
        {
            assert(head);

            Node<T>* newHead = head->next;
            T value = head->value;

            delete head;

            head = newHead;

            return value;
        }

        T operator[](int index)
        {
            Node<T>* currentElement = head;
            int currentNum = 0;

            while(currentElement && currentNum != index)
            {
                currentNum++;

                currentElement = currentElement->next;
            }

            if (index == currentNum)
            {
                T returnedValue = currentElement->value;
                return returnedValue;
            }

            return 0;
        }

        int length()
        {   
            int len = 0;

            Node<T>* currentElement = head;

            while(currentElement)
            {
                len++;

                currentElement = currentElement->next;
            }

            return len;
        }
};

#endif