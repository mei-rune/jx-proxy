
/**
 * -*- C++ -*-
 * -------------------------------------------------------------------------------
 * - ¤╭⌒╮ ╭⌒╮                   serialize.h,v 1.0 2005/03/29 15:41:34
 *  ╱◥██◣ ╭╭ ⌒╮
 * ︱田︱田田| ╰------
 * -------------------------------------------------------------------------------
 */


#ifndef _serialize_hpp_
#define _serialize_hpp_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
#include <list>
#include <map>
#include <vector>
#include <set>
#include "jingxian/serialize/reader.hpp"
#include "jingxian/serialize/writer.hpp"

_jingxian_begin

class serialize_object
{
public:
    virtual ~serialize_object() {}
    virtual bool write(serialize_writer& stream, serialize_context& context, const tchar* name = _T("serialize_object")) = 0;
    virtual bool read(serialize_reader& stream, serialize_context& context, const tchar* name = _T("serialize_object")) = 0;
};

bool inline serialize(serialize_writer& stream, serialize_context& context, bool t, const tchar* name = 0)
{
    return stream.write(context, t, name);
}

bool inline serialize(serialize_writer& stream, serialize_context& context, char t, const tchar* name = 0)
{
    return stream.write(context, (int8_t)t, name);
}

bool inline serialize(serialize_writer& stream, serialize_context& context, short t, const tchar* name = 0)
{
    return stream.write(context, (int16_t)t, name);
}

bool inline serialize(serialize_writer& stream, serialize_context& context, int t, const tchar* name = 0)
{
    return stream.write(context, t, name);
}

bool inline serialize(serialize_writer& stream, serialize_context& context, long t, const tchar* name = 0)
{
    return stream.write(context, (int32_t)t, name);
}

bool inline serialize(serialize_writer& stream, serialize_context& context, __int64 t, const tchar* name = 0)
{
    return stream.write(context, (int64_t)t, name);
}

bool inline serialize(serialize_writer& stream, serialize_context& context, unsigned char t, const tchar* name = 0)
{
    return stream.write(context, (int8_t)t, name);
}

bool inline serialize(serialize_writer& stream, serialize_context& context, unsigned short t, const tchar* name = 0)
{
    return stream.write(context, (int16_t)t, name);
}

bool inline serialize(serialize_writer& stream, serialize_context& context, unsigned int t, const tchar* name = 0)
{
    return stream.write(context, (int32_t)t, name);
}

bool inline serialize(serialize_writer& stream, serialize_context& context, unsigned long t, const tchar* name = 0)
{
    return stream.write(context, (int32_t)t, name);
}

bool inline serialize(serialize_writer& stream, serialize_context& context, unsigned __int64 t, const tchar* name = 0)
{
    return stream.write(context, (int64_t)t, name);
}

template < class _Elem,
class _Traits,
class _Ax >
bool inline serialize(serialize_writer& stream, serialize_context& context, const std::basic_string<_Elem, _Traits, _Ax>& t, const tchar* name = 0)
{
    serialize_context::string_guard guard(context);
    size_t len = (t.size() + 1) * sizeof(std::basic_string<_Elem, _Traits, _Ax>::value_type);
    return stream.write(context, (int32_t)(len) , name)
           && stream.write(context, t.c_str(), len , name);
}

template< typename T >
bool inline serialize(serialize_writer& stream, serialize_context& context, const T& s1, const tchar* name = 0)
{
    return s1.write(stream, context, name);
}

//反序列化基础函数

bool inline deserialize(serialize_writer& stream, serialize_context& context, bool& t, const tchar* name = 0)
{
    return stream.read(context, t, name);
}

template< typename T, typename P>
inline bool ___deserialize_object(serialize_writer& stream, serialize_context& context, P& t, const tchar* name = 0)
{
    T value;
    if (stream.read(context, value, name))
    {
        t = (P)value;
        return true;
    }
    return false;
}

bool inline deserialize(serialize_reader& stream, serialize_context& context, char& t, const tchar* name = 0)
{
    return ___deserialize_object< int8_t, char>(stream, context, t, name);
}

bool inline deserialize(serialize_reader& stream, serialize_context& context, short& t, const tchar* name = 0)
{
    return ___deserialize_object< int16_t, short>(stream, context, t, name);
}

bool inline deserialize(serialize_reader& stream, serialize_context& context, int& t, const tchar* name = 0)
{
    return stream.read(t, name);
}

bool inline deserialize(serialize_reader& stream, serialize_context& context, long& t, const tchar* name = 0)
{
    return ___deserialize_object< int32_t, long>(stream, context, t, name);
}

bool inline deserialize(serialize_reader& stream, serialize_context& context, __int64& t, const tchar* name = 0)
{
    return stream.read(context, t, name);
}

bool inline deserialize(serialize_reader& stream, serialize_context& context, unsigned char& t, const tchar* name = 0)
{
    return ___deserialize_object< int8_t, unsigned char>(stream, context, t, name);
}

bool inline deserialize(serialize_reader& stream, serialize_context& context, unsigned short& t, const tchar* name = 0)
{
    return ___deserialize_object< int16_t, unsigned short>(stream, context, t, name);
}

bool inline deserialize(serialize_reader& stream, serialize_context& context, unsigned int& t, const tchar* name = 0)
{
    return ___deserialize_object< int32_t, unsigned int>(t, stream, name);
}

bool inline deserialize(serialize_reader& stream, serialize_context& context, unsigned long& t, const tchar* name = 0)
{
    return ___deserialize_object< int32_t, unsigned long>(t, stream, name);
}

bool inline deserialize(serialize_reader& stream, serialize_context& context, unsigned __int64& t, const tchar* name = 0)
{
    return ___deserialize_object<int64_t, unsigned __int64>(t, stream, name);
}

template < class _Elem,
class _Traits,
class _Ax >
bool inline deserialize(serialize_writer& stream, serialize_context& context, std::basic_string<_Elem, _Traits, _Ax>& t, const tchar* name = 0)
{
    //typedef std::basic_string<_Elem,_Traits,_Ax> string_type;

    serialize_context::string_guard guard(context);

    int value;
    if (!stream.read(context, value, name))
        return false;

    t.resize(value);
    size_t len = 0;
    if (!stream.read(context, (void*)t.c_str(), len, name))
        return false;

    t.resize(len - 1);
    return true;
}



template< typename T >
bool inline deserialize(serialize_reader& stream, serialize_context& context, T& s1, const tchar* name = 0)
{
    return s1.read(stream, context, name);
}

template< typename T>
bool inline serialize(serialize_writer& stream, serialize_context& context, const std::auto_ptr<T>& ptr_, const tchar* name =  0)
{
    return serialize(stream, context, *ptr_, name);
}

template< typename T1, typename T2>
bool inline serialize(serialize_writer& stream, serialize_context& context, const std::pair<T1, T2>& pair_, const tchar* name =  0)
{
    return serialize(pair_.second, stream, name);
}

template< typename _Init>
bool inline serialize(serialize_writer& stream, serialize_context& context, _Init _F, _Init _E, const size_t& size = -1, const tchar * name = 0)
{
    size_t sz = size;
    if (-1 == sz)
        sz  = std::distance(_F, _E);

    bool ret = false;
    if (ret = stream.open(name, sz))
    {
        for (; _F != _E && (ret = serialize(*_F, stream)); _F++);
        if (ret) ret = stream.close(name);
    }
    return ret;
}


template< typename T1, typename T2 >
bool inline serialize(serialize_writer& stream, serialize_context& context, const std::list< T1, T2 >& list_ , const tchar* name =  0)
{
    return serialize(list_.begin(), list_.end(), stream, list_.size(),  name);
}
template< typename T1, typename T2 >
bool inline serialize(serialize_writer& stream, serialize_context& context, const std::vector< T1, T2 >& vector_ , const tchar* name = 0)
{
    return serialize(vector_.begin(), vector_.end(), stream, vector_.size(), name);
}
template< typename T1, typename T2, typename T3 >
bool inline serialize(serialize_writer& stream, serialize_context& context, const std::set< T1, T2, T3 >& set_ , const tchar* name = 0)
{
    return serialize(set_.begin(), set_.end(), stream, set_.size(), name);
}

template< typename T1, typename T2, typename T3, typename T4 >
bool inline serialize(serialize_writer& stream, serialize_context& context, const std::map< T1, T2, T3, T4 >& map_ , const tchar* name = 0)
{
    return serialize(map_.begin(), map_.end(), stream, map_.size(), name);
}
template< typename T1, typename T2, typename T3, typename T4 >
bool inline serialize(serialize_writer& stream, serialize_context& context, const std::multimap< T1, T2, T3, T4 >& map_ , const tchar* name = 0)
{
    return serialize(map_.begin(), map_.end(), stream, map_.size(), name);
}

template< typename T>
bool inline deserialize(serialize_writer& stream, serialize_context& context, std::auto_ptr<T>& ptr_, const tchar* name =  0)
{
    if (ptr.get() == 0)
        ptr_.reset(new T());
    return deserialize(*ptr_, stream, name);
}

template< typename T1, typename T2 >
bool inline deserialize(serialize_writer& stream, serialize_context& context, std::list< T1, T2 >& list_ , const tchar* name =  0)
{
    bool ret = false;
    size_t size = -1;


    if (ret = stream.open(name, size))
    {
        list_.clear();

        while (ret && (size == -1 || list_.size() < size))
        {
            std::list< T1, T2 >::value_type val;//std::list< T1,T2 >::iterator it = list_.insert(list.end(), std::list< T1,T2 >::value_type());
            if (ret = deserialize(val, stream))
                list_.push_back(val);

        };

        //可能某些流如xml等不支持size,
        //如果 支持size：必须读取相应size个且能close,才为成功
        //如果不支持size：必须要能close为成功

        ret = (size == -1 || list_.size() == size) && stream.close(name)  ;
    }
    return ret;
}
template< typename T1, typename T2 >
bool inline deserialize(serialize_writer& stream, serialize_context& context, std::vector< T1, T2 >& vector_ , const tchar* name = 0)
{
    bool ret = false;
    size_t size = -1;
    if (ret = stream.open(name, size))
    {
        vector_.clear();
        if (size != -1) vector_.reserve(size);
        while (ret && (size == -1 || vector_.size() < size))
        {
            std::vector< T1, T2 >::value_type val;//std::vector< T1,T2 >::iterator it = vector_.insert(vector_.end(), std::vector< T1,T2 >::value_type());
            if (ret = deserialize(val, stream))
                vector_.push_back(val);
        };

        //可能某些流如xml等不支持size,
        //如果 支持size：必须读取相应size个且能close,才为成功
        //如果不支持size：必须要能close为成功

        ret = (size == -1 || vector_.size() == size) && stream.close(name)  ;

    }
    return ret;
}
template< typename T1, typename T2, typename T3 >
bool inline deserialize(serialize_writer& stream, serialize_context& context, std::set< T1, T2, T3 >& set_ , const tchar* name = 0)
{
    bool ret = false;
    size_t size = -1;
    if (ret = stream.open(name, size))
    {
        set_.clear();
        while (ret && (size == -1 || set_.size() < size))
        {
            std::set< T1, T2, T3 >::value_type val;
            if (ret = deserialize(val, stream))
                set_.insert(val);
        };


        //可能某些流如xml等不支持size,
        //如果 支持size：必须读取相应size个且能close,才为成功
        //如果不支持size：必须要能close为成功

        ret = (size == -1 || set_.size() == size) && stream.close(name)  ;
    }
    return ret;
    //return serialize(set_.begin(), set_.end(), stream, set_.size(), name );
}

template< typename T1, typename T2, typename T3, typename T4 >
bool inline deserialize(serialize_writer& stream, serialize_context& context, std::map< T1, T2, T3, T4 >& map_ , const tchar* name = 0)
{
    bool ret = false;
    size_t size = -1;
    if (ret = stream.open(name, size))
    {
        map_.clear();
        while (ret && (size == -1 || map_.size() < size))
        {
            std::map< T1, T2, T3, T4 >::key_type  key;
            std::map< T1, T2, T3, T4 >::referent_type  val;
            if (ret = (deserialize(val, stream) && deserialize_getkey(key, (const std::map< T1, T2, T3, T4 >::referent_type)val)))
                map_.insert(std::map< T1, T2, T3, T4 >::value_type(key, val));
        };

        //可能某些流如xml等不支持size,
        //如果 支持size：必须读取相应size个且能close,才为成功
        //如果不支持size：必须要能close为成功

        ret = (size == -1 || map_.size() == size) && stream.close(name)  ;
    }
    return ret;
    //return serialize(map_.begin(), map_.end(), stream, map_.size(), name );
}
template< typename T1, typename T2, typename T3, typename T4 >
bool inline deserialize(serialize_writer& stream, serialize_context& context, std::multimap< T1, T2, T3, T4 >& map_ , const tchar* name = 0)
{
    bool ret = false;
    size_t size = -1;
    if (ret = stream.open(name, size))
    {
        map_.clear();
        while (ret && (size == -1 || map_.size() < size))
        {
            std::multimap< T1, T2, T3, T4 >::key_type  key;
            std::multimap< T1, T2, T3, T4 >::referent_type  val;
            if (ret = (deserialize(val, stream) && deserialize_getkey(key, (const std::multimap< T1, T2, T3, T4 >::referent_type)val)))
                map_.insert(std::map< T1, T2, T3, T4 >::value_type(key, val));
        };

        //可能某些流如xml等不支持size,
        //如果 支持size：必须读取相应size个且能close,才为成功
        //如果不支持size：必须要能close为成功

        ret = (size == -1 || map_.size() == size) && stream.close(name)  ;
    }
    return ret;
}


#include "jingxian/serialize/serialize_macro.h"

_jingxian_end

#endif // _serialize_hpp_
