

#ifndef _GUARD_H_
#define _GUARD_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "jingxian/exception.h"


_jingxian_begin

template <class LOCK>
class guard
{
public:

    guard(LOCK &l, bool nothrow = true)
            : lock_(&l)
            , owner_(false)
    {
        if (!this->lock() && !nothrow)
            ThrowException1(LockException , _T("������ʧ��"));
    }

    guard(LOCK &l, bool is_lock, bool nothrow)
            : lock_(&l)
            , owner_(false)
    {
        if (is_lock && !this->lock() && !nothrow)
            ThrowException1(LockException , _T("������ʧ��"));
    }

    ~guard(void)
    {
        this->unlock();
    }

    bool lock(void)
    {
        return this->owner_ = this->lock_->acquire();
    }

#if(_WIN32_WINNT >= 0x0400)
    bool try_lock(void)
    {
        return this->owner_ = this->lock_->tryacquire();
    }
#endif // (_WIN32_WINNT >= 0x0400)

    void unlock(void)
    {
        if (!this->owner_)
            return ;

        this->owner_ = false;
        this->lock_->release();
    }

    bool locked(void) const
    {
        return this->owner_ ;
    }

// bool remove (void)
// {
    //return this->lock_->remove ();
// }

// void disown (void);
// {
    //this->owner_ = false;
// }

protected:

    guard(LOCK *lock): lock_(lock), owner_(false) {}

    LOCK *lock_;

    bool owner_;

private:

    DECLARE_NO_COPY_CLASS(guard);
};

_jingxian_end

#endif //_GUARD_H_