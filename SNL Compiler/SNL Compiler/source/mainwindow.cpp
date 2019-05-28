#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFile>
#include <QFileDialog>
#include "SNL_LL1.h"
#include"SNL_Lexer.h"
#include <iostream>
#include <QTableWidget>
#include <QMessageBox>
#include <QLabel>
#include <QScrollArea>
using namespace std;

ProductionSet* p=NULL;
Lexer* lex=NULL;

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

void MainWindow::on_InputToken_clicked()
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

void MainWindow::on_InputProductionset_clicked()
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
}


void MainWindow::on_TokenList_clicked()
{
    if(lex==NULL){
        QMessageBox::warning(NULL, "warning", "Input token first", QMessageBox::Retry | QMessageBox::Cancel,QMessageBox::Retry);
        return;
    }
    bool flag = lex->RunFile();
    if(!flag){
        QMessageBox::warning(NULL, "ERROR", "LEX ERROR" + QString::fromStdString(lex->lex_err_msg) , QMessageBox::Retry | QMessageBox::Cancel,QMessageBox::Retry);
        return;
    }
     QTableWidget* TokenTable = new QTableWidget(0,2);

     vector<SNL_TOKEN_TYPE>m_Token_Vec = lex->getTokenVec();
     vector<string>m_Token_Contant_Vec = lex->getTokenContantVec();
     auto token_iter = m_Token_Vec.begin();
     auto token_contain_iter = m_Token_Contant_Vec.begin();
     while (token_iter != m_Token_Vec.end() || token_contain_iter != m_Token_Contant_Vec.end()) {
         int row = TokenTable->rowCount();
         TokenTable->insertRow(row);
         TokenTable->setItem(row,0,new QTableWidgetItem(QString::fromStdString(get_Token_Str(*token_iter))));
         TokenTable->setItem(row,1,new QTableWidgetItem(QString::fromStdString(*token_contain_iter)));

         if (token_iter != m_Token_Vec.end()) {
             token_iter++;
         }
         if (token_contain_iter != m_Token_Contant_Vec.end()) {
             token_contain_iter++;
         }
     }
     TokenTable->show();
}

void MainWindow::on_FirstTable_clicked()
{
    if(p==NULL){
        QMessageBox::warning(NULL, "warning", "haven't input productionset", QMessageBox::Retry | QMessageBox::Cancel,QMessageBox::Retry);
        return;
    }
    map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> sets = p->get_First_Sets();
    QTableWidget* FirstTable = new QTableWidget(0,2);
    FirstTable->setHorizontalHeaderLabels(QStringList()<<"typename"<<"token");
    auto m_terminal=p->get_All_Terminals();
    for (auto iter = sets.begin(); iter != sets.end(); iter++) {
        if (m_terminal.find(iter->first) == m_terminal.end()) {
            int row = FirstTable->rowCount();
            FirstTable->insertRow(row);
            FirstTable->setItem(row,0,new QTableWidgetItem(QString::fromStdString(get_Token_Str(iter->first))));
            FirstTable->setItem(row,1,new QTableWidgetItem(QString::fromStdString(p->get_token_vec_str<set<SNL_TOKEN_TYPE>>(iter->second))));
        }
    }
    FirstTable->show();
}

void MainWindow::on_FollowTable_clicked()
{
    if(p==NULL){
        QMessageBox::warning(NULL, "warning", "haven't input productionset", QMessageBox::Retry | QMessageBox::Cancel,QMessageBox::Retry);
        return;
    }
    map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> sets = p->get_Follow_Sets();
    QTableWidget* FollowTable = new QTableWidget(0,2);
    FollowTable->setHorizontalHeaderLabels(QStringList()<<"typename"<<"token");
    auto m_terminal=p->get_All_Terminals();
    for (auto iter = sets.begin(); iter != sets.end(); iter++) {
        if (m_terminal.find(iter->first) == m_terminal.end()) {
            int row = FollowTable->rowCount();
            FollowTable->insertRow(row);
            FollowTable->setItem(row,0,new QTableWidgetItem(QString::fromStdString(get_Token_Str(iter->first))));
            FollowTable->setItem(row,1,new QTableWidgetItem(QString::fromStdString(p->get_token_vec_str<set<SNL_TOKEN_TYPE>>(iter->second))));
        }
    }
    FollowTable->show();
}

void MainWindow::on_PredicctTable_clicked()
{
    if(p==NULL){
        QMessageBox::warning(NULL, "warning", "haven't input productionset", QMessageBox::Retry | QMessageBox::Cancel,QMessageBox::Retry);
        return;
    }
    map<int, set<SNL_TOKEN_TYPE>> m_predict_set = p->get_Predict_Sets();
    QTableWidget* PredictTable = new QTableWidget(0,2);
    PredictTable->setHorizontalHeaderLabels(QStringList()<<"typename"<<"token");

    for (auto iter = m_predict_set.begin(); iter != m_predict_set.end(); iter++) {
        int row = PredictTable->rowCount();
        PredictTable->insertRow(row);
        PredictTable->setItem(row,0,new QTableWidgetItem(QString::number(iter->first)));
        PredictTable->setItem(row,1,new QTableWidgetItem(QString::fromStdString(p->get_token_vec_str<set<SNL_TOKEN_TYPE>>(iter->second))));
    }
    PredictTable->show();
}


void MainWindow::on_GrammarTree_clicked()
{
    if(p==NULL&lex==NULL){
        QMessageBox::warning(NULL, "warning", "Token and productionset are missing", QMessageBox::Retry | QMessageBox::Cancel,QMessageBox::Retry);
        return;
    }
    if(p==NULL){
        QMessageBox::warning(NULL, "warning", "Productionset missing", QMessageBox::Retry | QMessageBox::Cancel,QMessageBox::Retry);
        return;
    }
    if(lex==NULL){
        QMessageBox::warning(NULL, "warning", "Token missing", QMessageBox::Retry | QMessageBox::Cancel,QMessageBox::Retry);
        return;
    }
    vector<SNL_TOKEN_TYPE> in = lex->getTokenVec();
    vector<string> in_str = lex->getTokenContantVec();
    p->setInputStrem(in,in_str);
    if(p->grammarAnalysis() == -1){
        std::cout<<p->get_errMsg();

    };
    p->buildTree();
    string stdString = p->getTreeToDOTLanguage();
    QByteArray byteArray(stdString.c_str(), stdString.length());
    ui->textBrowser->setText(byteArray);

    stdString = p->getTreeToStr();
    QByteArray byteArrayGT(stdString.c_str(), stdString.length());
    QTabWidget *tabwidget = new QTabWidget;
    QScrollArea *scrollArea = new QScrollArea;
    QLabel *widget = new QLabel;
    widget->setText(byteArrayGT);
    widget->adjustSize();
    scrollArea->setWidget(widget);
    tabwidget->addTab(scrollArea,"GrammarTree");
    tabwidget->show();

}
