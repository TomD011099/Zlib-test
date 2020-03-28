#include "zlib.h"
#include <iostream>
#include <vector>

int level;
std::vector<uint8_t> bus;
std::vector<uint8_t> send;

void compress_test(const std::vector<uint8_t> &payload) {
    std::vector<uint8_t> buffer;

    const size_t BUFSIZE = 128 * 1024;
    uint8_t temp_buffer[BUFSIZE];

    z_stream strm;
    strm.zalloc = nullptr;
    strm.zfree = nullptr;
    strm.next_in = const_cast<Bytef*>(payload.data());
    strm.avail_in = payload.size();
    strm.next_out = temp_buffer;
    strm.avail_out = BUFSIZE;

    deflateInit(&strm, level);

    while (strm.avail_in != 0) {
        int res = deflate(&strm, Z_NO_FLUSH);
        //assert(res == Z_OK); assert not found???
        if (strm.avail_out == 0) {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
    }

    int deflate_res = Z_OK;
    while (deflate_res == Z_OK) {
        if (strm.avail_out == 0) {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
        deflate_res = deflate(&strm, Z_FINISH);
    }

    //assert(deflate_res == Z_STREAM_END);
    buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
    deflateEnd(&strm);

    send.swap(buffer);
}

void decompress_test(const std::vector<uint8_t> &payload) {
    std::vector<uint8_t> buffer;
    std::vector<uint8_t> out;

    const size_t BUFSIZE = 128 * 1024;
    uint8_t temp_buffer[BUFSIZE];

    z_stream strm;
    strm.zalloc = nullptr;
    strm.zfree = nullptr;
    strm.next_in = const_cast<Bytef*>(payload.data());
    strm.avail_in = payload.size();
    strm.next_out = temp_buffer;
    strm.avail_out = BUFSIZE;

    inflateInit(&strm);

    while (strm.avail_in != 0) {
        int res = inflate(&strm, Z_NO_FLUSH);
        //assert(res == Z_OK); assert not found???
        if (strm.avail_out == 0) {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
    }

    int inflate_res = Z_OK;
    while (inflate_res == Z_OK) {
        if (strm.avail_out == 0) {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
        inflate_res = deflate(&strm, Z_FINISH);
    }

    //assert(deflate_res == Z_STREAM_END);
    buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
    inflateEnd(&strm);

    out.swap(buffer);
    //this->receive(out);

    std::string str(out.begin(), out.end());
    std::cout << "Rec\n" << str << "\n" << str.size() << "\n";
}

void flush() {
    std::string str(send.begin(), send.end());
    std::cout << "Compr\n" << str << "\n" << str.size() << "\n\n";
    bus = send;
}

void sendData(const std::vector<uint8_t> &payload) {
    compress_test(payload);

    flush();
}

void receiveData(const std::vector<uint8_t> &payload) {
    decompress_test(payload);
}

int main(int argc, char* argv[]) {
    level = 1;

    std::string str = "Hello world!";
    //std::string str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec quis tortor libero. Etiam fringilla tristique dui, eget aliquam libero. Aliquam at consectetur eros, eget sollicitudin eros. Phasellus sodales tellus vitae iaculis facilisis. Suspendisse potenti. Etiam bibendum est eu dignissim pretium. Nam tincidunt purus vitae nulla convallis, quis dignissim felis tempor. Duis aliquet diam quam, vitae elementum augue maximus quis. Suspendisse eget faucibus tellus, non bibendum tortor. Sed interdum vulputate tellus, sed convallis sem rutrum et. Fusce commodo hendrerit tempor. Fusce aliquam ornare vulputate. Aliquam suscipit ut tortor vitae eleifend. Nulla sit amet ante condimentum libero vulputate egestas nec eget risus. Nunc gravida orci mi, ut gravida velit condimentum nec.\n"
    //                  "\n"
    //                  "Praesent dictum feugiat nulla. In eu enim neque. Aenean tempor neque vitae convallis bibendum. Fusce pretium lorem quis diam tempus ullamcorper. Etiam condimentum pellentesque augue. Nunc mauris velit, ultricies metus.";
    std::vector<uint8_t> payload(str.begin(), str.end());

    std::cout << "Original\n" << str << "\n" << str.size() << "\n\n";

    sendData(payload);

    receiveData(bus);

    return 0;
}



