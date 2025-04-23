#include "Stack.h"
#include <iostream>

// Constructor to initialize an empty stack
Stack::Stack() {
    this->top=-1; // indicates an empty stack
}

// Adds a province to the top of the stack
void Stack::push(int province) {
   if(top ==MAX_SIZE-1) {
        std::cerr<<"The Stack is full cannot push any further!"<<std::endl;
   }else {
        data[++top]=province; // increase top value by 1 and then insert the given element
    }
}

// Removes and returns the top province from the stack
int Stack::pop() {
    if(isEmpty()) {
        std::cerr<<"The stack is empty cannot pop any further!"<<std::endl;
        return -1024; // change this later
    }else {
        return data[top--]; // take the value there then decrement by 1
    }
}

// Returns the top province without removing it
int Stack::peek() const {
    return data[top];
}

// Checks if the stack is empty
bool Stack::isEmpty() const {
    return top==-1;
}

// Function to get the current size of the stack
int Stack::getSize() const {
    return top+1;
}
