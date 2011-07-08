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

#ifndef ETH_DATA_MANAGER_H_
#define ETH_DATA_MANAGER_H_

#include "ETHCommon.h"
#include <map>
#include <string>

enum ETH_CUSTOM_DATA_TYPE
{
	ETHDT_NODATA = 0,
	ETHDT_FLOAT = 1,
	ETHDT_INT,
	ETHDT_UINT,
	ETHDT_STRING,
	ETHDT_VECTOR2,
	ETHDT_VECTOR3,
	ETHDT_MAX,
	//ETHDT_END = 64
};

#define ETH_CUSTOM_DATA_TYPE_COUNT (ETHDT_MAX)

class ETHCustomData
{
public:
	inline virtual float GetFloat() const { return 0.0f; }
	inline virtual int GetInt() const { return 0; }
	inline virtual unsigned int GetUInt() const { return 0; }
	inline virtual str_type::string GetString() const { return GS_L(""); }
	inline virtual Vector2 GetVector2() const { return Vector2(0,0); }
	inline virtual Vector3 GetVector3() const { return Vector3(0,0,0); }

	inline virtual bool Set(const float value) { GS2D_UNUSED_ARGUMENT(value); return false; }
	inline virtual bool Set(const int value) { GS2D_UNUSED_ARGUMENT(value); return false; }
	inline virtual bool Set(const unsigned int value) { GS2D_UNUSED_ARGUMENT(value); return false; }
	inline virtual bool Set(const str_type::string &sValue) { GS2D_UNUSED_ARGUMENT(sValue); return false; }
	inline virtual bool Set(const Vector2 &v) { GS2D_UNUSED_ARGUMENT(v); return false; }
	inline virtual bool Set(const Vector3 &v) { GS2D_UNUSED_ARGUMENT(v); return false; }

	virtual ETH_CUSTOM_DATA_TYPE GetType() const = 0;
	virtual str_type::string GetValueAsString() const = 0;
};

typedef boost::shared_ptr<ETHCustomData> ETHCustomDataPtr;
typedef boost::shared_ptr<const ETHCustomData> ETHCustomDataConstPtr;

class ETHIntData : public ETHCustomData {
	int v;
public: inline ETHIntData(const int v) { this->v = v; }
		inline bool Set(const int value) { v = value; return true; }
		inline int GetInt() const { return v; }
		inline ETH_CUSTOM_DATA_TYPE GetType() const { return ETHDT_INT; }
		inline str_type::string GetValueAsString() const { str_type::stringstream ss; ss << v; return ss.str(); }
};

class ETHUIntData : public ETHCustomData {
	unsigned int v;
public: inline ETHUIntData(const unsigned int v) { this->v = v; }
		inline bool Set(const unsigned int value) { v = value; return true; }
		inline unsigned int GetUInt() const { return v; }
		inline ETH_CUSTOM_DATA_TYPE GetType() const { return ETHDT_UINT; }
		inline str_type::string GetValueAsString() const { str_type::stringstream ss; ss << v; return ss.str(); }
};

class ETHFloatData : public ETHCustomData {
	float v;
public: inline ETHFloatData(const float v) { this->v = v; }
		inline bool Set(const float value) { v = value; return true; }
		inline float GetFloat() const { return v; }
		inline ETH_CUSTOM_DATA_TYPE GetType() const { return ETHDT_FLOAT; }
		inline str_type::string GetValueAsString() const { str_type::stringstream ss; ss << v; return ss.str(); }
};

class ETHStringData : public ETHCustomData {
	str_type::string v;
public: inline ETHStringData(const str_type::string &v) { this->v = v; }
		inline bool Set(const str_type::string &value) { v = value; return true; }
		inline str_type::string GetString() const { return v; }
		inline ETH_CUSTOM_DATA_TYPE GetType() const { return ETHDT_STRING; }
		inline str_type::string GetValueAsString() const { return v; }
};

class ETHVector2Data : public ETHCustomData {
	Vector2 v;
public: inline ETHVector2Data(const Vector2 &v) { this->v = v; }
		inline bool Set(const Vector2 &value) { v = value; return true; }
		inline Vector2 GetVector2() const { return v; }
		inline ETH_CUSTOM_DATA_TYPE GetType() const { return ETHDT_VECTOR2; }
		inline str_type::string GetValueAsString() const { str_type::stringstream ss; ss << "(" << v.x << ", " << v.y << ")"; return ss.str(); }
};

class ETHVector3Data : public ETHCustomData {
	Vector3 v;
public: inline ETHVector3Data(const Vector3 &v) { this->v = v; }
		inline bool Set(const Vector3 &value) { v = value; return true; }
		inline Vector3 GetVector3() const { return v; }
		inline ETH_CUSTOM_DATA_TYPE GetType() const { return ETHDT_VECTOR3; }
		inline str_type::string GetValueAsString() const { str_type::stringstream ss; ss << "(" << v.x << ", " << v.y << ", " << v.z << ")"; return ss.str(); }
};

class ETHCustomDataManager
{
	static const str_type::string DATA_NAME[ETH_CUSTOM_DATA_TYPE_COUNT];

public:
	str_type::string GetDebugStringData() const;
	static const str_type::string &GetDataName(const unsigned int n);

	void AddData(const str_type::string &name, const ETHCustomDataConstPtr &dataIn);

	void SetFloat(const str_type::string &name, const float &value);
	void SetInt(const str_type::string &name, const int &value);
	void SetUInt(const str_type::string &name, const unsigned int &value);
	void SetString(const str_type::string &name, const str_type::string &sValue);
	void SetVector2(const str_type::string &name, const Vector2 &v);
	void SetVector3(const str_type::string &name, const Vector3 &v);

	bool GetFloat(const str_type::string &name, float &outValue) const;
	bool GetInt(const str_type::string &name, int &outValue) const;
	bool GetUInt(const str_type::string &name, unsigned int &outValue) const;
	bool GetString(const str_type::string &name, str_type::string &outValue) const;
	bool GetVector2(const str_type::string &name, Vector2 &outValue) const;
	bool GetVector3(const str_type::string &name, Vector3 &outValue) const;

	void AddToFloat(const str_type::string &name, const float &value);
	void AddToInt(const str_type::string &name, const int &value);
	void AddToUInt(const str_type::string &name, const unsigned int &value);
	void AddToVector2(const str_type::string &name, const Vector2 &v);
	void AddToVector3(const str_type::string &name, const Vector3 &v);

	void MultiplyFloat(const str_type::string &name, const float &value);
	void MultiplyInt(const str_type::string &name, const int &value);
	void MultiplyUInt(const str_type::string &name, const unsigned int &value);
	void MultiplyVector2(const str_type::string &name, const float &value);
	void MultiplyVector3(const str_type::string &name, const float &value);

	ETH_CUSTOM_DATA_TYPE Check(const str_type::string &name) const;
	bool HasData() const;
	bool EraseData(const str_type::string &name);
	unsigned int GetNumVariables() const;
	void Clear();

	void InsertData(const ETHCustomDataManager &dataIn);
	void MoveData(ETHCustomDataManager &dataOut) const;
	void CopyMap(std::map<str_type::string, ETHCustomDataPtr> &inMap) const;
	str_type::string GetValueAsString(const str_type::string &name) const;

	bool ReadDataFromXMLFile(TiXmlElement *pRoot);
	bool WriteDataToFile(TiXmlElement *pHeadRoot) const;

private:
	std::map<str_type::string, ETHCustomDataPtr> m_data;
};

#endif