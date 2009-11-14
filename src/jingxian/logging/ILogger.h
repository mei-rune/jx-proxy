
#ifndef _jingxian_log_h_
#define _jingxian_log_h_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include <assert.h>
# include "jingxian/string/string.h"
# include "jingxian/jingxian_export.h"

_jingxian_begin


namespace logging
{
enum
{
    Fatal
    , Error
    , Info
    , Debug
    , Warn
    , Trace
};


typedef long LevelPtr;

namespace spi
{

/**
 * @Brief ILogger ��־�ӿ�
 * ����BT����־�ӿڣ�ÿһ���������־����������̳У�ÿһ����Ҫ����־�Ķ����Եõ�һ�������Ľӣ�
 * �����Ƽ�ֱ��ʹ��������һ��Ԥ����õĺ꣬��ʹ�ú�
 */
class ILogger
{
public:


    /**
     * destructor
     */
    virtual ~ILogger() {};

    /**
     * assert ���
     * @param[ in ] assertion �����Ƿ�Ϊ��
     * @param[ in ] message ��־����
     * @param[ in ] file ��־��¼��Դ�ļ���
     * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
     */
    virtual void assertLog(bool assertion, const LogStream& msg, const char* file = 0, int line = -1) = 0;

    /**
     * fatal������־�Ƿ���Լ���־
     * @return ����true,������false
     */
    virtual bool isFatalEnabled() const = 0;

    /**
     * ��¼fatal������־
     * @param[ in ] message ��־����
     * @param[ in ] file ��־��¼��Դ�ļ���
     * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
     */
    virtual void fatal(const LogStream& message, const char* file = 0, int line = -1) = 0;

    /**
     * error������־�Ƿ���Լ���־
     * @return ����true,������false
     */
    virtual bool isErrorEnabled() const = 0;

    /**
     * ��¼error������־
     * @param[ in ] message ��־����
     * @param[ in ] file ��־��¼��Դ�ļ���
     * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
     */
    virtual void error(const LogStream& message, const char* file = 0, int line = -1) = 0;

    /**
     * info������־�Ƿ���Լ���־
     * @return ����true,������false
     */
    virtual bool isInfoEnabled() const = 0;

    /**
     * ��¼info������־
     * @param[ in ] message ��־����
     * @param[ in ] file ��־��¼��Դ�ļ���
     * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
     */
    virtual void info(const LogStream& message, const char* file = NULL, int line = -1) = 0;

    /**
     * debug������־�Ƿ���Լ���־
     * @return ����true,������false
     */
    virtual bool isDebugEnabled() const = 0;

    /**
     * ��¼debug������־
     * @param[ in ] message ��־����
     * @param[ in ] file ��־��¼��Դ�ļ���
     * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
     */
    virtual void debug(const LogStream& message, const char* file = 0, int line = -1) = 0;

    /**
     * warn������־�Ƿ���Լ���־
     * @return ����true,������false
     */
    virtual bool isWarnEnabled() const = 0;

    /**
     * ��¼warn������־
     * @param[ in ] message ��־����
     * @param[ in ] file ��־��¼��Դ�ļ���
     * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
     */
    virtual void warn(const LogStream& message, const char* file = NULL, int line = -1) = 0;

    /**
     * Trace������־�Ƿ���Լ���־
     * @return ����true,������false
     */
    virtual bool isTraceEnabled() const = 0;

    /**
     * ��¼trace������־
     * @param[ in ] message ��־����
     * @param[ in ] file ��־��¼��Դ�ļ���
     * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
     */
    virtual void trace(const LogStream& message, const char* file = NULL, int line = -1) = 0;

    /**
     * Trace������־�Ƿ���Լ���־
     * @return ����true,������false
     */
    virtual bool isCritEnabled() const = 0;

    /**
     * ��¼trace������־
     * @param[ in ] message ��־����
     * @param[ in ] file ��־��¼��Դ�ļ���
     * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
     */
    virtual void crit(const LogStream& message, const char* file = NULL, int line = -1) = 0;

    /**
     * level������־�Ƿ���Լ���־
     * @return ����true,������false
     */
    virtual bool isEnabledFor(const logging::LevelPtr& level) const = 0;

    /**
     * ��¼level������־
     * @param[ in ] message ��־����
     * @param[ in ] file ��־��¼��Դ�ļ���
     * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
     */
    virtual void log(const logging::LevelPtr& level, const LogStream& message,
                     const char* file = 0, int line = -1) = 0;

    /**
     * ȡ�õ�ǰ����־�ļ���
     * @return ��־�ļ���
     */
    virtual logging::LevelPtr getLevel() const = 0;
};

class ILogFactory
{
public:

    virtual ~ILogFactory() {};

    virtual ILogger* make(const tchar* nm) = 0;
};

}

}

_jingxian_end


#endif // _JINGXIAN_Log_H_