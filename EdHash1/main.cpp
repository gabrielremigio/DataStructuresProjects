#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>

using namespace std;


uint32_t checksum(string key)
{
    uint32_t ans = int(key[0]);
    for (uint32_t i = 1; i < key.length(); i++)
    {
        if (key[i] == ' ') continue;
        else
        {
            ans = ans ^ key[i];
        }
    }
    return ans;
}

struct Server
{
    uint32_t capacity{};
    string** slots;

    Server(uint32_t size)
    {
        capacity = size;
        slots = new string*[size];
        for (uint32_t i = 0; i < capacity; i++)
        {
            slots[i] = nullptr;
        }
    }

    bool isNotFull()
    {
        for (uint32_t i = 0; i < capacity; i++)
        {
            if (slots[i] == nullptr) return true;
        }
        return false;
    }

    void add(string* value)
    {
        for (uint32_t i = 0; i < capacity; i++)
        {
            if (slots[i] == nullptr)
            {
                slots[i] = value;
                return;
            }
        }
    }

    void printServer(ostream &file)
    {
        for (uint32_t i = 0; i < capacity; i++)
        {
            if (slots[i]->empty() || i == capacity)
            {
                file << "\n";
                break;
            }
            else if (i == 0)
            {
                file << *slots[i];
            }
            else
            {
                file << ", " << slots[i];
            }
        }
    }
};


struct Hashtable
{
    uint32_t size{};
    Server** table{};

    Hashtable(uint32_t space, uint32_t serverCapacity)
    {
        size = space;
        table = new Server*[space];
        for (uint32_t i = 0; i < space; i++)
        {
            table[i] = new Server(serverCapacity);
        }
    }
    uint32_t hash(uint32_t i, string key)
    {
        return (7919 * checksum(move(key)) +  i * (104729 * checksum(move(key)) + 123)) % size;
    }


    uint32_t put(string value)
    {
        uint32_t i{};
        while (true)
        {
            uint32_t hashV = hash(i, value);
            if (table[hashV]->isNotFull())
            {
                table[hashV]->add(&value);
                return hashV;
            }
            i++;
        }
    }

    void putPrint(string value, ofstream &file)
    {
        uint32_t fstHash = hash(0, value);
        uint32_t finalServer = put(value);
        if (fstHash == finalServer)
        {
            file << "[S" << fstHash << "] ";
            table[fstHash]->printServer(file);
        }
        else
        {
            file << "S" << fstHash << "->S" << finalServer << "\n";
            file << "[S" << finalServer << "] ";
            table[finalServer]->printServer(file);
        }
    }
};

int main(int argc, char* argv[])
{
    cout << "Quantidade de argumentos (argc): " << argc << endl;
    for (int i = 0; i < argc; i++) {
        cout << "Argumento " << i << " argv[" << i << "]: " << argv[i] << endl;
    }
    ifstream input("entrada.txt");
    ofstream output("saida.txt");

    if (!input.is_open() || !output.is_open()) {
        cerr << "Erro ao abrir os arquivos de entrada ou saida." << endl;
    }
    uint32_t numberOfServers{};
    string temp{};
    getline(input, temp);
    numberOfServers = stoi(temp);

    uint32_t serversCapacity{};
    string temp2{};
    getline(input, temp2);
    serversCapacity = stoi(temp2);


    uint32_t numberOfEntries{};
    string temp3{};
    getline(input, temp3);
    numberOfEntries = stoi(temp3);

    auto hashtable = new Hashtable(numberOfServers, serversCapacity);

    string throwAway{};
    string line{};
    for (uint32_t i = 0; i < 2 * numberOfEntries; i++)
    {
        getline(input, line);
        line = line.substr(2);
        hashtable->putPrint(line, output);
    }

    input.close();
    output.close();
}
