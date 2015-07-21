//
// Copyright 2012-2013, Syoyo Fujita.
//
// Licensed under 2-clause BSD liecense.
//
// Slightly modified by John DeCorato (2014)
#ifndef _TINY_OBJ_LOADER_H
#define _TINY_OBJ_LOADER_H

#include <string>
#include <vector>
#include <map>
#include <qvector3d.h>
#include <assert.h>

namespace tinyobj {

typedef struct
{
    std::string name;

    float ambient[3];
    float diffuse[3];
    float specular[3];
    float transmittance[3];
    float emission[3];
    float shininess;
    float ior;                // index of refraction
    float dissolve;           // 1 == opaque; 0 == fully transparent
    // illumination model (see http://www.fileformat.info/format/material/)
    int illum;

    std::string ambient_texname;
    std::string diffuse_texname;
    std::string specular_texname;
    std::string normal_texname;
    std::map<std::string, std::string> unknown_parameter;
} material_t;

typedef struct
{
    std::vector<float>          positions;
    std::vector<float>          normals;
    std::vector<float>          texcoords;
    std::vector<unsigned int>   indices;
} mesh_t;

typedef struct
{
    std::string  name;
    material_t   material;
    mesh_t       mesh;
} shape_t;


// Stores an edge by its vertices
struct Edge
{
	Edge(unsigned int _a, unsigned int _b)
	{
		assert(_a != _b);
		if (_a < _b)
		{
			a = _a;
			b = _b;
		}
		else 
		{
			a = _b;
			b = _a;
		}
	}

	unsigned int a;
	unsigned int b;
};


struct CompareEdges
{
	bool operator()(const Edge& e1, const Edge& e2)
	{
		if (e1.a < e2.a)
			return true;
		else if (e1.a == e2.a)
			return e1.b < e2.b;
		else
			return false;
	}
};

struct CompareVectors
{
	bool operator()(const QVector3D a, const QVector3D b)
	{
		if (a.x() < b.x()) {
            return true;
        }
        else if (a.x() == b.x()) {
            if (a.y() < b.y()) {
                return true;
            }
            else if (a.y() == b.y()) {
                if (a.z() < b.z()) {
                    return true;
                }
            }
        }
        
        return false;
	}
};

/// Loads .obj from a file.
/// 'shapes' will be filled with parsed shape data
/// The function returns error string.
/// Returns empty string when loading .obj success.
/// 'mtl_basepath' is optional, and used for base path for .mtl file.
std::string LoadObj(
    std::vector<shape_t>& shapes,   // [output]
    const char* filename,
    const char* mtl_basepath = NULL);

};

#endif  // _TINY_OBJ_LOADER_H
