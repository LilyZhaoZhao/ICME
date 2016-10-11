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

using namespace std;
map<string, string > filter;
map<string, vector<int> > apTime;
//map<string, vector<string> > apUser;
//map<string, vector<int> > apCatg;

typedef pair<string, set<string> > PAIR;

bool cmp_by_value(const PAIR& lhs, const PAIR& rhs){
    return lhs.second.size() > rhs.second.size();
}

//得到poi mac集
void getPoi(string poi){
  ifstream ifs1(poi.c_str());// categoryValidation/mac1617181920

  string mac, lat, lon, ssid, addr1, addr2, addr3;
  string token,line;
  int count = 0;
//  vector<string> v;
  while(getline(ifs1, line)){
      istringstream iss(line);
      count=0;
//      v.clear();
      while(getline(iss, token, '|')){
          count ++;
          switch(count){
//              case 1:
//                  ssid = token;
//                  break;
          case 2:
              mac = token;
              break;
/*              case 3:
              lon = token;
              break;
          case 4:
              lat = token;
              break;
          case 5:
              addr1 = token;
              break;
          case 6:
              addr2 = token;
              break;
*/
          case 7:
              addr3 = token;
              break;
          }
      }
/*
      v.push_back(ssid);
      v.push_back(lon);
      v.push_back(lat);
      v.push_back(addr1);
      v.push_back(addr2);
      v.push_back(addr3);
      filter[mac] = v;
*/
      filter[mac] = addr3;
  }
  ifs1.close();

}


//用于获取：每天每个小时每个ap的连接次数
void fact(string whichDay){

      ifstream ifs2(whichDay.c_str());// TecentData/wifi_conn/20150316/safe_wifi_connect_sample_export

      string token,line;
      int count = 0;
      string end_time, guid, bssid, connect_time;
      time_t t;
      //int catalogint;
      vector< int > arry(24, 0);//初始化ap类别频次矩阵，全部值初始化为0
      int end_hour = 0, star_hour = 0;
      while(getline(ifs2, line)){
  			  istringstream iss(line);
  		    count=0;
  		    while(getline(iss, token, '|')){
  			      count ++;
              switch(count){
/*
              case 1:
                    end_time = token;
                    char buf[128];
                    strcpy(buf,token.c_str());
                    struct tm tm_;
                    strptime(buf, "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间

                    end_hour = int(tm_.tm_hour);
                    break;
*/
              case 2:
                  guid = token;
                  break;
              case 4:
                  bssid = token;
                  break;
              case 6:
                  connect_time = token.substr(0,10);
                  t  = atoi(connect_time.c_str());
                  struct tm* tm = localtime(&t);
                  //char date[50];
                  //cout<< tm->tm_hour<<endl;
                  //strftime(date, sizeof(date), "%Y-%m-%d %T", tm);
                  //connect_time = string(date);
                  //connect_time = connect_time.split(' ')
                  //connect_time = connect_time[1]
                  //connect_time = connect_time.split(':')
                  star_hour = int(tm->tm_hour);
                  break;
              }
  		    }
          if(filter.find(bssid)!=filter.end()){
            if(apTime.find(bssid) == apTime.end()){
                apTime[bssid] = arry;
            }
            apTime[bssid][star_hour] += 1;

          //    catalogint = atoi(catalog.c_str()); //string to int
           // cout<<catalogint<<endl;
//           for(int h=star_hour; h<=end_hour;h++)
//              apTime[bssid][h] += 1;
          }
	          //    catalogint = atoi(catalog.c_str()); //string to int
             // cout<<catalogint<<endl;
	          //  userCatg[guid][catalogint-1] += 1;

  	  }

      ifs2.close();
}

void outputFile(string s){
      ofstream ofs1(s.c_str());// szUtilization_0316

      string ap;
      for(map<string,vector<int> >::const_iterator it=apTime.begin(); it!=apTime.end(); ++it){
          ap = it->first;
          ofs1<< ap ;
          vector<int> ::const_iterator it2 = it->second.begin();
          for ( ; it2 != it->second.end(); it2++){
              ofs1<<','<<(*it2);
          }
          ofs1<<endl; //ofs1: 每个ap的连接次数

      }
      ofs1.close();

}

int main(int argc, char* argv[]){

  string s1 = argv[1];// categoryValidation/mac1617181920v2
  string s2 = argv[2];// /TecentData/wifi_conn/201503
  string s3 = argv[3];// utilization0316to0329

  getPoi(s1);

  string s, istr;
  for(int i=16;i<=29;i++){
    std::stringstream ss;
    ss << i;
    ss >> istr;
    s = s2+istr+"/safe_wifi_connect_sample_export";
    fact(s);
  }
  outputFile(s3);
  return 0;
}
