// CMSC 341 - Fall 2024 - Project 3
#include "mqueue.h"

//Constructor
//Constructs a heap based on the passed in parameters
MQueue::MQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure) {
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;
  m_heap = nullptr;
  m_size = 0;
}

//Destructor
//Deallocates the heap
MQueue::~MQueue() {
  clear();
}

//Clear
//Clears the heap
void MQueue::clear() {
  deleteHeap(m_heap);
}

//Copy Constructor
//Constructs a heap based on another heap
MQueue::MQueue(const MQueue& rhs) {
  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  m_structure = rhs.m_structure;
  m_heapType = rhs.m_heapType;
  m_heap = duplicateHeap(rhs.m_heap);
}

//Overloaded Assignment
//Assignment Operator for MQueue objects
MQueue& MQueue::operator=(const MQueue& rhs) {
  //Protection against self assignment
  if (this != &rhs) {
    //Clears this heap
    clear();
    m_size = rhs.m_size;
    m_priorFunc = rhs.m_priorFunc;
    m_structure = rhs.m_structure;
    m_heapType = rhs.m_heapType;
    //Duplicates the rhs heap and assigns it to this heap
    m_heap = duplicateHeap(rhs.m_heap);
  }
  return *this;
}

//mergeWithQueue()
//Merges this heap with another MQueue object
void MQueue::mergeWithQueue(MQueue& rhs) {
  //Protection against self merging
  if (this != &rhs) {
    //If the rhs heap is similar to this heap
    if (m_structure == rhs.m_structure && m_heapType == rhs.m_heapType && m_priorFunc == rhs.m_priorFunc) {
      m_heap = merge(m_heap, rhs.m_heap);
      m_size += rhs.m_size;
      rhs.m_heap = nullptr;
      rhs.m_size = 0;
    }
    //If heap structure or type/priority functions don't match, throws exception
    else {
      throw "domain_error"s;
    }
  }
}

//insertOrder()
//Inserts an order into the heap
bool MQueue::insertOrder(const Order& order) {
  //If the order is valid
  if (m_priorFunc(order) != 0) {
    m_heap = merge(m_heap, new Node(order));
    m_size++;
    return true;
  }
  return false;
}

//numOrders
//Returns the size of the heap (amount of orders)
int MQueue::numOrders() const {
  return m_size;
}

//getPriorityFn()
//Returns the heap's priority function
prifn_t MQueue::getPriorityFn() const {
  return m_priorFunc;
}

//getNextOrder()
//Pops the priority node out of the heap
Order MQueue::getNextOrder() {
  //If the heap isn't empty
  if (m_heap != nullptr) {
    //Separates the heapRoot
    Node *leftSubtree = m_heap->m_left;
    Node *rightSubtree = m_heap->m_right;
    Order order(m_heap->m_order);
    delete m_heap;
    //Merges two subtrees
    m_heap = merge(leftSubtree, rightSubtree);
    m_size--;
    return order;
  }
  //Throws exception if the heap is empty
  throw "out_of_range"s;
}

//setPriorityFn()
//Sets the priority function and heapType and rebuilds the heap
void MQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  m_priorFunc = priFn;
  m_heapType = heapType;
  rebuild();
}

//getHeapType()
//Returns heapType
HEAPTYPE MQueue::getHeapType() const {
  return m_heapType;
}

//setStructure()
//Sets the structure and rebuilds the heap
void MQueue::setStructure(STRUCTURE structure) {
  m_structure = structure;
  rebuild();
}

//getStructure()
//Returns the structure type
STRUCTURE MQueue::getStructure() const {
  return m_structure;
}

//printOrderQueue()
//Prints out all the orders in the queue
void MQueue::printOrderQueue() const {
  cout << "Contents of the queue:" << endl;
  printPreOrder(m_heap);
}

//dump()
//Prints out the heap
void MQueue::dump() const {
  //If the heap is empty
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}

//dump()
//Recursively prints out the heap in post-order
void MQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    //Skew output
    if (m_structure == SKEW)
      cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer;
    //Leftist output
    else
      cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

//deleteHeap()
//Recursively deallocates the heap
void MQueue::deleteHeap(Node *heapRoot) {
  //If the heap is not empty
  if (heapRoot != nullptr) {
    deleteHeap(heapRoot->m_left);
    deleteHeap(heapRoot->m_right);
    delete heapRoot;
  }
}

//merge()
//Merges two heaps
Node *MQueue::merge(Node *heapOne, Node *heapTwo) {
  //Base Case
  if (heapOne == nullptr) {
    return heapTwo;
  }
  //Base Case
  else if (heapTwo == nullptr) {
    return heapOne;
  }
  //If heapTwo has higher priority than heapOne
  //Recursive Case: Skew Heap
  if ((m_structure == SKEW) && (((m_heapType == MINHEAP) && (m_priorFunc(heapOne->m_order) > m_priorFunc(heapTwo->m_order))) || ((m_heapType == MAXHEAP) && (m_priorFunc(heapOne->m_order) < m_priorFunc(heapTwo->m_order))))) {
    //Swaps heapOne and heapTwo
    swap(heapOne, heapTwo);
  }
  //Recursive Case: Leftist Heap
  if ((m_structure == LEFTIST) && (((m_heapType == MINHEAP) && (m_priorFunc(heapOne->m_order) < m_priorFunc(heapTwo->m_order))) || ((m_heapType == MAXHEAP) && (m_priorFunc(heapOne->m_order) > m_priorFunc(heapTwo->m_order))))) {
    return leftistMerge(heapOne, heapTwo);
  }
  else if (m_structure == LEFTIST) {
    return leftistMerge(heapTwo, heapOne);
  }
  
  //Rest of Skew Code
  //Swapping left and right subtrees
  swap(heapOne->m_left, heapOne->m_right);

  //Recursive Call
  heapOne->m_left = merge(heapOne->m_left, heapTwo);
  return heapOne;
}

//leftistMerge()
//Used for merging leftist heaps
Node *MQueue::leftistMerge(Node *heapOne, Node *heapTwo) {
  //If the left node is empty
  if (heapOne->m_left == nullptr) {
    heapOne->m_left = heapTwo;
  }
  else {
    heapOne->m_right = merge(heapOne->m_right, heapTwo);
    if (heapOne->m_left->m_npl < heapOne->m_right->m_npl) {
      swap(heapOne->m_left, heapOne->m_right);
    }
    heapOne->m_npl = heapOne->m_right->m_npl + 1;
  }
  return heapOne;
}

//duplicateHeap()
//Duplicates a heap and returns the duplicate's root
Node *MQueue::duplicateHeap(Node *heapRoot) {
  //Base Case
  if (heapRoot == nullptr) {
    return nullptr;
  }
  //Recursive Calls
  Node *node = new Node(heapRoot->m_order);
  node->m_left = duplicateHeap(heapRoot->m_left);
  node->m_right = duplicateHeap(heapRoot->m_right);
  node->m_npl = heapRoot->m_npl;
  return node;
}

//rebuild()
//Rebuilds the heap
void MQueue::rebuild() {
  Node *newHeap = nullptr;
  //Loops until m_heap is empty
  for (int i = m_size; i > 0; i--) {
    Node *priority = m_heap;
    //Pops the root and merges subtrees
    m_heap = merge(m_heap->m_left, m_heap->m_right);
    priority->m_left = nullptr;
    priority->m_right = nullptr;
    priority->m_npl = 0;
    //Adds the node to the newHeap
    newHeap = merge(newHeap, priority);
  }
  m_heap = newHeap;
}

//printPreOrder()
//Recursively prints out the orders in the heap in pre-order
void MQueue::printPreOrder(Node *node) const {
  //Recurses until the node is nullptr
  if (node != nullptr) {
    cout << '[' << m_priorFunc(node->m_order) << ']' << ' ' << node->m_order << endl;
    printPreOrder(node->m_left);
    printPreOrder(node->m_right);
  }
}

//Overloaded Insertion Operator for Order objects
ostream& operator<<(ostream& sout, const Order& order) {
  sout  << "Customer: " << order.getCustomer()
        << ", importance: " << order.getImportance() 
        << ", quantity: " << order.getQuantity() 
        << ", order placement time: " << order.getFIFO();
  return sout;
}

//Overloaded Insertion Operator for Node objects
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.m_order;
  return sout;
}