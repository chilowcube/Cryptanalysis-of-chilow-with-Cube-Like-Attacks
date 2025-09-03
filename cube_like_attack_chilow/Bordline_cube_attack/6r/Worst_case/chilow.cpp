#include"split.h" 
#include"chilow.h"
#include<chrono>
#include<thread>
#include<mutex>
#include<map>
#include"polynomial.h"
#include<cassert>
#include<string>

#define DEBUG 0

using namespace std;

mutex workmutex;
mutex smutex;

//extern int THREADNUM = 32;


vector<int> L32{11, 5, 9, 12};
vector<int> L64{3, 1, 26, 50};
vector<int> L128{17, 7, 11, 14};

vector<Polynomial> Linear( const vector<Polynomial> & X, int lenth)
{
	vector<Polynomial> Y(lenth);
	for(int i = 0; i< lenth; i++)
	{
		if(lenth == 32)
			Y[i] = X[ ( L32[0]*i+L32[1]) % lenth] + X[ ( L32[0]*i+L32[2]) % lenth] + X[ ( L32[0]*i+L32[3]) % lenth];
		else if(lenth == 64)
			Y[i] = X[ ( L64[0]*i+L64[1]) % lenth] + X[ ( L64[0]*i+L64[2]) % lenth] + X[ ( L64[0]*i+L64[3]) % lenth];
		else
			Y[i] = X[ ( L128[0]*i+L128[1]) % lenth] + X[ ( L128[0]*i+L128[2]) % lenth] + X[ ( L128[0]*i+L128[3]) % lenth];
	}

	return Y;

}


vector<Polynomial> LinearM(const vector<Polynomial> & X, int length, int THREADNUM)
{
    vector<Polynomial> Y(length);

    auto task = [&](int start, int end) {
        for (int i = start; i < end; i++) {
            if (length == 32) {
                Y[i] = X[(L32[0]*i + L32[1]) % length]
                     + X[(L32[0]*i + L32[2]) % length]
                     + X[(L32[0]*i + L32[3]) % length];
            } else if (length == 64) {
                Y[i] = X[(L64[0]*i + L64[1]) % length]
                     + X[(L64[0]*i + L64[2]) % length]
                     + X[(L64[0]*i + L64[3]) % length];
            } else {
                Y[i] = X[(L128[0]*i + L128[1]) % length]
                     + X[(L128[0]*i + L128[2]) % length]
                     + X[(L128[0]*i + L128[3]) % length];
            }
        }
    };

    vector<thread> threads;
    int step = (length + THREADNUM - 1) / THREADNUM;  
    for (int t = 0; t < THREADNUM; t++) {
        int start = t * step;
        int end   = min(start + step, length);
        if (start < end) {
            threads.emplace_back(task, start, end);
        }
    }

    for (auto &th : threads) th.join();

    return Y;
}


vector<Polynomial> Chichi( const vector<Polynomial> & X, int lenth)
{
	vector<Polynomial> Y(lenth);

	int a = lenth/2 - 1;
	for(int i = 0; i < a; i++)
		//Y[i] = X[i] + X[(i+1)%a]*X[(i+2)%a] + X[(i+2)%a];
		Y[i] = X[(i+1)%a]*X[(i+2)%a];
	int b = lenth/2 + 1;
	for(int i = 0; i < b; i++)
		//Y[i+a] = X[i+a] +X[(i+1)%b+a]*X[(i+2)%b+a] + X[(i+2)%b+a];
		Y[i+a] = X[(i+1)%b+a]*X[(i+2)%b+a];

	//int m = lenth/2;
	//Y[m-3] = Y[m-3] + X[m] +X[m-3];
	//Y[m-2] = Y[m-2] + X[m-1] + X[m-2];
	//Y[m-1] = Y[m-1] + X[m-3] + X[m-1] + X[m];
	//Y[m] = Y[m] + X[m-2] + X[m];

	return Y;
}



vector<Polynomial> ChichiM(const vector<Polynomial> & X, int length, int THREADNUM)
{
    vector<Polynomial> Y(length);

    int a = length / 2 - 1;
    int b = length / 2 + 1;
    int m = length / 2;

    vector<thread> threads;

    auto task1 = [&](int start, int end) {
        for (int i = start; i < end; i++) {
            //Y[i] = X[i] + X[(i+1)%a] * X[(i+2)%a] + X[(i+2)%a];
            Y[i] = X[(i+1)%a] * X[(i+2)%a];
        }
    };

    auto task2 = [&](int start, int end) {
        for (int i = start; i < end; i++) {
            //Y[i+a] = X[i+a] + X[(i+1)%b+a] * X[(i+2)%b+a] + X[(i+2)%b+a];
            Y[i+a] = X[(i+1)%b+a] * X[(i+2)%b+a];
        }
    };

    int stepA = (a + THREADNUM - 1) / THREADNUM;
    for (int t = 0; t < THREADNUM; t++) {
        int start = t * stepA;
        int end   = min(start + stepA, a);
        if (start < end)
            threads.emplace_back(task1, start, end);
    }

    int stepB = (b + THREADNUM - 1) / THREADNUM;
    for (int t = 0; t < THREADNUM; t++) {
        int start = t * stepB;
        int end   = min(start + stepB, b);
        if (start < end)
            threads.emplace_back(task2, start, end);
    }

    for (auto &th : threads) th.join();

    //Y[m-3] = Y[m-3] + X[m]   + X[m-3];
    //Y[m-2] = Y[m-2] + X[m-1] + X[m-2];
    //Y[m-1] = Y[m-1] + X[m-3] + X[m-1] + X[m];
    //Y[m]   = Y[m]   + X[m-2] + X[m];

    return Y;
}




//
//vector<Polynomial> ChichiM( const vector<Polynomial> & X, int lenth)
//{
//	vector<Polynomial> Y(lenth);
//
//	int a = lenth/2 - 1;
//	for(int i = 0; i < a; i++)
//		Y[i] = X[i] + X[(i+1)%a]*X[(i+2)%a] + X[(i+2)%a];
//	int b = lenth/2 + 1;
//	for(int i = 0; i < b; i++)
//		Y[i+a] = X[i+a] +X[(i+1)%b+a]*X[(i+2)%b+a] + X[(i+2)%b+a];
//
//	int m = lenth/2;
//	Y[m-3] = Y[m-3] + X[m] +X[m-3];
//	Y[m-2] = Y[m-2] + X[m-1] + X[m-2];
//	Y[m-1] = Y[m-1] + X[m-3] + X[m-1] + X[m];
//	Y[m] = Y[m] + X[m-2] + X[m];
//
//	return Y;
//}
//



vector<Polynomial> addConst( vector<Polynomial> & X, int r )
{
    vector<Polynomial> Y(128);

    int constant = r ^ ( 1 << (r+4));
    for(int i = 0; i < 128; i++)
    {
            if(i < 96)
                    Y[i] = X[i];
            else
                    Y[i] = X[i] + Polynomial ( to_string(((constant >> (i-96)) & 0b1)) ) ;
    }

    return Y;
}

