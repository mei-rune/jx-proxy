
#ifndef _NTSERVICE_H_
#define _NTSERVICE_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include <vector>
# include "jingxian/string/os_string.h"

_jingxian_begin

class IInstance
{
public:
    virtual ~IInstance() { }

    /**
     * NT ������
     */
    virtual const tstring& name() const = 0;

    /**
     * ��������
     */
    virtual int onRun(const std::vector<tstring>& arguments) = 0;

    /**
     * ���յ�һ���û������֪ͨ
     * @param dwEventType �û�������¼�����
     * @param lpEventData �û�������¼�����
     * @remarks ע�⣬�����Է����쳣��
     */
    virtual void onControl(DWORD dwEventType
                           , LPVOID lpEventData) = 0;


    /**
     * ���յ�һ������ֹͣ��֪ͨ
     * @remarks ע�⣬�����Է����쳣��
     */
    virtual void interrupt() = 0;

    /**
     * ���������
     */
    virtual const tstring& toString() const = 0;
};


/**
 * ������ں���
 */
int serviceMain(IInstance* instance);//, DWORD argc,LPTSTR *argv);

/**
 * ��װһ�� Win32 ����
 * @param[ in ] name Win32 ���������
 * @param[ in ] display Win32 �����������Ϣ
 * @param[ in ] executable Win32 �����ִ�г�������
 * @param[ in ] args Win32 ����Ĳ���
 * @return �ɹ�����0,���򷵻ط�0
 */
int installService(const tstring& name
                   , const tstring& display
                   , const tstring& description
                   , const tstring& executable
                   , const std::vector<tstring>& args
                   , tostream& out);

/**
 * ж��һ�� Win32 ����
 * @param[ in ] name Win32 ���������
 * @return �ɹ�����0,���򷵻ط�0
 */
int uninstallService(const tstring& name, tostream& out);

/**
 * ����һ�� Win32 ����
 * @param[ in ] name Win32 ���������
 * @param[ in ] args Win32 ����Ĳ���
 * @return �ɹ�����0,���򷵻ط�0
 */
int startService(const tstring& name, const std::vector<tstring>& args, tostream& out);

/**
 * ֹͣһ�� Win32 ����
 * @param[ in ] name Win32 ���������
 * @return �ɹ�����0,���򷵻ط�0
 */
int stopService(const tstring& name, tostream& out);

_jingxian_end

#endif // _NTSERVICE_H_
