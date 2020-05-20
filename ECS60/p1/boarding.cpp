// Jordan Leung, Elaine Cho 
// boarding.cpp

#include <iostream>
#include <fstream>
#include "StackAr.h"
#include "QueueAr.h"
typedef enum { none, storL1, storL2, ACout, DFout, ACin, DFin} stateEnum;

using namespace std;

class Row {
	public:
		int num; // row number 
		StackAr<char> leftSeats; // left column, 3 seats/row
		StackAr<char> rightSeats; // right column 3 seats/row
		StackAr<char> leftWait; // up to two people waiting
		StackAr<char> rightWait; // up to two people waiting
		int aisleNum; // holds the assigned row number of the passenger in the current aisle
		char aisleLetter; // holds the assigned seat number of the passenger in the current aisle
		stateEnum state;
		Row(): leftSeats(3), rightSeats(3), leftWait(2), rightWait(2), aisleNum(0), state(none) {}

};

/* 
function that creates the airplane that will hold the rows
*/
void createPlane(Queue <Row> &airplane) {
	
	// creating a queue of rows 
	//Row row; // initializes an instance of a row of class Row 

	for (int i = 48; i > 0; i--) {
		Row row;
		row.num = i; // assigns the row number to the row
		row.state = none; // initialize the row state to be empty
		airplane.enqueue(row); //enqueues the specific row into airplane
	}
}

int passIntoPlane(Queue <Row> &airplane, ifstream &infile) {
	Queue <Row> temp(48); // temporarily holds the data from airplane to iterate through 

	// characteristics of the people walking into the airplane 
	int number; // the row number 
	char seatLetter; // the seat position 
	Row currentRow; // for the current row to be compared to
	Row prevRow; // for the previous row to compare to the current row 
	int count = 0;
	int time = 0; // initial time before loading the passengers 
	int seated = 0; //initial number of passengers seated 
	// runs until all 288 passengers are seated 
	while (seated < 288) { 		// iterates through all 48 rows, from 48 to 1. 

		// there is no previous row behind 48. 
		prevRow.num = 0; 
		prevRow.state = none;
		for (int i = 48; i > 0; i--) {

			currentRow = airplane.dequeue();

			switch (currentRow.state) {	
			
				case none :
					// if current row is the row number assigned 
					if (currentRow.num == currentRow.aisleNum) {

						currentRow.state = storL1;
					} else { //if current row is not the row number assigned 
						// if the next row is empty 

						if (prevRow.aisleNum == 0) {
							// move the passenger to the previous (forward) row
							prevRow.aisleNum = currentRow.aisleNum;
							prevRow.aisleLetter = currentRow.aisleLetter;
							currentRow.aisleNum = 0; 
						// if next row is not empty 
						} 
					
					}
					break;

				case storL1 :
					// after passenger stores the first luggage 
					currentRow.state = storL2;
					break;

				case storL2 : //after passenger stores the 2nd luggage 
					// check if left seats are empty 
					if (currentRow.aisleLetter == 'A' || currentRow.aisleLetter == 'B' || currentRow.aisleLetter == 'C') {
						if (currentRow.leftSeats.isEmpty() == false && currentRow.leftSeats.top() > currentRow.aisleLetter) {
							// runs if there is a person in your way to the seat
							// pushes 

							currentRow.leftWait.push(currentRow.leftSeats.topAndPop());

							currentRow.state = ACout; 


						} else { 
							
							// you can sit down! the row you were occupying is now empty 
							
							// you sit down if no one is in your way
							currentRow.leftSeats.push(currentRow.aisleLetter); 
							currentRow.state = none;
							//currentRow.aisleNum = 0; // no passenger in the aisle, so we set the assigned number to be 0
							//seated++;
						}

					}
					// check if right seats are empty 
					else if (currentRow.aisleLetter == 'D' || currentRow.aisleLetter == 'E' || currentRow.aisleLetter == 'F') {
						if (currentRow.rightSeats.isEmpty() == false && currentRow.rightSeats.top() < currentRow.aisleLetter) {
							// runs if there is a person in your way to the seat
							// pushes 

							currentRow.rightWait.push(currentRow.rightSeats.topAndPop());
							currentRow.state = DFout;

						
						} else { 
							 // you can sit down! the row you were occupying is now empty
							
							// you sit down if no one is in your way 
							currentRow.rightSeats.push(currentRow.aisleLetter);
							//currentRow.aisleNum = 0;
							currentRow.state = none;

							//seated++;

						}
					}
					break;

				case ACout :
					if (currentRow.leftSeats.isEmpty() == false && currentRow.leftSeats.top() > currentRow.aisleLetter) { // a person is in the way
						// place into waiting stack 
						currentRow.leftWait.push(currentRow.leftSeats.topAndPop());
						currentRow.state = ACout;
					}
					else { // no more people are in the way
						// you sit down if no one is in your way 
						currentRow.leftSeats.push(currentRow.aisleLetter);
						//currentRow.aisleNum = 0;
						//seated++;
						if (currentRow.leftWait.isEmpty() == false) {
							currentRow.state = ACin;
						} else {
							currentRow.state = none;
						}
					}
					break;

				case DFout :
					if (currentRow.rightSeats.isEmpty() == false && currentRow.rightSeats.top() < currentRow.aisleLetter) { // a person is in the way
						// place into waiting stack
						currentRow.rightWait.push(currentRow.rightSeats.topAndPop());
						currentRow.state = DFout;
					} else { // no more people are in the way
						// you sit down if no one is in your way 
						currentRow.rightSeats.push(currentRow.aisleLetter);
						//currentRow.aisleNum = 0;
						//seated++;

						if (currentRow.rightWait.isEmpty() == false) {
							currentRow.state = DFin;
						} else {
						currentRow.state = none;
						}
					}
					break;

				case ACin :
					// there is still someone waiting in the aisle
					//if (currentRow.leftWait.isEmpty() == false) {
					currentRow.leftSeats.push(currentRow.leftWait.topAndPop());
					//	currentRow.state = ACin; 
					// no one is left in the aisle
					//} else if (currentRow.leftWait.isEmpty() == true) {
					if (currentRow.leftWait.isEmpty() == true) {
						currentRow.state = none; 
					}
					//}
					break;

				case DFin :
					// there is still someone waiting in the aisle
					//if (currentRow.rightWait.isEmpty() == false ) {
					currentRow.rightSeats.push( currentRow.rightWait.topAndPop());
					//	currentRow.state = DFin;
					// no one is left in the aisle
					//} else if (currentRow.rightWait.isEmpty() == true ) {
					if ( currentRow.rightWait.isEmpty() == true) {
						currentRow.state = none;
					}
					
					break;

				/*case wait :
					if (prevRow.aisleNum == 0) {
						// moves the passenger along to the previous(next) row
						prevRow.aisleNum = currentRow.aisleNum;
						prevRow.aisleLetter = currentRow.aisleLetter; 
						currentRow.aisleNum = 0;
						cout << "wait1" << endl; 
						// no one is in the current row 
						currentRow.state = none;
					} else {
						//cout << "wait2" << endl; 
						// passenger has to wait again 
						currentRow.state = wait; 
						cout << "wait2" << endl;
					}
					break; */
			}

			if (currentRow.state == none && currentRow.aisleNum == currentRow.num) {
				currentRow.aisleNum = 0;
				seated++;
			}

			// pushes into temp only if the previous row is not before 48, or (0)
			if (prevRow.num != 0) {
				temp.enqueue(prevRow); // push previous row into temp 
			}
			prevRow = currentRow; // set previous row to current row
			
 
		} 
		// if true, a passenger is allowed to walk into row 1 (airplane)
		if (count < 288 && prevRow.aisleNum == 0){
			// taking two values, one for number and the other for seatLetter
			infile >> number >> seatLetter; 
			prevRow.aisleNum = number;
			prevRow.aisleLetter = seatLetter;
			count++;
			

			}
		
		// enqueue the first row back into temp 
		temp.enqueue(prevRow);
		// now that temp is full, make airplane full again and temp empty
		for (int  i = 48; i > 0; i--) {
			Row tempDeq;
			tempDeq = temp.dequeue();
			airplane.enqueue(tempDeq);
		}
		time += 5; //incrementing time by 5  
	}

	return time;	
}

int main(int argc, char** argv) {
    int time = 0;
    
    ifstream infile(argv[1]);
   

    Queue <Row> airplane(48);
    createPlane(airplane);
    time = passIntoPlane(airplane, infile);
    cout << "Back to front: " << time << endl;
    time = 0;

    Queue <Row> airplane2(48);
    createPlane(airplane2);
    infile.ignore(500, '\n');
    time = passIntoPlane(airplane2, infile);
    cout << "Random: " << time << endl;
    time = 0;

    Queue <Row> airplane3(48);
    createPlane(airplane3);
    infile.ignore(500, '\n');
    time = passIntoPlane(airplane3, infile);
    cout << "Outside in: " << time << endl;
    // time = 0;
    return 0;
}
	/*int time;
	 // initializes a queue that takes type Row and call it airplane

	ifstream infile1;
	 1st line
	Queue<Row> airplane1(48);
	createPlane(airplane1); // creates an empty airplane and stores it into airplane 
	time = passIntoPlane(airplane1, infile1); // passengers in file are being loaded into the airplane
	cout << "Back to front: " << time << endl;
	time = 0;

	Queue<Row> airplane2(48);
	createPlane(airplane2); // creates an empty airplane and stores it into airplane 
	infile1.ignore(500, '\n');
	time = passIntoPlane(airplane2, infile1); // passengers in file are being loaded into the airplane
	cout << "Random: " << time << endl;
	time = 0;


	Queue<Row> airplane3(48);
	createPlane(airplane3); // creates an empty airplane and stores it into airplane 
	infile1.ignore(500, '\n');
	infile1.ignore(500, '\n');

	time = passIntoPlane(airplane3, infile1); // passengers in file are being loaded into the airplane
	cout << "Outside in: " << time << endl;
*/

	/*// 2nd line
	Queue<Row> airplane2(48);
	createPlane(airplane2); // creates an empty airplane and stores it into airplane 
	ifstream infile2;

	infile2.ignore(500, '\n');
	time = passIntoPlane(airplane2, infile2); // passengers in file are being loaded into the airplane
	cout << "Random: " << time << endl;

	// 3rd line
	Queue<Row> airplane3(48);
	createPlane(airplane3); // creates an empty airplane and stores it into airplane 
	ifstream infile3;

	infile3.ignore(500, '\n');
	infile3.ignore(500, '\n');
	time = passIntoPlane(airplane3, infile3); // passengers in file are being loaded into the airplane
	cout << "Outside in: " << time << endl;*/






