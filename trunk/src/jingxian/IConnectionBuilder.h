
#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "jingxian/string/string.h"
# include "jingxian/exception.h"
# include "jingxian/ITransport.h"

_jingxian_begin

class IConnectionBuilder
{
public:

    virtual ~IConnectionBuilder() {};
    /**
     * ����һ������
     *
     * @param[ in ] endPoint ����Ŀ���ַ
     * @param[ in ] onComplete �����ӳɹ�ʱ�����ñ�ί�д���һ��������
     * @param[ in ] onError  �����ӷ���������ʼ����������������ʱ��
    * �ñ�ί�У������쳣������������ConnectError��InitializeError
     * @param[ in ] context �������ӵ�������
     * @exception ConnectError �����ӷ�������ʱ�����ô�����ί��ʱ������</exception>
     * @exception InitializeError ��ʼ����������������ʱ�����ô�����ί��ʱ������</exception>
    */
    virtual void connect(const tchar* endPoint
                         , OnBuildConnectionComplete onComplete
                         , OnBuildConnectionError onError
                         , void* context) = 0;

    /**
     * ȡ�õ�ַ������
     */
    virtual const tstring& toString() const = 0;
};

inline tostream& operator<<(tostream& target, const IConnectionBuilder& connectionBuilder)
{
    target << connectionBuilder.toString();
    return target;
}


_jingxian_end

#endif // CONNECTOR_H
