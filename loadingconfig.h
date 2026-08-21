#ifndef LOADINGCONFIG_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<cstring>
#include<io.h>
#include <direct.h>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDebug>
using namespace std;
#define LOADINGCONFIG_H
#pragma execution_character_set("utf-8")

class LoadingConfig {
private:
    string filePath = "TOTOO\\TextSearcher\\TextSearcherConfig.txt";
    string defaultFileTypes = ".text.go.c.cpp.java.m.py.jsp.html.css.js.";
    string defaultApplication = "D:/Program Files(x86)/quicklook/QuickLook-3.7.0/QuickLook.exe";

public:
    string fileTypes;
    string application;

    // 构造函数，初始化成员变量并尝试读取配置文件
    LoadingConfig() {
        readConfig();
    }

    // 成员函数，读取配置文件或使用默认值
    void readConfig() {
        cout<<">>>LoadingConfig filePath"<<filePath<<endl;

        ifstream file(filePath);
        if (!file.is_open()) {
            // 文件不存在，使用默认值
            cout << "配置文件不存在，使用默认值。" << endl;
            fileTypes = defaultFileTypes;
            application = defaultApplication;

            ofstream outFile(filePath);
            if (outFile.is_open()) {
                outFile << fileTypes << endl;
                outFile << application << endl;
                outFile.close();
            } else {
//                cerr << "无法创建文件: " << filePath << endl;
                createFileRecursively(Ss2Qs(filePath));
            }


        } else {
            // 文件存在，加载内容
            getline(file, fileTypes);
            getline(file, application);
            file.close();
        }
    }
    QString Ss2Qs(string ss){
        return QString::fromLocal8Bit(ss.c_str());
    }
    string Qs2Ss(QString qs){
        return string((const char *)qs.toLocal8Bit().constData());
    }

    // 成员函数，将配置写回文件
    void writeConfig(string fileTypes,string application) {
        ofstream outFile(filePath);
        if (outFile.is_open()) {
            outFile << fileTypes << endl;
            outFile << application << endl;
            outFile.close();
        } else {
            cerr << "无法写入文件: " << filePath << endl;
        }
    }

    void createFileRecursively(const QString &filePath) {
        // 提取文件所在的目录路径
        QDir dir(QFileInfo(filePath).absolutePath());

        // 如果目录不存在，则递归地创建它
        if (!dir.exists()) {
            if (!dir.mkpath(".")) {
                qDebug() << "Failed to create directories:" << dir.absolutePath();
                return;
            }
            qDebug() << "Created directories:" << dir.absolutePath();
        }

        // 现在可以安全地创建文件了
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to create file:" << filePath;
            return;
        }

        // 写入一些内容到文件
        QTextStream out(&file);
        out << "Default configuration settings for TextSearcher." << Qt::endl;

        // 关闭文件
        file.close();
        qDebug() << "Created file:" << filePath;
    }

};

#endif // LOADINGCONFIG_H
