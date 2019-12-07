#include "PriorityQueue.h"

#include <algorithm>

template <typename VertexPtr>
int PriorityQueue<VertexPtr>::height() const
{
    return isEmpty() ? -1 : root->height;
}

template <typename VertexPtr>
int PriorityQueue<VertexPtr>::bfactor() const
{
    return isEmpty() ? 0 : rightSubtree().height() - leftSubtree().height();
}

template <typename VertexPtr>
void PriorityQueue<VertexPtr>::fixHeight() const
{
    if (!isEmpty())
    {
        int leftAVLheight = leftSubtree().height();
        int rightAVLheight = rightSubtree().height();
        root->height = 1 + std::max(leftAVLheight, rightAVLheight);
    }
}

template <typename VertexPtr>
void PriorityQueue<VertexPtr>::rotateLeft()
{
    AVLnode* node = rightSubtree().root;
    rightSubtree() = node->left;
    node->left = *this;
    fixHeight();
    this->root = node;
    fixHeight();
}

template <typename VertexPtr>
void PriorityQueue<VertexPtr>::rotateRight()
{
    AVLnode* node = leftSubtree().root;
    leftSubtree() = node->right;
    node->right = *this;
    fixHeight();
    this->root = node;
    fixHeight();
}

template <typename VertexPtr>
void PriorityQueue<VertexPtr>::balance()
{
    if (isEmpty())
        return;
    fixHeight();
    int balanceFactor = bfactor();
    if (balanceFactor == 2)
    {
        if (rightSubtree().bfactor() < 0)
            rightSubtree().rotateRight();
        return rotateLeft();
    }
    else if (balanceFactor == -2)
    {
        if (leftSubtree().bfactor() > 0)
            leftSubtree().rotateLeft();
        return rotateRight();
    }
}

template <typename VertexPtr>
VertexPtr PriorityQueue<VertexPtr>::findMin() const
{
    const PriorityQueue& leftAVL = leftSubtree();
    if (leftAVL.isEmpty())
        return root->vertex;
    return leftAVL.findMin();
}

template <typename VertexPtr>
bool PriorityQueue<VertexPtr>::contains(VertexPtr v) const
{
    if (isEmpty())
        return false;
    else if (v == root->vertex)
        return true;
    else if (v->getDistance() < root->vertex->getDistance())
        return leftSubtree().contains(v);
    else
        return rightSubtree().contains(v);
}

template <typename VertexPtr>
void PriorityQueue<VertexPtr>::insert(VertexPtr v)
{
    if (isEmpty())
        root = new AVLnode(v);
    else if (v->getDistance() < root->vertex->getDistance())
        leftSubtree().insert(v);
    else if (v->getDistance() >= root->vertex->getDistance())
        rightSubtree().insert(v);
    balance();
}

template <typename VertexPtr>
void PriorityQueue<VertexPtr>::remove(VertexPtr v)
{
    if (isEmpty())
        return;
    if (v != root->vertex && v->getDistance() == root->vertex->getDistance()) {
        leftSubtree().remove(v);
        rightSubtree().remove(v);
    }
    if (v->getDistance() < root->vertex->getDistance())
        leftSubtree().remove(v);
    else if (v->getDistance() > root->vertex->getDistance())
        rightSubtree().remove(v);
    else {
        PriorityQueue& leftAVL = leftSubtree();
        PriorityQueue& rightAVL = rightSubtree();
        if (!leftAVL.isEmpty() && !rightAVL.isEmpty()) {
            root->vertex = rightAVL.findMin();
            rightAVL.remove(root->vertex);
        }
        else {
            AVLnode* toRemove = root;
            *this = leftAVL.isEmpty() ? rightAVL : leftAVL;
            rightAVL.root = leftAVL.root = nullptr;
            delete toRemove;
        }
    }
    balance();
}
