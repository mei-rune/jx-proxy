
#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "jingxian/logging/ILogger.h"
# include "jingxian/logging/ITracer.h"


_jingxian_begin

namespace logging
{
namespace spi
{
ILogger* makeLogger(const tstring& nm);

ILogFactory* setLogFactory(ILogFactory* factory);

ITracer* makeTracer(const tstring& nm, const tstring& host, const tstring& peer, const tstring& sessionId);

ITraceFactory* setTraceFactory(ITraceFactory* factory);
}

/**
 * @Brief ILogger ��־�ӿ�
 * ����BT����־�ӿڣ�ÿһ���������־����������̳У�ÿһ����Ҫ����־�Ķ����Եõ�һ�������Ľӣ�
 * �����Ƽ�ֱ��ʹ��������һ��Ԥ����õĺ꣬��ʹ�ú�
 */
class logger
{
public:
  //logger(const tchar* nm);

  logger(const tstring& nm);

  virtual ~logger();

  /**
   * assert ���
   * @param[ in ] assertion �����Ƿ�Ϊ��
   * @param[ in ] message ��־����
   * @param[ in ] file ��־��¼��Դ�ļ���
   * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
   */
  virtual void assertLog(bool assertion, const LogStream& msg, const char* file, int line);

  /**
   * CRIT ������־�Ƿ���Լ���־
   * @return ����true,������false
   */
  virtual bool isCritEnabled() const;

  /**
   * ��¼ CRIT ������־
   * @param[ in ] message ��־����
   * @param[ in ] file ��־��¼��Դ�ļ���
   * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
   */
  virtual void crit(const LogStream& message, const char* file, int line);

  /**
   * fatal������־�Ƿ���Լ���־
   * @return ����true,������false
   */
  virtual bool isFatalEnabled() const;

  /**
   * ��¼fatal������־
   * @param[ in ] message ��־����
   * @param[ in ] file ��־��¼��Դ�ļ���
   * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
   */
  virtual void fatal(const LogStream& message, const char* file, int line);

  /**
   * error������־�Ƿ���Լ���־
   * @return ����true,������false
   */
  virtual bool isErrorEnabled() const;

  /**
   * ��¼error������־
   * @param[ in ] message ��־����
   * @param[ in ] file ��־��¼��Դ�ļ���
   * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
   */
  virtual void error(const LogStream& message, const char* file, int line);

  /**
   * info������־�Ƿ���Լ���־
   * @return ����true,������false
   */
  virtual bool isInfoEnabled() const;

  /**
   * ��¼info������־
   * @param[ in ] message ��־����
   * @param[ in ] file ��־��¼��Դ�ļ���
   * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
   */
  virtual void info(const LogStream& message, const char* file, int line);

  /**
   * debug������־�Ƿ���Լ���־
   * @return ����true,������false
   */
  virtual bool isDebugEnabled() const;

  /**
   * ��¼debug������־
   * @param[ in ] message ��־����
   * @param[ in ] file ��־��¼��Դ�ļ���
   * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
   */
  virtual void debug(const LogStream& message, const char* file, int line);

  /**
   * warn������־�Ƿ���Լ���־
   * @return ����true,������false
   */
  virtual bool isWarnEnabled() const;

  /**
   * ��¼warn������־
   * @param[ in ] message ��־����
   * @param[ in ] file ��־��¼��Դ�ļ���
   * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
   */
  virtual void warn(const LogStream& message, const char* file, int line);

  /**
   * Trace������־�Ƿ���Լ���־
   * @return ����true,������false
   */
  virtual bool isTraceEnabled() const;

  /**
   * ��¼trace������־
   * @param[ in ] message ��־����
   * @param[ in ] file ��־��¼��Դ�ļ���
   * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
   */
  virtual void trace(const LogStream& message, const char* file, int line);

  /**
   * level������־�Ƿ���Լ���־
   * @return ����true,������false
   */
  virtual bool isEnabledFor(const logging::LevelPtr& level) const;

  /**
   * ��¼level������־
   * @param[ in ] message ��־����
   * @param[ in ] file ��־��¼��Դ�ļ���
   * @param[ in ] line ��־��¼��Դ�ļ��ĵ�ǰ��
   */
  virtual void log(const LevelPtr& level, const LogStream& message,
                   const char* file, int line);

  /**
   * ȡ�õ�ǰ����־�ļ���
   * @return ��־�ļ���
   */
  virtual LevelPtr getLevel() const;

private:
  NOCOPY(logger);

  spi::ILogger* impl_;
};

}



#ifndef _NO_LOG_

#ifndef LOG
#define LOG(logger, level, message) { \
  if(logger.isEnabledFor(level)) {\
  LogStream oss; \
  oss << message; \
  logger.fatal(level, oss, __FILE__, __LINE__); }}
#endif // LOG

#ifndef LOG_DEBUG
#define LOG_DEBUG(logger, message) { \
  if(logger.isDebugEnabled()) {\
  LogStream oss; \
  oss << message; \
  logger.debug( oss, __FILE__, __LINE__); }}
#endif // LOG_DEBUG

#ifndef LOG_INFO
#define LOG_INFO(logger, message) { \
  if(logger.isInfoEnabled()) {\
  LogStream oss; \
  oss << message; \
  logger.info( oss, __FILE__, __LINE__); }}
#endif // LOG_INFO

#ifndef LOG_WARN
#define LOG_WARN(logger, message ) { \
  if(logger.isWarnEnabled()) {\
  LogStream oss; \
  oss << message; \
  logger.warn( oss, __FILE__, __LINE__); }}
#endif // LOG_WARN

#ifndef LOG_ERROR
#define LOG_ERROR(logger, message) { \
  if(logger.isErrorEnabled()) {\
  LogStream oss; \
  oss << message; \
  logger.error( oss, __FILE__, __LINE__); }}
#endif // LOG_ERROR

#ifndef LOG_FATAL
#define LOG_FATAL(logger, message) { \
  if(logger.isFatalEnabled()) {\
  LogStream oss; \
  oss << message; \
  logger.fatal( oss, __FILE__, __LINE__); }}
#endif // LOG_FATAL

#ifndef LOG_TRACE
#define LOG_TRACE(logger, message) { \
  if(logger.isTraceEnabled()) {\
  LogStream oss; \
  oss << message; \
  logger.trace( oss, __FILE__, __LINE__); }}
#endif // LOG_TRACE

#ifndef LOG_CRITICAL
#define LOG_CRITICAL(logger, message) { \
  if(logger.isCritEnabled()) {\
  LogStream oss; \
  oss << message; \
  logger.crit( oss, __FILE__, __LINE__); }}
#endif // TP_CRITICAL

#ifndef LOG_ASSERT
#define LOG_ASSERT(logger,assertion) { \
  LogStream oss;                      \
  logger.assertLog( assertion ,oss, __FILE__, __LINE__);   \
  (void)( (!!(assertion)) || (_wassert(_CRT_WIDE(#assertion), _CRT_WIDE(__FILE__), __LINE__), 0) );	\
  }

#endif // LOG_ASSERT

#else // _NO_LOG_

#ifndef BT_NDC
#define BT_NDC( logger, ndc , msg )       {}
#endif // BT_NDC

#ifndef LOG
#define LOG(logger, level, message)     {}
#endif // LOG

#ifndef LOG_DEBUG
#define LOG_DEBUG(logger, message)      {}
#endif // LOG_DEBUG

#ifndef LOG_INFO
#define LOG_INFO(logger, message)     {}
#endif // LOG_INFO

#ifndef LOG_WARN
#define LOG_WARN(logger, message)     {}
#endif // LOG_WARN

#ifndef LOG_ERROR
#define LOG_ERROR(logger, message)      {}
#endif // LOG_ERROR

#ifndef LOG_FATAL
#define LOG_FATAL(logger, message)      {}
#endif // LOG_FATAL

#ifndef LOG_TRACE
#define LOG_TRACE(logger, message)      {}
#endif // LOG_TRACE

#ifndef LOG_ASSERT
#define LOG_ASSERT(logger,assertion )     (void)( (!!(assertion)) || (_wassert(_CRT_WIDE(#assertion), _CRT_WIDE(__FILE__), __LINE__), 0) );
#endif // LOG_ASSERT

#endif // _NO_LOG_

_jingxian_end

#endif // _LOGGING_H_