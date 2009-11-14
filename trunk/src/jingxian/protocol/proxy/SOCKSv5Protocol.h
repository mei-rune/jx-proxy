
#ifndef _SOCKSv5Protocol_H_
#define _SOCKSv5Protocol_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "jingxian/buffer/OutBuffer.h"
# include "jingxian/Connector.h"
# include "jingxian/protocol/BaseProtocol.h"
# include "jingxian/protocol/proxy/SOCKSv5Incoming.h"
# include "jingxian/protocol/proxy/SOCKSv5Outgoing.h"
# include "jingxian/protocol/proxy/ICredentialPolicy.h"

_jingxian_begin

namespace proxy
{
class Proxy;

namespace SocksError
{
enum Type
{
    Success = 0x5A//    ����ת��
    ,
    Error = 0x5B//    �ܾ�ת����һ����ʧ��
    ,
    NoConnect = 0x5C//    �ܾ�ת����SOCKS 4 Server�޷����ӵ�SOCS 4 Client����������
    //              IDENT����
    , ErrorUser = 0x5D//    �ܾ�ת�����������е�USERID��IDENT���񷵻�ֵ�����

};
}

namespace SOCKSv5Error
{
enum Type
{
    Success = 0 //       �ɹ�
    ,
    Error = 0x01 //        һ����ʧ��
    ,
    Deny = 0x02 //        ��������ת��
    ,
    NetworkError = 0x03 //        ���粻�ɴ�
    ,
    HostError = 0x04 //        �������ɴ�
    ,
    Timeout = 0x05 //        ���Ӿܾ�
    ,
    TTLError = 0x06 //        TTL��ʱ
    ,
    NotSupportCommand = 0x07 //        ��֧��������е�CMD
    ,
    NotSupportAddr = 0x08 //        ��֧��������е�ATYP
};
}

class SOCKSv5Protocol : public BaseProtocol
{
public :
    SOCKSv5Protocol(Proxy* server);
    ~SOCKSv5Protocol();

    virtual void onConnected(ProtocolContext& context);
    virtual size_t onReceived(ProtocolContext& context);
    virtual void onDisconnected(ProtocolContext& context, errcode_t errCode, const tstring& reason);

    virtual void writeIncoming(const std::vector<io_mem_buf>& buf);
    virtual void writeOutgoing(const std::vector<io_mem_buf>& buf);

    void onError(ProtocolContext& context);
    size_t onHello(ProtocolContext& context, InBuffer& inBuffer);
    size_t onAuthenticating(ProtocolContext& context, InBuffer& inBuffer);
    size_t onCommand(ProtocolContext& context, InBuffer& inBuffer);

    void connectTo(ProtocolContext& context, const tstring& host);
    void onConnectComplete(ITransport* transport, ProtocolContext& context);
    void onConnectError(const ErrorCode&, ProtocolContext& context);

    void sendReply(ProtocolContext& context, int reply, int version, int addressType, const char* addr, size_t len, int port);

    Proxy* internalCore();
private:
    Proxy* server_;
    int status_;
    std::auto_ptr<proxy::ICredentialPolicy> credentialPolicy_;
    typedef ConnectProxy<SOCKSv5Protocol, ProtocolContext&> connectorType;
    connectorType* connectProxy_;

    SOCKSv5Outgoing outgoing_;
    SOCKSv5Incoming incoming_;

#ifdef DEBUG_TRACE
    tstring sessionPath_;
    FILE* sessionId_;
#endif
};
}

_jingxian_end

#endif //_SOCKSv5Protocol_H_