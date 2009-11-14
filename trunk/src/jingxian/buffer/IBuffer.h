
#ifndef _IBuffer_h_
#define _IBuffer_h_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include <vector>
# include "jingxian/buffer/buffer.h"

# include "jingxian/exception.h"
# include "jingxian/buffer/buffer.h"

_jingxian_begin

namespace ExceptionStyle
{
enum type
{
    THROW
    , NOTHROW
};
}

namespace buffer_type
{
enum
{
    npos = -1
};
}

class IBuffer
{
public:
    virtual ~IBuffer() {}

    /**
     * �������ж���д����ʱ,ָ�뵱ǰλ��Ҳ��ǰ�ƶ�,������
     * ������סָ��ĵ�ǰλ��,����ع���Ὣָ���ƶ���
     * ������ʼλ��.
     *
     * ����һ������
     */
    virtual int beginTranscation() = 0;
    /**
     * �ع�һ������
     */
    virtual void rollbackTranscation(int) = 0;

    /**
     * �ύһ������
     */
    virtual void commitTranscation(int) = 0;

    /**
     * �������ڷ�������ʱ�Ƿ��׳��쳣.
     */
    virtual void exceptions(ExceptionStyle::type exceptionStyle) = 0;

    /**
     * ȡ�����ڷ�������ʱ����ʽ.
     */
    virtual ExceptionStyle::type exceptions() const = 0;

    /**
     * ��ǰ���Ƿ��ڴ���״̬
     */
    virtual bool fail() const = 0;

    /**
     * ȡ�ô����
     */
    virtual errcode_t error() const = 0;

    /**
     * �����ǰ���Ĵ���״̬
     */
    virtual void clearError() = 0;
};


/**
 * �������ж���д����ʱ,ָ�뵱ǰλ��Ҳ��ǰ�ƶ�,������
 * ������סָ��ĵ�ǰλ��,����ع���Ὣָ���ƶ���
 * ������ʼλ��.
 */
template<typename T, typename I>
class TranscationScope
{
public:
    TranscationScope(T& buffer)
            : buffer_(buffer)
            , transcationId_(buffer.beginTranscation())
    {
    }

    ~TranscationScope()
    {
        if (-1 != transcationId_)
            return;
        buffer_.rollbackTranscation(transcationId_);
    }

    void commit()
    {
        if (-1 != transcationId_)
            return;
        buffer_.commitTranscation(transcationId_);
        transcationId_ = -1;
    }

private:
    NOCOPY(TranscationScope);
    T& buffer_;
    I transcationId_;
};

_jingxian_end

#endif //_IBuffer_h_