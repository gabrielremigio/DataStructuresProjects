#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace std;

struct Book
{
    int64_t ISBN{};
    string author{};
    string title{};

    void setFromLine(string line)
    {
        uint32_t pos = line.find('&');
        ISBN = stoull(line.substr(0, 13));
        author = line.substr(14, pos - 14);
        title = line.substr(pos + 1);
    }
};

struct SearchArray
{
    Book** arr{};
    uint32_t size{};

    SearchArray(uint32_t arrSize)
    {
        size = arrSize;
        arr = new Book*[size];
    }

    int64_t binarySearch(uint64_t code, int32_t start, int32_t end, uint32_t &numberOfTries)
    {
        int32_t pivot = (start + end)/2;
        if (end < start)
        {
            numberOfTries++;
            return -1;
        }
        else if (arr[pivot]->ISBN == code)
        {
            numberOfTries++;
            return pivot;
        }
        else if (arr[pivot]->ISBN < code)
        {
            numberOfTries++;
            return binarySearch(code, pivot + 1, end, numberOfTries);
        }
        else
        {
            numberOfTries++;
            return binarySearch(code, start, pivot - 1, numberOfTries);
        }

    }

    int32_t interpolationSearch(uint64_t code, int32_t start, int32_t end, uint32_t &numberOfTries)
    {
        if (end < start)
        {
            return -1;
        }
        int32_t pivot = floor(start + (arr[end]->ISBN - arr[start]->ISBN) % (end - start + 1));
        if (arr[pivot]->ISBN == code)
        {
            numberOfTries++;
            return pivot;
        }
        else if (arr[pivot]->ISBN < code)
        {
            numberOfTries++;
            return interpolationSearch(code, pivot + 1, end, numberOfTries);
        }
        else
        {
            numberOfTries++;
            return interpolationSearch(code, start, pivot - 1, numberOfTries);
        }
    }
    void printSearch(ostream &file, uint64_t code, int32_t start, int32_t end, uint32_t &bWins, uint32_t &iWins, uint32_t &totalBAttemps, uint32_t &totalIAttempts)
    {
        uint32_t binaryTries{}, interpolationTries{};
        int32_t resultIndex{};
        resultIndex = interpolationSearch(code, start, end, interpolationTries);
        binarySearch(code, start, end, binaryTries);

        totalIAttempts += interpolationTries;
        totalBAttemps += binaryTries;

        if (interpolationTries <= binaryTries)
            iWins++;
        else
            bWins++;

        if (resultIndex == -1)
        {
            file << "[" << code << "]" << "B=" << binaryTries << ",I=" << interpolationTries << ":ISBN_NOT_FOUND" << endl;
        }
        else
        {
            file << "[" << code << "]" << "B=" << binaryTries << ",I="
            << interpolationTries << ":Author:" << arr[resultIndex]->author
            << ",Title:" << arr[resultIndex]->title << endl;
        }
    }
};


int main(int argc, char* argv[])
{
    cout << "Quantidade de argumentos (argc): " << argc << endl;
    for (int i = 0; i < argc; i++)
    {
        cout << "Argumento " << i << " argv[" << i << "]: " << argv[i] << endl;
    }
    ifstream input("entrada.txt");
    ofstream output("saida.output");

    if (!input.is_open() || !output.is_open())
    {
        cerr << "Erro ao abrir os arquivos de entrada ou saida." << endl;
    }

    uint32_t numberOfEntries{}, numberOfSearches{};
    string temp;
    getline(input, temp);
    numberOfEntries = stoi(temp);

    SearchArray* database = new SearchArray(numberOfEntries);
    string line{};
    for (uint32_t i = 0; i < numberOfEntries; i++)
    {
        Book* newBook = new Book;
        getline(input, line);
        newBook->setFromLine(line);
        database->arr[i] = newBook;
    }

    string temp1;
    getline(input, temp1);
    numberOfSearches = stoi(temp1);
    string line2{};
    uint64_t currentISBN{};
    uint32_t interpolationWins{}, binaryWins{}, totalBinaryAttempts{}, totalInterpolationAttempts{};

    for (uint32_t i = 0; i < numberOfSearches; i++)
    {
        getline(input, line2);
        currentISBN = stoull(line2);
        database->printSearch(output, currentISBN, 0, database->size - 1, binaryWins, interpolationWins, totalBinaryAttempts, totalInterpolationAttempts);
    }

    uint32_t mediumBinary{}, mediumInterpolation{};
    mediumBinary = totalBinaryAttempts / numberOfSearches;
    mediumInterpolation = totalInterpolationAttempts / numberOfSearches;

    output << "BINARY=" << binaryWins << ":" << mediumBinary << endl;
    output << "INTERPOLATION=" << interpolationWins << ":" << mediumInterpolation << endl;
    input.close();
    output.close();
}
