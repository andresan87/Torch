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

#ifndef ETH_COMMON_H_
#define ETH_COMMON_H_

#include "../gs2d/src/gs2d.h"
#include "../gs2d/src/gs2daudio.h"
#include "../gs2d/src/gs2dinput.h"

#ifndef ANDROID
 #include "../gs2d/src/gs2dplayer.h"
#endif

#ifdef GS2D_STR_TYPE_WCHAR
 #include "../vendors/tinyxml_utf16/tinyxml.h"
#else
 #include "../vendors/tinyxml_ansi/tinyxml.h"
#endif

using namespace gs2d::math;
using namespace gs2d;

#include <math.h>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "ETHSpeedTimer.h"
#include "ETHDirectories.h"

#define _ETH_EXTRA_BYTES 512
#define _ETH_LESSER_EXTRA_BYTES 64
#define _ETH_MAX_FILENAME_SIZE 256
#define _ETH_LARGE_FILENAME_SIZE 512
#define _ETH_MAX_PARTICLE_SYS_PER_ENTITY 2
#define _ETH_DEFAULT_SHADOW_RANGE 512.0f

#define _ETH_SMALL_NUMBER (0.1f)

#define ETH_DEFAULT_PARTICLE_BITMAP	GS_L("particle.png")
#define ETH_APP_PROPERTIES_FILE GS_L("app.enml")
#define ETH_DEFAULT_BITMAP_FONT GS_L("Verdana14_shadow.fnt")

#define _ETH_EMPTY_SCENE_STRING GS_L("empty")

#define ETH_WHITE_V3 (Vector3(1.0f,1.0f,1.0f))
#define ETH_BLACK_V3 (Vector3(0.0f,0.0f,0.0f))
#define ETH_WHITE_V4 (Vector4(1.0f,1.0f,1.0f,1.0f))
#define ETH_BLACK_V4 (Vector4(0.0f,0.0f,0.0f,1.0f))

#define _ETH_SHADOW_SCALEX (0.8f)
#define _ETH_SHADOW_SCALEY (8.0f)
#define _ETH_SHADOW_FAKE_STRETCH (2.2f)
#define _ETH_SHADOW_SIZE_TOLERANCE (0.95f)
#define _ETH_DEFAULT_LIGHT_INTENSITY (2.0f)
#define _ETH_DEFAULT_AMBIENT_LIGHT (0.3f)
#define _ETH_MIN_BUCKET_SIZE (128.0f)
#define _ETH_MAX_BUCKETS (512)
#define _ETH_DEFAULT_BUCKET_SIZE (256.0f)
#define _ETH_SCENE_EDITOR_BUCKET_SIZE (Vector2(512.0f,512.0f))

#define ETH_STREAM_DECL(name) gs2d::str_type::stringstream name; name

#ifdef _MSC_VER
	#ifdef GS2D_STR_TYPE_ANSI
		#define _ETH_SAFE_swscanf sscanf_s
	#else
		#define _ETH_SAFE_swscanf swscanf_s
	#endif
#else
	#ifdef GS2D_STR_TYPE_ANSI
		#define _ETH_SAFE_swscanf sscanf
	#else
		#define _ETH_SAFE_swscanf swscanf
	#endif
#endif

#define ETH_CALLBACK_PREFIX GS_L("ETHCallback_")

#define ETH_INLINE inline

typedef long ETH_INT;
typedef unsigned char ETH_BOOL;
#define ETH_TRUE (1)
#define ETH_FALSE (0)

// TO-DO/TODO: remove deprecation constants in near future
enum ETH_ENTITY_TYPE
{
	ETH_HORIZONTAL = 0,
	ETH_GROUND_DECAL = 1, // deprecated
	ETH_VERTICAL,
	ETH_OVERALL, // depracated
	ETH_OPAQUE_DECAL, // deprecated
	ETH_LAYERABLE
};

enum ETH_MESSAGE
{
	ETH_ERROR = 0,
	ETH_WARNING = 1,
	ETH_INFO = 2
};

enum ETH_SCENE_SORT_MODE
{
	ETH_HEIGHT = 0,
	ETH_LAYERDEPTH = 1,
};

struct ETH_VIDEO_MODE
{
	ETH_VIDEO_MODE();
	unsigned int width, height;
	GS_PIXEL_FORMAT pf;
};

// TODO: more robust collision implementation for line intersection, rotation and sliding
struct ETH_COLLISION_BOX
{
	ETH_COLLISION_BOX();
	ETH_COLLISION_BOX(const Vector3 &v3Pos, const Vector3 &v3Size);
	bool Collide(const ETH_COLLISION_BOX &other, const Vector3 &entityPos0, const Vector3 &entityPos1) const;
	Vector3 pos, size;

	bool ReadFromXMLFile(TiXmlElement *pElement);
	bool WriteToXMLFile(TiXmlElement *pRoot) const;
};

struct ETH_COLLISION_IN_SCENE : public ETH_COLLISION_BOX
{
	ETH_COLLISION_IN_SCENE();
	ETH_COLLISION_IN_SCENE(const Vector3 &v3Pos, const Vector3 &v3Size, const int id);
	ETH_COLLISION_IN_SCENE(const ETH_COLLISION_BOX &box, const int id);
	int ownerID;
};

struct ETH_VIEW_RECT
{
	Vector2 v2Min, v2Max;
};

struct ETH_WINDOW_ENML_FILE
{
	ETH_WINDOW_ENML_FILE(const str_type::string& fileName, const Platform::FileManagerPtr& fileManager);

	unsigned int width, height;
	bool windowed, vsync;
	str_type::string title;
};

struct ETH_STARTUP_RESOURCES_ENML_FILE
{
	ETH_STARTUP_RESOURCES_ENML_FILE(const str_type::string& fileName, const Platform::FileManagerPtr& fileManager);

	str_type::string emtprojFilename;
	str_type::string escFilename;
};

// namespace for global functions and variables
namespace ETHGlobal {
	const str_type::char_t wcsEntityType[][64] =
	{
		GS_L("horizontal"),
		GS_L("ground decal"),
		GS_L("vertical"),
		GS_L("overall"),
		GS_L("opaque decal"),
		GS_L("layerable")
	};

	ETH_INLINE GS_ENTITY_ORIGIN ConvertToGSSO(const ETH_ENTITY_TYPE type)
	{
		switch (type)
		{
			case ETH_GROUND_DECAL:
			case ETH_OPAQUE_DECAL:
			case ETH_OVERALL:
			case ETH_LAYERABLE:
			case ETH_HORIZONTAL:
				return GSEO_CENTER;
				break;
			case ETH_VERTICAL:
				return GSEO_CENTER_BOTTOM;
				break;
			default:
				return GSEO_DEFAULT;
		};
	}

	ETH_INLINE Vector4 ConvertToV4(const GS_COLOR color)
	{
		return Vector4(
			(float)color.r/255.0f,
			(float)color.g/255.0f,
			(float)color.b/255.0f,
			(float)color.a/255.0f);
	}

	ETH_INLINE str_type::string GetFileName(const str_type::string &source)
	{
		const unsigned int len = source.length();
		str_type::string r = source;
		unsigned int t;
		for (t=len-1; t>0; t--)
		{
			if (r[t] == GS_L('\\') || r[t] == GS_L('/'))
			{
				r = r.substr(t+1);
				break;
			}
		}
		return r;
	}

	ETH_INLINE str_type::string RemoveExtension(const str_type::char_t *source)
	{
		str_type::string dest = source;
		const unsigned int len = dest.length();
		for (unsigned int t=0; t<len; t++)
		{
			if (source[t] == GS_L('.'))
			{
				dest.resize(t);
				break;
			}
		}
		return dest;
	}

	ETH_INLINE str_type::string GetDataResourceFullPath(const str_type::string& path, const str_type::string& file)
	{
		return path+file;
	}

	ETH_INLINE Vector2 GetBucket(const Vector2 &v2, const Vector2 &v2BucketSize)
	{
		return Vector2(floor(v2.x/v2BucketSize.x),floor(v2.y/v2BucketSize.y));
	}

	ETH_INLINE Vector2 ToScreenPos(const Vector3 &v3Pos, const Vector2 &zAxisDirection)
	{
		return Vector2(v3Pos.x, v3Pos.y)+(zAxisDirection*v3Pos.z);
	}

	ETH_INLINE void GetIntersectingBuckets(std::list<Vector2> &outList, const Vector2 &v2Pos,
										 const Vector2 &v2Size, const Vector2 &v2BucketSize,
										 const bool includeUpperSeams = false,
										 const bool includeLowerSeams = false)
	{
		const Vector2 v2Min = GetBucket(v2Pos, v2BucketSize) - ((includeUpperSeams) ? Vector2(1,1) : Vector2(0,0));
		const Vector2 v2Max = GetBucket(v2Pos+v2Size, v2BucketSize) + ((includeLowerSeams) ? Vector2(1,1) : Vector2(0,0));

		outList.clear();
		for (float y = v2Min.y; y<=v2Max.y; y+=1.0f)
		{
			for (float x = v2Min.x; x<=v2Max.x; x+=1.0f)
			{
				outList.push_back(Vector2(x, y));
				if (outList.size() > _ETH_MAX_BUCKETS)
				{
					return;
				}
			}
		}
	}

	ETH_INLINE Vector2 ToVector2(const Vector3 &v3)
	{
		return Vector2(v3.x, v3.y);
	}

	ETH_INLINE Vector2 V2iToV2F(const Vector2i &v2)
	{
		return Vector2(static_cast<float>(v2.x), static_cast<float>(v2.y));
	}

	ETH_INLINE bool ToBool(const ETH_BOOL b)
	{
		return (b == ETH_TRUE) ? true : false;
	}

	ETH_INLINE bool FileExists(const str_type::string &file, const Platform::FileManagerPtr& fileManager)
	{
		// TODO/TO-DO use a dedicated existence check method for that
		Platform::FileBuffer out;
		return fileManager->GetFileBuffer(file, out);
	}

	ETH_INLINE bool CollideRect(const Vector2 &pos0, const Vector2 &size0, const Vector2 &pos1, const Vector2 &size1)
	{
		const Vector2 halfSize0 = size0/2;
		const Vector2 halfSize1 = size1/2;

		const Vector2 v2Min0 = pos0-halfSize0;
		const Vector2 v2Max0 = pos0+halfSize0;

		const Vector2 v2Min1 = pos1-halfSize1;
		const Vector2 v2Max1 = pos1+halfSize1;

		if (v2Min0.x > v2Max1.x)
			return false;
		if (v2Min0.y > v2Max1.y)
			return false;

		if (v2Max0.x < v2Min1.x)
			return false;
		if (v2Max0.y < v2Min1.y)
			return false;

		return true;
	}

	ETH_INLINE bool PointInRect(const Vector2 &p, const Vector2 &pos0, const Vector2 &size0)
	{
		const Vector2 halfSize0 = size0/2;
		const Vector2 v2Min0 = pos0-halfSize0;
		const Vector2 v2Max0 = pos0+halfSize0;

		if (p.x > v2Max0.x)
			return false;
		if (p.y > v2Max0.y)
			return false;

		if (p.x < v2Min0.x)
			return false;
		if (p.y < v2Min0.y)
			return false;

		return true;
	}

	ETH_INLINE bool IsSphereInScreen(const Vector3& v3Pos, const float radius, const VideoPtr& video)
	{
		const Vector2 v2Pos(v3Pos.x, v3Pos.y-v3Pos.z);
		const Vector2& v2Cam = video->GetCameraPos();
		const Vector2& v2Screen = video->GetScreenSizeF();
		const float minX = v2Cam.x-radius;
		const float maxX = v2Cam.x+v2Screen.x+radius;
		const float minY = v2Cam.y-radius;
		const float maxY = v2Cam.y+v2Screen.y+radius;
		if (v2Pos.x < minX || v2Pos.x > maxX || v2Pos.y < minY || v2Pos.y > maxY)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	ETH_INLINE float ComputeDepth(const float height, const float maxHeight, const float minHeight)
	{
		return ((height-minHeight)/(maxHeight-minHeight));
	}

	ETH_INLINE str_type::string Vector3ToString(const Vector3 &v3)
	{
		str_type::stringstream ss;
		ss << GS_L("(") << v3.x << GS_L(", ") << v3.y << GS_L(", ") << v3.z << GS_L(")");
		return ss.str();
	}

	ETH_INLINE float ParseFloat(const str_type::char_t *str)
	{
		float f = 0.0f;
		_ETH_SAFE_swscanf(str, GS_L("%f"), &f);
		return f;
	}

	ETH_INLINE int ParseInt(const str_type::char_t *str)
	{
		int n = 0;
		_ETH_SAFE_swscanf(str, GS_L("%d"), &n);
		return n;
	}

	ETH_INLINE unsigned int ParseUInt(const str_type::char_t *str)
	{
		unsigned int n = 0;
		_ETH_SAFE_swscanf(str, GS_L("%u"), &n);
		return n;
	}

	ETH_INLINE float ParseFloatStd(const str_type::string &str)
	{
		return ParseFloat(str.c_str());
	}

	ETH_INLINE int ParseIntStd(const str_type::string &str)
	{
		return ParseInt(str.c_str());
	}

	ETH_INLINE unsigned int ParseUIntStd(const str_type::string &str)
	{
		return ParseUInt(str.c_str());
	}
} // namespace ETHGlobal

#endif