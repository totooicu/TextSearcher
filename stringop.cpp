#pragma execution_character_set("utf-8")

#include "stringop.h"
#include<iostream>
#include<string>
#include<QDebug>
#include<vector>
#include<map>
#include<regex>
#include<fstream>
using namespace std;



    stringOP::stringOP() {
    }

    stringOP::stringOP(string s) {
        setTxt(s);
    }

    void stringOP::setTxt(string s) {
        txt = s;
    }

    stringOP& stringOP::operator=(string s){
    txt=s;
    return *this;
    }

    stringOP& stringOP::operator=(stringOP& s){
    *this=s;
    return *this;
    }


    //<<<<<<<找<<<结果0 1 2 3 4 ...【1】
    //<<<<<<<找<<<结果0 3 【2默认】
    /*
    vector<int>*stringOP::findPlace(string aim,int op) {
        vector<int>* vi = new(vector<int>);

        for (int i = 0; i <= int(txt.size()-aim.size()); i++) {
            int j = 0;
            for (; j < int(aim.size()); j++) {
                if (txt[i + j] != aim[j])break;
            }
            if (j == int(aim.size()))vi->push_back(i);
        }

        if (op==2)for(int i=1,j=0;i<int(vi->size());){
//            qDebug()<<i<<j<<vi->at(i)<<vi->at(j);
            if(vi->at(i)-vi->at(j)<int(aim.size()))
                vi->erase(vi->begin()+i);
            else {j=i;i++;}}

        return vi;
    }
    */


vector<int> stringOP::findPlace(const string& aim,const string&txt,int op) {
    vector<int> vi;

    const int txtSize = static_cast<int>(txt.size());
    const int aimSize = static_cast<int>(aim.size());

    vector<int> nextArray(aimSize, 0);
    getNextArray(aim, nextArray);

    int i = 0;
    int j = 0;

    while (i < txtSize) {
        if (txt[i] == aim[j]) {
            i++;
            j++;
        }

        if (j == aimSize) {
            vi.push_back(i - j);
            j = nextArray[j - 1];
        } else if (i < txtSize && txt[i] != aim[j]) {
            if (j != 0) {
                j = nextArray[j - 1];
            } else {
                i++;
            }
        }
    }

    if (op == 2) {
        for (int i = 1, j = 0; i < static_cast<int>(vi.size());) {
            if (vi[i] - vi[j] < aimSize) {
                vi.erase(vi.begin() + i);
            } else {
                j = i;
                i++;
            }
        }
    }

    return vi;
}

vector<int> stringOP::findPlaceWithRegex(const string& regexPattern,const string&txt, int op) {
    vector<int> vi;
    regex pattern(regexPattern);
    const int txtSize = static_cast<int>(txt.size());

    for (int i = 0; i < txtSize; i++) {
        string substring = txt.substr(i);
        smatch match;
        if (regex_search(substring, match, pattern) && match.prefix().length() == i) {
            vi.push_back(i);
        }
    }

    if (op == 2) {
        for (int i = 1, j = 0; i < static_cast<int>(vi.size());) {
            if (vi[i] - vi[j] < static_cast<int>(regex_replace(txt.substr(vi[j], vi[i] - vi[j]), pattern, "#").length())) {
                vi.erase(vi.begin() + i);
            } else {
                j = i;
                i++;
            }
        }
    }

    return vi;
}

void stringOP::getNextArray(const string& aim, vector<int>& nextArray) {
    int length = static_cast<int>(aim.size());
    int i = 1;
    int j = 0;
    while (i < length) {
        if (aim[i] == aim[j]) {
            j++;
            nextArray[i] = j;
            i++;
        } else {
            if (j != 0) {
                j = nextArray[j - 1];
            } else {
                nextArray[i] = 0;
                i++;
            }
        }
    }
}




    int stringOP::indexNum(vector<int>vi,int goalNum) {
        int i = 0, j = vi.size(), m = (i + j) / 2;
        if (j == 0)return -1;
        for (; i < j;) {
            if (vi[m] > goalNum)j = m - 1;
            else i = m + 1;
            m = (i + j) / 2;
        }
        if (m >=int( vi.size()))m--;
        if (vi[m] > goalNum)m--;
        return m;
    }

    vector<string> stringOP::getMid(string Laim,string Raim) {
        vector<int> Lvi = findPlace(Laim,txt);
        vector<int> Rvi = findPlace(Raim,txt);
        //printVector(*Lvi,"LVI");
        //printVector(*Rvi,"RVI");

        int LR = Raim.length();
        int l, r;
        vector<string>S ;

        for (r = 0; int(Rvi.size())>r; r++) {
            l = indexNum(Lvi, Rvi[r]);
            //printf("【%d，%d】》【%d，%d】", l, r, (*Lvi)[l], (*Rvi)[r]);

            if (l == -1)continue;
            if (l >= 0) {
                S.push_back(txt.substr(Lvi[l], Rvi[r] + LR- Lvi[l]));
                Lvi.erase(Lvi.begin()+l);
            }

        }

        return S;
    }


    //字符串_aa_bb_cc_
    // 0b 1 0 1 1 【默认为0】
    // 未匹配到返回原串 包含regalS 包含结尾 包含开头
    vector<string> stringOP::getMid2( string regoalS, int op) {

        vector<int>P =findPlace(regoalS,txt,2);

        vector<string>S ;
//        if(txt.size()==0)return S;
        if(P.size()==0){
            S.push_back(txt);
            return S;}
        if ((op & 1) != 0){
            S.push_back(txt.substr(0, P[0]));
            qDebug()<<"include head";
        }
        if(P.size()==1)return S;
        int i = 0;
        for (; i < int(P.size() - 1); i++) {
            S.push_back(txt.substr(P[i], P[i+1]-P[i]));
        }
        if ((op & 2) != 0){
            S.push_back(txt.substr(P[i]));
            qDebug()<<"include tail";
        }


        if((op & 4) != 0){

            for(int i=0;i<int(S.size());i++)


            if(S.at(i).size()>=regoalS.size())
                S.at(i)=stringOP::replace2(S.at(i),regoalS,"");
//            qDebug()<<QString::fromStdString(S.at(i));
//qDebug()<<"no include regoalS";
        }

//        qDebug()<<"the end";
        return S;
    }

    string stringOP::eraseString(int begin,int lon,vector<string> delS) {
        vector<int>PL ;
        vector<int>PR;
        int s;
        map<int, int,sortAsc>m1;
        for (string i : delS) {
            PR=PL = findPlace(i,txt);
            s= i.size();
//        vectorOP::myreplace(PR,0,PR.size(), [s](int e) ->int{return e + s; });
         for(int ii=0;ii<int(PR.size());ii++)
             PR[ii]+=s;

//        vectorOP::vec2map(m1, PL, PR);
        for (int ii = 0; ii < int(PL.size()); ii++)m1[PL[ii]] = PR[ii];
        }
        PL.clear(); PR.clear();
//        vectorOP::map2vec(m1, PL, PR);
        for(auto i:m1){
            PL.push_back(i.first);
            PR.push_back(i.second);
        }

//        vectorOP::printVector(PL,"PL");
//        vectorOP::printVector(PR, "PR");
        //printMap(m1);
        PL.push_back(txt.size());
        PR.insert(PR.begin(), 0);
        string NS ;
        for (int i = 0; i < int(PR.size()); i++) {
            NS=NS+(txt.substr(PR[i], PL[i] - PR[i]));
        }
        txt =NS;
        return NS;
    }

    string stringOP::eraseString(vector<string> delS) {
        return eraseString(0, txt.size(), delS);
    }
    string stringOP::eraseString(int begin, int lon, string delS) {
        vector<string>a ;
        a.push_back(delS);
    return eraseString(begin, lon, a);
    }
    string stringOP::eraseString(string delS) {
        return eraseString(0, txt.size(), delS);
    }

    string stringOP::eraseChar(int begin, int lon, string delC) {
        string NS ;
        for(auto i:txt)
        if(delC.find(i)==string::npos)NS.push_back(i);
        txt = NS;
        return NS;
    }

    string stringOP::eraseChar( string delC) {
        return eraseChar(0,txt.size(), delC);

    }

    string stringOP::eraseChar2(int begin, int lon, string includeC) {
        string NS ;
        for (auto i : txt)
            if (includeC.find(i) != string::npos)NS.push_back(i);
        txt = NS;
        return NS;
    }

    string stringOP::eraseChar2(string includeC) {
        return eraseChar2(0, txt.size(), includeC);

    }



    void stringOP::print(string name ) {

        qDebug()<<endl << QString::fromStdString(name)<< ">>>[" << QString::fromStdString(txt) <<"]"<< endl;
    }


    //未完成
    vector<int> stringOP::reFindPlace(string aim) {
        regex r(aim);
        vector<int> vi ;
        int p=0;
        while (p= regex_search(txt.substr(p+1), r)) {
            vi.push_back(p);
            cout << p <<endl;
        }
        return vi;
    }

    string stringOP::replace(string oldS,string newS){
        vector<int> P=this->findPlace(oldS,txt);
//        vectorOP::printVector(P,"P");
        int s=oldS.size();
        int D=0,d=newS.size()-oldS.size();
        for(int i : P){
//qDebug()<<QString::fromStdString(to_string(i+D*d))<<"|||"<<QString::fromStdString(to_string(i+s+D*d));
//qDebug()<<QString::fromStdString(txt.substr(D*d+i,s));
txt.replace(txt.begin()+i+D*d,txt.begin()+i+s+D*d,newS);
D++;
//print(to_string(i));
        }

//        qDebug()<<QString::fromStdString(txt);
        return txt;
    }


    string stringOP::replace2(string S,string oldS,string newS){
        stringOP sop(S);
        sop.replace(oldS,newS);
        return sop.txt;
    }



    vector<vector<string>> stringOP::ESCS2List(string sourceS)
    {   stringOP SS(sourceS);
//        SS.print();
        vector<string> listDic={" ","\\","'","\n",","};
//        SS.replace("[]","NULL");
        SS.eraseChar("[]");
//        SS.print("erase");

        int j=30;
        string dels;

        //替换回符号
        for(string i :listDic){
            dels="%"+to_string(j);
//            qDebug()<<"dels"<<QString::fromStdString(dels)<<QString::fromStdString(i);
            sourceS=SS.replace(dels,i);
            j++;
        }
        SS.print("替换回符号/30");

        vector<string> slist=SS.getMid2(">>>",0b1111);
      vectorOP::printVector(slist,">>>分割");

//      替换回符号/20,以,分割
        vector<vector<string>>Slist;
        stringOP ss;
        qDebug()<<"slist.size>>>"<<slist.size();
        for(string i:slist){
            ss.txt=i;
            qDebug()<<"N\n"<<QString::fromStdString(i) ;


            vector<string> vs=ss.getMid2(",",0b1111);
            /*111
             * 【1…… 2022-12-20%2010:45:38…… name…… ……
             * 19…… 2022-12-20%2009:09:36…… name1…… null……
             * 29…… 2022-12-20%2015:51:23…… 考研单词…… ……
             * 30…… 2022-12-20%2015:51:35…… 高考单词…… ……
             * 31…… 2022-12-20%2015:51:47…… 重点单词…… ……
             * 32…… 2022-12-20%2015:52:01…… 阅读单词…… ……
             * 33…… 2022-12-20%2015:52:11…… 听力单词…… …………】
              110
              【2022-12-20%2010:45:38…… name…… …… 19……
                2022-12-20%2009:09:36…… name1…… null…… 29……
                2022-12-20%2015:51:23…… 考研单词…… …… 30……
                2022-12-20%2015:51:35…… 高考单词…… …… 31……
                2022-12-20%2015:51:47…… 重点单词…… …… 32……
                2022-12-20%2015:52:01…… 阅读单词…… …… 33……
                2022-12-20%2015:52:11…… 听力单词…… …………】
*/

            Slist.push_back(vs);
            vectorOP::printVector(vs,"vs");

//            qDebug()<<"A\n"<<QString::fromStdString(Slist.end()->c_str()) ;
//            vectorOP::printVector(Slist[a]);

        }
        qDebug()<<"Slist.push_back(vs)ed";
        vectorOP::printVector(Slist,"Slist");
        qDebug()<<"printed";

        j=20;
        vector<vector<string>>SSlist;
        for(auto i : listDic){
            dels="%"+to_string(j);
            for(int k=0;k<int(Slist.size());k++){
//                qDebug()<<QString::fromStdString(i)<<k<<(typeid(Slist[k])!=typeid(int));
                if (typeid(Slist[k])!=typeid(int)){
                    for(int h=0;h<int(Slist[k].size());h++){

                        Slist[k][h]=stringOP::replace2(Slist[k][h],dels,i);
                    }
                }
            }
            j++;
        }
vectorOP::printVector(Slist,"SSList");
        return Slist;


    }
    vector<string> stringOP::readTxtByLine(string file)
    {
        ifstream infile;
        string s;
        vector<string>ss;
        infile.open(file.data());   //将文件流对象与文件连接起来
        if(!infile.is_open()){
            cout<<">>>read fial:"<<file;
            return ss;
        }
        assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行


        while(getline(infile,s))
        {
//            cout<<s<<endl;
            ss.push_back(s);
        }
        infile.close();             //关闭文件输入流
        return ss;
    }

    string stringOP::readTxtByChar(string file)
    {
        ifstream infile;
        infile.open(file.data());   //将文件流对象与文件连接起来
        if(!infile.is_open()){
            cout<<">>>read fial:"<<file;
            return ">>>read fial:"+file;
        }
        assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行

        char c;
        string text="";
        infile >> noskipws;
        while (!infile.eof())
        {
            infile>>c;
            text.push_back(c);
//            cout<<c<<endl;

        }
        infile.close();             //关闭文件输入流
        return text;
    }



    template<class T>
    vector<vector<T>> stringOP::reShape(vector<T> List, int w)
    {   vector<vector<T>> bookMsg;
        int i=0;
        qDebug()<<"List.size()>>>"<<List.size();
        for(;i<int(List.size()/4*4);i+=w){
            bookMsg.push_back(vectorOP::sub(List,i,w));
        }
        return bookMsg;
    }



   template vector<vector<int>> stringOP::reShape(vector<int> , int );
    template vector<vector<double>> stringOP::reShape(vector<double> , int );
    template vector<vector<string>> stringOP::reShape(vector<string> , int );
    template vector<vector<float>> stringOP::reShape(vector<float> , int );
    template vector<vector<bool>> stringOP::reShape(vector<bool> , int );
