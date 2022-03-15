#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/random.h>





//METHODS:
void createToSrv();
int stringToNum(char* input);
void closeProg();
void printResult();
void mystrcat(char* a, char* b);
char* intToChar(int a, char* ab);

//GLOBAL VARS:
int toSrvFD;
int pID;
char fileName[100] = "to_client_";



int main(int argc, char* argv[]) {
	//CHECK FOR VALID INPUT:
	if (argc != 5) {
		printf("ERROR_FROM_EX2\n");
		exit(-1);
	}

	//Convert String input to int
	pID = getpid();
	int serverPID = stringToNum(argv[1]);
	int first = stringToNum(argv[2]);
	int op = stringToNum(argv[3]);
	int sec = stringToNum(argv[4]);

	//CHECK FOR DIVIDING BY ZERO:
	if ((op == 4) && (sec == 0)) {
		printf("ERROR_FROM_EX2\n");
		exit(-1);
	}


	//Write input to "to_srv.txt"
	createToSrv();
	write(toSrvFD, &pID, sizeof(int));
	write(toSrvFD, &first, sizeof(int));
	write(toSrvFD, &op, sizeof(int));
	write(toSrvFD, &sec, sizeof(int));
	close(toSrvFD);
	printf("\nend of stage 5.d\n");


	signal(SIGUSR1, printResult);
	signal(SIGALRM, closeProg);

	//signaling the server
	int sent = kill(serverPID, SIGUSR1);
	if (sent < 0)
	{
		printf("ERROR_FROM_EX2\n");
		//we created to_srv but there is no server, so lets remove to_srv and exit
		int r = remove("to_srv.txt"); 
		if (r != 0)
			printf("ERROR_FROM_EX2\n");
		exit(-1);
	}

	//wait for reaply
	alarm(30);
	pause();

}




////////////////////////////////////////////////////////////////////////////////////////////////////
//createToSrv: Try to Create to_srv.txt, if already exsited/in use --> try again [Limited to 10 times]
///////////////////////////////////////////////////////////////////////////////////////////////////
void createToSrv()
{

	for (int i = 0; i < 10; i++)
	{
		toSrvFD = open("to_srv.txt", O_WRONLY | O_CREAT | O_EXCL, 0777); //Create & Not existed
		if (toSrvFD > 0) //Success
		{
			if(i>0) //sleep() been used
			printf("end of stage 5.e\n");

			return;
		}

		else
		{
			unsigned int sec;
			getrandom(&sec, sizeof(unsigned), 0);
			sleep(sec % 6); //Lets try again is 6sec+-
		}
	}

	//If we reached here: Create "to_srv.txt" failed
	printf("end of stage 5.e\n");
	printf("ERROR_FROM_EX2\n");
	exit(-1);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
//Close Program
///////////////////////////////////////////////////////////////////////////////////////////////////
void closeProg()
{
	signal(SIGALRM, closeProg);
	printf("Client closed because no response was received from the server for 30 seconds\n");
	exit(-1);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//Print the result, fileame: "to_client_clientID.txt"
///////////////////////////////////////////////////////////////////////////////////////////////////
void printResult()
{
	alarm(0); //Reset the alarm 
	signal(SIGUSR1, printResult);

	//Create string to_client_xxx.txt, filename="to_client_"
	intToChar(pID, fileName);
	mystrcat(fileName, ".txt");

	int textFile = open(fileName, O_RDONLY);
	if (textFile < 0)
	{
		printf("ERROR_FROM_EX2\n");
		exit(-1);
	}
	int buf;
	while (read(textFile, &buf, 4))
		printf("%d\n", buf);

	close(textFile);
	int r = remove(fileName);
	if (r != 0)
		printf("ERROR_FROM_EX2\n");

	printf("end of stage 5.j\n\n");
	//sleep(60); //FOR PDF ONLY!
	exit(1);
}





//Strings Methods:
////////////////////////////////////////////////////////////////////////////////////////////////////
//stringToNum: Convert String to number
///////////////////////////////////////////////////////////////////////////////////////////////////
int stringToNum(char* input)
{

	int result = 0;
	int sign = 1; //1 Positive, -1 Negetive
	int i = 0; //index 

	// If number is negative, then update sign 
	if (input[0] == '-'){
		sign = -1;
		i++; 
	}

	// Iterate through all digits and update the result 
	while(input[i] != '\0')
	{
		result = result * 10 + input[i] - '0';
		i++;
	}
	

	return sign * result;



	//positive ints algo:
/*
	int result = 0;
	for (int i = 0; input[i] != '\0'; ++i)
		result = result * 10 + input[i] - '0';

	return result;
	*/
}



////////////////////////////////////////////////////////////////////////////////////////////////////
//String Append to String
///////////////////////////////////////////////////////////////////////////////////////////////////
void mystrcat(char* a, char* b) {
	int ln = 0;

	while (a[ln] != '\0') {
		ln++;
	}
	int i = 0;
	while (b[i] != '\0') {
		a[ln + i] = b[i];
		i++;
	}
	a[ln + i + 1] = '\0';

}



////////////////////////////////////////////////////////////////////////////////////////////////////
//Number Append to String
///////////////////////////////////////////////////////////////////////////////////////////////////
char* intToChar(int a, char* ab) {
	int i = 0;
	char c[10] = { '\0' };
	int arr[100];
	int temp = a;
	int ln = 0;

	while (temp != 0) {
		temp = temp / 10;
		ln++;
	}

	if (a == 0) c[0] = '0';

	while (a != 0) {
		int d = a % 10;
		a = a / 10;

		c[ln - 1] = (d + '0');
		ln--;
	}

	mystrcat(ab, c);
	return ab;

}
