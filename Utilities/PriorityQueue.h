#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QDebug>

template <typename VertexPtr>
class PriorityQueue
{
private:
    struct AVLnode
    {
        VertexPtr vertex;
        int height;
        PriorityQueue left;
        PriorityQueue right;
        AVLnode(VertexPtr v) : vertex(v), height(0), left(), right() {}
    };

    AVLnode* root = nullptr;

    PriorityQueue& rightSubtree() { return root->right; }
    PriorityQueue& leftSubtree() { return root->left; }
    const PriorityQueue& rightSubtree() const { return root->right; }
    const PriorityQueue& leftSubtree() const { return root->left; }

    int height() const;
    int bfactor() const;
    void fixHeight() const;
    void rotateLeft();
    void rotateRight();
    void balance();

public:
    PriorityQueue() = default;
    ~PriorityQueue() { delete root; }

    bool isEmpty() const { return root == nullptr; }
    VertexPtr findMin() const;
    bool contains(VertexPtr v) const;

    void insert(VertexPtr v);
    void remove(VertexPtr v);
};

#include "PriorityQueue.tpp"

#endif // PRIORITYQUEUE_H
