#include "BSTree.h"

#include <iostream>
using namespace std;

BSTree::BSTree() {
    root = nullptr;
}

void BSTree::insert(const string& newdata) {
    Node* newnode = new Node(newdata);
    if (root == nullptr) {  // case for empty Tree
        root = newnode;
        return;
    }
    Node* curr = root;
start:
    if (newdata == curr->getData()) {  // if data is identical
        curr->increaseCount();
        delete newnode;
        return;
    }
    // case for if newdata is larger than curr
    if (newdata > curr->getData()) {
        if (curr->getRightChild() != nullptr) {  //if its not the bottom of the tree yet
            curr = curr->getRightChild();
            goto start;
        } else {  // if it reached the bottom of the tree
            curr->setRightChild(newnode);
            newnode->setParent(curr);
            return;
        }
    }
    // case for if newdata is smaller than curr
    if (newdata < curr->getData()) {
        if (curr->getLeftChild() != nullptr) {
            curr = curr->getLeftChild();
            goto start;
        } else {
            curr->setLeftChild(newnode);
            newnode->setParent(curr);
            return;
        }
    }
}
/*
  Five possible situations
  Item not found (Do nothing or throw exception)
  Removing a leaf (easy)
  Removing a node with only one child - right only
  Removing a node with only one child - left only
  Removing a node with two children

*/

/*
Reworked remove and removeHelper
Assistance from Tanessha Sharma and Geeks for Geeks algorithm
*/
void BSTree::remove(const string &key) {
	Node* nodeToDelete = findNode(key, root);
	
  //if nodeToDelete doesn't exist
	if (search(key) == false) {
    return;
  }

  //empty tree
	if (root == nullptr) {
		return;
	}
	
  //if nodeToDelete is a duplicate
	if (nodeToDelete->getCount() > 1) {
		nodeToDelete->decreaseCount();
		return;
	}
	
  //Recusive call
	removeHelper(nodeToDelete, key);
}

void BSTree::removeHelper(Node* removeNode, string nodeData)
{
  Node* keyParent = removeNode->getParent();

  //For leaf node case
  if (removeNode->hasChild() == false)
  {
    //if removeNode is a root
    if(keyParent == nullptr) 
    {
      root = nullptr;
       
    }
    //removeNode is left leaf
    else if(keyParent->getLeftChild() == removeNode)
    {
      keyParent->setLeftChild(nullptr);
      delete removeNode;
    }
    //removeNode is right leaf
    else if(keyParent->getRightChild() == removeNode)
    {
      keyParent->setRightChild(nullptr);
      delete removeNode;
    }
    return;
  }
  //If successor to removeNode exists
  else if (removeNode->leftChildExists() == true) 
  {
    Node* replacementNode = findPredecessor(removeNode);
    removeNode->setData(replacementNode->getData());
    removeNode->setCount(replacementNode->getCount());
    if(replacementNode->getCount() > 1)
    {
      replacementNode->decreaseCount();
    }
    removeHelper(replacementNode, replacementNode->getData());
    
  }
  //If predecessor to removeNode exists
  else
  {
    Node* replacementNode = findSuccessor(removeNode);
    removeNode->setData(replacementNode->getData());
    removeNode->setCount(replacementNode->getCount());

    if(replacementNode->getCount() > 1)
    {
      replacementNode->decreaseCount();
    }
    removeHelper(replacementNode, replacementNode->getData());
    return;
    
  }
}

//dont question this one. its the worst function ive ever written i think

bool BSTree::search(const string& lookforthis) const {
    Node* searchNode = findNode(lookforthis, root);
    if (searchNode == nullptr) {
        return false;
    }
    return true;
}


string BSTree::largest() const {
    Node* curr = root;
    string largestString = "";
    if (root == nullptr) {
        return "";
    }
    while (curr->rightChildExists() == true) {
        curr = curr->getRightChild();
    }
    largestString = curr->getData();
    if (root == nullptr) {
        return "";
    }

    return largestString;
}

string BSTree::smallest() const {
    Node* curr = root;
    string smallestString = "";

    if (root == nullptr) {
        return "";
    }
    while (curr->leftChildExists() == true) {
        curr = curr->getLeftChild();
    }
    smallestString = curr->getData();
    if (root == nullptr) {
        return "";
    }
    return smallestString;
}

int BSTree::height(const string& startString) const {
    if (!search(startString)) {  // case for string doesnt exist
        return -1;
    }
    Node* curr = findNode(startString, root);
    int ans = findHeight(curr);  //calls recursive function
    return ans;
}

//random helper function that I just didnt want to code into the actual parent function
//the only time it is used is right below this function
int larger(int one, int two) {
    if (one < two) {
        return two;
    }
    return one;
}

int BSTree::findHeight(Node* start) const {
    if (start == nullptr) {
        return -1;
    }
    if (start->hasChild() == false) {
        return 0;
    } else {
        return 1 + larger(findHeight(start->getLeftChild()), findHeight(start->getRightChild()));
    }
}

void BSTree::inOrder() const {
    inOrder(root);
    cout << endl;
}

void BSTree::inOrder(Node* start) const {
    if (start == nullptr) {
        return;
    }
    inOrder(start->getLeftChild());
    cout << start->getData() << '(' << start->getCount() << ')' << ", ";
    inOrder(start->getRightChild());
    //cout << endl;
}

void BSTree::postOrder() const {
    postOrder(root);
    cout << endl;
}

void BSTree::postOrder(Node* start) const {
    if (start == nullptr) {
        return;
    }
    postOrder(start->getLeftChild());
    postOrder(start->getRightChild());
    cout << start->getData() << '(' << start->getCount() << ')' << ", ";
}

void BSTree::preOrder() const {
    preOrder(root);
    cout << endl;
}

void BSTree::preOrder(Node* start) const {
    if (start == nullptr) {
        return;
    }
    cout << start->getData() << '(' << start->getCount() << ')' << ", ";
    preOrder(start->getLeftChild());
    preOrder(start->getRightChild());
    //cout << endl;
}

BSTree::~BSTree() {
    deletetraverse(root);
}

void BSTree::deletetraverse(Node* start) {
    if (start == nullptr) {
        return;
    }
    deletetraverse(start->getLeftChild());
    deletetraverse(start->getRightChild());
    delete start;
}

Node* BSTree::findNode(string nodeData, Node* startNode) const {
    Node* currNode = startNode;

    while (currNode != nullptr) {
        if (currNode->getData() == nodeData) {
            return currNode;
        } else if (nodeData < currNode->getData()) {
            currNode = currNode->getLeftChild();
        } else {
            currNode = currNode->getRightChild();
        }
    }
    if (currNode == nullptr) {
        return nullptr;
    }

    return currNode;
}

Node* BSTree::findSuccessor(Node* startNode) {
    Node* currNode = startNode->getRightChild();
    while (currNode->getLeftChild() != nullptr) {
        currNode = currNode->getLeftChild();
    }

    return currNode;
}

Node* BSTree::findPredecessor(Node* startNode) {
    Node* currNode = startNode->getLeftChild();
    while (currNode->getRightChild() != nullptr) {
        currNode = currNode->getRightChild();
    }

    return currNode;
}

