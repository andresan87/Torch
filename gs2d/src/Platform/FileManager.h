/*-----------------------------------------------------------------------

 gameSpace2d (C) Copyright 2008-2011 Andre Santee
 http://www.asantee.net/gamespace/
 http://groups.google.com/group/gamespacelib

    This file is part of gameSpace2d.

    gameSpace2d is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    gameSpace2d is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with gameSpace2d. If not, see
    <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------*/

#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include "../gs2dtypes.h"

namespace Platform {

template <class T> class _FileBuffer
{
public:
    _FileBuffer(const int size);
    ~_FileBuffer();
    int GetBufferSize();
    T *GetAddress();
private:
    T *m_buffer;
    int m_bufferSize;
};
typedef boost::shared_ptr<_FileBuffer<unsigned char> > FileBuffer;

template <class T>
_FileBuffer<T>::_FileBuffer(const int size)
{
    assert(sizeof(T) == 1);
    m_buffer = NULL;
    m_bufferSize = size;
    if (size > 0)
        m_buffer = new T [size];
}

template <class T>
_FileBuffer<T>::~_FileBuffer()
{
    if (m_buffer)
    {
        delete [] m_buffer;
    }
}

template <class T>
int _FileBuffer<T>::GetBufferSize()
{
    return m_bufferSize;
}

template <class T>
T *_FileBuffer<T>::GetAddress()
{
    return m_buffer;
}

class FileManager
{
public:
    virtual bool IsLoaded() const = 0;
    virtual bool GetFileBuffer(const gs2d::str_type::string& fileName, FileBuffer &out) = 0;
    virtual bool GetAnsiFileString(const gs2d::str_type::string& fileName, gs2d::str_type::string &out) = 0;
    virtual bool GetUTF8BOMFileString(const gs2d::str_type::string& fileName, gs2d::str_type::string &out) = 0;
    virtual bool GetUTF16FileString(const gs2d::str_type::string& fileName, gs2d::str_type::string &out) = 0;
};
typedef boost::shared_ptr<FileManager> FileManagerPtr;

}

#endif