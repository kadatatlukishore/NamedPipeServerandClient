#include <iostream>
#include <windows.h>
#include <string>

struct MyStruct {
    std::string data;
    size_t length;
};

int main() {
    // Connect to the named pipe
    HANDLE hPipe;
    while (true) {
        hPipe = CreateFile(
            TEXT("\\\\.\\pipe\\mynamedpipe"),
            GENERIC_READ,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        // Wait for the server to create the pipe
        if (GetLastError() != ERROR_PIPE_BUSY) {
            std::cerr << "Could not open pipe." << GetLastError() << std::endl;
            return 1;
        }

        // Wait for the pipe to be available
        if (!WaitNamedPipe(TEXT("\\\\.\\pipe\\myamedpipe"), 20000)) {
            std::cerr << "Could not open pipe: 20 second wait timed out." << std::endl;
            return 1;
        }
    }

    std::cout << "Connected to the server. Waiting for messages..." << std::endl;

    // Continuously read messages from the named pipe
    MyStruct data;
    while (true) {
        DWORD bytesRead;
        BOOL isSuccess = ReadFile(
            hPipe,
            &data,
            sizeof(data),
            &bytesRead,
            NULL
        );

        if (!isSuccess || bytesRead == 0) {
            std::cerr << "Failed to read from named pipe." << std::endl;
            break;
        }

        std::cout << "Received: " << data.data << std::endl;
        std::cout << "Press Enter to continue..." << std::endl;
        std::cin.get(); // Wait for Enter key press
    }

    // Close the pipe
    CloseHandle(hPipe);
    return 0;
}

