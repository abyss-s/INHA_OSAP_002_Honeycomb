﻿#include "../header/avl_tree.h"
#include <gtest/gtest.h>
#include "../header/bst_tree.h"
#include "../source/avl_tree.cc"
#include "../source/bst_tree.cc"
// SetUp함수 실행 후 테스트 초기 상태
//   1
//    \
//     2

TEST(AVLTreeTest, ConstructorTest) {
  AVLTree<int> avl;

  int size = avl.getSize();

  ASSERT_EQ(0, size);
  ASSERT_EQ(nullptr, avl.getRoot());
}
class AvlTreeTestFixture : public testing::Test {
 public:
  AvlTreeTestFixture();
  virtual ~AvlTreeTestFixture();
  void SetUp() override;
  void TearDown() override;

 protected:
  int getBalanceFactorWrapper(NodePtr<int> current_node) {
    return avl_.getBalanceFactor(current_node);
  }

 protected:
  AVLTree<int> avl_;
};
AvlTreeTestFixture::AvlTreeTestFixture() {}

AvlTreeTestFixture::~AvlTreeTestFixture() {}

void AvlTreeTestFixture::SetUp() {
  std::cout << "[SetUp] 1~2삽입\n";
  avl_.insert(1);
  avl_.insert(2);
}
void AvlTreeTestFixture::TearDown() {}

// 현재 상태(1~3 삽입)
//   2
//  / \
// 1   3
TEST_F(AvlTreeTestFixture, recursiveInsertTest) {
  this->avl_.recursiveInsert(avl_.getRoot(), 3);
  ASSERT_EQ(1, this->avl_.findDepthByValue(3));
}

// 현재 상태(1~3 삽입)
//   2
//  / \
// 1   3
TEST_F(AvlTreeTestFixture, insertTest_General) {
  this->avl_.insert(3);
  ASSERT_EQ(1, this->avl_.findDepthByValue(1));
  ASSERT_EQ(0, this->avl_.findDepthByValue(2));
  ASSERT_EQ(1, this->avl_.findDepthByValue(3));
}

// 현재 상태(1~4 삽입)
//   2
//  / \
// 1   3
//      \
//       4
TEST_F(AvlTreeTestFixture, getBalanceFactorTest) {
  this->avl_.insert(3);
  this->avl_.insert(4);
  ASSERT_EQ(-1, this->avl_.getBalanceFactor(this->avl_.getRoot()));
}

TEST_F(AvlTreeTestFixture, BalancingTest_RR) {
  this->avl_.insert(3);
  this->avl_.insert(4);
  this->avl_.insert(5);
  ASSERT_EQ(1, this->avl_.findDepthByValue(1));
  ASSERT_EQ(0, this->avl_.findDepthByValue(2));
  ASSERT_EQ(2, this->avl_.findDepthByValue(3));
  ASSERT_EQ(1, this->avl_.findDepthByValue(4));
  ASSERT_EQ(2, this->avl_.findDepthByValue(5));
}

TEST_F(AvlTreeTestFixture, BalancingTest_RL) {
  this->avl_.insert(3);
  this->avl_.insert(5);
  this->avl_.insert(4);
  ASSERT_EQ(1, this->avl_.findDepthByValue(1));
  ASSERT_EQ(0, this->avl_.findDepthByValue(2));
  ASSERT_EQ(2, this->avl_.findDepthByValue(3));
  ASSERT_EQ(1, this->avl_.findDepthByValue(4));
  ASSERT_EQ(2, this->avl_.findDepthByValue(5));
}

TEST_F(AvlTreeTestFixture, BalancingTest_LL) {
  this->avl_.insert(3);
  this->avl_.insert(-1);
  this->avl_.insert(-2);
  ASSERT_EQ(2, this->avl_.findDepthByValue(1));
  ASSERT_EQ(0, this->avl_.findDepthByValue(2));
  ASSERT_EQ(1, this->avl_.findDepthByValue(3));
  ASSERT_EQ(1, this->avl_.findDepthByValue(-1));
  ASSERT_EQ(2, this->avl_.findDepthByValue(-2));
}

TEST_F(AvlTreeTestFixture, BalancingTest_LR) {
  this->avl_.insert(3);
  this->avl_.insert(-2);
  this->avl_.insert(-1);
  ASSERT_EQ(2, this->avl_.findDepthByValue(1));
  ASSERT_EQ(0, this->avl_.findDepthByValue(2));
  ASSERT_EQ(1, this->avl_.findDepthByValue(3));
  ASSERT_EQ(1, this->avl_.findDepthByValue(-1));
  ASSERT_EQ(2, this->avl_.findDepthByValue(-2));
}

TEST_F(AvlTreeTestFixture, eraseTest) {
  this->avl_.insert(3);
  this->avl_.insert(4);

  int key = 3;
  cout << "삭제할 키: " << key << "\n";
  int size_before = avl_.getSize();
  int depth_before = avl_.findDepthByValue(key);
  cout << "삭제 이전 depth: " << depth_before << "\n";
  cout << "삭제 이전 size: " << size_before << "\n";

  int erase_result = avl_.erase(key);
  if (erase_result != 0) {
    int size_after = avl_.getSize();
    int depth_after = avl_.findDepthByValue(key);
    cout << "삭제 이후 depth: " << depth_after << "\n";
    cout << "삭제 이후 size: " << size_after << "\n";
    EXPECT_EQ(size_after, size_before - 1);
  } else {
    cout << "에러: 키가 존재하지 않음\n";
  }
}

TEST_F(AvlTreeTestFixture, RotateLeftTest) {
  // insert(3)
  //   2
  //  / \
  // 1   3
  avl_.insert(3);

  // roateLeft()
  //     3
  //    /
  //   2
  //  /
  // 1
  avl_.setRoot(avl_.rotateLeft(avl_.getRoot()));

  cout << "key 3's leftchild :" << avl_.IsKey(3)->left->key << '\n';
  ASSERT_EQ(2, this->avl_.IsKey(3)->left->key);
}

TEST_F(AvlTreeTestFixture, RotateRightTest) {
  // insert(3)
  //   2
  //  / \
  // 1   3
  avl_.insert(3);

  // roateRight()
  //     1
  //      \
  //       2
  //        \
  //         3
  avl_.setRoot(avl_.rotateRight(avl_.getRoot()));

  cout << "key 3's rightchild :" << avl_.IsKey(1)->right->key << '\n';
  ASSERT_EQ(2, this->avl_.IsKey(1)->right->key);
}