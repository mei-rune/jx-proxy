
#ifndef _EchoProtocol_H_
#define _EchoProtocol_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "jingxian/protocol/BaseProtocol.h"
# include "jingxian/buffer/OutBuffer.h"


_jingxian_begin

class EchoProtocol : public BaseProtocol
{
public:
    EchoProtocol()
		: BaseProtocol(_T("EchoProtocol"))
    {
    }

    /**
     * ��ָ����ʱ�����û���յ��κ�����
     *
     * @param[ in ] context �Ự��������
    */
    virtual void onTimeout(ProtocolContext& context)
    {
    }

    /**
     * ���Ự�����󣬱����á�
     *
     * @param[ in ] context �Ự��������
    */
    virtual void onConnected(ProtocolContext& context)
    {
        LOG_INFO(logger_, _T("�����ӵ��� - ") << context.transport().peer());
    }

    /**
     * ���Ự�رպ󣬱����á�
     *
     * @param[ in ] context �Ự��������
     * @param[ in ] errCode �رյ�ԭ��,Ϊ0�Ǳ�ʾ�����ر�
     * @param[ in ] reason �رյ�ԭ������
    */
    virtual void onDisconnected(ProtocolContext& context, errcode_t errCode, const tstring& reason)
    {
        LOG_INFO(logger_, _T("���ӶϿ� - ") << context.transport().peer());
    }

    /**
     * �����µ���Ϣ����ʱ�������á�
     *
     * @param[ in ] context �Ự��������
     * @param[ in ] buffer �����µ�����Ϣ�Ļ�����
    */
    virtual size_t onReceived(ProtocolContext& context)
    {
        OutBuffer out(&context.transport());
        for (std::vector<io_mem_buf>::const_iterator it = context.inMemory().begin()
                ; it != context.inMemory().end(); ++ it)
            out.writeBlob(it->buf, it->len);

        return out.size();
    }
private:
    NOCOPY(EchoProtocol);
};

_jingxian_end

#endif //_EchoProtocol_H_