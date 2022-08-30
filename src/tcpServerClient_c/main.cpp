#include <random>
#include <map>
#include <iostream>
#include <cmath>
#include <boost/math/distributions.hpp>
#include <boost/program_options.hpp>
#include <stdlib.h>
#include <random>
#include <numeric>
#include <fstream>
#include <string>
#include <list>

extern "C"
{
  #include "tcpclient.h"
}
namespace bpo = boost::program_options;
using namespace std;

void readFile(const char* filename, list<string>& lines)
{
    lines.clear();
    ifstream file(filename);
    string s;
    while (getline(file, s))
        lines.push_back(s);
}

int main(int argc, char const *argv[]){

        std::cout << "... Starting Traffic Generation ..." << std::endl;

	std::ifstream trafficpattern( "processedin.txt" );
	if(trafficpattern.is_open()) {
		string trafficArray[5];

		for(int i = 0; i < 5; ++i) {
	    		trafficpattern >> trafficArray[i];
		}
	}

	std::cout << trafficArray[1] << std::endl;

	/*
	while(1){
		sendBytes(device_subtot[device]);
		device_subtot[device] = 0;
	}
	*/

}
