#ifndef MENUSTYLE_H
#define MENUSTYLE_H
#include "header.h" 
using namespace std;


// functionEnter
void pressEnter(){
    cout<<bold_magenta(">> Press Enter to Coutinue : ");
    cin.ignore();
}
// >> main Menu
void menuMain(){
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


#endif