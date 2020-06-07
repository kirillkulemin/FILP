#include <iostream>
#define bsr __builtin_clz
#define bsf __builtin_ctz

using namespace std;

int setOneOnPosition(int num, int position)
{
    num |= (1 << position);
    return num;
}

int setZeroOnPosition(int num, int position)
{
    num &= ~(1 << position);
    return num;
}

class Block
{
public:
    char Level;
    bool Free;
    void* Data;
    Block* Next;
    Block* Prev;
    Block* Neib;
};

class OwnAllocator
{
private:
    Block* block = new Block();
    int mask = 0;
    int* levelCount;
    int maxDeg;
public:
    OwnAllocator(size_t newSize)
    {
        maxDeg = bsr(newSize) + 1;
        levelCount = new int[maxDeg + 1];
        for (int i = 0; i <= maxDeg; i++)
            levelCount[i] = 0;
        levelCount[maxDeg] = 1;
        mask = setOneOnPosition(mask, maxDeg);
        block -> Next = block;
        block -> Prev = block;
        block -> Data = malloc(1 << maxDeg);
        block -> Level = maxDeg;
        int level = block -> Level;
        block -> Free = true;
        block -> Neib = nullptr;
    }
    void* allocate(size_t initSize)
    {
        int blockSize = 1 << (bsf(initSize) + 1);
        int searchMask = ~(blockSize - 1);
        int level = bsf(mask & searchMask);
        while(true)
        {
            if (block -> Level == level && block -> Free)
                break;
            block = block -> Next;
        }
        while(blockSize <= (1 << level - 1))
        {
            Block* nextBlock = new Block();
            nextBlock -> Data = block -> Data + (1 << (level - 1));
            nextBlock -> Level = block -> Level = level - 1;
            nextBlock -> Prev = block;
            nextBlock -> Next = block -> Next;
            nextBlock -> Neib = block;
            nextBlock -> Free = true;
            block -> Next = nextBlock;
            block -> Neib -> Neib = nullptr;
            block -> Neib = nextBlock;
            levelCount[level]--;
            levelCount[level - 1] += 2;
            if (levelCount[level] == 0)
                mask = setZeroOnPosition(mask, level);
            level--;
        }
        block -> Free = false;
        return block -> Data;
    }
    void printLevels()
    {
        for (int i = 0; i <= maxDeg; i++)
        {
            if (levelCount[i] > 0)
                cout << i << " " << levelCount[i] <<endl;
        }
    }
    void deallocate(void* data)
    {
        while(true)
        {
            if (block -> Data == data)
                break;
            block = block -> Next;
        }
        delete data;
        while (!(block -> Neib -> Free))
        {
            Block* prevBlock;
            Block* nextBlock;
            if (block -> Prev == block -> Neib)
            {
                nextBlock = block;
                prevBlock = block -> Neib;
            }
            else
            {
                nextBlock = block -> Neib;
                prevBlock = block;
            }
            prevBlock -> Next = nextBlock -> Next;
            prevBlock -> Level++;
            if (prevBlock -> Next -> Level == prevBlock -> Level)
                prevBlock -> Neib = prevBlock -> Next;
            if (prevBlock -> Prev -> Level == prevBlock -> Level)
                prevBlock -> Neib = prevBlock -> Prev;
            int level = nextBlock -> Level;
            levelCount[level] -= 2;
            if (levelCount[level] == 0)
                mask = setZeroOnPosition(mask, level);
            delete nextBlock;
            block = prevBlock;
        }
    }
};

//example

int main()
{
    auto a = new OwnAllocator(10);
    a->printLevels();
    auto d = a->allocate(8);
    a->printLevels();
    auto b = a->allocate(4);
    a->printLevels();
    auto c = a->allocate(2);
    a->printLevels();
    auto s = a->allocate(1);
    a->printLevels();
    auto v = a->allocate(1);
    a->printLevels();
    a->deallocate(b);
    a->deallocate(c);
    a->deallocate(d);
    a->deallocate(s);
    a->deallocate(v);
    a->printLevels();
}
