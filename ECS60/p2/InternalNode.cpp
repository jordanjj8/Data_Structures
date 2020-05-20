#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
  // used when updating key
  cout << "count of internalnode " << count << endl;
  for (int i = count-1 ; i >= 0; i--) {
    if (value > keys[i]) {
      children[i]->insert(value);
      break;
    }

  }

  return NULL; // to avoid warnings for now.
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  cout << "internal node insert is called" << endl;
  //cout << "insert ran" << endl;
  children[0] = oldRoot; // old root will always be leaf node with smaller values, per sean's rules
  //cout << "This is children[0]: " << children[0] << endl;
  keys[0] = children[0]->getMinimum(); // update key
  //cout << "This is keys[0]: " << keys[0] << endl;
  children[1] = node2;
  keys[1] = children[1]->getMinimum();

  children[0]->setParent(this);
  children[1]->setParent(this);
  count = count + 1;
  count = count + 1;
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  // add child to parent for when >2 leaf nodes total
} // InternalNode::insert()// students may write this

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++) {
    // cout << "This is keys[0]: " << keys[0] << endl;
    // cout << "This is keys[0]: " << keys[0] << endl;
    cout << keys[i] << ' ';
    
  }
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


