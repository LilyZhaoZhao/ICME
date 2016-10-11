/*
  从深圳市所有的ap中随机选取多个点，分别以每个点为中心，
    取给定长度半径内的区域内的所有ap。
*/
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

double minDistance = 0.01;
double minLat = 22.45773;
double maxLat = 22.858906;
double minLon = 113.768345;
double maxLon = 114.60889;


//map<string, string> filter;
map<string, pair<float, float> > macLonlat;
map<string, int> macCtgy;//迭代更新，最终包含了预测类别
map<string, int> macCtgyReal;//全部的mac真实类别
map<string, int> macCtgyTest;//所有未知类型的mac
typedef map<string, vector<int> > distri;
distri macCtgyDistribution;


typedef pair<string, pair<float, float> > PAIR;

bool cmp_by_value(const pair<string, float> & m1, const pair<string, float> & m2) {
        return m1.second > m2.second;
}


void getRegion(string s, double pLon, double pLat, int num)
{

  int count = 0, i=0;
  float lon=0, lat=0;
  string istr, token, line, mac, catagory;

  std::stringstream ss;
  ss << num;
  ss >> istr;
  string s1 = "regionA"+istr+"_intCatagory";
  ofstream ofs(s1.c_str());

  ifstream ifs(s.c_str());
  while(getline(ifs, line)){
     istringstream iss(line);
     count=0;
     while(getline(iss, token, '|')){
         count ++;
         switch(count){
         case 2:
             mac = token;
             break;
         case 3:
             lon = atof(token.c_str());
             break;
         case 4:
             lat = atof(token.c_str());
             break;
         case 8:
             catagory = token;
             //cout<< catagory<<endl;
             break;
         }
     }
     //cout<<"lon,lat:  "<<lon<<','<<lat<<endl;
     //cout<<"pLon,pLat:  "<<pLon<<','<<pLat<<endl;

     if(abs(lon-pLon)<=minDistance && abs(lat-pLat)<=minDistance)
     {
       //cout<<line;
       ofs << line <<endl;
     }
     }
     ofs.close();
     ifs.close();
}


double random(double start, double end)
{
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

vector<double> getPoint(double min, double max)
{
    vector<double> v;
    srand(unsigned(time(0)));
    for(int icnt = 0; icnt != 10; ++icnt)
        v.push_back(random(min, max));
    return v;
}

int main(int argc, char* argv[])
{

    vector<double> vLon = getPoint(minLon, maxLon);
    vector<double> vLat = getPoint(minLat, maxLat);
    for(int i=0; i<10; i++)
    {
      cout<<vLon[i]<<','<<vLat[i]<<endl;
    }
    string s = argv[1];// data/shenzhen/safe_wifi_poi_shenzhen_intCatagory
    for(int i=0; i<10; i++)
    {
        getRegion(s, vLon[i], vLat[i], i);
    }


    return 0;
}
