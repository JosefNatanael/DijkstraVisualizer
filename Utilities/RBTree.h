#ifndef RBTREE_H
#define RBTREE_H

#include <QDebug>
#include "Vertex.h"

struct Node
{
    Vertex* data = nullptr;
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;
    int color;
};
class RedBlackTree
{
private:
    Node* root;
    Node* TNULL;
    void initializeNULLNode(Node* node, Node* parent)
    {
        node->data = nullptr;
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->color = 0;
    }
    void preOrderHelper(Node* node)
    {
        if (node != TNULL)
        {
            qDebug() << node->data << " ";
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }
    void inOrderHelper(Node* node)
    {
        if (node != TNULL)
        {
            inOrderHelper(node->left);
            qDebug() << node->data << " ";
            inOrderHelper(node->right);
        }
    }
    void postOrderHelper(Node* node)
    {
        if (node != TNULL)
        {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            qDebug() << node->data << " ";
        }
    }
    Node* searchTreeHelper(Node* node, Vertex* key)
    {
        if (node == TNULL || key == node->data)
        {
            return node;
        }
        if (key->getDistance() == node->data->getDistance()) {
            searchTreeHelper(node->left, key);
            searchTreeHelper(node->right, key);
        }
        if (key->getDistance() < node->data->getDistance())
        {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }
    void removeFix(Node* x)
    {
        Node* s;
        while (x != root && x->color == 0)
        {
            if (x == x->parent->left)
            {
                s = x->parent->right;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }
                if (s->left->color == 0 && s->right->color == 0)
                {
                    s->color = 1;
                    x = x->parent;
                }
                else
                {
                    if (s->right->color == 0)
                    {
                        s->left->color = 0;
                        s->color = 1;
                        rightRotate(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else
            {
                s = x->parent->left;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }
                if (s->right->color == 0 && s->right->color == 0)
                {
                    s->color = 1;
                    x = x->parent;
                }
                else
                {
                    if (s->left->color == 0)
                    {
                        s->right->color = 0;
                        s->color = 1;
                        leftRotate(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = 0;
    }
    void rbTransplant(Node* u, Node* v)
    {
        if (u->parent == nullptr)
        {
            root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }
    void removeNodeHelper(Node* node, Vertex* key)
    {
        Node* z = TNULL;
        Node* x;
        Node* y;
        bool found = false;
        while (node != TNULL)
        {
            if (node->data->getDistance() == key->getDistance())
            {
                if (node->data != key && !found) {
                    removeNodeHelper(node->left, key);
                    removeNodeHelper(node->right, key);
                    return;
                }
                else {
                    if (!found) {
                        z = node;
                    }
                    found = true;
                }
            }
            if (node->data->getDistance() <= key->getDistance())
            {
                node = node->right;
            }
            else
            {
                node = node->left;
            }
        }
        if (z == TNULL)
        {
            qDebug() << "Key not found in the tree" << endl;
            return;
        }
        y = z;
        int y_original_color = y->color;
        if (z->left == TNULL)
        {
            x = z->right;
            rbTransplant(z, z->right);
        }
        else if (z->right == TNULL)
        {
            x = z->left;
            rbTransplant(z, z->left);
        }
        else
        {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z)
            {
                x->parent = y;
            }
            else
            {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == 0)
        {
            removeFix(x);
        }
    }
    void insertFix(Node* k)
    {
        Node* u;
        while (k->parent->color == 1)
        {
            if (k->parent == k->parent->parent->right)
            {
                u = k->parent->parent->left;
                if (u->color == 1)
                {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->left)
                    {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
                }
            }
            else
            {
                u = k->parent->parent->right;
                if (u->color == 1)
                {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->right)
                    {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root)
            {
                break;
            }
        }
        root->color = 0;
    }
public:
    RedBlackTree()
    {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }
    bool isEmpty() const
    {
        return root == nullptr || root == TNULL;
    }
    void preorder()
    {
        preOrderHelper(this->root);
    }
    void inorder()
    {
        inOrderHelper(this->root);
    }
    void postorder()
    {
        postOrderHelper(this->root);
    }
    Node* searchTree(Vertex* k)
    {
        return searchTreeHelper(this->root, k);
    }
    Node* minimum(Node* node)
    {
        while (node->left != TNULL)
        {
            node = node->left;
        }
        return node;
    }
    Vertex* findMin() {
        Node* temp = minimum(root);
        return temp->data;
    }
    Node* maximum(Node* node)
    {
        while (node->right != TNULL)
        {
            node = node->right;
        }
        return node;
    }
    Node* successor(Node* x)
    {
        if (x->right != TNULL)
        {
            return minimum(x->right);
        }
        Node* y = x->parent;
        while (y != TNULL && x == y->right)
        {
            x = y;
            y = y->parent;
        }
        return y;
    }
    Node* predecessor(Node* x)
    {
        if (x->left != TNULL)
        {
            return maximum(x->left);
        }
        Node* y = x->parent;
        while (y != TNULL && x == y->left)
        {
            x = y;
            y = y->parent;
        }
        return y;
    }
    void leftRotate(Node* x)
    {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != TNULL)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }
    void rightRotate(Node* x)
    {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != TNULL)
        {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }
    void insert(Vertex* key)
    {
        Node* node = new Node;
        node->parent = nullptr;
        node->data = key;
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1;
        Node* y = nullptr;
        Node* x = this->root;
        while (x != TNULL)
        {
            y = x;
            if (node->data->getDistance() < x->data->getDistance())
            {
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }
        node->parent = y;
        if (y == nullptr)
        {
            root = node;
        }
        else if (node->data->getDistance() < y->data->getDistance())
        {
            y->left = node;
        }
        else
        {
            y->right = node;
        }
        if (node->parent == nullptr)
        {
            node->color = 0;
            return;
        }
        if (node->parent->parent == nullptr)
        {
            return;
        }
        insertFix(node);
    }
    Node* getRoot()
    {
        return this->root;
    }
    void remove(Vertex* data)
    {
        removeNodeHelper(this->root, data);
    }

};

#endif // RBTREE_H
