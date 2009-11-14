

#ifndef _CONNECTED_SOCKET_H_
#define _CONNECTED_SOCKET_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include <Winsock2.h>
# include <fstream>
# include "jingxian/IProtocol.h"
# include "jingxian/ISession.h"
# include "jingxian/ProtocolContext.h"
# include "jingxian/networks/connection_status.h"
# include "jingxian/logging/logging.h"
# include "jingxian/networks/IOCPServer.h"
# include "jingxian/networks/TCPContext.h"
# include "jingxian/networks/buffer/IncomingBuffer.h"
# include "jingxian/networks/buffer/OutgoingBuffer.h"

_jingxian_begin

/**
 * On��ͷ�ĺ������û�ֱ�ӵ��õķ����в�����ʹ�á�
 * ���û����õķ���ΪITransport�ӿ��еķ���
 */
class ConnectedSocket : public ITransport, public ISession
{
public:

    ConnectedSocket(IOCPServer* core
                    , SOCKET sock
                    , const tstring& host
                    , const tstring& peer);

    virtual ~ConnectedSocket();

    /**
     * @implements initialize
     */
    virtual void initialize();

    /**
     * @implements bindProtocol
     */
    virtual IProtocol* bindProtocol(IProtocol* protocol);

    /**
     * @implements startReading
     */
    virtual void startReading();

    /**
     * @implements stopReading
     */
    virtual void stopReading();

    /**
     * @implements write
     */
    virtual void write(buffer_chain_t* buffer);
    virtual void writeBatch(buffer_chain_t** buffers, size_t len);

    /**
     * @implements disconnection
     */
    virtual void disconnection();

    /**
     * @implements disconnection
     */
    virtual void disconnection(const tstring& error);

    /**
     * @implements host
     */
    virtual const tstring& host() const;

    /**
     * @implements peer
     */
    virtual const tstring& peer() const;

    /**
     * @implements timeout
     */
    virtual time_t timeout() const;

    /**
     * @implements transport
     */
    virtual ITransport* transport()
    {
        return this;
    }

    /**
     * @implements protocol
     */
    virtual IProtocol*  protocol()
    {
        return protocol_;
    }

    /**
     * @implements toString
     */
    virtual const tstring& toString() const;


    SOCKET handle()
    {
        return socket_;
    }
    ITracer* tracer()
    {
        return tracer_;
    }


    void onWrite(const ICommand& command, size_t bytes_transferred);
    void onRead(const ICommand& command, size_t bytes_transferred);
    void onError(const ICommand& command, transport_mode::type mode, errcode_t error, const tstring& description);
    void onDisconnected(const ICommand& command, errcode_t error, const tstring& description);

    buffer_chain_t* allocateProtocolBuffer();

private:
    NOCOPY(ConnectedSocket);

    void doRead();
    void doWrite();
    void doDisconnect(transport_mode::type mode, errcode_t error, const tstring& description);


    /// iocp���������
    IOCPServer* core_;
    /// socket ����
    SOCKET socket_;
    /// ���ص�ַ
    tstring host_;
    /// Զ�̵�ַ
    tstring peer_;
    /// ������ǰ������״̬
    connection_status::type state_;
    /// ��ʱʱ��
    time_t timeout_;
    /// Э�鴦����
    IProtocol* protocol_;
    /// �����������
    TCPContext context_;
    /// �Ƿ��ѳ�ʼ��
    bool isInitialize_;
    ///��ͣ����ʱ������������ʱ���λ��

    bool stopReading_;
    /// ��ʾ����һ��������,����û�з���
    bool reading_;
    IncomingBuffer incoming_;

    /// ��ʾ����һ��д����,����û�з���
    bool writing_;
    OutgoingBuffer outgoing_;

    /// ������Ͽ�,Ϊ������ٷ�������д������.
    bool shutdowning_;
    /// ���汻ֹͣ��ԭ��
    tstring disconnectReason_;

    ///�ǲ���ӵ�core��sessions������
    bool isPosition_;
    ///core��sessions�����е�λ��
    SessionList::iterator sessionPosition_;

    /// ��־����
    ITracer* tracer_;
    tstring toString_;

#ifdef DUMPFILE
    std::auto_ptr<std::ofstream> os;
    std::auto_ptr<std::ofstream> is;
#endif
};

_jingxian_end

#endif // _CONNECTED_SOCKET_H_