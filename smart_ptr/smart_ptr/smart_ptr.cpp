#include <iostream>

using namespace std;

template <typename T>
class smart_pointer
{
private:
    T* obj;
    int ref_count;
public:
    smart_pointer(T* ptr=nullptr)
    {
        obj = ptr;
		ref_count++;
    }

    ~smart_pointer()
    {
        delete obj;
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
			obj = nullptr;
		}
		return ref_count;
    }

    T& operator*()
    {
		return *obj;
	}

	smart_pointer& operator=(smart_pointer& sptr)
	{
		if (ref_count > 0)
        {
			release();
		}
		if (this != &sptr)
		{
			obj = sptr.obj;
			ref_count = sptr.ref_count;
			ref_count++;
		}
		return *this;
	}

    T* operator->() const
	{
		return obj;
	}

	bool isNull() const
	{
	    return obj == nullptr;
    }
};


//example
class Example
{
public:
	int field;
};

int multiply(smart_pointer<Example> example)
{
	return example->field * 3;
}

int main()
{
	smart_pointer<Example> without_field;
	smart_pointer<Example> with_field(new Example());
	without_field = with_field;
	without_field->field = 10;
	cout << "field value with -> " << with_field->field << "\n";
	cout << "field value with . " << (*with_field).field << "\n";
	with_field->field = multiply(with_field);
	cout << "without field value after = " << without_field->field << "\n";
	cout << "with field value " << with_field->field << "\n";
}
