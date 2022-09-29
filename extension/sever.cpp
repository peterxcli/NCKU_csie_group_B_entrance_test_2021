#include <fstream> // for writing files
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <zmq.h>

typedef unsigned char uchar;

using namespace std;

static string base64_decode(const string &in) {
    // Ref: https://stackoverflow.com/a/34571089/9625777
    string out;
    vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
        T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val = 0, valb = -8;
    for (uchar c : in) {
        if (T[c] == -1)
            break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

int main() {
    while (1) {
        // Create socket
        void *context = zmq_ctx_new() void *requester = zmq_socket(context, ZMQ_PULL) if (zmq_connect(requester, "tcp://localhost:5557") != 0) {
            // The zmq_connect() function returns zero if successful.
            // Otherwise it returns -1
            cout << "No connection found\n";
            continue;
        }
        cout << "Got a connectopn\n";

        // Receive data
        char buffer[4194328];
        zmq_recv(requester, buffer, 4194328, 0)
            //cout << buffer;

            // decode data
            auto decodedData = base64_decode(buffer);

        // write results to a file
        ofstream myFile("res.ppm");
        myFile << decodedData;
        myFile.close();

        // Close socket
        zmq_close(requester);
        zmq_ctx_destroy(context);
    }
    return 0;
}
