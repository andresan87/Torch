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

#ifndef ETH_SCENE_H_
#define ETH_SCENE_H_

#include "ETHEntityArray.h"
#include "ETHBucketManager.h"
#include "ETHSceneProperties.h"
#include "ETHResourceProvider.h"
#include "ETHASUtil.h"
#include "ETHTempEntityHandler.h"

class ETHScene
{
public:
	ETHScene(const str_type::string& fileName, ETHResourceProviderPtr provider, const ETHSceneProperties& props, asIScriptModule *pModule, asIScriptContext *pContext,
			 const Vector2 &v2BucketSize = Vector2(_ETH_DEFAULT_BUCKET_SIZE,_ETH_DEFAULT_BUCKET_SIZE));

	ETHScene(ETHResourceProviderPtr provider, const ETHSceneProperties& props, asIScriptModule *pModule, asIScriptContext *pContext,
			 const Vector2 &v2BucketSize = Vector2(_ETH_DEFAULT_BUCKET_SIZE,_ETH_DEFAULT_BUCKET_SIZE));

	void ClearResources();

	bool SaveToFile(const str_type::string& fileName);
	int AddEntity(ETHRenderEntity* pEntity);
	int AddEntity(ETHRenderEntity* pEntity, const str_type::string& alternativeName);
	void SetSceneProperties(const ETHSceneProperties &prop);
	void AddLight(const ETHLight &light);
	void AddLight(const ETHLight &light, const Vector3& v3Pos);
	bool GenerateLightmaps(const int id = -1);
	bool RenderScene(const bool roundUp, SpritePtr pOutline = SpritePtr(), SpritePtr pInvisibleEntSymbol = SpritePtr());
	void EnableLightmaps(const bool enable);
	bool AreLightmapsEnabled() const;
	void EnableRealTimeShadows(const bool enable);
	bool AreRealTimeShadowsEnabled() const;
	void RunCallbacksFromList();
	void ForceAllSFXStop();

	const ETHSceneProperties* GetSceneProperties() const;
	ETHSceneProperties* GetEditableSceneProperties();

	bool AddFloatData(const str_type::string &entity, const str_type::string &name, const float value);
	bool AddIntData(const str_type::string &entity, const str_type::string &name, const int value);
	bool AddUIntData(const str_type::string &entity, const str_type::string &name, const unsigned int value);
	bool AddStringData(const str_type::string &entity, const str_type::string &name, const str_type::string &value);
	bool AddVector2Data(const str_type::string &entity, const str_type::string &name, const Vector2 &value);
	bool AddVector3Data(const str_type::string &entity, const str_type::string &name, const Vector3 &value);
	bool AddCustomData(const str_type::string &entity, const str_type::string &name, const ETHCustomDataConstPtr &inData);

	/// number of lights (last-time-drawn)
	int GetNumLights();
	/// actual number of lights (slower)
	int CountLights();

	/// Lightmap state set and get
	void ShowLightmaps(const bool show);
	bool AreLightmapsShown();

	/// Light intensity setter
	void SetLightIntensity(const float intensity);
	float GetLightIntensity() const;

	/// Sets the ambient light color
	void SetAmbientLight(const Vector3 &color);
	Vector3 GetAmbientLight() const;

	/// Toggle drawing buckets around the visible bucket
	void SetBorderBucketsDrawing(const bool enable);
	bool IsDrawingBorderBuckets() const;

	/// Sets a new direction to the Z axis (in screen-space).
	void SetZAxisDirection(const Vector2 &v2);
	Vector2 GetZAxisDirection() const;

	int GetLastID() const;
	float GetMaxHeight() const;
	float GetMinHeight() const;
	Vector2 GetBucketSize() const;
	void SetHaloRotation(const bool enable);
	bool GetHaloRotation() const;
	int GetNumRenderedEntities();

	ETHBucketManager& GetBucketManager();

	void GetIntersectingEntities(const Vector2 &v2Here, ETHEntityArray &outVector, const bool screenSpace);

	unsigned int GetNumEntities() const;

	void RecoverResources();

private:

	bool LoadFromFile(const str_type::string& fileName);

	void Init(ETHResourceProviderPtr provider, const ETHSceneProperties& props, asIScriptModule *pModule, asIScriptContext *pContext);

	bool RenderTransparentLayer(std::list<ETHRenderEntity*> &halos,
								const float maxHeight, const float minHeight);
	bool RenderList(float &maxHeight, float &minHeight, SpritePtr pOutline, SpritePtr pInvisibleEntSymbol,
					std::list<ETHRenderEntity*> &outParticles, std::list<ETHRenderEntity*> &outHalos,
					const bool roundUp);

	bool RenderParticleList(std::list<ETHRenderEntity*> &particles);
	bool AssignCallbackScript(ETHSpriteEntity* entity);
	bool DrawBucketOutlines();
	bool ReadFromXMLFile(TiXmlElement *pElement);

	float ComputeDrawHash(const float depth, const Vector2& camPos, const Vector3& entityPos, const ETH_ENTITY_TYPE& type) const;

	ETHBucketManager m_buckets;
	ETHTempEntityHandler m_tempEntities;

	std::list<ETHLight> m_lights;

	ETHResourceProviderPtr m_provider;
	ETHSceneProperties m_sceneProps;
	float m_maxSceneHeight, m_minSceneHeight;
	int m_idCounter;
	int m_nCurrentLights;
	int m_nRenderedEntities;
	bool m_enableLightmaps;
	bool m_showingLightmaps;
	bool m_rotatingHalos;
	bool m_usingRTShadows;
	asIScriptModule *m_pModule;
	asIScriptContext *m_pContext;
};

typedef boost::shared_ptr<ETHScene> ETHScenePtr;

#endif