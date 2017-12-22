#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>

#include "scene_parser.hpp"

SceneParser::SceneParser(std::string path) :
    input(path.c_str())
{
}

SceneParser::~SceneParser()
{
}

SceneComponent* SceneParser::GetNext() {
    std::string line;

    // check for errors in opening the file
    if(input.fail()){
        return nullptr;
    }
    SceneValue val;
    ValueList vals;

    //Loop through reading each line
    std::string command;
    if (input >> command) { //Read first word in the line (i.e., the command type)
        if (command[0] == '#'){
            getline(input, line); //skip rest of line
            return new SceneComponent(CK_NONE, ValueList());
        }

        if (command == "camera") {
            for (int i = 0; i < 10; i++) {
                input >> val.d_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_CAMERA, vals);

        } else if (command == "film_resolution") {
            for (int i = 0; i < 2; i++) {
                input >> val.i_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_FILM_RES, vals);

        } else if (command == "sphere") {
            for (int i = 0; i < 4; i++) {
                input >> val.d_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_SPHERE, vals);

        } else if (command == "max_vertices") {
            for (int i = 0; i < 1; i++) {
                input >> val.i_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_MAX_VERTS, vals);

        } else if (command == "max_normals") {
            for (int i = 0; i < 1; i++) {
                input >> val.i_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_MAX_NORMALS, vals);

        } else if (command == "vertex") {
            for (int i = 0; i < 3; i++) {
                input >> val.d_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_VERTEX, vals);

        } else if (command == "normal") {
            for (int i = 0; i < 3; i++) {
                input >> val.d_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_NORMAL, vals);

        } else if (command == "triangle") {
            for (int i = 0; i < 3; i++) {
                input >> val.i_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_TRIANGLE, vals);

        } else if (command == "normal_triangle") {
            for (int i = 0; i < 6; i++) {
                input >> val.i_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_NORMAL_TRIANGLE, vals);

        } else if (command == "background") {
            for (int i = 0; i < 3; i++) {
                input >> val.d_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_BACKGROUND, vals);

        } else if (command == "material") {
            for (int i = 0; i < 14; i++) {
                input >> val.d_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_MATERIAL, vals);

        } else if (command == "directional_light") {
            for (int i = 0; i < 6; i++) {
                input >> val.d_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_DIRECTIONAL_LIGHT, vals);

        } else if (command == "point_light") {
            for (int i = 0; i < 6; i++) {
                input >> val.d_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_POINT_LIGHT, vals);

        } else if (command == "spot_light") {
            for (int i = 0; i < 11; i++) {
                input >> val.d_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_SPOT_LIGHT, vals);

        } else if (command == "ambient_light") {
            for (int i = 0; i < 3; i++) {
                input >> val.d_val;
                vals.push_back(val);
            }
            return new SceneComponent(CK_AMBIENT_LIGHT, vals);

        } else if (input.eof()) {
            return nullptr;
        } else {
            return new SceneComponent(CK_NONE, vals);
        }
    }

    return nullptr;
}
