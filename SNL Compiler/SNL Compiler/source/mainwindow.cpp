#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFile>
#include <QFileDialog>
#include "SNL_LL1.h"
#include"SNL_Lexer.h"
#include <iostream>
#include <QTableWidget>
using namespace std;

string f_t;
string f_p;
ProductionSet* p;
Lexer* lex;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".", "Txt(*.txt);;c files(*.c);; cpp files(*.cpp)");
        if(filename.isEmpty() == false)
        {
            QFile file(filename);
            QByteArray ba = filename.toLatin1();
            const char *c = ba.data();
            lex = new Lexer(c);
            bool isok = file.open(QIODevice::ReadOnly);//以只读的方式打开文件
            if(isok == true)
            {
                QByteArray array;
                while(file.atEnd() == false)
                {
                    array += file.readLine();//将文件中的读取的内容保存在字节数组中。
                }
                ui->textBrowser->setText(array);
            }
            file.close();//文件读取完毕后关闭文件。
        }
        lex->RunFile();
}

void MainWindow::on_pushButton_5_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", "Txt(*.txt);;c files(*.c);;c++ files(*.cpp)");
        if(filename.isEmpty() == false)
        {
            QFile file(filename);
            bool isok = file.open(QIODevice::WriteOnly);
            if(isok == true)
            {
                QString str = ui->textBrowser->toPlainText();
                file.write(str.toUtf8());
            }
            file.close();
        }
}

void MainWindow::on_pushButton_6_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".", "Txt(*.txt);;c files(*.c);; cpp files(*.cpp)");
        if(filename.isEmpty() == false)
        {
            QFile file(filename);
            p = new ProductionSet(filename.toStdString());
            bool isok = file.open(QIODevice::ReadOnly);//以只读的方式打开文件
            if(isok == true)
            {
                QByteArray array;
                while(file.atEnd() == false)
                {
                    array += file.readLine();//将文件中的读取的内容保存在字节数组中。
                }
                ui->textBrowser->setText(array);
            }
            file.close();//文件读取完毕后关闭文件。
        }
        //p->SNL_AnalyseProcess(lex->getTokenVec());
}

void MainWindow::on_pushButton_3_clicked()
{
    map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> sets = p->get_Follow_Sets();
    QTableWidget* SetMapTable = new QTableWidget(sets.size(),2);
    SetMapTable->setHorizontalHeaderLabels(QStringList()<<"typename"<<"token");
    int i=0;
    auto m_terminal=p->get_All_Terminals();
    for (auto iter = sets.begin(); iter != sets.end(); iter++) {
        if (m_terminal.find(iter->first) == m_terminal.end()) {
            SetMapTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Token_Type_Name_Map.find(iter->first)->second)));
            SetMapTable->setItem(i,1,new QTableWidgetItem(QString::fromStdString(p->get_token_str<set<SNL_TOKEN_TYPE>>(iter->second))));
            i++;
        }
    }
    SetMapTable->show();
}
