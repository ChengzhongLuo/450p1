#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <vector>

#include "crc.h"
#include "paritycheck.h"
#include "checksum.h"


using namespace std;


char* error_data(char* data, char* error)
{
    int datalen = strlen(data);
    int exorlen = (strlen(data)<strlen(error))? strlen(data): strlen(error);
    char* result = new char[datalen + 1];
    for(int i = 0; i < exorlen; i++)
    {
        result[i] = exor(data[i],error[i]);
    }
    if(strlen(data)>strlen(error)){
        for(int i=exorlen; i <datalen; i++)
        {
            result[i] = exor(data[i],'0');
        }
    }
    return result;
}

string error_data(string data, string error)
{
    int datalen = data.length();
    int errorlen = error.length();
    int exorlen = (datalen<errorlen)? datalen: errorlen;
    string result;
    for(int i = 0; i < exorlen; i++)
    {
        if(data[i]==error[i])
            result.append("0");
        else
            result.append("1");
    }
    if(datalen>errorlen){
        result.append(data.substr(errorlen,datalen-errorlen));
    }
    return result;
}


int main()
{
    //Open file
    ifstream file("data.txt");
    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    //Read data line by line
    string line;
    while (getline(file, line)) {
        size_t spacePos = line.find(' '); //find the position of space for separation
        if (spacePos == string::npos) {
            cerr << "Space not found in line: " << line << endl;
            continue;
        }

        string part1 = line.substr(0, spacePos); //first part is the data
        string part2 = line.substr(spacePos + 1); //second part is the error
        //copies of the data for parity and checksum
        string dataChecksum = part1;
        string dataParity = part1;
        string errorstring = part2;

        //copy data and error into char
        char error[256];
        char datacrc8[256], datacrc16[256];
        strncpy(datacrc8, part1.c_str(), sizeof(datacrc8) - 1);
        strncpy(datacrc16, part1.c_str(), sizeof(datacrc16) - 1);
        strncpy(error, part2.c_str(), sizeof(error) - 1);

        //use the given crc generating bits
        char crc_g8[20]="100110001";
        char crc_g16[20]="10001000000100001";

        char* crc8 = crc_encode(datacrc8,crc_g8);//return is 8-bit CRC, datacrc8 will have crc8 at the end
        char* errorDatacrc8 = error_data(datacrc8,error);//introduce error
        //cout << "DATA:" << errorDatacrc8 << endl;
        bool crc8result = crc_decode(errorDatacrc8,crc8,crc_g8);//check the received data using CRC8
        //cout << "Data:" << data << endl;

        //same for CRC16
        char* crc16 = crc_encode(datacrc16,crc_g16);
        char* errorDatacrc16 = error_data(datacrc16,error);
        //cout << "DATA:" << errorDatacrc16 << endl;
        bool crc16result = crc_decode(errorDatacrc16,crc16,crc_g16);

//2D-PARITY
        //return the data with parity bits inserted, the 8-bit column parity and the 9-bit row parity
        auto [newdataParity, originalcolParity, originalrowParity] = encodeData(dataParity);
        //cout << "Data: " << dataParity << endl;
        string errordataParity = error_data(newdataParity,errorstring);//introduce error
        //cout << "Error Data: " << dataParity << endl;

        //separate the codeword to 64-bit data, the 8-bit column parity and the 9-bit row parity
        auto [receivedtext, colParity, rowParity] = decodeData(errordataParity);
        bool parityresult = paritycheck(receivedtext,colParity,rowParity);//check the received data
        
//        cout << "Result:" << parityresult;
//        cout << "Original Data: " << receivedtext << endl;
//        cout << "Column Parity: " << colParity << endl;
//        cout << "Row Parity: " << rowParity << endl;

//Checksum
        string csum;
        csum = computeChecksum(dataChecksum);
        dataChecksum.append(csum);
        //cout<<"Checksum:"<<csum<<endl;
        string errordataChecksum = error_data(dataChecksum,errorstring);
        //cout<<errordataChecksum<<endl;
        bool checksumresult = checksum_decode(errordataChecksum);//check the received data
        //printchecksumresult(csum,checksumresult);
        //cout<<dataChecksum<<endl;


        //print the results
        cout << "==========================" << endl;
        cout << "Data:" << part1 << endl;
        cout << "Error:" << part2 << endl;
        printcrcresult(crc8, crc8result);
        printcrcresult(crc16, crc16result);
        printparityresult(parityresult,originalcolParity,originalrowParity);
        printchecksumresult(csum,checksumresult);

        delete[] crc8;
        delete[] crc16;
        delete[] errorDatacrc8;
    }

    file.close();
    return 0;
}
