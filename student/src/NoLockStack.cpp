#include "NoLockStack.h"

NoLockStack::NoLockStack(){
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
    return true;
}

/** Gets the number of elements in the stack.
 * 
 *  @returns An int representing the number of elements in the stack.
 */
int NoLockStack::size(){
    //TODO
    return 0;
}