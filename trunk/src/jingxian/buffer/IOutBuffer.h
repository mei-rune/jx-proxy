
#ifndef _IOutBuffer_H_
#define _IOutBuffer_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "jingxian/buffer/IBuffer.h"

_jingxian_begin

/**
 * OutBuffer �� IBuffer һ����˵Ӧ������̳�,��
 * �Ҳ�����̫����,��ʵ�ָ���ʱ��ע�ⲻҪͬʱ��
 * �� InBuffer;
 */
class IOutBuffer : public IBuffer
{
public:
    virtual ~IOutBuffer(void) {}

    virtual IOutBuffer& writeBoolean(bool value) = 0;
    virtual IOutBuffer& writeInt8(int8_t value) = 0;
    virtual IOutBuffer& writeInt16(int16_t value) = 0;
    virtual IOutBuffer& writeInt32(int32_t value) = 0;
    virtual IOutBuffer& writeInt64(const int64_t& value) = 0;
    virtual IOutBuffer& writeBlob(const void* blob, size_t len) = 0;
};

_jingxian_end

#endif //_IOutBuffer_H_