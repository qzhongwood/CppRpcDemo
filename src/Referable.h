#pragma once
#include "AtomicCounter.h"

class Referable
{
public:
    Referable();

    /**
    * Increase smart reference count.
    * Should be only called by the <CODE>Ref</CODE> objects.
    *
    * @return Smart reference count after increasing.
    */
    inline long increaseReferenceCount () { return ++_referenceCount; }

    /**
    * Decrease smart reference count.
    * If reference count is zero this object should be deleted.
    * Should be only called by the <CODE>Ref</CODE> objects.
    *
    * @return Smart reference count after decreasing.
    */
    inline long decreaseReferenceCount () { return --_referenceCount; }

    /**
    * Returns current smart reference count.
    * <CODE>Ref</CODE> class does not need this method.
    * Normally this method is not needed.
    */
    inline long getReferenceCount () const { return _referenceCount; }

    /**
    * Sets the current smart reference count.
    * <CODE>Ref</CODE> class does not need this method.
    * Normally this method is not needed.
    */
    inline void setReferenceCount (long value) { _referenceCount = value; }

    virtual ~Referable();
private:
    AtomicCounter _referenceCount;
};


template <class T> class Ref
{
public:
    /**
    * Default constructor.
    */
    Ref() : object(NULL) {}

    /**
    * Copy constrcutor.
    */
    Ref(const Ref<T>& other) : object(NULL) { set(other.object); }

    /**
    * Value constructor.
    */
    Ref(T* obj) : object(obj) { if(obj) obj->increaseReferenceCount(); }

    /**
    * Virtual destructor which releases the object reference.
    */
    virtual ~Ref() { set(NULL); }

    /**
    * Allows other classes to point directly the
    * reference class' method. Because of templates, no
    * casting is needed.
    */
    inline T* operator->() const { return object; }

    /**
    * Sets the reference of this object to the
    * reference of the other object.
    */
    inline Ref<T>& operator=(Ref<T> const& other) { set(other.object); return *this; }

    /**
    * Sets a new object for this reference.
    */
    inline Ref<T>& operator=(T* obj) { set(obj); return *this; }

    /**
    * Sets a new object for this reference.
    */
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

        T*      tmp = object;
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

    /**
    * Return object pointer. This method should be used with
    * care because it breaks the encapsulation.
    * Typically this method is needed for the method calls
    * which require literal object pointer.
    *
    * <P>It may not be bad idea to pass the <CODE>Ref</CODE>
    * objects as method arguments.</P>
    *
    * @return Object pointer or <CODE>NULL</CODE>.
    */
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

