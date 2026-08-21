#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <stdio.h>
#include<io.h>
#include<QTreeWidgetItem>
#include<stdlib.h>
#include<malloc.h>
#include<map>
#include<fstream>
#include <time.h>
#include<string>
#include<QDebug>
#include"stringop.h"
using namespace std;
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include<QLabel>
#include <set>
#include"loadingconfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoadingConfig config;
    map<QTreeWidgetItem*,bool>memo;
    set<QTreeWidgetItem*>LoadedItem;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTreeWidget *tree ;
    string path;
    stringOP sop;
QTableWidget* tableWidget;
long long rowCount = 0; // 行号
    QTextEdit *TextEdit,*outTextEdit,*searchTextEdit,*openMethodTextEdit,*searchTypeTextEdit;
    QLabel *InputFolderPath,*ClickedFolderPath,*FileType,*FileContextm,*Application;
    QPushButton *pushButton,*outPushButton,*searchPushButton,*upperPushButton;
    set<string>fileMemo{"text","go","c","cpp","java","m","py","jsp","html","css","js"};

    QString Ss2Qs(string ss){
        return QString::fromLocal8Bit(ss.c_str());
    }
    string Qs2Ss(QString qs){
        return string((const char *)qs.toLocal8Bit().constData());
    }

    template<class T>
    void DfsListFolderFiles(const string path, T *parent,int deep=1);
    void removeItem(QTreeWidgetItem *item);
    void removeSelectedItems(QTreeWidget *treeWidget);
    void DfsSearchFiles(string path, string regoal);
    void listFolderFiles(QString path, QTreeWidget *tree);
    void searchFiles(QString path, QString searchString);
    void handleTableWidgetClicked(QTableWidgetItem *item);
public slots:
    void myOnItemClicked(string path, QTreeWidgetItem *item, string fname);
    void on_itemClicked(QTreeWidgetItem *item, int column);
    void handleButtonClicked();
    void handleOutButtonClicked();
    void handleSearchButtonClicked();
    void on_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void handleTableWidgetDoubleClicked(QTableWidgetItem *item);
    void handleUpperButtonClicked();
private:
    Ui::MainWindow *ui;
private slots:
    void handleItemClicked(QTreeWidgetItem *item, int column);

};
#endif // MAINWINDOW_H
