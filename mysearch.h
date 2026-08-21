#ifndef SEARCH_H
#define SEARCH_H
#include <stdio.h>
#include<stdlib.h>
#include<io.h>
#include<malloc.h>
#include<fstream>
#include <time.h>
#include<string>
#include<QDebug>
#include"stringop.h"
using namespace std;

class mysearch
{
public:

    /*
    *深度优先递归遍历当前目录下的文件夹、文件及子文件夹和文件
    */

    void DfsListFolderFiles(string path)
    {   cout<<path<<endl;

        _finddata_t file_info;
        string current_path = path + "/*.*";
        int handle = _findfirst(current_path.c_str(), &file_info);

        //返回值为-1则查找失败
        if (-1 == handle)
        {
            cout << "cannot match the path" << endl;
            return;
        }


        do
        { cout<<"here"<<endl;
            //目录
            if (file_info.attrib == _A_SUBDIR)
            {
                cout << file_info.name << endl;
                //.是当前目录，..是上层目录，须排除掉这两种情况
                if (strcmp(file_info.name, "..") != 0 && strcmp(file_info.name, ".") != 0)
                    DfsListFolderFiles(path);
            }
            else
            {
                cout << file_info.name << endl;
            }
        } while (!_findnext(handle, &file_info));
       //关闭文件句柄
        _findclose(handle);
    }

};

#endif // SEARCH_H
