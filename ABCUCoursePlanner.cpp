//============================================================================
// Name        : ABCUCoursePlanner (Project 2)
// Author      : Jonathan Warner
// Version     : 1.0
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

// Course Structure
struct Course {
	string courseId; // Unique identifier
	string courseTitle; // Title of course
	vector<string> prereqs; // Vector of course's prerequisite courses
};

// Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// Default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// Initialize with a course
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

// Class containing data members and methods that implement a binary search tree

class BinarySearchTree {
private:
	Node* root;

	void addNode(Node* node, Course course);
	void printInOrder(Node* node);
	
public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void PrintAll();
	void Insert(Course course);
	Course Search(string courseId);
};

// Default Constructor
BinarySearchTree::BinarySearchTree() {
	// Initializes root by setting it equal to nullptr
		root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
	// Recurses from root deleting every node
}

// Prints the entire tree in order
void BinarySearchTree::PrintAll() {
	// Calls printInOrder with root to begin printing tree
	this->printInOrder(root);
}

// Print courses in tree in order from left to right
void BinarySearchTree::printInOrder(Node* node) {
	// If tree is empty, print sample schedule
	if (root == nullptr) {
		cout << "Here is a sample schedule: " << endl;
		cout << "CSCI100, Introduction to Computer Science" << endl;
		cout << "CSCI101, Introduction to Programming in C++" << endl;
		cout << "CSCI200, Data Structures" << endl;
		cout << "CSCI301, Advanced Programming in C++" << endl;
		cout << "CSCI300, Introduction to Algorithms" << endl;
		cout << "CSCI350, Operating Systems" << endl;
		cout << "CSCI400, Large Software Development" << endl;
		cout << "MATH201, Discrete Mathematics" << endl;
		cout << endl;
	}
	// If starting node is not empty print course's information in order from left to right
	if (node != nullptr) {
		printInOrder(node->left);
		cout << node->course.courseId << ", " << node->course.courseTitle;
		for (string prereq : node->course.prereqs) {
			cout << ", " << prereq;
		}
		cout << endl;
		printInOrder(node->right);
	}
}

// Adds course to tree with node as current node in tree
void BinarySearchTree::addNode(Node* node, Course course) {
	// If inputted node's course's courseId is greater than course's courseId, add course to left subtree
	if (node->course.courseId.compare(course.courseId) > 0) {
		// If left subtree is empty, create new node using course
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		// Else call addNode to continue down left subtree
		else {
			this->addNode(node->left, course);
		}
	}
	// Else add to right subtree
	else {
		// If right subtree is empty, create new node using course
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		// Else call addNode to continue down right subtree
		else {
			this->addNode(node->right, course);
		}
	}
}

// Public interface for inserting new courses into tree
void BinarySearchTree::Insert(Course course) {
	// Sets root of tree to inputted course if tree is empty
	if (root == nullptr) {
		root = new Node(course);
	}
	// Else adds inputted course to root for sorting
	else {
		this->addNode(root, course);
	}
}

// Public interface for searching the tree for specific course
Course BinarySearchTree::Search(string courseId) {
	// Creates node to begin search at root
	Node* current = root;

	// Keeps looping down tree until desired course is found or bottom of tree is reached
	while (current != nullptr) {
		// If current course matches, return it
		if (courseId.compare(current->course.courseId) == 0) {
			return current->course;
		}
		// If courseId is smaller than current, traverse left subtree
		if (courseId.compare(current->course.courseId) < 0) {
			current = current->left;
		}
		// Else traverse right subtree
		else if (courseId.compare(current->course.courseId) > 0) {
			current = current->right;
		}

	}
	// Returns dummy course if desired course is not found
	Course course;
	course.courseId = "NOMATCH";
	cout << "No matching courses found" << endl;
	return course;
}

// Creates a course from inputted elements
Course createCourse(vector<string> elements) {
	// Course variable to store inputted data
	Course course;
	for (int i = 0; i < elements.size(); ++i) {
		// Sets courseId to the first element in elements
		if (i == 0) {
			course.courseId = elements.at(i);
		}
		// Sets courseTitle to the second element in elements
		if (i == 1) {
			course.courseTitle = elements.at(i);
		}
		// If current element is third or greater add element to prereqs vector
		if (i > 1) {
			course.prereqs.push_back(elements.at(i));
		}
	}
	// Return newly created course
	return course;
}

/* Opens file with inputted fileName
   Reads each line and creates and course from inputs
   Stores created course into inputted BinarySearchTree
*/
bool openFile(string fileName, BinarySearchTree* tree) {
	// Variables to store data from inputFile
	string inputLine;
	string tempInput;
	vector<string> elements;
	vector<string> prereqs;
	Course tempCourse;

	// Boolean for error checking
	bool dataLoaded = false;

	// Opens inputfile
	ifstream inputFile(fileName);

	// Checks if inputfile was opened correctly
	if (!inputFile.is_open()) {
		cout << "Error opening file" << endl;
		return dataLoaded;
	}

	// Reads each line in file
	while (getline(inputFile, inputLine)) {
		// Clears previous elements
		elements.clear();
		istringstream inputStream(inputLine);

		// Reads each line up to delimiter character: ','
		while (getline(inputStream, tempInput, ',')) {
			elements.push_back(tempInput);
		}
	// Checks if inputted entries have the required amount of parameters
	// Required amount is 2 or more
	if (elements.size() < 2) {
		cout << "ERROR: An entry has less than 2 parameters" << endl;
		continue;
	}

	// Create course with inputted elements using create course
	// Store in tempCourse
	tempCourse = createCourse(elements);

	// Add each prereq from tempCourse to prereqs
	for (string prereq : tempCourse.prereqs) {
		prereqs.push_back(prereq);
		}
	// Insert tempCourse into binary search tree
	tree->Insert(tempCourse);
	}

	// Create course for prerequisite check
	Course reqChecker;

	// For each prereq in prereqs, check if every course has valid prerequisites
	for (string prereq : prereqs) {
		reqChecker = tree->Search(prereq);
		if (reqChecker.courseId == "NOMATCH") {
			cout << "ERROR: A course has a prerequisite that does not exist in the list of courses." << endl;
			cout << "Missing prerequisite: " << prereq << endl;
			break;
		}
	}
	dataLoaded = true;
	return dataLoaded;
}

// Displays main menu to user and allow for interaction
void mainMenu(BinarySearchTree* tree) {
	// Variable to control menu selection
	int choice = 0;
	string userInput;
	Course course;
	bool dataLoaded = false;

	// Prints each menu option and handles main menu interaction with switch cases
	while (choice != 4 && choice != 9) {
		cout << "Main Menu:" << endl;
		cout << "1. Load Courses" << endl;
		cout << "2. Print All Courses" << endl;
		cout << "3. Print Course" << endl;
		cout << "9. Quit" << endl;

		// Get user's input for menu selection
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(99, '\n');
			cout << "Invalid option. Enter a number." << endl;
			continue;
		}
		else {
			switch (choice) {

				// Opens file and load courses
			case 1:
				cout << "Enter file name for course input:" << endl;

				// Gets user input for file name
				cin >> userInput;

				// Calls openFile using file name and passes the binary search tree
				dataLoaded = openFile(userInput, tree);
				break;

				// Prints every course alphanumerically
			case 2:
				tree->PrintAll();
				cout << endl;
				break;

				// Prints desired course based on courseId
			case 3:
				if (!dataLoaded) {
					cout << "ERROR: No user data loaded" << endl;
					break;
				}
				cout << "Enter desired Course Identification Number: " << endl;
				// Get user input for desired course
				cin >> userInput;
				// Converts user input to uppercase string
				transform(userInput.begin(), userInput.end(), userInput.begin(), ::toupper);

				// Searches for desired course with Search
				course = tree->Search(userInput);

				// If desired course is found, print it
				if (course.courseId != "NOMATCH") {
					cout << course.courseId << ", " << course.courseTitle << endl;
					cout << "Prerequisites: ";
					if (course.prereqs.size() > 0) {
						for (int i = 0; i < course.prereqs.size(); ++i) {
							cout << course.prereqs.at(i);
							if (i < course.prereqs.size() - 1) {
								cout << ", ";
							}
						}
						cout << endl;
					}
					else {
						cout << "None" << endl;
					}
					cout << endl;
				}
				break;

				// Exits program
				// Covers case 4 because it's more intuitive and case 9 because it's the option actually listed
			case 4:
			case 9:
				cout << "Thank you for using the course planner!" << endl;
				exit(0);
				break;

			default:
				cout << choice << " is not a valid option. " << endl;
				continue;
			}
		}
	}
}


int main() {
	// Create binary search tree to store courses
	BinarySearchTree* tree;
	tree = new BinarySearchTree();

	// Display main menu
	mainMenu(tree);
}
