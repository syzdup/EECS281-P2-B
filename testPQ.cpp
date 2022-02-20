// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

/*
 * Compile this test against your .h files to make sure they compile. Note how
 * the eecs281 priority queues can be constructed with the different types. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations more thoroughly.  If you do not call a
 * function from here, it is NOT compiled due to templates!  So for instance,
 * if you don't add code to call updatePriorities(), you could have compiler
 * errors that you don't even know about.
 *
 * Our makefile will build an executable named testPQ if you type 'make testPQ'
 * or 'make alltests' (without the quotes).
 *
 * Notice that testPairing() tests the range-based constructor but main() and
 * testPriorityQueue() do not.  Make sure to test the range-based constructor
 * for other PQ types, and also test the PairingPQ-specific member functions.
 *
 * This is NOT a complete test of your priority queues, you have to add code
 * to do more testing!
 *
 * You do not have to submit this file, but it won't cause problems if you do.
 */

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "Eecs281PQ.h"
#include "BinaryPQ.h"
#include "UnorderedPQ.h"
#include "PairingPQ.h"
#include "SortedPQ.h"

using namespace std;


// Used to test a priority queue containing pointers to integers.
struct IntPtrComp {
    bool operator() (const int *a, const int *b) const {
        return *a < *b;
    }
};

void testHiddenData(const string &pqType) {
    struct HiddenData {
        int data;
    };
    struct HiddenDataComp {
        bool operator()(const HiddenData &a, const HiddenData &b) const {
            if (a.data == b.data) return true;
            return false;
        }
    };

    cout << "Testing " << pqType << " with hidden data" << endl;

    HiddenData hidden1{2};
    HiddenData hidden2{1};
    Eecs281PQ<HiddenData *, HiddenDataComp> *pq = nullptr;
    pq->push(&hidden1);
    pq->push(&hidden2);

    assert(pq->top() == &hidden1);
    
} // testHiddenData()

void testUpdatePrioritiesHelper(Eecs281PQ<int *, IntPtrComp> *pq) {
    vector<int> data;
    data.reserve(100);
    data.push_back(1);
    data.push_back(5);
    data.push_back(10);
    data.push_back(7);

    // NOTE: If you add more data to the vector, don't push the pointers
    // until AFTER the vector stops changing size!  Think about why.
    // You can add up to 100 values, or change the reserve if you want more.

    for (size_t i = 0; i < data.size(); ++i) {
        pq->push(&data[i]);
    } // for

    // Change the first value (which is pointed to by the pq), and check it.
    data[0] = 3;
    pq->updatePriorities();
    assert(*pq->top() == 10);
    pq->pop();
    assert(*pq->top() == 7);
    pq->pop();
    assert(*pq->top() == 5);
    pq->pop();
    assert(*pq->top() == 3);
    pq->pop();
    assert(pq->empty());
    std::cout << "Update priorities test complete.\n";
} // testUpdatePrioritiesHelper()

void testUpdatePriorities(const string &pqType) {
    Eecs281PQ<int *, IntPtrComp> *pq = nullptr;
    cout << "Testing updatePriorities() on " << pqType << endl;

    if (pqType == "Unordered") {
        pq = new UnorderedPQ<int *, IntPtrComp>;
    } else if(pqType == "Sorted") {
        pq = new SortedPQ<int *, IntPtrComp>;
    } else if(pqType == "Binary") {
        pq = new BinaryPQ<int *, IntPtrComp>;
    } else {
        pq = new PairingPQ<int *, IntPtrComp>;
    }
    if (!pq) {
        cout << "Invalid pq pointer; did you forget to create it?" << endl;
        return;
    } // if

    testUpdatePrioritiesHelper(pq);
    delete pq;
} // testUpdatePriorities()


// Very basic testing.
void testPriorityQueue(Eecs281PQ<int> *pq, const string &pqType) {
    cout << "Testing priority queue: " << pqType << endl;

    pq->push(3);
    pq->push(4);
    assert(pq->size() == 2);
    assert(pq->top() == 4);

    pq->pop();
    assert(pq->size() == 1);
    assert(pq->top() == 3);
    assert(!pq->empty());

    pq->pop();
    assert(pq->size() == 0);
    assert(pq->empty());

    // TODO: Add more testing here!
    pq->push(6);
    pq->push(5);
    pq->push(4);
    pq->push(3);
    pq->pop();
    assert(pq->top() == 5);

    cout << "testPriorityQueue() succeeded!" << endl;
} // testPriorityQueue()

// Test updating elements and updating priorities
void test_update_pairing() {
    cout << "Testing update pairing" << endl;
    PairingPQ<int*, IntPtrComp> * pq1 = new PairingPQ<int*, IntPtrComp>;

    vector<int> vec;
    vec.push_back(56);
    vec.push_back(33);
    vec.push_back(97);
    vec.push_back(2);
    vec.push_back(100);

    for(size_t i = 0; i < vec.size(); ++i) {
        pq1->push(&vec[i]);
    }

    assert(*(pq1->top()) == 100);

    vec[4] = 96;
    pq1->updatePriorities();

    assert(*(pq1->top()) == 97);
    pq1->pop();
    assert(*(pq1->top()) == 96);

    vec.push_back(1);
    auto * new_node = pq1->addNode(&vec[5]);
    vec.push_back(100);
    pq1->updateElt(new_node, &vec[6]);
    assert(*(pq1->top()) == 100);

    // make a big update_pri test here 
    PairingPQ<int*, IntPtrComp> * pq2 = new PairingPQ<int*, IntPtrComp>; 
    vector<int> vec2;
    vec2.push_back(7);
    vec2.push_back(3);
    vec2.push_back(8);
    vec2.push_back(55);
    vec2.push_back(2);

    for(size_t i = 0; i < vec2.size(); ++i) {
        pq2->push(&vec2[i]);
    }
    assert(*(pq2->top()) == 55);

    // change values and update
    vec2[0] = 56;
    vec2[1] = 33;
    vec2[2] = 2;
    vec2[3] = 1;

    pq2->updatePriorities();

    assert(*(pq2->top()) == 56);
    pq2->pop();
    assert(*(pq2->top()) == 33);
    pq2->pop();
    assert(*(pq2->top()) == 2);
    pq2->pop();
    assert(*(pq2->top()) == 2);
    pq2->pop();
    assert(*(pq2->top()) == 1);
    pq2->pop();
    assert(pq2->empty() == true);

    cout << "Testing update pairing succeeded." << endl;
}

// Test the pairing heap's range-based constructor, copy constructor,
// and operator=().
//
void testPairing(vector<int> & vec) {
    cout << "Testing Pairing Heap separately" << endl;
    Eecs281PQ<int> * pq1 = new PairingPQ<int>(vec.begin(), vec.end());
    Eecs281PQ<int> * pq2 = new PairingPQ<int>(*((PairingPQ<int> *)pq1));
    // This line is different just to show two different ways to declare a
    // pairing heap: as an Eecs281PQ and as a PairingPQ. Yay for inheritance!
    PairingPQ<int> * pq3 = new PairingPQ<int>();
    *pq3 = *((PairingPQ<int> *)pq2);

    pq1->push(3);
    pq2->pop();
    assert(pq1->size() == 3);
    assert(!pq1->empty());
    assert(pq1->top() == 3);
    pq2->push(pq3->top());
    assert(pq2->top() == pq3->top());

    cout << "Basic tests done." << endl;

    pq1->push(0);
    pq1->push(8);
    pq1->push(7);
    pq1->push(4);
    assert(pq1->top() == 8);
    pq1->pop();
    assert(pq1->top() == 7);
    pq1->updatePriorities();
    pq1->pop();
    assert(pq1->top() == 4);

    cout << "Calling destructors" << endl;
    delete pq1;
    delete pq2;
    delete pq3;

    cout << "testPairing() succeeded" << endl;
    
    test_update_pairing();
} // testPairing()


int main() {
    // Basic pointer, allocate a new PQ later based on user choice.
    Eecs281PQ<int> *pq;
    vector<string> types{ "Unordered", "Sorted", "Binary", "Pairing" };
    unsigned int choice;

    cout << "PQ tester" << endl << endl;
    for (size_t i = 0; i < types.size(); ++i)
        cout << "  " << i << ") " << types[i] << endl;
    cout << endl;
    cout << "Select one: ";
    cin >> choice;

    if (choice == 0) {
        pq = new UnorderedPQ<int>;
    } // if
    else if (choice == 1) {
        pq = new SortedPQ<int>;
    } // else if
    else if (choice == 2) {
        pq = new BinaryPQ<int>;
    } // else if
    else if (choice == 3) {
        pq = new PairingPQ<int>;
    } // else if
    else {
        cout << "Unknown container!" << endl << endl;
        exit(1);
    } // else
   
    testPriorityQueue(pq, types[choice]);
    testUpdatePriorities(types[choice]);

    if (choice == 3) {
        vector<int> vec;
        vec.push_back(0);
        vec.push_back(1);
        testPairing(vec);
    } // if

    // Clean up!
    delete pq;

    return 0;
} // main()
