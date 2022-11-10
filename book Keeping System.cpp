#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <vector>

using namespace std;

/*
Function 			: storeBalance(string date,string acno, int type,float a,float balance)
Description 		: this function will store balances in relevent text files
Input parameters 	: string date
					  string acno - account number
					  int type - operation type
					  float a - amount to be added or reduced
					  float balance - final balance					   
Return 				: print in text file
*/
void storeBalance(string date,string acno, int type,float a,float balance)
{
	string name=acno+".txt"; //create the file name
	ofstream file; 
	file.open(name.c_str(), ios::out | ios::app); //open file
	file<<date<<","<<type<<","<<a<<","<<balance<<"\n"; //write data
    file.close();
}

/*
Function 			: getLastBalance(string acno)
Description 		: this function will return last balance in the text file
Input parameters 	: string acno - account number			   
Return 				: return last balance in the text file
*/
float getLastBalance(string acno)
{
	vector<float> v; //store existing balances in a vector and then get the last balance
	string name=acno+".txt";
	ifstream file;
	file.open(name.c_str(),ios::out | ios::app);
	if(file.is_open())
	{
		string linen;
		while(getline(file,linen))
		{
			stringstream ssn(linen);
			string date,op,opbal,amn;
			getline(ssn,date,',');
			getline(ssn,op,',');
			getline(ssn,opbal,',');
			getline(ssn,amn,',');
			float amount = atof(amn.c_str());
			v.push_back(amount); //store balances in vector
		}
	}
	file.close();
	int si=v.size();
	float closingB=v[si-1]; //return last balance
	
	return closingB;
}

/*
Function 			: enterTransactions(string date,string acno, int type,float opBal,float amount)
Description 		: this function will store transactions in relevent text files
Input parameters 	: string date
					  string acno - account number
					  int type - operation type
					  float opBal 
					  float amount - amount to be added or reduced
					  float balance - final balance					   
Return 				: print in text file
*/
void enterTransactions(string date,string acno, int type,float opBal,float amount)
{
	float finalB;
	if(type==1) //deposit
	{
		finalB=opBal+amount;
		storeBalance(date,acno,type,amount,finalB);
	}
	
	else if(type==2) //withdraw
	{
		finalB=opBal-amount;
		storeBalance(date,acno,type,amount,finalB);
	}
}

/*
Function 			: enterTransactions(string date,string acno, int type,float opBal,float amount)
Description 		: this function will add charges or interests to the accounts
Input parameters 	: string d - current date
					  string next - next date
					  vector <string> &accNumbers - vector by reference		   
Return 				: print in text file
*/
void DoOtherOperations(vector<string> &accNumbers, string d, string next)
{
	int x=accNumbers.size(); //get no of accounts
	int currentD= atoi(d.c_str()); //convert current date to a integer
	int nextD = atoi(next.c_str()); //convert next date to a integer
	for(int j=currentD;j<nextD;j++) //loop through the date period
	{
		float charge=0, interest=0; //to bank.txt , we need to add all charges and interests
		float bb=getLastBalance("bank"); //last balance of bank.txt
		string D; //convert integer date to a string to store in the text file
		stringstream sn;
		sn<<j;
		sn>>D;
		for(int i=0;i<x;i++) //each day loop through all accounts and perform relevent task according to the balances
		{
			float b=getLastBalance(accNumbers[i]); //get last balance of the account
			
			if(b<1000 && b>=0)
			{
				
				storeBalance(D,accNumbers[i],4,10,b-10);
				charge+=10;
				b=b-10; //reduce the last balance 
				float totalB=b+(b*0.0002);
				storeBalance(D,accNumbers[i],3,b*0.0002,totalB); //add the interest after deduct 10 rupees
				interest+=b*0.0002;
			}
			
			else if(b<0)
			{
				storeBalance(D,accNumbers[i],4,50,b-50);
				charge+=50;
			}
			
			else if(b>=1000)
			{
				float totalB=b+(b*0.0002);
				storeBalance(D,accNumbers[i],3,b*0.0002,totalB);
				interest+=b*0.0002;
			}
		}
		storeBalance(D,"bank",4,charge,bb+charge); //add total charge of each day
		bb=bb+charge;
		storeBalance(D,"bank",3,interest,bb-interest); //reduce totalinterest of each day
	}
}

/*
Function 			: storebankAccount(string date,int type, float a, float Balance)
Description 		: this function will store initial opening balance of the bank
Input parameters 	: string date
					  int type - operation type
					  float a - amount to be added or reduced
					  float Balance - final balance					   
Return 				: print in text file
*/
void storebankAccount(string date,int type, float a, float Balance)
{
	ofstream fi;
	fi.open("bank.txt", ios::out | ios::app);
	fi<<date<<","<<type<<","<<a<<","<<Balance<<"\n";
    fi.close();
}

/*
Function 			: printUserInputDetails(string acno, string dateI)
Description 		: this function will print user interface data
Input parameters 	: string dateI
					  string acno - account number				   
Return 				: print user interface data
*/
void printUserInputDetails(string acno, string dateI)
{
	string name=acno+".txt";
	ifstream file;
	file.open(name.c_str(),ios::out | ios::app); //open relevent account according to the user input
	if(file.is_open())
	{
		string linen;
		while(getline(file,linen))
		{
			stringstream ssn(linen);
			string date,op,bal,amn;
			getline(ssn,date,',');
			getline(ssn,op,',');
			getline(ssn,amn,',');
			getline(ssn,bal,',');
			
			int t = atoi(op.c_str());
			float amount = atof(amn.c_str());
			float baln = atof(bal.c_str());
			if(date==dateI) //check if the date equals to the userinput
			{
				if(t==3) //we have to print the last balance of each day before the interest add
				{
					cout<<"balance before interest : "<<baln-amount<<endl;
					cout<<"balance after interest : "<<baln;
					break;
				}
			}
		}
	}
	file.close();
}

int main()
{
	vector<string> accNumbers; //vector to store account numbers from balance.txt
	ifstream balan("balance.txt"); //read from balance.txt
	if(balan.is_open())
	{
		string line;
		while(getline(balan,line)) //read line by line
		{
			stringstream ss(line);
			string date,accno,b;
			getline(ss,date,',');
			getline(ss,accno,',');
			getline(ss,b,',');
			float balance = atof(b.c_str()); //convert string to float
			accNumbers.push_back(accno);
		}
	}
	balan.close();
	int size=accNumbers.size();
	for(int i=0;i<size;i++) //delete initial text files if there are any
	{
		string name=accNumbers[i]+".txt"; //create text file name
		ofstream file1; //open file
		remove(name.c_str()); //remove that file
	}
	remove("bank.txt"); //remove bank.txt
	
	ifstream bal("balance.txt"); //again open balance.txt to store opening balances
	if(bal.is_open())
	{
		float total=0; //this varible used to add up all the balances to store in the bank,txt
		string line;
		while(getline(bal,line)) //read line by line
		{
			stringstream ss(line);
			string date,accno,b;
			getline(ss,date,',');
			getline(ss,accno,',');
			getline(ss,b,',');
			float balance = atof(b.c_str());
			total+=balance;
			storeBalance(date,accno,0,balance,balance); //store balance in relevent account.txt
		}
		storebankAccount("20210101",0, total, total); //store opening balance of bank.txt
	}
	bal.close(); //close balance.txt
	
	//4 vectors to store transactions.txt file
	vector<string> TDates; //vector to store dates
	vector<string> TAccounts; //vector to store account numbers
	vector<int> TOperations; //vector to store type of operations
	vector<float> TAmounts; //vector to store amounts
	ifstream transac("transaction.txt"); //read from transactions.txt
	if(transac.is_open())
	{	
		string linen;
		while(getline(transac,linen)) //read line by line
		{
			stringstream ssn(linen);
			string date,accno,op,amn;
			getline(ssn,date,',');
			getline(ssn,accno,',');
			getline(ssn,op,',');
			getline(ssn,amn,',');
			int t=atoi(op.c_str()); //convert type of operation to a integer
			float amount = atof(amn.c_str()); //convert amount to a float
			TDates.push_back(date); //store in the relevent vectors
			TAccounts.push_back(accno);
			TOperations.push_back(t);
			TAmounts.push_back(amount);
		}
	}
	transac.close();
	
	int noOfTs=TDates.size(); //no of transactions is the size of above 4 vectors
	for(int i=0;i<noOfTs;i++) //loop through every transactions
	{
		float prevB=getLastBalance(TAccounts[i]); //get last balance from the relevent account
		float prevBankBal=getLastBalance("bank"); //get last balance from the bank.txt
		enterTransactions(TDates[i],TAccounts[i], TOperations[i],prevB,TAmounts[i]); //enter transaction in account.txt
		enterTransactions(TDates[i],"bank", TOperations[i],prevBankBal,TAmounts[i]); //enter transaction in bank.txt
		//next the program checks the nest date. if its not equal that means the day is ended and we need to add interests or deduct charges
		if(i<noOfTs-1) 
		{
			if(TDates[i]!=TDates[i+1])
			{
				DoOtherOperations(accNumbers, TDates[i], TDates[i+1]); //end of the day, do other operations
			}
		}
		
	}
	
	//user interface
	int choice;
	string dateInput, accountInput;
	cout<<"you can view the details of following accounts:\n";
	cout<<"\t1.user account\n\t2.bank vault account\n";
	cout<<"please enter the number of type of account you want to see: ";
	while(true)
	{
		cin>>choice; //type of account
		if(choice==1 || choice==2)
			break;
		else
			cout<<"invalid input. please try again. enter a correct number again :";
	}
	if(choice==1) //account.txt
	{
		cout<<"\nenter the account number you want to see: ";
		cin>>accountInput;
		bool valid=false;
		for(int i=0;i<size;i++)
		{
			if(accNumbers[i]==accountInput) //check if the account is in the file
			{
				valid=true;
				break;
			}
		}
		if(valid==false)
			cout<<"entered account number is not in the system ";
		else
		{
			cout<<"enter the date you want to get details (enter in yyyymmdd format) :";
			cin>>dateInput;
			int inputDate= atoi(dateInput.c_str()); //convert date to a integer
			if(inputDate>=20210101 && inputDate<20210111) // validity check the date
			{
				printUserInputDetails(accountInput, dateInput); //print the details
			}
			else
				cout<<"entered date is incorrect or not in the system";
		}
		cout<<endl;
	}
	else if(choice==2)
	{
		cout<<"enter the date you want to get details (enter in yyyymmdd format) :";
		cin>>dateInput;
		int inputDate= atoi(dateInput.c_str());
		if(inputDate>=20210101 && inputDate<=20210111)
		{
			printUserInputDetails("bank", dateInput);
		}
		else
			cout<<"entered date is incorrect or not in the system";
	}
	
	return 0;
}

