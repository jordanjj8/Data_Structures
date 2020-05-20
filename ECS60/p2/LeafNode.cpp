#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p, BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize]; // values is array that is of size LSize, with a bunch of ints in it
}  // LeafNode()


// Gets minimum
// Used to find min value in current leaf node to give to left sibling
int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} //LeafNode::getMinimum()

// Gets maximum
// Used for when moving max values to right sibling
int LeafNode::getMaximum()const
{
  if(count > 0)  // should always be the case
    return values[count - 1]; // LSize, not 0
  else
    return 0;

}

// when left sibling is not full and can take one of your current leaf node values
void LeafNode::addToLeft(int value) {
  leftSibling->insert(value); // add the smallest number 
}

// when right sibling is not full and can take one of your current leaf node values
void LeafNode::addToRight(int value) {
  rightSibling->insert(value);
}

void LeafNode::addToCurrent(int value) {
  if (count == 0 || value > values[count - 1]) { // adds value to values if 1. empty, 2. greater than any value in values 
    values[count] = value; // adding value to front of array (values)
    //cout << "This is count: " << count << endl;
  }

  else { // runs if the value being added is less than the greatest value in values 
    for (int i = 0; i <= count - 1; i++) { // starts at index 0, runs through until max size of values 
      if (values[i] > value) { // executes if the current value in values is greater than value (so insert here!)
        for (int j = count; j > i; j--) { //shifts starting from the end of the values, till i  
          values[j] = values[j - 1];

        }
        values[i] = value;
        break;
      }
    }
  }
  count++; // add to count
  //cout << "Count should be incremented here: " << count << endl;
}

// Insert value into BTree
// insert function returns pointer to new Leaf if splits, else returns NULL 
LeafNode* LeafNode::insert(int value)
{
  LeafNode* newLeaf = NULL;
  // 1) check if current leaf node has space
  cout << "count of insert " << count << endl;
  if (leftSibling)
        cout << "left sib count before " << leftSibling->getCount() << endl;
  if (rightSibling)
        cout << "right sib count before " << rightSibling->getCount() << endl;
  if (count < leafSize) {
    addToCurrent(value);
  }
  else if (leftSibling && leftSibling->getCount() < leafSize) { // look left; check to see if a left Sib exists, and if so, if it's full
    int min;
    min = getMinimum();
    if (value < min) {
      addToLeft(value);
    }
    else {
      addToLeft(min);
      remove(min); // once you've added the minimum to left sibling, remove it from current leaf node
      addToCurrent(value);
    }
  }
  else if (rightSibling && rightSibling->getCount() < leafSize) { // look right
    int max;
    max = getMaximum();
    if (value > max) {
      addToRight(value);
    }
    else {
      addToRight(max);
      remove(max); // once you've added the maximum to right sibling, remove it from current leaf node
      addToCurrent(value);
    }
  }
  else {


    if (value > getMaximum()) {
      cout << "split1 is running" << endl;
      newLeaf = split();
      //cout << "we added insertion value to right sibling" << endl;
      //cout << value << endl;
      rightSibling->insert(value);

      if (leftSibling)
        cout << "left sib count after " << leftSibling->getCount() << endl;
    } 
    else if (value < getMinimum()) {
      cout << "split2 is running" << endl;
      newLeaf = split();
      //cout << "we added max of left sibling to the right sibling" << endl;
      addToRight(getMaximum());
      cout << "this is get maximum value " << getMaximum() << endl;
      remove(getMaximum());
      insert(value);
      cout << "current count, should be 1: " << count << endl;
      if (rightSibling)
        cout << "right sib count after split " << rightSibling->getCount() << endl;
      if (leftSibling)
        cout << "left sib count after split " << leftSibling->getCount() << endl;
    }
  }
  return newLeaf; // returns newLeaf, which is NULL if no split
                  // else newLeaf returns the new leafNode created 
}

void LeafNode::remove(int remNum) {
  if (remNum != getMaximum() && getMaximum() != getMinimum()) {
    for (int i = 0; i < count - 1; i++) { // starts at index 0, runs through until max size of values 
        if (remNum <= values[i]) {
          values[i] = values[i+1];
        }
    }
    //values[count-1] = NULL; 
    count--;
  }
  else {
    //values[count - 1] = NULL; // if removing largest value, can just remove it by setting = to NULL
    count--;
  }
}

LeafNode* LeafNode::split() {
  int half;
  if (leafSize % 2 == 0) {
    half = 0.5*leafSize;

  }
  else {
    half = 0.5*leafSize + 1;
  }

  LeafNode *rightNode = new LeafNode(leafSize, parent, this, rightSibling);
  setRightSibling(rightNode);
  if (rightSibling) {
    rightSibling->setLeftSibling(rightNode);
    rightSibling = rightNode;
  }

  for (int i = half; i < leafSize; i++) {
    rightNode->insert(values[i]);
    remove(values[i]);
    cout << "count after split, in for loop " << count << endl;
    //count--;
  }

  return rightNode;
}

// Print BTree
void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  //cout << "this is array " << values[1] << endl;
  cout << endl;
} // LeafNode::print()


