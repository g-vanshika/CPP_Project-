#include <iostream>
using namespace std;

struct node {
    int data;
    node* link;
};

void insertAtend(node*& head, int val) {
    node* temp = new node;
    temp->data = val;
    temp->link = NULL;

    if (head == NULL) {
        head = temp;
        return;
    }

    node* tptr = head;
    while (tptr->link != NULL) {
        tptr = tptr->link;
    }
    tptr->link = temp;
}

void createList(node*& head, int nodes) {
    int val;
    cout << "Enter " << nodes << " values:\n";
    for (int i = 0; i < nodes; i++) {
        cin >> val;
        insertAtend(head, val);
    }
}

void printList(node*& head) {
    if (head == NULL) {
        cout << "List is empty"<<endl;
        return;
    }
    node* pptr = head;
    while (pptr != NULL) {
        cout << pptr->data << endl;
        pptr = pptr->link;
    }
    cout << endl;
}

void countNodes(node*& head) {
    int cnt = 0;
    node* temp = head;
    while (temp != NULL) {
        cnt++;
        temp = temp->link;
    }
    cout << "Number of nodes: " << cnt << endl;
}

void insertAtstart(node*& head, int val) {
    node* temp = new node;
    temp->data = val;
    temp->link = head;
    head = temp;
}

void insertAtposition(node*& head, int pos, int val) {
    if (pos <= 0) {
        cout << "Invalid position\n";
        return;
    }
    if (pos == 1) {
        insertAtstart(head, val);
        return;
    }

    node* temp = new node;
    temp->data = val;

    node* ptr = head;
    for (int i = 1; i < pos - 1 && ptr != NULL; i++) {
        ptr = ptr->link;
    }

    if (ptr == NULL) {
        cout << "Position out of range\n";
        delete temp;
        return;
    }

    temp->link = ptr->link;
    ptr->link = temp;
}

void deleteFirst(node*& head) {
    if (!head) return;
    node* temp = head;
    head = head->link;
    delete temp;
}

void deletelast(node*& head) {
    if (!head) return;
    if (!head->link) {
        delete head;
        head = NULL;
        return;
    }
    node* ptr = head;
    while (ptr->link->link != NULL) {
        ptr = ptr->link;
    }
    delete ptr->link;
    ptr->link = NULL;
}

void deleteposition(node*& head, int pos) {
    if (pos <= 0 || !head) return;
    if (pos == 1) {
        deleteFirst(head);
        return;
    }

    node* prev = head;
    for (int i = 1; i < pos - 1 && prev != NULL; i++) {
        prev = prev->link;
    }

    if (prev == NULL || prev->link == NULL) {
        cout << "Position out of range\n";
        return;
    }

    node* toDelete = prev->link;
    prev->link = toDelete->link;
    delete toDelete;
}

void reverseList(node*& head) {
    node* prev = NULL;
    node* curr = head;
    while (curr != NULL) {
        node* next = curr->link;
        curr->link = prev;
        prev = curr;
        curr = next;
    }
    head = prev;
}

int main() {
    int nodes;
    cout << "Enter number of nodes: ";
    cin >> nodes;

    node* head = NULL;
    createList(head, nodes);

    
    return 0;
}
