
#ifndef _Proxy_H_
#define _Proxy_H_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files

class RequestPacket
{
public:
    //+----+----+----+----+----+----+----+----+----+----+...+----+
    //| VN | CD | DSTPORT |      DSTIP        | USERID      |NULL|
    //+----+----+----+----+----+----+----+----+----+----+...+----+
    //   1    1      2              4           variable       1
    //VN      SOCKSЭ��汾�ţ�Ӧ����0x04
    //CD      SOCKS�����ȡ����ֵ:
    //        0x01    CONNECT
    //        0x02    BIND
    //DSTPORT CD��صĶ˿���Ϣ
    //DSTIP   CD��صĵ�ַ��Ϣ
    //USERID  �ͻ�����USERID
    //NULL    0x00

    Encoding _encoding;
    byte[] _IOBuffer;
    int _length = 8 + 1;

    public RequestPacket(Encoding encoding)
            : this(encoding, new byte[1024], 8 + 1)
    {
    }

    public RequestPacket(Encoding encoding, byte[] IOBuffer, int length)
    {
        _encoding = encoding;
        _IOBuffer = IOBuffer;
        _length = length;
    }


    public RequestPacket(Encoding encoding, IOBuffer IOBuffer)
    {
        _encoding = encoding;
        _length = IOBuffer.Count;
        if (0 == IOBuffer.Begin)
        {
            _IOBuffer = IOBuffer.Array;
        }
        else
        {
            _IOBuffer = new byte[_length];
            IOBuffer.CopyTo(_IOBuffer);
        }
    }

    public int Version
    {
        get { return (int)_IOBuffer[0];
            }
        set { _IOBuffer[0] = (byte)value;
            }
    }

    public int Command
    {
        get { return (int)_IOBuffer[1];
            }
        set { _IOBuffer[1] = (byte)value;
            }
    }

    public short Port
    {
        get { return Helper.ReadInt16(_IOBuffer, 2, true);
            }
        set { Helper.Write(_IOBuffer, 2, value, true);
            }
    }

    public bool IsValidIP
    {
        get
        {
            return (_IOBuffer[4] != 0);
        }
    }

    public byte[] IP
    {
        get
        {
            byte[] ip = new byte[4];
            Array.Copy(_IOBuffer, 4, ip, 0, 4);
            return ip;
        }
        set
        {
            if (4 != value.Length)
                throw new ArgumentException("���ȴ���4!", "IP");
            Array.Copy(value, 0, _IOBuffer, 4, 4);
        }
    }

    public IPEndPoint EndPoint
    {
        get { return new IPEndPoint(new IPAddress(IP), Port);
            }
        set
        {
            Port = (short)value.Port;
            IP = value.Address.GetAddressBytes();
        }
    }

    /// <summary>
    /// ����User��Host����null
    /// </summary>
    public string Host
    {
        get
        {
            if (IsValidIP)
                return null;

            for (int i = 8; i < _length; i++)
            {
                if (_IOBuffer[i] != 0)
                    continue;
                return _encoding.GetString(_IOBuffer, i + 1, _length - i - 2);

            }
            return null;
        }
        set
        {
            byte[] ip = new byte[0];
            ip[0] = 0;
            ip[1] = 0;
            ip[2] = 0;
            ip[3] = 1;

            byte[] u = _encoding.GetBytes(value);
            Array.Copy(u, 0, _IOBuffer, _length, u.Length);
            _IOBuffer[_length + u.Length + 1] = 0;
            _length = _length + u.Length + 1;
        }
    }

    /// <summary>
    /// ����User��Host����null
    /// </summary>
    public string User
    {
        get
        {

            int length = _length - 8 - 1;
            if (0 >= length)
                return null;

            byte[] b = new byte[length];
            Array.Copy(_IOBuffer, 8, b, 0, length);
            if (!IsValidIP)
            {
                for (int i = 0; i < b.Length; i++)
                {
                    if (_IOBuffer[i] != 0)
                        continue;

                    if (0 == i)
                        return null;

                    return _encoding.GetString(b);
                }
            }
            return _encoding.GetString(b);
        }
        set
        {
            byte[] u = _encoding.GetBytes(value);
            Array.Copy(u, 0, _IOBuffer, 8, u.Length);
            _IOBuffer[8 + u.Length] = 0;
            _length = 8 + u.Length + 1;
        }
    }

    public IOBuffer ToBuffer()
    {
        return new IOBuffer(_IOBuffer, 0, _length);
    }
};

class ResponsePacket
{
    //+----+----+----+----+----+----+----+----+
    //| VN | CD | DSTPORT |      DSTIP        |
    //+----+----+----+----+----+----+----+----+
    //   1    1      2              4
    //VN      Ӧ��Ϊ0x00������0x04
    //CD      ��ȡ����ֵ:
    //        0x5A    ����ת��
    //        0x5B    �ܾ�ת����һ����ʧ��
    //        0x5C    �ܾ�ת����SOCKS 4 Server�޷����ӵ�SOCS 4 Client����������
    //                IDENT����
    //        0x5D    �ܾ�ת�����������е�USERID��IDENT���񷵻�ֵ�����
    //DSTPORT CD��صĶ˿���Ϣ
    //DSTIP   CD��صĵ�ַ��Ϣ

    byte[] _IOBuffer = new byte[8];

    public ResponsePacket()
    {
    }

    public ResponsePacket(byte[] IOBuffer)
    {
        _IOBuffer = IOBuffer;
    }


    public ResponsePacket(IOBuffer IOBuffer)
    {
        if (0 == IOBuffer.Begin)
        {
            _IOBuffer = IOBuffer.Array;
        }
        else
        {
            _IOBuffer = new byte[8];
            IOBuffer.CopyTo(_IOBuffer);
        }
    }

    public int Version
    {
        get { return (int)_IOBuffer[0];
            }
        set { _IOBuffer[0] = (byte)value;
            }
    }

    public int Error
    {
        get { return (int)_IOBuffer[1];
            }
        set { _IOBuffer[1] = (byte)value;
            }
    }

    public short Port
    {
        get { return Helper.ReadInt16(_IOBuffer, 2, true);
            }
        set { Helper.Write(_IOBuffer, 2, value, false);
            }
    }

    public byte[] IP
    {
        get
        {
            byte[] ip = new byte[4];
            Array.Copy(_IOBuffer, 4, ip, 0, 4);
            return ip;
        }
        set
        {
            if (4 != value.Length)
                throw new ArgumentException("���ȴ���4!", "IP");
            Array.Copy(value, 0, _IOBuffer, 4, 4);
        }
    }

    public IPEndPoint EndPoint
    {
        get { return new IPEndPoint(new IPAddress(IP), Port);
            }
        set
        {
            Port = (short)value.Port;
            IP = value.Address.GetAddressBytes();
        }
    }

    public string GetErrorMessage()
    {
        switch (Error)
        {
        case 0x5A:
            return "����ת��";
        case 0x5B:
            return "�ܾ�ת����һ����ʧ��";
        case 0x5C:
            return "�ܾ�ת����SOCKS 4 Server�޷����ӵ�SOCS 4 Client����������IDENT����";
        case 0x5D:
            return "�ܾ�ת�����������е�USERID��IDENT���񷵻�ֵ�����";
        }
        return "δ֪����";
    }

    public IOBuffer ToBuffer()
    {
        return new IOBuffer(_IOBuffer, 0, 8);
    }
};

_jingxian_end

#endif // _Proxy_H_