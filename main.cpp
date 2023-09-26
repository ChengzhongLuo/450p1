#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <vector>
#include <tuple>

#include "crc.h"
#include "paritycheck.h"
#include "checksum.h"


using namespace std;


char* error_data(char* data, char* error)
{
    int dataLen = strlen(data);
    int exorLen = (strlen(data)<strlen(error))? strlen(data): strlen(error);
    //cout<<dataLen<<endl<<strlen(error)<<exorLen<<endl;
    char* result = new char[dataLen + 1];
    for(int i = 0; i < exorLen; i++)
    {
        result[i] = exor(data[i],error[i]);
    }
    if(strlen(data)>strlen(error)){
        for(int i=exorLen; i <dataLen; i++)
        {
            result[i] = exor(data[i],'0');
        }
    }
    result[dataLen]='\0';
    return result;
}

string error_data(string data, string error)
{
    int dataLen = data.length();
    int errorlen = error.length();
    int exorLen = (dataLen<errorlen)? dataLen: errorlen;
    string result;
    for(int i = 0; i < exorLen; i++)
    {
        if(data[i]==error[i])
            result.append("0");
        else
            result.append("1");
    }
    if(dataLen>errorlen){
        result.append(data.substr(errorlen,dataLen-errorlen));
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
        datacrc8[sizeof(datacrc8) - 1] = '\0';
        strncpy(datacrc16, part1.c_str(), sizeof(datacrc16) - 1);
        datacrc16[sizeof(datacrc16) - 1] = '\0';
        strncpy(error, part2.c_str(), sizeof(error) - 1);
        error[sizeof(error) - 1] = '\0';

        //use the given crc generating bits
        char crc_g8[20]="100110001";
        char crc_g16[20]="10001000000100001";

        char* crc8 = crc_encode(datacrc8,crc_g8);//return is 8-bit CRC, datacrc8 will have crc8 at the end
        //cout<<"CRC: "<<crc8<<endl;
        char* errorDatacrc8 = error_data(datacrc8,error);//introduce error
        //cout << "DATA:" << errorDatacrc8 << endl;
        bool crc8result = crc_decode(errorDatacrc8,crc8,crc_g8);//check the received data using CRC8
        //cout << "Data:" << datacrc8 << endl;

        //same for CRC16
        char* crc16 = crc_encode(datacrc16,crc_g16);
        char* errorDatacrc16 = error_data(datacrc16,error);
        //cout << "DATA:" << errorDatacrc16 << endl;
        bool crc16result = crc_decode(errorDatacrc16,crc16,crc_g16);

//2D-PARITY
        //return the data with parity bits inserted, the 8-bit column parity and the 9-bit row parity
        auto [newdataParity, originalcolParity, originalrowParity] = encodeData(dataParity);
        //cout << "Data: " << dataParity << endl;
        string errorDataParity = error_data(newdataParity,errorstring);//introduce error
        //cout << "Error Data: " << dataParity << endl;

        //separate the codeword to 64-bit data, the 8-bit column parity and the 9-bit row parity
        auto [receivedText, colParity, rowParity] = decodeData(errorDataParity);
        bool parityResult = parityCheck(receivedText,colParity,rowParity);//check the received data

//        cout << "Result:" << parityresult;
//        cout << "Original Data: " << receivedText << endl;
//        cout << "Column Parity: " << colParity << endl;
//        cout << "Row Parity: " << rowParity << endl;

//Checksum
        string csum;
        csum = computeChecksum(dataChecksum);
        dataChecksum.append(csum);
        //cout<<"Checksum:"<<csum<<endl;
        string errordataChecksum = error_data(dataChecksum,errorstring);
        //cout<<errordataChecksum<<endl;
        bool checksumresult = checksumDecode(errordataChecksum);//check the received data
        //printChecksumResult(csum,checksumresult);
        //cout<<dataChecksum<<endl;


        //print the results
        cout << "==========================" << endl;
        cout << "Data:" << part1 << endl;
        cout << "Error:" << error << endl;
        printCrcResult(crc8, crc8result);
        printCrcResult(crc16, crc16result);
        printParityResult(parityResult,originalcolParity,originalrowParity);
        printChecksumResult(csum, checksumresult);

        delete[] crc8;
        delete[] crc16;
        delete[] errorDatacrc8;
    }

    file.close();
    return 0;
}