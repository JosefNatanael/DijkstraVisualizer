#include "RedBlackPriority.h"

RBNode::RBNode(Vertex* vertex)
{
    this->vertex = vertex;
    color = NodeColor::RED;
    left = right = parent = nullptr;
}


RedBlackPriority::RedBlackPriority()
{
    root = nullptr;
}

Vertex *RedBlackPriority::findMin()
{
    return minValueNode(root)->vertex;
}

void RedBlackPriority::insert(Vertex* v)
{
    RBNode* node = new RBNode(v);
    root = insertBST(root, node);
    fixInsertRBTree(node);
}

void RedBlackPriority::remove(Vertex* v)
{
    RBNode* node = deleteBST(root, v);
    fixRemoveRBTree(node);
}

void RedBlackPriority::rotateLeft(RBNode*& node)
{
    RBNode *right_child = node->right;
    node->right = right_child->left;

    if (node->right != nullptr)
        node->right->parent = node;

    right_child->parent = node->parent;

    if (node->parent == nullptr)
        root = right_child;
    else if (node == node->parent->left)
        node->parent->left = right_child;
    else
        node->parent->right = right_child;

    right_child->left = node;
    node->parent = right_child;
}

void RedBlackPriority::rotateRight(RBNode*& node)
{
    RBNode *left_child = node->left;
    node->left = left_child->right;

    if (node->left != nullptr)
        node->left->parent = node;

    left_child->parent = node->parent;

    if (node->parent == nullptr)
        root = left_child;
    else if (node == node->parent->left)
        node->parent->left = left_child;
    else
        node->parent->right = left_child;

    left_child->right = node;
    node->parent = left_child;
}

void RedBlackPriority::fixInsertRBTree(RBNode*& ptr)
{
    RBNode *parent = nullptr;
    RBNode *grandparent = nullptr;
    while (ptr != root && getColor(ptr) == NodeColor::RED && getColor(ptr->parent) == NodeColor::RED) {
        parent = ptr->parent;
        grandparent = parent->parent;
        if (parent == grandparent->left) {
            RBNode *uncle = grandparent->right;
            if (getColor(uncle) == NodeColor::RED) {
                setColor(uncle, NodeColor::BLACK);
                setColor(parent, NodeColor::BLACK);
                setColor(grandparent, NodeColor::RED);
                ptr = grandparent;
            } else {
                if (ptr == parent->right) {
                    rotateLeft(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                rotateRight(grandparent);
                std::swap(parent->color, grandparent->color);
                ptr = parent;
            }
        } else {
            RBNode *uncle = grandparent->left;
            if (getColor(uncle) == NodeColor::RED) {
                setColor(uncle, NodeColor::BLACK);
                setColor(parent, NodeColor::BLACK);
                setColor(grandparent, NodeColor::RED);
                ptr = grandparent;
            } else {
                if (ptr == parent->left) {
                    rotateRight(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                rotateLeft(grandparent);
                std::swap(parent->color, grandparent->color);
                ptr = parent;
            }
        }
    }
    setColor(root, NodeColor::BLACK);
}

void RedBlackPriority::fixRemoveRBTree(RBNode*& node)
{
    if (node == nullptr)
        return;

    if (node == root) {
        root = nullptr;
        return;
    }

    if (getColor(node) == NodeColor::RED || getColor(node->left) == NodeColor::RED || getColor(node->right) == NodeColor::RED) {
        RBNode *child = node->left != nullptr ? node->left : node->right;

        if (node == node->parent->left) {
            node->parent->left = child;
            if (child != nullptr)
                child->parent = node->parent;
            setColor(child, NodeColor::BLACK);
            delete (node);
        } else {
            node->parent->right = child;
            if (child != nullptr)
                child->parent = node->parent;
            setColor(child, NodeColor::BLACK);
            delete (node);
        }
    } else {
        RBNode *sibling = nullptr;
        RBNode *parent = nullptr;
        RBNode *ptr = node;
        setColor(ptr, NodeColor::DOUBLE_BLACK);
        while (ptr != root && getColor(ptr) == NodeColor::DOUBLE_BLACK) {
            parent = ptr->parent;
            if (ptr == parent->left) {
                sibling = parent->right;
                if (getColor(sibling) == NodeColor::RED) {
                    setColor(sibling, NodeColor::BLACK);
                    setColor(parent, NodeColor::RED);
                    rotateLeft(parent);
                } else {
                    if (getColor(sibling->left) == NodeColor::BLACK && getColor(sibling->right) == NodeColor::BLACK) {
                        setColor(sibling, NodeColor::RED);
                        if(getColor(parent) == NodeColor::RED)
                            setColor(parent, NodeColor::BLACK);
                        else
                            setColor(parent, NodeColor::DOUBLE_BLACK);
                        ptr = parent;
                    } else {
                        if (getColor(sibling->right) == NodeColor::BLACK) {
                            setColor(sibling->left, NodeColor::BLACK);
                            setColor(sibling, NodeColor::RED);
                            rotateRight(sibling);
                            sibling = parent->right;
                        }
                        setColor(sibling, parent->color);
                        setColor(parent, NodeColor::BLACK);
                        setColor(sibling->right, NodeColor::BLACK);
                        rotateLeft(parent);
                        break;
                    }
                }
            } else {
                sibling = parent->left;
                if (getColor(sibling) == NodeColor::RED) {
                    setColor(sibling, NodeColor::BLACK);
                    setColor(parent, NodeColor::RED);
                    rotateRight(parent);
                } else {
                    if (getColor(sibling->left) == NodeColor::BLACK && getColor(sibling->right) == NodeColor::BLACK) {
                        setColor(sibling, NodeColor::RED);
                        if (getColor(parent) == NodeColor::RED)
                            setColor(parent, NodeColor::BLACK);
                        else
                            setColor(parent, NodeColor::DOUBLE_BLACK);
                        ptr = parent;
                    } else {
                        if (getColor(sibling->left) == NodeColor::BLACK) {
                            setColor(sibling->right, NodeColor::BLACK);
                            setColor(sibling, NodeColor::RED);
                            rotateLeft(sibling);
                            sibling = parent->left;
                        }
                        setColor(sibling, parent->color);
                        setColor(parent, NodeColor::BLACK);
                        setColor(sibling->left, NodeColor::BLACK);
                        rotateRight(parent);
                        break;
                    }
                }
            }
        }
        if (node == node->parent->left)
            node->parent->left = nullptr;
        else
            node->parent->right = nullptr;
        delete(node);
        setColor(root, NodeColor::BLACK);
    }
}

RBNode *RedBlackPriority::minValueNode(RBNode*& node)
{
    RBNode *ptr = node;

    while (ptr->left != nullptr)
        ptr = ptr->left;

    return ptr;
}

int RedBlackPriority::getBlackHeight(RBNode *node)
{
    int blackheight = 0;
    while (node != nullptr) {
        if (getColor(node) == NodeColor::BLACK)
            blackheight++;
        node = node->left;
    }
    return blackheight;
}

RBNode *RedBlackPriority::insertBST(RBNode*& root, RBNode*& node)
{
    if (root == nullptr)
        return node;

    if (node->vertex->getDistance() < root->vertex->getDistance()) {
        root->left = insertBST(root->left, node);
        root->left->parent = root;
    } else if (node->vertex->getDistance() >= root->vertex->getDistance()) {
        root->right = insertBST(root->right, node);
        root->right->parent = root;
    }

    return root;
}

RBNode *RedBlackPriority::deleteBST(RBNode*& root, Vertex* v)
{
    if (root == nullptr)
        return root;

    if (v != root->vertex && v->getDistance() == root->vertex->getDistance()) {
        deleteBST(root->left, v);
        deleteBST(root->right, v);
    }
    else if (v->getDistance() < root->vertex->getDistance())
        return deleteBST(root->left, v);

    else if (v->getDistance() > root->vertex->getDistance())
        return deleteBST(root->right, v);

    if (root->left == nullptr || root->right == nullptr)
        return root;

    RBNode *temp = minValueNode(root->right);
    root->vertex = temp->vertex;
    return deleteBST(root->right, temp->vertex);
}
