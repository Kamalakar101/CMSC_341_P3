// CMSC 341 - Fall 2024 - Project 3
#include "mqueue.h"
MQueue::MQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)
{
  
}
MQueue::~MQueue()
{
  
}
void MQueue::clear() {
    
}
MQueue::MQueue(const MQueue& rhs)
{
  
}
MQueue& MQueue::operator=(const MQueue& rhs) {
  
}
void MQueue::mergeWithQueue(MQueue& rhs) {
  
}
bool MQueue::insertOrder(const Order& order) {
    
}
int MQueue::numOrders() const
{
  
}
prifn_t MQueue::getPriorityFn() const {
  
}
Order MQueue::getNextOrder() {
    
}
void MQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
                      
}
void MQueue::setStructure(STRUCTURE structure){
    
}
STRUCTURE MQueue::getStructure() const {
  
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