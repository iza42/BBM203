#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    front=rear=-1;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    if(isFull()) {
        std::cout<<"the queue is full , cannot enqueue any further"<<std::endl;
    }
    else {
        if(isEmpty()) {
            front=rear=0; // if the queue is empty at first, both the front and the rear must be moved, which is something unique to a circular queue, in a linear queue, only the rear is moved at first
            data[rear]=province;
        }else {
            //increase the rear by 1, then put the element there ,where the rear indicates
            rear=(rear+1)%MAX_QUEUE_SIZE;
            data[rear]=province;
        }
    }
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    int x =-1; //if the queue is empty, it will return a value of -1
    if(isEmpty()) {
        std::cout<<"the queue is empty cannot dequeue any further"<<std::endl;
    }else if(front==rear) { //  display function shows the old values because you did the soft delete  if you do not set this condition
        front=rear=-1;
    }
    else {
        //take what the current front shows and move the front u forward by 1, which is already something like soft delete
        x = data[front];
        front= (front+1) % MAX_QUEUE_SIZE;
    }
    return x;
}

// Returns the front province without removing it
int Queue::peek() const {
    if(!isEmpty())
        return data[front];
    std::cout<<"the queue is empty thus cannot peek"<<std::endl;
    return -1;
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    return front==-1;
}
bool Queue::isFull() const {
    return  (rear+1)%MAX_QUEUE_SIZE==front;
}
// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    if (isFull()) {
        std::cout << "The circular queue is full, cannot enqueue the priority element." << std::endl;
        return;
    }
    //Take the front one step back (in a circular way)
    front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    //Place Province in the new front position
    data[front] = province;

    }



