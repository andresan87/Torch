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

#ifndef ETH_TEMP_ENTITY_HANDLER_H_
#define ETH_TEMP_ENTITY_HANDLER_H_

#include "ETHBucketManager.h"

class ETHTempEntityHandler
{
public:
	ETHTempEntityHandler(ETHResourceProviderPtr provider);
	~ETHTempEntityHandler();

	bool AddEntityWhenEligible(ETHRenderEntity* entity);
	bool IsTempEntityEligible(ETHRenderEntity* entity) const;

	void CheckTemporaryEntities(const Vector2& zAxisDir, ETHBucketManager& buckets);

	bool AddCallbackWhenEligible(ETHRenderEntity* entity);
	bool IsCallbackEligible(ETHRenderEntity* entity) const;

	void RunCallbacksFromList();

	bool IsCallbackListEmpty() const;

private:
	void ClearCallbackEntities();
	void TestEntityLists() const;

	ETHResourceProviderPtr m_provider;

	/*
	 * This list will hold every temporary or dynamic entity with callbacks.
	 * It must be kept separetely because these entities will be updated at every,
	 * frame even if they're culled out by the spatial hashing system.
	 */
	std::list<ETHRenderEntity*> m_dynamicOrTempEntities;

	/*
	 * This list holds every callback script function that must be called per
	 * frame. So we don't have to call them during the rendering iteration
	 */
	std::list<ETHRenderEntity*> m_lastFrameCallbacks;
};

#endif