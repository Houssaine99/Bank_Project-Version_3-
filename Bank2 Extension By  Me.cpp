
#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <vector>
#include <fstream>

std::string ClientsFileName = "Clients.txt";
std::string UsersFileName = "Users.txt";

void ShowMainMenu();
void ShowTransactionsMenu();
void ShowManageUsersScreen();
void Login();

enum enMainMenuOptions
{
	eClientsList = 1,
	eAddClient = 2,
	eDeleteClient = 3,
	eUpdateClient = 4,
	eFindClient = 5,
	eTransactions = 6,
	eManageUsers = 7,
	eLogout = 8
};

enum enTransactionsMenuOptions
{
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalances = 3,
	eMainMenuTransactions = 4
};

enum enManageUsersMenuOptions
{
	eListUsers = 1,
	eAddNewUser = 2,
	eDeleteUser = 3,
	eUpdateUser = 4,
	eFindUser = 5,
	eMainMenuUsers = 6
};

struct sClients
{
	std::string AccountNumber;
	std::string PINCode;
	std::string Name;
	std::string Phone;

	double AccountBalance = 0;

	bool MarkForDelete = false;
};

struct sUsers
{
	std::string Username;
	std::string Password;

	short Permissions = 0;

	bool MarkForDelete = false;
};

sUsers CurrentUser;

// ----------------------------------> Data_Base <----------------------------------

std::vector <std::string> SplitString(std::string StrToSplit, std::string delim)
{
	std::vector <std::string> vSplitedString;
	std::string Word;
	short pos = 0;

	while ((pos = StrToSplit.find(delim)) != std::string::npos)
	{
		Word = StrToSplit.substr(0, pos);

		if (Word != "")
			vSplitedString.push_back(Word);

		StrToSplit.erase(0, pos + delim.length());
	}
	if (StrToSplit != "")
		vSplitedString.push_back(StrToSplit);

	return vSplitedString;
}

sClients ConvertLineToClientRecord(std::string Line, std::string Seperator = "#//#")
{
	std::vector <std::string> vSplitedLine = SplitString(Line, Seperator);
	sClients Client;

	Client.AccountNumber = vSplitedLine[0];
	Client.PINCode = vSplitedLine[1];
	Client.Name = vSplitedLine[2];
	Client.Phone = vSplitedLine[3];
	Client.AccountBalance = std::stod(vSplitedLine[4]);

	return Client;
}

sUsers ConvertLineToUserRecord(std::string Line, std::string Seperator = "#//#")
{
	std::vector <std::string> vSplitedLine = SplitString(Line, Seperator);
	sUsers User;

	User.Username = vSplitedLine[0];
	User.Password = vSplitedLine[1];
	User.Permissions = std::stoi(vSplitedLine[2]);

	return User;
}

std::string ConvertRecordToLine(sClients Record, std::string Seperator = "#//#")
{
	std::string Line;

	Line += Record.AccountNumber + Seperator;
	Line += Record.PINCode + Seperator;
	Line += Record.Name + Seperator;
	Line += Record.Phone + Seperator;
	Line += std::to_string(Record.AccountBalance);

	return Line;

}

std::string ConvertRecordToLine(sUsers Record, std::string Seperator = "#//#")
{
	std::string Line;

	Line += Record.Username + Seperator;
	Line += Record.Password + Seperator;
	Line += std::to_string(Record.Permissions);

	return Line;
}

std::vector <sClients> LoadClientsDataFromFile(std::string FileName)
{
	std::vector <sClients> vAllClients;

	std::fstream MyFile;
	MyFile.open(FileName, std::ios::in);

	if (MyFile.is_open())
	{
		std::string Line;
		sClients Client;

		while (std::getline(MyFile, Line))
		{
			Client = ConvertLineToClientRecord(Line);
			vAllClients.push_back(Client);
		}
		MyFile.close();
	}

	return vAllClients;
}

std::vector <sUsers> LoadUsersDateFromFile(std::string FileName)
{
	std::vector <sUsers> vUsers;

	std::fstream MyFile;
	MyFile.open(FileName, std::ios::in);

	if (MyFile.is_open())
	{
		std::string Line;
		sUsers User;

		while (std::getline(MyFile, Line))
		{
			User = ConvertLineToUserRecord(Line);
			vUsers.push_back(User);
		}

		MyFile.close();
	}

	return vUsers;
}

bool ClientExistsByAccountNumber(std::string FileName, std::string AccountNumber)
{
	std::fstream MyFile;
	MyFile.open(FileName, std::ios::in);

	if (MyFile.is_open())
	{
		std::string Line;
		sClients Client;

		while (std::getline(MyFile, Line))
		{
			Client = ConvertLineToClientRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
		}
		MyFile.close();
	}
	return false;
}

void AddDataLineToFile(std::string FileName, std::string Line)
{
	std::fstream MyFile;
	MyFile.open(FileName, std::ios::out | std::ios::app);

	if (MyFile.is_open())
	{
		MyFile << Line << std::endl;
		MyFile.close();
	}
}

void SaveClientsDataToFile(std::string FileName, std::vector <sClients>& vAllClients)
{
	std::fstream MyFile;
	MyFile.open(FileName, std::ios::out);

	if (MyFile.is_open())
	{
		std::string Line;
		for (sClients& Client : vAllClients)
		{
			if (!Client.MarkForDelete)
			{
				Line = ConvertRecordToLine(Client);
				MyFile << Line << std::endl;
			}
		}
		MyFile.close();
	}
}

void SaveUsersDataToFile(std::string FileName, std::vector <sUsers>& vAllUsers)
{
	std::fstream MyFile;
	MyFile.open(FileName, std::ios::out);

	if (MyFile.is_open())
	{
		std::string Line;

		for (sUsers& User : vAllUsers)
		{
			if (!User.MarkForDelete)
			{
				Line = ConvertRecordToLine(User);
				MyFile << Line << std::endl;
			}
		}

		MyFile.close();
	}
}

// ----------------------------------> Input <----------------------------------

short ReadMainMenuOption()
{
	short InputValue = 0;
	std::cout << "Choose what do you want to do [1 to 8]? ";
	std::cin >> InputValue;

	return InputValue;
}

short ReadTransactionsMenuOption()
{
	short InputValue = 0;
	std::cout << "Choose what do you want to do [1 to 4]? ";
	std::cin >> InputValue;

	return InputValue;
}

short ReadManageUsersMenuOption()
{
	short InputValue = 0;
	std::cout << "Choose what do you want to do [1 to 6]? ";
	std::cin >> InputValue;

	return InputValue;
}

sClients ReadNewClient()
{
	sClients Client;

	std::cout << "Enter Account Number? ";
	std::getline(std::cin >> std::ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(ClientsFileName, Client.AccountNumber))
	{
		std::cout << "\nClient with account number [" << Client.AccountNumber << "] is already exists! Enter Account Number? ";
		std::getline(std::cin, Client.AccountNumber);
	}

	std::cout << "Enter PIN Code? ";
	std::getline(std::cin, Client.PINCode);

	std::cout << "Enter Client Name? ";
	std::getline(std::cin, Client.Name);

	std::cout << "Enter Phone Number? ";
	std::getline(std::cin, Client.Phone);

	std::cout << "Enter Account Balance? ";
	std::cin >> Client.AccountBalance;

	return Client;
}

std::string ReadAccountNumber()
{
	std::string InputValue;
	std::cout << "Enter Account Number? ";
	std::getline(std::cin >> std::ws, InputValue);

	return InputValue;
}

sClients ChangeClientRecord(std::string AccountNumber)
{
	sClients Client;

	Client.AccountNumber = AccountNumber;

	std::cout << "\n\nEnter PIN Code? ";
	std::getline(std::cin >> std::ws, Client.PINCode);

	std::cout << "Enter Name? ";
	std::getline(std::cin, Client.Name);

	std::cout << "Enter Phone? ";
	std::getline(std::cin, Client.Phone);

	std::cout << "Enter Account Balance? ";
	std::cin >> Client.AccountBalance;

	return Client;
}

sUsers ReadUserLoginInfo()
{
	sUsers User;

	std::cout << "Enter Username? ";
	std::getline(std::cin >> std::ws, User.Username);

	std::cout << "Enter Password? ";
	std::getline(std::cin, User.Password);

	return User;
}

short ReadPermissionToSet()
{
	char Answer = 'Y';
	short Permissions = 0;

	std::cout << "\nDo you want to give full access (y/n)? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		return -1;

	std::cout << "\nDo you want to give access to: \n";

	std::cout << "\nShow Client List? y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += 1;

	std::cout << "\nAdd New Clinet? y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += 2;

	std::cout << "\nDelete Client? y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += 4;

	std::cout << "\nUpdate Client? y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += 8;

	std::cout << "\nFind Client? y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += 16;

	std::cout << "\nTransactions? y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += 32;

	std::cout << "\nManage Users? y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += 64;

	return Permissions;
}

sUsers ChangUserRecord(std::string Username)
{
	sUsers User;
	
	User.Username = Username;
	
	std::cout << "\nEnter password: ";
	std::getline(std::cin >> std::ws, User.Password);

	User.Permissions = ReadPermissionToSet();

	return User;
}

// ----------------------------------> Processing <----------------------------------

void ShowAccessDeniedScreen()
{
	std::cout << "-------------------------------------\n";
	std::cout << "Access Denied:\n";
	std::cout << "You don't have permission to do this,\n";
	std::cout << "Please contact your admin.";
	std::cout << "\n-------------------------------------\n";
}

void BackToMainMenu()
{
	std::cout << "\n\nPress any key to back to Main Menu ...";
	system("pause>0");
	system("cls");
	ShowMainMenu();
}

void BackToTransactionsMenu()
{
	std::cout << "\n\nPress any key to back to Transactions Menu ...";
	system("pause>0");
	system("cls");
	ShowTransactionsMenu();
}

void BackToManageUsersMenu()
{
	std::cout << "\n\nPress any key to back to Manage Users Menu ...";
	system("pause>0");
	system("cls");
	ShowManageUsersScreen();
}

void AddNewClient()
{
	sClients Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
	char AddMore = 'Y';

	do
	{
		std::cout << "Adding New Client\n\n";

		AddNewClient();

		std::cout << "\n\nClient Added Susccessfully. Do you want to add more clients (y/n)? ";
		std::cin >> AddMore;

		std::cout << std::endl << std::endl;

	} while (toupper(AddMore) == 'Y');
}

bool FindClientByAccountNumber(std::string AccountNumber, sClients& Client, std::vector <sClients>& vAllClients)
{
	for (sClients& C : vAllClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool FindUserByUsername(std::string Username, sUsers& User, std::vector <sUsers>& vAllUsers)
{
	for (sUsers& U : vAllUsers)
	{
		if (U.Username == Username)
		{
			User = U;
			return true;
		}
	}
	return false;
}

void PrintClientCard(sClients& Client)
{
	std::cout << "\nThe following are the client details:\n";
	std::cout << "---------------------------------------\n";
	std::cout << "Account Number : " << Client.AccountNumber << std::endl;
	std::cout << "PIN Code       : " << Client.PINCode << std::endl;
	std::cout << "Client Name    : " << Client.Name << std::endl;
	std::cout << "Phone Number   : " << Client.Phone << std::endl;
	std::cout << "Account Balance: " << Client.AccountBalance << std::endl;
	std::cout << "---------------------------------------\n";
}

void PrintUserCard(sUsers& User)
{
	std::cout << "\nThe following are the client details:\n";
	std::cout << "---------------------------------------\n";
	std::cout << "Username    : " << User.Username << std::endl;
	std::cout << "Password    : " << User.Password << std::endl;
	std::cout << "Permissions : " << User.Permissions << std::endl;
	std::cout << "---------------------------------------\n";
}

void MarkClientForDeleteByAccountNumber(std::string AccountNumber, std::vector <sClients>& vAllClients)
{
	for (sClients& Client : vAllClients)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			Client.MarkForDelete = true;
			break;
		}
	}
}

void MarkUserForDeleteByUsername(std::string Username, std::vector <sUsers>& vAllUsers)
{
	for (sUsers& User : vAllUsers)
	{
		if (User.Username == Username)
		{
			User.MarkForDelete = true;
			break;
		}
	}
}

void DeleteClientByAccountNumber(std::string AccountNumber, std::vector <sClients>& vAllClients)
{
	sClients Client;
	char Answer = 'Y';

	if (FindClientByAccountNumber(AccountNumber, Client, vAllClients))
	{
		PrintClientCard(Client);

		std::cout << "\n\nAre you sure you want to delete this client (y/n)? ";
		std::cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vAllClients);
			SaveClientsDataToFile(ClientsFileName, vAllClients);

			std::cout << "\n\nClient Deleted Successfully!";
		}
	}
	else
		std::cout << "\n\nClient with account number [" << AccountNumber << "] is NOT found!";
}

void UpdateClientByAccountNumber(std::string AccountNumber, std::vector <sClients>& vAllClients)
{
	sClients Client;
	char Answer = 'Y';

	if (FindClientByAccountNumber(AccountNumber, Client, vAllClients))
	{
		PrintClientCard(Client);

		std::cout << "\n\nAre you sure you want to update this client (y/n)? ";
		std::cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			for (sClients& C : vAllClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientsDataToFile(ClientsFileName, vAllClients);

			std::cout << "\n\nClient Updated Successfully!";
		}
	}
	else
		std::cout << "\n\nClient with account number [" << AccountNumber << "] is NOT found!";
}

void DepositBalanceToClientByAccountNumber(std::string AccountNumber, double Amount, std::vector <sClients>& vAllClients)
{
	char Answer = 'y';

	std::cout << "\n\nAre you sure you want to perform this transaction (y/n)? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (sClients& Client : vAllClients)
		{
			if (Client.AccountNumber == AccountNumber)
			{
				Client.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vAllClients);
				std::cout << "\n\nOperation Done Successfully. New balance is: " << Client.AccountBalance << std::endl;
				break;
			}
		}
	}
}

bool FindUserByUsernameAndPassword(std::string Username, std::string Password, sUsers& CurrentUser)
{
	std::vector <sUsers> vAllUsers = LoadUsersDateFromFile(UsersFileName);

	for (sUsers& User : vAllUsers)
	{
		if (User.Username == Username && User.Password == Password)
		{
			CurrentUser = User;
			return true;
		}
	}

	return false;
}

bool UserExistByUsername(std::string Username)
{
	std::vector <sUsers> vAllUsers = LoadUsersDateFromFile(UsersFileName);

	for (sUsers& User : vAllUsers)
	{
		if (User.Username == Username)
			return true;
	}

	return false;
}

sUsers AddNewUser()
{
	sUsers User;

	std::cout << "Enter Username? ";
	std::getline(std::cin >> std::ws, User.Username);

	while (UserExistByUsername(User.Username))
	{
		std::cout << "\nUser with username [" << User.Username << "] is alerady exist, enter another username? ";
		std::getline(std::cin >> std::ws, User.Username);
	}

	std::cout << "Enter Password? ";
	std::getline(std::cin, User.Password);

	User.Permissions = ReadPermissionToSet();

	return User;
}

void AddNewUsers()
{
	sUsers User;
	char AddMore = 'Y';

	do
	{
		std::cout << "Adding New User\n\n";

		AddDataLineToFile(UsersFileName, ConvertRecordToLine(AddNewUser()));

		std::cout << "\n\nUser Added Successfully. Do you want to add more Users (y/n)? ";
		std::cin >> AddMore;

		std::cout << std::endl << std::endl;

	} while (toupper(AddMore) == 'Y');

}

void DeleteUserByUsername(std::string Username, std::vector <sUsers>& vAllUsers)
{
	sUsers User;
	char Answer = 'y';

	if (Username == "Admin")
		std::cout << "\n\nYou can not delete this user!";

	else if (Username == CurrentUser.Username)
		std::cout << "\n\n[" << Username << "] is active you can't delete it right now!";

	else if (FindUserByUsername(Username, User, vAllUsers))
	{
		PrintUserCard(User);

		std::cout << "\n\nAre you sure you want to delete this user (y/n)? ";
		std::cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkUserForDeleteByUsername(Username, vAllUsers);
			SaveUsersDataToFile(UsersFileName, vAllUsers);

			std::cout << "\n\nUser Deleted Successfully!";
		}
	}

	else
		std::cout << "\n\nUser with username [" << Username << "] is NOT found!";
}

void UpdateUserByUsername(std::string Username, std::vector <sUsers>& vAllUsers)
{
	sUsers User;
	char Answer = 'y';

	if (FindUserByUsername(Username, User, vAllUsers))
	{
		PrintUserCard(User);

		std::cout << "\n\nAre you sure you want to update this user (y/n)? ";
		std::cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			for (sUsers& U : vAllUsers)
			{
				if (U.Username == Username)
				{
					U = ChangUserRecord(Username);
					break;
				}
			}

			SaveUsersDataToFile(UsersFileName, vAllUsers);

			std::cout << "\n\nUser Updated Successfully!";
		}
	}
	else
		std::cout << "\n\nUser with username [" << Username << "] is NOT found!";
}

// ----------------------------------> Output <----------------------------------

void ShowUsersListScreen()
{
	std::vector <sUsers> vAllUsers = LoadUsersDateFromFile(UsersFileName);

	int NumberOfClients = vAllUsers.size();

	std::cout << "\n\t\t\t\t\tUsers List (" << NumberOfClients << ") User(s)\n";

	std::cout << "-------------------------------------------------------------------------------------------------------------\n";
	std::cout << "| " << std::left << std::setw(30) << "Username";
	std::cout << "| " << std::left << std::setw(28) << "Password";
	std::cout << "| " << std::left << std::setw(44) << "Permission" << std::left << std::setw(1) << "|";
	std::cout << "\n-------------------------------------------------------------------------------------------------------------\n";

	for (sUsers& User : vAllUsers)
	{
		std::cout << "| " << std::left << std::setw(30) << User.Username;
		std::cout << "| " << std::left << std::setw(28) << User.Password;
		std::cout << "| " << std::left << std::setw(44) << User.Permissions << std::left << std::setw(1) << "|\n";
	}
	std::cout << "-------------------------------------------------------------------------------------------------------------\n";
}

void ShowAddNewUsersScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Add New User Screen";
	std::cout << "\n------------------------------------------------------------\n";

	AddNewUsers();
}

void ShowDeleteUserScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Delete User Screen";
	std::cout << "\n------------------------------------------------------------\n";

	std::vector <sUsers> vAllUsers = LoadUsersDateFromFile(UsersFileName);
	std::string Username;

	std::cout << "Enter Username? ";
	std::getline(std::cin >> std::ws, Username);

	DeleteUserByUsername(Username, vAllUsers);
}

void ShowUpdateUserScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Update User Screen";
	std::cout << "\n------------------------------------------------------------\n";

	std::vector <sUsers> vAllUsers = LoadUsersDateFromFile(UsersFileName);
	std::string Username;

	std::cout << "Please enter username? ";
	std::getline(std::cin >> std::ws, Username);

	UpdateUserByUsername(Username, vAllUsers);
}

void ShowFindUserScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Find User Screen";
	std::cout << "\n------------------------------------------------------------\n";

	sUsers User;
	std::vector <sUsers> vAllUsers = LoadUsersDateFromFile(UsersFileName);
	std::string Username;

	std::cout << "Enter username: ";
	std::getline(std::cin >> std::ws, Username);

	if (FindUserByUsername(Username, User, vAllUsers))
		PrintUserCard(User);
	else
		std::cout << "\n\nUser with username [" << Username << "] is NOT found!";
}

void PerformManageUsersMenuOptions(enManageUsersMenuOptions ManageUsersMenuOption)
{
	switch (ManageUsersMenuOption)
	{
	case enManageUsersMenuOptions::eListUsers:
		system("cls");
		ShowUsersListScreen();
		BackToManageUsersMenu();
		break;

	case enManageUsersMenuOptions::eAddNewUser:
		system("cls");
		ShowAddNewUsersScreen();
		BackToManageUsersMenu();
		break;

	case enManageUsersMenuOptions::eDeleteUser:
		system("cls");
		ShowDeleteUserScreen();
		BackToManageUsersMenu();
		break;

	case enManageUsersMenuOptions::eUpdateUser:
		system("cls");
		ShowUpdateUserScreen();
		BackToManageUsersMenu();
		break;

	case enManageUsersMenuOptions::eFindUser:
		system("cls");
		ShowFindUserScreen();
		BackToManageUsersMenu();
		break;

	case enManageUsersMenuOptions::eMainMenuUsers:
		system("cls");
		ShowMainMenu();
		break;
	}
}

void ShowManageUsersScreen()
{
	std::cout << "============================================================\n";
	std::cout << "                    Manage Users Menu Screen\n";
	std::cout << "============================================================\n";
	std::cout << "\t[1] List Users.\n";
	std::cout << "\t[2] Add New User.\n";
	std::cout << "\t[3] Delete User.\n";
	std::cout << "\t[4] Update User.\n";
	std::cout << "\t[5] Find User.\n";
	std::cout << "\t[6] Main Menu.\n";
	std::cout << "============================================================\n";
	PerformManageUsersMenuOptions((enManageUsersMenuOptions)ReadManageUsersMenuOption());
}

void ShowDepositScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Deposit Screen";
	std::cout << "\n------------------------------------------------------------\n";

	sClients Client;
	std::vector <sClients> vAllClients = LoadClientsDataFromFile(ClientsFileName);
	std::string AccountNumber = ReadAccountNumber();
	double Amount = 0;

	while (!FindClientByAccountNumber(AccountNumber, Client, vAllClients))
	{
		std::cout << "\nClient with account number [" << AccountNumber << "] does not exist.\n\n";
		AccountNumber = ReadAccountNumber();
	}

	PrintClientCard(Client);

	std::cout << "\n\nEnter deposit amount? ";
	std::cin >> Amount;

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vAllClients);
}

void ShowWithdrawScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Withdraw Screen";
	std::cout << "\n------------------------------------------------------------\n";

	sClients Client;
	double Amount = 0;
	std::vector <sClients> vAllClients = LoadClientsDataFromFile(ClientsFileName);
	std::string AccountNumber = ReadAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, Client, vAllClients))
	{
		std::cout << "\nClient with account number [" << AccountNumber << "] does not exist.\n\n";
		AccountNumber = ReadAccountNumber();
	}

	PrintClientCard(Client);

	std::cout << "\n\nEnter withdraw amount? ";
	std::cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		std::cout << "\nAmount exceeded the balance, you can withdraw up to " << Client.AccountBalance << std::endl;
		std::cout << "\nPlease enter another amount? ";
		std::cin >> Amount;
	}

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vAllClients);
}

void PrintClientRecordBalanceLine(sClients Client)
{
	std::cout << "| " << std::left << std::setw(15) << Client.AccountNumber;
	std::cout << "| " << std::left << std::setw(58) << Client.Name;
	std::cout << "| " << std::left << std::setw(29) << Client.AccountBalance << std::left << std::setw(1) << "|";
}

void ShowTotalBalancesScreen()
{
	std::vector <sClients> vAllClients = LoadClientsDataFromFile(ClientsFileName);
	int NumberOfClients = vAllClients.size();
	double TotalBalances = 0;

	std::cout << "\n\t\t\t\t\tBalances List (" << NumberOfClients << ") Client(s)\n";

	std::cout << "-------------------------------------------------------------------------------------------------------------\n";
	std::cout << "| " << std::left << std::setw(15) << "Account Number";
	std::cout << "| " << std::left << std::setw(58) << "Client Name";
	std::cout << "| " << std::left << std::setw(29) << "Account Balance" << std::left << std::setw(1) << "|";
	std::cout << "\n-------------------------------------------------------------------------------------------------------------\n";

	for (sClients& Client : vAllClients)
	{
		PrintClientRecordBalanceLine(Client);
		TotalBalances += Client.AccountBalance;
		std::cout << std::endl;
	}

	std::cout << "-------------------------------------------------------------------------------------------------------------\n";
	std::cout << "\n\t\t\t\t\t\t\t\tTotal Balances = " << TotalBalances;
}

void PerformTransactionsMenuOptions(enTransactionsMenuOptions TransactionMenuOption)
{
	switch (TransactionMenuOption)
	{
	case enTransactionsMenuOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		BackToTransactionsMenu();
		break;

	case enTransactionsMenuOptions::eWithdraw:
		system("cls");
		ShowWithdrawScreen();
		BackToTransactionsMenu();
		break;

	case enTransactionsMenuOptions::eTotalBalances:
		system("cls");
		ShowTotalBalancesScreen();
		BackToTransactionsMenu();
		break;

	case enTransactionsMenuOptions::eMainMenuTransactions:
		system("cls");
		ShowMainMenu();
		break;
	}
}

void ShowTransactionsMenu()
{
	std::cout << "============================================================\n";
	std::cout << "                    Transactions Menu\n";
	std::cout << "============================================================\n";
	std::cout << "\t[1] Deposit.\n";
	std::cout << "\t[2] Withdraw.\n";
	std::cout << "\t[3] Total Balances.\n";
	std::cout << "\t[4] Main Menu.\n";
	std::cout << "============================================================\n";
	PerformTransactionsMenuOptions((enTransactionsMenuOptions)ReadTransactionsMenuOption());
}

void ShowClientsListScreen()
{
	std::vector <sClients> vAllClients = LoadClientsDataFromFile(ClientsFileName);

	int NumberOfClients = vAllClients.size();

	std::cout << "\n\t\t\t\t\tClients List (" << NumberOfClients << ") Client(s)\n";

	std::cout << "-------------------------------------------------------------------------------------------------------------\n";
	std::cout << "| " << std::left << std::setw(15) << "Account Number";
	std::cout << "| " << std::left << std::setw(10) << "PIN Code";
	std::cout << "| " << std::left << std::setw(40) << "Client Name";
	std::cout << "| " << std::left << std::setw(15) << "Phone Number";
	std::cout << "| " << std::left << std::setw(18) << "Account Balance" << std::left << std::setw(1) << "|";
	std::cout << "\n-------------------------------------------------------------------------------------------------------------\n";

	if (NumberOfClients == 0)
		std::cout << "|\t\t\t\t\tNo Clients To Show In The System" << std::right << std::setw(37) << "|" << std::endl;

	else
		for (sClients& Client : vAllClients)
		{
			std::cout << "| " << std::left << std::setw(15) << Client.AccountNumber;
			std::cout << "| " << std::left << std::setw(10) << Client.PINCode;
			std::cout << "| " << std::left << std::setw(40) << Client.Name;
			std::cout << "| " << std::left << std::setw(15) << Client.Phone;
			std::cout << "| " << std::left << std::setw(18) << Client.AccountBalance << std::left << std::setw(1) << "|" << std::endl;
		}

	std::cout << "-------------------------------------------------------------------------------------------------------------\n";
}

void ShowAddClientsScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Add New Clients Screen";
	std::cout << "\n------------------------------------------------------------\n";

	AddNewClients();
}

void ShowDeleteClientScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Delete Clients Screen";
	std::cout << "\n------------------------------------------------------------\n";

	std::vector <sClients> vAllClients = LoadClientsDataFromFile(ClientsFileName);
	std::string AccountNumber = ReadAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vAllClients);
}

void ShowUpdateClientScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Update Client Screen";
	std::cout << "\n------------------------------------------------------------\n";

	std::vector <sClients> vAllClients = LoadClientsDataFromFile(ClientsFileName);
	std::string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vAllClients);
}

void ShowFindClientScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Find Client Screen";
	std::cout << "\n------------------------------------------------------------\n";

	sClients Client;
	std::vector <sClients> vAllClients = LoadClientsDataFromFile(ClientsFileName);
	std::string AccountNumber = ReadAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, Client, vAllClients))
		PrintClientCard(Client);
	else
		std::cout << "\n\nClient with account number [" << AccountNumber << "] is NOT found!";
}

void ShowEndScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Program Ends";
	std::cout << "\n------------------------------------------------------------\n";
}

void PerformMainMenuOptions(enMainMenuOptions MainMenuOption)
{
	switch (MainMenuOption)
	{
	case enMainMenuOptions::eClientsList:
		system("cls");

		if ((CurrentUser.Permissions & 1) || (CurrentUser.Permissions == -1))
			ShowClientsListScreen();
		else
			ShowAccessDeniedScreen();

		BackToMainMenu();
		break;

	case enMainMenuOptions::eAddClient:
		system("cls");

		if ((CurrentUser.Permissions & 2) || (CurrentUser.Permissions == -1))
			ShowAddClientsScreen();
		else
			ShowAccessDeniedScreen();

		BackToMainMenu();
		break;

	case enMainMenuOptions::eDeleteClient:
		system("cls");

		if ((CurrentUser.Permissions & 4) || (CurrentUser.Permissions == -1))
			ShowDeleteClientScreen();
		else
			ShowAccessDeniedScreen();

		BackToMainMenu();
		break;

	case enMainMenuOptions::eUpdateClient:
		system("cls");

		if ((CurrentUser.Permissions & 8) || (CurrentUser.Permissions == -1))
			ShowUpdateClientScreen();
		else
			ShowAccessDeniedScreen();

		BackToMainMenu();
		break;

	case enMainMenuOptions::eFindClient:
		system("cls");

		if ((CurrentUser.Permissions & 16) || (CurrentUser.Permissions == -1))
			ShowFindClientScreen();
		else
			ShowAccessDeniedScreen();

		BackToMainMenu();
		break;

	case enMainMenuOptions::eTransactions:
		system("cls");

		if ((CurrentUser.Permissions & 32) || (CurrentUser.Permissions == -1))
			ShowTransactionsMenu();
		else
		{
			ShowAccessDeniedScreen();
			BackToMainMenu();
		}

		break;

	case enMainMenuOptions::eManageUsers:
		system("cls");

		if ((CurrentUser.Permissions & 64) || (CurrentUser.Permissions == -1))
			ShowManageUsersScreen();
		else
		{
			ShowAccessDeniedScreen();
			BackToMainMenu();
		}

		break;

	case enMainMenuOptions::eLogout:
		system("cls");
		Login();
		break;
	}
}

void ShowMainMenu()
{
	std::cout << "============================================================\n";
	std::cout << "                    Main Menu Screen\n";
	std::cout << "============================================================\n";
	std::cout << "\t[1] Show Clients List.\n";
	std::cout << "\t[2] Add New Client.\n";
	std::cout << "\t[3] Delete Client.\n";
	std::cout << "\t[4] Update Client Info.\n";
	std::cout << "\t[5] Find Client.\n";
	std::cout << "\t[6] Transactions.\n";
	std::cout << "\t[7] Manage Users.\n";
	std::cout << "\t[8] Logout.\n";
	std::cout << "============================================================\n";
	PerformMainMenuOptions((enMainMenuOptions)ReadMainMenuOption());
}

void Login()
{
	sUsers User;
	bool IsFirstAttempt = true;

	do
	{
		std::cout << "\n------------------------------------------------------------\n";
		std::cout << "                    Login Screen";
		std::cout << "\n------------------------------------------------------------\n";

		if (!IsFirstAttempt)
			std::cout << "Invalide Username/Password!\n";

		User = ReadUserLoginInfo();

		IsFirstAttempt = false;

		system("cls");

	} while (!FindUserByUsernameAndPassword(User.Username, User.Password, CurrentUser));

	ShowMainMenu();
}

int main()
{
	Login();
	return 0;
}