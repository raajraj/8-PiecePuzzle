//
//  main.cpp
//  8-Piece-Puzzle
//
//  Created by Raajitha Rajkumar
//  SID: 862015848
//  CS 170: Intro to Artificial Intelligence
//  rrajk001@ucr.edu

#include <iostream>
#include <string>
#include <queue>
#include <list>
#include <iterator>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <utility>

using namespace std;

//-----------------------------------------------------------------------------------------------------------
// CLASSES
//-----------------------------------------------------------------------------------------------------------
// This is the node class. It mainly containes the puzzle
// priority = g(n) + h(n)
// gn is the g(n) or the depth from the initial to current state
// hueristic is the cost, and will be different depending on algorithm
// parent is the node's parent node

class node{
    public:
        int puzzle[3][3];
        int priority;
        int gn;
        int hueristic;
        node * parent;
    
        
};

// This is the problem class. It mainly containes the states and operators
// initial and goal state are nodes that contain the puzzles for each respectively
// There are four functions that perform as operators that return the nodes after it has been operated on

class problem{
    public:
        node * initial_state;
        node * goal_state;
        node * moveUp(node *);
        node * moveDown(node *);
        node * moveLeft(node *);
        node * moveRight(node *);
};

// I decided to make a solution class that holds all the info needed for the solution
// the node for the solution
// the maximum number of nodes in a queue at a time
// the times the nodes have been expanded
// a message to display if it is failed

class solution{
    public:
        node * reached_goal;
        long maxNodes;
        int timesExpanded;
        void failed(){
            cout << endl << "The Algorithm Failed" << endl;
        };
};

// This class is needed for my priority queue
// When assigning priorities, I need to compare the priority of the value and not the puzzle
// This class allows me to do so which cannot normally be done with priority queue functions in c++

class compare{
    public:
        int operator() (node* &a, node* &b){
            return a->priority > b->priority;
        }
};

//-----------------------------------------------------------------------------------------------------------
// OPERATORS
//-----------------------------------------------------------------------------------------------------------

/* The next following four functions are operators for the problem class
 These operators are all the same except they move the blank to different positions.
 All these functions take in a node, shift it around, and return the node with the
 shifted puzzle in place. The reason I reassign the node in the beginning is to
 avoid changing the orginal node that gets passed in. I will explain the first function
 which can serve as an explaination for the next ones. */

node * problem:: moveUp(node * current_puzzle){
    node * new_leaf = new node; // new leaf node
    
    // iteration through puzzle take in the parameter to assign to new node
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            new_leaf->puzzle[i][j] = current_puzzle->puzzle[i][j];
        }
    }
    
    // working with new node
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(new_leaf->puzzle[i][j] == 0){ // if blank found
                if(i == 0){ // if i == 0, the blank is at the highest and cannot move up
                    return current_puzzle;
                }else{ // else swap the 0 with the node above it
                    new_leaf->puzzle[i][j] = new_leaf->puzzle[i-1][j];
                    new_leaf->puzzle[i-1][j] = 0;
                    return new_leaf;
                }
            }
        }
    }
    
    return new_leaf;
}

node * problem:: moveDown(node * current_puzzle){
    node * new_leaf = new node;
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            new_leaf->puzzle[i][j] = current_puzzle->puzzle[i][j];
        }
    }
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(new_leaf->puzzle[i][j] == 0){
                if(i == 2){ // if i == 2, the blank is at the highest and cannot move down
                    return current_puzzle;
                }else{ // else swap the 0 with the node below it
                    new_leaf->puzzle[i][j] = new_leaf->puzzle[i+1][j];
                    new_leaf->puzzle[i+1][j] = 0;
                    return new_leaf;
                }
            }
        }
    }
    
    return new_leaf;
}

node * problem:: moveRight(node * current_puzzle){
    node * new_leaf = new node;
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            new_leaf->puzzle[i][j] = current_puzzle->puzzle[i][j];
        }
    }
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(new_leaf->puzzle[i][j] == 0){
                if(j == 2){ // if j == 2, the blank is at the highest and cannot move right
                    return current_puzzle;
                    
                }else{ // else swap the 0 with the node above ti the right of it
                    new_leaf->puzzle[i][j] = new_leaf->puzzle[i][j+1];
                    new_leaf->puzzle[i][j+1] = 0;
                    return new_leaf;
                }
            }
        }
    }
    
    return new_leaf;
}

node * problem:: moveLeft(node * current_puzzle){
    node * new_leaf = new node;
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            new_leaf->puzzle[i][j] = current_puzzle->puzzle[i][j];
        }
    }
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(new_leaf->puzzle[i][j] == 0){
                if(j == 0){ // if j == 0, the blank is at the highest and cannot move left
                    return current_puzzle;
                }else{ // else swap the 0 with the node to the left of it
                    new_leaf->puzzle[i][j] = new_leaf->puzzle[i][j-1];
                    new_leaf->puzzle[i][j-1] = 0;
                    return new_leaf;
                }
            }
        }
    }
    
    return new_leaf;
}

//-----------------------------------------------------------------------------------------------------------
// GLOBAL FUNCTIONS
//-----------------------------------------------------------------------------------------------------------

// This is a global function that prints any puzzle with a node parameter

void printPuzzle(node * puzzle){
    for(int i = 0; i < 3; i++){
        cout << endl << "-------------" << endl;
        cout << "| ";
        for(int j = 0; j < 3; j++){
            cout << puzzle->puzzle[i][j] << " | ";
        }
        
    }
    cout << endl << "-------------" << endl;
    cout << "g(n) = " << puzzle->gn << " h(n) " << puzzle->hueristic << endl;
}

/* This function is a very simple function that acquires the user's input
 and converts it into working integers in a 2D array.
 It has a for loop for each line of input and returns a node with the puzzle*/
node * acquirePuzzle(string f, string s, string t){
    node * initialPuzzle = new node;
    string one, two, three;
    unsigned long buffer = 0;
    int spaceCount = 0;
    
    // goes through the first string and looks for a space
    for(unsigned long i = 0; i < f.size(); i++){
        if(f.at(i) == ' ' && spaceCount == 0){ // if theres a space, get first integer
            buffer = i; // use buffer for next char
            one = f.substr(0,i); // grab string
            initialPuzzle->puzzle[0][0] = stoi(one); // convert to int
            spaceCount++; // increase space count
        }else if(f.at(i) == ' ' && spaceCount == 1){ // next char found
            two = f.substr(buffer + 1, i - buffer); // assign second digit
            initialPuzzle->puzzle[0][1] = stoi(two); // convert it to an int
            three = f.substr(i+1, f.size() - i+1); // now you know the third is after the second
            initialPuzzle->puzzle[0][2] = stoi(three); // convert it to an integer
        }
    }
    
    // reset variables
    spaceCount = 0;
    buffer = 0;
    
    // The following code does the same exact as the above but with strings s and t
    
    for(unsigned long i = 0; i < s.size(); i++){
        if(s.at(i) == ' ' && spaceCount == 0){
            buffer = i;
            one = s.substr(0,i);
            initialPuzzle->puzzle[1][0] = stoi(one);
            spaceCount++;
        }else if(s.at(i) == ' ' && spaceCount == 1){
            two = s.substr(buffer + 1, i - buffer);
            initialPuzzle->puzzle[1][1] = stoi(two);
            three = s.substr(i+1, s.size() - i+1);
            initialPuzzle->puzzle[1][2] = stoi(three);
        }
    }
    
    spaceCount = 0;
    buffer = 0;
    
    for(unsigned long i = 0; i < t.size(); i++){
        if(t.at(i) == ' ' && spaceCount == 0){
            buffer = i;
            one = t.substr(0,i);
            initialPuzzle->puzzle[2][0] = stoi(one);
            spaceCount++;
        }else if(t.at(i) == ' ' && spaceCount == 1){
            two = t.substr(buffer + 1, i - buffer);
            initialPuzzle->puzzle[2][1] = stoi(two);
            
            three = t.substr(i+1, t.size() - i+1);
            initialPuzzle->puzzle[2][2] = stoi(three);
        }
    }
    
    return initialPuzzle;
}

// This simply iterates through the puzzle and checks for
// equality between the two nodes given in the parameter
int checkEquality(node * curr, node * goal){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(curr->puzzle[i][j] != goal->puzzle[i][j]){
                return 0;
            }
        }
    }
    
    return 1;
}

// There is an explored list and a frontier list.
// Though the frontier is in a p_queue, I made
// a list as well to make it iterable. This code
// simply iterates and used the checkEquality function
// that checks to see if the node has been explored or in the frontier

int checkIfExploredAndFrontiered(node * curr, list<node*> explored, list<node*> front){
    for(list<node*>::iterator it = explored.begin(); it != explored.end(); ++it){
        if(checkEquality(curr, *it)){
            return 0;
        }
    }
    
    for(list<node*>::iterator it = front.begin(); it != front.end(); ++it){
        if(checkEquality(curr, *it)){
            return 0;
        }
    }

    return 1;
}

// This is a simple function that checks for the max value in a list
// I again use an iterator, as it is easy to implement to iterate
// through a list of node values that gets appended from the algorithms

long assignMaxNode(list <long> pqueue){
    long max = 0;
    for(list<long>::iterator it = pqueue.begin(); it != pqueue.end(); ++it){
        if(*it > max){
            max = *it;
        }
    }

    return max;
}

//-----------------------------------------------------------------------------------------------------------
// ALGORITHMS
//-----------------------------------------------------------------------------------------------------------
// This function serves as my uniform cost search.
// It takes in a problem and returns a solution
// g(n) represents depth and h(n) is set to 0
solution * UniformCostSearch(problem given){
    int priority = 0; // set the priority or g(n) to 0
    long maxNodeCount = 0; // maximum amount of nodes in a queue
    int expanded = 0; // amount of times expanded
    solution * goalReached = new solution; // initiate a solution
    node * goal = given.goal_state; // initiate a goal node
    node * start = given.initial_state; // initiate a start node
    start->priority = priority; // set g(n)
    start->hueristic = 0; // set to 0
    start->parent = NULL; // no parent for head node
    
    priority_queue<node*, vector<node*>, compare> frontier; // this priority queue prioritizes the frontier by g(n) and stores the nodes
    list <node*> frontierIterable; // this is the frontier stored in a list so it is iterable
    list <node*> explored; // this is a list of the explored nodes
    list <long> pqueuesize; // queue for max nodes
    
    frontier.push(start); // we push the start node to the frontier
    frontierIterable.push_back(start); // whatever we push to the frontier will get pushed to the list
    
    while(!frontier.empty()){
        // if the frontier is empty, it is failed
        if(!frontier.top()){
            return goalReached;
        }
        
        priority++; //update the depth g(n)

        node * leaf = frontier.top(); // pick out a new lead
        explored.push_back(leaf); // we add our leaf to the explored list
        printPuzzle(leaf); // print the leaf for the trace
        frontier.pop(); // pop the leaf
        
        // if the leaf equals the goal, we return it
        if(checkEquality(leaf, goal)){
            // assign solution object
            goalReached->reached_goal = leaf;
            goalReached->maxNodes = maxNodeCount;
            goalReached->timesExpanded = expanded;
            goalReached->reached_goal->hueristic = 0;
            cout << endl << endl << "GOAL REACHED!" << endl;
            return goalReached;
        }
        
        cout << endl << "expanding node..." << endl;
        expanded++; // update expanded
        
        // We start the process of expanding the leaf
        // First, we initiate a node to move in a direction
        // Then, we make the parent the leaf
        // We must check if the node has been in the frontier or the explored list before adding it to the frontier
        
        node * up = given.moveUp(leaf);
        up->parent = leaf;
        if(checkIfExploredAndFrontiered(up, explored, frontierIterable)){
            cout << "moving up" << endl;
            up->priority = priority;
            up->gn = priority;
            up->hueristic = 0;
            frontier.push(up);
            frontierIterable.push_back(up);
        }
        
        node * down = given.moveDown(leaf);
        down->parent = leaf;
        if(checkIfExploredAndFrontiered(down, explored, frontierIterable)){
            cout << "moving down" << endl;
            down->priority = priority;
            down->gn = priority;
            down->hueristic = 0;
            frontier.push(down);
            frontierIterable.push_back(down);
        }
        
        
        node * right = given.moveRight(leaf);
        right->parent = leaf;
        if(checkIfExploredAndFrontiered(right, explored, frontierIterable)){
            cout << "moving right" << endl;
            right->priority = priority;
            right->gn = priority;
            right->hueristic = 0;
            frontier.push(right);
            frontierIterable.push_back(right);
        }
        
       
        node * left = given.moveLeft(leaf);
        left->parent = leaf;
        if(checkIfExploredAndFrontiered(left, explored, frontierIterable)){
            cout << "moving left" << endl;
            left->priority = priority;
            left->gn = priority;
            left->hueristic = 0;
            frontier.push(left);
            frontierIterable.push_back(left);
        }
        
        pqueuesize.push_back(frontier.size()); // append max node queue
        maxNodeCount = assignMaxNode(pqueuesize); // acquire max node
        
    }
    
    return goalReached;
}

/*---------------------
 |  11  |  12  |  13  |
 ----------------------
 |  21  |  22  |  23  |
 ----------------------
 |  31  |  32  |  33  |
 ----------------------*/

/* the distance will be worked out as follows and be multiplied by 2 for simplicity purposes
 
 euclidean distance = sqrt((x2^2 - x1^2) + (y2^2 - y1^2))
 
 */

int findEuclideanHueristic(node * init){
    int euclideanDistance = 0;
    int count = 1; // the count variable is essentially the variable that is the value on the goal of our puzzle
    int x, y, xcoord, ycoord = 0; // these values will be used for calculating the euclidean distance
    
    // distance look up table, this is the coordinates of the puzzle by goal value
    vector<int> distance[9];
    
    distance[0] = {2, 2};
    distance[1] = {0, 0};
    distance[2] = {0, 1};
    distance[3] = {0, 2};
    distance[4] = {1, 0};
    distance[5] = {1, 1};
    distance[6] = {1, 2};
    distance[7] = {2, 0};
    distance[8] = {2, 1};
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if((init->puzzle[i][j] != count && count != 9) || (init->puzzle[i][j] == 0 && count != 9)){ // when a value does not match up with goal
                
                xcoord = distance[init->puzzle[i][j]].at(0); // calculate coordinates from lookup table
                ycoord = distance[init->puzzle[i][j]].at(1); // calculate coordinates from lookup table
                x = abs(i - xcoord); // get absolute value
                y = abs(j - ycoord); // get absolute value
                euclideanDistance = euclideanDistance + hypot(x , y); // hypotenuse function will return distance
            }
            count++; // raise the value
        }
    }
    
    return euclideanDistance;
}

solution * A_EuclideanDistance(problem given){
    long maxNodeCount = 0; // maximum amount of nodes in a queue
    int expanded = 0; // amount of times expanded
    int gn = 0; // assign g(n)
    
    solution * goalReached = new solution; // initiate a solution
    node * goal = given.goal_state; // initiate a goal node
    node * start = given.initial_state; // initiate a start node
    
    start->gn = 0; // depth is 0
    start->hueristic = findEuclideanHueristic(start); // call to find h(n)
    start->priority = start->gn + start->hueristic; // add the g(n) and h(n)
    start->parent = NULL; // start has no parent
    
    priority_queue<node*, vector<node*>, compare> frontier; // this priority queue prioritizes the frontier by g(n) + h(n) and stores the nodes
    list <node*> frontierIterable; // this is the frontier stored in a list so it is iterable
    list <node*> explored; // this is a list of the explored nodes
    list <long> pqueuesize; // this is a queue for the max node count
    
    frontier.push(start); // we push the start node to the frontier
    frontierIterable.push_back(start); // whatever we push to the frontier will get pushed to the list
    
    while(!frontier.empty()){
        // if the frontier is empty, it is failed
        if(!frontier.top()){
            return goalReached;
        }
        
        gn++; //update the depth g(n)

        node * leaf = frontier.top(); // pick out a new leaf
        explored.push_back(leaf); // we add our leaf to the explored list
        printPuzzle(leaf); // print the leaf for the trace
        frontier.pop();
        
        // if the leaf equals the goal, we return it
        if(checkEquality(leaf, goal)){
            goalReached->reached_goal = leaf;
            goalReached->maxNodes = maxNodeCount;
            goalReached->timesExpanded = expanded;
            goalReached->reached_goal->gn = gn;
            goalReached->reached_goal->hueristic = findEuclideanHueristic(leaf);
            
            return goalReached;
        }
        
        cout << endl << "expanding node..." << endl;
        expanded++; // update times expanded
        
        // We start the process of expanding the leaf
        // First, we initiate a node to move in a direction
        // Then, we make the parent the leaf
        // We must check if the node has been in the frontier or the explored list before adding it to the frontier
        
        node * up = given.moveUp(leaf);
        up->parent = leaf;
        if(checkIfExploredAndFrontiered(up, explored, frontierIterable)){
            cout << "moving up" << endl;
            up->hueristic = findEuclideanHueristic(up);
            up->gn = gn;
            up->priority = up->hueristic;
            frontier.push(up);
            frontierIterable.push_back(up);
        }
        
        node * down = given.moveDown(leaf);
        down->parent = leaf;
        if(checkIfExploredAndFrontiered(down, explored, frontierIterable)){
            cout << "moving down" << endl;
            down->hueristic = findEuclideanHueristic(down);
            down->gn = gn;
            down->priority = down->hueristic;
            frontier.push(down);
            frontierIterable.push_back(down);
        }
        
        
        node * right = given.moveRight(leaf);
        right->parent = leaf;
        if(checkIfExploredAndFrontiered(right, explored, frontierIterable)){
            cout << "moving right" << endl;
            right->hueristic = findEuclideanHueristic(right);
            right->gn = gn;
            right->priority = right->hueristic;
            frontier.push(right);
            frontierIterable.push_back(right);
        }
        
       
        node * left = given.moveLeft(leaf);
        left->parent = leaf;
        if(checkIfExploredAndFrontiered(left, explored, frontierIterable)){
            cout << "moving left" << endl;
            left->hueristic = findEuclideanHueristic(left);
            left->gn = gn;
            left->priority = left->hueristic;
            frontier.push(left);
            frontierIterable.push_back(left);
        }
        
        pqueuesize.push_back(frontier.size());
        maxNodeCount = assignMaxNode(pqueuesize);
    }
    
    return goalReached;
}

// This function is a helper to the misplaced tile
// It goes through the given node's puzzle
// and returns a value for amount of tiles misplaced

int findMisplacedTileHueristic(node * start){
    int misplaced = 0; // counter
    int count[] = {1, 2, 3, 4, 5, 6, 7, 8, 0}; // lookup table
    int k = 0; // index
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(start->puzzle[i][j] != count[k]){ // if it does not equal goal look up table
                misplaced++; // increment counter
            }
            k++;
        }
    }
    
    return misplaced;
}

solution * A_MisplacedTile(problem given){
    long maxNodeCount = 0; // maximum amount of nodes in a queue
    int expanded = 0; // amount of times expanded
    int gn = 0;
    solution * goalReached = new solution; // initiate a solution
    node * goal = given.goal_state; // initiate a goal node
    node * start = given.initial_state; // initiate a start node
    
    start->gn = 0;
    start->hueristic = findMisplacedTileHueristic(start);
    start->priority = start->gn + start->hueristic;
    start->parent = NULL;
    
    priority_queue<node*, vector<node*>, compare> frontier; // this priority queue prioritizes the frontier by g(n) and stores the nodes
    list <node*> frontierIterable; // this is the frontier stored in a list so it is iterable
    list <node*> explored; // this is a list of the explored nodes
    list <long> pqueuesize;
    
    frontier.push(start); // we push the start node to the frontier
    frontierIterable.push_back(start); // whatever we push to the frontier will get pushed to the list
    
    while(!frontier.empty()){
        // if the frontier is empty, it is failed
        if(!frontier.top()){
            return goalReached;
        }
        
        gn++; //update the depth g(n)

        node * leaf = frontier.top(); // pick out a new lead
        explored.push_back(leaf); // we add our leaf to the explored list
        printPuzzle(leaf); // print the leaf for the trace
        frontier.pop();
        
        // if the leaf equals the goal, we return it
        if(checkEquality(leaf, goal)){
            goalReached->reached_goal = leaf;
            goalReached->maxNodes = maxNodeCount;
            goalReached->timesExpanded = expanded;
            return goalReached;
        }
        
        cout << endl << "expanding node..." << endl;
        expanded++;
        
        // We start the process of expanding the leaf
        // First, we initiate a node to move in a direction
        // Then, we make the parent the leaf
        // We must check if the node has been in the frontier or the explored list before adding it to the frontier
        
        node * up = given.moveUp(leaf);
        up->parent = leaf;
        if(checkIfExploredAndFrontiered(up, explored, frontierIterable)){
            cout << "moving up" << endl;
            up->hueristic = findMisplacedTileHueristic(up);
            up->gn = gn;
            up->priority = up->hueristic;
            frontier.push(up);
            frontierIterable.push_back(up);
        }
        
        node * down = given.moveDown(leaf);
        down->parent = leaf;
        if(checkIfExploredAndFrontiered(down, explored, frontierIterable)){
            cout << "moving down" << endl;
            down->hueristic = findMisplacedTileHueristic(down);
            down->gn = gn;
            down->priority = down->hueristic;
            frontier.push(down);
            frontierIterable.push_back(down);
        }
        
        
        node * right = given.moveRight(leaf);
        right->parent = leaf;
        if(checkIfExploredAndFrontiered(right, explored, frontierIterable)){
            cout << "moving right" << endl;
            right->hueristic = findMisplacedTileHueristic(right);
            right->gn = gn;
            right->priority = right->hueristic;
            frontier.push(right);
            frontierIterable.push_back(right);
        }
        
       
        node * left = given.moveLeft(leaf);
        left->parent = leaf;
        if(checkIfExploredAndFrontiered(left, explored, frontierIterable)){
            cout << "moving left" << endl;
            left->hueristic = findMisplacedTileHueristic(left);
            left->gn = gn;
            left->priority = left->hueristic;
            frontier.push(left);
            frontierIterable.push_back(left);
        }
        
        pqueuesize.push_back(frontier.size());
        maxNodeCount = assignMaxNode(pqueuesize);
    }
    
    return goalReached;
}

int main() {
    string firstLine, secondLine, thirdLine;
    string firstInput, secondInput;
    node * initialState = new node;
    node * goalState = new node;
    solution * finalSolution = new solution;
    problem gameProblem;
    
    // We begin by asking for input
    cout << "Welcome to the 862015848 8-Piece-Puzzle!\n\n";
    cout << "Enter 1 for a default puzzle or 2 to input your own puzzle: ";
    getline(cin, firstInput);
    
    // if the input is for user puzzle, get the user input
    if(firstInput == "2"){
        cout << "input the lines of the puzzle, separate with spaces, and use a 0 as a blank: " << endl;
        cout << "Enter the first line: ";
        getline(cin, firstLine);

        cout << "Enter the second line: ";
        getline(cin, secondLine);
        
        cout << "Enter the third line: ";
        getline(cin, thirdLine);
        cout << endl;
        
        initialState = acquirePuzzle(firstLine, secondLine, thirdLine);
        initialState->gn = 0;
        initialState->hueristic = 0;
        
        cout << "YOUR PUZZLE WITH COST SET TO 0" << endl;
        printPuzzle(initialState);
        
    }else{ // else use default puzzle
        initialState->puzzle[0][0] = 1;
        initialState->puzzle[0][1] = 2;
        initialState->puzzle[0][2] = 0;
        initialState->puzzle[1][0] = 4;
        initialState->puzzle[1][1] = 5;
        initialState->puzzle[1][2] = 3;
        initialState->puzzle[2][0] = 7;
        initialState->puzzle[2][1] = 8;
        initialState->puzzle[2][2] = 6;
    }
    
    // goal puzzle will always be the same
    goalState->puzzle[0][0] = 1;
    goalState->puzzle[0][1] = 2;
    goalState->puzzle[0][2] = 3;
    goalState->puzzle[1][0] = 4;
    goalState->puzzle[1][1] = 5;
    goalState->puzzle[1][2] = 6;
    goalState->puzzle[2][0] = 7;
    goalState->puzzle[2][1] = 8;
    goalState->puzzle[2][2] = 0;
    
    gameProblem.initial_state = initialState; // initiate start state
    gameProblem.goal_state = goalState; // initiate goal state
    
    // ask user which algorithm should be selected
    cout << endl << "Pick an algorithm to use for your search:" << endl;
    cout << endl << "1. Uniform Cost Search" << endl;
    cout << "2. A * Misplaced Tile" << endl;
    cout << "3. A * Euclidean Distance" << endl << endl;
    
    getline(cin, secondInput);
    
    if(secondInput == "1"){
        finalSolution = UniformCostSearch(gameProblem); // execute UCS
    }else if(secondInput == "2"){
        finalSolution = A_MisplacedTile(gameProblem); // execute misplaced tile
    }else{
        finalSolution = A_EuclideanDistance(gameProblem); // execute euclidean distance
    }
    
    cout << endl << "Goal was reached in " << finalSolution->reached_goal->gn << " moves..." << endl;
    cout << endl << "The maximum number of nodes in the queue was " << finalSolution->maxNodes << " times!" << endl;
    cout << "The search algorithm expanded a total of " << finalSolution->timesExpanded << " times!" << endl;
    
    
    
    return 0;
}
