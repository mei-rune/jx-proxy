
#ifndef _itransport_h_
#define _itransport_h_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "jingxian/exception.h"
# include "jingxian/buffer/buffer.h"

_jingxian_begin

class IProtocol;

class ITransport
{
public:
    virtual ~ITransport() {}

    /**
     * ��ʼ�� Transport ʵ��
     */
    virtual void initialize() = 0;

    /**
         * ָ���� @see{IProtocol} �ӿ������ն���������
     * @return ���ؾɵ� @see{IProtocol} �ӿ�
         */
    virtual IProtocol* bindProtocol(IProtocol* protocol) = 0;

    /**
     * ��ʼ������
     */
    virtual void startReading() = 0;

    /**
     * ֹͣ������
     */
    virtual void stopReading() = 0;

    /**
     * �������ݣ�ע�������첽��  )
     * @param[ in ] buffer �����͵����ݿ�
     */
    virtual void write(buffer_chain_t* buffer) = 0;

    /**
         * ���Ͷ�����ݣ�ע�������첽��  )
         * @param[ in ] buffers �����͵����ݿ�����ָ��
         * @param[ in ] len ���ݿ�ĸ���
         */
    virtual void writeBatch(buffer_chain_t** buffers, size_t len) = 0;

    /**
     * �ر�����
     */
    virtual void disconnection() = 0;

    /**
         * �ر�����
         */
    virtual void disconnection(const tstring& error) = 0;

    /**
     * Դ��ַ
     */
    virtual const tstring& host() const = 0;

    /**
     * Ŀ���ַ
     */
    virtual const tstring& peer() const = 0;

    /**
     * ���� @see{protocol} ��onTimeout�¼��ĳ�ʱʱ��
     */
    virtual time_t timeout() const = 0;

    /**
     * ȡ�õ�ַ������
     */
    virtual const tstring& toString() const = 0;
};


class ErrorCode
{
public:
    ErrorCode(const tchar* err)
            : code_(0)
            , err_(err)
    {}

    ErrorCode(int code)
            : code_(code)
            , err_(lastError(code))
    {
    }

    ErrorCode(int code, const tstring& err)
            : code_(code)
            , err_(err)
    {
    }

    ErrorCode(int code, const tchar* err)
            : code_(code)
            , err_(err)
    {
    }

    virtual ~ErrorCode()
	{
	}


    int errorCode() const
    {
        return code_;
    }

    const tstring& toString() const
    {
        return err_;
    }

private:
    int code_;
    tstring err_;
};

inline tostream& operator<<(tostream& target, const ITransport& transport)
{
    target << transport.toString();
    return target;
}

inline tostream& operator<<(tostream& target, const ErrorCode& err)
{
    target << err.toString();
    return target;
}

typedef void (*OnBuildConnectionComplete)(ITransport* transport, void* context);
typedef void (*OnBuildConnectionError)(const ErrorCode& err,  void* context);

_jingxian_end

#endif //_itransport_h_
