#pragma once
#include "AtomicCounter.h"

class Referable
{
public:
    Referable();

    inline long increaseReferenceCount () 
    { 
        return ++referenceCount; 
    }

    inline long decreaseReferenceCount () 
    { 
        return --referenceCount;
    }

    inline long getReferenceCount () const 
    { 
        return referenceCount; 
    }

    inline void setReferenceCount (long value) 
    { 
        referenceCount = value; 
    }

    virtual ~Referable();
private:
    AtomicCounter referenceCount;
};


template <class T> class Ref
{
public:
    Ref() : object(NULL) 
    {}

    Ref(const Ref<T>& other) : object(NULL) 
    { 
        set(other.object);
    }

    Ref(T* obj) : object(obj) 
    { 
        if(obj) obj->increaseReferenceCount();
    }

    virtual ~Ref() 
    { 
        set(NULL);
    }

    inline T* operator->() const
    { 
        return object;
    }

    inline Ref<T>& operator = (Ref<T> const& other) 
    { 
        set(other.object); 
        return *this; 
    }

    inline Ref<T>& operator = (T* obj) 
    { 
        set(obj); 
        return *this; 
    }

    inline void set (T* newobj)
    {
        // If equal do nothing
        if(newobj == object)
        {
            return;
        }

        // Increase reference count
        if(newobj)
        {
            newobj->increaseReferenceCount();
        }

        T* tmp = object;
        object = newobj;

        // Decrease the reference count of the old referable
        if(tmp)
        {
            if(tmp->decreaseReferenceCount() == 0)
            {
                delete tmp;
            }
        }
    }

    
    // Return object pointer. This method should be used with
    // care because it breaks the encapsulation.
    inline T* get() const
    {
        return object;
    }

    // to compare with NULL without creation of a temporary object
    typedef Ref<T> this_type;
    typedef T * this_type::*unspecified_bool_type;
    inline operator unspecified_bool_type() const
    {
        return object ? &this_type::object : NULL;
    }

private:
    T* object;
};

template<class T1, class T2> inline bool operator==(Ref<T1> const& a, Ref<T2> const& b)
{
    return (a.get() == b.get());
}

template<class T1, class T2> inline bool operator!=(Ref<T1> const& a, Ref<T2> const& b)
{
    return (a.get() != b.get());
}

