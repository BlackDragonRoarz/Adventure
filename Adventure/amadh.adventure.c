#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>

pthread_t threadId;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

//This function is called by a child thread 
//and uses mutexes to create a file with the updated server time
//it truncates the previous time. It then gets reported back to the user 
void* getTime(void *argument)
{
	int mutexRt;

	pthread_t pThreadID = pthread_self();
	
	if(pthread_equal(pThreadID, threadId))
	{	pthread_mutex_lock(&mutex1);
			char buff[500];
			memset(buff, '\0', 500);
			time_t timeOfDay;
			timeOfDay = time(NULL);
			sprintf(buff, ctime(&timeOfDay));
			int fd;
			char* newFile = "./currentTime.txt";
			ssize_t nwritten;
			fd = open(newFile, O_RDWR | O_CREAT | O_TRUNC, 0600);
			nwritten = write(fd, buff, strlen(buff)* sizeof(char));
			ssize_t nread;
			fd = open(newFile, O_RDWR, 0600);
			nread = read(fd, buff, strlen(buff)* sizeof(char));
			printf("\n%s\n", buff);


		pthread_mutex_unlock(&mutex1);


	}

		return;

}


struct Room
{
	char* roomName;
	char* connection1;
	char* connection2;
	char* connection3;
	char* connection4;
	char* connection5;
	char* connection6;	

};

struct Directory
{
	char* dirName;

};


//returns the game directory for the rooms that the game will play with
char* getNewestRoomDirectory();

//returns the starting room struct that we will use to kick off the game


//This takes a roomName from either the user input or from the starting room struct and returns
//a connection struct back to main

char* displayLines(char* newestDirectory, int lineNumber, int roomNumber);

//This function will init all of the rooms with their respective names
void initRoom(char*, struct Room*, int);


//This takes a room struct and displays a line to the user screen containing the current room info
void displayCurrentRoom(struct Room* currentRoom);

//This takes a connection struct and displays the connection info to the user screen
void displayConnections(struct Room* currentRoom);

//This simply displays the "WHERE TO? >" prompt to the user
void displayPrompt();

//This displays an error message to the user after they type in the wrong input
void displayError();

//This function will recieve input from the user and will return the string to the main function
char* getInput(struct Room*);
//display a congradulations message
void congrats();
//Display the amount of steps that the user will use
void steps(int);
//display the history that the user took to win
void history(char**, int);
//returns true or false saying that the current room is connected to the room represented by the str
int isConnected(struct Room* currentRoom, char* str);
//This function will validate the user input 
int validateInput(char* input);

int main()
{	struct stat attr;
	struct stat* attrPtr = &attr;
	int stepcount = 0;
	char* histArray[1000];
	memset(histArray, '\0', 1000);
	system("clear");
	char con1[50];	char con2[50];	char con3[50];	char con4[50];	char con5[50];	char con6[50];	char con0[50];	
	char* con0p = con0;
	char* con1p = con1;
	char* con2p = con2;
	char* con3p = con3;
	char* con4p = con4;
	char* con5p = con5;
	char* con6p = con6;


	struct Room startRoom;
	struct Room room1;
	struct Room room2;
	struct Room room3;
	struct Room room4;
	struct Room room5;
	struct Room endRoom;
	
	struct Room* room0p = &startRoom;
	struct Room* room1p = &room1;
	struct Room* room2p = &room2;
	struct Room* room3p = &room3;
	struct Room* room4p = &room4;
	struct Room* room5p = &room5;
	struct Room* room6p = &endRoom;
	
	struct Directory gameDirectory;
	//This will point to the current room throughout the program
	struct Room* currentRoom = NULL;
	//this variable will trigger the end of the game when it is set to 1
	int end = 0;
	//initialize the rooms
	initRoom(con0p, room0p, 0);
	initRoom(con1p, room1p, 1);
	initRoom(con2p, room2p, 2);
	initRoom(con3p, room3p, 3);
	initRoom(con4p, room4p, 4);
	initRoom(con5p, room5p, 5);
	initRoom(con6p, room6p, 6);
	//initialized the game directory structure
	gameDirectory.dirName = getNewestRoomDirectory();
	
	//sets the current room to the randomly chosen first room name
	currentRoom = room0p;

	//GAME LOOP
	while(!end)
	{
		displayCurrentRoom(currentRoom);
		displayConnections(currentRoom);
		char* input = getInput(currentRoom);	
		//This code checks to see if the input is equal to one of the chosen game room's names
		//and then if it is, it checks to see if it is connected to that room. If both are
		//true then we make that room the connected room
		
		if((strcmp(input, "time"))==0)
		{
			int error;
			error = pthread_create(&threadId, NULL, &getTime, NULL);
			int *ptr;
			pthread_join(threadId, (void**)&(ptr));
			
		
			


		}
		else if((strcmp(input, startRoom.roomName) == 0) && isConnected(currentRoom, input))
		{currentRoom = room0p;
			printf("\n");
			histArray[stepcount] = startRoom.roomName;
			stepcount++;
		}
		else
		if((strcmp(input, room1.roomName) == 0) && isConnected(currentRoom, input))
		{currentRoom = room1p;
			
			printf("\n");
			histArray[stepcount] = room1.roomName;
			stepcount++;
		}
		else
		if((strcmp(input, room2.roomName) == 0) && isConnected(currentRoom, input))
		{currentRoom = room2p;
			
			printf("\n");
			histArray[stepcount] = room2.roomName;
			stepcount++;
		}
		else
		if((strcmp(input, room3.roomName) == 0) && isConnected(currentRoom, input))
		{currentRoom = room3p;
		
			printf("\n");
			histArray[stepcount] = room3.roomName;
			stepcount++;
		}
		else
		if((strcmp(input, room4.roomName) == 0) && isConnected(currentRoom, input))
		{currentRoom = room4p;
			printf("\n");
			histArray[stepcount] = room4.roomName;
			stepcount++;

		}
		else
		if((strcmp(input, room5.roomName) == 0) && isConnected(currentRoom, input))
		{currentRoom = room5p;
			printf("\n");
			histArray[stepcount] = room5.roomName;
			stepcount++;

		}
		else
		if((strcmp(input, endRoom.roomName) == 0) && isConnected(currentRoom, input))
		{currentRoom = room6p;
			printf("\n");
			histArray[stepcount] = endRoom.roomName;
			stepcount++;

		}
		else
		{
			displayError();
		}

		//game ends if this condition is met
		if (currentRoom == room6p)	
		{
			congrats();
			steps(stepcount);
			history(histArray, stepcount);
			printf("\n");
			end = 1;
		}	
	}

	


return 0;



}

void history(char** histArray, int steps)
{
//This prints the users path to victory using the histArray passed to the function
//which should carry all of the rooms visited.
	printf("YOUR PATH TO VICTORY WAS:\n");
	int i=0;
	for(i = 0; i< steps; i++)
	{
		printf("%s\n", histArray[i]);
	}

	return;
}


void steps(int num)
{
	printf("YOU TOOK %d STEPS. ", num);
return;
}


//This function will init the room structures by taking
//a pointer to an existing structure and initializing
//the existing room structure with data obtained through
//out the function
void initRoom(char* conp, struct Room* room, int roomNum)
{
	char* someRoom;
	conp = getNewestRoomDirectory();
	someRoom = displayLines(conp, 0, roomNum);
	room->roomName = someRoom;

	conp = getNewestRoomDirectory();
	someRoom = displayLines(conp, 1, roomNum);
	room->connection1 = someRoom;

	conp = getNewestRoomDirectory();
	someRoom = displayLines(conp, 2, roomNum);
	room->connection2 = someRoom;

	conp = getNewestRoomDirectory();
	someRoom = displayLines(conp, 3, roomNum);
	room->connection3 = someRoom;

	conp = getNewestRoomDirectory();
	someRoom = displayLines(conp, 4, roomNum);
	room->connection4 = someRoom;

	conp = getNewestRoomDirectory();
	someRoom = displayLines(conp, 5, roomNum);
	room->connection5 = someRoom;

	conp = getNewestRoomDirectory();
	someRoom = displayLines(conp, 6, roomNum);
	room->connection6 = someRoom;


return;
}

//This was a very odd function that i had to implement
//For whatever reason my program needed to call this multiple
//times (once for every room pointer). the function
char* getNewestRoomDirectory()
{
	struct stat statStruct;
	char* path;
	path = "amadh.rooms.";
	stat(path, &statStruct);
	
	struct stat fileInfo;
    	DIR *directoryPtr;
    	struct dirent *directoryStructPtr;
   	directoryPtr = opendir("./");
    	char* newestDir;
	
	if (directoryPtr)
    	{
			
		directoryStructPtr = readdir(directoryPtr); 
		time_t newestFileTime = 0; 
		while (directoryStructPtr != NULL )
		{
			int val = strncmp(path, directoryStructPtr->d_name, 11);
//			printf("%.11s\n", directoryStructPtr->d_name);
			if(val == 0) 
    	        	{	
				stat(directoryStructPtr->d_name, &fileInfo);
				time_t time = fileInfo.st_mtime;
				if(time > newestFileTime)
				{
					newestFileTime = time;
					newestDir = directoryStructPtr->d_name;
				}
//				printf("%s: %s\n", directoryStructPtr->d_name,  ctime(&fileInfo.st_mtime));
			}
			directoryStructPtr = readdir(directoryPtr); 
		}
    	}	
return newestDir;
}


//This function made me rip my hair out. It took for ever, and 
//if the slightest thing is changed, it will take forever to fix
//so dont manipulate with the particularites of it, because it is highly
//dependant on the rooms program
char* displayLines(char* newestDirectory, int lineNumber, int roomNumber)
{	int fileDescriptor;

	char* dirPath = newestDirectory;
	char* connectionName = NULL;	
	char* roomPath;
//I had to first establish which room file we were going to access		
	if(roomNumber == 0)
	{
		roomPath = "/room0\0";
	}else if(roomNumber == 1)
	{
		roomPath = "/room1\0";
	}else if(roomNumber == 2)
	{
		roomPath = "/room2\0";
	}else if(roomNumber == 3)
	{
		roomPath = "/room3\0";
	}else if(roomNumber == 4)
	{
		roomPath = "/room4\0";
	}else if(roomNumber == 5)
	{
		roomPath = "/room5\0";
	}else if(roomNumber == 6)
	{
		roomPath = "/room6\0";
	}
	roomPath = strcat(dirPath, roomPath);
	ssize_t nread;
	//the choice for this buffer was verry 
	//deliberate. If you mess with it, it 
	//will ruin the program
	char line[18];
	int fd = open(roomPath, O_RDWR, 0600);
	memset(line, '\0', sizeof(line));
	int i =0;
	
	for(i=0; i<lineNumber; i++)
	{
		lseek(fd, 18, SEEK_CUR);
	}	
	//These are some containers to strip out unwanted words from our string
	char garbage1[20], garbage2[20], garbage3[20];
	//Here we used string scanning to pluck out the correct string from 
	//the larger field
	if(lineNumber == 0)
	{

		connectionName = (char *) malloc(5 * sizeof(char));
	 	lseek(fd, 0, SEEK_CUR);	
		nread = read(fd, line, sizeof(line));
		sscanf(line, "%s %s %s", garbage1, garbage2, connectionName);
		
//		printf("%s", connectionName);
		lseek(fd, 0, SEEK_SET);

	}	
	if(lineNumber == 1)
	{	
		connectionName = (char *) malloc(5 * sizeof(char));
		lseek(fd, 0, SEEK_CUR);
		nread = read(fd, line, sizeof(line));
		sscanf(line, "%s %s", garbage1, connectionName);
	//	printf("%s", connectionName);
		
		lseek(fd, 0, SEEK_SET);
		
	}
	if(lineNumber == 2)
	{       
		connectionName = (char *) malloc(5 * sizeof(char));
		lseek(fd, 1, SEEK_CUR);
		
		nread = read(fd, line, sizeof(line));
		sscanf(line, "%s %s", garbage1, connectionName);
	//	printf("%s", connectionName);

		lseek(fd, 0, SEEK_SET);
	}		
	if(lineNumber == 3)
	{	
		lseek(fd, 2, SEEK_CUR);
		nread = read(fd, line, sizeof(line));
		char str1[4];
		strcpy(str1, "TYPE");
		if(strstr(line, str1))
		{
	//		char* connectionName = (char *) malloc(10 * sizeof(char));
	//		lseek(fd, 7, SEEK_CUR);
	//		sscanf(line, "%s %s %s", garbage1, garbage2, connectionName);	
		//	printf("%s", connectionName);
			connectionName = NULL;
		}else
		{	
			connectionName = (char *) malloc(5 * sizeof(char));
			sscanf(line, "%s %s", garbage1, connectionName);
	//		printf("%s", connectionName);


		}

		lseek(fd, 0, SEEK_SET);
	}if(lineNumber == 4)
	{
		
		lseek(fd, 3, SEEK_CUR);
		nread = read(fd, line, sizeof(line));
		char str1[4];
		strcpy(str1, "TYPE");
		if(strstr(line, str1))
		{
	//		char* connectionName = (char *) malloc(10 * sizeof(char));
	//		lseek(fd, 7, SEEK_CUR);
	//		sscanf(line, "%s %s %s", garbage1, garbage2, connectionName);	
		//	printf("%s", connectionName);
			connectionName = NULL;
		}else
		{	
			connectionName = (char *) malloc(5 * sizeof(char));
			sscanf(line, "%s %s", garbage1, connectionName);
	//		printf("%s", connectionName);


		}


		lseek(fd, 0, SEEK_SET);
	}if(lineNumber == 5)
	{	
		lseek(fd, 4, SEEK_CUR);
		nread = read(fd, line, sizeof(line));
		char str1[4];
		strcpy(str1, "TYPE");
		if(strstr(line, str1))
		{
	//		char* connectionName = (char *) malloc(10 * sizeof(char));
	//		lseek(fd, 6, SEEK_CUR);
	//		sscanf(line, "%s %s %s", garbage1, garbage2, connectionName);	
		//	printf("%s", connectionName);
		connectionName = NULL;
		}else
		{	
			connectionName = (char *) malloc(5 * sizeof(char));
			sscanf(line, "%s %s", garbage1, connectionName);


		}

		lseek(fd, 0, SEEK_SET);
	}if(lineNumber == 6)
	{

		lseek(fd, 5, SEEK_CUR);
		nread = read(fd, line, sizeof(line));
		char str1[4];
		strcpy(str1, "TYPE");
		if(strstr(line, str1))
		{
			connectionName = NULL;
		}else
		{	
			connectionName = (char *) malloc(5 * sizeof(char));
			sscanf(line, "%s %s", garbage1, connectionName);


		}

	

		lseek(fd, 0, SEEK_SET);
	}if(lineNumber == 7)
	{

		lseek(fd, 10, SEEK_CUR);
		connectionName = (char *) malloc(10 * sizeof(char));
		nread = read(fd, line, sizeof(line));
		
	
			sscanf(line, "%s %s", garbage1, connectionName);
	//		printf("%s", connectionName);
			connectionName = NULL;
			lseek(fd, 0, SEEK_SET);
	}





	return connectionName;

}

//This takes a room struct and displays a line to the user screen containing the current room info
void displayCurrentRoom(struct Room* currentRoom)
{
	printf("CURRENT LOCATION: ");
	printf("%s", currentRoom->roomName);
	printf("\n");
	return;
}

//This takes a connection struct and displays the connection info to the user screen
void displayConnections(struct Room* currentRoom)
{
	printf("POSSIBLE CONNECTIONS: ");
		printf("%s", currentRoom->connection1);
		printf(", ");
		printf("%s", currentRoom->connection2);

		if(currentRoom->connection3 != NULL)
		{	printf(", ");
			printf("%s", currentRoom->connection3);
		
			if(currentRoom->connection4 != NULL)
			{	printf(", ");
				printf("%s", currentRoom->connection4);
				if(currentRoom->connection5 != NULL)
				{	printf(", ");
					printf("%s", currentRoom->connection5);
			
					if(currentRoom->connection6 != NULL)
					{	printf(", ");
						printf("%s", currentRoom->connection6);
					}

				}

			}
	
		}




		printf(".\n");
	return;
}
//This simply displays the "WHERE TO? >" prompt to the user
void displayPrompt()
{	printf("WHERE TO? >");
	return;
}

//This displays an error message to the user after they type in the wrong input
void displayError()
{

	printf("\n\nHUH? I DONT UNDERSTAND THAT ROOM.\nTRY AGAIN.\n\n");

	return;
}
//This function takes a current room and compares all of its connections with 
//the incoming string to see if there is a match
int isConnected(struct Room* currentRoom, char* str)
{
	int connected = 0;
	if(currentRoom->connection1 != NULL)
	{
		if (strcmp(currentRoom->connection1, str)==0)
			{connected = 1;}
	}
	if(currentRoom->connection2 != NULL)
	{
		if (strcmp(currentRoom->connection2, str)==0)
			{connected = 1;}
	}
	if(currentRoom->connection3 != NULL)
	{
		if (strcmp(currentRoom->connection3, str)==0)
			{connected = 1;}
	}
	if(currentRoom->connection4 != NULL)
	{
		if (strcmp(currentRoom->connection4, str)==0)
			{connected = 1;}
	}
	if(currentRoom->connection5 != NULL)
	{
		if (strcmp(currentRoom->connection5, str)==0)
			{connected = 1;}
	}
	if(currentRoom->connection6 != NULL)
	{
		if (strcmp(currentRoom->connection6, str)==0)
			{connected = 1;}
	}


	return connected;	
}

//This function will recieve input from the user and will return the string to the main function
char* getInput(struct Room* currentRoom)
{	
	int valid = 0;	
	char* input = (char*) malloc(100 * sizeof(char));

	while(!valid)
	{	
		memset(input, '\0', sizeof(input));
		displayPrompt();
		fgets(input, 100, stdin);
		input[strlen(input)-1]='\0';
		valid = validateInput(input);
		if(!valid)
		{
			displayError();
			displayCurrentRoom(currentRoom);
			displayConnections(currentRoom);	
		}
	}	
	return input; 
}

//This function will validate the user input 
int validateInput(char* input)
{
	int valid;	
	if(strcmp(input, "ROOM0") == 0)
	{valid = 1;}else
	if(strcmp(input, "ROOM1") == 0)
	{valid = 1;}else
	if(strcmp(input, "ROOM2") == 0)
	{valid = 1;}else
	if(strcmp(input, "ROOM3") == 0)
	{valid = 1;}else
	if(strcmp(input, "ROOM4") == 0)
	{valid = 1;}else
	if(strcmp(input, "ROOM5") == 0)
	{valid = 1;}else
	if(strcmp(input, "ROOM6") == 0)
	{valid = 1;}else
	if(strcmp(input, "ROOM7") == 0)
	{valid = 1;}else
	if(strcmp(input, "ROOM8") == 0)
	{valid = 1;}else
	if(strcmp(input, "ROOM9") == 0)
	{valid = 1;}else
	if(strcmp(input, "time") == 0)
	{valid = 1;}else
	{
		valid = 0;
	}


	return valid;
}


void congrats()
{
	printf("CONGRADULATIONS!! YOU WON!\n\n");

}
