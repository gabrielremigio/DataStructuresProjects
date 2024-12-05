#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

template<typename T>
struct Element {
    Element(T newData) : data(newData) {}

    Element<T>* next{};
    Element<T>* previous{};
    T data{};
};

template<typename T>
struct LinkedList {
    Element<T>* head{};
    uint32_t length{};

    void addToTail(T newElement) {
        Element<T>* ptr = new Element<T>(newElement);
        if (head == nullptr) {
            head = ptr;
            head->next = ptr;
            head->previous = ptr;
        } else {
            ptr->previous = head->previous;
            ptr->next = head;
            head->previous->next = ptr;
            head->previous = ptr;
        }
        length++;
    }

    bool isFound(T dt) {
        uint32_t i{};
        for (Element<T>* cursor = head; cursor && i < length; cursor = cursor->next, i++) {
            if (cursor->data == dt) {
                return true;
            }
        }
        return false;
    }

    Element<T>* findFromData(T dt) {
        uint32_t i{};
        for (Element<T>* cursor = head; cursor && i < length; cursor = cursor->next, i++) {
            if (cursor->data == dt) {
                return cursor;
            }
        }
        return nullptr;
    }

    bool removeFromData(T dt) {
        uint32_t i{};
        for (Element<T>* cursor = head; cursor && i < length; cursor = cursor->next, i++)
        {
            if (cursor->data == dt) {
                cursor->next->previous = cursor->previous;
                cursor->previous->next = cursor->next;
                if (cursor == head) {
                    head = cursor->next;
                }
                delete cursor;
                length--;
                return true;
            }
        }
        return false;
    }

    void printAll() {
        uint32_t i{};
        for (Element<T>* cursor = head; cursor && i < length; cursor = cursor->next, i++) {
            cout << cursor->data << endl;
        }
    }

    void interpretLine(string line, ostream& output) {
        if (line.find("ADD") != string::npos) {
            string name = line.substr(4);
            if (!isFound(name)) {
                addToTail(name);
                output << "[ OK  ] ADD " << name << endl;
            } else {
                output << "[ERROR] ADD " << name << endl;
            }
        } else if (line.find("SHOW") != string::npos) {
            string name = line.substr(5);
            if (isFound(name)) {
                Element<string>* element = findFromData(name);
                output << "[ OK  ] " << element->previous->data << "<-" << element->data << "->" << element->next->data << endl;
            } else {
                output << "[ERROR] ?<-" << name << "->?" << endl;
            }
        } else if (line.find("REMOVE") != string::npos) {
            string name = line.substr(7);
            if (removeFromData(name))
                output << "[ OK  ] " << line << endl;
            else
                output << "[ERROR] " << line << endl;
        } else {
            output << "Unknown command" << endl;
        }
    }
};

int main(int argc, char* argv[]) {
    // Exibindo a quantidade de argumentos
    cout << "Quantidade de argumentos (argc): " << argc << endl;
    // Iterando sobre o(s) argumento(s) do programa
    for(uint32_t i = 0; i < argc; i++)
    {
        // Mostrando o argumento i
        cout << "Argumento " << i << " argv[" << i << "]: " << argv[i] << endl;
    }
    ifstream input(argv[1]);
    ofstream output(argv[2]);

    if (!input.is_open() || !output.is_open()) {
        cerr << "Erro ao abrir os arquivos de entrada ou saída." << endl;
        return 1;
    }

    LinkedList<string> socialMedia;
    string line;

    while (getline(input, line)) {
        socialMedia.interpretLine(line, output);
    }

    input.close();
    output.close();

    return 0;
}
