//
//  main.cpp
//  lab7
//
//  Created by Le Tran Ngoc My on 9/10/20.
//  Copyright © 2020 Le Tran Ngoc My. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>

template <typename T, int M>
class BTreeNode {
//friend class BTree<T, M>;
public:
    
    BTreeNode() {
        leaf = true;
        keyTally = 0;
        for (int i = 0; i < M-1; i++) {
            keys[i] = 0;
            pointers[i] = nullptr;
        }
        pointers[M-1] = nullptr;
    }
    
    BTreeNode(const T& el) {
        keys[0] = el;
        keyTally = 1;
        leaf = true;
        pointers[0] = nullptr;
        for (int i = 1; i < M-1; i++) {
            keys[i] = 0;
        }
        pointers[M-1] = nullptr;
    }
    
    bool contains(const T& key) {
    for (int i = 0; i < keyTally; i++) {
        if (key == keys[i]) {
            return true;
        }
    }
    return false;
    }
    
    BTreeNode<T,M>* nextNode(const T& key) {
        if (key > keys[keyTally-1]) {
            return pointers[keyTally];
        } else if (key < keys[0]){
            return pointers[0];
        } else {
            for (int i = 1; i < keyTally ; i++) {
            if (key < keys[i] && key > keys[i-1]) {
                return pointers[i];
            }
        }
        }
        return nullptr;
    }
    
    BTreeNode<T,M>* insertKey(const T& key, BTreeNode<T,M>* &leaf) {
        if (leaf->keyTally < M-1) {
            if (key > leaf->keys[leaf->keyTally-1]) {
                leaf->keys[leaf->keyTally] = key;
            } else {
            bool inserted = false;
            for (int i = 0; i < leaf->keyTally; i++) {
                if (key < leaf->keys[i] && !inserted) {
                    for (int j = leaf->keyTally-1; j > i-1; j--) {
                        leaf->keys[j+1] = leaf->keys[j];
                    }
                    leaf->keys[i] = key;
                    inserted = true;
                }
            }
            }
            leaf->keyTally++;
            return leaf;
        } else {
            int m;
            if (M%2 == 0) {
                m = M/2;
            } else {
                m = (M-1)/2;
            }
            std::vector<int> ints;
            int i = 0;
            while (leaf->keys[i] < key && i < leaf->keyTally) {
                ints.push_back(leaf->keys[i]);
                i++;
            }
            ints.push_back(key);
            i++;
            while (i <= leaf->keyTally) {
                ints.push_back(leaf->keys[i-1]);
                i++;
            }
            BTreeNode<T,M>* newNode = new BTreeNode<T,M>();
            int tomove;
            for (int j = 0; j < ints.size(); j++) {
                if (j < m) {
                    leaf->keys[j] = ints[j];
                } else if (j == m) {
                    tomove = ints[j];
                    leaf->keys[j] = 0;
                } else {
                    newNode->keys[j-m-1] = ints[j];
                    if (j != ints.size()-1) {
                        leaf->keys[j] = 0;
                    }
                }
            }
            leaf->keyTally = m;
            newNode->keyTally = ints.size() - m - 1;
            BTreeNode<T,M>* newRoot = new BTreeNode<T,M>(tomove);
            for (int j = 0; j < M; j++) {
                leaf->pointers[j] = nullptr;
            }
            newRoot->pointers[0] = leaf;
            newRoot->pointers[1] = newNode;
            newRoot->leaf = false;
            for (int j = 2; j < M; j++) {
                newRoot->pointers[j] = nullptr;
            }
            return newRoot;
        }
    }

    void removeKey(const T& key) {
        for (int i = 0; i < keyTally-1; i++) {
            if (keys[i] >= key) {
                keys[i] = keys[i+1];
            }
        }
        keys[keyTally-1] = 0;
        keyTally--;
    }
    
    void traverse() {
        if (leaf) {
            for (int i = 0; i < keyTally; i++) {
                std::cout << keys[i] << " ";
            }
        } else {
            for (int i = 0; i < keyTally; i++) {
                pointers[i]->traverse();
                std::cout << keys[i] << " ";
            }
            pointers[keyTally]->traverse();
        }
    }
    
    void snapShot(std::ostream& out) {
        out << '\"' << keys[0];
        for (int n = 1; n < keyTally; n++) {
          out << ':' << keys[n];
        }
        out << '\"';
        if (!leaf) {
          out << '[';
          pointers[0]->snapShot(out);
          for (int n = 1; n <= keyTally; n++) {
            out << ',';
            pointers[n]->snapShot(out);
          }
          out << ']';
        }
    }

//private:
    bool leaf;
    int keyTally;
    T keys[M - 1];
    BTreeNode* pointers[M];
    
};

template <typename T, int M>
class BTree {
public:
    
    BTree() {
        root = nullptr;
    }
    
    bool contains(const T& key, BTreeNode<T,M>* &node) {
        if (node->contains(key)) {
            return true;
        }
        while (!node->leaf) {
            node = node->nextNode(key);
            return contains(key, node);
        }
        return false;
    }
    
    std::vector<BTreeNode<T,M>*> getPath(const T& key) {
        BTreeNode<T,M>* rootx;
        rootx = root;
        if (!contains(key,rootx)) {
        std::vector<BTreeNode<T,M>*> path;
        path.push_back(root);
            BTreeNode<T,M>* node = root;
            while (!node->leaf) {
                node = node->nextNode(key);
                path.push_back(node);
            }
        return path;
        } else {
            std::vector<BTreeNode<T,M>*> path;
            BTreeNode<T,M>* node = root;
            path.push_back(root);
                while (!node->contains(key)) {
                    node = node->nextNode(key);
                    path.push_back(node);
                }
            return path;
        }
    }
    
    BTreeNode<T,M>* getLeaf(const T& key) {
        BTreeNode<T,M>* rootx;
        rootx = root;
        if (!contains(key,rootx)) {
            return rootx;
        } else {
            BTreeNode<T,M>* node = root;
            while (!node->leaf) {
                if (!node->contains(key)) {
                    node = node->nextNode(key);
                } else {
                    for (int i = 0; i < node->keyTally; i++) {
                        if (node->keys[i] == key) {
                            if (contains(key,node->pointers[i])) {
                                node = node->pointers[i];
                            } else {
                                node = node->pointers[i+1];
                            }
                        }
                    }
                }
            }
            return node;
        }
    }
    
    void insert(const T& key) {
        bool inserted = false;
        if (root == nullptr) {
            root = new BTreeNode<T,M>(key);
        } else {
        BTreeNode<T,M>* rootx;
        rootx = root;
        if (!contains(key,rootx)) {
            BTreeNode<T,M>* insertedNode = rootx->insertKey(key, rootx);
            if (rootx == root) {
                root = insertedNode;
            } else {
                if (insertedNode->keyTally > 1) {
                    BTreeNode<T,M>* n;
                    n = getLeaf(key);
                    n = insertedNode;
                } else {
                    std::vector<BTreeNode<T,M>*> path;
                    path = getPath(key);
                    int i = (int) path.size()-2;
                    bool quit = false;
                    for (; i>=0; i--) {
                        if (path[i]->keyTally < M-1) {
                            if (!quit) {
                            path[i] = path[i]->insertKey(insertedNode->keys[0],path[i]);
                            for (int j = path[i]->keyTally-1; j >= 0; j--) {
                            if (!inserted) {
                                if (path[i]->keys[j] > insertedNode->keys[0]) {
                                    path[i]->pointers[j+1] = path[i]->pointers[j];
                                } else if (path[i]->keys[j] == insertedNode->keys[0]) {
                                    path[i]->pointers[j+1] = insertedNode->pointers[1];
                                    path[i]->pointers[j] = insertedNode->pointers[0];
                                    inserted = true;
                                }
                            }
                            }
                            quit = true;
                            }
                        } else {
                            if (!quit) {
                            std::vector<BTreeNode<T,M>*> ptrs;
                            int j = 0;
                            while (path[i]->keys[j] < insertedNode->keys[0]) {
                                ptrs.push_back(path[i]->pointers[j]);
                                j++;
                            }
                            ptrs.push_back(insertedNode->pointers[0]);
                            ptrs.push_back(insertedNode->pointers[1]);
                            j++;
                            while (j < M) {
                                ptrs.push_back(path[i]->pointers[j]);
                                j++;
                            }
                            path[i] = path[i]->insertKey(insertedNode->keys[0],path[i]);
                            path[i]->pointers[1]->leaf = false;
                            if (ptrs.size()%2 == 0) {
                                for (int m = 0; m < ptrs.size()/2; m++) {
                                    path[i]->pointers[0]->pointers[m] = ptrs[m];
                                    path[i]->pointers[1]->pointers[m] = ptrs[m + ptrs.size()/2];
                                }
                                for (int m = ptrs.size()/2; m < M; m++) {
                                    path[i]->pointers[0]->pointers[m] = nullptr;
                                    path[i]->pointers[1]->pointers[m] = nullptr;
                                }
                            } else {
                                for (int m = 0; m < (ptrs.size()-1)/2; m++) {
                                    path[i]->pointers[0]->pointers[m] = ptrs[m];
                                    path[i]->pointers[1]->pointers[m] = ptrs[m + (ptrs.size()+1)/2];
                                }
                                for (int m = (ptrs.size()-1)/2; m < M; m++) {
                                    if (m == (ptrs.size()-1)/2) {
                                        path[i]->pointers[0]->pointers[m] = ptrs[m];;
                                    }
                                    path[i]->pointers[0]->pointers[m] = nullptr;
                                    path[i]->pointers[1]->pointers[m] = nullptr;
                                }
                            }
                            }
                        }
                        insertedNode = path[i];
                    }
                root = path[0];
                }
            }
        }
        }
    }
    
    void remove(T& key){
        BTreeNode<T,M>* rootx;
        rootx = root;
        if (contains(key,rootx)) {
            std::vector<BTreeNode<T,M>*> path = getPath(key);
            std::vector<BTreeNode<T,M>*> paths;
            int s;
            if (!path[path.size()-1]->leaf) {
                bool found = false;
                int i = 1;
                while (!found) {
                    BTreeNode<T,M>* rootx1;
                    rootx1 = root;
                    BTreeNode<T,M>* rootx2;
                    rootx2 = root;
                    if (contains(key-i,rootx1)) {
                        s = key-i;
                        found = true;
                    } else if (contains(key+i,rootx2)) {
                        s = key+i;
                        found = true;
                    }
                    paths = getPath(s);
                    if (!paths[paths.size()-1]->leaf) {
                        found = false;
                    }
                    i++;
                }
                for (int j = 0; j < path[path.size()-1]->keyTally; j++) {
                    if (path[path.size()-1]->keys[j] == key) {
                        path[path.size()-1]->keys[j] = s;
                    }
                }
                paths[paths.size()-1]->removeKey(s);
                root = path[0];
            } else {
                path[path.size()-1]->removeKey(key);
            }
            int m;
            if ((M-1)%2 == 0) {
                m = (M-1)/2;
            } else {
                m = M/2;
            }
            bool quit = false;
            BTreeNode<T,M>* node;
            BTreeNode<T,M>* parent;
            if (paths.size() != 0) {
                path = paths;
            }
            for (int e = path.size()-1; e >= 1; e--) {
            node = path[e];
            parent = path[e-1];
            if (node != root && !quit) {
            if (node->keyTally < m) {
                BTreeNode<T,M>* left;
                BTreeNode<T,M>* right;
                int indexp;
                if (node->keys[0] > parent->keys[parent->keyTally-1]) {
                    indexp = parent->keyTally-1;
                    left = parent->pointers[parent->keyTally-1];
                    right = nullptr;
                } else {
                    bool founds = false;
                    int k = 0;
                    while (!founds) {
                        if (node->keys[0] < parent->keys[k]) {
                            if (k == 0) {
                                left = nullptr;
                                right = parent->pointers[k+1];
                            } else {
                                left = parent->pointers[k-1];
                                right = parent->pointers[k+1];
                            }
                            founds = true;
                            indexp = k;
                        }
                        k++;
                    }
                }
                if ((left != nullptr && left->keyTally > m) || (right != nullptr && right->keyTally > m)) {
                    if (right != nullptr && right->keyTally > m) {
                        node = node->insertKey(parent->keys[indexp], node);
                        parent->removeKey(parent->keys[indexp]);
                        parent = node->insertKey(right->keys[0], parent);
                        right->removeKey(right->keys[0]);
                        if (!node->leaf) {
                            node->pointers[node->keyTally] = right->pointers[0];
                            for (int t = 0; t <= right->keyTally; t++) {
                                right->pointers[t] = right->pointers[t+1];
                            }
                            right->pointers[right->keyTally+1] = nullptr;
                        }
                    } else {
                        node = node->insertKey(parent->keys[indexp], node);
                        parent->removeKey(parent->keys[indexp]);
                        parent = parent->insertKey(left->keys[left->keyTally-1], parent);
                        left->removeKey(left->keys[left->keyTally-1]);
                        if (!node->leaf) {
                            for (int t = node->keyTally; t > 0; t--) {
                                node->pointers[t] = node->pointers[t-1];
                            }
                            node->pointers[0] = left->pointers[left->keyTally+1];
                            left->pointers[left->keyTally+1] = nullptr;
                        }
                    }
                    quit = true;
                } else if ((left != nullptr && left->keyTally == m) || (right != nullptr && right->keyTally == m)) {
                    if (right != nullptr && right->keyTally == m) {
                        node = node->insertKey(parent->keys[indexp], node);
                        for (int t = 0; t < right->keyTally; t++) {
                            node = node->insertKey(right->keys[t], node);
                        }
                        parent->removeKey(parent->keys[indexp]);
                        while (indexp < parent->keyTally) {
                            parent->pointers[indexp+1] = parent->pointers[indexp+2];
                            indexp++;
                        }
                        if (!node->leaf) {
                            for (int t = right->keyTally; t >= 0; t--) {
                                node->pointers[M-1-(right->keyTally-t)] = right->pointers[t];
                            }
                        }
                        parent->pointers[indexp+1] = nullptr;
                    } else {
                        if (!node->leaf) {
                            for (int t = node->keyTally; t >= 0; t--) {
                                node->pointers[M-1-(node->keyTally-t)] = node->pointers[t];
                            }
                            for (int t = 0; t <= left->keyTally; t++) {
                                node->pointers[t] = left->pointers[t];
                            }
                        }
                        node = node->insertKey(parent->keys[indexp], node);
                        for (int t = 0; t < left->keyTally; t++) {
                            node = node->insertKey(left->keys[t], node);
                        }
                        parent->removeKey(parent->keys[indexp]);
                        while (indexp-1 < parent->keyTally) {
                            parent->pointers[indexp] = parent->pointers[indexp+1];
                            indexp++;
                        }
                        parent->pointers[indexp] = nullptr;
                    }
                    if (parent->keys[0] == 0) {
                        root = parent = node;
                    }
                }
            } else {
                quit = true;
            }
            }
            }
        }
    }
    
    //inorder
    void traverse() {
        root->traverse();
    }
    
    void snapShot(std::ostream& out) {
        out << "TreeForm[";
        root->snapShot(out);
        out << ']' << std::endl;
    }
    
private:
    BTreeNode<T,M>* root;
    
};

int main(int argc,const char* argv[]) {
    
    BTree<int,5> x;
    
    std::vector<int> input;
    std::vector<int> toremove;
    
    int n;
    std::ifstream in;
    in.open("/Users/letranngocmy/XCodeProjects/CMSC 270/lab7/lab7/input.txt");
    while(in >> n) input.push_back(n);
    in.close();
    
    in.open("/Users/letranngocmy/XCodeProjects/CMSC 270/lab7/lab7/remove.txt");
    while(in >> n) toremove.push_back(n);
    in.close();
    
    for (int k = 0; k < input.size(); k++) {
        //int t = input[k];
        x.insert(input[k]);
    }
    
    for (int k = 0; k < toremove.size(); k++) {
        int t = toremove[k];
        x.remove(toremove[k]);
    }
    
    std::ofstream out1("/Users/letranngocmy/XCodeProjects/CMSC 270/lab7/lab7/tree.txt");
    x.snapShot(out1);
    out1.close();
    
    x.traverse();
    
    return 0;
}
