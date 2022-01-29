#include <iostream>
#include <fstream>
#include <vector>

template<class T>
class AVLNode {
public:
    
    AVLNode() {
        left = right = nullptr;
        balance = 0;
    }
    
    AVLNode(const T& el, AVLNode *l = nullptr, AVLNode *r = nullptr) {
        key = el; left = l; right = r;
        balance = 0;
    }
    
    bool operator==(const AVLNode& n) const {
      return key == n.key;
    }
    
    bool operator!=(const AVLNode& n) const {
      return key != n.key;
    }
    
    T key;
    int balance;
    AVLNode *left, *right;
    
};

template<class T>
class AVLTree {
public:
  AVLTree() { root = nullptr;}
  
  void insert(const T& key) { insert(root,key); }
  
  void snapShot(std::ostream& out) {
    out << "TreeForm[";
    snapShot(out,root);
    out << "]" << std::endl; }
private:
  AVLNode<T>* root;

  void rightRotation(AVLNode<T>* &node) {
    AVLNode<T>* left = node->left;
    node->left = left->right;
    left->right = node;
    node = left;
  }

  void leftRotation(AVLNode<T>* &node) {
    AVLNode<T>* right = node->right;
    node->right = right->left;
    right->left = node;
    node = right;
  }
    
  void LRRotation(AVLNode<T>* &node) {
    AVLNode<T>* left = node->left;
    AVLNode<T>* leftright = left->right;
    left->right = leftright->left;
    node->left = leftright->right;
    leftright->left = left;
    leftright->right = node;
    node = leftright;
  }
    
  void RLRotation(AVLNode<T>* &node) {
    AVLNode<T>* right = node->right;
    AVLNode<T>* rightleft = right->left;
    right->left = rightleft->right;
    node->right = rightleft->left;
    rightleft->right = right;
    rightleft->left = node;
    node = rightleft;
  }
    
  int height(AVLNode<T>* &node) {
    int h = 0;
    while (node != nullptr) {
        h++;
        if (node->balance == -1) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return h;
  }
    
  void correctBalance(AVLNode<T>* &tochange) {
    AVLNode<T> tochangex = *tochange;
    int lh = height(tochangex.left);
    int rh = height(tochangex.right);
    if (lh == rh) {
        tochange->balance = 0;
    } else if (lh > rh) {
        tochange->balance = -1;
    } else {
        tochange->balance = 1;
    }
  }

  void insert(AVLNode<T>* &node,const T& key) {
    if(node == nullptr) {
      node = new AVLNode<T>(key);
      //return true;
    } else {
        AVLNode<T>* newNode = new AVLNode<T>(key);
        std::vector<AVLNode<T>*> path;
        path.push_back(node);
        //bool increase = false;
        while ((node->left != nullptr && key < node->key && node->left != newNode) || (node->right != nullptr && key > node->key && node->right != newNode)) {
            if (key > node->key) {
                node = node->right;
            } else {
                node = node->left;
            }
            path.push_back(node);
        }
        for (int i = 0; i < path.size(); i++) {
            correctBalance(path[i]);
        }
        if (key > node->key) {
            if (node->balance == -1) {
                node->right = newNode;
                node->balance = 0;
            } else if (node->balance == 0) {
                node->balance = 1;
                node->right = newNode;
            }
        } else if (key < node->key) {
            if (node->balance == 1) {
                node->left = newNode;
                node->balance = 0;
            } else if (node->balance == 0) {
                node->left = newNode;
                node->balance = -1;
            }
        }
        root = node;
        if (path.size() > 1) {
        AVLNode<T>* tochange;
        for (int k = path.size(); k > 1; k--) {
            AVLNode<T>* updated = root;
            tochange = path[k-2];
            if (tochange->key < updated->key) {
                tochange->right = updated;
            } else {
                tochange->left = updated;
            }
            if (tochange->balance == 0 && updated->balance != 0) {
                if (tochange->right == updated) {
                    tochange->balance = 1;
                } else {
                    tochange->balance = -1;
                }
            } else if (tochange->balance == -1 && updated->balance != 0) {
                if (updated->balance == -1) {
                    AVLNode<T> tochangex = *tochange;
                    AVLNode<T>* x = &tochangex;
                    int d = height(x->right) - height(x->left);
                    if (d > 1 || d < -1) {
                    updated->balance = 0;
                    tochange->balance = 0;
                    rightRotation(tochange);
                    }
                } else {
                    AVLNode<T> tochangex = *tochange;
                    AVLNode<T>* x = &tochangex;
                    correctBalance(x);
                    if (x->balance != 0) {
                    if (tochange->right == nullptr) {
                        updated->balance = 0;
                        tochange->balance = 0;
                        LRRotation(tochange);
                    } else {
                        int d = height(x->right) - height(x->left);
                        if (d > 1 || d < -1) {
                        LRRotation(tochange);
                        tochange->balance = 0;
                        correctBalance(tochange->left);
                        correctBalance(tochange->right);
                        }
                    }
                    } else {
                        correctBalance(tochange);
                    }
                }
            } else if (tochange->balance == 1 && updated->balance != 0) {
                if (updated->balance == 1) {
                    AVLNode<T> tochangex = *tochange;
                    AVLNode<T>* x = &tochangex;
                    int d = height(x->right) - height(x->left);
                    if (d > 1 || d < -1) {
                    updated->balance = 0;
                    tochange->balance = 0;
                    leftRotation(tochange);
                    }
                } else {
                    AVLNode<T> tochangex = *tochange;
                    AVLNode<T>* x = &tochangex;
                    correctBalance(x);
                    if (x->balance != 0) {
                    if (tochange->left == nullptr) {
                        updated->balance = 0;
                        tochange->balance = 0;
                        RLRotation(tochange);
                    } else {
                        int d = height(x->right) - height(x->left);
                        if (d > 1 || d < -1) {
                        RLRotation(tochange);
                        tochange->balance = 0;
                        correctBalance(tochange->left);
                        correctBalance(tochange->right);
                        }
                    }
                    } else {
                        correctBalance(tochange);
                    }
                }
            } else {
            correctBalance(tochange);
            }
            root = tochange;
        }
        }
    }
  }
  
  void snapShot(std::ostream& out,AVLNode<T> *p) {
    out << '\"' << p->key << '\"';
    if(p->left != nullptr || p->right != nullptr) {
      out << '[';
    if(p->left==nullptr)
      out << "\"\"";
    else
      snapShot(out,p->left);
    out << ',';
    if(p->right==nullptr)
      out << "\"\"";
    else
      snapShot(out,p->right);
    out << ']';
    }
  }
};


int main(int argc,const char* argv[]) {

    AVLTree<int> x;
    std::vector<int> v;
    
    int n;
    std::ifstream in; // Input file stream object
    in.open("/Users/letranngocmy/XCodeProjects/CMSC 270/lab6/lab6/numbers.txt"); // Open the data file
    while(in >> n) v.push_back(n);
    in.close();
    
    for (int k = 0; k < v.size(); k++) {
        //int t = v[k];
        x.insert(v[k]);
    }
    
    std::ofstream out("/Users/letranngocmy/XCodeProjects/CMSC 270/lab6/lab6/tree.txt");
    x.snapShot(out);
    out.close();
    
    return 0;
}
