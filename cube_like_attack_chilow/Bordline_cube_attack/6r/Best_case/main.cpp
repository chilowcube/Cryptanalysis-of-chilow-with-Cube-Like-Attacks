#include<iostream>
#include <algorithm>
#include<vector>
#include<string>
#include<chrono>
#include"split.h"
#include"chilow.h"
#include<iomanip>
#include<cassert>
#include<cstdio>
#include<fstream>
#include <chrono> 


using namespace std;


void print_(const vector<Polynomial> X)
{
	for(int i = 0; i < X.size(); i++)
		cout << i <<" : " <<X[i] << "  ";

	cout << endl << endl;
}


void filt_(vector<Polynomial> &X, int degree)
{
	for(int i = 0; i < X.size(); i++)
	{
		string tmp = X[i].getPoly();

		vector<string> monomial = split(tmp, "+");

		vector<string> mol;

		for(auto it : monomial)
		{
			int count  = 0;
			for(int k = 0; k < it.size(); k++)
			{
				if(it[k] == 120 || it[k] == 116)
					count++;
			}
			if(count == degree)
				mol.emplace_back(it);
		}


		if(mol.size() >= 1)
			X[i] = Polynomial(join(mol, "+"));
		else
			X[i] = Polynomial("0");

	}

}




void to_map(const vector<Polynomial> &X3, vector<unordered_map<uint64_t, Polynomial>> &r3)
{
	for(int i = 0; i < X3.size(); i++)
	{
		string tmp = X3[i].getPoly();
		vector<string> monomial = split(tmp, "+");
		for(auto it: monomial)
		{
			if(it != "")
			{
				string front, back;
				int count;
				for(int k = 0; k < it.size(); k++)
				{
					if(it[k] == 116)
					{
						count = k;
						break;
					}
				}
				front = it.substr(0, count-1);
				back = it.substr(count+1);
				vector<string> term  = split(back, "*t"); 
				uint64_t num = 0;
				for(auto tt: term)
				{
					if(tt != "")
					{
						//cout << tt << endl;
						num = num | (uint64_t(0x1)<< stoi(tt));
					}
				}

				auto it_map = r3[i].find(num);
				if (it_map == r3[i].end()) {
				    r3[i][num] = Polynomial(front);
				} else {
				    it_map->second = it_map->second + Polynomial(front);
				}
			}

		}
	}
}

void cout_(const Polynomial a, int bit)
{
	fstream wd;
	wd.open("./res/5r_"+to_string(bit) + ".txt", ios::out);
	int count_ = 0;
	int degree_min = 100;
	int degree_max = 0;

	string tmp = a.getPoly();
	vector<string> monomial = split(tmp, "+");

	count_ = monomial.size();

	for(auto ittt: monomial)
	{

        	int star_count = count(ittt.begin(), ittt.end(), '*');

        	int deg = star_count + 1; 

        	if (deg < degree_min) degree_min = deg;
        	if (deg > degree_max) degree_max = deg;

		wd << ittt << endl;
	}

	wd << "Total: " << count_ << "  terms" << endl;
	wd << "degree_min: " << degree_min << "    degree_max:" <<  degree_max  << endl;
	wd.close();

}


Polynomial laststep(const unordered_map<uint64_t, Polynomial> &a, const unordered_map<uint64_t, Polynomial> &b, int bit)
{
	
	//fstream wd;
	//wd.open("./res/5r_"+to_string(bit) + ".txt", ios::out);
	//int count_ = 0;
	//int degree_min = 10;
	//int degree_max = 0;
	Polynomial res;
	for(auto it = a.begin(); it != a.end(); it++)
	{
		auto itt = b.find(0x5555555500000000^(it->first));
		if(itt != b.end())
		{
			Polynomial c = it->second * itt -> second;

			res = res + c;

			//string tmp = c.getPoly();
			//vector<string> monomial = split(tmp, "+");

			//count_ += monomial.size();

			//for(auto ittt: monomial)
			//{

        		//	int star_count = count(ittt.begin(), ittt.end(), '*');

        		//	int deg = star_count + 1; 

        		//	if (deg < degree_min) degree_min = deg;
        		//	if (deg > degree_max) degree_max = deg;

			//	wd << ittt << endl;
			//}

		}
	}

	return res;

	//wd << "Total: " << count_ << "  terms" << endl;
	//wd << "degree_min: " << degree_min << "    degree_max:" <<  degree_max  << endl;
	//wd.close();

}



int main(int argc, char *argv[])
{
	auto start = chrono::high_resolution_clock::now();
	
	int Rounds = 5;


	Polynomial X[32];
	Polynomial T[64];
	Polynomial K[128];


        vector<int> Cube_c = {}; 
        vector<int> Cube_t = {32, 34, 36, 38, 40, 42, 44, 46,
                           48, 50, 52, 54, 56, 58, 60, 62};

	for (int i = 0; i < 32; i++) {
	    if (find(Cube_c.begin(), Cube_c.end(), i) != Cube_c.end()) {
	        X[i] = Polynomial("x" + to_string(i));
	    } else {
	        X[i] = Polynomial("0");
	    }
	}
	
	for (int i = 0; i < 64; i++) {
	    if (find(Cube_t.begin(), Cube_t.end(), i) != Cube_t.end()) {
	        T[i] = Polynomial("t" + to_string(i));
	    } else {
	        T[i] = Polynomial("0");
	    }
	}

	for(int i = 0; i < 128; i++)
	{
		K[i] = Polynomial ("k" + to_string(i));
	}

	vector<vector<Polynomial>> DX(Rounds+1, vector<Polynomial>(32));
	vector<vector<Polynomial>> DY(Rounds, vector<Polynomial>(32));
	vector<vector<Polynomial>> DZ(Rounds-1, vector<Polynomial>(32));

	vector<vector<Polynomial>> TX(Rounds, vector<Polynomial>(64));
	vector<vector<Polynomial>> TY(Rounds, vector<Polynomial>(64));
	vector<vector<Polynomial>> TZ(Rounds-1, vector<Polynomial>(64));


	vector<vector<Polynomial>> KX(Rounds, vector<Polynomial>(128));
	vector<vector<Polynomial>> KY(Rounds-1, vector<Polynomial>(128));
	vector<vector<Polynomial>> KZ(Rounds-1, vector<Polynomial>(128));

	for(int i = 0; i <32; i++)
		DX[0][i]= X[i] + K[i+96];
	for(int i = 0; i < 64; i++)
		TX[0][i] = T[i] + K[i];
	for(int i = 0; i < 128; i++)
		KX[0][i] = K[i];


	
	int r;
	for(r = 0; r < Rounds-1; r++)
	{
		//if(r == Rounds - 1)
		//{
		//	cout << "\n\n\n the last round!\n\n\n" << endl;
		//	//TY[r] = Linear(TX[r], 64);
		//	filt_(DX[r], 1 << (r-1));
		//	DY[r] = ChichiM(DX[r], 32, 32);
		//	filt_(DY[r], 1 << r);
		//	//for(int i = 0; i < 32; i++)
		//	//	DX[r+1][i] = TY[r][i] + DY[r][i]; 
		//}
		//else
		{
			//KY[r] = addConst(KX[r], r);
			//KZ[r] = Chichi(KY[r], 128);
			//KX[r+1] = Linear(KZ[r], 128);


			TY[r] = ChichiM(TX[r], 64, 64);
			filt_(TY[r], 1 << r);
			TZ[r] = LinearM(TY[r], 64, 64);

			for(int i = 0; i < 64; i++)
				//TX[r+1][i]= KX[r+1][i] + TZ[r][i];
				TX[r+1][i]= TZ[r][i];

			DY[r] = ChichiM(DX[r], 32, 64);
			filt_(DY[r], 1 << r);
			DZ[r] = LinearM(DY[r], 32, 64);
			for(int i = 0; i <32; i++)
				DX[r+1][i] = DZ[r][i] + TZ[r][i];
		}
	}



	vector<unordered_map<uint64_t, Polynomial>> r4(32);
	vector<unordered_map<uint64_t, Polynomial>> t4(64);
	to_map(DX[4], r4);
	to_map(TX[4], t4);

	vector<Polynomial> DX5(32);
	vector<Polynomial> TX5(64);

	for(int i = 0; i < 15; i++)
	{
		DX5[i] = laststep(r4[(i+1)%15], r4[(i+2)%15], i);
	}


	for(int i = 0; i < 17; i++)
	{
		DX5[i+15] = laststep(r4[(i+1)%17+15], r4[(i+2)%17+15], i+15);
	}

	for(int i = 0; i < 31; i++)
		TX5[i] = laststep(t4[(i+1)%31], t4[(i+2)%31], i+32);
	for(int i = 0; i < 33; i++)
		TX5[i+31] = laststep(t4[(i+1)%33+31], t4[(i+2)%33+31], i+32+31);


	vector<Polynomial> DX5_1(32);
	vector<Polynomial> TX5_1(64);

	DX5_1 = LinearM(DX5, 32, 32); 
	TX5_1 = LinearM(TX5, 64, 64); 


	vector<Polynomial> DX5_2(32);
	for(int i = 0; i < 32; i++)
	{
		DX5_2[i] = DX5_1[i] + TX5_1[i];
		cout_(DX5_2[i], i);
	}

	auto end = chrono::high_resolution_clock::now();

	auto duration = chrono::duration_cast<chrono::seconds>(end - start);
	cout << "running time: " << duration.count() << " s" << endl;


    	//vector<pair<int,int>> configs = {
    	//    {15, 0}, {17, 15}
    	//};

    	//for (auto [len, offset] : configs) {
    	//    for (int i = 0; i < len; i++) {
    	//        threads.emplace_back([&, i, len, offset]() {
    	//            laststep(r4[(i+1)%len + offset], r4[(i+2)%len + offset], i + offset);
    	//        });
    	//    }
    	//}

    	//for (auto& th : threads) {
    	//    if (th.joinable()) th.join();
    	//}	


        	


	//cout << "the data path:" << endl;
	//print_(DX[0]);
	//print_(DY[0]);
	//print_(DZ[0]);
	//print_(DX[1]);
	//print_(DY[1]);
	//print_(DX[2]);

	//print_(DY[r-1]);

	//cout << "the tweak path:" << endl;
	//print_(TX[0]);
	//print_(TY[0]);
	//print_(TZ[0]);
	//print_(TX[1]);
	//print_(TY[1]);

	//cout << "the key path:" << endl;
	//print_(KX[0]);
	//print_(KY[0]);
	//print_(KZ[0]);
	//print_(KX[1]);

	return 0;
}


