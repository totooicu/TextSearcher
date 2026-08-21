#ifndef VECTOROP_H
#define VECTOROP_H
#pragma once
#include <string>
#include <vector>
#include<map>
using namespace std;


#include <QString>
#include <iostream>
#include <sstream>
#include <string>
#include<QDebug>


class vectorOP {
public:

    template<class T >
    static string printVector(vector<T>& v, string name = "printVector",string separator="><",int show=1);

    template<class T >
    static string printVector(vector<vector<T>> & v, string name = "printVector2",string separator="><");

//    template<typename T>
//    void static printVectorS(vector<T> &v, string name);


    template<class T>
    static string vec2str(vector<T> vs, string fill = "");

    template <class T , class Fn>
    static void myreplace(vector<T>& v, int startIndex, int lon, Fn f);

//    template <class T , class _Fn>
//    static void myreplace(vector<T>& v, _Fn f);

    template<class T , class T2,class T3>
    static void printMap(map<T, T2,T3>& v, string name = "printMap");

    template<class T , class T2>
    static void printMap(map<T, T2>& v, string name = "printMap");

    template<class T1, class T2>
    static void map2vec(map<T1, T2>v, vector<T1>& v1, vector<T2>& v2);

    template<class T1, class T2>
    static map<T1, T2> vec2map(map<T1, T2> &v, vector<T1> v1, vector<T2> v2);
    template<class T1, class T2, class T3,class T4>
    static void map2vec(map<T1, T2, T3,T4> v, vector<T1> &v1, vector<T2> &v2);
    template<class T1, class T2, class T3,class T4>
    static map<T1, T2, T3,T4> vec2map(map<T1, T2, T3,T4> &v, vector<T1> v1, vector<T2> v2);

    template<class T>
    //lon:-1后面全部，0不截取
    vector<T> static sub(vector<T>,int head,int lon=-1);


};


class sortDesc {
public:
    template<class T>
    bool operator()(T e1, T e2) const{
        return e1 > e2;
    }
};
class sortAsc {
public:
    template<class T>
    bool operator()(T e1, T e2)const {
        return e1 < e2;
    }
};



#endif // VECTOROP_H
