#include "zlib.h"
#include <iostream>
#include <vector>
#include <cstring>

unsigned char * pCompressedData;
int level;
std::vector<uint8_t> bus;

void flush() {
    std::vector<uint8_t> compressedPayload(pCompressedData, pCompressedData + 512);

    std::string str(compressedPayload.begin(), compressedPayload.end());
    std::cout << "Compr\n" << str << "\n" << str.size() << "\n\n";

    bus = compressedPayload;
}

void sendData(const std::vector<uint8_t> &payload) {
    //Can be anything --> should be dig enough to prevent overflow
    unsigned char data[1100];
    std::copy(payload.begin(), payload.end(), data);

    unsigned long dataSize = payload.size();

    unsigned long compressedDataSize = dataSize;
    pCompressedData = new unsigned char[compressedDataSize];

    std::cout << compressedDataSize << " " << strlen(reinterpret_cast<const char*>(pCompressedData)) << "\n";

    compress2(pCompressedData, &compressedDataSize, data, dataSize, level);

    std::cout << compressedDataSize << " " << strlen(reinterpret_cast<const char*>(pCompressedData)) << "\n";

    flush();
}

void receiveData(const std::vector<uint8_t> &payload) {
    unsigned long nCompressedDataSize = payload.size();
    //pCompressedData = nullptr;
    unsigned char temp[1100];

    std::copy(payload.begin(), payload.end(), temp);

    unsigned long nDataSize = 10 * nCompressedDataSize;
    auto * pUncompressedData =    new unsigned char[1100];

    int out = uncompress(pUncompressedData, &nDataSize, temp, nCompressedDataSize);

    // data --> input corruputed or incomplete (here incomplete)
    if (out == Z_OK)
        std::cout << "Rec\n" << pUncompressedData << "\n" << nDataSize << "\n";
    else if (out == Z_BUF_ERROR)
        std::cout << "Buf";
    else if (out == Z_DATA_ERROR)
        std::cout << "data";
    else if (Z_MEM_ERROR)
        std::cout << "mem";
}

int main(int argc, char* argv[]) {
    level = 1;
    pCompressedData = nullptr;

    std::string str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec quis tortor libero. Etiam fringilla tristique dui, eget aliquam libero. Aliquam at consectetur eros, eget sollicitudin eros. Phasellus sodales tellus vitae iaculis facilisis. Suspendisse potenti. Etiam bibendum est eu dignissim pretium. Nam tincidunt purus vitae nulla convallis, quis dignissim felis tempor. Duis aliquet diam quam, vitae elementum augue maximus quis. Suspendisse eget faucibus tellus, non bibendum tortor. Sed interdum vulputate tellus, sed convallis sem rutrum et. Fusce commodo hendrerit tempor. Fusce aliquam ornare vulputate. Aliquam suscipit ut tortor vitae eleifend. Nulla sit amet ante condimentum libero vulputate egestas nec eget risus. Nunc gravida orci mi, ut gravida velit condimentum nec.\n"
                      "\n"
                      "Praesent dictum feugiat nulla. In eu enim neque. Aenean tempor neque vitae convallis bibendum. Fusce pretium lorem quis diam tempus ullamcorper. Etiam condimentum pellentesque augue. Nunc mauris velit, ultricies metus.";
    std::vector<uint8_t> payload(str.begin(), str.end());

    std::cout << "Original\n" << str << "\n" << str.size() << "\n\n";

    sendData(payload);

    receiveData(bus);

    return 0;
}



