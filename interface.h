#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets>
#include <QtCore>
#include <QProgressBar>
#include "huffman.h"

namespace Ui {
class interface1;
}

class interface1 : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit interface1(QMainWindow *parent = 0);
    ~interface1();

    huffman hff;

    QString fileAddress;
    QString name;
    
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::interface1 *ui;
    QProgressBar* bar;
};

#endif // INTERFACE_H
