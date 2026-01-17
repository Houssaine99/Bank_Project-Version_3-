#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <vector>
#include <fstream>

void ShowMainMenu();
void ShowTransactionsMenu();
void ShowManageUsersMenu();
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

enum enMainMenuPermissions
{
	pAll = -1, 
	pListClient = 1,
	pAddNewClient = 2,
	pDeleteClient = 4,
	pUpdateClient = 8,
	pFindClient = 16,
	pTransactions = 32,
	pManageUsers = 64
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

std::string ClientsFileName = "Clients.txt";
std::string UsersFileName = "Users.txt";
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

sUsers ConvertLineToUsreRecord(std::string Line, std::string Seperator = "#//#")
{
	sUsers User;
	std::vector <std::string> vSplitedLine = SplitString(Line, Seperator);

	User.Username = vSplitedLine[0];
	User.Password = vSplitedLine[1];
	User.Permissions = std::stoi(vSplitedLine[2]);

	return User;
}

std::string ConvertClientRecordToLine(sClients Record, std::string Seperator = "#//#")
{
	std::string Line;

	Line += Record.AccountNumber + Seperator;
	Line += Record.PINCode + Seperator;
	Line += Record.Name + Seperator;
	Line += Record.Phone + Seperator;
	Line += std::to_string(Record.AccountBalance);

	return Line;

}

std::string ConvertUserRecordToLine(sUsers Record, std::string Seperator = "#//#")
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

std::vector <sUsers> LoadUsersDataFromFile(std::string FileName)
{
	std::fstream MyFile;
	std::vector <sUsers> vAllUsers;

	MyFile.open(FileName, std::ios::in);

	if (MyFile.is_open())
	{
		std::string Line;
		sUsers User;

		while (std::getline(MyFile, Line))
		{
			User = ConvertLineToUsreRecord(Line);
			vAllUsers.push_back(User);
		}

		MyFile.close();
	}

	return vAllUsers;
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
				Line = ConvertClientRecordToLine(Client);
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
			Line = ConvertUserRecordToLine(User);

			if (!User.MarkForDelete)
				MyFile << Line << std::endl;
		}
		MyFile.close();
	}
}

bool UserExistsByUsername(std::string Username, std::string FileName)
{
	std::fstream MyFile;
	MyFile.open(FileName, std::ios::in);

	if (MyFile.is_open())
	{
		std::string Line;
		sUsers User;

		while (std::getline(MyFile, Line))
		{
			User = ConvertLineToUsreRecord(Line);

			if (User.Username == Username)
			{
				MyFile.close();
				return true;
			}
		}

		MyFile.close();
	}

	return false;
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

short ReadPermissionsToSet()
{
	short Permissions = 0;
	char Answer = 'Y';

	std::cout << "\nDo you want to give full access? y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		return enMainMenuPermissions::pAll;

	std::cout << "\nDo you want to give access to:\n";

	std::cout << "\nShow Client List y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enMainMenuPermissions::pListClient;

	std::cout << "\nAdd New Client y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enMainMenuPermissions::pAddNewClient;

	std::cout << "\nDelete Client y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enMainMenuPermissions::pDeleteClient;

	std::cout << "\nUpdate Client y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enMainMenuPermissions::pUpdateClient;

	std::cout << "\nFind Client y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enMainMenuPermissions::pFindClient;

	std::cout << "\nTransactions y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enMainMenuPermissions::pTransactions;

	std::cout << "\nManage Users y/n? ";
	std::cin >> Answer;

	if (toupper(Answer) == 'Y')
		Permissions += enMainMenuPermissions::pManageUsers;

	return Permissions;
}

sUsers ReadNewUser()
{
	sUsers User;

	std::cout << "Enter Username? ";
	std::getline(std::cin >> std::ws, User.Username);

	while (UserExistsByUsername(User.Username, UsersFileName))
	{
		std::cout << "\nUser with username [" << User.Username << "] is already exist, Enter another username? ";
		std::getline(std::cin >> std::ws, User.Username);
	}

	std::cout << "Enter Password? ";
	std::getline(std::cin, User.Password);

	User.Permissions = ReadPermissionsToSet();

	return User;
}

sUsers ChangeUserRecord(std::string Username)
{
	sUsers User;
	User.Username = Username;

	std::cout << "\nEnter Password? ";
	std::getline(std::cin >> std::ws, User.Password);

	User.Permissions = ReadPermissionsToSet();

	return User;
}

// ----------------------------------> Processing <----------------------------------

void BackToMainMenu()
{
	std::cout << "\n\nPress any key to back to Main Menu...";
	system("pause>0");
	system("cls");
	ShowMainMenu();
}

void BackToTransactionsMenu()
{
	std::cout << "\n\nPress any key to back to Transactions Menu...";
	system("pause>0");
	system("cls");
	ShowTransactionsMenu();
}

void BackToManageUsersMenu()
{
	std::cout << "\n\nPress any key to back to Manage Users Menu...";
	system("pause>0");
	system("cls");
	ShowManageUsersMenu();
}

void AddNewClient()
{
	sClients Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertClientRecordToLine(Client));
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
	std::cout << "\nThe following are the User details:\n";
	std::cout << "---------------------------------------\n";
	std::cout << "Username   : " << User.Username << std::endl;
	std::cout << "Password   : " << User.Password << std::endl;
	std::cout << "Permissions: " << User.Permissions << std::endl;
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

void DeleteUserByUsername(std::string Username, std::vector <sUsers>& vAllUsers)
{
	sUsers User;
	char Answer = 'Y';

	if (FindUserByUsername(Username, User, vAllUsers))
	{
		PrintUserCard(User);

		std::cout << "\n\nAre you sure you want to delete this user (y/n)? ";
		std::cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkUserForDeleteByUsername(Username, vAllUsers);
			SaveUsersDataToFile(UsersFileName, vAllUsers);

			std::cout << "\nUser Deleted Successfully!";
		}
	}
	else
		std::cout << "\n\nUser with username [" << Username << "] is NOT found!";
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

void UpdateUserByUsername(std::string Username, std::vector <sUsers>& vAllUsers)
{
	sUsers User;
	char Answer = 'Y';

	if (FindUserByUsername(Username, User, vAllUsers))
	{
		PrintUserCard(User);

		std::cout << "\nAre you sure you want to update this User (y/n)? ";
		std::cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			for (sUsers& U : vAllUsers)
			{
				if (U.Username == Username)
				{
					U = ChangeUserRecord(Username);
					break;
				}
			}

			SaveUsersDataToFile(UsersFileName, vAllUsers);

			std::cout << "\nUser Updated Successfully!";
		}
	}
	else
		std::cout << "\n\nUser with username [" << Username << "] is NOT found!";
}

void AddDepositToAccountBalanceByAccountNumber(std::string AccountNumber, double Amount, std::vector <sClients>& vAllClients)
{
	char Answer = 'Y';

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

bool FindUserByUsernameAndPassword(std::string Username, std::string Password, sUsers& User)
{
	std::vector <sUsers> vAllUsers = LoadUsersDataFromFile(UsersFileName);

	for (sUsers& U : vAllUsers)
	{
		if (U.Username == Username && U.Password == Password)
		{
			User = U;
			return true;
		}
	}

	return false;
}

bool LoadUserInfo(std::string Username, std::string Password)
{
	if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
		return true;
	else
		return false;
}

void AddNewUser()
{
	sUsers User;
	User = ReadNewUser();
	AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers()
{
	char Answer = 'Y';

	do
	{
		std::cout << "Adding New User:\n\n";
		
		AddNewUser();

		std::cout << "\nUser Added Successfully, do you want to add more users (y/n)? ";
		std::cin >> Answer;

		std::cout << std::endl;

	} while (toupper(Answer) == 'Y');
}

bool CheckAccessPermission(enMainMenuPermissions Permission)
{
	if (CurrentUser.Permissions == enMainMenuPermissions::pAll)
		return true;

	if ((Permission & CurrentUser.Permissions) == Permission)
		return true;
	else
		return false;
}

// ----------------------------------> Output <----------------------------------

void ShowAccessDeniedMessage()
{
	std::cout << "-------------------------------------\n";
	std::cout << "Access Denied,\nYou don't have permission to do this,\nPlease contact your admin.\n";
	std::cout << "-------------------------------------\n";
}

void PrintUsersRecordLine(sUsers User)
{
	std::cout << "| " << std::left << std::setw(30) << User.Username;
	std::cout << "| " << std::left << std::setw(28) << User.Password;
	std::cout << "| " << std::left << std::setw(44) << User.Permissions << std::left << std::setw(1) << "|\n";
}

void ShowListUsersScreen()
{
	std::vector <sUsers> vAllUsers = LoadUsersDataFromFile(UsersFileName);

	int NumberOfClients = vAllUsers.size();

	std::cout << "\n\t\t\t\t\tUsers List (" << NumberOfClients << ") User(s)\n";

	std::cout << "-------------------------------------------------------------------------------------------------------------\n";
	std::cout << "| " << std::left << std::setw(30) << "Username";
	std::cout << "| " << std::left << std::setw(28) << "Password";
	std::cout << "| " << std::left << std::setw(44) << "Permission" << std::left << std::setw(1) << "|";
	std::cout << "\n-------------------------------------------------------------------------------------------------------------\n";

	for (sUsers& User : vAllUsers)
	{
		PrintUsersRecordLine(User);
	}
	std::cout << "-------------------------------------------------------------------------------------------------------------\n";
}

void ShowAddNewUsersScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Add New Users Screen";
	std::cout << "\n------------------------------------------------------------\n";

	AddNewUsers();
}

void ShowDeleteUserScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Delete User Screen";
	std::cout << "\n------------------------------------------------------------\n";

	std::vector <sUsers> vAllUsers = LoadUsersDataFromFile(UsersFileName);

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

	std::vector <sUsers> vAllUsers = LoadUsersDataFromFile(UsersFileName);

	std::string Username;
	std::cout << "Enter Username? ";
	std::getline(std::cin >> std::ws, Username);

	UpdateUserByUsername(Username, vAllUsers);
}

void ShowFindUserScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Find User Screen";
	std::cout << "\n------------------------------------------------------------\n";

	sUsers User;
	std::vector <sUsers> vAllUsers = LoadUsersDataFromFile(UsersFileName);

	std::string Username;
	std::cout << "Enter Username? ";
	std::getline(std::cin >> std::ws, Username);

	if (FindUserByUsername(Username, User, vAllUsers))
		PrintUserCard(User);
	else
		std::cout << "\nUser with username [" << Username << "] is NOT found!";
}

void PerformManageUsersMenuOptions(enManageUsersMenuOptions ManageUsersMenuOption)
{
	switch (ManageUsersMenuOption)
	{
	case enManageUsersMenuOptions::eListUsers:
		system("cls");
		ShowListUsersScreen();
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

void ShowManageUsersMenu()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		BackToMainMenu();
		return;
	}

	system("cls");
	std::cout << "============================================================\n";
	std::cout << "                    Manage Users Menu\n";
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
	double Amount = 0;
	std::vector <sClients> vAllClients = LoadClientsDataFromFile(ClientsFileName);
	std::string AccountNumber = ReadAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, Client, vAllClients))
	{
		std::cout << "\nClient with account number [" << AccountNumber << "] does not exist.\n\n";
		AccountNumber = ReadAccountNumber();
	}

	PrintClientCard(Client);

	std::cout << "\nEnter deposit amount? ";
	std::cin >> Amount;

	AddDepositToAccountBalanceByAccountNumber(AccountNumber, Amount, vAllClients);
}

void ShowWithdrawScreen()
{
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                   Withdraw Screen";
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

	std::cout << "\nEnter withdraw amount? ";
	std::cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		std::cout << "\n\nAmount exceeds balance, you can withdraw up to " << Client.AccountBalance << std::endl;
		std::cout << "\n\nEnter another amount? ";
		std::cin >> Amount;
	}

	AddDepositToAccountBalanceByAccountNumber(AccountNumber, -Amount, vAllClients);
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

void PerformTransactionsMenuOptions(enTransactionsMenuOptions TransactionsMenuOption)
{
	switch (TransactionsMenuOption)
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
	if (!CheckAccessPermission(enMainMenuPermissions::pTransactions))
	{
		ShowAccessDeniedMessage();
		BackToMainMenu();
		return;
	}

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
	if (!CheckAccessPermission(enMainMenuPermissions::pListClient))
	{
		ShowAccessDeniedMessage();
		return;
	}
	
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
	if (!CheckAccessPermission(enMainMenuPermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Add New Clients Screen";
	std::cout << "\n------------------------------------------------------------\n";

	AddNewClients();
}

void ShowDeleteClientScreen()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		return;
	}
	
	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Delete Clients Screen";
	std::cout << "\n------------------------------------------------------------\n";

	std::vector <sClients> vAllClients = LoadClientsDataFromFile(ClientsFileName);
	std::string AccountNumber = ReadAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vAllClients);
}

void ShowUpdateClientScreen()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pUpdateClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	std::cout << "\n------------------------------------------------------------\n";
	std::cout << "                    Update Client Screen";
	std::cout << "\n------------------------------------------------------------\n";

	std::vector <sClients> vAllClients = LoadClientsDataFromFile(ClientsFileName);
	std::string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vAllClients);
}

void ShowFindClientScreen()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

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

void PerformMainMenuOptions(enMainMenuOptions MainMenuOption)
{
	switch (MainMenuOption)
	{
	case enMainMenuOptions::eClientsList:
		system("cls");
		ShowClientsListScreen();
		BackToMainMenu();
		break;

	case enMainMenuOptions::eAddClient:
		system("cls");
		ShowAddClientsScreen();
		BackToMainMenu();
		break;

	case enMainMenuOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		BackToMainMenu();
		break;

	case enMainMenuOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		BackToMainMenu();
		break;

	case enMainMenuOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		BackToMainMenu();
		break;

	case enMainMenuOptions::eTransactions:
		system("cls");
		ShowTransactionsMenu();
		break;

	case enMainMenuOptions::eManageUsers:
		system("cls");
		ShowManageUsersMenu();
		break;

	case enMainMenuOptions::eLogout:
		system("cls");
		Login();
		break;
	}
}

void ShowMainMenu()
{
	system("cls");
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
	bool LoginFaild = false;
	std::string Username, Password;

	do
	{
		system("cls");

		std::cout << "\n------------------------------------------------------------\n";
		std::cout << "                    Login Screen";
		std::cout << "\n------------------------------------------------------------\n";

		if (LoginFaild)
			std::cout << "Invalid Username/Password!\n";

		std::cout << "Enter Username? ";
		std::getline(std::cin >> std::ws, Username);

		std::cout << "Enter Password? ";
		std::getline(std::cin, Password);

		LoginFaild = !LoadUserInfo(Username, Password);

	} while (LoginFaild);

	ShowMainMenu();
}

int main()
{
	Login();
	return 0;
}
