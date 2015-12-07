#ifndef EMBREELOADER_H
#define EMBREELOADER_H

#include<objloader.h>

class embreeLoader
{
public:
    embreeLoader();
};

std::vector<float> mGeometry;
std::vector<int> mGeometry_indices;

void LoadOBJFile(const char* filename, const char* basepath = NULL)
{
    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, filename, basepath);

    // Push Mesh Data to triangle geometry
    for (size_t i = 0; i < shapes.size(); i++)
    {
        assert((shapes[i].mesh.indices.size() % 3) == 0);
    }
}

#endif // EMBREELOADER_H
