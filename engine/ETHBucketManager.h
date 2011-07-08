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

#ifndef ETH_BUCKET_MANAGER_H_
#define ETH_BUCKET_MANAGER_H_

class ETHRenderEntity;
class ETHSpriteEntity;
class ETHEntity;
class ETHEntityArray;

#include "ETHSceneProperties.h"
#include "ETHResourceProvider.h"
#include <list>

// Vector2 hash function
namespace boost {
std::size_t hash_value(Vector2 const& p);
} // namepace boost

#include <boost/unordered/unordered_map.hpp>

typedef std::list<ETHRenderEntity*> ETHEntityList;
typedef boost::unordered_map<Vector2, ETHEntityList, boost::hash<Vector2> > ETHBucketMap;

class ETHBucketManager
{
public:
	ETHBucketManager(const ETHResourceProviderPtr& provider, const Vector2& bucketSize, const bool drawingBorderBuckets);
	~ETHBucketManager();

	enum SIDE
	{
		FRONT = 0,
		BACK = 1
	};

	ETHBucketMap::iterator GetFirstBucket();
	ETHBucketMap::iterator GetLastBucket();
	ETHBucketMap::iterator Find(const Vector2& key);
	ETHBucketMap::const_iterator Find(const Vector2& key) const;
	ETHBucketMap::const_iterator GetFirstBucket() const;
	ETHBucketMap::const_iterator GetLastBucket() const;

	bool IsEmpty() const;
	bool IsDrawingBorderBuckets() const;
	std::size_t GetNumEntities(const Vector2& key) const;
	void SetBorderBucketsDrawing(const bool enable);

	void Add(ETHRenderEntity* entity, const SIDE side);
	const Vector2& GetBucketSize() const;

	bool MoveEntity(const int id, const Vector2 &currentBucket, const Vector2 &destBucket);
	unsigned int GetNumEntities() const;

	/// Search for an entity whose location collides with the 'at' point
	/// It returns the entity ID # and it's further data. If nAfterThisID is greater than
	/// -1 the entity returned will be another one (if the point collides with more than
	/// one entities)
	int SeekEntity(const Vector2 &at, ETHEntity **pOutData, const ETHSceneProperties& props, ETHEntity *pAfterThisOne = 0);

	/// Seek the entity by ID #
	ETHSpriteEntity *SeekEntity(const int id);

	/// Seek the entity by it's original file name file name
	ETHSpriteEntity *SeekEntity(const str_type::string& fileName);

	/// Delete the entity by ID #
	bool DeleteEntity(const int id, const Vector2 &v2SearchBucket);

	/// get an array of pointers with all entities named 'name' in scene
	bool GetEntityArrayByName(const str_type::string& name, ETHEntityArray &outVector);

	/// get an array of pointers with all entities in the bucket
	bool GetEntityArrayFromBucket(const Vector2 &v2Bucket, ETHEntityArray &outVector);

	/// get an array of visible entities
	void GetVisibleEntities(ETHEntityArray &outVector);

	/// get an array containing all entities that intersect with the point
	void GetIntersectingEntities(const Vector2 &v2Here, ETHEntityArray &outVector, const bool screenSpace, const ETHSceneProperties& props);

	/// get an array containing all entities in scene
	bool GetEntityArray(ETHEntityArray &outVector);

private:
	ETHResourceProviderPtr m_provider;
	ETHBucketManager& operator=(const ETHBucketManager& p);
	ETHBucketMap m_entities;
	const Vector2 m_bucketSize;
	bool m_drawingBorderBuckets;
};

#endif