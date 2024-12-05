#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

struct Document
{
    string name;
    uint32_t numberOfPages;

    void writeDocumentToFile(ostream &file)
    {
        file << name << "-" << numberOfPages << "p" << endl;
    }
};

template<typename T>
struct Node
{
    Node<T>(T newData) : data(newData){}

    Node<T> *next{};
    T data{};
};

template<typename T>
struct Queue
{
    Node<T> *start{};
    Node<T> *end{};

    void pushBack(T newData)
    {
        Node<T> *newNode = new Node<T>(newData);
        if (start == nullptr)
        {
            start = newNode;
            end = newNode;
        }
        else
        {
            end->next = newNode;
            end = newNode;
        }
    }

    void popFront()
    {
        Node<T> *temp = start;
        delete(start);
        start = temp->next;
        delete(temp);
    }

    void printQueueToFile(ostream &file)
    {
        for (Node<Document> *cursor = start; cursor; cursor = cursor->next)
            if (cursor != end)
            {
                file << " " << cursor->data.name << "-" << cursor->data.numberOfPages << "p,";
            }
            else
            {
                file << " " << cursor->data.name << "-" << cursor->data.numberOfPages << "p" << endl;
            }
    }
};

template<typename T>
struct Stack
{
    Node<T> *top{};
    uint32_t length{};

    void pileUp(T newData)
    {
        Node<T> *newNode = new Node<T>(newData);
        if (top == nullptr)
        {
            top = newNode;
        }
        else
        {
            newNode->next = top;
            top = newNode;
        }
    }

    void pileDown()
    {
        Node<T> *temp = top;
        delete(top);
        top = temp->next;
        delete(temp);
    }

    bool isEmpty()
    {
        return top == nullptr;
    }
};

struct Printer
{
    static Stack<Document*>* printedStack;
    string name{};
    uint32_t numberOfPages{};
    bool isUseless = false;

    Stack<Document*> printedDocuments{};
};

Document* lineToDocument(string line)
{
    string name;
    uint32_t pages;
    istringstream str(line);
    str >> name;
    str >> pages;
    return new Document{name, pages};
}

Printer *findIdlePrinter(Printer *printers[], uint32_t numOfPrinters)
{
    Printer *minAdress = printers[0];
    for (uint32_t i = 0; i < numOfPrinters; i++)
    {
        if (printers[i]->numberOfPages < minAdress->numberOfPages)
        {
            minAdress = printers[i];
        }
    }
    return minAdress;
}

Printer *findNotUselessIdlePrinter(Printer *printers[], uint32_t numOfPrinters)
{
    Printer *minAdress = printers[0];
    for (uint32_t i = 0; i < numOfPrinters; i++)
    {
        if (minAdress->isUseless)
        {
            minAdress = printers[i];
            continue;
        }
        if (printers[i]->isUseless) continue;
        if (printers[i]->printedDocuments.top->data->numberOfPages < minAdress->printedDocuments.top->data->numberOfPages)
        {
            minAdress = printers[i];
        }
    }
    return minAdress;
}

void printDocumentsStack(Stack<Document*> stack, ostream &file)
{
    for (Node<Document*> *cursor = stack.top; cursor; cursor = cursor->next)
    {
        if (cursor->next != nullptr)
        {
            file << " " << cursor->data->name << "-" << cursor->data->numberOfPages << "p,";
        }
        else
        {
            file << " " << cursor->data->name << "-" << cursor->data->numberOfPages << "p" << endl;
        }
    }
}

void printDocumentsInOrder(Stack<Document*>* stack, ostream &file)
{
    for (Node<Document*>* cursor = stack->top; cursor; cursor = cursor->next)
    {
        file << cursor->data->name << " " << cursor->data->numberOfPages << endl;
    }
}

Stack<Document*>* Printer::printedStack;
int main(int argc, char *argv[])
{
    cout << "Quantidade de argumentos (argc): " << argc << endl;
    for (int i = 0; i < argc; i++)
    {
        cout << "Argumento " << i << " argv[" << i << "]: " << argv[i] << endl;
    }
    ifstream input("entrada.txt");
    ofstream output("saida.txt");

    if (!input.is_open() || !output.is_open())
    {
        cerr << "Erro ao abrir os arquivos de entrada ou saida." << endl;
    }

    string temp1;
    uint32_t numberOfPrinters{};
    getline(input, temp1);
    numberOfPrinters = stoi(temp1);

    Printer *printers[numberOfPrinters]{};
    for (uint32_t i = 0; i < numberOfPrinters; i++)
    {
        Printer *newPrinter = new Printer();
        getline(input, newPrinter->name);
        printers[i] = newPrinter;
    }

    string temp2;
    uint32_t numberOfDocuments{};
    getline(input, temp2);
    numberOfDocuments = stoi(temp2);

    Printer::printedStack = new Stack<Document*>;
    uint32_t totalNumberOfPages{};
    string line{};
    Printer *idle{};
    for (uint32_t i = 0; i < numberOfDocuments; i++)
    {
        getline(input, line);
        Document* newDocument = lineToDocument(line);
        totalNumberOfPages += newDocument->numberOfPages;
        idle = findIdlePrinter(printers, numberOfPrinters);
        if (!idle->printedDocuments.isEmpty())
        {
            Printer::printedStack->pileUp(idle->printedDocuments.top->data);
        }
        idle->numberOfPages += newDocument->numberOfPages;
        output << "[" << idle->name << "]";
        idle->printedDocuments.pileUp(newDocument);
        printDocumentsStack(idle->printedDocuments, output);
    }
    for (uint32_t i = 0; i < numberOfPrinters; i++)
    {
        idle = findNotUselessIdlePrinter(printers, numberOfPrinters);
        Printer::printedStack->pileUp(idle->printedDocuments.top->data);
        idle->isUseless = true;
    }
    output << totalNumberOfPages << "p" << endl;
    printDocumentsInOrder(Printer::printedStack, output);

    input.close();
    output.close();
}
