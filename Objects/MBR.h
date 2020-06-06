#ifndef MBR_H
#define MBR_H

#include <QDateTime>
#include <string>
#include <stdio.h>
#include <iostream>
#include "Partition.h"
#include "DataStructures/LinkedList.h"

using namespace std;

class MBR{
public:
    int size;
    string time;
    int signature;
    char fit;
    LinkedList *partitions;
    MBR(int size, int signature, char fit);
    void newPartition(Partition part);
    void deletePartition(string name);
private:
    string getDateTime();
};

MBR::MBR(int size, int signature, char fit){
    this->size = size;
    this->time = getDateTime();
    this->signature = signature;
    this->fit = fit;
    partitions = new LinkedList();
}

void MBR::newPartition(Partition part){

}

void MBR::deletePartition(string name){

}

string MBR::getDateTime(){
    QDate fecha = QDate::currentDate();
    QTime hora = QTime::currentTime();
    QString time = QDateTime(fecha, hora).toString("dd.MM.yyyy hh:mm:ss");

    string t = time.toStdString();

    return t;
}

#endif // MBR_H
