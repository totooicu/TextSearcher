#ifndef STRINGOP_H
#define STRINGOP_H

#include<iostream>
#include<string>
#include<vector>
#include<regex>
#include <algorithm>
#include"vectorop.h"
using namespace std;
#pragma once

class stringOP {

public:
    string txt;

    stringOP() ;
    stringOP(string s) ;

    void setTxt(string s) ;
    vector<int> static findPlace(const string &aim, const string &txt , int op=2);
    vector<int> static findPlaceWithRegex(const string &regexPattern, const string &txt , int op = 2);
    void static getNextArray(const string &aim, vector<int> &nextArray);

    int indexNum(vector<int>vi,int goalNum) ;
    vector<string> getMid(string Laim,string Raim) ;

    //_aa_bb_cc_->aa,bb,cc
    // aa_bb_cc
   //00b包含开头与结尾aa,cc

    vector<string> getMid2( string regoalS, int op) ;

    string eraseString(int begin,int lon,vector<string> delS) ;
    string eraseString(vector<string> delS) ;
    string eraseString(int begin, int lon, string delS);
    string eraseString(string delS);
    string eraseChar(int begin, int lon, string delC) ;
    string eraseChar( string delC) ;

    string eraseChar2(int begin, int lon, string includeC) ;

    string eraseChar2(string includeC) ;


    void print(string name = "stringOP");

    //未完成
    vector<int> reFindPlace(string aim) ;

    string replace(string oldS, string newS);

    stringOP &operator =(string s);
    stringOP &operator =(stringOP &s);
    string static replace2(string S, string oldS, string newS);
    vector<vector<string> > ESCS2List(string sourceS);

    template<class T>
    vector<vector<T> > reShape(vector<T> List, int w=1);


    string static readTxtByChar(string file);
    vector<string> static readTxtByLine(string file);
};
#endif // STRINGOP_H
