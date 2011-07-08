/*-----------------------------------------------------------------------

 Ethanon Engine (C) Copyright 2009-2011 Andre Santee
 http://www.asantee.net/ethanon/

    This file is part of Ethanon Engine.

    Ethanon Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    Ethanon Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Ethanon Engine. If not, see
    <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------*/

#ifndef ETH_ENTITY_ARRAY_H_
#define ETH_ENTITY_ARRAY_H_

#include "ETHEntity.h"
#include <vector>

class ETHEntityArray
{
public:
	ETHEntityArray();
	~ETHEntityArray();

	void AddRef();
	void Release();

	ETHEntityArray &operator=(const ETHEntityArray &other);
	void push_back(ETHEntityRawPtr p);
	unsigned int size() const;
	ETHEntityRawPtr &operator[] (const unsigned int index);
	const ETHEntityRawPtr &operator[] (const unsigned int index) const;
	void clear();
	ETHEntityArray &operator+=(const ETHEntityArray &other);
	void unique();
	void removeDeadEntities();

private:
	std::vector<ETHEntityRawPtr> m_vector;
	int m_ref;
};

ETHEntityArray *ETHEntityArrayFactory();

#endif
