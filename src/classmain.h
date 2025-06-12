#ifndef CLASSMAIN_H
#define CLASSMAIN_H
#include "header.h"

// Forward declarations to resolve circular dependencies
class Employee;
class User;
class Client;
class Project;

// Enum for user roles
enum UserRole
{
    ADMIN,
    MANAGER,
    VIEWER
};

// Simple date structure for deadlines and attendance
struct Date
{
    int year;
    int month;
    int day;

    // Custom comparison operator for std::map key
    bool operator<(const Date &other) const
    {
        if (year != other.year)
            return year < other.year;
        if (month != other.month)
            return month < other.month;
        return day < other.day;
    }

    string toString() const
    {
        return to_string(year) + "-" + (month < 10 ? "0" : "") + to_string(month) + "-" + (day < 10 ? "0" : "") + to_string(day);
    }
};

// Employee Class (Core Data)
class Employee
{
public:
    int id;
    string name;
    string department;
    string position;
    double salary;
    string hiringStatus;        // e.g., Applied, Hired, Active
    map<Date, bool> attendance; // Date -> Present/Absent
    double hoursWorked;
    double vacationDays;
    double sickDays;
    double otherLeaveDays;
    int assignedClientId;  // To link with Client
    int assignedProjectId; // To link with Project

    Employee(int id, const string &name, const string &department, const string &position, double salary)
        : id(id), name(name), department(department), position(position), salary(salary), hiringStatus("Applied"), hoursWorked(0), vacationDays(0), sickDays(0), otherLeaveDays(0), assignedClientId(-1), assignedProjectId(-1) {}

    void display() const
    {
        cout << "ID: " << id << ", Name: " << name << ", Dept: " << department << ", Position: " << position << ", Salary: $" << fixed << setprecision(2) << salary << ", Status: " << hiringStatus << endl;
    }
};

// User Class (Core Data)
class User
{
public:
    string username;
    string password;
    UserRole role;

    User(const string &username, const string &password, UserRole role)
        : username(username), password(password), role(role) {}

    // Methods to check permissions
    bool canAdd() const { return role == ADMIN; }
    bool canUpdate() const { return role == ADMIN || role == MANAGER; }
    bool canDelete() const { return role == ADMIN; }
    bool canView() const { return role == ADMIN || role == MANAGER || role == VIEWER; }
};

// Client Class (Core Data)
class Client
{
public:
    int id;
    string name;
    string contactPerson;
    string contactEmail;

    Client(int id, const string &name, const string &contactPerson, const string &contactEmail)
        : id(id), name(name), contactPerson(contactPerson), contactEmail(contactEmail) {}

    void display() const
    {
        cout << "Client ID: " << id << ", Name: " << name << ", Contact: " << contactPerson << " (" << contactEmail << ")" << endl;
    }
};

// Project Class (Core Data)
class Project
{
public:
    int id;
    string name;
    string description;
    Date deadline;
    int clientId; // Link to a client

    Project(int id, const string &name, const string &description, const Date &deadline, int clientId)
        : id(id), name(name), description(description), deadline(deadline), clientId(clientId) {}

    void display() const
    {
        cout << "Project ID: " << id << ", Name: " << name << ", Deadline: " << deadline.toString() << ", Description: " << description << endl;
    }
};

// --- FEATURE CLASSES ---

// 1. Login System
class LoginSystem
{
private:
    vector<User> &users; // Reference to the main users vector
    User *&currentUser;  // Reference to the main current user pointer

public:
    LoginSystem(vector<User> &allUsers, User *&activeUser) : users(allUsers), currentUser(activeUser)
    {
        // Initial hardcoded users (moved from WorkerManagementSystem constructor)
        if (users.empty())
        {
            users.emplace_back("admin", "admin123", ADMIN);
            users.emplace_back("manager", "manager123", MANAGER);
            users.emplace_back("viewer", "viewer123", VIEWER);
        }
    }

    void login()
    {
        string username, password;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        for (User &user : users)
        {
            if (user.username == username && user.password == password)
            {
                currentUser = &user;
                cout << "Login successful. Welcome, " << currentUser->username << "!" << endl;
                cout << "Role: ";
                switch (currentUser->role)
                {
                case ADMIN:
                    cout << "Admin";
                    break;
                case MANAGER:
                    cout << "Manager";
                    break;
                case VIEWER:
                    cout << "Viewer";
                    break;
                }
                cout << endl;
                cout << "Available Actions: ";
                if (currentUser->canAdd())
                    cout << "Add, ";
                if (currentUser->canUpdate())
                    cout << "Update, ";
                if (currentUser->canDelete())
                    cout << "Delete, ";
                if (currentUser->canView())
                    cout << "View";
                cout << endl;
                return;
            }
        }
        cout << "Login failed. Invalid credentials." << endl;
        currentUser = nullptr;
    }

    void logout()
    {
        currentUser = nullptr;
        cout << "Logged out successfully." << endl;
    }

    void addUser(User *currentLoggedInUser)
    {
        if (!currentLoggedInUser || currentLoggedInUser->role != ADMIN)
        {
            cout << "Permission denied. Only Admins can add users." << endl;
            return;
        }
        string username, password, roleStr;
        cout << "Enter new username: ";
        cin >> username;
        cout << "Enter new password: ";
        cin >> password;
        cout << "Enter role (ADMIN, MANAGER, VIEWER): ";
        cin >> roleStr;

        UserRole newRole;
        if (roleStr == "ADMIN")
            newRole = ADMIN;
        else if (roleStr == "MANAGER")
            newRole = MANAGER;
        else if (roleStr == "VIEWER")
            newRole = VIEWER;
        else
        {
            cout << "Invalid role." << endl;
            return;
        }
        users.emplace_back(username, password, newRole);
        cout << "User " << username << " added successfully." << endl;
    }

    void manageUserRolesAndCredentials(User *currentLoggedInUser)
    {
        if (!currentLoggedInUser || currentLoggedInUser->role != ADMIN)
        {
            cout << "Permission denied. Only Admins can manage user roles and credentials." << endl;
            return;
        }
        string username;
        cout << "Enter username to modify: ";
        cin >> username;

        for (User &user : users)
        {
            if (user.username == username)
            {
                cout << "Enter new role (ADMIN, MANAGER, VIEWER) or 'nochange': ";
                string newRoleStr;
                cin >> newRoleStr;
                if (newRoleStr != "nochange")
                {
                    if (newRoleStr == "ADMIN")
                        user.role = ADMIN;
                    else if (newRoleStr == "MANAGER")
                        user.role = MANAGER;
                    else if (newRoleStr == "VIEWER")
                        user.role = VIEWER;
                    else
                    {
                        cout << "Invalid role." << endl;
                        return;
                    }
                }

                cout << "Enter new password or 'nochange': ";
                string newPassword;
                cin >> newPassword;
                if (newPassword != "nochange")
                {
                    user.password = newPassword;
                }
                cout << "User " << username << " updated successfully." << endl;
                return;
            }
        }
        cout << "User not found." << endl;
    }
};

// 2. Employee Management (Process Management + Base Employee Features)
class EmployeeManagement
{
private:
    vector<Employee> &employees; // Reference to the main employees vector
    int &nextEmployeeId;         // Reference to the global ID counter

public:
    EmployeeManagement(vector<Employee> &allEmployees, int &idCounter)
        : employees(allEmployees), nextEmployeeId(idCounter) {}

    void addEmployee(User *currentUser)
    {
        if (!currentUser || !currentUser->canAdd())
        {
            cout << "Permission denied. Only Admins can add employees." << endl;
            return;
        }
        string name, department, position;
        double salary;
        cout << "Enter Name: ";
        cin.ignore(); // Clear buffer
        getline(cin, name);
        cout << "Enter Department: ";
        getline(cin, department);
        cout << "Enter Position: ";
        getline(cin, position);
        cout << "Enter Salary: ";
        cin >> salary;

        employees.emplace_back(nextEmployeeId++, name, department, position, salary);
        cout << "Employee added successfully. ID: " << employees.back().id << endl;
    }

    void updateEmployeeDetails(User *currentUser)
    {
        if (!currentUser || !currentUser->canUpdate())
        {
            cout << "Permission denied. Only Admins and Managers can update employee details." << endl;
            return;
        }
        int id;
        cout << "Enter employee ID to update: ";
        cin >> id;

        for (Employee &emp : employees)
        {
            if (emp.id == id)
            {
                cout << "Enter new Name (or 'nochange'): ";
                string newValue;
                cin.ignore();
                getline(cin, newValue);
                if (newValue != "nochange")
                    emp.name = newValue;

                cout << "Enter new Department (or 'nochange'): ";
                getline(cin, newValue);
                if (newValue != "nochange")
                    emp.department = newValue;

                cout << "Enter new Position (or 'nochange'): ";
                getline(cin, newValue);
                if (newValue != "nochange")
                    emp.position = newValue;

                cout << "Enter new Salary (or '0' for nochange'): ";
                double newSalary;
                cin >> newSalary;
                if (newSalary != 0)
                    emp.salary = newSalary;

                cout << "Employee details updated successfully." << endl;
                return;
            }
        }
        cout << "Employee not found." << endl;
    }

    void deleteEmployeeRecord(User *currentUser)
    {
        if (!currentUser || !currentUser->canDelete())
        {
            cout << "Permission denied. Only Admins can delete employee records." << endl;
            return;
        }
        int id;
        cout << "Enter employee ID to delete: ";
        cin >> id;

        auto it = remove_if(employees.begin(), employees.end(), [id](const Employee &emp)
                            { return emp.id == id; });

        if (it != employees.end())
        {
            employees.erase(it, employees.end());
            cout << "Employee record deleted successfully." << endl;
        }
        else
        {
            cout << "Employee not found." << endl;
        }
    }

    void setHiringStatus(User *currentUser)
    {
        if (!currentUser || !currentUser->canUpdate())
        {
            cout << "Permission denied. Only Admins and Managers can set hiring status." << endl;
            return;
        }
        int id;
        cout << "Enter employee ID to set hiring status: ";
        cin >> id;

        for (Employee &emp : employees)
        {
            if (emp.id == id)
            {
                cout << "Enter new hiring status (e.g., Applied, Hired, Active): ";
                string status;
                cin >> status;
                emp.hiringStatus = status;
                cout << "Hiring status updated successfully." << endl;
                return;
            }
        }
        cout << "Employee not found." << endl;
    }

    void displayAllEmployees(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        cout << "\nAll Employees:" << endl;
        if (employees.empty())
        {
            cout << "No employees in the system." << endl;
            return;
        }

        // Define column widths for better formatting (adjust as needed)
        const int idWidth = 4;
        const int nameWidth = 20;
        const int deptWidth = 15;
        const int posWidth = 20;
        const int salaryWidth = 10;
        const int statusWidth = 15;

        // Print table header
        cout << "|" << left << setw(idWidth) << "ID"
             << "|" << left << setw(nameWidth) << "Name"
             << "|" << left << setw(deptWidth) << "Dept"
             << "|" << left << setw(posWidth) << "Position"
             << "|" << left << setw(salaryWidth) << "Salary"
             << "|" << left << setw(statusWidth) << "Status"
             << "|" << endl;

        // Print separator line
        cout << "|" << string(idWidth, '-')
             << "|" << string(nameWidth, '-')
             << "|" << string(deptWidth, '-')
             << "|" << string(posWidth, '-')
             << "|" << string(salaryWidth, '-')
             << "|" << string(statusWidth, '-')
             << "|" << endl;

        // Print employee data rows
        for (const auto &emp : employees)
        {
            cout << "|" << left << setw(idWidth) << emp.id
                 << "|" << left << setw(nameWidth) << emp.name
                 << "|" << left << setw(deptWidth) << emp.department
                 << "|" << left << setw(posWidth) << emp.position
                 << "|" << left << setw(salaryWidth) << fixed << setprecision(2) << emp.salary
                 << "|" << left << setw(statusWidth) << emp.hiringStatus
                 << "|" << endl;
        }
    }

    void displayOneEmployeeByID(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        int id;
        cout << "Enter employee ID: ";
        cin >> id;

        for (const auto &emp : employees)
        {
            if (emp.id == id)
            {
                cout << "\nEmployee Details:" << endl;
                emp.display(); // Uses the original Employee::display()
                return;
            }
        }
        cout << "Employee not found." << endl;
    }

    void searchEmployees(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        cout << "Search by Name, ID, or Department: ";
        string query;
        cin.ignore(); // Clear buffer
        getline(cin, query);

        cout << "\nFound Employees:" << endl;
        bool found = false;

        // Print table header for search results
        const int idWidth = 4;
        const int nameWidth = 20;
        const int deptWidth = 15;
        const int statusWidth = 15;
        const int positionWidth = 20; // Added position for completeness

        cout << "|" << left << setw(idWidth) << "ID"
             << "|" << left << setw(nameWidth) << "Name"
             << "|" << left << setw(deptWidth) << "Dept"
             << "|" << left << setw(positionWidth) << "Position"
             << "|" << left << setw(statusWidth) << "Status"
             << "|" << endl;
        cout << "|" << string(idWidth, '-')
             << "|" << string(nameWidth, '-')
             << "|" << string(deptWidth, '-')
             << "|" << string(positionWidth, '-')
             << "|" << string(statusWidth, '-')
             << "|" << endl;

        for (const auto &emp : employees)
        {
            // Check if name contains query, ID matches query, or department contains query
            if (emp.name.find(query) != string::npos ||
                to_string(emp.id) == query ||
                emp.department.find(query) != string::npos)
            {

                cout << "|" << left << setw(idWidth) << emp.id
                     << "|" << left << setw(nameWidth) << emp.name
                     << "|" << left << setw(deptWidth) << emp.department
                     << "|" << left << setw(positionWidth) << emp.position
                     << "|" << left << setw(statusWidth) << emp.hiringStatus
                     << "|" << endl;
                found = true;
            }
        }
        if (!found)
        {
            cout << "No employees found matching your query." << endl;
        }
    }
};

// 3. Resource Management
class ResourceManagement
{
private:
    vector<Employee> &employees; // Reference to the main employees vector

public:
    ResourceManagement(vector<Employee> &allEmployees) : employees(allEmployees) {}

    void assignEmployeeToDepartment(User *currentUser)
    {
        if (!currentUser || !currentUser->canUpdate())
        {
            cout << "Permission denied." << endl;
            return;
        }
        int empId;
        string newDept;
        cout << "Enter employee ID: ";
        cin >> empId;
        cout << "Enter new department: ";
        cin.ignore();
        getline(cin, newDept);

        for (Employee &emp : employees)
        {
            if (emp.id == empId)
            {
                emp.department = newDept;
                cout << "Employee " << empId << " assigned to " << newDept << " successfully." << endl;
                return;
            }
        }
        cout << "Employee not found." << endl;
    }

    void viewResourceAllocationPerDepartment(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        map<string, int> departmentCounts;
        for (const auto &emp : employees)
        {
            departmentCounts[emp.department]++;
        }

        cout << "\nResource Allocation per Department:" << endl;
        if (departmentCounts.empty())
        {
            cout << "No departments to display." << endl;
            return;
        }

        const int deptNameWidth = 20;
        const int countWidth = 10;

        cout << "|" << left << setw(deptNameWidth) << "Department"
             << "|" << left << setw(countWidth) << "Employees"
             << "|" << endl;
        cout << "|" << string(deptNameWidth, '-')
             << "|" << string(countWidth, '-')
             << "|" << endl;

        for (const auto &pair : departmentCounts)
        {
            cout << "|" << left << setw(deptNameWidth) << pair.first
                 << "|" << left << setw(countWidth) << pair.second
                 << "|" << endl;
        }
    }

    void reassignEmployeesBetweenDepartments(User *currentUser)
    {
        if (!currentUser || !currentUser->canUpdate())
        {
            cout << "Permission denied." << endl;
            return;
        }
        int empId;
        string newDept; // No need for oldDept in input, just find by ID and change
        cout << "Enter employee ID to reassign: ";
        cin >> empId;
        cout << "Enter new department: ";
        cin.ignore();
        getline(cin, newDept);

        for (Employee &emp : employees)
        {
            if (emp.id == empId)
            {
                emp.department = newDept;
                cout << "Employee " << empId << " reassigned to " << newDept << " successfully." << endl;
                return;
            }
        }
        cout << "Employee not found." << endl;
    }

    void viewPositionRoleDistribution(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        map<string, int> positionCounts;
        for (const auto &emp : employees)
        {
            positionCounts[emp.position]++;
        }

        cout << "\nPosition/Role Distribution:" << endl;
        if (positionCounts.empty())
        {
            cout << "No positions to display." << endl;
            return;
        }

        const int posNameWidth = 25;
        const int countWidth = 10;

        cout << "|" << left << setw(posNameWidth) << "Position/Role"
             << "|" << left << setw(countWidth) << "Employees"
             << "|" << endl;
        cout << "|" << string(posNameWidth, '-')
             << "|" << string(countWidth, '-')
             << "|" << endl;

        for (const auto &pair : positionCounts)
        {
            cout << "|" << left << setw(posNameWidth) << pair.first
                 << "|" << left << setw(countWidth) << pair.second
                 << "|" << endl;
        }
    }
};

// 4. Time Management
class TimeManagement
{
private:
    vector<Employee> &employees; // Reference to the main employees vector

public:
    TimeManagement(vector<Employee> &allEmployees) : employees(allEmployees) {}

    void recordEmployeeAttendance(User *currentUser)
    {
        if (!currentUser || !currentUser->canUpdate())
        {
            cout << "Permission denied." << endl;
            return;
        }
        int empId;
        int year, month, day;
        char presentChar;
        bool present;

        cout << "Enter employee ID: ";
        cin >> empId;

        for (Employee &emp : employees)
        {
            if (emp.id == empId)
            {
                cout << "Enter date (YYYY MM DD): ";
                cin >> year >> month >> day;
                cout << "Is employee present? (y/n): ";
                cin >> presentChar;
                present = (presentChar == 'y' || presentChar == 'Y');
                emp.attendance[{year, month, day}] = present;
                cout << "Attendance recorded for " << emp.name << " on " << Date{year, month, day}.toString() << ": " << (present ? "Present" : "Absent") << endl;
                return;
            }
        }
        cout << "Employee not found." << endl;
    }

    void trackWorkHoursOrShifts(User *currentUser)
    {
        if (!currentUser || !currentUser->canUpdate())
        {
            cout << "Permission denied." << endl;
            return;
        }
        int empId;
        double hours;
        cout << "Enter employee ID: ";
        cin >> empId;
        cout << "Enter hours worked/assigned for a shift: ";
        cin >> hours;

        for (Employee &emp : employees)
        {
            if (emp.id == empId)
            {
                emp.hoursWorked += hours;
                cout << "Work hours updated for " << emp.name << ". Total: " << emp.hoursWorked << endl;
                return;
            }
        }
        cout << "Employee not found." << endl;
    }

    void manageLeaveBalances(User *currentUser)
    {
        if (!currentUser || !currentUser->canUpdate())
        {
            cout << "Permission denied." << endl;
            return;
        }
        int empId;
        string leaveType;
        double days;
        cout << "Enter employee ID: ";
        cin >> empId;
        cout << "Enter leave type (vacation, sick, other): ";
        cin >> leaveType;
        cout << "Enter number of days: ";
        cin >> days;

        for (Employee &emp : employees)
        {
            if (emp.id == empId)
            {
                if (leaveType == "vacation")
                    emp.vacationDays += days;
                else if (leaveType == "sick")
                    emp.sickDays += days;
                else if (leaveType == "other")
                    emp.otherLeaveDays += days;
                else
                {
                    cout << "Invalid leave type." << endl;
                    return;
                }
                cout << "Leave balance updated for " << emp.name << ". " << leaveType << " days: " << days << endl;
                return;
            }
        }
        cout << "Employee not found." << endl;
    }
};

// 5. Client Relationship Management
class ClientRelationshipManagement
{
private:
    vector<Client> &clients;     // Reference to the main clients vector
    vector<Employee> &employees; // Reference to the main employees vector
    vector<Project> &projects;   // Reference to the main projects vector
    int &nextClientId;           // Reference to the global client ID counter

public:
    ClientRelationshipManagement(vector<Client> &allClients, vector<Employee> &allEmployees, vector<Project> &allProjects, int &idCounter)
        : clients(allClients), employees(allEmployees), projects(allProjects), nextClientId(idCounter) {}

    void addClientRecord(User *currentUser)
    {
        if (!currentUser || !currentUser->canAdd())
        {
            cout << "Permission denied." << endl;
            return;
        }
        string name, contactPerson, contactEmail;
        cout << "Enter Client Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Contact Person: ";
        getline(cin, contactPerson);
        cout << "Enter Contact Email: ";
        getline(cin, contactEmail);

        clients.emplace_back(nextClientId++, name, contactPerson, contactEmail);
        cout << "Client record added successfully. ID: " << clients.back().id << endl;
    }

    void assignEmployeesToClientsAccounts(User *currentUser)
    {
        if (!currentUser || !currentUser->canUpdate())
        {
            cout << "Permission denied." << endl;
            return;
        }
        int empId, clientId;
        cout << "Enter Employee ID: ";
        cin >> empId;
        cout << "Enter Client ID to assign: ";
        cin >> clientId;

        bool empFound = false;
        bool clientFound = false;

        for (Employee &emp : employees)
        {
            if (emp.id == empId)
            {
                emp.assignedClientId = clientId;
                empFound = true;
                break;
            }
        }

        for (const Client &client : clients)
        {
            if (client.id == clientId)
            {
                clientFound = true;
                break;
            }
        }

        if (empFound && clientFound)
        {
            cout << "Employee " << empId << " assigned to Client " << clientId << " successfully." << endl;
        }
        else
        {
            cout << "Employee or Client not found." << endl;
        }
    }

    void trackClientSpecificProjectsOrContacts(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        int clientId;
        cout << "Enter Client ID to view projects/contacts: ";
        cin >> clientId;

        bool clientFound = false;
        for (const Client &client : clients)
        {
            if (client.id == clientId)
            {
                cout << "\nClient Details:" << endl;
                client.display(); // Displays individual client details
                clientFound = true;
                break;
            }
        }

        if (!clientFound)
        {
            cout << "Client not found." << endl;
            return;
        }

        cout << "\nProjects for Client " << clientId << ":" << endl;
        bool projectFoundForClient = false;

        // Collect projects for this client to display in a table
        vector<const Project *> clientProjects;
        for (const Project &proj : projects)
        {
            if (proj.clientId == clientId)
            {
                clientProjects.push_back(&proj);
                projectFoundForClient = true;
            }
        }

        if (!projectFoundForClient)
        {
            cout << "No projects found for this client." << endl;
        }
        else
        {
            const int projIdWidth = 8;
            const int projNameWidth = 25;
            const int deadlineWidth = 15;
            const int descWidth = 30;

            cout << "|" << left << setw(projIdWidth) << "Proj ID"
                 << "|" << left << setw(projNameWidth) << "Project Name"
                 << "|" << left << setw(deadlineWidth) << "Deadline"
                 << "|" << left << setw(descWidth) << "Description"
                 << "|" << endl;
            cout << "|" << string(projIdWidth, '-')
                 << "|" << string(projNameWidth, '-')
                 << "|" << string(deadlineWidth, '-')
                 << "|" << string(descWidth, '-')
                 << "|" << endl;

            for (const auto *proj : clientProjects)
            {
                cout << "|" << left << setw(projIdWidth) << proj->id
                     << "|" << left << setw(projNameWidth) << proj->name
                     << "|" << left << setw(deadlineWidth) << proj->deadline.toString()
                     << "|" << left << setw(descWidth) << proj->description
                     << "|" << endl;
            }
        }
    }

    void displayAllClients(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        cout << "\nAll Clients:" << endl;
        if (clients.empty())
        {
            cout << "No clients in the system." << endl;
            return;
        }

        const int clientIdWidth = 8;
        const int clientNameWidth = 25;
        const int contactPersonWidth = 20;
        const int contactEmailWidth = 30;

        cout << "|" << left << setw(clientIdWidth) << "Client ID"
             << "|" << left << setw(clientNameWidth) << "Client Name"
             << "|" << left << setw(contactPersonWidth) << "Contact Person"
             << "|" << left << setw(contactEmailWidth) << "Contact Email"
             << "|" << endl;
        cout << "|" << string(clientIdWidth, '-')
             << "|" << string(clientNameWidth, '-')
             << "|" << string(contactPersonWidth, '-')
             << "|" << string(contactEmailWidth, '-')
             << "|" << endl;

        for (const auto &client : clients)
        {
            cout << "|" << left << setw(clientIdWidth) << client.id
                 << "|" << left << setw(clientNameWidth) << client.name
                 << "|" << left << setw(contactPersonWidth) << client.contactPerson
                 << "|" << left << setw(contactEmailWidth) << client.contactEmail
                 << "|" << endl;
        }
    }
};

// 6. Project Management
class ProjectManagement
{
private:
    vector<Project> &projects;   // Reference to the main projects vector
    vector<Employee> &employees; // Reference to the main employees vector
    vector<Client> &clients;     // Reference to the main clients vector
    int &nextProjectId;          // Reference to the global project ID counter

public:
    ProjectManagement(vector<Project> &allProjects, vector<Employee> &allEmployees, vector<Client> &allClients, int &idCounter)
        : projects(allProjects), employees(allEmployees), clients(allClients), nextProjectId(idCounter) {}

    void createProject(User *currentUser)
    {
        if (!currentUser || !currentUser->canAdd())
        {
            cout << "Permission denied." << endl;
            return;
        }
        string name, description;
        int year, month, day;
        int clientId;

        cout << "Enter Project Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Description: ";
        getline(cin, description);
        cout << "Enter Deadline (YYYY MM DD): ";
        cin >> year >> month >> day;
        cout << "Enter Client ID for this project: ";
        cin >> clientId;

        bool clientExists = false;
        for (const auto &client : clients)
        {
            if (client.id == clientId)
            {
                clientExists = true;
                break;
            }
        }

        if (!clientExists)
        {
            cout << "Client with ID " << clientId << " not found. Project cannot be created." << endl;
            return;
        }

        projects.emplace_back(nextProjectId++, name, description, Date{year, month, day}, clientId);
        cout << "Project created successfully. ID: " << projects.back().id << endl;
    }

    void assignEmployeesToProjects(User *currentUser)
    {
        if (!currentUser || !currentUser->canUpdate())
        {
            cout << "Permission denied." << endl;
            return;
        }
        int empId, projId;
        cout << "Enter Employee ID: ";
        cin >> empId;
        cout << "Enter Project ID to assign: ";
        cin >> projId;

        bool empFound = false;
        bool projFound = false;

        for (Employee &emp : employees)
        {
            if (emp.id == empId)
            {
                emp.assignedProjectId = projId;
                empFound = true;
                break;
            }
        }

        for (const Project &proj : projects)
        {
            if (proj.id == projId)
            {
                projFound = true;
                break;
            }
        }

        if (empFound && projFound)
        {
            cout << "Employee " << empId << " assigned to Project " << projId << " successfully." << endl;
        }
        else
        {
            cout << "Employee or Project not found." << endl;
        }
    }

    void trackProjectDeadlines(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        cout << "\nProject Deadlines:" << endl;
        if (projects.empty())
        {
            cout << "No projects to display deadlines for." << endl;
            return;
        }

        const int projIdWidth = 8;
        const int projNameWidth = 25;
        const int deadlineWidth = 15;
        const int descWidth = 30;

        cout << "|" << left << setw(projIdWidth) << "Proj ID"
             << "|" << left << setw(projNameWidth) << "Project Name"
             << "|" << left << setw(deadlineWidth) << "Deadline"
             << "|" << left << setw(descWidth) << "Description"
             << "|" << endl;
        cout << "|" << string(projIdWidth, '-')
             << "|" << string(projNameWidth, '-')
             << "|" << string(deadlineWidth, '-')
             << "|" << string(descWidth, '-')
             << "|" << endl;

        for (const auto &proj : projects)
        {
            cout << "|" << left << setw(projIdWidth) << proj.id
                 << "|" << left << setw(projNameWidth) << proj.name
                 << "|" << left << setw(deadlineWidth) << proj.deadline.toString()
                 << "|" << left << setw(descWidth) << (proj.description.length() > descWidth ? proj.description.substr(0, descWidth - 3) + "..." : proj.description) // Truncate long descriptions
                 << "|" << endl;
        }
    }

    void viewEmployeesAssignedToProjects(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        int projId;
        cout << "Enter Project ID to view assigned employees: ";
        cin >> projId;

        bool projectFound = false;
        for (const auto &proj : projects)
        {
            if (proj.id == projId)
            {
                projectFound = true;
                cout << "\nEmployees assigned to Project " << proj.name << " (ID: " << proj.id << "):" << endl;

                vector<const Employee *> assignedEmployees;
                for (const auto &emp : employees)
                {
                    if (emp.assignedProjectId == projId)
                    {
                        assignedEmployees.push_back(&emp);
                    }
                }

                if (assignedEmployees.empty())
                {
                    cout << "No employees assigned to this project." << endl;
                }
                else
                {
                    const int idWidth = 4;
                    const int nameWidth = 20;
                    const int deptWidth = 15;
                    const int posWidth = 20;
                    const int statusWidth = 15;

                    cout << "|" << left << setw(idWidth) << "ID"
                         << "|" << left << setw(nameWidth) << "Name"
                         << "|" << left << setw(deptWidth) << "Dept"
                         << "|" << left << setw(posWidth) << "Position"
                         << "|" << left << setw(statusWidth) << "Status"
                         << "|" << endl;
                    cout << "|" << string(idWidth, '-')
                         << "|" << string(nameWidth, '-')
                         << "|" << string(deptWidth, '-')
                         << "|" << string(posWidth, '-')
                         << "|" << string(statusWidth, '-')
                         << "|" << endl;

                    for (const auto *emp : assignedEmployees)
                    {
                        cout << "|" << left << setw(idWidth) << emp->id
                             << "|" << left << setw(nameWidth) << emp->name
                             << "|" << left << setw(deptWidth) << emp->department
                             << "|" << left << setw(posWidth) << emp->position
                             << "|" << left << setw(statusWidth) << emp->hiringStatus
                             << "|" << endl;
                    }
                }
                return;
            }
        }
        if (!projectFound)
        {
            cout << "Project not found." << endl;
        }
    }
};

// 7. Business Intelligence
class BusinessIntelligence
{
private:
    vector<Employee> &employees; // Reference to the main employees vector

public:
    BusinessIntelligence(vector<Employee> &allEmployees) : employees(allEmployees) {}

    void countTotalEmployees(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        cout << "Total Employees: " << employees.size() << endl;
    }

    void departmentWiseEmployeeStatistics(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        map<string, int> departmentCounts;
        map<string, double> departmentSalaries;

        for (const auto &emp : employees)
        {
            departmentCounts[emp.department]++;
            departmentSalaries[emp.department] += emp.salary;
        }

        cout << "\nDepartment-wise Employee Statistics:" << endl;
        if (departmentCounts.empty())
        {
            cout << "No department data to display." << endl;
            return;
        }

        const int deptNameWidth = 20;
        const int countWidth = 10;
        const int avgSalaryWidth = 15;

        cout << "|" << left << setw(deptNameWidth) << "Department"
             << "|" << left << setw(countWidth) << "Count"
             << "|" << left << setw(avgSalaryWidth) << "Avg Salary"
             << "|" << endl;
        cout << "|" << string(deptNameWidth, '-')
             << "|" << string(countWidth, '-')
             << "|" << string(avgSalaryWidth, '-')
             << "|" << endl;

        for (const auto &pair : departmentCounts)
        {
            double avgSalary = (pair.second > 0) ? (departmentSalaries[pair.first] / pair.second) : 0.0;
            cout << "|" << left << setw(deptNameWidth) << pair.first
                 << "|" << left << setw(countWidth) << pair.second
                 << "|" << left << setw(avgSalaryWidth) << fixed << setprecision(2) << avgSalary
                 << "|" << endl;
        }
    }

    void calculateSalaryMetrics(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        if (employees.empty())
        {
            cout << "No employees to calculate salary metrics." << endl;
            return;
        }

        double totalSalary = 0;
        double minSalary = employees[0].salary;
        double maxSalary = employees[0].salary;

        for (const auto &emp : employees)
        {
            totalSalary += emp.salary;
            if (emp.salary < minSalary)
                minSalary = emp.salary;
            if (emp.salary > maxSalary)
                maxSalary = emp.salary;
        }

        cout << "\nSalary Metrics:" << endl;
        cout << "Average Salary: $" << fixed << setprecision(2) << (totalSalary / employees.size()) << endl;
        cout << "Max Salary: $" << fixed << setprecision(2) << maxSalary << endl;
        cout << "Min Salary: $" << fixed << setprecision(2) << minSalary << endl;
    }

    void sortEmployees(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            cout << "Permission denied." << endl;
            return;
        }
        cout << "Sort employees by (name, salary, department): ";
        string sortBy;
        cin >> sortBy;

        if (sortBy == "name")
        {
            sort(employees.begin(), employees.end(), [](const Employee &a, const Employee &b)
                 { return a.name < b.name; });
        }
        else if (sortBy == "salary")
        {
            sort(employees.begin(), employees.end(), [](const Employee &a, const Employee &b)
                 { return a.salary < b.salary; });
        }
        else if (sortBy == "department")
        {
            sort(employees.begin(), employees.end(), [](const Employee &a, const Employee &b)
                 { return a.department < b.department; });
        }
        else
        {
            cout << "Invalid sort option." << endl;
            return;
        }

        cout << "\nEmployees sorted by " << sortBy << ":" << endl;
        // Re-using EmployeeManagement::displayAllEmployees's table logic for sorted display
        // Define column widths for better formatting (adjust as needed)
        const int idWidth = 4;
        const int nameWidth = 20;
        const int deptWidth = 15;
        const int posWidth = 20;
        const int salaryWidth = 10;
        const int statusWidth = 15;

        // Print table header
        cout << "|" << left << setw(idWidth) << "ID"
             << "|" << left << setw(nameWidth) << "Name"
             << "|" << left << setw(deptWidth) << "Dept"
             << "|" << left << setw(posWidth) << "Position"
             << "|" << left << setw(salaryWidth) << "Salary"
             << "|" << left << setw(statusWidth) << "Status"
             << "|" << endl;

        // Print separator line
        cout << "|" << string(idWidth, '-')
             << "|" << string(nameWidth, '-')
             << "|" << string(deptWidth, '-')
             << "|" << string(posWidth, '-')
             << "|" << string(salaryWidth, '-')
             << "|" << string(statusWidth, '-')
             << "|" << endl;

        // Print employee data rows
        for (const auto &emp : employees)
        {
            cout << "|" << left << setw(idWidth) << emp.id
                 << "|" << left << setw(nameWidth) << emp.name
                 << "|" << left << setw(deptWidth) << emp.department
                 << "|" << left << setw(posWidth) << emp.position
                 << "|" << left << setw(salaryWidth) << fixed << setprecision(2) << emp.salary
                 << "|" << left << setw(statusWidth) << emp.hiringStatus
                 << "|" << endl;
        }
    }
};

// Main System Class - Orchestrates Feature Classes
class WorkerManagementSystem
{
private:
    // Main data storage vectors
    vector<Employee> employees;
    vector<User> users;
    vector<Client> clients;
    vector<Project> projects;

    User *currentUser; // Points to the currently logged-in user

    // ID counters
    int nextEmployeeId;
    int nextClientId;
    int nextProjectId;

    // Feature class instances
    LoginSystem loginSystem;
    EmployeeManagement employeeManagement;
    ResourceManagement resourceManagement;
    TimeManagement timeManagement;
    ClientRelationshipManagement clientRelationshipManagement;
    ProjectManagement projectManagement;
    BusinessIntelligence businessIntelligence;

public:
    WorkerManagementSystem() : currentUser(nullptr),
                               nextEmployeeId(1),
                               nextClientId(1),
                               nextProjectId(1),
                               loginSystem(users, currentUser), // Pass by reference
                               employeeManagement(employees, nextEmployeeId),
                               resourceManagement(employees),
                               timeManagement(employees),
                               clientRelationshipManagement(clients, employees, projects, nextClientId),
                               projectManagement(projects, employees, clients, nextProjectId),
                               businessIntelligence(employees)
    {
        // Attempt to load data on startup
        loadSystemDataFromFile("worker_data.txt");
    }

    // --- File Handling (Save/Load data to file) ---
    void saveSystemDataToFile(const string &filename)
    {
        ofstream outFile(filename);
        if (!outFile.is_open())
        {
            cout << "Error: Could not open file " << filename << " for saving." << endl;
            return;
        }

        // Save metadata (like next IDs)
        outFile << "NEXT_EMPLOYEE_ID:" << nextEmployeeId << endl;
        outFile << "NEXT_CLIENT_ID:" << nextClientId << endl;
        outFile << "NEXT_PROJECT_ID:" << nextProjectId << endl;

        // Save Employees (simple CSV-like format)
        outFile << "EMPLOYEES_START" << endl;
        for (const auto &emp : employees)
        {
            outFile << emp.id << ","
                    << emp.name << ","
                    << emp.department << ","
                    << emp.position << ","
                    << emp.salary << ","
                    << emp.hiringStatus << ","
                    << emp.hoursWorked << ","
                    << emp.vacationDays << ","
                    << emp.sickDays << ","
                    << emp.otherLeaveDays << ","
                    << emp.assignedClientId << ","
                    << emp.assignedProjectId;
            // For attendance, it's more complex; for simplicity, skipping for now or a very basic representation
            // You might serialize attendance as a JSON string or separate lines
            outFile << endl;
        }
        outFile << "EMPLOYEES_END" << endl;

        // Save Users
        outFile << "USERS_START" << endl;
        for (const auto &user : users)
        {
            outFile << user.username << ","
                    << user.password << ","
                    << user.role << endl; // Enum value will be integer
        }
        outFile << "USERS_END" << endl;

        // Save Clients
        outFile << "CLIENTS_START" << endl;
        for (const auto &client : clients)
        {
            outFile << client.id << ","
                    << client.name << ","
                    << client.contactPerson << ","
                    << client.contactEmail << endl;
        }
        outFile << "CLIENTS_END" << endl;

        // Save Projects
        outFile << "PROJECTS_START" << endl;
        for (const auto &proj : projects)
        {
            outFile << proj.id << ","
                    << proj.name << ","
                    << proj.description << ","
                    << proj.deadline.year << ","
                    << proj.deadline.month << ","
                    << proj.deadline.day << ","
                    << proj.clientId << endl;
        }
        outFile << "PROJECTS_END" << endl;

        outFile.close();
        cout << "System data saved to " << filename << " successfully." << endl;
    }

    void loadSystemDataFromFile(const string &filename)
    {
        ifstream inFile(filename);
        if (!inFile.is_open())
        {
            // cout << "No existing data file found (" << filename << "). Starting with empty system." << endl;
            return; // Not an error if file doesn't exist on first run
        }

        // Clear current data before loading
        employees.clear();
        users.clear();
        clients.clear();
        projects.clear();
        nextEmployeeId = 1;
        nextClientId = 1;
        nextProjectId = 1;
        currentUser = nullptr; // Reset current user

        string line;
        string currentSection = "";
        int maxLoadedEmployeeId = 0;
        int maxLoadedClientId = 0;
        int maxLoadedProjectId = 0;

        while (getline(inFile, line))
        {
            if (line == "EMPLOYEES_START")
            {
                currentSection = "EMPLOYEES";
                continue;
            }
            else if (line == "USERS_START")
            {
                currentSection = "USERS";
                continue;
            }
            else if (line == "CLIENTS_START")
            {
                currentSection = "CLIENTS";
                continue;
            }
            else if (line == "PROJECTS_START")
            {
                currentSection = "PROJECTS";
                continue;
            }
            else if (line.find("NEXT_EMPLOYEE_ID:") == 0)
            {
                nextEmployeeId = stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("NEXT_CLIENT_ID:") == 0)
            {
                nextClientId = stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("NEXT_PROJECT_ID:") == 0)
            {
                nextProjectId = stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("_END") != string::npos)
            {
                currentSection = ""; // End of a section
                continue;
            }

            if (currentSection == "EMPLOYEES")
            {
                stringstream ss(line);
                string segment;
                vector<string> seglist;
                while (getline(ss, segment, ','))
                {
                    seglist.push_back(segment);
                }
                if (seglist.size() >= 12)
                { // Ensure enough fields
                    Employee emp(stoi(seglist[0]), seglist[1], seglist[2], seglist[3], stod(seglist[4]));
                    emp.hiringStatus = seglist[5];
                    emp.hoursWorked = stod(seglist[6]);
                    emp.vacationDays = stod(seglist[7]);
                    emp.sickDays = stod(seglist[8]);
                    emp.otherLeaveDays = stod(seglist[9]);
                    emp.assignedClientId = stoi(seglist[10]);
                    emp.assignedProjectId = stoi(seglist[11]);
                    // Attendance is complex to load from this simple format, skipping for now
                    employees.push_back(emp);
                    if (emp.id >= maxLoadedEmployeeId)
                        maxLoadedEmployeeId = emp.id;
                }
            }
            else if (currentSection == "USERS")
            {
                stringstream ss(line);
                string username, password, roleStr;
                getline(ss, username, ',');
                getline(ss, password, ',');
                getline(ss, roleStr);
                users.emplace_back(username, password, static_cast<UserRole>(stoi(roleStr)));
            }
            else if (currentSection == "CLIENTS")
            {
                stringstream ss(line);
                string idStr, name, contactPerson, contactEmail;
                getline(ss, idStr, ',');
                getline(ss, name, ',');
                getline(ss, contactPerson, ',');
                getline(ss, contactEmail);
                clients.emplace_back(stoi(idStr), name, contactPerson, contactEmail);
                if (stoi(idStr) >= maxLoadedClientId)
                    maxLoadedClientId = stoi(idStr);
            }
            else if (currentSection == "PROJECTS")
            {
                stringstream ss(line);
                string idStr, name, description, yearStr, monthStr, dayStr, clientIdStr;
                getline(ss, idStr, ',');
                getline(ss, name, ',');
                getline(ss, description, ',');
                getline(ss, yearStr, ',');
                getline(ss, monthStr, ',');
                getline(ss, dayStr, ',');
                getline(ss, clientIdStr);
                projects.emplace_back(stoi(idStr), name, description, Date{stoi(yearStr), stoi(monthStr), stoi(dayStr)}, stoi(clientIdStr));
                if (stoi(idStr) >= maxLoadedProjectId)
                    maxLoadedProjectId = stoi(idStr);
            }
        }
        inFile.close();
        // Update next IDs if loaded data has higher IDs
        if (maxLoadedEmployeeId >= nextEmployeeId)
            nextEmployeeId = maxLoadedEmployeeId + 1;
        if (maxLoadedClientId >= nextClientId)
            nextClientId = maxLoadedClientId + 1;
        if (maxLoadedProjectId >= nextProjectId)
            nextProjectId = maxLoadedProjectId + 1;

        cout << "System data loaded from " << filename << " successfully." << endl;
    }

    void showMainMenu()
    {
        if (!currentUser)
        {
            cout << "\n--- Worker Management System ---" << endl;
            cout << "1. Login" << endl;
            cout << "2. Exit" << endl;
            cout << "Enter your choice: ";
            return;
        }
        system("cls");
        menuMain();
    }

    void processManagementMenu()
    {
        int choice;
        do
        {
            system("cls");
            printtHeader("Process Management");
            menuPM();
            cin >> choice;
            switch (choice)
            {
            case 1:
                system("cls");
                printtHeader("Add New Employee");
                employeeManagement.addEmployee(currentUser);
                pressEnter();
                break;
            case 2:
                system("cls");
                printtHeader("Update Employee Details");
                employeeManagement.updateEmployeeDetails(currentUser);
                pressEnter();
                break;
            case 3:
                system("cls");
                printtHeader("Delete Employee Record");
                employeeManagement.deleteEmployeeRecord(currentUser);
                pressEnter();
                break;
            case 4:
                system("cls");
                printtHeader("Set Hiring Status");
                employeeManagement.setHiringStatus(currentUser);
                pressEnter();
                break;
            case 5:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 5);
    }

    void resourceManagementMenu()
    {
        int choice;
        do
        {
            system("cls");
            printtHeader("Resource Management");
            menuRM();
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                system("cls");
                printHeaderStyle1("Assign Employee to Department");
                resourceManagement.assignEmployeeToDepartment(currentUser);
                pressEnter();
            }
            break;
            case 2:
                system("cls");
                printHeaderStyle1("View Resource Allocation per Department");
                resourceManagement.viewResourceAllocationPerDepartment(currentUser);
                pressEnter();
                break;
            case 3:
                system("cls");
                printHeaderStyle1("Reassign Employees between Departments");
                resourceManagement.reassignEmployeesBetweenDepartments(currentUser);
                pressEnter();
                break;
            case 4:
            {
                system("cls");
                printHeaderStyle1("View Position/Role Distribution");
                resourceManagement.viewPositionRoleDistribution(currentUser);
                pressEnter();
            }
            break;
            case 5:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 5);
    }

    void timeManagementMenu()
    {
        int choice;
        do
        {
            system("cls");
            printtHeader("Resource Management");
            menuTM();
            cin >> choice;

            switch (choice)
            {
            case 1:
                timeManagement.recordEmployeeAttendance(currentUser);
                break;
            case 2:
                timeManagement.trackWorkHoursOrShifts(currentUser);
                break;
            case 3:
                timeManagement.manageLeaveBalances(currentUser);
                break;
            case 4:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 4);
    }

    void clientRelationshipManagementMenu()
    {
        int choice;
        do
        {
            system("cls");
            printtHeader("Client Relationship Management");
            menuCRM();
            cin >> choice;
            switch (choice)
            {
            case 1:
                clientRelationshipManagement.addClientRecord(currentUser);
                break;
            case 2:
                clientRelationshipManagement.assignEmployeesToClientsAccounts(currentUser);
                break;
            case 3:
                clientRelationshipManagement.trackClientSpecificProjectsOrContacts(currentUser);
                break;
            case 4:
                clientRelationshipManagement.displayAllClients(currentUser);
                break; // New call
            case 5:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 5);
    }

    void projectManagementMenu()
    {
        int choice;
        do
        {
            system("cls");
            printtHeader("Project Management");
            menuPMM();
            cin >> choice;

            switch (choice)
            {
            case 1:
                projectManagement.createProject(currentUser);
                break;
            case 2:
                projectManagement.assignEmployeesToProjects(currentUser);
                break;
            case 3:
                projectManagement.trackProjectDeadlines(currentUser);
                break;
            case 4:
                projectManagement.viewEmployeesAssignedToProjects(currentUser);
                break;
            case 5:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 5);
    }

    void businessIntelligenceMenu()
    {
        int choice;
        do
        {
            system("cls");
            printtHeader("Business Intelligence");
            menuBI();
            cin >> choice;

            switch (choice)
            {
            case 1:
                businessIntelligence.countTotalEmployees(currentUser);
                break;
            case 2:
                businessIntelligence.departmentWiseEmployeeStatistics(currentUser);
                break;
            case 3:
                businessIntelligence.calculateSalaryMetrics(currentUser);
                break;
            case 4:
                businessIntelligence.sortEmployees(currentUser);
                break;
            case 5:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 5);
    }

    void baseSystemFeaturesMenu()
    {
        int choice;
        do
        {
            system("cls");
            printtHeader("Base System Features (Display/Search Employees)");
            menuBF();
            cin >> choice;

            switch (choice)
            {
            case 1:
                employeeManagement.displayAllEmployees(currentUser);
                break;
            case 2:
                employeeManagement.displayOneEmployeeByID(currentUser);
                break;
            case 3:
                employeeManagement.searchEmployees(currentUser);
                break;
            case 4:
                saveSystemDataToFile("worker_data.txt");
                break; // Call save
            case 5:
                loadSystemDataFromFile("worker_data.txt");
                break; // Call load
            case 6:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 6);
    }

    void userManagementMenu()
    {
        int choice;
        do
        {
            system("cls");
            printtHeader("User Management");
            menuUM();
            cin >> choice;

            switch (choice)
            {
            case 1:
                loginSystem.addUser(currentUser);
                break;
            case 2:
                loginSystem.manageUserRolesAndCredentials(currentUser);
                break;
            case 3:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 3);
    }

    void run()
    {
        int choice;
        do
        {

            showMainMenu();
            cin >> choice;
            if (!currentUser && choice != 1 && choice != 2)
            {
                cout << "Please login first." << endl;
                continue;
            }

            if (currentUser)
            {
                switch (choice)
                {
                case 1:
                    processManagementMenu();
                    break;
                case 2:

                    resourceManagementMenu();
                    break;
                case 3:

                    timeManagementMenu();
                    break;
                case 4:

                    clientRelationshipManagementMenu();
                    break;
                case 5:

                    projectManagementMenu();
                    break;
                case 6:

                    businessIntelligenceMenu();
                    break;
                case 7:

                    baseSystemFeaturesMenu();
                    break;
                case 8:
                    if (currentUser->role == ADMIN)
                    {

                        userManagementMenu();
                    }
                    else
                    {
                        cout << "Permission denied. Only Admins can access User Management." << endl;
                    }
                    break;
                case 9:
                    loginSystem.logout();
                    break; // Call logout from LoginSystem
                case 10:
                    cout << "Exiting Worker Management System. Goodbye!" << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            }
            else
            { // Not logged in
                switch (choice)
                {
                case 1:
                    loginSystem.login();
                    break; // Call login from LoginSystem
                case 2:
                    cout << "Exiting Worker Management System. Goodbye!" << endl;
                    break;
                default:
                    cout << "Invalid choice. Please login or exit." << endl;
                }
            }
        } while ((currentUser && choice != 10) || (!currentUser && choice != 2));
    }
};

#endif