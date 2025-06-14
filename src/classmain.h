#ifndef CLASSMAIN_H
#define CLASSMAIN_H
#include "header.h"
#include <string>  // For std::string
#include <sstream> // For std::ostringstream
#include <iomanip> // For std::fixed and std::setprecision

// Forward declarations to resolve circular dependencies
class Employee;
class User;
class Client;
class Project;
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(n) << a_value;
    return out.str();
}
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
        Table employee_details;
        employee_details.add_row({"Attribute", "Value"});
        employee_details[0].format().font_style({FontStyle::bold});
        employee_details.add_row({"ID", to_string(id)});
        employee_details.add_row({"Name", name});
        employee_details.add_row({"Department", department});
        employee_details.add_row({"Position", position});
        employee_details.add_row({"Salary", "$" + to_string_with_precision(salary)});
        employee_details.add_row({"Hiring Status", hiringStatus});
        employee_details.add_row({"Hours Worked", to_string_with_precision(hoursWorked, 1)});
        employee_details.add_row({"Vacation Days", to_string(vacationDays)});
        employee_details.add_row({"Sick Days", to_string(sickDays)});
        employee_details.add_row({"Other Leave", to_string(otherLeaveDays)});
        employee_details.add_row({"Assigned Client ID", (assignedClientId == -1 ? "N/A" : to_string(assignedClientId))});
        employee_details.add_row({"Assigned Project ID", (assignedProjectId == -1 ? "N/A" : to_string(assignedProjectId))});

        cout << employee_details << endl;
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
        Table client_details;
        client_details.add_row({"Attribute", "Value"});
        client_details[0].format().font_style({FontStyle::bold});
        client_details.add_row({"Client ID", to_string(id)});
        client_details.add_row({"Name", name});
        client_details.add_row({"Contact Person", contactPerson});
        client_details.add_row({"Contact Email", contactEmail});
        cout << client_details << endl;
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
        Table project_details;
        project_details.add_row({"Attribute", "Value"});
        project_details[0].format().font_style({FontStyle::bold});
        project_details.add_row({"Project ID", to_string(id)});
        project_details.add_row({"Name", name});
        project_details.add_row({"Description", description});
        project_details.add_row({"Deadline", deadline.toString()});
        project_details.add_row({"Client ID", to_string(clientId)});
        cout << project_details << endl;
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
        system("cls");
        printHeaderStyle3("---|Login System|---");
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
                cout << green(">> Login successful. Welcome, ") << currentUser->username << green("!") << endl;
                cout << endl;
                cout << bold_cyan("Role: ");
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
                cout << bold_cyan("Available Actions: ");
                if (currentUser->canAdd())
                    cout << "Add, ";
                if (currentUser->canUpdate())
                    cout << "Update, ";
                if (currentUser->canDelete())
                    cout << "Delete, ";
                if (currentUser->canView())
                    cout << "View";
                cout << endl;
                pressEnter();
                return;
            }
        }
        cout << red("Login failed. Invalid credentials.") << endl;
        currentUser = nullptr;
        pressEnter();
    }

    void logout()
    {
        currentUser = nullptr;
        cout << yellow("Logged out successfully.") << endl;
        pressEnter();
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

                cout << green("Employee details updated successfully.") << endl;
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

        Table table;
        table.add_row({"ID", "Name", "Department", "Position", "Salary", "Status"});
        table[0].format().font_style({FontStyle::bold});

        for (const auto &emp : employees)
        {
            table.add_row({to_string(emp.id), emp.name, emp.department, emp.position, to_string_with_precision(emp.salary), emp.hiringStatus});
        }
        cout << table << endl;
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
                emp.display();
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

        Table results;
        results.add_row({"ID", "Name", "Department", "Position", "Status"});
        results[0].format().font_style({FontStyle::bold});

        bool found = false;
        for (const auto &emp : employees)
        {
            if (emp.name.find(query) != string::npos ||
                to_string(emp.id) == query ||
                emp.department.find(query) != string::npos)
            {
                results.add_row({to_string(emp.id), emp.name, emp.department, emp.position, emp.hiringStatus});
                found = true;
            }
        }

        if (!found)
        {
            cout << "No employees found matching your query." << endl;
        }
        else
        {
            cout << results << endl;
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

        Table table;
        table.add_row({"Department", "Number of Employees"});
        table[0].format().font_style({FontStyle::bold});

        for (const auto &pair : departmentCounts)
        {
            table.add_row({pair.first, to_string(pair.second)});
        }
        cout << table << endl;
    }

    void reassignEmployeesBetweenDepartments(User *currentUser)
    {
        if (!currentUser || !currentUser->canUpdate())
        {
            cout << "Permission denied." << endl;
            return;
        }
        int empId;
        string newDept;
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

        Table table;
        table.add_row({"Position", "Number of Employees"});
        table[0].format().font_style({FontStyle::bold});

        for (const auto &pair : positionCounts)
        {
            table.add_row({pair.first, to_string(pair.second)});
        }
        cout << table << endl;
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

class ClientRelationshipManagement
{
private:
    std::vector<Client> &clients;
    std::vector<Employee> &employees;
    std::vector<Project> &projects;
    int &nextClientId;

public:
    ClientRelationshipManagement(std::vector<Client> &allClients,
                                 std::vector<Employee> &allEmployees,
                                 std::vector<Project> &allProjects,
                                 int &idCounter)
        : clients(allClients), employees(allEmployees), projects(allProjects), nextClientId(idCounter) {}

    void addClientRecord(User *currentUser)
    {
        if (!currentUser || !currentUser->canAdd())
        {
            std::cout << "Permission denied." << std::endl;
            return;
        }

        std::string name, contactPerson, contactEmail;
        std::cin.ignore();
        std::cout << "Enter Client Name: ";
        std::getline(std::cin, name);
        std::cout << "Enter Contact Person: ";
        std::getline(std::cin, contactPerson);
        std::cout << "Enter Contact Email: ";
        std::getline(std::cin, contactEmail);

        clients.emplace_back(nextClientId++, name, contactPerson, contactEmail);
        std::cout << "Client record added successfully. ID: " << clients.back().id << std::endl;
    }

    void assignEmployeesToClientsAccounts(User *currentUser)
    {
        if (!currentUser || !currentUser->canUpdate())
        {
            std::cout << "Permission denied." << std::endl;
            return;
        }

        int empId, clientId;
        std::cout << "Enter Employee ID: ";
        std::cin >> empId;
        std::cout << "Enter Client ID to assign: ";
        std::cin >> clientId;

        bool empFound = false, clientFound = false;
        for (auto &emp : employees)
        {
            if (emp.id == empId)
            {
                emp.assignedClientId = clientId;
                empFound = true;
                break;
            }
        }

        for (const auto &client : clients)
        {
            if (client.id == clientId)
            {
                clientFound = true;
                break;
            }
        }

        if (empFound && clientFound)
            std::cout << "Employee assigned successfully." << std::endl;
        else
            std::cout << "Employee or Client not found." << std::endl;
    }

    void displayAllClients(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            std::cout << "Permission denied." << std::endl;
            return;
        }

        if (clients.empty())
        {
            std::cout << "No clients found." << std::endl;
            return;
        }

        Table table;

        // Header row
        table.add_row({"Client ID", "Client Name", "Contact Person", "Contact Email"});
        table[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center).font_color(Color::cyan).border_bottom("─");

        // Client rows
        for (const auto &c : clients)
        {
            table.add_row({std::to_string(c.id),
                           c.name,
                           c.contactPerson,
                           c.contactEmail});
        }

        // Center-align all rows
        for (size_t i = 1; i < table.size(); ++i)
        {
            table[i].format().font_align(FontAlign::center);
        }

        std::cout << table << std::endl;
    }

    void trackClientSpecificProjectsOrContacts(User *currentUser)
    {
        if (!currentUser || !currentUser->canView())
        {
            std::cout << "Permission denied." << std::endl;
            return;
        }

        int clientId;
        std::cout << "Enter Client ID to view projects/contacts: ";
        std::cin >> clientId;

        bool clientFound = false;
        for (const auto &client : clients)
        {
            if (client.id == clientId)
            {
                std::cout << "\nClient Details:\n";
                client.display();
                clientFound = true;
                break;
            }
        }

        if (!clientFound)
        {
            std::cout << "Client not found." << std::endl;
            return;
        }

        std::cout << "\nProjects for Client " << clientId << ":\n";

        Table projects_table;
        projects_table.add_row({"Project ID", "Name", "Deadline", "Description"});
        projects_table[0].format().font_style({FontStyle::bold});

        bool hasProjects = false;
        for (const auto &proj : projects)
        {
            if (proj.clientId == clientId)
            {
                projects_table.add_row({to_string(proj.id), proj.name, proj.deadline.toString(), proj.description});
                hasProjects = true;
            }
        }

        if (!hasProjects)
        {
            std::cout << "No projects found for this client." << std::endl;
        }
        else
        {
            cout << projects_table << endl;
        }
    }

    void exportClientsToExcel(const std::string &filePath)
    {
        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        ws.title("Clients");

        ws.cell("A1").value("ID");
        ws.cell("B1").value("Name");
        ws.cell("C1").value("Contact Person");
        ws.cell("D1").value("Contact Email");

        int row = 2;
        for (const auto &client : clients)
        {
            ws.cell("A" + std::to_string(row)).value(client.id);
            ws.cell("B" + std::to_string(row)).value(client.name);
            ws.cell("C" + std::to_string(row)).value(client.contactPerson);
            ws.cell("D" + std::to_string(row)).value(client.contactEmail);
            ++row;
        }

        try
        {
            wb.save(filePath);
            std::cout << "Clients exported to Excel successfully to: " << filePath << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error exporting to Excel: " << e.what() << std::endl;
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

        Table table;
        table.add_row({"Project ID", "Name", "Deadline", "Description"});
        table[0].format().font_style({FontStyle::bold});

        for (const auto &proj : projects)
        {
            table.add_row({to_string(proj.id), proj.name, proj.deadline.toString(), proj.description});
        }
        cout << table << endl;
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

                Table table;
                table.add_row({"ID", "Name", "Department", "Position", "Status"});
                table[0].format().font_style({FontStyle::bold});
                bool assignedFound = false;

                for (const auto &emp : employees)
                {
                    if (emp.assignedProjectId == projId)
                    {
                        table.add_row({to_string(emp.id), emp.name, emp.department, emp.position, emp.hiringStatus});
                        assignedFound = true;
                    }
                }

                if (!assignedFound)
                {
                    cout << "No employees assigned to this project." << endl;
                }
                else
                {
                    cout << table << endl;
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

        Table table;
        table.add_row({"Department", "Employee Count", "Average Salary"});
        table[0].format().font_style({FontStyle::bold});

        for (const auto &pair : departmentCounts)
        {
            double avgSalary = (pair.second > 0) ? (departmentSalaries[pair.first] / pair.second) : 0.0;
            table.add_row({pair.first, to_string(pair.second), to_string_with_precision(avgSalary)});
        }
        cout << table << endl;
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

        Table table;
        table.add_row({"Metric", "Value"});
        table[0].format().font_style({FontStyle::bold});
        table.add_row({"Average Salary", "$" + to_string_with_precision(totalSalary / employees.size())});
        table.add_row({"Maximum Salary", "$" + to_string_with_precision(maxSalary)});
        table.add_row({"Minimum Salary", "$" + to_string_with_precision(minSalary)});
        cout << table << endl;
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
        displayAllEmployees(currentUser);
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

        Table table;
        table.add_row({"ID", "Name", "Department", "Position", "Salary", "Status"});
        table[0].format().font_style({FontStyle::bold});

        for (const auto &emp : employees)
        {
            table.add_row({to_string(emp.id), emp.name, emp.department, emp.position, to_string_with_precision(emp.salary), emp.hiringStatus});
        }
        cout << table << endl;
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
        loadSystemDataFromFile("worker_data.xlsx");
    }

    void saveSystemDataToFile(const string &filename)
    {
        xlnt::workbook wb;

        // Metadata Sheet
        xlnt::worksheet meta_ws = wb.active_sheet();
        meta_ws.title("Metadata");
        meta_ws.cell("A1").value("NEXT_EMPLOYEE_ID");
        meta_ws.cell("B1").value(nextEmployeeId);
        meta_ws.cell("A2").value("NEXT_CLIENT_ID");
        meta_ws.cell("B2").value(nextClientId);
        meta_ws.cell("A3").value("NEXT_PROJECT_ID");
        meta_ws.cell("B3").value(nextProjectId);

        // Employees Sheet
        xlnt::worksheet emp_ws = wb.create_sheet();
        emp_ws.title("Employees");
        emp_ws.cell("A1").value("ID");
        emp_ws.cell("B1").value("Name");
        emp_ws.cell("C1").value("Department");
        emp_ws.cell("D1").value("Position");
        emp_ws.cell("E1").value("Salary");
        emp_ws.cell("F1").value("HiringStatus");
        emp_ws.cell("G1").value("HoursWorked");
        emp_ws.cell("H1").value("VacationDays");
        emp_ws.cell("I1").value("SickDays");
        emp_ws.cell("J1").value("OtherLeaveDays");
        emp_ws.cell("K1").value("AssignedClientId");
        emp_ws.cell("L1").value("AssignedProjectId");
        int row = 2;
        for (const auto &emp : employees)
        {
            emp_ws.cell(1, row).value(emp.id);
            emp_ws.cell(2, row).value(emp.name);
            emp_ws.cell(3, row).value(emp.department);
            emp_ws.cell(4, row).value(emp.position);
            emp_ws.cell(5, row).value(emp.salary);
            emp_ws.cell(6, row).value(emp.hiringStatus);
            emp_ws.cell(7, row).value(emp.hoursWorked);
            emp_ws.cell(8, row).value(emp.vacationDays);
            emp_ws.cell(9, row).value(emp.sickDays);
            emp_ws.cell(10, row).value(emp.otherLeaveDays);
            emp_ws.cell(11, row).value(emp.assignedClientId);
            emp_ws.cell(12, row).value(emp.assignedProjectId);
            row++;
        }

        // Users Sheet
        xlnt::worksheet user_ws = wb.create_sheet();
        user_ws.title("Users");
        user_ws.cell("A1").value("Username");
        user_ws.cell("B1").value("Password");
        user_ws.cell("C1").value("Role");
        row = 2;
        for (const auto &user : users)
        {
            user_ws.cell(1, row).value(user.username);
            user_ws.cell(2, row).value(user.password);
            user_ws.cell(3, row).value(user.role);
            row++;
        }

        // Clients Sheet
        xlnt::worksheet client_ws = wb.create_sheet();
        client_ws.title("Clients");
        client_ws.cell("A1").value("ID");
        client_ws.cell("B1").value("Name");
        client_ws.cell("C1").value("ContactPerson");
        client_ws.cell("D1").value("ContactEmail");
        row = 2;
        for (const auto &client : clients)
        {
            client_ws.cell(1, row).value(client.id);
            client_ws.cell(2, row).value(client.name);
            client_ws.cell(3, row).value(client.contactPerson);
            client_ws.cell(4, row).value(client.contactEmail);
            row++;
        }

        // Projects Sheet
        xlnt::worksheet project_ws = wb.create_sheet();
        project_ws.title("Projects");
        project_ws.cell("A1").value("ID");
        project_ws.cell("B1").value("Name");
        project_ws.cell("C1").value("Description");
        project_ws.cell("D1").value("DeadlineYear");
        project_ws.cell("E1").value("DeadlineMonth");
        project_ws.cell("F1").value("DeadlineDay");
        project_ws.cell("G1").value("ClientId");
        row = 2;
        for (const auto &proj : projects)
        {
            project_ws.cell(1, row).value(proj.id);
            project_ws.cell(2, row).value(proj.name);
            project_ws.cell(3, row).value(proj.description);
            project_ws.cell(4, row).value(proj.deadline.year);
            project_ws.cell(5, row).value(proj.deadline.month);
            project_ws.cell(6, row).value(proj.deadline.day);
            project_ws.cell(7, row).value(proj.clientId);
            row++;
        }

        wb.save(filename);
        cout << "System data saved to " << filename << " successfully." << endl;
    }

    void loadSystemDataFromFile(const string &filename)
    {
        try
        {
            xlnt::workbook wb;
            wb.load(filename);

            employees.clear();
            users.clear();
            clients.clear();
            projects.clear();

            // Metadata
            auto meta_ws = wb.sheet_by_title("Metadata");
            nextEmployeeId = meta_ws.cell("B1").value<int>();
            nextClientId = meta_ws.cell("B2").value<int>();
            nextProjectId = meta_ws.cell("B3").value<int>();

            // Employees
            auto emp_ws = wb.sheet_by_title("Employees");
            for (auto row : emp_ws.rows(false))
            {
                if (row[0].to_string() == "ID")
                    continue; // Skip header
                Employee emp(row[0].value<int>(), row[1].to_string(), row[2].to_string(), row[3].to_string(), row[4].value<double>());
                emp.hiringStatus = row[5].to_string();
                emp.hoursWorked = row[6].value<double>();
                emp.vacationDays = row[7].value<double>();
                emp.sickDays = row[8].value<double>();
                emp.otherLeaveDays = row[9].value<double>();
                emp.assignedClientId = row[10].value<int>();
                emp.assignedProjectId = row[11].value<int>();
                employees.push_back(emp);
            }

            // Users
            auto user_ws = wb.sheet_by_title("Users");
            for (auto row : user_ws.rows(false))
            {
                if (row[0].to_string() == "Username")
                    continue; // Skip header
                users.emplace_back(row[0].to_string(), row[1].to_string(), static_cast<UserRole>(row[2].value<int>()));
            }

            // Clients
            auto client_ws = wb.sheet_by_title("Clients");
            for (auto row : client_ws.rows(false))
            {
                if (row[0].to_string() == "ID")
                    continue; // Skip header
                clients.emplace_back(row[0].value<int>(), row[1].to_string(), row[2].to_string(), row[3].to_string());
            }

            // Projects
            auto project_ws = wb.sheet_by_title("Projects");
            for (auto row : project_ws.rows(false))
            {
                if (row[0].to_string() == "ID")
                    continue; // Skip header
                projects.emplace_back(row[0].value<int>(), row[1].to_string(), row[2].to_string(), Date{row[3].value<int>(), row[4].value<int>(), row[5].value<int>()}, row[6].value<int>());
            }
            cout << "System data loaded from " << filename << " successfully." << endl;
        }
        catch (const xlnt::exception &e)
        {
            // It's okay if the file doesn't exist on first run
            // std::cerr << "Could not load " << filename << ": " << e.what() << std::endl;
        }
    }

    void showMainMenu()
    {
        if (!currentUser)
        {
            system("cls");
            printHeaderStyle3("---|Login System|---");
            menuLogin();
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
                system("cls");
                printHeaderStyle1("Record Employee Attendance");
                timeManagement.recordEmployeeAttendance(currentUser);
                pressEnter();
                break;
            case 2:
                printHeaderStyle1("Track Work Hours or Shifts");
                timeManagement.trackWorkHoursOrShifts(currentUser);
                pressEnter();
                break;
            case 3:
                printHeaderStyle1("Manage Leave Balances");
                timeManagement.manageLeaveBalances(currentUser);
                pressEnter();
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
                system("cls");
                printHeaderStyle1("Add Client Record");
                clientRelationshipManagement.addClientRecord(currentUser);
                pressEnter();
                break;
            case 2:
                system("cls");
                printHeaderStyle1("Assign Employees to Clients/Accounts");
                clientRelationshipManagement.assignEmployeesToClientsAccounts(currentUser);
                pressEnter();
                break;
            case 3:
                system("cls");
                printHeaderStyle1("Track Client-Specific Projects or Contacts");
                clientRelationshipManagement.trackClientSpecificProjectsOrContacts(currentUser);
                pressEnter();
                break;
            case 4:
                system("cls");
                printHeaderStyle1("View All Clients");
                clientRelationshipManagement.displayAllClients(currentUser);
                pressEnter();
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
                system("cls");
                printHeaderStyle1("Create Project");
                projectManagement.createProject(currentUser);
                pressEnter();
                break;
            case 2:
                system("cls");
                printHeaderStyle1("Assign Employees to Projects");
                projectManagement.assignEmployeesToProjects(currentUser);
                pressEnter();
                break;
            case 3:
                system("cls");
                printHeaderStyle1("Track Project Deadlines");
                projectManagement.trackProjectDeadlines(currentUser);
                pressEnter();
                break;
            case 4:
                system("cls");
                printHeaderStyle1("View Employees Assigned to Projects");
                projectManagement.viewEmployeesAssignedToProjects(currentUser);
                pressEnter();
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
                system("cls");
                printHeaderStyle1("Count Total Employees");
                businessIntelligence.countTotalEmployees(currentUser);
                pressEnter();
                break;
            case 2:
                system("cls");
                printHeaderStyle1("Department-wise Employee Statistics");
                businessIntelligence.departmentWiseEmployeeStatistics(currentUser);
                pressEnter();
                break;
            case 3:
                system("cls");
                printHeaderStyle1("Average, Max, and Min Salaries");
                businessIntelligence.calculateSalaryMetrics(currentUser);
                pressEnter();
                break;
            case 4:
                system("cls");
                printHeaderStyle1("Sort Employees");
                businessIntelligence.sortEmployees(currentUser);
                pressEnter();
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
                system("cls");
                printHeaderStyle1("Display All Employees");
                employeeManagement.displayAllEmployees(currentUser);
                pressEnter();
                break;
            case 2:
                system("cls");
                printHeaderStyle1("Display One Employee by ID");
                employeeManagement.displayOneEmployeeByID(currentUser);
                pressEnter();
                break;
            case 3:
                system("cls");
                printHeaderStyle1("Search Employees");
                employeeManagement.searchEmployees(currentUser);
                pressEnter();
                break;
            case 4:
                system("cls");
                printHeaderStyle1("Save System Data");
                saveSystemDataToFile("worker_data.xlsx");
                pressEnter();
                break; // Call save
            case 5:
                system("cls");
                printHeaderStyle1("Load System Data");
                loadSystemDataFromFile("worker_data.xlsx");
                pressEnter();
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
                system("cls");
                printHeaderStyle1("Add New User");
                loginSystem.addUser(currentUser);
                pressEnter();
                break;
            case 2:
                system("cls");
                printHeaderStyle1("Manage User Roles and Credentials");
                loginSystem.manageUserRolesAndCredentials(currentUser);
                pressEnter();
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