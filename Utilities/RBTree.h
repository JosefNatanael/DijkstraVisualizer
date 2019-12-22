#ifndef RBTREE_H
#define RBTREE_H

#include <QDebug>
#include "Vertex.h"

enum class NodeColor {
    BLACK,
    RED
};

struct RBNode
{
    Vertex* data = nullptr;
    RBNode* parent = nullptr;
    RBNode* left = nullptr;
    RBNode* right = nullptr;
    NodeColor color;
};

class RedBlackTree
{
private:
    RBNode* root;
    RBNode* nullNode;

private:
    void initializeNULLNode(RBNode* node, RBNode* parent);
    void removeFix(RBNode* x);
    void rbTransplant(RBNode* u, RBNode* v);
    void removeNodeHelper(RBNode* node, Vertex* key);
    void insertFix(RBNode* k);

public:
    RedBlackTree();
    ~RedBlackTree();

public:
    bool isEmpty() const;
    RBNode* getRoot();

    RBNode* minimum(RBNode* node);
    Vertex* findMin();

    void leftRotate(RBNode* x);
    void rightRotate(RBNode* x);

    void insert(Vertex* key);
    void remove(Vertex* data);
};

#endif // RBTREE_H
