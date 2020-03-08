#include <iostream>
#include <windows.h>
#include <winbase.h>
#include <map>
#include <iterator>
#include <ctime>

using namespace std;

char* open_file()
{
    HANDLE file_handle;
    file_handle = CreateFile("61542.txt", GENERIC_READ,
        FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, 0);
    DWORD buf_size = GetFileSize(file_handle, NULL);
    cout << "file size: " << buf_size << endl;
    char* buf = new char[1 + buf_size];
    DWORD sr;
    ReadFile(file_handle, buf, buf_size, &sr, NULL);
    CloseHandle(file_handle);
    return buf;
}

int main()
{
    char* buf = open_file();
    unsigned int start_time =  clock();
    map<string, size_t> words;
    char* current_word = strtok(buf, " ,.-\n\t\r:;");
    while (current_word != NULL)
    {
        words[current_word]++;
        current_word = strtok (NULL, " ,.-\n\t\r:;");
    }
    unsigned int end_time = clock();
    cout <<  "time: " << end_time - start_time << endl;
    map<int, string> res;
    for (const auto &p : words )
        res[p.second] = p.first;
    for (const auto &p : res )
        cout << p.first << ": " << p.second << endl;
}
