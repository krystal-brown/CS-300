//============================================================================
// Name        : LinkedList.cpp
// Author      : Krystal Brown
// Version     : 1.0
// Description : Lab 3-2 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include "CSVparser.hpp"

using namespace std;

// Forward declarations (MUST be above the class)
struct Bid;
double strToDouble(string str, char ch);
void displayBid(Bid bid);

// Simple struct to hold bid info
struct Bid {
    string bidId;
    string title;
    string fund;
    double amount;

    Bid() { amount = 0.0; }
};

//============================================================================
// LinkedList class
//============================================================================
class LinkedList {

private:
    struct Node {
        Bid bid;
        Node* next;

        Node() : next(nullptr) {}
        Node(Bid aBid) : bid(aBid), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

// Constructor
LinkedList::LinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Destructor
LinkedList::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

// Append
void LinkedList::Append(Bid bid) {
    Node* newNode = new Node(bid);

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }

    size++;
}

// Prepend
void LinkedList::Prepend(Bid bid) {
    Node* newNode = new Node(bid);

    newNode->next = head;
    head = newNode;

    if (tail == nullptr) {
        tail = newNode;
    }

    size++;
}

// PrintList
void LinkedList::PrintList() {
    Node* current = head;

    while (current != nullptr) {
        displayBid(current->bid);   // calls free function
        current = current->next;
    }
}

// Remove
void LinkedList::Remove(string bidId) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->bid.bidId == bidId) {

            if (previous == nullptr) {
                head = current->next;
                if (current == tail) {
                    tail = nullptr;
                }
            }
            else {
                previous->next = current->next;
                if (current == tail) {
                    tail = previous;
                }
            }

            delete current;
            size--;
            return;
        }

        previous = current;
        current = current->next;
    }
}

// Search
Bid LinkedList::Search(string bidId) {
    Node* current = head;

    while (current != nullptr) {
        if (current->bid.bidId == bidId) {
            return current->bid;
        }
        current = current->next;
    }

    return Bid();
}

// Size
int LinkedList::Size() {
    return size;
}

//============================================================================
// Helper functions
//============================================================================

// Display a bid
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | "
        << bid.amount << " | " << bid.fund << endl;
}

// Prompt user for bid info
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

// Load bids from CSV
void loadBids(string csvPath, LinkedList* list) {
    cout << "Loading CSV file " << csvPath << endl;

    csv::Parser file = csv::Parser(csvPath);

    try {
        for (int i = 0; i < file.rowCount(); i++) {
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            list->Append(bid);
        }
    }
    catch (csv::Error& e) {
        cerr << e.what() << endl;
    }
}

// Convert string to double
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

//============================================================================
// Main program
//============================================================================

int main(int argc, char* argv[]) {

    string csvPath, bidKey;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98109";
    }

    clock_t ticks;
    LinkedList bidList;
    Bid bid;

    int choice = 0;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);
            break;

        case 2:
            ticks = clock();
            loadBids(csvPath, &bidList);
            cout << bidList.Size() << " bids read" << endl;
            ticks = clock() - ticks;
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 3:
            bidList.PrintList();
            break;

        case 4:
            ticks = clock();
            bid = bidList.Search(bidKey);
            ticks = clock() - ticks;

            if (!bid.bidId.empty()) {
                displayBid(bid);
            }
            else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 5:
            bidList.Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;
    return 0;
}