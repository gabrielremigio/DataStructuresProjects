#include <iostream>
#include <cstdint>
#include <fstream>

using namespace std;
struct Element
{
    Element* next{};
    Element* previous{};

    uint32_t data{};

    void insertAfterThis(Element* newElement)
    {
        newElement->previous = this;
        newElement->next = next;
        next = newElement;
    }

    void insertBeforeThis (Element* newElement)
    {
        newElement->next = this;
        newElement->previous = previous;
        previous = newElement;
    }

    void remove()
    {
        this->previous->next = this->next;
        this->next->previous = this->previous;

    }
};

int main()
{
    Element data1, data2, data3;

    data1.data = 1;
    data2.data = 2;
    data3.data = 3;

    data1.insertAfterThis(&data2);
    data2.insertAfterThis(&data3);
    // data1.previous = &data3;
    // data3.next = &data1;


    for (Element* current = &data1; current; current = current->next)
    {
        cout << current->data << endl;
    }

    cout << endl;

    data2.remove();

    for (Element* current = &data1; current; current = current->next)
    {
        cout << current->data << endl;
    }
}