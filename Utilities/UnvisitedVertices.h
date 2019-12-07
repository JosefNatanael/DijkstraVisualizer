#ifndef UNVISITEDVERTICES_H
#define UNVISITEDVERTICES_H

#include "Vertex.h"
#include <vector>

class UnvisitedVertices
{
public:
    struct HashNode {
        HashNode(unsigned int key, const Vertex* const value) : key(key), value(value) {}
        unsigned int key;
        const Vertex* const value;
    };

    UnvisitedVertices(unsigned int size);
    unsigned int hashFunction(unsigned int key);
    void insertNode(unsigned int key, const Vertex* const value);
    void deleteNode(unsigned int key);
    bool exists(unsigned int key);

private:
    std::vector<HashNode*> array;


};

#endif // UNVISITEDVERTICES_H
