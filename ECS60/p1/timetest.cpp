// Elaine Cho, Jordan Leung
// timetest.cpp
#include <iostream>
#include <fstream> // lib for files in/outputs 

#include "CPUTimer.h"
#include "dsexceptions.h"
#include "vector.h"

#include "LinkedList.h"
#include "StackLi.h"
#include "StackAr.h"
#include "CursorList.h"
#include "QueueAr.h"
#include "SkipList.h"

using namespace std;

const int MAX_SIZE = 500001;

vector<CursorNode <int> > cursorSpace(MAX_SIZE); // creates a vvector for somewhere to store it 


void RunList(string fileName) {
	List<int> list; // initializing empty list
	ListItr<int> itr = list.zeroth(); // returning an iterator to the head
	ifstream infile; // read from this file, look through this 
	infile.open(fileName.c_str()); // open only takes C strings, so use c_str()
	infile.ignore(500, '\n'); // skip first line 
	
	char cmd;
	int value; 

	while (infile >> cmd >> value) {
		if (cmd == 'i') {
			list.insert(value, itr);
		}
		else if (cmd == 'd') {
			list.remove(value);
		}
	}
}

void RunCursorList(string fileName) {
	CursorList<int> cursorList(cursorSpace); // initializing empty list
	CursorListItr<int> itr = cursorList.zeroth(); // returning an iterator to the head
	ifstream infile; // read from this file, look through this 
	infile.open(fileName.c_str()); // open only takes C strings, so use c_str()
	infile.ignore(500, '\n'); // skip first line 
	
	char cmd;
	int value; 

	while (infile >> cmd >> value) {
		if (cmd == 'i') {
			cursorList.insert(value, itr);
		}
		else if (cmd == 'd') {
			cursorList.remove(value);
		}
	}
}

void RunStackAr(string fileName) {
	StackAr<int> stack(MAX_SIZE);
	ifstream infile; // read from this file, look through this 
	infile.open(fileName.c_str()); // open only takes C strings, so use c_str()
	infile.ignore(500, '\n'); // skip first line 

	char cmd;
	int value;

	while (infile >> cmd >> value) {
		if (cmd == 'i') {
			stack.push(value);
		}
		else if (cmd == 'd') {
			stack.pop();
		}
	}
}

void RunStackLi(string fileName) {
	StackAr<int> stackLi(MAX_SIZE);
	ifstream infile; // read from this file, look through this 
	infile.open(fileName.c_str()); // open only takes C strings, so use c_str()
	infile.ignore(500, '\n'); // skip first line 

	char cmd;
	int value;

	while (infile >> cmd >> value) {
		if (cmd == 'i') {
			stackLi.push(value);
		}
		else if (cmd == 'd') {
			stackLi.pop();
		}
	}	
}

void RunQueueAr(string fileName) {
	Queue<int> queue(MAX_SIZE);
	ifstream infile; // read from this file, look through this 
	infile.open(fileName.c_str()); // open only takes C strings, so use c_str()
	infile.ignore(500, '\n'); // skip first line 

	char cmd;
	int value;

	while (infile >> cmd >> value) {
		if (cmd == 'i') {
			queue.enqueue(value);
		}
		else if (cmd == 'd') {
			queue.dequeue();
		}
	}	
}

void RunSkipList(string fileName) {
	SkipList<int> skipList(0, MAX_SIZE);
	ifstream infile; // read from this file, look through this 
	infile.open(fileName.c_str()); // open only takes C strings, so use c_str()
	infile.ignore(500, '\n'); // skip first line 

	char cmd;
	int value;

	while (infile >> cmd >> value) {
		if (cmd == 'i') {
			skipList.insert(value);
		}
		else if (cmd == 'd') {
			skipList.deleteNode(value);
		}
	}	
}

int getChoice () {
	int choice = 0;
    cout <<  endl;
	cout << "      ADT Menu" << endl;
	cout << "0. Quit" << endl;
	cout << "1. LinkedList" << endl;
	cout << "2. CursorList" << endl;
	cout << "3. StackAr" << endl;
	cout << "4. StackLi" << endl;
	cout << "5. QueueAr" << endl;
	cout << "6. SkipList" << endl;

	cout << "Your choice >> ";
	cin >> choice;
	return choice;

}

int main () {
	int choice = 0;
	CPUTimer ct; // from CPUTimer.h
	string fileName;

	cout << "Filename >> ";
	cin >> fileName;
	do {
		choice = getChoice();
		ct.reset();
		switch (choice) {
			case 1: RunList(fileName); break;
			case 2: RunCursorList(fileName); break;
			case 3: RunStackAr(fileName); break;
			case 4: RunStackLi(fileName); break;
			case 5: RunQueueAr(fileName); break;
			case 6: RunSkipList(fileName); break;
		}
		cout << "CPU time: " << ct.cur_CPUTime() << endl;
	}
	while (choice > 0);

	return 0;
}