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

#include "ETHCustomDataManager.h"
#include <iostream>

const str_type::string ETHCustomDataManager::DATA_NAME[ETH_CUSTOM_DATA_TYPE_COUNT] =
{
	(GS_L("")),
	(GS_L("float")),
	(GS_L("int")),
	(GS_L("uint")),
	(GS_L("string")),
	(GS_L("vector2")),
	(GS_L("vector3"))
};

const str_type::string &ETHCustomDataManager::GetDataName(const unsigned int n)
{
	assert(n<ETH_CUSTOM_DATA_TYPE_COUNT);
	return DATA_NAME[n];
}

void ETHCustomDataManager::SetFloat(const str_type::string &name, const float &value)
{
	m_data[name] = ETHCustomDataPtr(new ETHFloatData(value));
}

void ETHCustomDataManager::SetInt(const str_type::string &name, const int &value)
{
	m_data[name] = ETHCustomDataPtr(new ETHIntData(value));
}

void ETHCustomDataManager::SetUInt(const str_type::string &name, const unsigned int &value)
{
	m_data[name] = ETHCustomDataPtr(new ETHUIntData(value));
}

void ETHCustomDataManager::SetString(const str_type::string &name, const str_type::string &sValue)
{
	m_data[name] = ETHCustomDataPtr(new ETHStringData(sValue));
}

void ETHCustomDataManager::SetVector2(const str_type::string &name, const Vector2 &v)
{
	m_data[name] = ETHCustomDataPtr(new ETHVector2Data(v));
}

void ETHCustomDataManager::SetVector3(const str_type::string &name, const Vector3 &v)
{
	m_data[name] = ETHCustomDataPtr(new ETHVector3Data(v));
}

bool ETHCustomDataManager::GetFloat(const str_type::string &name, float &outValue) const
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return false;
	}
	if (iter->second->GetType() != ETHDT_FLOAT)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not a float");
		return false;
	}
	outValue = iter->second->GetFloat();
	return true;
}

bool ETHCustomDataManager::GetInt(const str_type::string &name, int &outValue) const
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return false;
	}
	if (iter->second->GetType() != ETHDT_INT)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not an int");
		return false;
	}
	outValue = iter->second->GetInt();
	return true;
}

bool ETHCustomDataManager::GetUInt(const str_type::string &name, unsigned int &outValue) const
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return false;
	}
	if (iter->second->GetType() != ETHDT_UINT)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not an uint");
		return false;
	}
	outValue = iter->second->GetUInt();
	return true;
}

bool ETHCustomDataManager::GetString(const str_type::string &name, str_type::string &outValue) const
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return false;
	}
	if (iter->second->GetType() != ETHDT_STRING)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not a string");
		return false;
	}
	outValue = iter->second->GetString();
	return true;
}

bool ETHCustomDataManager::GetVector2(const str_type::string &name, Vector2 &outValue) const
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return false;
	}
	if (iter->second->GetType() != ETHDT_VECTOR2)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not a vector2");
		return false;
	}
	outValue = iter->second->GetVector2();
	return true;
}

bool ETHCustomDataManager::GetVector3(const str_type::string &name, Vector3 &outValue) const
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return false;
	}
	if (iter->second->GetType() != ETHDT_VECTOR3)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not a vector3");
		return false;
	}
	outValue = iter->second->GetVector3();
	return true;
}

void ETHCustomDataManager::AddToFloat(const str_type::string &name, const float &value)
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return;
	}
	if (iter->second->GetType() != ETHDT_FLOAT)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not a float");
		return;
	}
	iter->second->Set(value+iter->second->GetFloat());
}

void ETHCustomDataManager::AddToInt(const str_type::string &name, const int &value)
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return;
	}
	if (iter->second->GetType() != ETHDT_INT)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not an int");
		return;
	}
	iter->second->Set(value+iter->second->GetInt());
}

void ETHCustomDataManager::AddToUInt(const str_type::string &name, const unsigned int &value)
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return;
	}
	if (iter->second->GetType() != ETHDT_UINT)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not an uint");
		return;
	}
	iter->second->Set(value+iter->second->GetUInt());
}

void ETHCustomDataManager::AddToVector2(const str_type::string &name, const Vector2 &v)
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return;
	}
	if (iter->second->GetType() != ETHDT_VECTOR2)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not a vector2");
		return;
	}
	iter->second->Set(v+iter->second->GetVector2());
}

void ETHCustomDataManager::AddToVector3(const str_type::string &name, const Vector3 &v)
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return;
	}
	if (iter->second->GetType() != ETHDT_VECTOR3)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not a vector3");
		return;
	}
	iter->second->Set(v+iter->second->GetVector3());
}

void ETHCustomDataManager::MultiplyFloat(const str_type::string &name, const float &value)
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return;
	}
	if (iter->second->GetType() != ETHDT_FLOAT)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not a float");
		return;
	}
	iter->second->Set(value*iter->second->GetFloat());
}

void ETHCustomDataManager::MultiplyInt(const str_type::string &name, const int &value)
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return;
	}
	if (iter->second->GetType() != ETHDT_INT)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not an int");
		return;
	}
	iter->second->Set(value*iter->second->GetInt());
}

void ETHCustomDataManager::MultiplyUInt(const str_type::string &name, const unsigned int &value)
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return;
	}
	if (iter->second->GetType() != ETHDT_UINT)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not an uint");
		return;
	}
	iter->second->Set(value*iter->second->GetUInt());
}

void ETHCustomDataManager::MultiplyVector2(const str_type::string &name, const float &value)
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return;
	}
	if (iter->second->GetType() != ETHDT_VECTOR2)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not a vector2");
		return;
	}
	iter->second->Set(iter->second->GetVector2()*value);
}

void ETHCustomDataManager::MultiplyVector3(const str_type::string &name, const float &value)
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		ETH_STREAM_DECL(ss) << GS_L("There's no matching data for ") << name;
		return;
	}
	if (iter->second->GetType() != ETHDT_VECTOR3)
	{
		ETH_STREAM_DECL(ss) << name << GS_L(":is not a vector3");
		return;
	}
	iter->second->Set(iter->second->GetVector3()*value);
}

ETH_CUSTOM_DATA_TYPE ETHCustomDataManager::Check(const str_type::string &name) const
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter != m_data.end())
	{
		return iter->second->GetType();
	}
	else
	{
		return ETHDT_NODATA;
	}
}

str_type::string ETHCustomDataManager::GetDebugStringData() const
{
	if (!m_data.empty())
	{
		str_type::stringstream ss;
		for (std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.begin(); iter != m_data.end(); iter++)
		{
			ss << DATA_NAME[iter->second->GetType()] << GS_L(" ") << iter->first
				<< GS_L(" = ") << iter->second->GetValueAsString() << std::endl;
		}
		return ss.str();
	}
	else
	{
		return GS_L("");
	}
}

bool ETHCustomDataManager::HasData() const
{
	return (m_data.size()>0);
}

bool ETHCustomDataManager::EraseData(const str_type::string &name)
{
	std::map<str_type::string, ETHCustomDataPtr>::iterator iter = m_data.find(name);
	if (iter != m_data.end())
	{
		m_data.erase(iter);
	}
	else
	{
		return false;
	}
	return true;
}

unsigned int ETHCustomDataManager::GetNumVariables() const
{
	return m_data.size();
}

void ETHCustomDataManager::Clear()
{
	m_data.clear();
}

void ETHCustomDataManager::MoveData(ETHCustomDataManager &dataOut) const
{
	
	for (std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.begin();
		 iter != m_data.end(); iter++)
	{
		dataOut.AddData(iter->first, iter->second);
	}
}

void ETHCustomDataManager::InsertData(const ETHCustomDataManager &dataIn)
{
	for (std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = dataIn.m_data.begin();
		 iter != dataIn.m_data.end(); iter++)
	{
		AddData(iter->first, iter->second);
	}
}

void ETHCustomDataManager::CopyMap(std::map<str_type::string, ETHCustomDataPtr> &inMap) const
{
	inMap.clear();
	for (std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.begin(); iter != m_data.end(); iter++)
	{
		switch (iter->second->GetType())
		{
		case ETHDT_FLOAT:
			inMap[iter->first] = ETHCustomDataPtr(new ETHFloatData(iter->second->GetFloat()));
			break;
		case ETHDT_INT:
			inMap[iter->first] = ETHCustomDataPtr(new ETHIntData(iter->second->GetInt()));
			break;
		case ETHDT_UINT:
			inMap[iter->first] = ETHCustomDataPtr(new ETHUIntData(iter->second->GetUInt()));
			break;
		case ETHDT_STRING:
			inMap[iter->first] = ETHCustomDataPtr(new ETHStringData(iter->second->GetString()));
			break;
		case ETHDT_VECTOR2:
			inMap[iter->first] = ETHCustomDataPtr(new ETHVector2Data(iter->second->GetVector2()));
			break;
		case ETHDT_VECTOR3:
			inMap[iter->first] = ETHCustomDataPtr(new ETHVector3Data(iter->second->GetVector3()));
			break;
		};
	}
}

void ETHCustomDataManager::AddData(const str_type::string &name, const ETHCustomDataConstPtr &dataIn)
{
	switch (dataIn->GetType())
	{
		case ETHDT_FLOAT:
			m_data[name] = ETHCustomDataPtr(new ETHFloatData(dataIn->GetFloat()));
			break;
		case ETHDT_INT:
			m_data[name] = ETHCustomDataPtr(new ETHIntData(dataIn->GetInt()));
			break;
		case ETHDT_UINT:
			m_data[name] = ETHCustomDataPtr(new ETHUIntData(dataIn->GetUInt()));
			break;
		case ETHDT_STRING:
			m_data[name] = ETHCustomDataPtr(new ETHStringData(dataIn->GetString()));
			break;
		case ETHDT_VECTOR2:
			m_data[name] = ETHCustomDataPtr(new ETHVector2Data(dataIn->GetVector2()));
			break;
		case ETHDT_VECTOR3:
			m_data[name] = ETHCustomDataPtr(new ETHVector3Data(dataIn->GetVector3()));
			break;
	};
}

str_type::string ETHCustomDataManager::GetValueAsString(const str_type::string &name) const
{
	std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.find(name);
	if (iter == m_data.end())
	{
		return GS_L("");
	}
	else
	{
		return iter->second->GetValueAsString();
	}
}

bool ETHCustomDataManager::ReadDataFromXMLFile(TiXmlElement *pRoot)
{
	TiXmlNode *pNode = pRoot->FirstChild(GS_L("CustomData"));
	if (pNode)
	{
		TiXmlElement *pEntities = pNode->ToElement();
		if (pEntities)
		{
			pNode = pEntities->FirstChild(GS_L("Variable"));
			if (pNode)
			{
				TiXmlElement *pVarIter = pNode->ToElement();
				if (pVarIter)
				{
					do
					{
						str_type::string type, name, value;
						TiXmlElement *pElement;

						// read the variable type (as a string)
						pNode = pVarIter->FirstChild(GS_L("Type"));
						if (pNode)
						{
							pElement = pNode->ToElement();
							if (pElement)
							{
								type = pElement->GetText();
							}
						}

						// read the variable name
						pNode = pVarIter->FirstChild(GS_L("Name"));
						if (pNode)
						{
							pElement = pNode->ToElement();
							if (pElement)
							{
								name = pElement->GetText();
							}
						}

						// read the variable value
						if (type != GS_L("") && name != GS_L(""))
						{
							pNode = pVarIter->FirstChild(GS_L("Value"));
							if (pNode)
							{
								pElement = pNode->ToElement();
								if (pElement)
								{
									if (type == DATA_NAME[ETHDT_VECTOR2])
									{
										Vector2 value(0,0);
										pElement->QueryFloatAttribute(GS_L("x"), &value.x);
										pElement->QueryFloatAttribute(GS_L("y"), &value.y);
										SetVector2(name, value);
									}
									else if (type == DATA_NAME[ETHDT_VECTOR3])
									{
										Vector3 value(0,0,0);
										pElement->QueryFloatAttribute(GS_L("x"), &value.x);
										pElement->QueryFloatAttribute(GS_L("y"), &value.y);
										pElement->QueryFloatAttribute(GS_L("z"), &value.z);
										SetVector3(name, value);
									}
									else
									{
										value = pElement->GetText();

										// passing the variable to the map
										if (type == DATA_NAME[ETHDT_FLOAT])
										{
											SetFloat(name, ETHGlobal::ParseFloat(value.c_str()));
										} else
										if (type == DATA_NAME[ETHDT_INT])
										{
											SetInt(name, ETHGlobal::ParseInt(value.c_str()));
										} else
										if (type == DATA_NAME[ETHDT_UINT])
										{
											SetUInt(name, ETHGlobal::ParseUInt(value.c_str()));
										} else
										if (type == DATA_NAME[ETHDT_STRING])
										{
											SetString(name, value);
										}
									}
								}
							}
						}

						pVarIter = pVarIter->NextSiblingElement();
					} while (pVarIter);
				}
			}
		}
	}
	return true;
}

bool ETHCustomDataManager::WriteDataToFile(TiXmlElement *pHeadRoot) const
{
	TiXmlElement *pCustomData = new TiXmlElement(GS_L("CustomData"));
	pHeadRoot->LinkEndChild(pCustomData);
	for (std::map<str_type::string, ETHCustomDataPtr>::const_iterator iter = m_data.begin();
		iter != m_data.end(); iter++)
	{
		TiXmlElement *pVariableRoot = new TiXmlElement(GS_L("Variable"));
		pCustomData->LinkEndChild(pVariableRoot); 

		TiXmlElement *pElement;
		pElement = new TiXmlElement(GS_L("Type"));
		pElement->LinkEndChild(new TiXmlText(DATA_NAME[iter->second->GetType()] ));
		pVariableRoot->LinkEndChild(pElement);

		pElement = new TiXmlElement(GS_L("Name"));
		pElement->LinkEndChild(new TiXmlText(iter->first));
		pVariableRoot->LinkEndChild(pElement);

		pElement = new TiXmlElement(GS_L("Value"));

		str_type::stringstream ss;
		switch (iter->second->GetType())
		{
		case ETHDT_FLOAT:
			ss << iter->second->GetFloat();
			break;
		case ETHDT_INT:
			ss << iter->second->GetInt();
			break;
		case ETHDT_UINT:
			ss << iter->second->GetUInt();
			break;
		case ETHDT_STRING:
			ss << iter->second->GetString();
			break;
		};

		switch (iter->second->GetType())
		{
		case ETHDT_FLOAT:
		case ETHDT_INT:
		case ETHDT_UINT:
		case ETHDT_STRING:
			pElement->LinkEndChild(new TiXmlText(ss.str()));
			break;
		case ETHDT_VECTOR2:
			pElement->SetDoubleAttribute(GS_L("x"), iter->second->GetVector2().x);
			pElement->SetDoubleAttribute(GS_L("y"), iter->second->GetVector2().y);
			break;
		case ETHDT_VECTOR3:
			pElement->SetDoubleAttribute(GS_L("x"), iter->second->GetVector3().x);
			pElement->SetDoubleAttribute(GS_L("y"), iter->second->GetVector3().y);
			pElement->SetDoubleAttribute(GS_L("z"), iter->second->GetVector3().z);
			break;
		};
		pVariableRoot->LinkEndChild(pElement);
	}

	return true;
}