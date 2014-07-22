#ifndef NODE_H
#define NODE_H

#include <QtCore>
#include <QByteArray>
#include <QString>
#include <QBitArray>
#include <iostream>

using namespace std;

class node
{
public:

    int frequency;
    QByteArray character;
    QByteArray newBits;
    node* left;
    node* right;

    node();
};

#endif // NODE_H
