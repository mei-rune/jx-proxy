
#ifndef _communicator_h_
#define _communicator_h_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "jingxian/buffer.h"
# include "jingxian/endpoint.h"
# include "jingxian/exception.h"
# include "jingxian/IConcurrentPort.h"

_jingxian_begin

enum SystemEvent
{
    CtrlC = 0,
    CtrlBreak = 1,
    CtrlClose = 2,
    CtrlLogoff = 5,
    CtrlShutdown = 6
}

//public interface ICommunicatingWalker
//{
//    void OnBeginWalk(ICommunicator reactorCore);
//    void OnEndWalk(ICommunicator reactorCore);

//    void OnBeginWalk(IReactorCore reactorCore);
//    void OnEndWalk(IReactorCore reactorCore);

//    void OnSession(IReactorCore reactorCore, ISession session);
//    void OnListenPort(IReactorCore reactorCore, IAcceptor listeningPort);
//}

class communicator : public IConcurrentPort
{
public:

    /**
     * ��ʼ����ֱ������Interrupt�ŷ���
     */
    virtual void runForever() = 0;

    /**
     * ֹͣ����
     */
    virtual void interrupt() = 0;

    /**
     * ����һ���쳣�¼�
     */
    virtual void send(const Exception& e) = 0;

    /**
     * ����һ��ϵͳ�¼�
     */
    virtual void send(const SystemEvent& e) = 0;
};

_jingxian_end

#endif //_communicator_h_