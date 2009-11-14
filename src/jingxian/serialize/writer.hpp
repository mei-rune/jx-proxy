
#ifndef _writer_hpp_
#define _writer_hpp_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files

_jingxian_begin

class serialize_writer
{
public:
	virtual ~serialize_writer(){};

	/**
	 * ��ʼ�����л�һ���ṹ
	 * @param[ in ] className �ṹ����
	 * @param[ in ] field �ֶ���
	 */
	virtual bool open(serialize_context& context, const tchar* className, const tchar* field) = 0;

	/**
	 * ���л�һ���ṹ����
	 * @param[ in ] className �ṹ����
	 */
	virtual bool close(serialize_context& context) = 0;

	/**
	 * ���л�һ��bool���ֶ�
	 * @param[ in ] t Ҫ���л����ֶ�
	 * @param[ in ] field_name �ֶ�����
	 */
	virtual bool write(serialize_context& context,  bool t, const tchar* field_name) = 0;

	/**
	 * ���л�һ��int8���ֶ�
	 * @param[ in ] t Ҫ���л����ֶ�
	 * @param[ in ] field_name �ֶ�����
	 */
	virtual bool write(serialize_context& context,  int8_t t, const tchar* field_name) = 0;

	/**
	 * ���л�һ��int16���ֶ�
	 * @param[ in ] t Ҫ���л����ֶ�
	 * @param[ in ] field_name �ֶ�����
	 */
	virtual bool write(serialize_context& context,  int16_t t, const tchar* field_name) = 0;

	/**
	 * ���л�һ��int32���ֶ�
	 * @param[ in ] t Ҫ���л����ֶ�
	 * @param[ in ] field_name �ֶ�����
	 */
	virtual bool write(serialize_context& context,  int32_t t, const tchar* field_name) = 0;

	/**
	 * ���л�һ��int64���ֶ�
	 * @param[ in ] t Ҫ���л����ֶ�
	 * @param[ in ] field_name �ֶ�����
	 */
	virtual bool write(serialize_context& context,  int64_t t, const tchar* field_name) = 0;

	/**
	 * ���л�һ��bool�ֶ�
	 * @param[ out ] blob Ҫ���л������ݿ�
	 * @param[ in ] len Ҫ���л������ݿ�ĳ���
	 * @param[ in ] field_name �ֶ�����
	 */
	virtual bool write(serialize_context& context, const void* blob, size_t len, const tchar* field_name = _T("blob") ) = 0;

	/**
	 * ���Ƿ�����
	 */
	virtual bool is_good() const = 0;

	/**
	 * ȡ�����һ�δ�������
	 */
	virtual const tstring& last_error() const = 0;

	/**
	 * �������һ�δ�������
	 */
	virtual void last_error( const tstring& err ) = 0;
};

_jingxian_end

#endif // _writer_hpp_