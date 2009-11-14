
#ifndef _IInBuffer_h_
#define _IInBuffer_h_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "jingxian/buffer/IBuffer.h"

_jingxian_begin

/**
 * InBuffer �� IBuffer һ����˵Ӧ������̳�,��
 * �Ҳ�����̫����,��ʵ�ָ���ʱ��ע�ⲻҪͬʱ��
 * �� OutBuffer;
 */
class IInBuffer : public IBuffer
{
public:
    virtual ~IInBuffer(void) {}

    virtual bool    readBoolean() = 0;
    virtual int8_t  readInt8() = 0;
    virtual int16_t readInt16() = 0;
    virtual int32_t readInt32() = 0;
    virtual int64_t readInt64() = 0;
    virtual void readBlob(void* blob, size_t len) = 0;

    /**
     * ��ǰ��offest���ֽ�
     * @params[ int ] λ���ƶ����ֽ���
     * @remarks  offest > 0 ʱ����ǰ�ƶ�,offest < 0 ʱ������ƶ�, ��offest�ƶ���λ�ó�����Χʱ���ƶ���ʼ�����.
     */
    virtual void seek(int offest) = 0;

    /**
     * �� Buffer �е����ݵĳ���
     */
    virtual size_t size() const = 0;

    /**
     * ����ָ�������ݵĵ�һ�γ���λ��
     */
    virtual size_t search(const void* context, size_t len) const = 0;

    /**
     * ����ָ�������ݵĵ�һ�γ���λ��
     */
    virtual size_t search(char ch) const = 0;

    /**
     * ����ָ�������ݵĵ�һ�γ���λ��
     */
    virtual size_t search(wchar_t ch) const = 0;

    /**
     * ����ָ�����ַ����������ַ���һ�γ���λ��
     */
    virtual size_t searchAny(const char* charset) const = 0;

    /**
     * ����ָ�����ַ����������ַ���һ�γ���λ��
     */
    virtual size_t searchAny(const wchar_t* charset) const = 0;

    /**
     * ȡ�� Buffer �е��������ݿ�
     */
    virtual const std::vector<io_mem_buf>& rawBuffer() const = 0;

    /**
     * ȡ�� Buffer �е��������ݿ���ܳ���
     */
    virtual size_t rawLength() const = 0;
};

_jingxian_end

#endif //_IInBuffer_h_