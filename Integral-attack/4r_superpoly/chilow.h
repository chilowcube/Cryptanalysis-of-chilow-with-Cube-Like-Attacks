#ifndef __ChilowHATF_H__
#define __ChilowHATF_H__

#include"polynomial.h"
#include<vector>
#include<map>

using namespace std;

extern int THREADNUM;


vector<Polynomial> Linear(const vector<Polynomial> & X, int lenth);

vector<Polynomial> LinearM(const vector<Polynomial> & X, int length, int THREADNUM);
vector<Polynomial> Chichi(const vector<Polynomial> & X, int lenth);
vector<Polynomial> ChichiM(const vector<Polynomial> & X, int lenth, int THREADNUM);

vector<Polynomial> addConst( vector<Polynomial> & X, int r );
//class AsconHATF:public HATF
//{
//    public:
//        AsconHATF( int ORDER, int size, int rd, int level = 0 );
//
//        void init( const vector<int> & D );
//
//        void initValue( vector<int> & D, int v  );
//
//        void initOne( vector<int> & D  );
//        void initMid( vector<int> & D  );
//        void initVec( vector< vector< int> > & D, vector<int> & row  );
//
//        void RunSboxAC( int conIndex );
//        void RunMatrix();
//        void RunFirstSboxAC( int conIndex );
//        void RunMatrixS( int conIndex );
//
//        void RunPA( int conIndex );
//        void RunPB();
//        void RunRF( int conIndex );
//        void initUV( int **UV, int rr  );
//        void initValueMap( vector<int> & D, const map< string, string > & MAP );
//
//
//        void RunSboxPN();
//        void RunSboxPNLast();
//        void resetBM();
//        //void Sub( int R, int START = 0, int END = 5, bool flag = false );
//        //void SubNoBias( int R, int START = 0, int END = 5, bool flag = false );
//        //void Sub_1( int R );
//        //void Sub_1_Bias( int R );
//        map<string, string> getValueMap( const vector<int> & D, int value );
//        map<string, string> getValueMap1( int value );
//
//};

#endif
