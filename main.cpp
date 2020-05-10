/*******************************************************************************************
 *Justin Pulido
 *lab4
 *October 6th, 2019
 *the main program will be testing the functions within the .cpp files. 
*******************************************************************************************/

#include "Sdisk.h"
#include "Filesys.h"
#include "Shell.h"
#include <vector>
#include <iostream>

using namespace std;

int main()
{
	//
	//This main program inputs commands to the shell.
	//It inputs commands as : command op1 op2
	//You should modify it to work for your implementation.
	Shell shell1("disk1", 256, 128);
	string s;
	string command="go";
	string op1,op2;

	while (command != "quit") {
   		command.clear();
    	op1.clear();
    	op2.clear();
    	cout << "$";

    	getline(cin,s);
    	int firstblank=s.find(' ');

    	if (firstblank < s.length()) s[firstblank]='#';

    	int secondblank=s.find(' ');

    	command=s.substr(0,firstblank);
    	if (firstblank < s.length()) {
    	  op1=s.substr(firstblank+1,secondblank-firstblank-1);
		}

    	if (secondblank < s.length()) {
    	  op2=s.substr(secondblank+1);
		}

    	if (command=="dir") {
			shell1.dir();
        }
       	if (command=="add") {
            // The variable op1 is the new file
			shell1.add(op1);
        }
       	if (command=="del") {
            // The variable op1 is the file
			shell1.del(op1);
        }
		if (command=="type") {
            // The variable op1 is the file
			shell1.type(op1);
        }
       	if (command=="copy")
        {
            // The variable op1 is the source file and the variable op2 is the destination file.
			shell1.copy(op1, op2);
        }
	}

	return 0;
}
