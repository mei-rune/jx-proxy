
#ifndef _IConcurrentPort_H_
#define _IConcurrentPort_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "jingxian/string/string.h"
# include "jingxian/IRunnable.h"

_jingxian_begin

/**
 * �߳̽ӿ�
 */
class IConcurrentPort
{
public:

    virtual ~IConcurrentPort() {}

    /**
     * ��ִ�з������͵��̵߳ȴ�����,�Ժ�ִ��
     *
     * @param[ in ] runnable ִ�з���
    */
    virtual bool send(IRunnable* runnable) = 0;

    /**
    * ȡ�õ�ַ������
    */
    virtual const tstring& toString() const = 0;
};

inline tostream& operator<<(tostream& target, const IConcurrentPort& concurrentPort)
{
    target << concurrentPort.toString();
    return target;
}

_jingxian_end

#endif // _IConcurrentPort_H_
