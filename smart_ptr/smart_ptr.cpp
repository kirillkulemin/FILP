#include <iostream>

using namespace std;

template <typename T>
class smart_pointer
{
    T* obj;
    int ref_count;

    smart_pointer(){}

    smart_pointer(T* ptr)
    {
        obj = ptr;
		ref_count++;
    }

    T* get_obj()
    {
        return obj;
    }

    T* set_obj(T* ptr)
    {
        obj = ptr;
        ref_count++;
        return ptr;
    }

    int release()
    {
        ref_count--;
        if (ref_count == 0)
		{
			delete obj;
			obj = NULL;
		}
		return ref_count;
    }

    T operator*()
    {
		return *obj;
	}

	smart_pointer& operator=(smart_pointer& sptr)
	{
		if (ref_count > 0) {
			release();
		}
		if (this != &sptr) {
			obj = sptr.obj;
			ref_count = sptr.ref_count;
			ref_count++;
		}
		return *this;
	}

    smart_pointer operator->() const
	{
		return obj;
	}
};

int main(){}
