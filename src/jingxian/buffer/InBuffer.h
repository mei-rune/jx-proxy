
#ifndef _InBuffer_H_
#define _InBuffer_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "jingxian/Buffer/BaseBuffer.H"
# include "jingxian/Buffer/IInBuffer.H"

_jingxian_begin


class InBuffer : public BaseBuffer<IInBuffer>
{
public:

    class TranscationData
    {
    public:

        // �Ѷ����ֽ���
        size_t readLength_;
        // ��ǰ���ڶ������ݿ�
        size_t current_;
        // ��ǰ���ڶ�������ָ��
        const char* currentPtr_;
        // ��ǰ���ڶ������ݿ��ʣ���ֽ���
        size_t currentLength_;
        // ���Դ���ʱ�Ĵ���ʽ
        ExceptionStyle::type exceptionStyle_;
        // ��ǰ�Ƿ��ڴ�����
        errcode_t errno_;
    };

    InBuffer(const std::vector<io_mem_buf>* buf = null_ptr, size_t totalLength = 0);
    virtual ~InBuffer(void);

    virtual void reset(const std::vector<io_mem_buf>* buf, size_t totalLength);

    virtual int beginTranscation();
    virtual void rollbackTranscation(int);
    virtual void commitTranscation(int);

    virtual bool    readBoolean();
    virtual int8_t  readInt8();
    virtual int16_t readInt16();
    virtual int32_t readInt32();
    virtual int64_t readInt64();
    virtual void readBlob(void* blob, size_t len);

    virtual void seek(int offest);

    virtual size_t size()const;

    virtual size_t search(char ch) const;
    virtual size_t search(wchar_t ch) const;
    virtual size_t search(const void* context, size_t len) const;
    virtual size_t searchAny(const char* charset) const;
    virtual size_t searchAny(const wchar_t* charset) const;

    virtual size_t readLength() const;
    virtual const std::vector<io_mem_buf>& rawBuffer() const;
    virtual size_t rawLength() const;

private:
    NOCOPY(InBuffer);

    // ���е������ڴ��
    const std::vector<io_mem_buf>* memory_;
    // ���ݵ����ֽ���,��ֵ���ᱻ����
    size_t totalLength_;

    // �Ѷ����ֽ���
    size_t readLength_;
    // ��ǰ���ڶ������ݿ�
    size_t current_;
    // ��ǰ���ڶ�������ָ��
    const char* currentPtr_;
    // ��ǰ���ڶ������ݿ��ʣ���ֽ���
    size_t currentLength_;

    std::vector<TranscationData> transcationDatas_;
};

_jingxian_end

#endif //_InBuffer_H_