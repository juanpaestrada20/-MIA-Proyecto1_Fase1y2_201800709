#include <QDebug>
#include <QDateTime>
#include <QString>
#include <string>
#include <iostream>
#include <istream>
#include <stdio.h>


using namespace std;

void menu();

int main()
{

    menu();

}

void menu(){
    string entrada = "";
    system("clear");
    cout << "MANEJO E IMPLEMENTACIO DE ARCHIVOS" << endl;
    while(entrada != "exit"){
        cout << "201800709@MIA: ~$ ";
        getline(cin, entrada);

    }
}
