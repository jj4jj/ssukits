
#include "algorithm/MemPool.h"

#include "iostream"
using std::cin;
using std::cout;
using std::endl;



int main()
{

    MemPool mmp;
    int iEntrySize = 127;
    int iMax = 1024*500;//50W
    size_t sz = mmp.GetBufferSize(iEntrySize,iMax);
    cout<<sz<<endl;
    char* buffer = new char[sz];
    cout<<"init:"<<mmp.Init(buffer,sz,iEntrySize,iMax)<<endl;

    for(int i = 0;i < iMax/2 + 1; ++i)
    {
        mmp.Alloc();
    }
    cout<<"alloc count:"<<mmp.Count()<<endl;
    for(int i = 0;i < iMax/2; ++i)
    {
        mmp.Free(i);
    }
    cout<<"free count:"<<mmp.Count()<<endl;
    for(int i = 0;i < iMax/2 + 1; ++i)
    {
        mmp.Alloc();
    }
    cout<<"alloc count:"<<mmp.Count()<<endl;
    mmp.Alloc();
    mmp.Alloc();
    cout<<"alloc count:"<<mmp.Count()<<endl;

    MemPool mp2;
    cout<<"attach:"<<mp2.Attach(buffer,sz,iEntrySize,iMax)<<endl;



    delete buffer;
    return 0;
}


