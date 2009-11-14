
#ifndef _base_socket_h_
#define _base_socket_h_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "Winsock2.h"
# include "Mswsock.h"
# include "jingxian/string/string.h"
# include "jingxian/buffer/IBuffer.h"

_jingxian_begin

# ifndef _io_mem_buf_buf_
# define _io_mem_buf_buf_
typedef WSABUF io_mem_buf;
# endif //_io_mem_buf_buf_

# ifndef _io_packect_buf_
# define _io_packect_buf_
typedef TRANSMIT_PACKETS_ELEMENT io_packect_buf;
# endif // ___iopack___

# ifndef _io_file_buf_
# define _io_file_buf_
typedef TRANSMIT_FILE_BUFFERS io_file_buf;
# endif // _io_file_buf_

namespace networking
{
enum select_mode
{
    select_read = 1
    , select_write = 2
    , select_error = 4
};


/**
 * ��ʼ��socket����
 */
bool initializeScket();

/**
 * �ر�socket����
 */
void shutdownSocket();

/**
 * �ж� socket �Ƿ������ݿɶ�
 */
bool isReadable(SOCKET sock);

/**
 * �ж� socket �Ƿ��д
 */
bool isWritable(SOCKET sock);

/**
 * ���� socket �Ƿ�����
 */
void setBlocking(SOCKET sock, bool val);

/**
 * �жϲ��ȴ�ֱ��socket���Խ��ж�(д)�������������ʱ
 * @params[ in ] timval ��ʱʱ��
 * @params[ in ] mode �жϵĵĲ������ͣ����select_modeö��
 * @return ���Բ�������true
 */
bool poll(SOCKET sock, const TIMEVAL& timeval, int select_mode);

/**
 * @see MSDN
 */
bool transmitFile(SOCKET hSocket,
                  HANDLE hFile,
                  DWORD nNumberOfBytesToWrite,
                  DWORD nNumberOfBytesPerSend,
                  LPOVERLAPPED lpOverlapped,
                  LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers,
                  DWORD dwFlags);

/**
 * @see MSDN
 */
bool acceptEx(SOCKET sListenSocket,
              SOCKET sAcceptSocket,
              PVOID lpOutputBuffer,
              DWORD dwReceiveDataLength,
              DWORD dwLocalAddressLength,
              DWORD dwRemoteAddressLength,
              LPDWORD lpdwBytesReceived,
              LPOVERLAPPED lpOverlapped);

/**
 * @see MSDN
 */
bool transmitPackets(SOCKET hSocket,
                     LPTRANSMIT_PACKETS_ELEMENT lpPacketArray,
                     DWORD nElementCount,
                     DWORD nSendSize,
                     LPOVERLAPPED lpOverlapped,
                     DWORD dwFlags);

/**
 * @see MSDN
 */
bool connectEx(SOCKET s,
               const struct sockaddr* name,
               int namelen,
               PVOID lpSendBuffer,
               DWORD dwSendDataLength,
               LPDWORD lpdwBytesSent,
               LPOVERLAPPED lpOverlapped);

/**
 * @see MSDN
 */
bool disconnectEx(SOCKET hSocket,
                  LPOVERLAPPED lpOverlapped,
                  DWORD dwFlags,
                  DWORD reserved);

/**
 * @see MSDN
 */
void getAcceptExSockaddrs(PVOID lpOutputBuffer,
                          DWORD dwReceiveDataLength,
                          DWORD dwLocalAddressLength,
                          DWORD dwRemoteAddressLength,
                          LPSOCKADDR* LocalSockaddr,
                          LPINT LocalSockaddrLength,
                          LPSOCKADDR* RemoteSockaddr,
                          LPINT RemoteSockaddrLength);

/**
 *  �� <schema>://<addr>:<port> ��ʽ��ȡ�� addr������ schema �� port ��
 *  ��ѡ��
 */
tstring fetchAddr(const tchar* host);

/**
 *  �� <schema>://<addr>:<port> ��ʽ��ȡ�� port������ schema �� port ��
 *  ��ѡ��
 */
const tchar* fetchPort(const tchar* host);

/**
 * �� <schema>://<addr>:<port> ��ʽ��ȡ�� addr �� port ת���� sockaddr����
 * �� schema �� port �ǿ�ѡ��,���� schema �����һ���ַ��� '6' ʱ��ʾ����
 * IPv6��ʽ.
 */
bool stringToAddress(const tchar* host
                     , struct sockaddr* addr
                     , int* len);

bool addressToString(struct sockaddr* name
                     , int len
                     , const tchar* schema
                     , tstring& host);
}

_jingxian_end

#endif /* _base_socket_h_ */
