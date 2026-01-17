# Bank System Extension 2 - User Management & Security

A comprehensive banking system project featuring multi-user access control, permission-based operations, and secure user authentication. This is Project 1 from the **Algorithms & Problem Solving Level 4** course by ProgrammingAdvices.

## 📋 Project Overview

This project extends the previous bank system by adding a complete user management system with role-based access control. It introduces a login system, user permissions, and administrative capabilities to manage both clients and system users.

## ✨ Key Features

### User Authentication & Authorization
- **Secure Login System**: Username and password authentication
- **Role-Based Permissions**: Granular control over user access rights
- **Session Management**: Current user tracking throughout the session

### Permission System
The system implements 7 distinct permissions:
1. **List Clients** - View all client records
2. **Add New Client** - Create new client accounts
3. **Delete Client** - Remove client records
4. **Update Client** - Modify client information
5. **Find Client** - Search for specific clients
6. **Transactions** - Access deposit/withdraw/balance operations
7. **Manage Users** - Full user management capabilities

### Main Functionalities
- **Client Management**: Complete CRUD operations for client records
- **Transaction Operations**: Deposit, withdrawal, and balance viewing
- **User Management**: Add, delete, update, find, and list system users
- **File-Based Storage**: Persistent data storage using text files
- **Access Control**: Permission checks before every sensitive operation

## 🎯 Implementation Approaches

This repository contains **two different implementations** of the same project:

### 1. My Implementation (`Bank2 Extension By Me.cpp`)
My personal approach to solving the project, featuring:
- Custom code organization and structure
- My own logic for permission handling
- Personal coding style and conventions
- Unique implementation of security checks

### 2. Instructor's Implementation (`Bank2 Extension By Dr.cpp`)
The official solution by **Mohammed Abu-Hadhoud** from ProgrammingAdvices, featuring:
- Professional-grade code structure
- Industry-standard patterns
- Clean code principles
- Optimized permission checking system using enums

## 🗂️ Project Structure

```
.
├── Bank2 Extension By Me.cpp          # My implementation
├── Bank2 Extension By Dr.cpp          # Instructor's implementation
├── Clients.txt                        # Client data storage
├── Users.txt                          # User credentials and permissions
└── README.md
```

## 🔐 Permission Levels

The system uses bitwise operations for permission management:

| Permission | Value | Description |
|------------|-------|-------------|
| All Access | -1 | Full system access |
| List Clients | 1 | View client list |
| Add Client | 2 | Create new clients |
| Delete Client | 4 | Remove clients |
| Update Client | 8 | Modify client data |
| Find Client | 16 | Search clients |
| Transactions | 32 | Financial operations |
| Manage Users | 64 | User administration |

Permissions can be combined (e.g., 3 = List + Add, 7 = List + Add + Delete)

## 💾 Data Storage Format

### Clients File Format
```
AccountNumber#//#PINCode#//#Name#//#Phone#//#Balance
```

### Users File Format
```
Username#//#Password#//#Permissions
```

## 🚀 Getting Started

### Prerequisites
- C++ compiler (GCC, Clang, or MSVC)
- C++11 or later standard

### Compilation
```bash
# Compile my implementation
g++ -o bank_me "Bank2 Extension By Me.cpp"

# Compile instructor's implementation
g++ -o bank_dr "Bank2 Extension By Dr.cpp"
```

### Running the Program
```bash
# Run my version
./bank_me

# Run instructor's version
./bank_dr
```

### Default Login Credentials
You'll need to create initial users in the `Users.txt` file:
```
Admin#//#1234#//#-1
```
(Username: Admin, Password: 1234, Permissions: Full Access)

## 🎓 Learning Outcomes

Through this project, I've gained experience in:
- **User Authentication**: Implementing secure login systems
- **Authorization & Permissions**: Role-based access control patterns
- **File I/O Operations**: Reading and writing structured data
- **Bitwise Operations**: Using bit flags for permission management
- **System Security**: Preventing unauthorized access to sensitive operations
- **Code Organization**: Separating concerns (Input/Processing/Output)
- **Data Validation**: Ensuring data integrity and preventing duplicates

## 🔄 Comparing Implementations

Both implementations achieve the same functionality but differ in:
- **Code Structure**: Organization and function placement
- **Variable Naming**: Different naming conventions
- **Permission Checking**: Different approaches to access control
- **Error Handling**: Various validation strategies
- **User Experience**: Minor differences in prompts and messages

Studying both versions provides insight into how the same problem can be solved in multiple valid ways.

## 📚 Course Context

This project is part of the **Algorithms & Problem Solving Level 4** course by ProgrammingAdvices, specifically:
- **Project Type**: Course Project 1
- **Focus**: User management and security in banking systems
- **Prerequisites**: Completion of Levels 1-3
- **Instructor**: Mohammed Abu-Hadhoud

## 🤝 Acknowledgments

- **ProgrammingAdvices** and **Mohammed Abu-Hadhoud** for the excellent course content and project requirements
- The comprehensive guidance that made this implementation possible

## 📄 License

This project is for educational purposes as part of the ProgrammingAdvices course curriculum.

## 🔗 Related Links

- **Course**: [Algorithms & Problem Solving Level 4](https://programmingadvices.com/p/08-algorithms-problem-solving-level-4)
- **ProgrammingAdvices YouTube**: [Programming Advices Channel](https://YouTube.com/c/ProgrammingAdvices)

---

**Note**: This project demonstrates both my personal problem-solving approach and the instructor's professional implementation, providing valuable learning through comparison and analysis of different coding strategies.
