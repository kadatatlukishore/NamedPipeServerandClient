// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <string>

#define BUF_SIZE 512

struct Data {
	std::string data;
	size_t length;
};

int main()
{

    while (TRUE) {
        HANDLE hPipe;
        hPipe = CreateNamedPipe(
            L"\\\\.\\pipe\\mynamedpipe",// pipe name 
            PIPE_ACCESS_OUTBOUND,       // read/write access 
            PIPE_TYPE_MESSAGE |         // message type pipe 
            PIPE_READMODE_MESSAGE |     // message-read mode 
            PIPE_NOWAIT,                // blocking mode 
            2,                          // max. instances  
            BUF_SIZE,                   // output buffer size 
            BUF_SIZE,                   // input buffer size 
            0,                          // client time-out 
            NULL);                      // default security attribute 

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            std::cout << "CreateNamedPipe failed, GLE=" << GetLastError() << std::endl;
            return -1;
        }

    

        // Wait for the client to connect; if it succeeds, 
        // the function returns a nonzero value. If the function
        // returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 
        BOOL fConnected = ConnectNamedPipe(hPipe, NULL) ?
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (fConnected) {

            std::cout << "Client is connected !!" << std::endl;

            Data sData;

            while (TRUE) {
                std::getline(std::cin, sData.data);
                sData.length = sizeof(sData.data);

                DWORD bytesWritten;

                // Write the reply to the pipe. 
                BOOL fSuccess = WriteFile(hPipe,               // handle to pipe 
                    &sData,              // buffer to write from 
                    sizeof(sData),       // number of bytes to write 
                    &bytesWritten,       // number of bytes written 
                    NULL);               // not overlapped I/O 

                if (!fSuccess || sData.data == "exit") {
                    std::cerr << "Failed to write to Named pipe" << std::endl;
                    break;
                }

                std::cout << "Data written to the named pipe successfully" << std::endl;
            }

            CloseHandle(hPipe);
        }
        else {
            CloseHandle(hPipe);
        }
    }

    return 0;
}