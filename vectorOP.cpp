#pragma execution_character_set("utf-8")
#include <string>
#include <iostream>
#include<algorithm>
#include <vector>
#include<map>
#include "vectorop.h"
#include<QDebug>

using namespace std;




//*******************P
template<class T=string>
string vectorOP::printVector(vector<T>& v, string name ,string separator,int show) {
    std::ostringstream oss;
    std::streambuf* pOldBuf = std::cout.rdbuf(oss.rdbuf());

    typename::std::vector<T>::iterator itBegin2 = v.begin();
    cout<<name<<">>>[";
    for (; itBegin2 != v.end(); itBegin2++){

    cout <<* itBegin2<<separator;
}
    cout<<"]";
std::cout.rdbuf(pOldBuf);

if(show)qDebug()<<QString::fromStdString(oss.str())  ;
return oss.str();

}

template<class T>
string vectorOP::printVector(vector<vector<T> > &v, string name,string separator)
{
    std::ostringstream oss;
    std::streambuf* pOldBuf = std::cout.rdbuf(oss.rdbuf());
    int a=0;
    cout<< name<< ">>>]";
    for(auto i:v)cout<<vectorOP::printVector(i,"index>>>"+to_string(a++),separator,0);
    cout<< "[\n";

    std::cout.rdbuf(pOldBuf);

    qDebug()<<QString::fromStdString(oss.str())  ;

    return oss.str();
}

template<class T>
string vectorOP::vec2str(vector<T> vs, string fill ) {
    string S = new(string);
    for (auto i : vs)
        S.append(fill + i);
    return S;
}

template <class T, class Fn>
void vectorOP::myreplace(vector<T> &v,int startIndex, int lon, Fn f) {
    typename::std::vector<T>::iterator it = v.begin();
    it += startIndex;
    for (; it != v.begin()+ startIndex+lon; it++)
         *it =f(*it);
}

//template <class T , class _Fn>
//void vectorOP::myreplace(vector<T>& v, _Fn f) {
//    vectorOP::myreplace(v, 0, v.size(), f);
//}




////////////////////////////////////////////////////////////////////////////////////////////

template<class T, class T2,class T3>
void vectorOP::printMap(map<T, T2,T3>& v, string name) {
    typename::std::map<T, T2,T3>::iterator itBegin2 = v.begin();
    qDebug() << QString::fromStdString(name) << ">>>[";
    for (; itBegin2 != v.end(); itBegin2++)
        qDebug()  << itBegin2->first << ":" << itBegin2->second << ",";
    qDebug()  << "]\n";
}

template<class T , class T2>
void vectorOP::printMap(map<T, T2>& v, string name) {
    typename::std::map<T, T2>::iterator itBegin2 = v.begin();
    qDebug() << QString::fromStdString(name) << ">>>[";
    for (; itBegin2 != v.end(); itBegin2++)
        qDebug()  << itBegin2->first << ":" << itBegin2->second << ",";
    qDebug()  << "]\n";
}

template<class T1,class T2>
void vectorOP::map2vec(map<T1, T2>v,vector<T1>&v1, vector<T2>& v2) {
    typename::std::map<T1, T2>::iterator itBegin2 = v.begin();
    for (; itBegin2 != v.end(); itBegin2++)
    v1.push_back(itBegin2->first);
    v2.push_back(itBegin2->second);
}

template<class T1, class T2>
map<T1, T2> vectorOP::vec2map(map<T1, T2>&v, vector<T1>v1, vector<T2>v2) {
    for (int i = 0; i < v1.size(); i++)v[v1[i]] = v2[i];
    return v;
}

template<class T1, class T2,class T3,class T4>
void vectorOP::map2vec(map<T1, T2,T3,T4>v, vector<T1>& v1, vector<T2>& v2) {
    typename::std::map<T1, T2,T3>::iterator itBegin2 = v.begin();
    for (; itBegin2 != v.end(); itBegin2++) {
        v1.push_back(itBegin2->first);
        v2.push_back(itBegin2->second);
    }
}

template<class T1, class T2,class T3,class T4>
map<T1, T2,T3,T4> vectorOP::vec2map(map<T1, T2,T3,T4>& v, vector<T1>v1, vector<T2>v2) {
    for (int i = 0; i < v1.size(); i++)v[v1[i]] = v2[i];
    return v;
}

template<class T>
vector<T> vectorOP::sub(vector<T>v, int head, int lon)
{
    vector<T>V;
    int i=lon==-1?v.size():head;
    for(;i<head+lon;i++){
        V.push_back(v[i]);
    }
    return V;
}

template vector<string> vectorOP::sub(vector<string>,int,int);
template vector<int> vectorOP::sub(vector<int>,int,int);
template vector<double> vectorOP::sub(vector<double>,int,int);
template vector<float> vectorOP::sub(vector<float>,int,int);
template vector<bool> vectorOP::sub(vector<bool>,int,int);


template string vectorOP::printVector(vector<string>&,string,string,int);
template string vectorOP::printVector(vector<int>&,string,string,int);
template string vectorOP::printVector(vector<double>&,string,string,int);
template string vectorOP::printVector(vector<float>&,string,string,int);
template string vectorOP::printVector(vector<bool>&,string,string,int);


template string vectorOP::printVector(vector<vector<string>>&,string,string);
template string vectorOP::printVector(vector<vector<int>>&,string,string);
template string vectorOP::printVector(vector<vector<double>>&,string,string);
template string vectorOP::printVector(vector<vector<float>>&,string,string);
template string vectorOP::printVector(vector<vector<bool>>&,string,string);
