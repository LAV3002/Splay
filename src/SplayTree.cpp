#include "SplayTree.h"

#include <utility>

struct SplayTree::Node
{
    int value;
    Node *left, *right, *parent;
    Node(int value)
        : value(value)
        , left(nullptr)
        , right(nullptr)
        , parent(nullptr)
    {
    }
    ~Node()
    {
        delete left;
        delete right;
    }
};

SplayTree::~SplayTree()
{
    delete root;
}

void SplayTree::rotate(Node * v)
{
    // v: v
    // p: v's parent
    // g: v's grandparent
    Node * p = v->parent;
    Node * Node::*first_descendant = v == p->left ? &Node::right : &Node::left;
    Node * Node::*second_descendant = v == p->left ? &Node::left : &Node::right;
    Node * g = p->parent;
    Node * t = p->*second_descendant;
    if (g) {
        if (g->*first_descendant == p) {
            g->*first_descendant = t;
        }
        else {
            g->*second_descendant = t;
        }
    }
    p->*second_descendant = std::exchange(t->*first_descendant, p);
    p->parent = t;
    t->parent = g;
    if (p->*second_descendant != nullptr) {
        (p->*second_descendant)->parent = p;
    }
}

SplayTree::Node * SplayTree::splay(Node * v)
{
    while (v->parent) {
        Node * g = v->parent->parent;
        if (g != nullptr) {
            if (v->parent == (v == v->parent->left ? g->left : g->right)) {
                rotate(v->parent);
            }
            else {
                rotate(v);
            }
        }
        rotate(v);
    }
    return v;
}

SplayTree::Node * SplayTree::max(Node * v) const
{
    Node * cur = v;
    while (cur->right) {
        cur = cur->right;
    }
    return splay(cur);
}

SplayTree::Node * SplayTree::merge(Node * l, Node * r)
{
    if (l == nullptr) {
        return r;
    }
    if (r == nullptr) {
        return l;
    }
    Node * cur = max(l);
    cur->right = r;
    cur->right->parent = cur;
    return cur;
}

SplayTree::Node * SplayTree::find(int value) const
{
    if (!root) {
        return nullptr;
    }
    Node * m = root;
    Node * best = nullptr;
    Node * deepest;
    while (m) {
        deepest = m;
        if (m->value <= value) {
            best = m;
            m = m->right;
        }
        else {
            m = m->left;
        }
    }
    return const_cast<SplayTree *>(this)->root = splay(best ? best : deepest);
}

bool SplayTree::contains(int value) const
{
    Node * cur = find(value);
    return cur != nullptr && cur->value == value;
}

bool SplayTree::insert(int value)
{
    if (root == nullptr) {
        root = new Node(value);
        ++tree_size;
        return true;
    }
    if (contains(value)) {
        return false;
    }
    Node * v = new Node(value);
    if (root->value > value) {
        root->parent = v;
        v->right = root;
    }
    else {
        if (root->right) {
            v->right = root->right;
            root->right->parent = v;
            root->right = nullptr;
        }
        root->parent = v;
        v->left = root;
    }
    root = v;
    ++tree_size;
    return true;
}

bool SplayTree::remove(int value)
{
    if (!contains(value)) {
        return false;
    }
    Node * cur = find(value);
    if (cur == nullptr) {
        return false;
    }
    if (cur->left) {
        cur->left->parent = nullptr;
    }
    if (cur->right) {
        cur->right->parent = nullptr;
    }
    root = merge(cur->left, cur->right);
    cur->left = nullptr;
    cur->right = nullptr;
    delete cur;
    --tree_size;
    return true;
}

std::size_t SplayTree::size() const
{
    return tree_size;
}

bool SplayTree::empty() const
{
    return tree_size == 0;
}

void SplayTree::dfs(Node * v, std::vector<int> & data) const
{
    if (v == nullptr) {
        return;
    }
    dfs(v->left, data);
    data.push_back(v->value);
    dfs(v->right, data);
}

std::vector<int> SplayTree::values() const
{
    std::vector<int> ret;
    ret.reserve(size());
    dfs(root, ret);
    return ret;
}
