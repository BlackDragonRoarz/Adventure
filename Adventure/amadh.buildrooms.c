//INCLUDE STATEMENTS
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <stdio.h>
	#include <string.h>
		#include <fcntl.h>
			#include <stdlib.h>
		#include <unistd.h>
	#include <sys/stat.h>
#include <sys/types.h>
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/



//DATA STRUCTS
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct RoomNames
{
	char* roomArray[30];
};

struct Bag
{
	int bag[100];
	int capacity;
	int size;
};

struct ConnectionPair
{
	int room1;
	int room2;
	int pairCode;
};

struct Room
{
	char* name;
	int randomizedIndex;
	char* type;
	int totalNumberOfConnections;
	struct Bag actualConnectionBag;
	struct Bag* actualBagPtr;
	int outgoingConnections[];
};

struct garbageBin
{
	int* randomBag;
	struct Room* roomStructArray;
};
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/



//FUNCTION PROTOTYPES
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void createRoomsDirectory(int);
void createRoomFiles(int);
struct RoomNames initRoomNames(struct RoomNames roomNames);
int * select7(struct RoomNames);
struct Room * createRoomStructs(int* randomBag, struct RoomNames);
void insertNamesIntoRoomFiles(int pId, struct Room* roomStructArray);
void addTypesToRoomStructs(struct Room* roomStructArray);
void insertTypesIntoRoomFiles(int pId, struct Room* roomStructArray);
int initTotalConnections(struct Room* roomStructArray); 
void addConnectionsToRoomStructs(struct Room* roomStructArray, struct RoomNames);
void insertConnectionsIntoRoomFiles(int pId, struct Room* roomStructArray);
struct ConnectionPair * initConnectionPairs();
void loadPairCodesIntoBag(struct ConnectionPair*, struct Bag*);
void loadPairsIntoRoomConnectionBags(struct Bag*, struct ConnectionPair*, struct Room*, int);
int validateConnectionPairOnRooms(struct ConnectionPair* pairStruct, struct Room* roomArray); 




//Helper Functions
int addToBag(int num, struct Bag* bagPtr);
int inBag(int num, struct Bag* bagPtr);
struct Bag* removeFromBag(int num, struct Bag* bagPtr);
void printBag(struct Bag* bagPtr);
void swap(struct Bag* bagPtr, int index1, int index2);
struct Bag * shuffleBag(struct Bag* bagPtr);
int arrayEmpty(int* numberArray);
int removeValFromBag(int* numberArray, int number);
void insertValueIntoBag(int* bag, int number, int sizeOfBag);
int bagFull(struct Bag* bagPtr);

int bagEmpty(struct Bag* bagPtr);
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
//we need to take the shuffled values out of the possible connections and insert them into
//the real connection bag one by one, until we have reached the totalNumberOfConnections limit


//MAIN FUNCTION
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int main()
{

		//seed the random generater
		time_t t;
		srand((unsigned) time(&t));	
		const int pId = getpid();	
		createRoomsDirectory(pId);
		createRoomFiles(pId);
		
		struct RoomNames roomNames;
		roomNames = initRoomNames(roomNames);	
		int* randomBag = select7(roomNames);


		struct Room* roomArray = createRoomStructs(randomBag, roomNames);
		
		insertNamesIntoRoomFiles(pId, roomArray);

		addTypesToRoomStructs(roomArray);
		
		
		//set all of the room struct's totalNumberOfConnections to their respective numbers
		int totalNumOfConnections = initTotalConnections(roomArray);
		
		//load the connectionPairArray with all 21 initialized connection pairing structs
		struct ConnectionPair* connectionPairArray = initConnectionPairs();
		
		//create a bag to hold all of the connection pair identifier codes
		struct Bag pairCodeBag;
		struct Bag* pairCodeBagPtr = &pairCodeBag;
		
		//load the identifier bag with all of the connection pair codes
		loadPairCodesIntoBag(connectionPairArray, pairCodeBagPtr);
		pairCodeBagPtr->size = 21;
		pairCodeBagPtr->capacity = 21;
				
		//Shuffle the pairCodeBag
		pairCodeBagPtr = shuffleBag(pairCodeBagPtr);
		
		//Load the pairs into the room structure outgoing connection arrays using the pairCodeBag
		//as a reference to the pairs themselves and then load those pairs into the outgoing
		// connection arrays if those arrays are not full. Then remove that pairCode from the pair
		// Code bag if successful insertion. If not a successful insertion then simply remove the 
		// pair code from the pairCodeBag anyways
		loadPairsIntoRoomConnectionBags(pairCodeBagPtr, connectionPairArray, roomArray, totalNumOfConnections); 
		insertConnectionsIntoRoomFiles(pId, roomArray);	
		insertTypesIntoRoomFiles(pId, roomArray);

		return 0;
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void loadPairCodesIntoBag(struct ConnectionPair* pairArray, struct Bag* pairCodeBag)
{
	int i = 0;
	for(i=0; i<21; i++)
	{
		pairCodeBag->bag[i] = i;		


	}	

return;

}
void loadPairsIntoRoomConnectionBags(struct Bag* pairCodeBag, struct ConnectionPair* pairArray, struct Room* roomArray, int totalNumOfConnections)
{
	
//This outer loop is what controlls how many double links we need to make
int outerLoopController = totalNumOfConnections/2; 
int i = 0;

for(i=0; i<outerLoopController; i++)
{
	int valid = 0;	
	//This while loop gets the next available connection pairing.
	//It verifies it's validity, then it inserts it into our room connection map structure
	while(!valid)
	{
		if(bagEmpty(pairCodeBag))
		{
			break;
		}	
		//#1retreive a random connectionPair from the pairArray (which we will get using the next available pairCode)
		int sizeOfpairCodeBag = pairCodeBag->size;
		int randomPairCodeIndex = rand() % (sizeOfpairCodeBag + 1 - 0 )+ 0;
		int randomPairCodeValue = pairCodeBag->bag[randomPairCodeIndex];
		struct ConnectionPair* pairStruct = &pairArray[randomPairCodeValue];	
		valid = validateConnectionPairOnRooms(pairStruct, roomArray);
		if(valid)
		{
			//insert the room#'s of the pairStruct into their corresponding roomArray[].actualConnectionBag.bag[]
			int room1Number = pairStruct->room1;	
			int room2Number = pairStruct->room2;
						
	
			addToBag(room2Number, roomArray[room1Number].actualBagPtr);
			addToBag(room1Number, roomArray[room2Number].actualBagPtr);
					
	
		//Now delete the reference to the pairStruct from the pairCodeBag by passing it to removeFromBag(bagPtr)

			pairCodeBag = removeFromBag(randomPairCodeValue, pairCodeBag);
									
		}else
		{
			pairCodeBag = removeFromBag(randomPairCodeValue, pairCodeBag);
		}
		

	}
}


return;

}

int validateConnectionPairOnRooms(struct ConnectionPair* pairStruct, struct Room* roomArray) 
{
	//once we have the connectionPair, we must now make sure that it is possible to connect the two rooms represented by the connectionPair element
	//first we must make sure that the two rooms represented by the connectionPair have the requisite space avaialable for another connection
		//If either one of them are full, then we must call removeBagElement() on the pairCodeBag at the index of the invalid pairing
		//if they both have space, then we are good to go, and we should connect those two rooms and delete the pairCode from the pairCodeBag
	int valid;
	int room1 = pairStruct->room1;
	int room2 = pairStruct->room2;

	if(bagFull(roomArray[room1].actualBagPtr) || bagFull(roomArray[room2].actualBagPtr))
	{
		valid = 0;
	}else
	{
		valid = 1;

	}

//We also have to check to see if the actualConnectionBag's would end up having any duplicates as a result
	if(inBag(room2, roomArray[room1].actualBagPtr) || inBag(room1, roomArray[room2].actualBagPtr))
	{
		valid  = 0;
	}

	

	return valid;
}

struct ConnectionPair * initConnectionPairs()
{
	struct ConnectionPair* ConnectionPairStructArray = malloc(21 * sizeof(struct ConnectionPair));
		
			struct ConnectionPair connPair0;
			connPair0.room1 = 0;
			connPair0.room2 = 1;
			connPair0.pairCode = 0;
			ConnectionPairStructArray[0] = connPair0;
	
			struct ConnectionPair connPair1;
			connPair1.room1 = 0;
			connPair1.room2 = 2;
			connPair1.pairCode = 1;
			ConnectionPairStructArray[1] = connPair1;
		
			struct ConnectionPair connPair2;
			connPair2.room1 = 0;
			connPair2.room2 = 3;
			connPair2.pairCode = 2;
			ConnectionPairStructArray[2] = connPair2;

			struct ConnectionPair connPair3;
			connPair3.room1 = 0;
			connPair3.room2 = 4;
			connPair3.pairCode = 3;
			ConnectionPairStructArray[3] = connPair3;

			struct ConnectionPair connPair4;
			connPair4.room1 = 0;
			connPair4.room2 = 5;
			connPair4.pairCode = 4;
			ConnectionPairStructArray[4] = connPair4;


			struct ConnectionPair connPair5;
			connPair5.room1 = 0;
			connPair5.room2 = 6;
			connPair5.pairCode = 5;
			ConnectionPairStructArray[5] = connPair5;


			struct ConnectionPair connPair6;
			connPair6.room1 = 1;
			connPair6.room2 = 2;
			connPair6.pairCode = 6;
			ConnectionPairStructArray[6] = connPair6;

			struct ConnectionPair connPair7;
			connPair7.room1 = 1;
			connPair7.room2 = 3;
			connPair7.pairCode = 7;
			ConnectionPairStructArray[7] = connPair7;

			struct ConnectionPair connPair8;
			connPair8.room1 = 1;
			connPair8.room2 = 4;
			connPair8.pairCode = 8;
			ConnectionPairStructArray[8] = connPair8;

			struct ConnectionPair connPair9;
			connPair9.room1 = 1;
			connPair9.room2 = 5;
			connPair9.pairCode = 9;
			ConnectionPairStructArray[9] = connPair9;
			
			struct ConnectionPair connPair10;
			connPair10.room1 = 1;
			connPair10.room2 = 6;
			connPair10.pairCode = 10;
			ConnectionPairStructArray[10] = connPair10;

			struct ConnectionPair connPair11;
			connPair11.room1 = 2;
			connPair11.room2 = 3;
			connPair11.pairCode = 11;
			ConnectionPairStructArray[11] = connPair11;


			struct ConnectionPair connPair12;
			connPair12.room1 = 2;
			connPair12.room2 = 4;
			connPair12.pairCode = 12;
			ConnectionPairStructArray[12] = connPair12;

			struct ConnectionPair connPair13;
			connPair13.room1 = 2;
			connPair13.room2 = 5;
			connPair13.pairCode = 13;
			ConnectionPairStructArray[13] = connPair13;

			struct ConnectionPair connPair14;
			connPair14.room1 = 2;
			connPair14.room2 = 6;
			connPair14.pairCode = 14;
			ConnectionPairStructArray[14] = connPair14;

			struct ConnectionPair connPair15;
			connPair15.room1 = 3;
			connPair15.room2 = 4;
			connPair15.pairCode = 15;
			ConnectionPairStructArray[15] = connPair15;

			struct ConnectionPair connPair16;
			connPair16.room1 = 3;
			connPair16.room2 = 5;
			connPair16.pairCode = 16;
			ConnectionPairStructArray[16] = connPair16;

			struct ConnectionPair connPair17;
			connPair17.room1 = 3;
			connPair17.room2 = 6;
			connPair17.pairCode = 17;
			ConnectionPairStructArray[17] = connPair17;

			struct ConnectionPair connPair18;
			connPair18.room1 = 4;
			connPair18.room2 = 5;
			connPair18.pairCode = 18;
			ConnectionPairStructArray[18] = connPair18;

			struct ConnectionPair connPair19;
			connPair19.room1 = 4;
			connPair19.room2 = 6;
			connPair19.pairCode = 19;
			ConnectionPairStructArray[19] = connPair19;
			
			struct ConnectionPair connPair20;
			connPair20.room1 = 5;
			connPair20.room2 = 6;
			connPair20.pairCode = 20;
			ConnectionPairStructArray[20] = connPair20;


	return ConnectionPairStructArray;


}
int initTotalConnections(struct Room* roomStructArray)
{
	
	int i = 0;
	int sum =0;
	int num0, num1, num2, num3, num4, num5, num6;
	do{
	sum = 0;
	num0 = rand() % (6 + 1 - 3) +3;
	num1 = rand() % (6 + 1 - 3) +3;
	num2 = rand() % (6 + 1 - 3) +3;
	num3 = rand() % (6 + 1 - 3) +3;
	num4 = rand() % (6 + 1 - 3) +3;
	num5 = rand() % (6 + 1 - 3) +3;
	num6 = rand() % (6 + 1 - 3) +3;
	sum = num0+num1+num2+num3+num4+num5+num6;
	}while(sum%2 != 0);

		roomStructArray[0].totalNumberOfConnections = num0;
		roomStructArray[0].actualConnectionBag.size = 0;
		roomStructArray[0].actualConnectionBag.capacity = num0;
		roomStructArray[0].actualBagPtr = &roomStructArray[0].actualConnectionBag;
	
		roomStructArray[1].totalNumberOfConnections = num1;
		roomStructArray[1].actualConnectionBag.size = 0;
		roomStructArray[1].actualConnectionBag.capacity = num1;
		roomStructArray[1].actualBagPtr = &roomStructArray[1].actualConnectionBag;
	
		roomStructArray[2].totalNumberOfConnections = num2;
		roomStructArray[2].actualConnectionBag.size = 0;
		roomStructArray[2].actualConnectionBag.capacity = num2;
		roomStructArray[2].actualBagPtr = &roomStructArray[2].actualConnectionBag;
	
		roomStructArray[3].totalNumberOfConnections = num3;
		roomStructArray[3].actualConnectionBag.size = 0;
		roomStructArray[3].actualConnectionBag.capacity = num3;
		roomStructArray[3].actualBagPtr = &roomStructArray[3].actualConnectionBag;
	
		roomStructArray[4].totalNumberOfConnections = num4;
		roomStructArray[4].actualConnectionBag.size = 0;
		roomStructArray[4].actualConnectionBag.capacity = num4;
		roomStructArray[4].actualBagPtr = &roomStructArray[4].actualConnectionBag;
	
		roomStructArray[5].totalNumberOfConnections = num5;
		roomStructArray[5].actualConnectionBag.size = 0;
		roomStructArray[5].actualConnectionBag.capacity = num5;
		roomStructArray[5].actualBagPtr = &roomStructArray[5].actualConnectionBag;
	
		roomStructArray[6].totalNumberOfConnections = num6;
		roomStructArray[6].actualConnectionBag.size = 0;
		roomStructArray[6].actualConnectionBag.capacity = num6;
		roomStructArray[6].actualBagPtr = &roomStructArray[6].actualConnectionBag;
	




	
	return sum;
}





//FUNCTIONS
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void addTypesToRoomStructs(struct Room* roomStructArray)
{
	roomStructArray[0].type = "START_ROOM";
	roomStructArray[1].type = "MID_ROOM";
	roomStructArray[2].type = "MID_ROOM";
	roomStructArray[3].type = "MID_ROOM";
	roomStructArray[4].type = "MID_ROOM";
	roomStructArray[5].type = "MID_ROOM";
	roomStructArray[6].type = "END_ROOM";
	return;
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void insertTypesIntoRoomFiles(int pId, struct Room* roomStructArray)
{
	ssize_t nread, nwritten;

int i = 0;
for(i=0; i<7; i++)
{	
	int roomFile;
	char roomPath[100];
	memset(roomPath, '\0', 100);
	sprintf(roomPath, "./amadh.rooms.%d/room%d", pId, i);
	roomFile = open(roomPath, O_RDWR | O_CREAT | O_APPEND, 0600);
	nwritten = write(roomFile, "ROOM TYPE: ", 11);
	roomFile = open(roomPath, O_RDWR | O_CREAT | O_APPEND, 0600);
	nwritten = write(roomFile, roomStructArray[i].type, strlen(roomStructArray[i].type) * sizeof(char));
	roomFile = open(roomPath, O_RDWR | O_CREAT | O_APPEND, 0600);
	nwritten = write(roomFile, "\n", 1);	
}

	return;
}
	/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/







/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
struct Bag * shuffleBag(struct Bag* bagPtr)
{
	int max = 100;
	int min = 50;
	int loopCounter = rand() % (max + 1 - min) + min;
	int i = 0;
	for(i = 0; i < loopCounter; i++)
	{
//		printf("loop\n");
		int index1 = rand() % bagPtr->size;
		int index2 = rand() % bagPtr->size;
		swap(bagPtr, index1, index2);
	}

	return bagPtr;
}

int bagEmpty(struct Bag* bagPtr)
{
	if(bagPtr->size == 0)
	{
		return 1;
	}
	return 0;

}

void swap(struct Bag* bagPtr, int index1, int index2)
{
//	printf("swap1\n");
	int val1 = bagPtr->bag[index1];
//	printf("swap2\n");
	int val2 = bagPtr->bag[index2];
//	printf("swap3\n");
	bagPtr->bag[index1] = val2;
//	printf("swap4\n");
	bagPtr->bag[index2] = val1;
	
	return;
}

int addToBag(int num, struct Bag* bagPtr) 
{
	if(inBag(num, bagPtr))
	{
		return 0;
	}else
	{
		int bagSize = bagPtr->size;
		bagPtr->bag[bagSize] = num;
		bagPtr->size++;
	}
		

	return 1;
}
int bagFull(struct Bag* bagPtr)
{
	if(bagPtr->size == bagPtr->capacity)
	{
		return 1;
	}
	return 0;
}
int inBag(int num, struct Bag* bagPtr)
{
	int bagSize = bagPtr->size;
	int i = -5;
	int inBag = 0;
	for(i=0; i<bagSize; i++)
	{
		if(bagPtr->bag[i] == num)
		{
			inBag = 1;
		}	
	} 
	
	return inBag;
}

struct Bag * removeFromBag(int num, struct Bag* bagPtr)
{
	int itemInBag = inBag(num, bagPtr);	
	struct Bag* newBagPtr = (struct Bag*) malloc(sizeof(struct Bag));	
	newBagPtr->size = 0;
	newBagPtr->capacity = bagPtr->capacity;
	if(itemInBag)
	{
		int i = 0;
		//loop over bagPtr->bag[i]
		for(i=0; i < bagPtr->size; i++)
		{
			if(bagPtr->bag[i] != num)
			{
				addToBag(bagPtr->bag[i], newBagPtr);
			}
		}
		//no need to call free because you did not call malloc. System takes care of freeing up memory when you reassign it.
		bagPtr = newBagPtr;
	}
	return bagPtr;


}

void printBag(struct Bag* bagPtr)
{
	int i=0;
	for(i=0; i<bagPtr->size; i++)
	{
		printf("Bag Element # %d: %d\n", i +1, bagPtr->bag[i]);
	}
	return;
}






/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void insertConnectionsIntoRoomFiles(int pId, struct Room* roomStructArray)
{
	ssize_t nread, nwritten;
int i = 0;
for(i=0; i<7; i++)
{
	int roomFile;
	char roomPath[100];
	char roomConnection[100];
	memset(roomPath, '\0', 100);
	memset(roomConnection, '\0', 100);
	sprintf(roomPath, "./amadh.rooms.%d/room%d", pId, i);
	int j=0;
	int total = roomStructArray[i].actualConnectionBag.size;
	for(j=0; j<total; j++)
	{
		int roomNumber = roomStructArray[i].actualConnectionBag.bag[j];
		roomFile = open(roomPath, O_RDWR | O_CREAT | O_APPEND, 0600);
		sprintf(roomConnection, "CONNECTION%d: %s\n", j+1, roomStructArray[roomNumber].name);
		nwritten = write(roomFile, roomConnection, strlen(roomConnection));
	}
}
	return;
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct Room * createRoomStructs(int* randomBag, struct RoomNames roomNames)
{
		//create Room Group
		//
	
	struct Room* roomStructArray = malloc(7 * sizeof(struct Room));	
	int i=0;
	for(i=0; i<7; i++)
	{		
		struct Room room;
;
		room.name = roomNames.roomArray[randomBag[i]];
		room.randomizedIndex = i;
		roomStructArray[i] = room;

	}

	return roomStructArray;
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void insertNamesIntoRoomFiles(int pId, struct Room* roomStructArray)
{

	ssize_t nread, nwritten;
	int roomFile;

int i = 0;
for(i = 0; i < 7; i++)
{
	char roomPath[100];
	memset(roomPath, '\0', 100);
	sprintf(roomPath, "./amadh.rooms.%d/room%d", pId, i);
	roomFile = open(roomPath, O_RDWR | O_CREAT | O_TRUNC, 0600);
	nwritten = write(roomFile, "ROOM NAME: ", 11);
	roomFile = open(roomPath, O_RDWR | O_CREAT | O_APPEND, 0600);
	nwritten = write(roomFile, roomStructArray[i].name, strlen(roomStructArray[i].name));
	roomFile = open(roomPath, O_RDWR | O_CREAT | O_APPEND, 0600);
	nwritten = write(roomFile, "\n", 1);	
	

}

return;
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//This function takes in a struct filled with room names as strings
	//The function should take those names and pick seven of them randomly
		//Next the function should output those names to the room files in a 
	//specified format:
//ROOM NAME: someName
int * select7(struct RoomNames roomNames)
{
	int orderedArray[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		int* bag = (int*) malloc(10 * sizeof(int));
			int sizeOfBag = 0;

	//this loop will choose random numbers from 1-10 and then remove the number chosen from the array
		//when the array is empty then loop exits
			while(!arrayEmpty(orderedArray)) //returns true if the value existed; false if it didnt
			{
				//generate a number from 1 to 10
					int randomNumber = rand() % 10;		
						//if there is a successful removal then insert that number into the chosen array 
						if(removeValFromBag(orderedArray, randomNumber))
						{						
							insertValueIntoBag(bag, randomNumber, sizeOfBag);
							sizeOfBag = sizeOfBag + 1;
						}
			}

		return bag;
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void insertValueIntoBag(int* bag, int number, int sizeOfBag)
{
	bag[sizeOfBag] = number;


	return;		
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int arrayEmpty(int* numberArray)
{
	int i = -5;
		int empty = 1;
			for(i = 0; i<10; i++)
			{
				if(numberArray[i] != -1 )
				{
					empty = 0;
				}	
			}	
	return empty;
	
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//This function takes an array and a number and first checks if that number exists in the bag; 
	//if it does exist in the bag, then it removes the value and returns true
//if the value does not exist in the bag then the function returns false to indicate no removal 
int removeValFromBag(int* numberArray, int number)
{
	//if the number does not exist in the bag then return false	
		if(numberArray[number] == -1)
		{	//element no longer exists in bag; return false to indicate unsuccessful removal
				return 0;
		}
		else
		{
			//element still exists in bag; remove element and return true to indicate successful removal
				numberArray[number] = -1;
			return 1;	
		}


	return -1;
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void createRoomFiles(int processId)
{
	char room1[100];
	char room2[100];
	char room3[100];
	char room4[100];
	char room5[100];
	char room6[100];
	char room7[100];
	
	//clear buffers
	memset(room1, '\0', 100);
	memset(room2, '\0', 100);
	memset(room3, '\0', 100);
	memset(room4, '\0', 100);
	memset(room5, '\0', 100);
	memset(room6, '\0', 100);
	memset(room7, '\0', 100);

	//Create the room names
	sprintf(room1, "./amadh.rooms.%d/room0", processId);
	sprintf(room2, "./amadh.rooms.%d/room1", processId);
	sprintf(room3, "./amadh.rooms.%d/room2", processId);
	sprintf(room4, "./amadh.rooms.%d/room3", processId);
	sprintf(room5, "./amadh.rooms.%d/room4", processId);
	sprintf(room6, "./amadh.rooms.%d/room5", processId);
	sprintf(room7, "./amadh.rooms.%d/room6", processId);

	//Create the rooms	
	int fd1, fd2, fd3, fd4, fd5, fd6, fd7;
	fd1 = open(room1, O_RDWR | O_CREAT | O_TRUNC, 0600);
	fd2 = open(room2, O_RDWR | O_CREAT | O_TRUNC, 0600);
	fd3 = open(room3, O_RDWR | O_CREAT | O_TRUNC, 0600);
	fd4 = open(room4, O_RDWR | O_CREAT | O_TRUNC, 0600);
	fd5 = open(room5, O_RDWR | O_CREAT | O_TRUNC, 0600);
	fd6 = open(room6, O_RDWR | O_CREAT | O_TRUNC, 0600);
	fd7 = open(room7, O_RDWR | O_CREAT | O_TRUNC, 0600);

	return;
}
struct RoomNames initRoomNames(struct RoomNames roomNames)
{	
	roomNames.roomArray[0] = "ROOM0";
	roomNames.roomArray[1] = "ROOM1";
	roomNames.roomArray[2] = "ROOM2";
	roomNames.roomArray[3] = "ROOM3";	
	roomNames.roomArray[4] = "ROOM4";
	roomNames.roomArray[5] = "ROOM5";
	roomNames.roomArray[6] = "ROOM6";
	roomNames.roomArray[7] = "ROOM7";
	roomNames.roomArray[8] = "ROOM8";
	roomNames.roomArray[9] = "ROOM9";

	return roomNames;	
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

void createRoomsDirectory(int pId)
{
	char dirName[100];
	memset(dirName, '\0', 100);
	sprintf(dirName, "./amadh.rooms.");
	char file[100];
	memset(file, '\0', 100);
	sprintf(file, "%s%d", dirName, pId);	
	mkdir(file, 0755);
	return;
}
