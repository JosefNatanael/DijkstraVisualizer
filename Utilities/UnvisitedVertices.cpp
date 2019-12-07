#include "UnvisitedVertices.h"

UnvisitedVertices::UnvisitedVertices(int size)
{
    array.resize(static_cast<unsigned int>(size));
    for (int i = 0; i < size; ++i) {
        array[static_cast<unsigned int>(i)] = nullptr;
    }
}

int UnvisitedVertices::hashFunction(int key)
{
    return key % static_cast<int>(array.size());
}

void UnvisitedVertices::insertNode(int key, const Vertex * const value)
{
    HashNode* temp = new HashNode(key, value);
    int index = hashFunction(key);
    // Guaranteed no colission.
    array[static_cast<unsigned>(index)] = temp;
}

void UnvisitedVertices::removeNode(int key)
{
    int index = hashFunction(key);
    // Guaranteed able to delete
    delete array[static_cast<unsigned int>(index)];
    array[static_cast<unsigned int>(index)] = nullptr;
}

bool UnvisitedVertices::exists(int key)
{
    int index = hashFunction(key);
    if (array[static_cast<unsigned int>(index)] == nullptr) {
        return false;
    }
    return true;
}
