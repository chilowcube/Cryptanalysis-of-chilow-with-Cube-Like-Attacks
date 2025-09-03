#include<iostream>
#include<sstream>
#include<vector>
#include<cmath>
#include<tuple>
#include<mutex>
#include<thread>
#include<bitset>
#include<set>
#include<algorithm>
#include <iomanip>
#include"thread_pool.h"
#include<chrono>
#include<ctime>
#include <stdint.h>
#include <random>
#include <climits>


using namespace std;
using namespace thread_pool;
mutex Cor_Mutex;



bitset<32> linear_32(bitset<32> A)
{
	bitset<32> B(0x0);
	for(int i = 0; i < 32; i++)
	{
		B[i] = A[(11*i+5)%32] ^ A[(11*i+9)%32] ^ A[(11*i+12)%32];
	}
	return B;
}

bitset<64> linear_64(bitset<64> A)
{
	bitset<64> B(0x0);
	for(int i = 0; i < 64; i++)
	{
		B[i] = A[(3*i+1)%64] ^ A[(3*i+26)%64] ^ A[(3*i+50)%64];
	}
	return B;
}

bitset<128> linear_128(bitset<128> A)
{
	bitset<128> B(0x0);
	for(int i = 0; i < 128; i++)
	{
		B[i] = A[(17*i+7)%128] ^ A[(17*i+11)%128] ^ A[(17*i+14)%128];
	}
	return B;
}


template <size_t N>
bitset<N> nolinear_N(const bitset<N>& A)
{
    static_assert(N >= 8, "Bitset size must be at least 8.");

    constexpr size_t n = N;
    constexpr size_t m = N / 2;

    bitset<N> B;

    for (size_t i = 0; i < m - 1; ++i)
    {
        B[i] = A[i] ^ ((~A[(i + 1) % (m - 1)]) & A[(i + 2) % (m - 1)]);
    }

    for (size_t i = 0; i < m + 1; ++i)
    {
        size_t idx = ((i + 1) % (m + 1)) + m - 1;
        size_t idx2 = ((i + 2) % (m + 1)) + m - 1;
        B[i + m - 1] = A[i + m - 1] ^ ((~A[idx]) & A[idx2]);
    }

    if (m >= 4)
    {

	B[m-3] = B[m-3] ^ A[m] ^ A[m-3];
	B[m-2] = B[m-2] ^ A[m-1] ^ A[m-2];
	B[m-1] = B[m-1] ^ A[m-3] ^ A[m] ^ A[m-1];
	B[m] = B[m] ^ A[m] ^ A[m-2];
    }

    return B;
}



bitset<32> encrypt_(bitset<32> C, bitset<64> T, bitset<128> K, int Round)
{
	bitset<128> KD;
	bitset<32> D;
	bitset<64> TD;

	for(int i = 0; i < 32; i++)
		D[i] = C[i] ^ K[64+i];
	for(int i = 0; i < 64; i++)
		TD[i] = T[i] ^ K[i];

	for(int r = 0; r < Round -1; r++)
	{
		int constant = r ^ ( 1 << (r+4));
		for(int i = 0; i < 128; i++)
		{
			if(i < 96)
				KD[i] = K[i];
			else
				KD[i] = K[i] ^ ((constant >> (i-96)) & 0b1);
		}
		K = nolinear_N(KD);
		KD = linear_128(K);
		K = KD;

		//tweak
		T = nolinear_N(TD);
		TD = linear_64(T);

		//data
		C = nolinear_N(D);
		D = linear_32(C);
		for(int i = 0; i < 32; i++)
			D[i] = D[i] ^ TD[i];
		for(int i = 0; i < 64; i++)
			TD[i] = TD[i] ^ K[i];
	}
	T = linear_64(TD);
	C = nolinear_N(D);
	
	for(int i = 0; i < 32; i++)
		C[i] = C[i] ^ T[i];

	return C;
}


void Worker1(uint64_t value, const vector<int>& cube_c, const vector<int> & cube_t, const bitset<32> &C, const bitset<64> &T, const bitset<128>& K, int Round, bitset<32>& res)
{
	bitset<128> KK = K;
	random_device rd;
	mt19937_64 gen(rd());
	uniform_int_distribution<unsigned long long> dist(0, ULLONG_MAX);
	
	unsigned long long r1 = dist(gen);
	unsigned long long r2 = dist(gen);
	
	for (int i = 0; i < 64; i++) {
	    KK[i] = (r1 >> i) & 1ULL;
	}
	for (int i = 0; i < 64; i++) {
	    KK[i + 64] = (r2 >> i) & 1ULL;
	}
	
	KK[7] = 1;
	KK[16] = 0;
	KK[22] = 0;
	KK[0] = 1;
	KK[46] = 1;
	KK[50] = 0;
	KK[56] = 0;

	bitset<32> CC = C;
	bitset<64> TT = T;
	for(int i = 0; i <33; i++)
	{
		if(i < 8)
			CC[cube_c[i]] = (value >> i) & 0b1;
		else
			TT[cube_t[i-8]] = (value >> i) & 0b1;
	}

	 bitset<32> R = encrypt_(CC, TT, KK, Round);

	 lock_guard< mutex > guard ( Cor_Mutex );
	 res = res ^ R;
}

void Worker2(uint64_t value, const vector<int> &cube_c, const vector<int> & cube_t, const bitset<32> &C, const bitset<64> &T, const bitset<128>& K, int Round, bitset<32>& res)
{
	bitset<32> CC = C;
	bitset<64> TT = T;

	TT[0] = 1;
	for(int i = 0; i <33; i++)
	{
		if(i < 8)
			CC[cube_c[i]] = (value >> i) & 0b1;
		else
			TT[cube_t[i-8]] = (value >> i) & 0b1;
	}
	 bitset<32> R = encrypt_(CC, TT, K, Round);

	 lock_guard< mutex > guard ( Cor_Mutex );
	 res = res ^ R;
}





int main( int argc, char * argv[] )
{

    int ROUND = 6;


    bitset<32> C(0x0);
    bitset<64> T(0x0);
    bitset<128> K;


    vector<int> cube_c = {6, 13 ,15, 17, 19, 21, 23, 28};
    vector<int> cube_t = {1, 3, 4, 6, 8, 10, 12, 15, 17, 19, 21, 27, 29,33, 35, 37, 42, 47, 49, 51, 53, 55, 57, 59, 61};
    

    ThreadPool thread_pool{128};
    cout << "Number of threads: " << thread_pool.num_threads() << endl;
    vector<std::future<void>> futures;


    vector<int> flag(32, 0);

    for(int i = 0; i < 10; i++)
    {

    	bitset<32> res(0x0);
    	futures.clear();

    	for(uint64_t j = 0; j <= 0b111111111111111111111111111111111; j++)
    	{
    	    //bitset<32> tmp(0x0);
    	    futures.emplace_back(thread_pool.Submit( Worker1, j, cube_c, cube_t, C, T, K, ROUND, ref(res)) );
    	    //res = res ^ tmp;
    	    if (futures.size() >= 100000) 
    	    {
    	    	for (auto& f : futures) f.get();  
    	    	futures.clear();                  
    	    }
    	}

    	for(int i = 0; i < 32; i++)
    	{
    	        if(res[i] == 0)
    	    	    flag[i] += 1;
    	}
    }

    for(int i = 0; i < 32; i++)
	cout << i << " : "<< flag[i] << endl;

    return 0;
}

