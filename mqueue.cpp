// CMSC 341 - Fall 2024 - Project 3
#include "mqueue.h"
MQueue::MQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure) {
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;
  m_heap = nullptr;
  m_size = 0;
}
MQueue::~MQueue() {
  clear();
}
void MQueue::clear() {
  deleteHeap(m_heap);
}
MQueue::MQueue(const MQueue& rhs) {
  
}
MQueue& MQueue::operator=(const MQueue& rhs) {
  
}
void MQueue::mergeWithQueue(MQueue& rhs) {
  
}
bool MQueue::insertOrder(const Order& order) {
  if (true) {
    if (m_heap == nullptr) {
      m_heap = new Node(order);
      m_size++;
      return true;
    }
    else {
      merge(m_heap, new Node(order));
      m_size++;
      return true;
    }
    return false;
  }
}
int MQueue::numOrders() const {
  return m_size;
}
prifn_t MQueue::getPriorityFn() const {
  return m_priorFunc;
}
Order MQueue::getNextOrder() {
  if (m_heap != nullptr) {
    //code to separate the heapRoot
    return m_heap->m_order;
  }
  //Throw exception
  return Order();
}
void MQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
                      
}
HEAPTYPE MQueue::getHeapType() const {
  return m_heapType;
}
void MQueue::setStructure(STRUCTURE structure) {
}
STRUCTURE MQueue::getStructure() const {
  return m_structure;
}
void MQueue::printOrderQueue() const {
  
}
void MQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}
void MQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer;
    else
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

void MQueue::deleteHeap(Node *heapRoot) {
  if (heapRoot != nullptr) {
    deleteHeap(heapRoot->m_left);
    deleteHeap(heapRoot->m_right);
    delete heapRoot;
  }
}

Node *MQueue::merge(Node *heapOne, Node *heapTwo) {
  if (heapOne == nullptr) {
    return heapTwo;
  }
  else if (heapTwo == nullptr) {
    return heapOne;
  }

  //If heapTwo has higher priority than heapOne
  if ((m_priorFunc(heapOne->m_order) > m_priorFunc(heapTwo->m_order) && m_heapType == MINHEAP) || (m_priorFunc(heapOne->m_order) < m_priorFunc(heapTwo->m_order) && m_heapType == MAXHEAP)) {
    //Swaps heapOne and heapTwo
    Node * temp = heapOne;
    heapOne = heapTwo;
    heapTwo = temp;
  }
  
  //Swapping left and right subtrees
  Node * temp = heapOne->m_left;
  heapOne->m_left = heapOne->m_right;
  heapOne->m_right = temp;

  //Recursive Call
  heapOne->m_left = merge(heapOne->m_left, heapTwo);
}

ostream& operator<<(ostream& sout, const Order& order) {
  sout  << "Customer: " << order.getCustomer()
        << ", importance: " << order.getImportance() 
        << ", quantity: " << order.getQuantity() 
        << ", order placement time: " << order.getFIFO();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.m_order;
  return sout;
}