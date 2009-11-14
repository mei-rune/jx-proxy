
#ifndef _IOCPServer_H_
#define _IOCPServer_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include <hash_map>
# include "jingxian/string/string.h"
# include "jingxian/logging/logging.h"
# include "jingxian/IReactorCore.h"
# include "jingxian/ISession.h"
# include "jingxian/networks/commands/ICommand.h"
# include "jingxian/networks/connection_status.h"
# include "jingxian/networks/networking.h"
# include "jingxian/networks/ThreadDNSResolver.h"
# include "jingxian/networks/ListenPort.H"

_jingxian_begin


typedef std::list<ISession*> SessionList;

class IOCPServer : public IReactorCore
{
public:
    IOCPServer(void);

    virtual ~IOCPServer(void);


    /**
     * ��ʼ���˿�(����Ѿ���ʼ������true)
       * @param[ in ] �����߳���
     */
    bool initialize(size_t number_of_threads);

    /**
     * @implements connectWith
     */
    virtual void connectWith(const tchar* endPoint
                             , OnBuildConnectionComplete onComplete
                             , OnBuildConnectionError onError
                             , void* context);

    /**
     * @implements listenWith
     */
    virtual bool listenWith(const tchar* endPoint, IProtocolFactory* protocolFactory);


    /**
     * @implements send
     */
    virtual bool send(IRunnable* runnable);

    /**
     * @implements runForever
     */
    virtual void runForever();

    /**
     * @implements interrupt
     */
    virtual void interrupt();

    /**
     * @implements bind
     */
    virtual bool bind(HANDLE systemHandler, void* completion_key);

    /**
     * @implements isRunning
     */
    virtual bool isRunning() const;

    /**
     * @implements resolver
     */
    virtual IDNSResolver& resolver();

    /**
     *  ����ʱִ�еĻص�������������Լ̳б�����
     */
    virtual void onIdle();

    /**
     * ��������
     */
    virtual void onExeception(int errCode, const tstring& description);

    /**
     * �������еĻ���·��
     */
    const tstring& basePath() const;

    /**
     * ���һ������
     * @param[ in ] session �Ự����
     * @return �������������е�λ��
     */
    SessionList::iterator addSession(ISession* session);

    /**
     * ɾ��һ������
     * @param[ in ] it ���������е�λ��
     */
    void removeSession(SessionList::iterator& it);

    /**
    * ȡ�õ�ַ������
    */
    virtual const tstring& toString() const;

private:
    NOCOPY(IOCPServer);

    /**
     * �رձ�����
     */
    void close(void);

    /**
     * ����һ���Ѿ���ɵ�������ɶ˿�
     */
    bool post(ICommand *result);

    /**
     * ��ȡ����ɵ��¼�,����������¼�
     * @return ��ʱ����1,��ȡ���¼����ɹ�������0,��ȡʧ�ܷ���-1
     */
    int handle_events(uint32_t milli_seconds);

    /**
     * ����δ���ص� IO ����
     */
    void wait(time_t seconds);

    /**
     * �ж��ǲ�����δ���ص� IO ����
     */
    bool isPending();

    void application_specific_code(ICommand *asynch_result,
                                   size_t bytes_transferred,
                                   const void *completion_key,
                                   errcode_t error);

    /// ��ɶ˿ھ��
    HANDLE completion_port_;
    /// ���Բ������߳���
    u_long number_of_threads_;
    /// �ǲ�����������
    bool isRunning_;
    /// socket ���Ӵ�������
    stdext::hash_map<tstring, IConnectionBuilder* > connectionBuilders_;
    /// Acceptor��������
    stdext::hash_map<tstring, IAcceptorFactory* > acceptorFactories_;
    /// ���ڼ�����Acceptor
    stdext::hash_map<tstring, ListenPort*> listenPorts_;
    /// dns ������
    ThreadDNSResolver resolver_;
    /// �������е� connection
    SessionList sessions_;
    /// �������еĻ���·��
    tstring path_;
    /// ��־�ӿ�
	logging::logger logger_;
    /// ʵ��������
    tstring toString_;
};

_jingxian_end

#endif //_IOCPServer_H_