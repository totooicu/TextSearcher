#include "mainwindow.h"
#include"mysearch.h"
#include <QApplication>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <string>
#include <QTextCodec>
#include<set>
void DfsListFolderFiles(string path,int deep=0)
{
//    cout<<path<<endl;

    _finddata_t file_info;
    string current_path = path + "/*.*";
    long long handle = _findfirst(current_path.c_str(), &file_info);

    //返回值为-1则查找失败
    if (-1 == handle)
    {
        cout << "cannot match the path" << endl;
        return;
    }


    do
    {
        //目录
        if (file_info.attrib == _A_SUBDIR|_A_ARCH)
        {

            //.是当前目录，..是上层目录，须排除掉这两种情况
            if (strcmp(file_info.name, "..") != 0 && strcmp(file_info.name, ".") != 0){
                for(int ii=0;ii<deep;ii++)cout<<"-";
                 cout << file_info.name << endl;
                 if(file_info.attrib == _A_SUBDIR)
                DfsListFolderFiles(path+"/"+file_info.name,deep+1);
        }}

    } while (!_findnext(handle, &file_info));
   //关闭文件句柄
    _findclose(handle);
}
set<string>fileMemo{"text","go","c","cpp","java","m","py","jsp","html","css","js"};

void DfsSearchFiles(string path,string regoal)
{
    cout << "your path" <<path<< endl;

    _finddata_t file_info;
    string current_path = path + "/*.*";
    long long handle = _findfirst(current_path.c_str(), &file_info);

    //返回值为-1则查找失败
    if (-1 == handle)
    {
        cout << "cannot match the path" <<path<< endl;
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
                 }else{
                auto typeV=stringOP::findPlace(".",file_info.name);
                cout << "your file path " <<fp<< endl;
//                cout << "your file type " <<((string)(file_info.name)).substr(typeV[typeV.size()-1])<< endl;

                if(!fileMemo.count(((string)(file_info.name)).substr(typeV[typeV.size()-1]+1)))continue;
                long long num=stringOP::findPlace(regoal,stringOP::readTxtByChar(fp)).size();
                cout<<"the num>>>"<<num<<endl;
                if(num>=1){

                    cout<<">>>"<<file_info.name<<">>>"<<fp<<">>>"<<num<<endl;

            }}
        }

    } while (!_findnext(handle, &file_info));
   //关闭文件句柄
    _findclose(handle);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec* codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);

//string s="flash.go";
//    auto typeV=stringOP::findPlace(".",s);
//for(int i:typeV)cout<<i<<" ";
//string fp="E:/StudyData/GO/src/byte_douyin_project-master/main.go";

//cout<<endl<<"your file type :"<<((string)(s)).substr(typeV[typeV.size()-1]+1)<<endl;
//    cout << "your file type :" <<fileMemo.count(((string)(s)).substr(typeV[typeV.size()-1]+1))<< endl;
//    cout<<(stringOP::readTxtByChar(fp));
//long long num=stringOP::findPlace(" ",stringOP::readTxtByChar(fp)).size();
//cout<<num;


//DfsSearchFiles("E:\\StudyData\\QT\\word"," ");
//DfsListFolderFiles("E:/StudyData/GO");

    MainWindow w;
    w.setWindowIcon(QIcon(":/map/totoo.jpg"));
    w.show();
    return a.exec();
}


