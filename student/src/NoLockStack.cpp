#include "NoLockStack.h"

NoLockStack::NoLockStack(){
    head.store(nullptr);
    atomic_init(&count, 0);
    //TODO
}

NoLockStack::~NoLockStack(){
    // Iterate through all elements and free them.
    while(head != NULL){
        Node* temp = head;
        head = (*head).next; // std::atomic doesn't like -> opperators
        delete temp;
    }
}

/** Creates a new node from the value passed in and makes that new node the head
 *  of the stack. Also incrememnts count.
 * 
 *  @param data The int to be stored in the stack.
 */
void NoLockStack::push(int data){
    Node * newnode = new Node{data};
    newnode->next = head;
    while(!head.compare_exchange_weak(newnode->next, newnode)){};
    count.exchange(count +1);
    //TODO
}

/** Gets the head node, remove it, and return it's value. Also decrements count.
 * 
 *  @param t An int passed by reference, such that you can set it equal to the value 
 *           removed and it will update the variable passed in when calling.
 *  @returns true if there was an item to pop and it popped, false otherwise.
 */
bool NoLockStack::pop(int& t){
    //TODO
    Node* node = head;
    Node* n = node->next;
    while (!head.compare_exchange_weak(node->next, n)){};
    count.exchange(count - 1);
    return true;
}

/** Gets the number of elements in the stack.
 * 
 *  @returns An int representing the number of elements in the stack.
 */
int NoLockStack::size(){
    //TODO
    return count.load();
}