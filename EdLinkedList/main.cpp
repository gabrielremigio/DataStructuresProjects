#include <iostream>
#include <cstdint>
#include <fstream>

using namespace std;
struct Element
{
    Element* next{};
    Element* previous{};

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
    char name[51];
};

int main()
{
    ifstream input ("entrada.txt");
    ofstream output ("saida.txt");

    char fileLine[58];
    while(input >> fileLine)
    {

    }


    input.close();
    output.close();
}