# include "pro_config.h"
# include "jingxian/Buffer/InBuffer.h"
# ifdef _GOOGLETEST_
#include <gtest/gtest.h>
#endif

_jingxian_begin

InBuffer::InBuffer(const std::vector<io_mem_buf>* buf, size_t totalLength)
        : memory_(null_ptr)
        , totalLength_(0)
        , readLength_(0)
        , current_(0)
        , currentPtr_(null_ptr)
        , currentLength_(0)
{
    reset(buf, totalLength);
}

//InBuffer::InBuffer(LPWSABUF ptr, size_t count, size_t totalLength)
//: totalLength_(totalLength)
//, readLength_(0)
//, current_(0)
//, currentPtr_(null_ptr)
//, currentLength_(0)
//{
//  if(is_null(ptr))
//    ThrowException1(ArgumentNullException, "ptr");
//  if(0 == count)
//  {
//    totalLength_ = 0;
//    return;
//  }
//
//  for(size_t i =0; i < count; ++i)
//  {
//    memory_.push_back(ptr[i]);
//  }
//
//  currentPtr_ = memory_[current_].buf;
//  currentLength_ = memory_[current_].len;
//}

InBuffer::~InBuffer(void)
{
}

void InBuffer::reset(const std::vector<io_mem_buf>* iobuf, size_t totalLength)
{
    if (is_null(iobuf))
        ThrowException1(ArgumentNullException, _T("iobuf"));

    memory_ = iobuf;

    if (-1 == totalLength)
    {
        totalLength = 0;

        for (std::vector<io_mem_buf>::const_iterator it = iobuf->begin()
                ; it != iobuf->end(); ++ it)
        {
            totalLength += it->len;
        }
    }

    totalLength_ = totalLength;
    readLength_ = 0;
    current_ = 0;

    currentPtr_ = (*memory_)[current_].buf;
    currentLength_ = (*memory_)[current_].len;

    transcationDatas_.clear();
}

int InBuffer::beginTranscation()
{
    TranscationData data;
    data.current_ = this->current_;
    data.currentLength_ = this->currentLength_;
    data.currentPtr_ = this->currentPtr_;
    data.readLength_ = this->readLength_;
    data.exceptionStyle_ = this->exceptionStyle_;
    data.errno_ = this->errno_;

    transcationDatas_.push_back(data);
    return (int)transcationDatas_.size();
}

void InBuffer::rollbackTranscation(int id)
{
    if (id <= 0 || id > (int)transcationDatas_.size())
        ThrowException(OutOfRangeException);

    TranscationData& data = transcationDatas_[id-1];

    this->current_ = data.current_;
    this->currentLength_ = data.currentLength_;
    this->currentPtr_ = data.currentPtr_;
    this->readLength_ = data.readLength_;
    this->exceptionStyle_ = data.exceptionStyle_;
    this->errno_ = data.errno_;

    transcationDatas_.resize(id - 1);
}

void InBuffer::commitTranscation(int id)
{
    size_t index = id;
    if (index <= 0 || index > transcationDatas_.size())
        ThrowException(OutOfRangeException);

    for (size_t i = transcationDatas_.size(); i >= index; --i)
    {
        TranscationData& data = transcationDatas_[i-1];

        this->current_ = data.current_;
        this->currentLength_ = data.currentLength_;
        this->currentPtr_ = data.currentPtr_;
        this->readLength_ = data.readLength_;
        this->exceptionStyle_ = data.exceptionStyle_;
        this->errno_ = data.errno_;
    }

    transcationDatas_.resize(index - 1);
}

bool InBuffer::readBoolean()
{
    return 0 != readInt8();
}

int8_t InBuffer::readInt8()
{
    int8_t value = 0;
    readBlob((char*)&value, sizeof(value));
    return value;
}

int16_t InBuffer::readInt16()
{
    int16_t value = 0;
    readBlob((char*)&value, sizeof(value));
    return value;
}

int32_t InBuffer::readInt32()
{
    int32_t value = 0;
    readBlob((char*)&value, sizeof(value));
    return value;
}

int64_t InBuffer::readInt64()
{
    int64_t value = 0;
    readBlob((char*)&value, sizeof(value));
    return value;
}

void InBuffer::readBlob(void* blob, size_t len)
{
    if (ERROR_SUCCESS != this->error())
        return ;

    if (0 >= len)
        return ;

    if (currentLength_ >= len)
    {
        memcpy(blob, currentPtr_, len);
        currentPtr_ += len;
        currentLength_ -= len;
        readLength_ += len;
        return ;
    }

    if (size() < len)
    {
        this->error(ERROR_HANDLE_EOF);
        return;
    }

    char* ptr = (char*)blob;
    size_t count = len;
    do
    {
        if (currentLength_ > count)
        {
            memcpy(ptr, currentPtr_, count);
            currentPtr_ += count;
            currentLength_ -= count;
            break;
        }

        memcpy(ptr, currentPtr_, currentLength_);
        count -= currentLength_;
        ptr += currentLength_;

        if ((*memory_).size() <= ++current_)
        {
            currentPtr_ = null_ptr;
            currentLength_ = 0;
            break;
        }
        else
        {
            currentPtr_ = (*memory_)[current_].buf;
            currentLength_ = (*memory_)[current_].len;
        }
    }
    while (0 < count);
    readLength_ += len;
}

void InBuffer::seek(int offestLen)
{
    if (0 == offestLen)
        return;

    if (0 < offestLen)
    {
        size_t offest = offestLen;

        if (size() <= offest)
        {
            currentPtr_ = null_ptr;
            currentLength_ = 0;
            current_ = (*memory_).size();
            readLength_ = totalLength_;
            return;
        }

        size_t len = offest - currentLength_;
        for (size_t i = current_ + 1; i < (*memory_).size(); ++i)
        {
            if (len < (*memory_)[i].len)
            {
                current_ = i;
                currentPtr_ = (*memory_)[i].buf + len;
                currentLength_ = (*memory_)[i].len - len;
                readLength_ += offest;
                return;
            }
            len += (*memory_)[i].len;
        }
        currentPtr_ = null_ptr;
        currentLength_ = 0;
        current_ = (*memory_).size();
        readLength_ = totalLength_;
        return ;
    }

    size_t offest = ::abs(offestLen);
    if (readLength_ <= offest)
    {
        current_ = 0;
        readLength_ = 0;

        if ((*memory_).empty())
        {
            currentPtr_ = null_ptr;
            currentLength_ = 0;
        }
        else
        {
            currentPtr_ = (*memory_)[current_].buf;
            currentLength_ = (*memory_)[current_].len;
        }
        return;
    }

    size_t len = offest - ((*memory_)[current_].len - currentLength_);
    for (size_t i = current_ - 1; i >= 0 ; --i)
    {
        if (len < (*memory_)[i].len)
        {
            current_ = i;
            currentPtr_ = (*memory_)[i].buf + ((*memory_)[i].len - len);
            currentLength_ = len;
            readLength_ -= offest;
            return;
        }
        len += (*memory_)[i].len;
    }

    current_ = 0;
    readLength_ = 0;
    if ((*memory_).empty())
    {
        currentPtr_ = null_ptr;
        currentLength_ = 0;
    }
    else
    {
        currentPtr_ = (*memory_)[current_].buf;
        currentLength_ = (*memory_)[current_].len;
    }
}

size_t InBuffer::size() const
{
    return totalLength_ - readLength_;
}

size_t InBuffer::readLength() const
{
    return readLength_;
}


size_t InBuffer::search(char ch) const
{
    char* p = (char*)::memchr(currentPtr_, ch, currentLength_);
    if (!is_null(p))
        return p - currentPtr_;

    size_t len  = currentLength_;
    for (size_t i = current_ + 1; i < (*memory_).size(); ++i)
    {
        p = (char*)::memchr((*memory_)[i].buf, ch, (*memory_)[i].len);
        if (!is_null(p))
        {
            len += (p - (*memory_)[i].buf);
            return len;
        }

        len += (*memory_)[i].len;
    }

    return buffer_type::npos;
}

size_t InBuffer::search(wchar_t ch) const
{
    return search(&ch, sizeof(ch));
}

inline size_t mem_search(const void* mem, size_t searchLen, const void* context, size_t len)
{
    for (size_t i = 0; i < searchLen; ++i)
    {
        if (!is_null(::memcmp(mem, context, len)))
            return i;
    }
    return buffer_type::npos;
}

size_t InBuffer::search(const void* context, size_t len) const
{
    if (is_null(context) || 0 == len || size() < len)
        return buffer_type::npos;

    if (1 == len)
        return search(*(char*)context);

    // �ڶ���ڴ���в��Ҷ���ַ�ʱ,Ҫע��һ������,��������������
    // 1.�ַ�������һ���ڴ����,�� {"aaaacccc", "sdfsafdddsss" } �ַ���"ddd"�����ڵڶ�������
    // 2.�ַ��������������ڵ��ڴ����,�� {"aaaacccc", "sdfsafdddsss" } �ַ���"ccsd"�����ڶ�������
    // 3.�ַ����������ڵĶ���ڴ����,�� {"aaaacccc","sa" "sdfsafdddsss" } �ַ���"ccsasd"��������������

    // tmpbuf ��������һ���ڴ�������len-1���ַ�����һ���ڴ���ǰlen-1���ַ�,
    std::vector<char> tmpbuf(4*len);
    // tmpbuf ����Ч�ַ��ĳ���.
    size_t tmpLen = 0;
    // ��������"&tmpBuf[0]"�����Ĵ���
    char* tmpPtr = &tmpbuf[0];


    // ��Ҫ���ʵ���һ���ڴ��
    size_t i = current_ + 1;
    // �Ѳ��ҹ����ڴ���ܳ���
    size_t seekLen = 0;

    // ��ǰ���ڴ�����ڴ��
    const char* ptr = currentPtr_;
    // ��ǰ���ڴ�����ڴ��Ĵ�С
    size_t count = currentLength_;

    do
    {

        size_t searchLen = 0;
        if (count > len)
        {
            // ������֮ǰ���ʵ��ڴ��Ľ�β�����ַ�(ע���п����Ƕ���ڴ������ݼ�������˵�ĵ�3�����),��
            // �ڽ�����Ҫ�������ڴ��ͷ���ַ���������
            if (tmpLen > 0)
            {
                size_t copy = len - 1;

                if (0 != ::memcpy_s(tmpPtr + tmpLen, tmpbuf.size() - tmpLen, ptr, copy))
                    ThrowException(RuntimeException);

                searchLen = tmpLen;
                size_t index = mem_search(tmpPtr, searchLen, context, len);
                if (buffer_type::npos != index)
                {
                    seekLen += index;
                    return seekLen;
                }

                seekLen += tmpLen;
                tmpLen = 0;
            }

            // ���㵱ǰ�ڴ��Ҫ�����ĳ���,��������ǰ���ڴ��,
            searchLen = (count - len + 1);
            size_t index = mem_search(ptr, searchLen, context, len);
            if (buffer_type::npos != index)
            {
                seekLen += index;
                return seekLen;
            }
        }

        // �������������ĳ���,������ε��ڴ�鳤��С��Ŀ���ĳ���,��searchLen��tmpLen��Ϊ0.
        if (0 == ::memcpy_s(tmpPtr + tmpLen, tmpbuf.size() - tmpLen, ptr + searchLen, count - searchLen))
            ThrowException(RuntimeException);
        seekLen += searchLen;
        tmpLen += (count - searchLen);

        // ������������ڴ��С��len, ���ܳ��ȴ��� len�����,���������������
        // ��Ŀ�곤��lenΪ3,���� 60 ���ڴ��ĳ���Ϊ2,����������ʱ�ͻᵼ�� tmpPtr
        // ָ����ڴ治��.
        if (tmpLen > len)
        {
            // ���㵱ǰ�ڴ��Ҫ�����ĳ���,��������ǰ���ڴ��,
            searchLen = (tmpLen - len + 1);
            size_t index = mem_search(tmpPtr, searchLen, context, len);
            if (buffer_type::npos != index)
            {
                seekLen += index;
                return seekLen;
            }

            if (0 == ::memmove_s(tmpPtr, tmpbuf.size(), tmpPtr + searchLen, tmpLen - searchLen))
                ThrowException(RuntimeException);

            tmpLen  -= searchLen;
            seekLen += searchLen;
        }

        if ((*memory_).size() <= i)
            return buffer_type::npos;

        ptr = (*memory_)[i].buf;
        count = (*memory_)[i].len;
        ++i;
    }
    while (true);
    return buffer_type::npos;
}

size_t InBuffer::searchAny(const char* charset) const
{
    if (is_null(charset))
        return buffer_type::npos;

    size_t charsetLen = string_traits<char>::strlen(charset);
    if (1 == charsetLen)
        return search(*charset);

    for (size_t i = 0; i < currentLength_; ++i)
    {
        if (!is_null(::memchr(charset, currentPtr_[i], charsetLen)))
            return i;
    }

    size_t len  = currentLength_;
    for (size_t i = current_ + 1; i < (*memory_).size(); ++i)
    {
        const char* ptr = (*memory_)[i].buf;
        int count = (*memory_)[i].len;

        for (int j = 0; j < count; ++j)
        {
            if (!is_null(::memchr(charset, ptr[j], charsetLen)))
            {
                len += j;
                return len;
            }
        }
        len += (*memory_)[i].len;
    }
    return buffer_type::npos;
}

size_t InBuffer::searchAny(const wchar_t* charset) const
{
    ThrowException(NotImplementedException);
}

const std::vector<io_mem_buf>& InBuffer::rawBuffer() const
{
    return *memory_;
}

size_t InBuffer::rawLength() const
{
    return totalLength_;
}


# ifdef _GOOGLETEST_
TEST(buffer, bufferSearch)
{
    //TODO: ��д����
}
#endif


_jingxian_end
