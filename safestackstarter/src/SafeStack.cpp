#include "SafeStack.h"

SafeStack::SafeStack(){
    //TODO
}
SafeStack::~SafeStack(){
    //TODO
}
void SafeStack::push(int data){
    //TODO
}
int SafeStack::pop(){
    int temp;
    // Lock
    pthread_mutex_lock(&stack_lock);
    // Wait for stuff to be in stack
    while((*numbers).size() == 0){
        pthread_cond_wait(&not_empty, &stack_lock);
    }
    // Get top
    temp = (*numbers).top();
    // Get rid of top
    (*numbers).pop();
    // Unlock (because we already have top)
    pthread_mutex_unlock(&stack_lock);
    // Return
    return temp;
}
int SafeStack::size(){
    //TODO
    return 0;
}