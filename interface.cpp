#include "interface.h"
#include "ui_interface.h"
#include <iostream>
#include <QFileDialog>
#include <QDebug>

//typedef QStringList (*_qt_filedialog_open_filenames_hook)(QWidget * parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options);
//typedef QString (*_qt_filedialog_open_filename_hook)     (QWidget * parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options);
//typedef QString (*_qt_filedialog_save_filename_hook)     (QWidget * parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options);
//typedef QString (*_qt_filedialog_existing_directory_hook)(QWidget *parent, const QString &caption, const QString &dir, QFileDialog::Options options);

//extern Q_GUI_EXPORT _qt_filedialog_open_filename_hook qt_filedialog_open_filename_hook;
//extern Q_GUI_EXPORT _qt_filedialog_open_filenames_hook qt_filedialog_open_filenames_hook;
//extern Q_GUI_EXPORT _qt_filedialog_save_filename_hook qt_filedialog_save_filename_hook;
//extern Q_GUI_EXPORT _qt_filedialog_existing_directory_hook qt_filedialog_existing_directory_hook;


interface1::interface1(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::interface1)
{
    ui->setupUi(this);
    //ui->progressBar->setValue(0);
}

interface1::~interface1()
{
    delete ui;
}

void interface1::on_pushButton_2_clicked() //add button
{
//    if (!qgetenv("QT_NO_NATIVE_FILEDIALOGS").isEmpty()) {
//        qt_filedialog_open_filename_hook = 0;
//        qt_filedialog_open_filenames_hook = 0;
//        qt_filedialog_save_filename_hook = 0;
//        qt_filedialog_existing_directory_hook = 0;
//    }

    fileAddress = QFileDialog::getOpenFileName();

    ui->textEdit->setPlainText(fileAddress);
}

void interface1::on_pushButton_3_clicked() //zip button
{
    ui->textEdit_2->clear();
    name = ui->lineEdit->text();
    hff.encodes(fileAddress, name);
    ui->textEdit_2->setPlainText("File was encoded!");
}

void interface1::on_pushButton_clicked() //unzip
{
    ui->textEdit_2->clear();
    name = ui->lineEdit->text();
    hff.decodes(fileAddress, name);
    ui->textEdit_2->setPlainText("HFF was decoded!");
}
