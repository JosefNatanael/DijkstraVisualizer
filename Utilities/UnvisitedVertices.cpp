#include "UnvisitedVertices.h"

UnvisitedVertices::UnvisitedVertices(unsigned int size)
{
    array.resize(size);
    for (unsigned int i = 0; i < size; ++i) {
        array[i] = nullptr;
    }
}

unsigned int UnvisitedVertices::hashFunction(unsigned int key)
{
    return key % array.size();
}

void UnvisitedVertices::insertNode(unsigned int key, const Vertex * const value)
{
    HashNode* temp = new HashNode(key, value);
    unsigned int index = hashFunction(key);
    // Guaranteed no colission.
    array[index] = temp;
}

void UnvisitedVertices::deleteNode(unsigned int key)
{
    unsigned int index = hashFunction(key);
    // Guaranteed able to delete
    delete array[index];
    array[index] = nullptr;
}

bool UnvisitedVertices::exists(unsigned int key)
{
    unsigned int index = hashFunction(key);
    if (array[index] == nullptr) {
        return false;
    }
    return true;
}
