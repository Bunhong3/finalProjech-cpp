#ifndef MENUSTYLE_H
#define MENUSTYLE_H
#include "header.h"

using namespace std;

// functionEnter
void pressEnter()
{
    cout << bold_magenta(">> Press Enter to Coutinue : ");
    cin.ignore();
    cin.get();
}

// printHeader
void printtHeader(const string &title)
{
    string border = "+" + string(title.length() + 4, '=') + "+";
    cout << "\n\033[1;36m" << border << "\n";
    cout << "|  " << title << "  |\n";
    cout << border << "\033[0m\n\n";
}
void printHeaderStyle1(const string &title)
{
    string border = "+" + string(title.length() + 4, '=') + "+";
    cout << "\n\033[1;36m" << border << "\n";
    cout << "|  " << title << "  |\n";
    cout << border << "\033[0m\n\n";
}
void printHeaderStyle2(const string &title)
{
    string border = "*" + string(title.length() + 4, '*') + "*";
    cout << "\n\033[1;32m" << border << "\n";
    cout << "*  " << title << "  *\n";
    cout << border << "\033[0m\n\n";
}
void printHeaderStyle3(const string &title)
{
    string arrows = "<" + string(title.length() + 4, '-') + ">";
    cout << "\n\033[1;35m" << arrows << "\n";
    cout << "|  " << title << "  |\n";
    cout << arrows << "\033[0m\n\n";
}
void printHeaderStyle4(const string &title)
{
    string border = "╔" + string(title.length() + 4, '═') + "╗";
    string bottom = "╚" + string(title.length() + 4, '═') + "╝";
    cout << "\n\033[1;33m" << border << "\n";
    cout << "║  " << title << "  ║\n";
    cout << bottom << "\033[0m\n\n";
}



// >> main Menu
void menuMain()
{
    vector<string> menuMain = {
        "Process Management",
        "Resource Management",
        "Time Management",
        "Client Relationship Management",
        "Project Management",
        "Business Intelligence",
        "Base System Features (Display/Search Employees)",
        "User Management (Admin Only)",
        "Logout",
        "Exit"};
    Table t;
    t.add_row({"No", "Menu"});
    for (int i = 0; i < menuMain.size(); i++)
    {
        t.add_row({to_string(i + 1), menuMain[i]});
    }
    t[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center);
    t[0].format().font_color(Color::yellow);
    for (int i = 1; i <= menuMain.size(); i++)
    {
        if (i == 10) // Exit option
            t[i][1].format().font_color(Color::red);
        else
            t[i][1].format().font_color(Color::cyan);
    }
    cout << t << endl;
    cout << bold_blue(">> Enter choice: ");
}



// Process Management  
void menuPM()
{
    vector<string> menuMain = {
        "Add New Employee",
        "Update Employee Details",
        "Delete Employee Record",
        "Set Hiring Status",
        "Back to Main Menu"};
    Table t;
    t.add_row({"No", "Menu"});
    for (int i = 0; i < menuMain.size(); i++)
    {
        t.add_row({to_string(i + 1), menuMain[i]});
    }
    t[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center);
    t[0].format().font_color(Color::yellow);
    for (int i = 1; i <= menuMain.size(); i++)
    {
        if (i == 5) // Exit option
            t[i][1].format().font_color(Color::red);
        else
            t[i][1].format().font_color(Color::cyan);
    }
    cout << t << endl;
    cout << bold_blue(">> Enter choice: ");
}



// Resource Management
void menuRM()
{
    vector<string> menuMain = {
        "Assign Employee to Department",
        "View Resource Allocation per Department",
        "Reassign Employees between Departments",
        "View Position/Role Distribution",
        "Back to Main Menu"};
    Table t;
    t.add_row({"No", "Menu"});
    for (int i = 0; i < menuMain.size(); i++)
    {
        t.add_row({to_string(i + 1), menuMain[i]});
    }
    t[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center);
    t[0].format().font_color(Color::yellow);
    for (int i = 1; i <= menuMain.size(); i++)
    {
        if (i == 5) // Exit option
            t[i][1].format().font_color(Color::red);
        else
            t[i][1].format().font_color(Color::cyan);
    }
    cout << t << endl;
    cout << bold_blue(">> Enter choice: ");
}


// Time Management  
void menuTM()
{
    vector<string> menuMain = {
        "Record Employee Attendance",
        "Track Work Hours or Shifts",
        "Manage Leave Balances",
        "Back to Main Menu"};
    Table t;
    t.add_row({"No", "Menu"});
    for (int i = 0; i < menuMain.size(); i++)
    {
        t.add_row({to_string(i + 1), menuMain[i]});
    }
    t[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center);
    t[0].format().font_color(Color::yellow);
    for (int i = 1; i <= menuMain.size(); i++)
    {
        if (i == 4) // Exit option
            t[i][1].format().font_color(Color::red);
        else
            t[i][1].format().font_color(Color::cyan);
    }
    cout << t << endl;
    cout << bold_blue(">> Enter choice: ");
}



//  Client Relationship Management
void menuCRM()
{
    vector<string> menuMain = {
        "Add Client Record",
        "Assign Employees to Clients/Accounts",
        "Track Client-Specific Projects or Contacts",
        "View All Clients",
        "Back to Main Menu"};
    Table t;
    t.add_row({"No", "Menu"});
    for (int i = 0; i < menuMain.size(); i++)
    {
        t.add_row({to_string(i + 1), menuMain[i]});
    }
    t[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center);
    t[0].format().font_color(Color::yellow);
    for (int i = 1; i <= menuMain.size(); i++)
    {
        if (i == 5) // Exit option
            t[i][1].format().font_color(Color::red);
        else
            t[i][1].format().font_color(Color::cyan);
    }
    cout << t << endl;
    cout << bold_blue(">> Enter choice: ");
}



// Project Management 
void menuPMM()
{
    vector<string> menuMain = {
        "Create Project",
        "Assign Employees to Projects",
        "Track Project Deadlines",
        "View Employees Assigned to Projects",
        "Back to Main Menu"};
    Table t;
    t.add_row({"No", "Menu"});
    for (int i = 0; i < menuMain.size(); i++)
    {
        t.add_row({to_string(i + 1), menuMain[i]});
    }
    t[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center);
    t[0].format().font_color(Color::yellow);
    for (int i = 1; i <= menuMain.size(); i++)
    {
        if (i == 5) // Exit option
            t[i][1].format().font_color(Color::red);
        else
            t[i][1].format().font_color(Color::cyan);
    }
    cout << t << endl;
    cout << bold_blue(">> Enter choice: ");
}


// businessIntelligence
void menuBI()
{
    vector<string> menuMain = {
        "Count Total Employees",
        "Department-wise Employee Statistics",
        "Average, Max, and Min Salaries",
        "Sort Employees",
        "Back to Main Menu"};
    Table t;
    t.add_row({"No", "Menu"});
    for (int i = 0; i < menuMain.size(); i++)
    {
        t.add_row({to_string(i + 1), menuMain[i]});
    }
    t[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center);
    t[0].format().font_color(Color::yellow);
    for (int i = 1; i <= menuMain.size(); i++)
    {
        if (i == 5) // Exit option
            t[i][1].format().font_color(Color::red);
        else
            t[i][1].format().font_color(Color::cyan);
    }
    cout << t << endl;
    cout << bold_blue(">> Enter choice: ");
} 



// Base System Features
void menuBF()
{
    vector<string> menuMain = {
        "Display All Employees",
        "Display One Employee by ID",
        "Search Employees",
        "Save System Data",
        "Load System Data",
        "Back to Main Menu"};
    Table t;
    t.add_row({"No", "Menu"});
    for (int i = 0; i < menuMain.size(); i++)
    {
        t.add_row({to_string(i + 1), menuMain[i]});
    }
    t[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center);
    t[0].format().font_color(Color::yellow);
    for (int i = 1; i <= menuMain.size(); i++)
    {
        if (i == 6) // Exit option
            t[i][1].format().font_color(Color::red);
        else
            t[i][1].format().font_color(Color::cyan);
    }
    cout << t << endl;
    cout << bold_blue(">> Enter choice: ");
} 



//  User Management
void menuUM()
{
    vector<string> menuMain = {
        "Add New User",
        "Delete User",
        "Manage User Role",
        "View All Users",
        "Back to main menu"
    };
    Table t;
    t.add_row({"No", "Menu"});
    for (int i = 0; i < menuMain.size(); i++)
    {
        t.add_row({to_string(i + 1), menuMain[i]});
    }
    t[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center);
    t[0].format().font_color(Color::yellow);
    for (int i = 1; i <= menuMain.size(); i++)
    {
        if (i == 5) // Exit option
            t[i][1].format().font_color(Color::red);
        else
            t[i][1].format().font_color(Color::cyan);
    }
    cout << t << endl;
    cout << bold_blue(">> Enter choice: ");
} 
// menu login
void menuLogin()
{
    vector<string> menuMain = {"Sign Up", "Sign In", "Exit"};
    Table t;
    t.add_row({"No", "Menu"});
    for (int i = 0; i < menuMain.size(); i++)
    {
        t.add_row({to_string(i + 1), menuMain[i]});
    }
    t[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center);
    t[0].format().font_color(Color::yellow);

    t[1][1].format().font_color(Color::cyan);
    t[2][1].format().font_color(Color::green);
    t[3][1].format().font_color(Color::red);

    cout << t << endl;
    cout << bold_blue(">> Enter choice: ");
}
#endif