#pragma once

#include <vector>

class SplayTree
{
public:
    bool contains(int value) const;
    bool insert(int value);
    bool remove(int value);

    std::size_t size() const;
    bool empty() const;

    std::vector<int> values() const;

    ~SplayTree();

private:
    struct Node;

private:
    std::size_t tree_size = 0;
    Node * root = nullptr;

    static void rotate(Node * v);
    Node * max(Node * v) const;
    static Node * splay(Node * v);
    Node * merge(Node * l, Node * r);
    Node * find(int value) const;
    void dfs(Node * v, std::vector<int> & data) const;
};
