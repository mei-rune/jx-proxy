
# include "pro_config.h"
# include <windows.h>
# include <winsvc.h>
# include "jingxian/lastError.h"
# include "jingxian/logging/logging.h"
# include "jingxian/utilities/NTService.h"

_jingxian_begin

#define SERVICE_NAME_SIZE       1024

static  logging::logger*         serviceLogger;
static  IInstance*              serviceInstance;
static  SERVICE_STATUS          serviceStatus;
static  SERVICE_STATUS_HANDLE   serviceHandle;
static  TCHAR                   serviceName[SERVICE_NAME_SIZE];
static  SERVICE_TABLE_ENTRY     serviceTable[] =
	{
		{ NULL, NULL },
		{ NULL, NULL }
	};


/**
 * ������ƺ���
 */
static DWORD WINAPI serviceCtrlHandler(DWORD dwControl,
                                       DWORD dwEventType,
                                       LPVOID lpEventData,
                                       LPVOID lpContext)
{
    serviceStatus.dwServiceType   = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwCurrentState    = SERVICE_RUNNING;
    serviceStatus.dwControlsAccepted  = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    serviceStatus.dwWin32ExitCode   = 0;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint    = 0;
    serviceStatus.dwWaitHint    = 0;

    switch (dwControl)
    {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        //LOG_CRITICAL((*serviceLogger), serviceName << _T( "  �����յ�ֹͣ����!"));

        serviceStatus.dwCurrentState  = SERVICE_STOP_PENDING;
        serviceStatus.dwWaitHint  = 4000;
        if (!SetServiceStatus(serviceHandle, &serviceStatus))
            LOG_WARN((*serviceLogger), serviceName
                     << _T("  �������� 'SERVICE_STOP_PENDING' ״̬ʧ�� - ")
                     << lastError(GetLastError()));

        serviceInstance->interrupt();
        break;
    default:
        serviceInstance->onControl(dwEventType, lpEventData);
        break;
    }
    return NO_ERROR;
}

/**
 * ������ں���
 */
static VOID WINAPI serviceEntry(DWORD argc, LPTSTR *argv)
{
    serviceHandle = RegisterServiceCtrlHandlerEx(serviceName, serviceCtrlHandler, NULL);
    if (0 == serviceHandle)
    {
        LOG_FATAL((*serviceLogger), serviceName << _T("  ����ע����ƻص�ʧ�� - ") << lastError(GetLastError()));
        return;
    }

    /* �������� */
    serviceStatus.dwServiceType   = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwCurrentState    = SERVICE_START_PENDING;
    serviceStatus.dwControlsAccepted  = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    serviceStatus.dwWin32ExitCode   = 0;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint    = 0;
    serviceStatus.dwWaitHint    = 2000;

    if (!SetServiceStatus(serviceHandle, &serviceStatus))
    {
        LOG_WARN((*serviceLogger), serviceName
                 << _T("  �������� 'SERVICE_START_PENDING' ״̬ʧ�� - ")
                 << lastError(GetLastError()));
    }

    /* ���������� */
    serviceStatus.dwCurrentState  = SERVICE_RUNNING;
    serviceStatus.dwWaitHint  = 0;
    if (!SetServiceStatus(serviceHandle, &serviceStatus))
    {
        LOG_WARN((*serviceLogger), serviceName
                 << _T("  �������� 'SERVICE_RUNNING' ״̬ʧ�� - ")
                 << lastError(GetLastError()));
    }

    std::vector<tstring> arguments;
    for (DWORD i = 0; i < argc; ++ i)
    {
        arguments.push_back(argv[i]);
    }


    //LOG_CRITICAL((*serviceLogger),  serviceName << _T( "  ���������ɹ�������������......"));
    serviceInstance->onRun(arguments);
    //LOG_CRITICAL((*serviceLogger),  serviceName << _T( "  �����˳������н���!"));

    serviceStatus.dwCurrentState  = SERVICE_STOPPED;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwCheckPoint  = 0;
    serviceStatus.dwWaitHint  = 0;
    if (!SetServiceStatus(serviceHandle, &serviceStatus))
        LOG_FATAL((*serviceLogger), serviceName << _T("  �������� 'SERVICE_STOPPED' ״̬ʧ�� - ") << lastError(GetLastError()));
}


/**
 * ��������
 */
int serviceMain(IInstance* instance)
{
    int result = -1;
    if (NULL == serviceLogger)
        serviceLogger = new logging::logger(_T("jingxian.system"));

    if (0 != _tcscpy_s(serviceName, SERVICE_NAME_SIZE, instance->name().c_str()))
    {
        LOG_WARN((*serviceLogger), _T("��������ʱ��������, ������ '") << instance->name() << _T("' ̫����"));
        SetLastError(ERROR_INVALID_NAME);

        result = -1;
        goto end;
    }

    serviceTable[0].lpServiceName = serviceName;
    serviceTable[0].lpServiceProc = serviceEntry;
    serviceInstance = instance;
    if (StartServiceCtrlDispatcher(serviceTable))
    {
        result = 0;
        goto end;
    }

    //if(ERROR_FAILED_SERVICE_CONTROLLER_CONNECT == GetLastError())
    //{
    //  std::vector<tstring> arguments;
    //  for(DWORD i = 0; i< argc; ++ i)
    //  {
    //    arguments.push_back(argv[i]);
    //  }
    //  instance->onRun(arguments);
    //
    //  result = 0;
    //  goto end;
    //}

end:
    if (NULL != serviceLogger)
        delete serviceLogger;
    serviceLogger = NULL;
    return result;
}

bool  waitForServiceState(SC_HANDLE hService, DWORD pendingState, SERVICE_STATUS& status)
{
    if (!QueryServiceStatus(hService, &status))
        return false;

    // ������ʼtick�����ݺͳ�ʼcheckpoint.
    DWORD startTickCount = GetTickCount();
    DWORD oldCheckPoint = status.dwCheckPoint;
    int tries = 60;

    // ��ѯ����״̬
    while (status.dwCurrentState != pendingState)
    {
        // ����ȴ�ʱ��( 1�� �� 10��)
        status.dwWaitHint = 1000;
        Sleep(1000);

        // �ټ�����״̬
        if (!QueryServiceStatus(hService, &status))
            return false;

        if (status.dwCheckPoint > oldCheckPoint)
        {
            // ����ǰ��һ����
            startTickCount = GetTickCount();
            oldCheckPoint = status.dwCheckPoint;
            tries = 60;
        }
        else if (--tries < 0)
        {
            break;
        }
    }

    return true;
}

void  showServiceStatus(const tstring& msg, SERVICE_STATUS& status, tostream& out)
{
    out << _T("CRIT ") <<  msg << _T(",")
    << _T("  ��ǰ״̬: ");

    switch (status.dwCurrentState)
    {
    case SERVICE_STOPPED:
        out << _T("��ֹͣ");
        break;
    case SERVICE_START_PENDING:
        out << _T("������");
        break;
    case SERVICE_STOP_PENDING:
        out << _T("ֹͣ��");
        break;
    case SERVICE_RUNNING:
        out << _T("������");
        break;
    case SERVICE_CONTINUE_PENDING:
        out << _T("�ָ���");
        break;
    case SERVICE_PAUSE_PENDING:
        out << _T("��ͣ��");
        break;
    case SERVICE_PAUSED:
        out << _T("����ͣ");
        break;
    default:
        out << _T("δ֪");
        break;
    }
    out << _T(",")
    << _T("  �˳�����: ") << status.dwWin32ExitCode << _T(",")
    << _T("  �����˳�����: ") << status.dwServiceSpecificExitCode << _T(",")
    << _T("  ����: ") << status.dwCheckPoint << _T(",")
    << _T("  �ȴ�������: ") << status.dwWaitHint << std::endl;
}

int  installService(const tstring& name
                    , const tstring& display
                    , const tstring& description
                    , const tstring& executable
                    , const std::vector<tstring>& args
                    , tostream& out)
{
    if (name.size() >= 254)
    {
        out << _T("��װ���� '") << name << _T("' ʧ�� - ������̫��!") << std::endl;
        return -1;
    }

    tstring disp = display;
    if (disp.empty())
    {
        disp = name;
    }

    tstring exec = executable;
    if (exec.empty())
    {
        // ʹ�ñ�ִ���ļ�
        tchar buf[_MAX_PATH];
        if (GetModuleFileName(NULL, buf, _MAX_PATH) == 0)
        {
            out << _T("ERROR ��װ���� '") << name << _T("' ʧ�� - û��ִ���ļ���!") << std::endl;
            return -1;
        }
        exec = buf;
    }

    // ����пո�Ļ���������
    tstring command;
    if (executable.find(_T(' ')) != tstring::npos)
    {
        command.push_back(_T('"'));
        command.append(exec);
        command.push_back(_T('"'));
    }
    else
    {
        command = exec;
    }

    // ƴ��ѡ���ַ���
    for (std::vector<tstring>::const_iterator p = args.begin(); p != args.end(); ++p)
    {
        command.push_back(_T(' '));

        if (p->find_first_of(_T(" \t\n\r")) != tstring::npos)
        {
            command.push_back(_T('"'));
            command.append(*p);
            command.push_back(_T('"'));
        }
        else
        {
            command.append(*p);
        }
    }

    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        out << _T("ERROR ��װ���� '") << name << _T("' ʧ��,���ܴ� SCM - ") << ::lastError(GetLastError()) << std::endl;
        return -1;
    }
    SC_HANDLE hService = CreateService(
                             hSCM,
                             name.c_str(),
                             disp.c_str(),
                             SC_MANAGER_ALL_ACCESS,
                             SERVICE_WIN32_OWN_PROCESS,
                             SERVICE_AUTO_START,
                             SERVICE_ERROR_NORMAL,
                             command.c_str(),
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL);

    if (hService == NULL)
    {
        out << _T("ERROR ��װ���� '") << name << _T("' ʧ��,���ܴ�����ʵ�� - ") << ::lastError(GetLastError()) << std::endl;
        CloseServiceHandle(hSCM);
        return -1;
    }

    if (!description.empty())
    {
        SERVICE_DESCRIPTION descr;
        descr.lpDescription = (tchar*)description.c_str();
        if (!ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &descr))
        {
            out << _T("WARN ��װ���� '") << name << _T("' ʱ,�������ʧ�� - ") << ::lastError(GetLastError()) << std::endl;
        }
    }

    CloseServiceHandle(hSCM);
    CloseServiceHandle(hService);

    out << _T("CRIT ��װ���� '") << name << _T("' �ɹ�") << std::endl;
    return 0;
}

int  uninstallService(const tstring& name, tostream& out)
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        out << _T("ERROR ж�ط��� '") << name << _T("' ʧ��,���ܴ� SCM - ") << ::lastError(GetLastError()) << std::endl;
        return -1;
    }

    SC_HANDLE hService = OpenService(hSCM, name.c_str(), SC_MANAGER_ALL_ACCESS);
    if (hService == NULL)
    {
        out << _T("ERROR ж�ط��� '") << name << _T("' ʧ��,���ܴ򿪷��� - ") << ::lastError(GetLastError()) << std::endl;
        CloseServiceHandle(hSCM);
        return -1;
    }

    BOOL b = DeleteService(hService);

    if (!b)
    {
        out << _T("ERROR ж�ط��� '") << name << _T("' ʧ��,����ɾ������ - ") << ::lastError(GetLastError()) << std::endl;
    }
    else
    {
        out << _T("CRIT ж�ط��� '") << name << _T("' �ɹ�") << std::endl;
    }

    CloseServiceHandle(hSCM);
    CloseServiceHandle(hService);

    return (b ? 0 : -1);
}

int  startService(const tstring& name, const std::vector<tstring>& args, tostream& out)
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        out << _T("ERROR �������� '") << name << _T("' ʧ��,���ܴ� SCM - ") << ::lastError(GetLastError()) << std::endl;
        return -1;
    }

    SC_HANDLE hService = OpenService(hSCM, name.c_str(), SC_MANAGER_ALL_ACCESS);
    if (hService == NULL)
    {
        out << _T("ERROR �������� '") << name << _T("' ʧ��,���ܴ򿪷��� - ") << ::lastError(GetLastError()) << std::endl;
        CloseServiceHandle(hSCM);
        return -1;
    }

    // ���ַ���ƴ�� char* []��ʽ
    const size_t argc = args.size();
    const tchar** argv = new const tchar*[argc];
    size_t i = 0;
    for (std::vector<tstring>::const_iterator p = args.begin(); p != args.end(); ++p)
    {
        argv[i++] = string_traits<tchar>::strdup(p->c_str());
    }

    // ��������
    BOOL b = StartService(hService, (DWORD)argc, argv);

    // �ͷ��ڴ�
    for (i = 0; i < argc; ++i)
    {
        string_traits<tchar>::free((tchar*)argv[i]);
    }
    delete[] argv;

    if (!b)
    {
        out << _T("ERROR �������� '") << name << _T("' ʧ�� - ") << ::lastError(GetLastError()) << std::endl;
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCM);
        return -1;
    }

    out << _T("TRACE ��������������,���Ե�...") << std::endl;

    SERVICE_STATUS status;
    if (!waitForServiceState(hService, SERVICE_RUNNING, status))
    {
        out << _T("ERROR �������� '") << name << _T("' ʧ��, ������״̬�������� - ") << ::lastError(GetLastError()) << std::endl;
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCM);
        return -1;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);

    if (status.dwCurrentState == SERVICE_RUNNING)
    {
        out << _T("CRIT �������� '") << name << _T("' �ɹ�, ����������.") << std::endl;
    }
    else
    {
        showServiceStatus(_T("������������������"), status, out);
        return -1;
    }

    return 0;
}

int  stopService(const tstring& name, tostream& out)
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        out << _T("ERROR ֹͣ���� '") << name << _T("' ʧ��, ���ܴ� SCM - ") << lastError(GetLastError()) << std::endl;
        return -1;
    }

    SC_HANDLE hService = OpenService(hSCM, name.c_str(), SC_MANAGER_ALL_ACCESS);
    if (hService == NULL)
    {
        out << _T("ERROR ֹͣ���� '") << name << _T("' ʧ��, ���ܴ򿪷��� - ") << lastError(GetLastError()) << std::endl;
        CloseServiceHandle(hSCM);
        return -1;
    }

    SERVICE_STATUS status;
    BOOL b = ControlService(hService, SERVICE_CONTROL_STOP, &status);

    if (!b)
    {
        out << _T("ERROR ֹͣ���� '") << name << _T("' ʧ�� - ") << lastError(GetLastError()) << std::endl;
        CloseServiceHandle(hSCM);
        CloseServiceHandle(hService);
        return -1;
    }

    out << _T("TRACE ����ֹͣ��,���Ե�...") << std::endl;

    ////
    //// �ȴ�����ֹͣ����һ������
    ////
    //if(!waitForServiceState(hService, SERVICE_STOP_PENDING, status))
    //{
    //    LOG_ERROR(logger_, _T("ֹͣ���� '") << name <<_T("' ʧ��,������״̬�������� - ") << lastError(GetLastError()) << std::endl;
    //    CloseServiceHandle(hService);
    //    CloseServiceHandle(hSCM);
    //    return -1;
    //}

    if (!waitForServiceState(hService, SERVICE_STOPPED, status))
    {
        out << _T("ERROR ֹͣ���� '") << name << _T("' ʧ��,������״̬�������� - ") << lastError(GetLastError()) << std::endl;
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCM);
        return -1;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);

    if (status.dwCurrentState == SERVICE_STOPPED)
    {
        out << _T("CRIT ֹͣ���� '") << name << _T("' �ɹ�.") << std::endl;
    }
    else
    {
        showServiceStatus(_T("������ֹͣ��������"), status, out);
        return -1;
    }

    return 0;
}

_jingxian_end