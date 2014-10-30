
#include "algorithm/HashTable.h"
#include "iostream"
using std::cin;
using std::cout;
using std::endl;


static size_t mycode(void * v)
{
    return (*(size_t*)v)%20;
}
static int mycmp(void* v1,void* v2)
{
    return (*(size_t*)v1 - *(size_t*)v2);
}
struct Entry
{
    char buff[127];
    ////////////////////////////////////////
    bool operator < (const Entry & rhs) const
    {
        return ((int)(*(size_t*)buff - *(size_t*)rhs.buff) < 0);
    }
    bool operator == (const Entry & rhs) const
    {
        return ((int)(*(size_t*)buff - *(size_t*)rhs.buff) == 0);
    }
};
namespace std
{
    namespace tr1
    {
        template<>
        struct hash<Entry>
        {
            size_t operator()(const Entry & ent) const 
            {
                return *(size_t*)(ent.buff);
            }
        };
    };
};
    

int main(int argc,char* argv[])
{

    int iEntrySize = 127;
    int iMax = 1024;//100W
    int iFactor = 5;
    time_t tseed = time(NULL);
    srand(tseed);
    Entry ent;
    if(argc > 1)
    {
        if(argv[1][0] == 's')
        {
            std::set<Entry> stdset;
            for(int i = 0;i < iMax; ++i)
            {
                *(size_t*)ent.buff = rand();
                stdset.insert(ent);
            }
            cout<<"alloc count:"<<stdset.size()<<endl;
        }
        else
        if(argv[1][0] == 'u')
        {
            unordered_set<Entry> stdset;
            for(int i = 0;i < iMax; ++i)
            {
                *(size_t*)ent.buff = rand();
                stdset.insert(ent);
            }
            cout<<"alloc count:"<<stdset.size()<<endl;
        }
        
    }
    else
    {
        HashTable ht;
            
        size_t sz = ht.GetBufferSize(iEntrySize,iMax,iFactor);
        cout<<sz<<endl;
        char* buffer = new char[sz];
        cout<<"init:"<<ht.Init(buffer,sz,iEntrySize,iMax,mycode,mycmp,iFactor)<<endl;

        char entry[127];
        srand(tseed);    
        for(int i = 0;i < 5*iMax; ++i)
        {
            *(size_t*)entry = rand();
            if(ht.Insert(entry) == 0)
            {
                void * p = ht.Find(entry);
                assert(memcmp(p,entry,sizeof(entry)) == 0);
            }
        }
        cout<<"alloc count :"<<ht.Count()<<endl;

        srand(tseed);
        for(int i = 0;i < iMax; ++i)
        {
            *(size_t*)entry = rand();
            ht.Remove(entry);
        }
        cout<<"free count (should be 0):"<<ht.Count()<<endl;

        for(int i = 0;i < iMax/2 + 1; ++i)
        {
            *(size_t*)entry = i;
            ht.Insert(entry);
        }
        cout<<"agin alloc count:"<<ht.Count()<<endl;
        for(int i = 0;i < iMax/2; ++i)
        {
            *(size_t*)entry = i;
            ht.Remove(entry);
        }
        cout<<"agin free count(should be 1):"<<ht.Count()<<endl;

        HashTable ht2;
        cout<<"attach:"<<ht2.Attach(buffer,sz,iEntrySize,iMax,mycode,mycmp,iFactor)<<endl;



        delete buffer;
    }
    return 0;
}

