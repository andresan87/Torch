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

#include "ETHScriptEntity.h"

ETHScriptEntity::ETHScriptEntity() : 
	m_ref(1),
	m_isAlive(true)
{
}

void ETHScriptEntity::Kill()
{
	if (!IsTemporary())
	{
		ForceSFXStop();
	}
	m_isAlive = false;
}

bool ETHScriptEntity::IsAlive() const
{
	return m_isAlive;
}
