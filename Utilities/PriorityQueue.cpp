#include "PriorityQueue.h"

#include <algorithm>

template <typename Vertex>
int PriorityQueue<Vertex>::height() const
{
    return isEmpty() ? -1 : root->height;
}

template <typename Vertex>
int PriorityQueue<Vertex>::bfactor() const
{
    return isEmpty() ? 0 : rightSubtree().height() - leftSubtree().height();
}

template <typename Vertex>
void PriorityQueue<Vertex>::fixHeight() const
{
    if (!isEmpty())
    {
        int leftAVLheight = leftSubtree().height();
        int rightAVLheight = rightSubtree().height();
        root->height = 1 + std::max(leftAVLheight, rightAVLheight);
    }
}

template <typename Vertex>
void PriorityQueue<Vertex>::rotateLeft()
{
    AVLnode* node = rightSubtree().root;
    rightSubtree() = node->left;
    node->left = *this;
    fixHeight();
    this->root = node;
    fixHeight();
}

template <typename Vertex>
void PriorityQueue<Vertex>::rotateRight()
{
    AVLnode* node = leftSubtree().root;
    leftSubtree() = node->right;
    node->right = *this;
    fixHeight();
    this->root = node;
    fixHeight();
}

template <typename Vertex>
void PriorityQueue<Vertex>::balance()
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

template <typename Vertex>
Vertex* PriorityQueue<Vertex>::findMin() const
{
    const PriorityQueue& leftAVL = leftSubtree();
    if (leftAVL.isEmpty())
        return root->vertex;
    return leftAVL.findMin();
}

template <typename Vertex>
bool PriorityQueue<Vertex>::contains(Vertex* v) const
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

template <typename Vertex>
void PriorityQueue<Vertex>::insert(Vertex *v)
{
    if (isEmpty())
        root = new AVLnode(v);
    else if (v->getDistance() < root->vertex->getDistance())
        leftSubtree().insert(v);
    else if (v->getDistance() >= root->vertex->getDistance())
        rightSubtree().insert(v);
    balance();
}

template <typename Vertex>
void PriorityQueue<Vertex>::remove(Vertex *v)
{
    if (isEmpty())
        return;
    if (v->getDistance() < root->vertex->getDistance())
        leftSubtree().remove(v);
    else if (v->getDistance() >= root->vertex->getDistance())
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
