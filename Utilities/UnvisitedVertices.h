#ifndef UNVISITEDVERTICES_H
#define UNVISITEDVERTICES_H

#include "Vertex.h"
#include <vector>

#include <QDebug>

class UnvisitedVertices
{
public:
    struct HashNode {
        HashNode(int key, const Vertex* const value) : key(key), value(value) {}
        unsigned int key;
        const Vertex* const value;
    };

    UnvisitedVertices(int size = 50);
    int hashFunction(int key);
    void insertNode(int key, const Vertex* const value);
    void removeNode(int key);
    bool exists(int key);

private:
    std::vector<HashNode*> array;
};

#endif // UNVISITEDVERTICES_H
