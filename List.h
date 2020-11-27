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
            
            Node<T>* nextElement = head;

            while(nextElement->next != NULL)
            {
                nextElement = nextElement->next;
            }

            nextElement->next = newElement;
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

        T findByIndex(int i)
        {
            Node<T>* currentElement = head;
            int currentNum = 0;

            while(currentElement && currentNum != i)
            {
                currentNum++;

                currentElement = currentElement->next;
            }

            if (i == currentNum)
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

/*int main()
{
    List<int> testList;

    testList.append(3);
    testList.append(4); 
    testList.append(5);

    std::cout << *(testList.findByIndex(1)) << std::endl;
    //std::cout << testList.head->value << std::endl;
    //std::cout << testList.popFront() << std::endl;
    
    return 0;
}*/
#endif