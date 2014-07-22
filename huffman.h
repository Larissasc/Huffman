#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <QList>
#include <QString>
#include "node.h"

class huffman
{
public:
    huffman();

    void encodes(QString inputAddress, QString name);
    void decodes(QString HFFAddress, QString name);

    QByteArray arquivo;
    QList<node> lista;
    QByteArray stringTree;
    int trash;

    int loading;

    void fillList();
    void setStringTree();
    void buildTree();
    bool isleaf(node * Node);
    void sortList();
    void setNewCodes();
    void setNewCodes(node * current, QByteArray key);
    void search(QByteArray code, node * start, QByteArray &newCode);
    void getinfo();
    void readTree();
    void readInput(QString inputAddress);
    void writeOutput(QString name);
    void readHFF(QString HFFAddress);
    void writeHFF(QString name);
    QByteArray bitsToBytes(QBitArray bits);
    QBitArray bytesToBits(QByteArray bytes);
};

#endif // HUFFMAN_H
