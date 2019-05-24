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
        vector<SNL_TOKEN_TYPE> in = lex->getTokenVec();
        p->setInputStrem(in);
        if(p->grammarAnalysis() == -1){
            std::cout<<p->get_errMsg();

        };
        std::cout<<p->getTree();
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
            SetMapTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(get_Token_Str(iter->first))));
            SetMapTable->setItem(i,1,new QTableWidgetItem(QString::fromStdString(p->get_token_vec_str<set<SNL_TOKEN_TYPE>>(iter->second))));
            i++;
        }
    }
    SetMapTable->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    lex->RunFile();
     QTableWidget* TokenTable = new QTableWidget(1,1);
     TokenTable->setItem(0,0,new QTableWidgetItem(QString::fromStdString(lex->printToken_And_Content().str())));
     TokenTable->show();
}

void MainWindow::on_pushButton_7_clicked()
{
    map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> sets = p->get_First_Sets();
    QTableWidget* SetMapTable = new QTableWidget(sets.size(),2);
    SetMapTable->setHorizontalHeaderLabels(QStringList()<<"typename"<<"token");
    int i=0;
    auto m_terminal=p->get_All_Terminals();
    for (auto iter = sets.begin(); iter != sets.end(); iter++) {
        if (m_terminal.find(iter->first) == m_terminal.end()) {
            SetMapTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(get_Token_Str(iter->first))));
            SetMapTable->setItem(i,1,new QTableWidgetItem(QString::fromStdString(p->get_token_vec_str<set<SNL_TOKEN_TYPE>>(iter->second))));
            i++;
        }
    }
    SetMapTable->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    map<int, set<SNL_TOKEN_TYPE>> m_predict_set = p->get_Predict_Sets();
    QTableWidget* ProductionSetTable = new QTableWidget(m_predict_set.size(),2);
    ProductionSetTable->setHorizontalHeaderLabels(QStringList()<<"typename"<<"token");
    int i=0;

    for (auto iter = m_predict_set.begin(); iter != m_predict_set.end(); iter++) {
            ProductionSetTable->setItem(i,0,new QTableWidgetItem(iter->first));
            ProductionSetTable->setItem(i,1,new QTableWidgetItem(QString::fromStdString(p->get_token_vec_str<set<SNL_TOKEN_TYPE>>(iter->second))));
            i++;
    }
    ProductionSetTable->show();
}
