//============================================================================
// Name        : ABCU Project2.cpp
// Author      : Mark Beatty
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>
#include <algorithm>
#include <cstdlib>
#include <fstream>

#include "CSVparser.hpp"

using namespace std;

// Forward declarations
double strToDouble(string str, char ch);

// Structure to hold Course information
struct Course {
    string CourseId; // unique identifier
    string CourseName;
    string PreReq1;
    string PreReq2;
    Course() {
        PreReq2 = 0.0;
    }
};

// Structure for tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }
};

// Defines class to hold data
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string CourseId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string CourseId);
    Course Search(string CourseId);
};

// Initialize variables
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

BinarySearchTree::~BinarySearchTree() {
    // Deletes every node
}

// Goes through the tree in order
void BinarySearchTree::InOrder() {
    this->inOrder(root);
}

// Inserts a course into the tree
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        this->addNode(root, course);
    }
}

// Removes a course from the tree
void BinarySearchTree::Remove(string CourseId) {
    this->removeNode(root, CourseId);
}

// Searches the tree for a course
Course BinarySearchTree::Search(string CourseId) {
    Node* current = root;
    while (current != nullptr) {
        if (current->course.CourseId.compare(CourseId) == 0) {
            return current->course;
        }
        if (CourseId.compare(current->course.CourseId) < 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    Course course;
    return course;
}

// Adds a course to the tree
void BinarySearchTree::addNode(Node* node, Course course) {
    if (node->course.CourseId.compare(course.CourseId) > 0) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            this->addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            this->addNode(node->right, course);
        }
    }
}

// Searches through the tree in order
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.CourseId << ": "
            << node->course.CourseName << " | "
            << node->course.PreReq1 << " | "
            << node->course.PreReq2 << endl;
        inOrder(node->right);
    }
}

// Searches for then removes a node from the tree
Node* BinarySearchTree::removeNode(Node* node, string CourseId) {
    if (node == nullptr) {
        return node;
    }
    if (CourseId.compare(node->course.CourseId) < 0) {
        node->left = removeNode(node->left, CourseId);
    }
    else if (CourseId.compare(node->course.CourseId) > 0) {
        node->right = removeNode(node->right, CourseId);
    }
    else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        else if (node->right != nullptr && node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->course = temp->course;
            node->right = removeNode(node->right, temp->course.CourseId);
        }
    }
    return node;
}

// Displays course info
void displayBid(Course course) {
    cout << course.CourseId << ": " << course.CourseName << " | " << course.PreReq1 << " | "
        << course.PreReq2 << endl;
    return;
}

// Loads and stores a CSV file
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // Initializes the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // Reads and displays the header
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // Reads the CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Creates a data structure and adds course info to the collection
            Course course;
            course.CourseId = file[i][1];
            course.CourseName = file[i][0];
            course.PreReq1 = file[i][8];
            course.PreReq2 = strToDouble(file[i][4], '$');

            cout  << course.CourseName << ", " << course.PreReq1 << ", " << course.PreReq2 << endl;

            // Course goes to end of list
            bst->Insert(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

// Converts a string to a double and removes unwanted characters
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

int main(int argc, char* argv[]) {

    // Processes command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "CourseId";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "ABCU.csv";
        bidKey = "CourseId";
    }

   
    // Define binary search tree for bids
    BinarySearchTree* bst{};

    Course course;

    // Displays user options and allows for user to pick an option
    cout << "\nWelcome to the course planner!\n" << endl;
    int choice = 0;
    while (choice != 9) {   

        cout << "   1. Load Data Structure" << endl;
        cout << "   2. Print Course List" << endl;
        cout << "   3. Print Course" << endl;
        cout << "   9. Exit" << endl;
        cout << "\nWhat would you like to do? ";
        cin >> choice;

        switch (choice) {

            // Arguments for above cases
        case 1:
            bst = new BinarySearchTree();
           
            // Calls to load bids
            loadBids(csvPath, bst);
            break;

        case 2:
            bst->InOrder();
            break;
                   // Validate that a BST exists & search/ display course info
        case 3:

            if (bst != nullptr) {
                    
                cout << "Print Course";
            }
            else
                cout << "Load courses first - option 1\n" << endl;
            break;

        
        }
    }

    cout << "\nThank you, good bye!" << endl;

    return 0;
}
