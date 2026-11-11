#include <iostream>

struct Node
{
    int patientId = 0;
    Node *next = nullptr;
};

struct Queue
{
    Node *head = nullptr; // Front of the line
    Node *tail = nullptr; // End of the line
};

// Enqueue patient by ID (O(1))
void enqueue(Queue &queue, int patientId)
{
    Node *newNode = new Node{patientId, nullptr};

    if (queue.tail == nullptr)
    {
        queue.head = newNode;
        queue.tail = newNode;
    }
    else
    {
        queue.tail->next = newNode;
        queue.tail = newNode;
    }
}

// Dequeue next patient (O(1))
void dequeue(Queue &queue)
{
    if (queue.head == nullptr)
    {
        std::cout << "Queue is empty. Cannot dequeue.\n";
        return;
    }

    Node *temp = queue.head;
    queue.head = queue.head->next;

    if (queue.head == nullptr)
    {
        queue.tail = nullptr; // Reset tail when last patient is removed
    }

    delete temp;
}

// View next patient ID without removing (O(1))
int peek(const Queue &queue)
{
    if (queue.head == nullptr)
    {
        std::cout << "Queue is empty.\n";
        return -1;
    }
    return queue.head->patientId;
}

// Check if queue has waiting patients (O(1))
bool isEmpty(const Queue &queue)
{
    return queue.head == nullptr;
}

// Clean up all dynamically allocated nodes
void clearQueue(Queue &queue)
{
    while (!isEmpty(queue))
    {
        dequeue(queue);
    }
}
