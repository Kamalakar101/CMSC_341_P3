// CMSC 341 - Fall 2024 - Project 3
#include "mqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = std::mt19937(10);// 10 is the fixed seed value
        m_unidist = std::uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

int priorityFn1(const Order &order);
int priorityFn2(const Order &order);

class Tester{
    public:
        //Helpers
        bool checkHeapProperty(Node *node, prifn_t priorityFunc, HEAPTYPE heapType);
        int getNPL(Node *node);
        bool checkNPL(Node *node);
        bool compareNPL(Node *node);
        void loadHeaptIntoVector(vector<Node *> &heapVec, Node *node);
        bool sameNodes(vector<Node *> &heapVec, Node *node);
        bool compareHeaps(Node *heapOne, Node *heapTwo, prifn_t pFunc);

        //Test Cases

        //Normal
        bool testMinHeapPostInsertion(MQueue &heap);
        bool testMaxHeapPostInsertion(MQueue &heap);
        bool testMinHeapRemoval(MQueue &heap);
        bool testMaxHeapRemoval(MQueue &heap);
        bool testNPLValues(MQueue &leftistHeap);
        bool testLeftistProperty(MQueue &leftistHeap);
        bool testChangedPriorityFunc(MQueue &heap, prifn_t pFunc, HEAPTYPE HEAPTYPE);
        bool testCopyConstructorNormal(MQueue &heap);
        bool testAssignmentOperatorNormal(MQueue &heapOne, MQueue &heapTwo);

        //Edge
        bool testMergeEmptyWithNormal(MQueue &nonEmptyHeap);
        bool testCopyConstructorEdge();
        bool testAssignmentOperatorEdge(MQueue &copy);

        //Error
        bool testDequeueEmptyQueue(MQueue &heap);
        bool testMergeQueuesDiffPriorityFuncs(MQueue &heapOne, MQueue &heapTwo);
};

int main() {
    Tester test;
    Random nameGen(97,122);
    Random FIFO(MINONE,MAX50); Random processTime(MINONE,MAX12);
    Random dueTime(MINONE,MAX12); Random slackTime(MINONE,MAX12);
    Random material(MINZERO,MAX100); Random importance(MINONE,MAX100);
    Random workers(MAX100,MAX200); Random quantity(MIN1000,MAX10000);
    //Test Heaps
    MQueue minLeftist(priorityFn2, MINHEAP, LEFTIST);
    MQueue minSkew(priorityFn2, MINHEAP, SKEW);
    MQueue maxLeftist(priorityFn2, MAXHEAP, LEFTIST);
    MQueue maxSkew(priorityFn2, MAXHEAP, SKEW);

    for (int i=0;i<300;i++){
        Order order(nameGen.getRandString(5),
                FIFO.getRandNum(), processTime.getRandNum(),
                dueTime.getRandNum(), slackTime.getRandNum(),
                material.getRandNum(), importance.getRandNum(),
                workers.getRandNum(), quantity.getRandNum());
        minLeftist.insertOrder(order);
        minSkew.insertOrder(order);
        maxLeftist.insertOrder(order);
        maxSkew.insertOrder(order);
    }

    //*********************** MIN HEAP POST-INSERTION ***********************
    cout << "*********************** MIN HEAP POST-INSERTION ***********************" << endl;
    cout << "Min-Heap Post-Insertion: ";
    if(test.testMinHeapPostInsertion(minLeftist) && test.testMinHeapPostInsertion(minSkew)) {
        cout << "Follows Heap Property! (PASSED)" << endl;
    }
    else {
        cout << "Does Not Follow Heap Property! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** MAX HEAP POST-INSERTION ***********************
    cout << "*********************** MAX HEAP POST-INSERTION ***********************" << endl;
    cout << "Max-Heap Post-Insertion: ";
    if(test.testMaxHeapPostInsertion(maxLeftist) && test.testMaxHeapPostInsertion(maxSkew)) {
        cout << "Follows Heap Property! (PASSED)" << endl;
    }
    else {
        cout << "Does Not Follow Heap Property! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** MIN HEAP REMOVALS ***********************
    cout << "*********************** MIN HEAP REMOVALS ***********************" << endl;
    cout << "Min-Heap Removal: ";
    if(test.testMinHeapRemoval(minLeftist) && test.testMinHeapRemoval(minSkew)) {
        cout << "Removes in Order! (PASSED)" << endl;
    }
    else {
        cout << "Does Not Remove in Order! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** MAX HEAP POST-INSERTION ***********************
    cout << "*********************** MAX HEAP POST-INSERTION ***********************" << endl;
    cout << "Max-Heap Removal: ";
    if(test.testMaxHeapRemoval(maxLeftist) && test.testMaxHeapRemoval(maxSkew)) {
        cout << "Removes in Order! (PASSED)" << endl;
    }
    else {
        cout << "Does Not Remove in Order! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** NPL ***********************
    cout << "*********************** NPL ***********************" << endl;
    cout << "Correct NPL: ";
    if(test.testNPLValues(maxLeftist) && test.testNPLValues(minLeftist)) {
        cout << "Correct NPL Values! (PASSED)" << endl;
    }
    else {
        cout << "Incorrect NPL Values! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** LEFTIST PROPERTY ***********************
    cout << "*********************** LEFTIST PROPERTY ***********************" << endl;
    cout << "Leftist Property: ";
    if(test.testLeftistProperty(maxLeftist) && test.testLeftistProperty(minLeftist)) {
        cout << "Follows Leftist Property! (PASSED)" << endl;
    }
    else {
        cout << "Does Not Follow Leftist Property! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** CHANGED PRIORITY FUNCTION ***********************
    cout << "*********************** CHANGED PRIORITY FUNCTION ***********************" << endl;
    cout << "Changed Priority Function: ";
    if(test.testChangedPriorityFunc(maxLeftist, priorityFn2, MINHEAP)) {
        cout << "Same Nodes! (PASSED)" << endl;
    }
    else {
        cout << "Not the Same Nodes! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** MERGE EMPTY WITH NORMAL ***********************
    cout << "*********************** MERGE EMPTY WITH NORMAL ***********************" << endl;
    cout << "Merge Empty Heap with Non-empty Heap: ";
    if(test.testChangedPriorityFunc(maxLeftist, priorityFn2, MINHEAP)) {
        cout << "Successful Merge! (PASSED)" << endl;
    }
    else {
        cout << "Unsuccessful Merge! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** COPY CONSTURCTOR NORMAL ***********************
    cout << "*********************** COPY CONSTURCTOR NORMAL ***********************" << endl;
    cout << "Copy Constructor Normal: ";
    if(test.testCopyConstructorNormal(maxLeftist)) {
        cout << "Successful Copy! (PASSED)" << endl;
    }
    else {
        cout << "Unsuccessful Copy! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** COPY CONSTURCTOR EDGE ***********************
    cout << "*********************** COPY CONSTURCTOR EDGE ***********************" << endl;
    cout << "Copy Constructor Edge: ";
    if(test.testCopyConstructorEdge()) {
        cout << "Successful Copy! (PASSED)" << endl;
    }
    else {
        cout << "Unsuccessful Copy! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** ASSIGNMENT OPERATOR NORMAL ***********************
    cout << "*********************** ASSIGNMENT OPERATOR NORMAL ***********************" << endl;
    cout << "Assignment Operator Normal: ";
    if(test.testAssignmentOperatorNormal(minSkew, maxSkew)) {
        cout << "Successful Assignment! (PASSED)" << endl;
    }
    else {
        cout << "Unsuccessful Assignment! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** ASSIGNMENT OPERATOR EDGE ***********************
    cout << "*********************** ASSIGNMENT OPERATOR EDGE ***********************" << endl;
    cout << "Assignment Operator Edge: ";
    if(test.testAssignmentOperatorEdge(minSkew)) {
        cout << "Successful Assignment! (PASSED)" << endl;
    }
    else {
        cout << "Unsuccessful Assignment! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** DEQUEUE EMPTY ***********************
    cout << "*********************** DEQUEUE EMPTY ***********************" << endl;
    cout << "Dequeue Empty: ";
    if(test.testDequeueEmptyQueue(minLeftist)) {
        cout << "Exception Thrown! (PASSED)" << endl;
    }
    else {
        cout << "Exception Not Thrown! (FAILED)" << endl;
    }
    cout << endl;

    //*********************** MERGE QUEUE WITH DIFFERENT PRIORITY FUNCTIONS ***********************
    cout << "*********************** MERGE QUEUE WITH DIFFERENT PRIORITY FUNCTIONS ***********************" << endl;
    cout << "Merge Queue with Different Priority Functions: ";
    if(test.testMergeQueuesDiffPriorityFuncs(maxLeftist, minSkew)) {
        cout << "Exception Thrown! (PASSED)" << endl;
    }
    else {
        cout << "Exception Not Thrown! (FAILED)" << endl;
    }
    cout << endl;

}

/* Priority functions */
int priorityFn1(const Order & order) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1101-10400]
    int minValue = MIN1000 + MAX100 + MINONE + MINZERO;
    int maxValue = MAX100 + MAX100 + MAX200 + MAX10000;
    //the larger value means the higher priority
    int priority = order.getMaterial() + order.getImportance() + 
                    order.getWorkForce() + order.getQuantity();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}

int priorityFn2(const Order & order) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [4-86]
    int minValue = MINONE + MINONE + MINONE + MINONE;
    int maxValue = MAX12 + MAX12 + MAX12 + MAX50;
    //the smaller value means the higher priority
    int priority = order.getFIFO() + order.getProcessTime() + 
                order.getDueTime() + order.getSlackTime();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}

bool Tester::checkHeapProperty(Node *node, prifn_t priorityFunc, HEAPTYPE heapType) {
    if (node == nullptr) {
        return true;
    }
    else if ((heapType == MINHEAP) && (((node->m_left != nullptr) && (priorityFunc(node->m_left->m_order) < priorityFunc(node->m_order))) || ((node->m_right != nullptr) && priorityFunc(node->m_right->m_order) < priorityFunc(node->m_order)))) {
        return false;
    }
    else if ((heapType == MAXHEAP) && (((node->m_left != nullptr) && (priorityFunc(node->m_left->m_order) > priorityFunc(node->m_order))) || ((node->m_right != nullptr) && priorityFunc(node->m_right->m_order) > priorityFunc(node->m_order)))) {
        return false;
    }
    return checkHeapProperty(node->m_left, priorityFunc, heapType) && checkHeapProperty(node->m_right, priorityFunc, heapType);
}

int Tester::getNPL(Node *node) {
    if (node == nullptr) {
        return -1;
    }
    return 1 + getNPL(node->m_right);
}

bool Tester::checkNPL(Node *node) {
    if (node == nullptr) {
        return true;
    }
    else if (node->m_npl != getNPL(node)) {
        return false;
    }
    return checkNPL(node->m_left) && checkNPL(node->m_right);
}

bool Tester::compareNPL(Node *node) {
    if (node == nullptr) {
        return true;
    }
    else if (1 + getNPL(node->m_left) < getNPL(node)) {
        return false;
    }
    return compareNPL(node->m_left) && compareNPL(node->m_right);
}

void Tester::loadHeaptIntoVector(vector<Node *> &heapVec, Node *node) {
    if (node != nullptr) {
        heapVec.push_back(node);
        loadHeaptIntoVector(heapVec, node->m_left);
        loadHeaptIntoVector(heapVec, node->m_right);
    }
}

bool Tester::sameNodes(vector<Node *> &heapVec, Node *node) {
    if (node == nullptr) {
        return true;
    }
    bool found = false;
    for (int i = 0; i < int(heapVec.size()); i++) {
        if (heapVec.at(i) == node) {
            found = true;
        }
    }
    if (!found) {
        return false;
    }
    return sameNodes(heapVec, node->m_left) && sameNodes(heapVec, node->m_right);
}

bool Tester::compareHeaps(Node *heapOne, Node *heapTwo, prifn_t pFunc) {
    if (heapOne == nullptr && heapTwo == nullptr) {
        return true;
    }
    else if (pFunc(heapOne->m_order) != pFunc(heapTwo->m_order)) {
        return false;
    }
    return compareHeaps(heapOne->m_left, heapTwo->m_left, pFunc) && compareHeaps(heapOne->m_right, heapTwo->m_right, pFunc);
}

bool Tester::testMinHeapPostInsertion(MQueue &heap) {
    if (checkHeapProperty(heap.m_heap, heap.m_priorFunc, heap.m_heapType)) {
        return true;
    }
    return false;
}

bool Tester::testMaxHeapPostInsertion(MQueue &heap) {
    if (checkHeapProperty(heap.m_heap, heap.m_priorFunc, heap.m_heapType)) {
        return true;
    }
    return false;
}

bool Tester::testMinHeapRemoval(MQueue &heap) {
    for (int i = heap.m_size / 2; i > 0; i--) {
        if (heap.m_size != 1 && (heap.m_priorFunc(heap.getNextOrder()) > heap.m_priorFunc(heap.m_heap->m_order))) {
            return false;
        }
    }
    return true;
}

bool Tester::testMaxHeapRemoval(MQueue &heap) {
    for (int i = heap.m_size / 2; i > 0; i--) {
        if (heap.m_size != 1 && (heap.m_priorFunc(heap.getNextOrder()) < heap.m_priorFunc(heap.m_heap->m_order))) {
            return false;
        }
    }
    return true;
}

bool Tester::testNPLValues(MQueue &leftistHeap) {
    if (checkNPL(leftistHeap.m_heap)) {
        return true;
    }
    return false;
}

bool Tester::testLeftistProperty(MQueue &leftistHeap) {
    if (compareNPL(leftistHeap.m_heap)) {
        return true;
    }
    return false;
}

bool Tester::testChangedPriorityFunc(MQueue &heap, prifn_t pFunc, HEAPTYPE heapType) {
    vector<Node *> heapVec;
    loadHeaptIntoVector(heapVec, heap.m_heap);
    heap.setPriorityFn(pFunc, heapType);
    if (pFunc == heap.m_priorFunc && heapType == heap.m_heapType && sameNodes(heapVec, heap.m_heap)) {
        return true;
    }
    return false;
}

bool Tester::testCopyConstructorNormal(MQueue &heap) {
    MQueue copy(heap);
    if (copy.m_heapType == heap.m_heapType && copy.m_size == heap.m_size 
    && copy.m_structure == heap.m_structure && copy.m_priorFunc == heap.m_priorFunc
    && compareHeaps(copy.m_heap, heap.m_heap, heap.m_priorFunc) && &copy != &heap) {
        return true;
    }
    return false;
}

bool Tester::testAssignmentOperatorNormal(MQueue &heapOne, MQueue &heapTwo) {
    heapOne = heapTwo;
    if (heapOne.m_heapType == heapTwo.m_heapType && heapOne.m_size == heapTwo.m_size 
    && heapOne.m_structure == heapTwo.m_structure && heapOne.m_priorFunc == heapTwo.m_priorFunc
    && compareHeaps(heapOne.m_heap, heapTwo.m_heap, heapTwo.m_priorFunc) && &heapOne != &heapTwo) {
        return true;
    }
    return false;
}

bool Tester::testMergeEmptyWithNormal(MQueue &nonEmptyHeap) {
    MQueue empty(priorityFn1, MINHEAP, LEFTIST);
    empty.mergeWithQueue(nonEmptyHeap);
    if (empty.m_heapType == nonEmptyHeap.m_heapType && empty.m_size == nonEmptyHeap.m_size 
    && empty.m_structure == nonEmptyHeap.m_structure && empty.m_priorFunc == nonEmptyHeap.m_priorFunc
    && nonEmptyHeap.m_heap == nullptr && empty.m_heap != nullptr) {
        return true;
    }
    return false;
}

bool Tester::testCopyConstructorEdge() {
    MQueue empty(priorityFn1, MINHEAP, LEFTIST);
    MQueue copy(empty);
    if (empty.m_heapType == copy.m_heapType && empty.m_size == copy.m_size 
    && empty.m_structure == copy.m_structure && empty.m_priorFunc == copy.m_priorFunc
    && copy.m_heap == nullptr && empty.m_heap == nullptr) {
        return true;
    }
    return false;
}

bool Tester::testAssignmentOperatorEdge(MQueue &heap) {
    MQueue empty(priorityFn1, MINHEAP, LEFTIST);
    empty = heap;
    if (empty.m_heapType == heap.m_heapType && empty.m_size == heap.m_size 
    && empty.m_structure == heap.m_structure && empty.m_priorFunc == heap.m_priorFunc
    && compareHeaps(empty.m_heap, heap.m_heap, heap.m_priorFunc)) {
        return true;
    }
    return false;
}

bool Tester::testDequeueEmptyQueue(MQueue &heap) {
    try {
        for (int i = heap.m_size + 1; i > 0; i--) {
            heap.getNextOrder();
        }
    }
    catch (string exception) {
        if (exception == "out_of_range") {
            return true;
        }
    }
    return false;
}

bool Tester::testMergeQueuesDiffPriorityFuncs(MQueue &heapOne, MQueue &heapTwo) {
    try {
        heapOne.mergeWithQueue(heapTwo);
    }
    catch (string exception) {
        if (exception == "domain_error") {
            return true;
        }
    }
    return false;
}
