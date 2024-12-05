#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

uint32_t checksum(const string &key) {
    uint32_t ans = 0;
    for (char c : key) {
        ans ^= static_cast<uint32_t>(c);
    }
    return ans;
}

struct Server {
    uint32_t capacity;
    string** slots;

    Server(uint32_t size) : capacity(size) {
        slots = new string*[capacity];
        for (uint32_t i = 0; i < capacity; i++) {
            slots[i] = nullptr;
        }
    }

    ~Server() {
        for (uint32_t i = 0; i < capacity; i++) {
            delete slots[i];
        }
        delete[] slots;
    }

    bool isNotFull() {
        for (uint32_t i = 0; i < capacity; i++) {
            if (slots[i] == nullptr) return true;
        }
        return false;
    }

    bool add(string* value) {
        for (uint32_t i = 0; i < capacity; i++) {
            if (slots[i] == nullptr) {
                slots[i] = value;
                return true;
            }
        }
        return false;
    }

    void printServer(ostream &file) const {
        bool first = true;
        for (uint32_t i = 0; i < capacity; i++) {
            if (slots[i] != nullptr) {
                if (!first) file << ", ";
                file << *slots[i];
                first = false;
            }
        }
    }
};

struct Hashtable {
    uint32_t size;
    Server** table;

    Hashtable(uint32_t space, uint32_t serverCapacity) : size(space) {
        table = new Server*[size];
        for (uint32_t i = 0; i < size; i++) {
            table[i] = new Server(serverCapacity);
        }
    }

    ~Hashtable() {
        for (uint32_t i = 0; i < size; i++) {
            delete table[i];
        }
        delete[] table;
    }

    uint32_t hash(uint32_t i, const string& key) const {
        uint32_t checksumValue = checksum(key);
        return ((7919 * checksumValue + i * (104729 * checksumValue + 123)) % size);
    }

    uint32_t put(const string& value) {
        uint32_t i = 0;
        while (i < true) {  // To avoid infinite loop
            uint32_t hashV = hash(i, value);
            if (table[hashV]->isNotFull()) {
                if (table[hashV]->add(new string(value))) {
                    return hashV;
                }
            }
            i++;
        }
        //throw runtime_error("Hashtable is full"); // In case of a full hashtable
    }

    void putPrint(const string& value, ofstream &file) {
        uint32_t fstHash = hash(0, value);
        uint32_t finalServer = put(value);
        if (fstHash == finalServer) {
            file << "[S" << fstHash << "] ";
            table[fstHash]->printServer(file);
            file << "\n";
        } else {
            file << "S" << fstHash << "->S" << finalServer << "\n";
            file << "[S" << finalServer << "] ";
            table[finalServer]->printServer(file);
            file << "\n";
        }
    }
};

int main(int argc, char* argv[]) {
    ifstream input("entrada.txt");
    ofstream output("saida.txt");

    if (!input.is_open() || !output.is_open()) {
        cerr << "Erro ao abrir os arquivos de entrada ou saída." << endl;
        return 1;
    }

    uint32_t numberOfServers{}, serversCapacity{}, numberOfEntries{};
    input >> numberOfServers >> serversCapacity >> numberOfEntries;

    auto hashtable = new Hashtable(numberOfServers, serversCapacity);

    string line;
    for (uint32_t i = 0; i < numberOfEntries; i++) {
        uint32_t numPatterns;
        input >> numPatterns;
        for (uint32_t j = 0; j < numPatterns; j++) {
            input >> line;
            hashtable->putPrint(line, output);
        }
    }

    input.close();
    output.close();

    delete hashtable;
    return 0;
}