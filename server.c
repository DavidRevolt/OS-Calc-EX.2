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


//Name: David Manshari
//ID: 307892935

//Name: Nir Abadi
//ID:206172546 


//METHODS:
void closeProg();
void childHandler();
void childProcess();
void mystrcat(char* a, char* b);
char* intToChar(int a, char* ab);



int main() {
	remove("to_srv.txt"); //Server open first and delete the to_srv.txt if exist
	printf("\nend of stage 5.c\n");

	signal(SIGUSR1, childHandler);
	signal(SIGALRM, closeProg);
	alarm(60);
	while (1)
		pause();
}




////////////////////////////////////////////////////////////////////////////////////////////////////
//childHandler
////////////////////////////////////////////////////////////////////////////////////////////////////
void childHandler()
{
	signal(SIGUSR1, childHandler);
	alarm(60);

	pid_t pid = fork();
	if (pid < 0)
	{
		printf("ERROR_FROM_EX2\n");
		exit(-1);
	}

	if (pid == 0) //child
	{
		printf("end of stage 5.f\n");
		childProcess(); //child process handler
	}

	return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//Child Process Handler
////////////////////////////////////////////////////////////////////////////////////////////////////
void childProcess()
{
    //open the "to_srv.txt" and read input
	int toSrvFD = open("to_srv.txt", O_RDONLY); 
	if (toSrvFD < 0) //Success
	{
		printf("ERROR_FROM_EX2\n");
		exit(-1);
	}

	int clientID;
	int firstNum; 
	int op;
	int secNum;

	read(toSrvFD, &clientID, sizeof(int));
	read(toSrvFD, &firstNum, sizeof(int));
	read(toSrvFD, &op, sizeof(int));
	read(toSrvFD, &secNum, sizeof(int));
	close(toSrvFD);

	//removing "to_srv.txt" so other clients can be used
	int r = remove("to_srv.txt");
	if (r != 0)
		printf("ERROR_FROM_EX2\n");

	printf("end of stage 5.g\n");


	//Creating "to_client_clientID" and writing the result
	char toClientTxt[100] = "to_client_";
	intToChar(clientID, toClientTxt);
	mystrcat(toClientTxt, ".txt");

	int toClientFile = open(toClientTxt, O_WRONLY | O_CREAT | O_TRUNC, 0777); //Create
	if (toClientFile < 0)
	{
		printf("ERROR_FROM_EX2\n");
		exit(-1);
	}
	printf("end of stage 5.h\n");



	//Calculating
	int result;
	switch (op) {

	case 1:result = firstNum + secNum;
		break;

	case 2:result = firstNum - secNum;
		break;

	case 3:result = firstNum * secNum;
		break;

	case 4:
		result = firstNum / secNum;
		break;
	}
	write(toClientFile, &result, sizeof(int));
	close(toClientFile);


	//send Ready signal to clientID
	int sent = kill(clientID, SIGUSR1);
	if (sent < 0)
	{
		printf("ERROR_FROM_EX2\n");
		exit(-1);
	}

	printf("end of stage 5.i\n");
	exit(1); //Child process is DONE!
}




////////////////////////////////////////////////////////////////////////////////////////////////////
//Close Program
///////////////////////////////////////////////////////////////////////////////////////////////////
void closeProg()
{
	signal(SIGALRM, closeProg);
	printf("\nThe server as closed because no service request from the last 60 seconds\n");
	int status, childz;
	while ((childz = wait(&status)) > 0);	//waits for zombies
	exit(1);
}




//Strings Method:
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