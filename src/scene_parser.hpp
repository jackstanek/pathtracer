#ifndef _SCENE_PARSER_HPP
#define _SCENE_PARSER_HPP

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

enum ComponentKey {
    CK_EOF,
    CK_NONE,
    CK_CAMERA,
    CK_FILM_RES,
    CK_OUTPUT_IMG,
    CK_MAX_VERTS,
    CK_MAX_NORMALS,
    CK_VERTEX,
    CK_NORMAL,
    CK_TRIANGLE,
    CK_NORMAL_TRIANGLE,
    CK_SPHERE,
    CK_BACKGROUND,
    CK_MATERIAL,
    CK_DIRECTIONAL_LIGHT,
    CK_POINT_LIGHT,
    CK_SPOT_LIGHT,
    CK_AMBIENT_LIGHT,
    CK_MAX_DEPTH,
    CK_N_KEYS
};

union SceneValue {
    SceneValue()
    {
    }

    ~SceneValue()
    {
    }

    double d_val;
    int i_val;
};

typedef std::vector<SceneValue> ValueList;

struct SceneComponent {
    inline SceneComponent(int key, ValueList vals) :
        key(key),
        values_(vals)
    {
    }

    const int key;

    inline const ValueList& values() const {
        return values_;
    }

private:
    ValueList values_;
};

class SceneParser {
public:
    SceneParser(std::string path);
    ~SceneParser();

    SceneComponent* GetNext();

private:
    std::ifstream input;
};

#endif
