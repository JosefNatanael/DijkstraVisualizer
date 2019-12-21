#ifndef REDBLACKPRIORITY_H
#define REDBLACKPRIORITY_H

#include <QDebug>
#include "Vertex.h"

enum class NodeColor { RED, BLACK, DOUBLE_BLACK };

struct RBNode
{
    Vertex* vertex;
    RBNode* left;
    RBNode* right;
    RBNode* parent = nullptr;
    NodeColor color;
    explicit RBNode(Vertex*);
};

class RedBlackPriority
{
public:
    explicit RedBlackPriority();

    Vertex* findMin();
    bool isEmpty() const { return root == nullptr; }

    void insert(Vertex* v);
    void remove(Vertex* v);

private:
    void rotateLeft(RBNode *&);
    void rotateRight(RBNode *&);

    void fixInsertRBTree(RBNode *&);
    void fixRemoveRBTree(RBNode *&);

    NodeColor getColor(RBNode*& node) { if (node == nullptr) return NodeColor::BLACK; return node->color; }
    void setColor(RBNode*& node, NodeColor color) { if (node == nullptr) return; node->color = color; }

    RBNode* minValueNode(RBNode*&);
    int getBlackHeight(RBNode* node);

    RBNode* insertBST(RBNode *&, RBNode *&);
    RBNode* deleteBST(RBNode *&, Vertex*);


private:
    RBNode* root;
};

#endif // REDBLACKPRIORITY_H
