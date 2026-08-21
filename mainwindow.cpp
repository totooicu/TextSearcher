#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QPushButton>
#include <QSplitter>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <thread>
#include"treewidgetitemwrapper.h"
#include<QHBoxLayout>
#include<QSvgRenderer>
#include <QTableWidget>
#include <set>
#include <thread>
#include <QtCore>
#include<functional>
#include <thread>


template<class T>
void MainWindow::DfsListFolderFiles(const string path,T *parent,int deep)
{   if (deep==0)return ;

    cout<<"now path:"<<path<<endl;
    _finddata_t file_info;
    string current_path = path + "/*.*";
    long long handle = _findfirst(current_path.c_str(), &file_info);
    //返回值为-1则查找失败
    if (-1 == handle)
    {
        cout << "cannot match the path(DfsListFolderFiles)"<<current_path << endl;
        return;
    }

    do{
        if ((file_info.attrib &( _A_SUBDIR|_A_ARCH))!=0)
        {
            //.是当前目录，..是上层目录，须排除掉这两种情况
            if ( strcmp(file_info.name, "..") == 0 || strcmp(file_info.name, ".") == 0)continue;

//            cout<<"Qs2Ss(searchTextEdit->toPlainText())="<<Qs2Ss(searchTextEdit->toPlainText())<<endl;
            QTreeWidgetItem *item1 = new QTreeWidgetItem(parent, QStringList(Ss2Qs(file_info.name)));

            DfsListFolderFiles(path+"/"+file_info.name,item1,deep-1);
                QIcon icon;
                 if((file_info.attrib & _A_SUBDIR)!=0){

         icon.addPixmap(QPixmap(":/map/dir_f4ea2a.png"), QIcon::Normal, QIcon::On);//节点打开状态
         icon.addPixmap(QPixmap(":/map/dir_dbdbdb.png"), QIcon::Normal, QIcon::Off);//节点关闭状态
        }else{
         icon.addPixmap(QPixmap(":/map/file_1296db.png"), QIcon::Normal, QIcon::On);//节点打开状态
         icon.addPixmap(QPixmap(":/map/file_1296db.png"), QIcon::Normal, QIcon::Off);//节点关闭状态
                 }
                 item1->setIcon(0,icon);
//                 if(deep>=1)memo[item1]=1;

    }} while (!_findnext(handle, &file_info));
   //关闭文件句柄
    _findclose(handle);
}


void MainWindow::DfsSearchFiles(string path,string regoal)
{
    cout << "your path" <<path<< endl;

    _finddata_t file_info;
    string current_path = path + "/*.*";
    long long handle = _findfirst(current_path.c_str(), &file_info);

    //返回值为-1则查找失败
    if (-1 == handle)
    {
        cout << "cannot match the path(DfsSearchFiles)" <<path<< endl;
        return;
    }

    do
    {
        //目录
        if ((file_info.attrib &( _A_SUBDIR|_A_ARCH))!=0)        {
            //.是当前目录，..是上层目录，须排除掉这两种情况
            if (strcmp(file_info.name, "..") == 0 || strcmp(file_info.name, ".") == 0)continue;
            string fp=path+"/"+file_info.name;
                 if((file_info.attrib & _A_SUBDIR)!=0){
            DfsSearchFiles(fp,regoal);
//            std::thread t1(&MainWindow::DfsSearchFiles,this,fp,regoal);
//            t1.join();
                 }else{
                auto typeV=stringOP::findPlace(".",file_info.name);
                if(typeV.size()==0)continue;
                cout << "your file path " <<fp<< endl;
//                cout << "your file type " <<((string)(file_info.name)).substr(typeV[typeV.size()-1])<< endl;

                if(!fileMemo.count(((string)(file_info.name)).substr(typeV[typeV.size()-1]+1)))continue;
                long long num=stringOP::findPlace(regoal,stringOP::readTxtByChar(fp)).size();

                if(num>=1){
                    cout<<"the num>>>"<<num<<"  rowCount>>>"<< rowCount<<endl;
                tableWidget->insertRow(rowCount); // 插入新行

                tableWidget->setItem(rowCount, 0, new QTableWidgetItem(Ss2Qs(file_info.name))); // 添加文件名
                tableWidget->setItem(rowCount, 1, new QTableWidgetItem(Ss2Qs(fp))); // 添加文件名
                tableWidget->setItem(rowCount++, 2, new QTableWidgetItem(Ss2Qs(to_string(num)))); // 添加文件

                }}
            }

        } while (!_findnext(handle, &file_info));
       //关闭文件句柄
        _findclose(handle);
    }

//递归删除节点
void MainWindow::removeItem(QTreeWidgetItem *item)
{
    int count = item->childCount();
    if(count==0)//没有子节点，直接删除
    {
        delete item;
        return;
    }

    for(int i=0; i<count; i++)
    {
        QTreeWidgetItem *childItem = item->child(0);//删除子节点
        removeItem(childItem);
    }
//    delete item;//最后将自己删除

}

//删除选中的节点及子节点
void  MainWindow::removeSelectedItems(QTreeWidget*treeWidget)
{
    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();

    for (int i = 0; i < items.size(); ++i) {
        removeItem(items[i]);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVBoxLayout* leftLayout = new QVBoxLayout;
//input
    TextEdit = new QTextEdit(this);
//    TextEdit->setAcceptDrops(false); // 关闭文本编辑框的拖拽功能
    TextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 设置文本编辑框的尺寸策略
    TextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 设置文本编辑框的垂直滚动条策略
    TextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 设置文本编辑框的水平滚动条策略
    TextEdit->setText("C:");

    pushButton = new QPushButton("Open 文件夹", this);

    outTextEdit = new QTextEdit(this);
//    outTextEdit->setAcceptDrops(false);
    outTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    outTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    outTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    searchTypeTextEdit=new QTextEdit(this);
//    searchTypeTextEdit->setAcceptDrops(false);
    searchTypeTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    searchTypeTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    searchTypeTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    searchTypeTextEdit->setText(Ss2Qs( config.fileTypes));

    searchTextEdit = new QTextEdit(this);
//    searchTextEdit->setAcceptDrops(false);
    searchTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    searchTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    searchTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    searchTextEdit->setText(".*");

    searchPushButton=new QPushButton("search file", this);

    openMethodTextEdit = new QTextEdit(this);
//    openMethodTextEdit->setAcceptDrops(false);
    openMethodTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    openMethodTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    openMethodTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    openMethodTextEdit->setText(Ss2Qs(config.application));

    upperPushButton=new QPushButton("上一层", this);

    InputFolderPath=new QLabel(this);
    InputFolderPath->setText("请输入文件夹路径：");
    ClickedFolderPath=new QLabel(this);
    ClickedFolderPath->setText("所点击文件（夹）路径：");
    FileType=new QLabel(this);
    FileType->setText("输入文件后缀：");
    FileContextm=new QLabel(this);
    FileContextm->setText("输入搜索内容：");
    Application=new QLabel(this);
    Application->setText("双击文件（夹）打开方式：");

//Alayout
    leftLayout->addWidget(InputFolderPath);
    leftLayout->addWidget(TextEdit);
    leftLayout->addWidget(pushButton);

    leftLayout->addWidget(ClickedFolderPath);
    leftLayout->addWidget(outTextEdit);

    leftLayout->addWidget(FileType);
    leftLayout->addWidget(searchTypeTextEdit);

//    leftLayout->addWidget(FileType);
    leftLayout->addWidget(FileContextm);
    leftLayout->addWidget(searchTextEdit);
    leftLayout->addWidget(searchPushButton);
    leftLayout->addWidget(Application);
    leftLayout->addWidget(openMethodTextEdit);
    leftLayout->addWidget(upperPushButton);


    QWidget* leftWidget = new QWidget(this);
    leftWidget->setLayout(leftLayout);

    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);

    splitter->addWidget(leftWidget);

    tree = new QTreeWidget(this);
    tree->setStyleSheet("background-color:#cfcfcf;");
    tree->setIndentation(40);
    tree->setIconSize(QSize(55, 24));

    path = "E:";
    QStringList fileData{"name","load","num"};
    vector<QStringList> fileDataList{fileData};
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3); // 设置表格的列数
    tableWidget->setHorizontalHeaderLabels(QStringList() << "filename" << "filepath" << "displaynum");

    // 添加数据

    // 假设你有一个包含文件信息的列表 fileDataList，每个元素是一个 QStringList，包括文件名、文件路径和字符出现次数
//    foreach (const QStringList& fileData, fileDataList) {
//        tableWidget->insertRow(rowCount); // 插入新行
//        tableWidget->setItem(rowCount, 0, new QTableWidgetItem(fileData.at(0))); // 添加文件名
//        tableWidget->setItem(rowCount, 1, new QTableWidgetItem(fileData.at(1))); // 添加文件路径
//        tableWidget->setItem(rowCount, 2, new QTableWidgetItem(fileData.at(2))); // 添加字符出现次数
//        rowCount++;
//    }

    // 将表格添加到页面的 C 分区
    QVBoxLayout* cLayout = new QVBoxLayout;
    cLayout->addWidget(tableWidget);

    QWidget* cWidget = new QWidget;
    cWidget->setLayout(cLayout);

    splitter->addWidget(tree);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    splitter->addWidget(cWidget);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setStretchFactor(2, 1);


    setCentralWidget(splitter);
    this->setWindowIcon(QIcon(":/map/totoo.jpg"));

    connect(tree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(on_itemClicked(QTreeWidgetItem*, int)));
    connect(tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(on_itemDoubleClicked(QTreeWidgetItem*, int)));

    connect(pushButton, &QPushButton::clicked, this, &MainWindow::handleButtonClicked);
    connect(tableWidget,&QTableWidget::itemDoubleClicked,this,&MainWindow::handleTableWidgetDoubleClicked);
    connect(tableWidget,&QTableWidget::itemClicked,this,&MainWindow::handleTableWidgetClicked);

//    connect(outPushButton, &QPushButton::clicked, this, &MainWindow::handleOutButtonClicked);
    connect(searchPushButton, &QPushButton::clicked, this, &MainWindow::handleSearchButtonClicked);
    connect(upperPushButton, &QPushButton::clicked, this, &MainWindow::handleUpperButtonClicked);

}


void MainWindow::handleTableWidgetClicked(QTableWidgetItem *item) {
    QModelIndex index = tableWidget->currentIndex();
    int column = index.column(); // 获取双击的列索引
    int row = index.row(); // 获取双击的行索引
    // 获取第1列的内容
    QTableWidgetItem* item1 = tableWidget->item(row, 1);

    if (item1 != nullptr) {
        outTextEdit->setText(item1->text());


    }
}
void MainWindow::handleTableWidgetDoubleClicked(QTableWidgetItem *item) {
    QModelIndex index = tableWidget->currentIndex();
    int column = index.column(); // 获取双击的列索引
    int row = index.row(); // 获取双击的行索引
    // 获取第1列的内容
    QTableWidgetItem* item1 = tableWidget->item(row, 1);
    if (item1 != nullptr) {
        QString program = Ss2Qs( config.application);

    string fp=Qs2Ss( item1->text());
    // 其他逻辑处理...

    // Example: 调用 QuickLook 程序打开第1列的内容
    if (column == 0) {
        QStringList arguments;
        arguments << item1->text();
            QProcess *process = new QProcess(this);
            process->start(program, arguments);
        }
    if(column==1){
        auto p=stringOP::findPlace("/",fp);
        tree->clear();
        path=fp.substr(0,p[p.size()-1]);
        DfsListFolderFiles(fp.substr(0,p[p.size()-1]),tree);
        ;
    }
    }
}

void MainWindow::handleUpperButtonClicked(){
    auto p=stringOP::findPlace("/",path);
    if(p.size()==0)return;
    path=path.substr(0,p[p.size()-1]);
    TextEdit->clear();
    TextEdit->setText(Ss2Qs(path));
    tree->clear();
    DfsListFolderFiles(path,tree);
}


void  MainWindow::handleSearchButtonClicked(){
    tree->clear();
    path= Qs2Ss(TextEdit->toPlainText());
    string searchText=Qs2Ss(searchTextEdit->toPlainText());
  cout<<"Path="<<path<<endl;
  rowCount=0;tableWidget->clear();
  tableWidget->setRowCount(0);

  string searchTypeText=Qs2Ss(searchTypeTextEdit->toPlainText());
  fileMemo.clear();
  auto p=stringOP::findPlace(".",searchTypeText);
  for(int i=1;i<p.size()-1;i++){
      fileMemo.insert(searchTypeText.substr(p[i]+1,p[i+1]-p[i]-1));
      cout<<"searchTypeTextEdit>>>"<<searchTypeText.substr(p[i]+1,p[i+1]-p[i]-1)<<endl;
  }

  std::thread t1(&MainWindow::DfsSearchFiles,this,path,searchText);
  t1.detach();
//  DfsSearchFiles(path,searchText);

}

void  MainWindow::handleButtonClicked(){
    tree->clear();
    path= Qs2Ss(TextEdit->toPlainText());
  cout<<"Path="<<path<<endl;
  DfsListFolderFiles(path,tree);

}

void  MainWindow::handleOutButtonClicked(){
    QString program = Ss2Qs(config.application);
    QStringList arguments;
    arguments << outTextEdit->toPlainText();

    QProcess *process = new QProcess(this);
    process->start(program, arguments);

}

MainWindow::~MainWindow()
{
    config.writeConfig(Qs2Ss(searchTypeTextEdit->toPlainText()),Qs2Ss(openMethodTextEdit->toPlainText()));
    delete ui;
}
void MainWindow::myOnItemClicked(string path,QTreeWidgetItem *item, string fname)
{    cout<<path + "./" + fname<<endl;
    this->DfsListFolderFiles(path + "./" + fname, item);
}

void MainWindow::on_itemClicked(QTreeWidgetItem* item, int column)
{
    QList<QString> pathList;

    QTreeWidgetItem* currentItem = item;
    while (currentItem)
    {
        QString currentValue = currentItem->text(column);
        pathList.prepend(currentValue);
        currentItem = currentItem->parent();
    }

    QString fullPath = Ss2Qs(path)+"/"+pathList.join("/");
    qDebug() << "Path from root to item: " << fullPath;

    string thisPath=Qs2Ss(fullPath);
//    cout<< "Path from root to item: " <<thisPath;

    outTextEdit->clear();
    outTextEdit->setText(Ss2Qs(thisPath));

    _finddata_t file_info;
    string current_path =thisPath;
    long long handle = _findfirst(current_path.c_str(), &file_info);
    //返回值为-1则查找失败
    if (-1 == handle)
    {
        cout << "cannot match the path" << endl;
        return;
    }
        if ((file_info.attrib &( _A_SUBDIR))!=0)
        {
            //.是当前目录，..是上层目录，须排除掉这两种情况
            if (strcmp(file_info.name, "..") == 0 || strcmp(file_info.name, ".") == 0)return;


    if(!memo[item])memo[item]=1;
    else return;
//    this->removeItem(item);
    DfsListFolderFiles(thisPath,item);
//    std::thread t1(&MainWindow::DfsListFolderFiles,this,thisPath,item);
    std::thread t1([this, thisPath, item]() {
        DfsListFolderFiles(thisPath, item);
    });
    t1.detach();

}}

void MainWindow::on_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    QString program = (openMethodTextEdit->toPlainText());

    QStringList arguments;

    arguments << outTextEdit->toPlainText();

    _finddata_t file_info;
    string current_path = Qs2Ss(outTextEdit->toPlainText());
    long long handle = _findfirst(current_path.c_str(), &file_info);
//auto parent1=type!=0?((QTreeWidget*)parent):((QTreeWidgetItem*)parent);
    //返回值为-1则查找失败
    if (-1 == handle)
    {
        return;
    }

        //目录
        if ((file_info.attrib &(_A_ARCH))!=0)
        {           //.是当前目录，..是上层目录，须排除掉这两种情况
            if (strcmp(file_info.name, "..") != 0 && strcmp(file_info.name, ".") != 0){

    QProcess *process = new QProcess(this);
    process->start(program, arguments);
            }}
}

void MainWindow::handleItemClicked(QTreeWidgetItem *item, int column)
{
}


template void MainWindow::DfsListFolderFiles(const string ,QTreeWidget *,int);
template void MainWindow::DfsListFolderFiles(const string ,QTreeWidgetItem *,int);
