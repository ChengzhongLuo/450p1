//
// Created by Kevin Luo on 9/23/23.
//

#ifndef INC_450P1_PARITYCHECK_H
#define INC_450P1_PARITYCHECK_H

#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <fstream>
#include <vector>

using namespace std;

tuple<string, string, string> encodeData(const string& data) ;
tuple<string, string, string> decodeData(const string& encodedData);
pair<string, string> getParities(const string& data);
bool paritycheck(string data, string col, string row);
void printparityresult(bool result,string colParity,string rowParity);


#endif //INC_450P1_PARITYCHECK_H
