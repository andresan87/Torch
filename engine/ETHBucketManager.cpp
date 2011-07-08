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

#include "ETHBucketManager.h"
#include "ETHEntityArray.h"
#include "ETHRenderEntity.h"
#include <iostream>

// Vector2 hash function
namespace boost {
inline std::size_t hash_value(Vector2 const& p)
{
	std::size_t seed = 0;
	boost::hash_combine(seed, p.x);
	boost::hash_combine(seed, p.y);
	return seed;
}
} // namepace boost

ETHBucketManager::ETHBucketManager(const ETHResourceProviderPtr& provider, const Vector2& bucketSize, const bool drawingBorderBuckets) :
	m_bucketSize(bucketSize),
	m_provider(provider),
	m_drawingBorderBuckets(drawingBorderBuckets)
{
}

ETHBucketManager::~ETHBucketManager()
{
	for (ETHBucketMap::iterator bucketIter = GetFirstBucket(); bucketIter != GetLastBucket(); bucketIter++)
	{
		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			(*iter)->Kill();
			(*iter)->Release();
		}
	}
}

ETHBucketMap::iterator ETHBucketManager::GetFirstBucket()
{
	return m_entities.begin();
}

ETHBucketMap::iterator ETHBucketManager::GetLastBucket()
{
	return m_entities.end();
}

ETHBucketMap::const_iterator ETHBucketManager::GetFirstBucket() const
{
	return m_entities.begin();
}

ETHBucketMap::const_iterator ETHBucketManager::GetLastBucket() const
{
	return m_entities.end();
}

bool ETHBucketManager::IsEmpty() const
{
	return m_entities.empty();
}

ETHBucketMap::const_iterator ETHBucketManager::Find(const Vector2& key) const
{
	return m_entities.find(key);
}

ETHBucketMap::iterator ETHBucketManager::Find(const Vector2& key)
{
	return m_entities.find(key);
}

const Vector2& ETHBucketManager::GetBucketSize() const
{
	return m_bucketSize;
}

bool ETHBucketManager::IsDrawingBorderBuckets() const
{
	return m_drawingBorderBuckets;
}

void ETHBucketManager::SetBorderBucketsDrawing(const bool enable)
{
	m_drawingBorderBuckets = enable;
}

std::size_t ETHBucketManager::GetNumEntities(const Vector2& key) const
{
	ETHBucketMap::const_iterator iter = Find(key);
	if (iter != GetLastBucket())
	{
		return iter->second.size();
	}
	else
	{
		return 0;
	}
}

void ETHBucketManager::Add(ETHRenderEntity* entity, const SIDE side)
{
	const Vector2 bucket = ETHGlobal::GetBucket(entity->GetPositionXY(), GetBucketSize());
	if (side == FRONT)
	{
		m_entities[bucket].push_front(entity);
	}
	else
	{
		m_entities[bucket].push_back(entity);
	}

	#ifdef _DEBUG
	ETH_STREAM_DECL(ss) << GS_L("Entity ") << entity->GetEntityName() << GS_L(" (ID#") << entity->GetID()
						<< GS_L(") added to bucket ") << GS_L("(") << bucket.x << GS_L(", ") << bucket.y << GS_L(")");
	m_provider->Log(ss.str(), Platform::Logger::INFO);
	#endif
}

// moves an entity from a bucket to another
bool ETHBucketManager::MoveEntity(const int id, const Vector2 &currentBucket, const Vector2 &destBucket)
{
	// if the destiny bucket is the current bucket, don't need to do anything
	if (currentBucket == destBucket)
		return true;

	// remove the entity from its current bucket
	ETHBucketMap::iterator bucketIter = Find(currentBucket);

	if (bucketIter == GetLastBucket())
	{
		ETH_STREAM_DECL(ss) << GS_L("The current bucket doesn't exist: (") << currentBucket.x << GS_L(",") << currentBucket.y << GS_L(")");
		m_provider->Log(ss.str(), Platform::Logger::ERROR);
		return false;
	}

	ETHRenderEntity* entity = 0;
	for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != bucketIter->second.end(); iter++)
	{
		if ((*iter)->GetID() == id)
		{
			entity = *iter;
			bucketIter->second.erase(iter);
			break;
		}
	}

	// if the entity hasn't been found
	if (!entity)
	{
		ETH_STREAM_DECL(ss) << GS_L("Couldn't find entity ID ") << id << GS_L(" to move");
		m_provider->Log(ss.str(), Platform::Logger::ERROR);
		return false;
	}

	// adds the entity to the destiny bucket
	if (entity->GetType() == ETH_HORIZONTAL)
	{
		m_entities[destBucket].push_front(entity);
	}
	else
	{
		m_entities[destBucket].push_back(entity);
	}

	#ifdef _DEBUG
	ETH_STREAM_DECL(ss) << GS_L("Entity moved from bucket (") << currentBucket.x << GS_L(",") << currentBucket.y << GS_L(") to bucket (")
		 << destBucket.x << GS_L(",") << destBucket.y << GS_L(")");
	m_provider->Log(ss.str(), Platform::Logger::INFO);
	#endif
	return true;
}

unsigned int ETHBucketManager::GetNumEntities() const
{
	unsigned int nEntities = 0;
	for (ETHBucketMap::const_iterator bucketIter = GetFirstBucket(); bucketIter != GetLastBucket(); bucketIter++)
	{
		nEntities += bucketIter->second.size();
	}
	return nEntities;
}

// TODO-TO-DO: this method is too large...
int ETHBucketManager::SeekEntity(const Vector2 &at, ETHEntity **pOutData, const ETHSceneProperties& props, ETHEntity *pAfterThisOne)
{
	int closestFromBehindID =-1;
	ETHSpriteEntity *pClosestFromBehind = 0;

	int currentID =-1;
	ETHSpriteEntity *pCurrent = 0;

	int closestFromTheFrontID =-1;
	ETHSpriteEntity *pClosestFromTheFront = 0;

	if (pAfterThisOne)
	{
		ETH_VIEW_RECT rect = pAfterThisOne->GetScreenRect(props);
		if (at.x < rect.v2Min.x || at.x > rect.v2Max.x ||
			at.y < rect.v2Min.y || at.y > rect.v2Max.y)
		{
			pAfterThisOne = 0;
		}
	}

	if (IsEmpty())
	{
		return -1;
	}

	std::list<Vector2> buckets;
	ETHGlobal::GetIntersectingBuckets(buckets, at, Vector2(1,1), GetBucketSize(), true, true);

	// seeks the closest intersecting entity from behind
	for (std::list<Vector2>::const_reverse_iterator sceneBucketIter = buckets.rbegin();
		sceneBucketIter != buckets.rend(); sceneBucketIter++)
	{
		ETHBucketMap::iterator bucketIter = Find(*sceneBucketIter);

		if (bucketIter == GetLastBucket())
			continue;

		ETHEntityList::const_reverse_iterator iEnd = bucketIter->second.rend();
		bool escape = false;
		for (ETHEntityList::reverse_iterator iter = bucketIter->second.rbegin(); iter != iEnd; iter++)
		{
			ETHSpriteEntity *pRenderEntity = (*iter);
			const ETH_VIEW_RECT box = pRenderEntity->GetScreenRect(props);

			if (at.x > box.v2Min.x && at.x < box.v2Max.x &&
				at.y > box.v2Min.y && at.y < box.v2Max.y)
			{
				if (!pAfterThisOne)
				{
					if (pOutData)
						*pOutData = (*iter);
					return (*iter)->GetID();
				}
				else
				{
					if (pRenderEntity->GetID() == pAfterThisOne->GetID())
					{
						pCurrent = const_cast<ETHRenderEntity*>(*iter);
						currentID = (*iter)->GetID();
						escape = true;
						break;
					}
				}
				pClosestFromBehind = const_cast<ETHRenderEntity*>(*iter);
				closestFromBehindID = pClosestFromBehind->GetID();
			}
		}
		if (escape)
			break;
	}

	// seeks the first intersecting entity from the front
	for (std::list<Vector2>::const_iterator sceneBucketIter = buckets.begin(); sceneBucketIter != buckets.end(); sceneBucketIter++)
	{
		ETHBucketMap::iterator bucketIter = Find(*sceneBucketIter);

		if (bucketIter == GetLastBucket())
			continue;

		ETHEntityList::iterator iter;
		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		bool escape = false;
		for (iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			ETHSpriteEntity *pRenderEntity = (*iter);
			const ETH_VIEW_RECT box = pRenderEntity->GetScreenRect(props);

			if (at.x > box.v2Min.x && at.x < box.v2Max.x &&
				at.y > box.v2Min.y && at.y < box.v2Max.y)
			{
				pClosestFromTheFront = const_cast<ETHRenderEntity*>(*iter);
				closestFromTheFrontID = pClosestFromTheFront->GetID();
				escape = true;
				break;
			}
		}
		if (escape)
			break;
	}
	if (closestFromBehindID >= 0)
	{
		if (pOutData && pClosestFromBehind)
			*pOutData = pClosestFromBehind;
		return closestFromBehindID;
	}
	if (pOutData && pClosestFromTheFront)
		*pOutData = pClosestFromTheFront;
	return closestFromTheFrontID;
}

ETHSpriteEntity* ETHBucketManager::SeekEntity(const int id)
{
	for (ETHBucketMap::iterator bucketIter = GetFirstBucket(); bucketIter != GetLastBucket(); bucketIter++)
	{
		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			if ((*iter)->GetID() == id)
				return (*iter);
		}
	}
	return 0;
}

ETHSpriteEntity* ETHBucketManager::SeekEntity(const str_type::string& fileName)
{
	for (ETHBucketMap::iterator bucketIter = GetFirstBucket(); bucketIter != GetLastBucket(); bucketIter++)
	{
		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			if ((*iter)->GetEntityName() == fileName)
				return (*iter);
		}
	}
	return 0;
}

bool ETHBucketManager::DeleteEntity(const int id, const Vector2 &v2SearchBucket)
{
	ETHBucketMap::iterator bucketIter = Find(v2SearchBucket);

	if (bucketIter == GetLastBucket())
	{
		ETH_STREAM_DECL(ss) << GS_L("Couldn't find the entity to delete: ID") << id;
		m_provider->Log(ss.str(), Platform::Logger::ERROR);
		return false;
	}

	ETHEntityList::const_reverse_iterator iEnd = bucketIter->second.rend();
	for (ETHEntityList::reverse_iterator iter = bucketIter->second.rbegin(); iter != iEnd; iter++)
	{
		if ((*iter)->GetID() == id)
		{
			#ifdef _DEBUG
			ETH_STREAM_DECL(ss) << GS_L("Entity ") << (*iter)->GetEntityName() << GS_L(" (ID#") << (*iter)->GetID() << GS_L(") removed (DeleteEntity method)");
			m_provider->Log(ss.str(), Platform::Logger::INFO);
			#endif

			(*iter)->Kill();
			(*iter)->Release();
			ETHEntityList::iterator i = iter.base();
			i--;
			bucketIter->second.erase(i);
			return true;
		}
	}
	return false;
}

bool ETHBucketManager::GetEntityArrayByName(const str_type::string& name, ETHEntityArray &outVector)
{
	const unsigned int oldSize = outVector.size();
	for (ETHBucketMap::iterator bucketIter = GetFirstBucket(); bucketIter != GetLastBucket(); bucketIter++)
	{
		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			ETHSpriteEntity *pEntity = *iter;
			if (pEntity->GetEntityName() == name)
			{
				outVector.push_back(pEntity);
			}
		}
	}
	if (outVector.size() == oldSize)
		return false;
	return true;
}

bool ETHBucketManager::GetEntityArrayFromBucket(const Vector2 &v2Bucket, ETHEntityArray &outVector)
{
	ETHBucketMap::iterator bucketIter = Find(v2Bucket);
	if (bucketIter == GetLastBucket())
		return false;

	ETHEntityList::const_iterator iEnd = bucketIter->second.end();
	for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
	{
		outVector.push_back(*iter);
	}

	if (outVector.size() == 0)
		return false;
	return true;
}

void ETHBucketManager::GetVisibleEntities(ETHEntityArray &outVector)
{
	// Get the list of visible buckets
	std::list<Vector2> bucketList;
	ETHGlobal::GetIntersectingBuckets(bucketList, m_provider->GetVideo()->GetCameraPos(),
									m_provider->GetVideo()->GetScreenSizeF(), GetBucketSize(),
									IsDrawingBorderBuckets(), IsDrawingBorderBuckets());

	// Loop through all visible Buckets
	for (std::list<Vector2>::iterator bucketPositionIter = bucketList.begin();
		bucketPositionIter != bucketList.end(); bucketPositionIter++)
	{
		ETHBucketMap::const_iterator bucketIter = Find(*bucketPositionIter);

		if (bucketIter == GetLastBucket())
			continue;

		if (bucketIter->second.empty())
			continue;

		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::const_iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			outVector.push_back(*iter);
		}
	}
}

void ETHBucketManager::GetIntersectingEntities(const Vector2 &v2Here, ETHEntityArray &outVector, const bool screenSpace, const ETHSceneProperties& props)
{
	ETHEntityArray temp;
	const Vector2 v2Bucket(ETHGlobal::GetBucket(v2Here, GetBucketSize()));

	// get the main bucket and all buckets around it
	GetEntityArrayFromBucket(v2Bucket, temp);
	GetEntityArrayFromBucket(v2Bucket+Vector2(1,0), temp);
	GetEntityArrayFromBucket(v2Bucket+Vector2(1,1), temp);
	GetEntityArrayFromBucket(v2Bucket+Vector2(0,1), temp);
	GetEntityArrayFromBucket(v2Bucket+Vector2(-1,1), temp);
	GetEntityArrayFromBucket(v2Bucket+Vector2(-1,0), temp);
	GetEntityArrayFromBucket(v2Bucket+Vector2(-1,-1), temp);
	GetEntityArrayFromBucket(v2Bucket+Vector2(0,-1), temp);
	GetEntityArrayFromBucket(v2Bucket+Vector2(1,-1), temp);

	for (unsigned int t=0; t<temp.size(); t++)
	{
		ETH_VIEW_RECT rect = temp[t]->GetScreenRect(props);

		if (!screenSpace)
		{
			rect.v2Max += m_provider->GetVideo()->GetCameraPos();
			rect.v2Min += m_provider->GetVideo()->GetCameraPos();
		}

		if (v2Here.x < rect.v2Min.x)
			continue;
		if (v2Here.y < rect.v2Min.y)
			continue;
		if (v2Here.x > rect.v2Max.x)
			continue;
		if (v2Here.y > rect.v2Max.y)
			continue;
		outVector.push_back(temp[t]);
	}
}

bool ETHBucketManager::GetEntityArray(ETHEntityArray &outVector)
{
	for (ETHBucketMap::iterator bucketIter = GetFirstBucket(); bucketIter != GetLastBucket(); bucketIter++)
	{
		ETHEntityList::const_iterator iEnd = bucketIter->second.end();
		for (ETHEntityList::iterator iter = bucketIter->second.begin(); iter != iEnd; iter++)
		{
			outVector.push_back(*iter);
		}
	}
	return true;
}