
#ifndef _MY_Exception_H_
#define _MY_Exception_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
#include <stdexcept>
#include "jingxian/string/os_string.h"
#include "jingxian/lastError.h"
#include "jingxian/utilities/StackTracer.h"

_jingxian_begin

#ifndef _RAISE
#define _RAISE(x)  throw (x)
#endif

class Exception : public std::runtime_error
{
public:
    virtual ~Exception() throw()
    {
    }

    const char* getFile() const
    {
        if (!fSrcFile)
            return "";
        return fSrcFile;
    }

    size_t getLine() const
    {
        return fSrcLine;
    }

    void setPosition(const char* const file
                     , size_t line)
    {
        fSrcFile = file;
        fSrcLine = line;
    }

    Exception()
            : std::runtime_error("<δ֪�쳣>")
            , fSrcFile(0)
            , fSrcLine(0)
    {
        initStackTrace(3);
    }

    Exception(const tstring& message)
            : std::runtime_error(toNarrowString(message))
            , fSrcFile(0)
            , fSrcLine(0)
    {
        initStackTrace(3);
    }

    Exception(const char* const srcFile
              , size_t srcLine
              , const tstring& message)
            : std::runtime_error(toNarrowString(message))
            , fSrcFile(srcFile)
            , fSrcLine(srcLine)
    {
        initStackTrace(3);
    }

    Exception(const char* const srcFile
              , size_t srcLine
              , const tstring& message
              , const Exception& e)
            : std::runtime_error(toNarrowString(message) + "," + e.what())
            , fSrcFile(srcFile)
            , fSrcLine(srcLine)
    {
        initStackTrace(3);
    }

    template< typename E >
    void Raise(E& e)
    {
        throw e;
    }

    void dump(tostream& target) const
    {
        target << _T("[ file:")
        << toTstring(getFile())
        << _T(" line:")
        << (int) getLine()
        << _T(" ] ")
        << std::endl
        << _stack;
    }

    virtual Exception* clone()
    {
        return new Exception(*this);
    }

    virtual void rethrow()
    {
        Raise(*this);
    }

    virtual void print(tostream& target) const
    {
        target << _T("Exception: ")
        << what();
        dump(target);

    }

#if !_HAS_EXCEPTIONS
protected:
    virtual void _Doraise() const
    {   // perform class-specific exception handling
        _RAISE(*this);
    }
#endif /* _HAS_EXCEPTIONS */

protected :

    Exception(const Exception& ex)
            : std::runtime_error(ex)
            , fSrcFile(ex.fSrcFile)
            , fSrcLine(ex.fSrcLine)
            , _stack(ex._stack)
    {
    }

    void initStackTrace(int skipFrames)
    {
        StackTracer stackWalker(StackTracer::RetrieveLine);
        stackWalker.ShowCallstack(skipFrames);
        _stack = toTstring(stackWalker.GetCallStack());
    }

    const char*     fSrcFile;
    size_t    fSrcLine;
    tstring   _stack;
};

inline tostream& operator<<(tostream& target, const Exception& err)
{
    err.print(target);
    return target;
}


#define MakeException(theType , msg )                       \
class theType : public Exception                            \
{                                                           \
public:                                                     \
                                                            \
    theType(const   char* const         srcFile             \
            , size_t        srcLine                         \
            , const tstring & m                             \
            , const Exception& e                            \
            ) :                                             \
        Exception(srcFile, srcLine, msg + m ,e )            \
    {                                                       \
                                                            \
    }                                                       \
    theType(const   char* const         srcFile             \
            , size_t        srcLine                         \
            , const tstring & m                             \
            ) :                                             \
        Exception(srcFile, srcLine, msg + m )               \
    {                                                       \
                                                            \
    }                                                       \
    theType(const   char* const         srcFile             \
            , size_t        srcLine                         \
            ) :                                             \
        Exception(srcFile, srcLine, msg )                   \
    {                                                       \
                                                            \
    }                                                       \
    theType( ) :                                            \
        Exception( msg )                                    \
    {                                                       \
                                                            \
    }                                                       \
    theType( const tstring & m  ) :                         \
        Exception( msg + m )                                \
    {                                                       \
                                                            \
    }                                                       \
                                                            \
    virtual ~theType() throw() {}                           \
    virtual Exception* clone()                              \
    {                                                       \
        return new theType( *this );                        \
    }                                                       \
    virtual void rethrow()                                  \
    {                                                       \
        Raise( *this );                                     \
    }                                                       \
    virtual void print(tostream& target) const              \
    {                                                       \
            target << MAKE_STRING( theType )                \
      << what();                    \
      dump( target );                 \
    }                                                       \
};


// ��������
#define IllegalArgumentError    _T("��Ч����")
#define ArgumentNullError       _T("����Ϊ��")
#define RuntimeError            _T("����ʱ����")
#define OutOfRangeError         _T("������Χ")
#define LockError               _T("������")
#define CtrlCHandlerError       _T("����Ctrl+C����")
#define NetError                _T("�������")
#define InvalidPointerError     _T("��Чָ��")
#define LengthError             _T("��Ч����")
#define EOFError                _T("�Ѿ�����β��")
#define PluginError             _T("�������")
#define URLError                _T("URL��������")
#define CastError               _T("ת��ʧ��")
#define NullError               _T("��ָ��")
#define NotFindError            _T("û���ҵ�")
#define _ExistError             _T("�Ѿ�����")
#define SystemError             _T("ϵͳ����")
#define TimeSyntaxError         _T("ʱ���ʽ����")
#define NotImplementedError     _T("û��ʵ��")


// �������
#define  ERR_BAD_BUF         -2
#define  ERR_BAD_BUF_LEN     -1
#define  ERR_SYS             -200 //  
#define  ERR_ARG             -201
#define  ERR_LEN             -202
#define  ERR_POINT           -203
#define  ERR_UNKOWN          -204
#define  ERR_MAXMSGLEN       -205
#define  ERR_HEADERLENGTH    -206
#define  ERR_HEADERTYPE      -207
#define  ERR_MEMORY          -208
#define  ERR_PARAMETER       -400
#define  ERR_OBJECT_NOEXIST  -501    //  ���󲻴���
#define  ERR_INTERNAL        -502    //  �ڲ�����
#define  ERR_UNKNOWN_COMMAND -503    //  ����ʶ�������
#define  ERR_AUTH            -504    //  û��Ȩ��
#define  ERR_TYPE            -505    //  ���ʹ�������һ��Ŀ¼
#define  ERR_SEEKFILE        -506    //  �ƶ��ļ���λ�ó���
#define  ERR_READFILE        -507    //  �ƶ��ļ���λ�ó���
#define  ERR_LENGTH          -508    //  �ڴ�̫С


// �����쳣��
MakeException(NullException , NullError);
MakeException(RuntimeException , RuntimeError);
MakeException(IllegalArgumentException, IllegalArgumentError);
MakeException(ArgumentNullException, ArgumentNullError);
MakeException(OutOfRangeException, OutOfRangeError);
MakeException(InvalidPointerException, InvalidPointerError);
MakeException(LengthException, LengthError);
MakeException(CastException, CastError);
MakeException(EOFException , EOFError);
MakeException(URLException, URLError);
MakeException(NotFindException, NotFindError);
MakeException(SystemException, SystemError);
MakeException(LockException , LockError);
MakeException(TimeSyntaxException , TimeSyntaxError);
MakeException(NotImplementedException, NotImplementedError);

// �쳣��
#define ThrowException( type ) throw type(__FILE__, __LINE__ )
#define ThrowException1( type ,m1 ) throw type(__FILE__, __LINE__ , m1 )
#define ThrowException2( type ,m1 ,m2 ) throw type(__FILE__, __LINE__ , m1, m2 )
#define ThrowException3( type ,m1 ,m2 ,m3 ) throw type(__FILE__, __LINE__ , m1, m2, m3 )
#define ThrowException4( type ,m1 ,m2 ,m3 ,m4) throw type(__FILE__, __LINE__ , m1, m2, m3, m4)

_jingxian_end

#endif //_MY_Exception_H_
