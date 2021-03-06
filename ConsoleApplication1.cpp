// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "config.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

using namespace std;
using std::vector;
using std::string;



// GLOBALS
vector<string> volList;
vector<string> dates;
vector<string> hosts;
vector<string> jv1;
vector<string> jv2;
vector<string> jv3;

vector<string> sab1;
vector<string> sab2;

// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from left & right
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
	return ltrim(rtrim(s, t), t);
}


void populateVolunteers(void)
{
	int a = 0;
	ifstream myfile("shabab.txt");
	if (!myfile)
	{
		cout << "Error opening output file" << endl;
		system("pause");
		return;
	}

	while (!myfile.eof())
	{
		getline(myfile, volList[a]);
		volList[a] = trim(volList[a]);
		//cout << volList[a] << "\n";
		a++;
	}
}

void populateDates(void)
{
	int a = 0;
	ifstream myfile("dates.txt");
	if (!myfile)
	{
		cout << "Error opening output file" << endl;
		system("pause");
		return;
	}

	while (!myfile.eof())
	{
		getline(myfile, dates[a]);
		dates[a] = trim(dates[a]);
		a++;
	}
}

void populateHosts(void)
{
	int a = 0;
	ifstream myfile("hosts.txt");
	if (!myfile)
	{
		cout << "Error opening output file" << endl;
		system("pause");
		return;
	}

	while (!myfile.eof())
	{
		getline(myfile, hosts[a]);
		hosts[a] = trim(hosts[a]);
		a++;
	}
}


void populateCohosts(void)
{
	int a = 0;
	ifstream myfile("cohosts.txt");
	if (!myfile)
	{
		cout << "Error opening output file" << endl;
		system("pause");
		return;
	}

	while (!myfile.eof())
	{
		getline(myfile, jv1[a]);
		jv1[a] = trim(jv1[a]);
		a++;
	}
}


void writeOutput(void)
{
	std::ofstream ofs;
	ofs.open("out.csv", std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < NUM_MIQAATS; i++)
	{
		ofs << jv1[i] << "," << jv2[i] << "," << jv3[i] << ",";
		if (DO_SABEEL)
		{
			ofs << sab1[i] << "," << sab2[i] << ",";
		}
		ofs << "\n";
	}
	ofs.close();

}

bool canAssign(string vol, int idx)
{
	// same day check
	if (hosts[idx].find(vol) != std::string::npos)
	{
		return false;
	}
	if (jv1[idx].find(vol) != std::string::npos)
	{
		return false;
	}
	if (jv2[idx].find(vol) != std::string::npos)
	{
		return false;
	}
	if (jv3[idx].find(vol) != std::string::npos)
	{
		return false;
	}
	if (sab1[idx].find(vol) != std::string::npos)
	{
		return false;
	}
	if (sab2[idx].find(vol) != std::string::npos)
	{
		return false;
	}

	// prev day check
	if (idx != 0)
	{
		if (jv1[idx - 1].find(vol) != std::string::npos)
		{
			return false;
		}
		if (jv2[idx - 1].find(vol) != std::string::npos)
		{
			return false;
		}
		if (jv3[idx - 1].find(vol) != std::string::npos)
		{
			return false;
		}
		if (sab1[idx - 1].find(vol) != std::string::npos)
		{
			return false;
		}
		if (sab2[idx - 1].find(vol) != std::string::npos)
		{
			return false;
		}
	}

	// next day check
	if (idx < (NUM_MIQAATS - 1))
	{
		if (hosts[idx + 1].find(vol) != std::string::npos)
		{
			return false;
		}
	}

	// kitabi check
	if (vol.find("Kitabi") != std::string::npos)
	{
		if ((dates[idx].find("Sat") == std::string::npos) &&
			(dates[idx].find("Sun") == std::string::npos))
		{
			return false;
		}
	}
	return true;

}
void assignJV(vector<string>& v1, int idx)
{
	string temp;
	for (int i = 0; i < NUM_VOLUNTEERS; i++)
	{
		if (canAssign(volList[i], idx))
		{
			v1[idx] = volList[i];
			temp = volList[i];
			volList.erase(volList.begin() + i);
			volList.push_back(temp);
			return;
		}
	}
}

int main()
{
	volList.resize(NUM_VOLUNTEERS,"");
	dates.resize(NUM_MIQAATS, "");
	hosts.resize(NUM_MIQAATS, "");
	jv1.resize(NUM_MIQAATS, "");
	jv2.resize(NUM_MIQAATS, "");
	jv3.resize(NUM_MIQAATS, "");
	sab1.resize(NUM_MIQAATS, "");
	sab2.resize(NUM_MIQAATS, "");

	populateVolunteers();
	populateDates();
	populateHosts();
	populateCohosts();
	// using built-in random generator:
	std::random_shuffle(volList.begin(), volList.end());

	//cout << "Shabab List \n";
	//cout << "Number:" << tempVol.size() <<"\n";
	//for (int i = 0; i < tempVol.size();i++)
	//{		
	//	cout << tempVol[i] << "\n";
	//}

	for (int i = 0; i < NUM_MIQAATS; i++)
	{
		if ((i != SKIP_JV1_A) && (i != SKIP_JV1_B) && (jv1[i].length() == 0))
		{
			assignJV(jv1, i);
		}
		if ((i != SKIP_JV2_A) && (i != SKIP_JV2_B))
		{
			assignJV(jv2, i);
		}
		if ((i != SKIP_JV3_A) && (i != SKIP_JV3_B))
		{
			if ((hosts[i] == "") || (hosts[i] == "POTLUCK"))
			{
				assignJV(jv3, i);
			}
		}

		if (DO_SABEEL)
		{
			assignJV(sab1, i);
			assignJV(sab2, i);
		}
	}

	writeOutput();
	

}

