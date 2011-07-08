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

#include "ETHTempEntityHandler.h"
#include "ETHRenderEntity.h"

ETHTempEntityHandler::ETHTempEntityHandler(ETHResourceProviderPtr provider) :
	m_provider(provider)
{
}

ETHTempEntityHandler::~ETHTempEntityHandler()
{
	for (ETHEntityList::iterator iter = m_dynamicOrTempEntities.begin(); iter != m_dynamicOrTempEntities.end(); iter++)
	{
		(*iter)->Release();
	}
	ClearCallbackEntities();
}

bool ETHTempEntityHandler::IsCallbackListEmpty() const
{
	return m_lastFrameCallbacks.empty();
}

void ETHTempEntityHandler::ClearCallbackEntities()
{
	for (std::list<ETHRenderEntity*>::iterator iter = m_lastFrameCallbacks.begin();
		iter != m_lastFrameCallbacks.end(); iter++)
	{
		(*iter)->Release();
	}
	m_lastFrameCallbacks.clear();
}

bool ETHTempEntityHandler::AddEntityWhenEligible(ETHRenderEntity* entity)
{
	if (IsTempEntityEligible(entity))
	{
		entity->AddRef();
		m_dynamicOrTempEntities.push_back(entity);
		return true;
	}
	else
	{
		return false;
	}
}

bool ETHTempEntityHandler::IsTempEntityEligible(ETHRenderEntity* entity) const
{
	return ((entity->HasCallback() && !entity->IsStatic()) || entity->IsTemporary());
}

bool ETHTempEntityHandler::AddCallbackWhenEligible(ETHRenderEntity* entity)
{
	if (IsCallbackEligible(entity))
	{
		m_lastFrameCallbacks.push_back(entity);
		entity->AddRef();
		return true;
	}
	else
	{
		return false;
	}
}

bool ETHTempEntityHandler::IsCallbackEligible(ETHRenderEntity* entity) const
{
	return (!IsTempEntityEligible(entity));
}

void ETHTempEntityHandler::TestEntityLists() const
{
	for (std::list<ETHRenderEntity*>::const_iterator a = m_dynamicOrTempEntities.begin(); a != m_dynamicOrTempEntities.end(); a++)
	{
		for (std::list<ETHRenderEntity*>::const_iterator b = m_lastFrameCallbacks.begin(); b != m_lastFrameCallbacks.end(); b++)
		{
			assert((*a)->GetID() != (*b)->GetID());
		}
	}
}

void ETHTempEntityHandler::CheckTemporaryEntities(const Vector2& zAxisDir, ETHBucketManager& buckets)
{
	#ifdef _DEBUG
	TestEntityLists();
	#endif

	for (std::list<ETHRenderEntity*>::iterator iter = m_dynamicOrTempEntities.begin(); iter != m_dynamicOrTempEntities.end();)
	{
		ETHRenderEntity* pRenderEntity = *iter;

		pRenderEntity->UpdateParticleSystems(zAxisDir);

		// if the particle system is finished, erase it
		if ((!pRenderEntity->GetSprite() && pRenderEntity->AreParticlesOver()))
		{
			const Vector2 v2Bucket = ETHGlobal::GetBucket(pRenderEntity->GetPositionXY(), buckets.GetBucketSize());
			ETHBucketMap::iterator bucketIter = buckets.Find(v2Bucket);

			if (bucketIter == buckets.GetLastBucket())
			{
				iter++;
				continue;
			}

			ETHEntityList::const_iterator iEnd = bucketIter->second.end();
			for (ETHEntityList::iterator entityIter = bucketIter->second.begin(); entityIter != iEnd; entityIter++)
			{
				if ((*entityIter)->GetID() != pRenderEntity->GetID())
					continue;

				#ifdef _DEBUG
				ETH_STREAM_DECL(ss) << GS_L("Entity ") << pRenderEntity->GetEntityName() << GS_L(" (ID#") << pRenderEntity->GetID() << GS_L(") removed (particle effects over)");
				m_provider->Log(ss.str(), Platform::Logger::INFO);
				#endif

				pRenderEntity->SetStopSFXWhenDestroyed(false);
				pRenderEntity->Kill();
				pRenderEntity->Release();
				entityIter = bucketIter->second.erase(entityIter);
				break;
			}

			#ifdef _DEBUG
			ETH_STREAM_DECL(ss) << GS_L("Entity ") << (*iter)->GetEntityName() << GS_L(" (ID#") << pRenderEntity->GetID() << GS_L(") removed from dynamic entity list (particle effects over)");
			m_provider->Log(ss.str(), Platform::Logger::INFO);
			#endif
			(*iter)->Release();
			iter = m_dynamicOrTempEntities.erase(iter);
			continue;
		}
		iter++;
	}
}

void ETHTempEntityHandler::RunCallbacksFromList()
{
	for (std::list<ETHRenderEntity*>::iterator iter = m_dynamicOrTempEntities.begin(); iter != m_dynamicOrTempEntities.end();)
	{
		if (!((*iter)->IsAlive()))
		{
			#ifdef _DEBUG
			ETH_STREAM_DECL(ss) << GS_L("Entity removed from dynamic entity list: ") << (*iter)->GetEntityName();
			m_provider->Log(ss.str(), Platform::Logger::INFO);
			#endif
			(*iter)->Release();
			iter = m_dynamicOrTempEntities.erase(iter);
			continue;
		}
		else
		{
			if ((*iter)->HasCallback())
			{
				(*iter)->RunCallbackScript();
			}
			iter++;
		}
	}

	for (std::list<ETHRenderEntity*>::iterator iter = m_lastFrameCallbacks.begin(); iter != m_lastFrameCallbacks.end();)
	{
		if (!((*iter)->IsAlive()))
		{
			#ifdef _DEBUG
			ETH_STREAM_DECL(ss) << GS_L("Entity callback removed: ") << (*iter)->GetEntityName();
			m_provider->Log(ss.str(), Platform::Logger::INFO);
			#endif
			(*iter)->Release();
			iter = m_lastFrameCallbacks.erase(iter);
			continue;
		}
		else
		{
			if ((*iter)->HasCallback())
			{
				(*iter)->RunCallbackScript();
			}
			(*iter)->Release();
			iter++;
		}
	}
	m_lastFrameCallbacks.clear();
}
