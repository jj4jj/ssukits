
#pragma once

//single thread
class RefCounter
{
public:
	RefCounter():iRefCount(0){}
	virtual ~RefCounter()
	{
		if(iRefCount )
	}
	void AddRef()
	{
		++iRefCount;
	}
	void Release()
	{
		--iRefCount;
		if(iRefCount <= 0)
		{
			delete this;
		}
	}
	int	 GetRefCount()
	{
		return iRefCount;
	}
private:
	int iRefCount;
};
