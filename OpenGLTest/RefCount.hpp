#ifndef __REF_COUNT_H
#define __REF_COUNT_H

#include <Windows.h>

class AllocD
{
public:
	static void* New(int size) {return ::operator new(size);}
	static void Delete(void *ptr) {::operator delete(ptr);}
	template <class Type, class... Args>
	static Type* Construct(Args... args)
	{
		Type* ptr = New(sizeof(Type));
		if (ptr) ::operator(ptr) new(args...);
		return ptr;
	}
	template <class Type>
	static void Destroy(Type* ptr)
	{
		if (ptr)
		{
			ptr->~Type();
			Delete(ptr);
		}
	}
};

class NoCopy
{
public:
	NoCopy() {};
	~NoCopy() {};
	// No copy allowed
	NoCopy(const NoCopy&) = delete;
	NoCopy(NoCopy&&) = delete;
	NoCopy& operator=(const NoCopy&) = delete;
};

template <class Traits, class Alloc=AllocD>
class RefCountT : public NoCopy
{
	typedef typename Traits::Value_Type Value_Type;
public:
    RefCountT() : _count(Traits::Init_Value) {};
    virtual ~RefCountT()
    {
        Release();
    }

	virtual RefCountT *Close() {return nullptr;}

    void AddRef()
    {
        Traits::AddRef(_count);
    }

    void Release()
    {
        if (!Traits::Release(count))
        {
            Alloc::Destroy(this);
        }
    }

private:
    Value_Type _count;
};

struct CountTraitsD
{
public:
	typedef int Value_Type;
    enum : Value_Type { Init_Value = 1 };

	Value_Type AddRef(Value_Type& value) {return ++value;}
	Value_Type Release(Value_Type& value) {return --value;}
};

struct CountTraitsM
{
public:
	typedef volatile long Value_Type;
    enum : Value_Type { Init_Value = 1 };

	Value_Type AddRef(Value_Type& value) {return InterlockedIncrement(&value);}
	Value_Type Release(Value_Type& value) {return InterlockedDecrement(&value);}
};

typedef RefCountT<CountTraitsD, AllocD> RefCountD;
typedef RefCountT<CountTraitsM, AllocD> RefCount;

template <class Type>
class RefBase
{
protected:
    RefBase() throw()
        : _ptr(nullptr)
    {
    }

    RefBase(Type* ptr) throw()
    {
        _ptr = ptr;
        if (_ptr != nullptr)
        {
            _ptr->AddRef();
        }
    }

    void Swap(RefBase& ref)
    {
        Type* ptr = ref._ptr;
        ref._ptr = _ptr;
        _ptr = ptr;
    }

public:
    ~RefBase() throw()
    {
        Release();
    }
    
    void Release() throw()
    {
        Type* temp = _ptr;
        if (temp)
        {
            _ptr = nullptr;
            temp->Release();
        }
    }

    Type** operator&() throw()
    {
        return &_ptr;
    }

    Type* operator->() const throw()
    {
        return _ptr;
    }

    Type& operator*() const
    {
        return *_ptr;
    }

    operator Type*() const throw()
    {
        return _ptr;
    }

    bool operator!() const throw()
    {
        return _ptr == nullptr;
    }

    bool operator!=(Type* ptr) const throw()
    {
        return !operator==(ptr);
    }

    bool operator==(Type* ptr) const throw()
    {
        return _ptr == ptr;
    }

private:
    Type* _ptr;
};

template <class Type>
class Ref : public RefBase<Type>
{
public:
    Ref()
    {
    }

    Ref(Type* ptr)
        : RefBase(ptr)
    {
    }

    Ref(const Ref& ref) throw()
        : RefBase(ref._ptr)
    {
    }

    Ref(Ref&& ref) throw()
        : RefBase()
    {
        Swap(ref);
    }

    Type* operator= (const Ref& ref) throw()
    {
        if (*this != ref)
        {
            Ref(ref).Swap(*this);
        }
        return *this;
    }

    Type* operator= (Type* ptr)
    {
        if (*this != ptr)
        {
            Ref(ptr).Swap(*this);
        }
        return *this;
    }
};

#endif // __REF_COUNT_H