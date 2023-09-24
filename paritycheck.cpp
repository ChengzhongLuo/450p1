#include "paritycheck.h"

tuple<string, string, string> encodeData(const string& data) {
    if (data.size() != 64) {
        throw invalid_argument("Input data should be 64 bits.");
    }
    string encodedData = "";
    vector<string> blocks(8);
    for (int i = 0; i < 8; ++i) {
        blocks[i] = data.substr(i * 8, 8);
    }

    // Calculate the column parity
    string columnParity(8, '0');
    for (int j = 0; j < 8; ++j) {
        int columnBitCount = 0;
        for (int i = 0; i < 8; ++i) {
            if (blocks[i][j] == '1') {
                columnBitCount++;
            }
        }
        columnParity[j] = (columnBitCount % 2) ? '1' : '0';
    }

    // Calculate the row parity for each 8-bit block
    string rowParity="";
    for (int i = 0; i < 8; i++) {
        int bitCount = 0;
        for (int j = 0; j < 8; ++j) {
            if (blocks[i][j] == '1') bitCount++;
        }
        encodedData.append(blocks[i]);
        if(bitCount % 2) {
            rowParity.append("1");
            encodedData.append("1");
        }
        else {
            rowParity.append("0");
            encodedData.append("0");
        }

    }

    int columnParityBitCount = 0;
    for(int j=0; j<8; j++) {
        if (columnParity[j] == '1')
            columnParityBitCount++;
    }
    encodedData.append(columnParity);
    if(columnParityBitCount % 2) {
        rowParity.append("1");
        encodedData.append("1");
    }
    else {
        rowParity.append("0");
        encodedData.append("0");
    }

    //cout<<rowParity<<endl;

    return {encodedData,columnParity,rowParity};
}

tuple<string, string, string> decodeData(const string& encodedData) {
    if (encodedData.size() != 81) {
        throw invalid_argument("Encoded data should be 81 bits.");
    }

    // Extracting original data and row parities
    string originalData = "";
    string rowParity = "";
    for (int i = 0; i < 8; ++i) {
        originalData += encodedData.substr(i * 9, 8);
        rowParity += encodedData[i * 9 + 8];
    }
    rowParity += encodedData[80];

    // Extracting column parity
    string columnParity = encodedData.substr(72, 8);

    return {originalData, columnParity, rowParity};
}

pair<string, string> getParities(const string& data) {
    if (data.size() != 64) {
        throw invalid_argument("Input data should be 64 bits.");
    }

    // Split the data into eight 8-bit blocks
    vector<string> blocks(8);
    for (int i = 0; i < 8; ++i) {
        blocks[i] = data.substr(i * 8, 8);
    }

    // Calculate the column parity
    string columnParity(8, '0');
    for (int j = 0; j < 8; ++j) {
        int columnBitCount = 0;
        for (int i = 0; i < 8; ++i) {
            if (blocks[i][j] == '1') {
                columnBitCount++;
            }
        }
        columnParity[j] = (columnBitCount % 2) ? '1' : '0';
    }

    // Calculate the row parity for each 8-bit block
    string rowParity="";
    for (int i = 0; i < 8; i++) {
        int bitCount = 0;
        for (int j = 0; j < 8; ++j) {
            if (blocks[i][j] == '1') bitCount++;
        }
        if(bitCount % 2) {
            rowParity.append("1");
        }
        else {
            rowParity.append("0");
        }
    }

    int columnParityBitCount = 0;
    for(int j=0; j<8; j++) {
        if (columnParity[j] == '1')
            columnParityBitCount++;
    }
    if(columnParityBitCount % 2) {
        rowParity.append("1");
    }
    else {
        rowParity.append("0");
    }
    return {columnParity, rowParity};
}

bool parityCheck(string data, string col, string row)
{
    auto [colRec, rowRec] = getParities(data);
    //cout<<col<<"\t"<<colRec<<endl;
    //cout<<row<<"\t"<<rowRec<<endl;
    if(col==colRec && row==rowRec){
        return true;
    }
    else
        return false;
}

void printParityResult(bool result,string colParity,string rowParity){
    if(result){
        cout<<"2D Parity"<<endl;
        cout<<"Col: "<<colParity<<" Row: "<<rowParity<<"\t\tResult: Pass"<<endl;}
    else{
        cout<<"2D Parity"<<endl;
        cout<<"Col: "<<colParity<<" Row: "<<rowParity<<"\t\tResult: Not Pass"<<endl;}
}

//
// Created by Kevin Luo on 9/23/23.
//

