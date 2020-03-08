#include <iostream>
#include <windows.h>
#include <winbase.h>
#include <map>
#include <iterator>
#include <ctime>

using namespace std;

struct Sector
{
    Sector* next;
};

template <class T>
class OwnAlloc
{
    public:
        typedef T value_type;
        OwnAlloc(){}
        ~OwnAlloc()
        {
            while (_pointer != NULL)
            {
                Sector* cur = _pointer;
                _pointer = _pointer -> next;
                delete cur;
            }
        }
        T* allocate(size_t size)
        {
            if (_pointer == NULL)
                _pointer = _allocate_block(size * sizeof(T));
            Sector* free_sector = _pointer;
            _pointer = _pointer->next;
            return reinterpret_cast<T*>(free_sector);
        }
        void deallocate(void* sector, size_t size)
        {
            reinterpret_cast<Sector*>(sector)->next = _pointer;
            _pointer = reinterpret_cast<Sector*>(sector);
        }
    private:
        Sector* _pointer = NULL;
        Sector* _allocate_block(size_t sector_size)
        {
            size_t block_size = 1000000;
            Sector* block_begin = reinterpret_cast<Sector*>(malloc(block_size));
            Sector* sector = block_begin;
            for (int i = 0; i < block_size / sector_size - 1; i++)
            {
                sector->next =
                    reinterpret_cast<Sector*>(reinterpret_cast<char *>(sector) + sector_size);
                sector = sector->next;
            }
            sector->next = NULL;
            return block_begin;
        }
};

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

struct CmpStr
{
   bool operator()(char const* first, char const* second) const
   {
      return strcmp(first, second) < 0;
   }
};

int main()
{
    char* buf = open_file();
    unsigned int start_time =  clock();
    map<char*, size_t, CmpStr, OwnAlloc<pair<char*, size_t>>> words;
    char* current_word = strtok(buf, " ,.-\n\t\r:;");
    while (current_word != NULL)
    {
        words[current_word]++;
        current_word = strtok(NULL, " ,.-\n\t\r:;");
    }
    unsigned int end_time = clock();
    cout <<  "time: " << end_time - start_time << endl;
    map<size_t, char*, less<int>, OwnAlloc<pair<char*, size_t>>> res;
    for (const auto &p : words )
        res[p.second] = p.first;
    for (const auto &p : res )
        cout << p.first << " : " << p.second << endl;
}
