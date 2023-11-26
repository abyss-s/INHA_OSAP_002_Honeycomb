﻿#include "../header/bst_tree.h"
#include <algorithm>
#include <iostream>

using namespace std;

/* Constructor */
template <typename T>
BinarySearchTree<T>::BinarySearchTree() : root_(nullptr), size_(0){};

/* Return size of BST */
template <typename T>
int BinarySearchTree<T>::getSize() {
  return this->size_;
}

/* Increase size of BST */
template <typename T>
void BinarySearchTree<T>::addSize() {
  this->size_ += 1;
}

/* Return true if BST is empty */
template <typename T>
bool BinarySearchTree<T>::isEmpty() {
  return this->root_ == nullptr;
}

/* Return true if any node has item as key value */
template <typename T>
NodePtr<T> BinarySearchTree<T>::IsKey(int item) {
  NodePtr<T> t = this->root_;

  /*key값을 찾거나 없다면 break*/
  while (t != nullptr && t->key != item) {
    t = (item < t->key) ? t->left : t->right;
  }
  return t;
}

/* Return depth of the node with item as key */
template <typename T>
int BinarySearchTree<T>::findDepthByValue(int item) {
  if (!IsKey(item)) {
    return 0;
  }
  NodePtr<T> t = this->root_;
  int depth = 0;

  /*key값을 찾거나 없다면 break*/
  while (t != nullptr && t->key != item) {
    t = (item < t->key) ? t->left : t->right;
    depth++;
  }
  return depth;
}

/* find min: item보다 작은 값 중 가장 작은 값을 리턴 */
template <typename T>
T BinarySearchTree<T>::minimum(int item) {
  NodePtr<T> x = IsKey(item);
  while (x->left != nullptr) {
    x = x->left;
  }
  cout << x->key << " " << findDepthByValue(x->key) << "\n";
  return x->key;  // 최솟값 출력 뿐만 아니라 값을 리턴하도록 수정
}

/* find max */
template <typename T>
T BinarySearchTree<T>::maximum(int item) {
  NodePtr<T> x = IsKey(item);
  if (x == nullptr) {
    return T{};
  }
  while (x->right != nullptr) {
    x = x->right;
  }
  cout << x->key << " " << findDepthByValue(x->key) << "\n";
  return x->key;
}

/* Return a root of tree*/
template <typename T>
NodePtr<T> BinarySearchTree<T>::getRoot() {
  return this->root_;
}

/* insert helper function to use in main.cc*/
template <typename T>
int BinarySearchTree<T>::insert(int item) {
  if (IsKey(item)) {
    cout << item << " is already exists\n";
    return -1;
  }
  this->root_ = recursiveInsert(this->root_, item);
  this->size_ = this->size_ + 1;
  return this->findDepthByValue(item);
}

/* insert a node in bst tree*/
template <typename T>
NodePtr<T> BinarySearchTree<T>::recursiveInsert(NodePtr<T> node, int item) {
  if (node == nullptr) {
    NodePtr<T> z = new Node<T>;
    z->key = item;
    return z;
  } else if (node->key < item) {
    node->right = recursiveInsert(node->right, item);
  } else {
    node->left = recursiveInsert(node->left, item);
  }

  node->height =
      1 + max(getHeight(node->left), getHeight(node->right));  // 높이 업뎃
  return node;
}

template <typename T>
int BinarySearchTree<T>::getHeight(NodePtr<T> current_node) {
  if (current_node == nullptr)
    return 0;
  else
    return current_node->height;
}

template <typename T>
int BinarySearchTree<T>::countNodesSmallerThan(NodePtr<T> root, int target) {
  if (root == nullptr) {
    return 0;
  }

  if (target < root->key) {
    return countNodesSmallerThan(root->left, target);
  } else if (target > root->key) {
    // 현재 노드와 현재 노드의 왼쪽 서브트리에 있는 노드의 개수를 합산
    return 1 + countNodesSmallerThan(root->right, target) +
           countNodesSmallerThan(root->left, target);
  } else {
    // 현재 노드와 현재 노드의 왼쪽 서브트리에 있는 노드의 개수를 합산
    return countNodesSmallerThan(root->left, target);
  }
}

template <typename T>
int BinarySearchTree<T>::rank(NodePtr<T> root, int target) {
  // cout << findDepthByValue(target) << " "
  //      << countNodesSmallerThan(root, target) + 1 << '\n';
  return countNodesSmallerThan(root, target) + 1;
}

// countNodesSmallerThan 은 자신보다 낮은 노드 개수를 세주는 함수
// Rank는 자신보다 작은 노드 개수 + 1 = rank를 구하는 함수.

/* erase: 노드 삭제 후 depth 반환하기 */
template <typename T>
int BinarySearchTree<T>::erase(const T& key) {
  NodePtr<T> node = IsKey(key);
  if (node != nullptr) {
    int depth = this->findDepthByValue(key);
    eraseNode(this->root_, key);
    size_--;
    return depth;
  } else
    return 0;
}

/* eraseNode: 자식 이식 및 후임자를 활용한 실질적인 노드 삭제 수행 */
template <typename T>
void BinarySearchTree<T>::eraseNode(NodePtr<T>& root, const T& key) {
  if (root == nullptr) {
    return;  // 비어 있다면 넘기기
  }

  if (key < root->key) {
    eraseNode(root->left, key);
  } else if (key > root->key) {
    eraseNode(root->right, key);
  } else {
    if (root->left == nullptr) {
      transplant(root->right);
    } else if (root->right == nullptr) {
      transplant(root->left);
    } else {
      NodePtr<T> successor = findSuccessor(root->right);
      root->key = successor->key;
      eraseNode(root->right, successor->key);
    }

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
  }
}

/* Transplant: 자식 노드 이식 함수 */
template <typename T>
void BinarySearchTree<T>::transplant(NodePtr<T>& x) {
  NodePtr<T> y = x;  // 노드 삭제를 위해 y 사용

  if (x->left == nullptr) {
    x = x->right;
  } else if (x->right == nullptr) {
    x = x->left;
  } else {
    NodePtr<T> z = x->right;  // z : 삭제할 x의 다음으로 가장 작은 수
    NodePtr<T> pZ = x;        // p[z] : z의 부모 노드

    /* 오른쪽 자식중 가장 작은 값 찾기*/
    while (z->left != nullptr) {
      pZ = z;
      z = z->left;
    }

    x->key = z->key;  // successor과 key값 교환

    /* 오른쪽 자식이 가장 작다면 */
    if (pZ == x) {
      pZ->right = z->right;  // z의 오른쪽 자식 붙여주기
    } else {
      pZ->left = z->right;  // 오른쪽 자식의 왼쪽 자식이 있다면
    }  // 그 z(successor)의 오른쪽 자식 p[z]의 왼쪽에 붙여주기

    // 이식 후 높이 업데이트
    pZ->height = 1 + max(getHeight(pZ->left), getHeight(pZ->right));
    x = pZ;
  }

  // y를 삭제
  delete y;
}

/* findSuccessor: 후임자 찾기 역할 */
template <typename T>
NodePtr<T> BinarySearchTree<T>::findSuccessor(const NodePtr<T>& node) {
  NodePtr<T> current = node;
  while (current->left != nullptr) {
    current = current->left;
  }
  return current;
}
