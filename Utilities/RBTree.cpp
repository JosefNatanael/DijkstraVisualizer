#include <QDebug>

#include "Vertex.h"
#include "RBTree.h"

RedBlackTree::RedBlackTree()
{
    nullNode = new RBNode;
    nullNode->color = NodeColor::BLACK;
    nullNode->left = nullptr;
    nullNode->right = nullptr;
    root = nullNode;
}
RedBlackTree::~RedBlackTree()
{
    delete nullNode;
}

bool RedBlackTree::isEmpty() const
{
    return root == nullptr || root == nullNode;
}

RBNode* RedBlackTree::minimum(RBNode* node)
{
    while (node->left != nullNode) {
        node = node->left;
    }
    return node;
}

Vertex* RedBlackTree::findMin() {
    RBNode* temp = minimum(root);
    return temp->data;
}

void RedBlackTree::leftRotate(RBNode* x)
{
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != nullNode) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rightRotate(RBNode* x)
{
    RBNode* y = x->left;
    x->left = y->right;
    if (y->right != nullNode) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    }
    else if (x == x->parent->right) {
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void RedBlackTree::insert(Vertex* key)
{
    RBNode* node = new RBNode;
    node->parent = nullptr;
    node->data = key;
    node->left = nullNode;
    node->right = nullNode;
    node->color = NodeColor::RED;
    RBNode* y = nullptr;
    RBNode* x = this->root;
    while (x != nullNode) {
        y = x;
        if (node->data->getDistance() < x->data->getDistance()) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    node->parent = y;
    if (y == nullptr) {
        root = node;
    }
    else if (node->data->getDistance() < y->data->getDistance()) {
        y->left = node;
    }
    else {
        y->right = node;
    }
    if (node->parent == nullptr) {
        node->color = NodeColor::BLACK;
        return;
    }
    if (node->parent->parent == nullptr) {
        return;
    }
    insertFix(node);
}

RBNode* RedBlackTree::getRoot()
{
    return this->root;
}

void RedBlackTree::remove(Vertex* data)
{
    removeNodeHelper(this->root, data);
}

void RedBlackTree::initializeNULLNode(RBNode* node, RBNode* parent)
{
    node->data = nullptr;
    node->parent = parent;
    node->left = nullptr;
    node->right = nullptr;
    node->color = NodeColor::BLACK;
}

void RedBlackTree::removeFix(RBNode* x)
{
    RBNode* s;
    while (x != root && x->color == NodeColor::BLACK) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == NodeColor::RED) {
                s->color = NodeColor::BLACK;
                x->parent->color = NodeColor::RED;
                leftRotate(x->parent);
                s = x->parent->right;
            }
            if (s->left->color == NodeColor::BLACK && s->right->color == NodeColor::BLACK) {
                s->color = NodeColor::RED;
                x = x->parent;
            }
            else {
                if (s->right->color == NodeColor::BLACK) {
                    s->left->color = NodeColor::BLACK;
                    s->color = NodeColor::RED;
                    rightRotate(s);
                    s = x->parent->right;
                }
                s->color = x->parent->color;
                x->parent->color = NodeColor::BLACK;
                s->right->color = NodeColor::BLACK;
                leftRotate(x->parent);
                x = root;
            }
        }
        else {
            s = x->parent->left;
            if (s->color == NodeColor::RED) {
                s->color = NodeColor::BLACK;
                x->parent->color = NodeColor::RED;
                rightRotate(x->parent);
                s = x->parent->left;
            }
            if (s->right->color == NodeColor::BLACK && s->right->color == NodeColor::BLACK) {
                s->color = NodeColor::RED;
                x = x->parent;
            }
            else {
                if (s->left->color == NodeColor::BLACK) {
                    s->right->color = NodeColor::BLACK;
                    s->color = NodeColor::RED;
                    leftRotate(s);
                    s = x->parent->left;
                }
                s->color = x->parent->color;
                x->parent->color = NodeColor::BLACK;
                s->left->color = NodeColor::BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = NodeColor::BLACK;
}

void RedBlackTree::rbTransplant(RBNode* u, RBNode* v)
{
    if (u->parent == nullptr) {
        root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void RedBlackTree::removeNodeHelper(RBNode* node, Vertex* key)
{
    RBNode* z = nullNode;
    RBNode* x;
    RBNode* y;
    bool found = false;
    while (node != nullNode) {
        if (node->data->getDistance() == key->getDistance()) {
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
        if (node->data->getDistance() <= key->getDistance()) {
            node = node->right;
        }
        else {
            node = node->left;
        }
    }
    if (z == nullNode) {
        return;
    }
    y = z;
    NodeColor y_original_color = y->color;
    if (z->left == nullNode) {
        x = z->right;
        rbTransplant(z, z->right);
    }
    else if (z->right == nullNode) {
        x = z->left;
        rbTransplant(z, z->left);
    }
    else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        }
        else {
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
    if (y_original_color == NodeColor::BLACK) {
        removeFix(x);
    }
}

void RedBlackTree::insertFix(RBNode* k)
{
    RBNode* u;
    while (k->parent->color == NodeColor::RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;
            if (u->color == NodeColor::RED)
            {
                u->color = NodeColor::BLACK;
                k->parent->color = NodeColor::BLACK;
                k->parent->parent->color = NodeColor::RED;
                k = k->parent->parent;
            }
            else {
                if (k == k->parent->left)
                {
                    k = k->parent;
                    rightRotate(k);
                }
                k->parent->color = NodeColor::BLACK;
                k->parent->parent->color = NodeColor::RED;
                leftRotate(k->parent->parent);
            }
        }
        else {
            u = k->parent->parent->right;
            if (u->color == NodeColor::RED)
            {
                u->color = NodeColor::BLACK;
                k->parent->color = NodeColor::BLACK;
                k->parent->parent->color = NodeColor::RED;
                k = k->parent->parent;
            }
            else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }
                k->parent->color = NodeColor::BLACK;
                k->parent->parent->color = NodeColor::RED;
                rightRotate(k->parent->parent);
            }
        }
        if (k == root) {
            break;
        }
    }
    root->color = NodeColor::BLACK;
}
