#include <iostream>

#ifndef DOUBLY_LINKED_LIST
#define DOUBLY_LINKED_LIST

template<class T>
class DLLNode {
public:
    DLLNode() {
        next = prev = nullptr;
    }
    DLLNode(const T& el) {
        info = el;
    }
    DLLNode(const T& el, DLLNode *n = nullptr, DLLNode *p = nullptr) {
        info = el; next = n; prev = p;
    }
    T info;
    DLLNode *next, *prev;
};

template<class T>
class DoublyLinkedList {
public:
    DoublyLinkedList() {
        head = tail = nullptr;
    }
    ~DoublyLinkedList() {
        clear();
    }
    bool isEmpty() const {
        return head == nullptr;
    }
    void clear();
    void setToNull() {
        head = tail = nullptr;
    }
    void addInMiddle(const T&);
    
    // Inner class iterator
    class iterator {
    friend class DoublyLinkedList;
    private:
      DLLNode<T> *nodePtr;
      // The constructor is private, so only our friends
      // can create instances of iterators.
      iterator(DLLNode<T> *newPtr) : nodePtr(newPtr) {}
    public:
      iterator() : nodePtr(nullptr) {}

      // Overload for the comparison operator !=
      bool operator!=(const iterator& itr) const {
        return nodePtr != itr.nodePtr;
      }

      // Overload for the dereference operator *
      T& operator*() const {
          return nodePtr->info;
      }

      // Overload for the postincrement operator ++
      iterator operator++(int) {
        iterator temp = *this;
        nodePtr = nodePtr->next;
        return temp;
      }
    }; // End of inner class iterator
    
    iterator begin() const {
      return iterator(head);
    }
    
    iterator end() const {
      return iterator(tail);
    }
    
    iterator insert(iterator position,const T& value) {
      DLLNode<T>* newNode = new DLLNode<T>(value, nullptr, nullptr);
      if (head == nullptr) {
          head = newNode;
          tail = newNode->next = nullptr;
      } else {
          if (position.nodePtr == head) {
              head = newNode;
              newNode->next = position.nodePtr;
              position.nodePtr->prev = newNode;
          } else if (position.nodePtr == tail) {
              DLLNode<T> *tmp = head;
              while (tmp->next != tail) {
                  tmp = tmp->next;
              }
              newNode->prev = tmp;
              tmp->next = newNode;
              tail = newNode->next = nullptr;
          } else {
              newNode->prev = position.nodePtr->prev;
              newNode->next = position.nodePtr;
              position.nodePtr->prev->next = newNode;
              position.nodePtr->prev = newNode;
          }
      }
      position.nodePtr = newNode;
      return position;
    }

    iterator erase(iterator position) {
      if (head != nullptr) {
      DLLNode<T> *toDelete = position.nodePtr;
      if (position.nodePtr->next != tail) {
          position.nodePtr = position.nodePtr->next;
      } else if (position.nodePtr == head) {
          position.nodePtr = nullptr;
      } else {
          position.nodePtr = position.nodePtr->prev;
      }
      if (toDelete == head) {
          head = toDelete->next;
          toDelete->next->prev = nullptr;
      } else if (toDelete->next == tail) {
          tail = toDelete->prev->next = nullptr;
      } else {
          toDelete->prev->next = toDelete->next;
          toDelete->next->prev = toDelete->prev;
      }
      //position.nodePtr = position.nodePtr->next;
      delete toDelete;
      } else {
          position.nodePtr = nullptr;
      }
      return position;
    }
    
protected:
    DLLNode<T> *head, *tail;
    friend std::ostream& operator<<(std::ostream&, const DoublyLinkedList<T>&);
};

template<class T>
void DoublyLinkedList<T>::addInMiddle(const T& el) {
    if (head != nullptr) {
        if (head->next != nullptr) {
            int i = 1;
            DLLNode<T> *tmp;
            for (tmp = head; tmp->next != nullptr; i++, tmp = tmp->next);
            for (tmp = head, i = i/2; i; i--, tmp = tmp->next);
            tmp->prev = tmp->prev->next = new DLLNode<T>(el,tmp,tmp->prev);
        }
        else {
            head->next = new DLLNode<T>(el,nullptr,head);
            tail = head->next->next;
        }
    }
    else {
        head = new DLLNode<T>(el);
        tail = head->next = nullptr;
    }
}

template<class T>
void DoublyLinkedList<T>::clear() {
    for (DLLNode<T> *tmp; head != nullptr; ) {
        tmp = head;
        head = head->next;
        delete tmp;
    }
}

#endif

int main(int argc,const char* argv[])
{
  DoublyLinkedList<int> v;
  
  v.insert(v.end(),2); // 2
  v.insert(v.end(),4); // 2 4
  v.insert(v.end(),5); // 2 4 5
  
  auto iter = v.begin(); // Points to 2
  iter = v.insert(iter, 1); // Insert 1 before 2 // 1 2 4 5 // Points to 1
  iter++; // Points to 2
  v.insert(iter++,3); // Insert 3 before 2 // 1 3 2 4 5 // Points to 4
  iter++; // Points to 5
  v.insert(iter,10); // Insert 10 before 5 // 1 3 2 4 10 5 // Points to 10

  iter = v.begin(); // Points to 1
  iter++; // Points to 3
  iter = v.erase(iter); // Erase 3 // 1 2 4 10 5 // Points to 2
  iter++; // Points to 4
  v.erase(iter); // Erase 4 // Points to 10 // 1 2 10 5
  
  for(auto itr = v.begin();itr != v.end();itr++)
    std::cout << *itr << std::endl;
    
  return 0;
}
