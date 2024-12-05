#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

struct File
{
    string name{};
    uint32_t numberOfBytes{};
    uint32_t insertPos{};
    bool isWritable{};

    void setFromLine(string line)
    {
        stringstream sstream(line);
        sstream >> name;

        string rorw{};
        sstream >> rorw;
        if (rorw == "ro")
            isWritable = false;
        else
            isWritable= true;

        sstream >> numberOfBytes;
    }

    void printFile(ostream &file)
    {
        string writable = (isWritable) ? "rw":"ro";
        string byte = (numberOfBytes == 1) ? "byte":"bytes";
        file << insertPos << " " << name << " " << writable << " " << numberOfBytes << byte << endl;
    }

};

template <typename T>
struct Node
{
    Node* left{};
    Node* right{};

    T data{};
};

struct FileBinaryTree
{
    Node<File*>* root{};
    uint32_t currentPos{};

    void addFileElement(string line)
    {
        Node<File*>* newNode = new Node<File*>();
        newNode->data->setFromLine(line);
        if (root == nullptr)
        {
            root = newNode;
            currentPos++;
        }
        else
        {
            for (Node<File*>* cursor = root; cursor; cursor = (newNode->data->name <= cursor->data->name) ? cursor->left:cursor->right)
            {
                if (cursor->data->name == newNode->data->name)
                {
                    if (cursor->data->isWritable)
                    {
                        currentPos++;
                        cursor->data = newNode->data;
                        cursor->data->insertPos = currentPos;
                        return;
                    }
                    else return;
                }
                else if (newNode->data->name <= cursor->data->name && cursor->left == nullptr)
                {
                    currentPos++;
                    newNode->data->insertPos = currentPos;
                    cursor->left = newNode;
                    return;
                }
                else if (newNode->data->name > cursor->data->name && cursor->right == nullptr)
                {
                    currentPos++;
                    newNode->data->insertPos = currentPos;
                    cursor->right = newNode;
                    return;
                }
            }
        }
    }

};

void printTreeEPD(Node<File*>* root, ostream &file)
{
    Node<File*>* current = root;
    if (current->left != nullptr)
        printTreeEPD(current->left, file);
    else if (current->right != nullptr)
    {
        current->data->printFile(file);
        printTreeEPD(current->right, file);
    }
    else
        current->data->printFile(file);
}

int main(int argc, char* argv[])
{
    cout << "Quantidade de argumentos (argc): " << argc << endl;
    for (int i = 0; i < argc; i++) {
        cout << "Argumento " << i << " argv[" << i << "]: " << argv[i] << endl;
    }
    ifstream input("entrada.input");
    ofstream output("saida.output");

    if (!input.is_open() || !output.is_open()) {
        cerr << "Erro ao abrir os arquivos de entrada ou saida." << endl;
    }

    string temp{};
    uint32_t numberOfEntries{};

    getline(input, temp);
    numberOfEntries = stoi(temp);
    string line{};

    FileBinaryTree database{};
    for (uint32_t i = 0; i < numberOfEntries; i++)
    {
        getline(input, line);
        database.addFileElement(line);
    }

    printTreeEPD(database.root, output);
}
