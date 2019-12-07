#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

template <typename Vertex>
class PriorityQueue
{
private:
    struct AVLnode
    {
        Vertex* vertex;
        int height;
        PriorityQueue left;
        PriorityQueue right;
        AVLnode(Vertex* v) : vertex(v), height(0), left(), right() {}
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
    Vertex* findMin() const;
    bool contains(Vertex* v) const;

    void insert(Vertex* v);
    void remove(Vertex* v);
};



#endif // PRIORITYQUEUE_H
