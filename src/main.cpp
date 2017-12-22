#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>
#include <vector>
#include <stdlib.h>

#include "color.hpp"
#include "helper.hpp"
#include "material.hpp"
#include "scene.hpp"
#include "scene_object.hpp"
#include "scene_parser.hpp"
#include "vector.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define ERROR() {usage(argv[0]); return -1;}

/* Print usage. */
void usage(char* prog)
{
    std::printf("USAGE: %s [-t <NUM>] [-o <PATH>] -s <PATH>\n"
                "-t <NUM>: render using NUM threads (default is 4)\n"
                "-o <PATH>: output to PATH (should be *.png. default is \"raytraced.png\")\n"
                "-s <PATH>: the scene file to be rendered\n",
                prog);
}

/* Helper that is passed into thread */
void render_stripe(Scene* scene, uint8_t start, uint8_t nthreads, uint8_t* dst)
{
    scene->RenderPixels(start, nthreads, dst);
}

int main(int argc, char* argv[])
{
    /* Parse arguments */
    std::string* outfile = nullptr;
    std::string* scenefile = nullptr;
    int thread_count = 4;

    int c = 1;

    while (c < argc) {
        std::string arg(argv[c]);
        if (arg == "-s") {
            if (++c >= argc) {
                std::fprintf(stderr, "No scene file supplied.\n");
                ERROR();
            }

            scenefile = new std::string(argv[c]);

            if (!file_exists(scenefile->c_str())) {
                std::fprintf(stderr,
                             "Scene description file %s "
                             "does not exist.\n", scenefile->c_str());
                return 1;
            }
        } else if (arg == "-o") {
            if (++c >= argc) {
                std::fprintf(stderr, "No output file supplied.\n");
                ERROR();
            }

            outfile = new std::string(argv[c]);
        } else if (arg == "-t") {
            if (++c >= argc) {
                std::fprintf(stderr, "No thread count given.\n");
                ERROR();
            }

            thread_count = atoi(argv[c]);
        }

        ++c;
    }

    if (!scenefile) {
        std::fprintf(stderr, "No scene file supplied.\n");
        ERROR();
    }

    if (!outfile) {
        outfile = new std::string("raytraced.png");
    }

    Scene scene;
    SceneParser parser(*scenefile);
    SceneComponent* sc;

    int max_verts = -1, max_norms = -1;
    VertexPool vert_pool, norm_pool;
    Vector3D v1, v2, v3, n1, n2, n3;

    MaterialPool mat_pool;
    mat_pool.push_back(DEFAULT_MAT);

    /* Build the scene. */
    while ((sc = parser.GetNext())) {
        auto v = sc->values();
        switch (sc->key) {
        case CK_CAMERA:
            scene.GetCamera().Configure(sc);
            break;
        case CK_FILM_RES:
        case CK_BACKGROUND:
        case CK_AMBIENT_LIGHT:
            scene.Configure(sc);
            break;
        case CK_MATERIAL:
            mat_pool.push_back(Material(sc));
            break;
        case CK_SPHERE:
            scene.AddObject(SceneObject::MakeFromComponent(sc, mat_pool));
            break;
        case CK_POINT_LIGHT:
        case CK_SPOT_LIGHT:
        case CK_DIRECTIONAL_LIGHT:
            scene.AddLight(LightSource::MakeFromComponent(sc));
            break;
        case CK_MAX_VERTS:
            max_verts = sc->values()[0].i_val;
            assert(max_verts > 0);
            break;
        case CK_MAX_NORMALS:
            max_norms = sc->values()[0].i_val;
            assert(max_norms > 0);
            break;
        case CK_VERTEX:
            if (max_verts < 1) {
                std::fprintf(stderr,
                             "ERROR: \"max_vertices\" not declared before "
                             "vertex in scene file %s\n", scenefile->c_str());
                return 1;
            } else if ((int) vert_pool.size() + 1 > max_verts) {
                std::fprintf(stderr,
                             "ERROR: number of vertices in scene file %s "
                             "exceeds max_vertices\n", scenefile->c_str());
                return 1;
            }
            vert_pool.push_back(Vector3D(sc));
            break;
        case CK_NORMAL:
            if (max_norms < 1) {
                std::fprintf(stderr,
                             "ERROR: \"max_normals\" not declared before "
                             "normal in scene file %s\n", scenefile->c_str());
                return 1;
            } else if ((int) norm_pool.size() + 1 > max_norms) {
                std::fprintf(stderr,
                             "ERROR: number of normals in scene file %s "
                             "exceeds max_normals\n", scenefile->c_str());
                return 1;
            }
            norm_pool.push_back(Vector3D(sc));
            break;
        case CK_NORMAL_TRIANGLE:
            n1 = norm_pool[v[3].i_val];
            n2 = norm_pool[v[4].i_val];
            n3 = norm_pool[v[5].i_val];
        case CK_TRIANGLE:
            v1 = vert_pool[v[0].i_val];
            v2 = vert_pool[v[1].i_val];
            v3 = vert_pool[v[2].i_val];
            if (sc->key == CK_TRIANGLE) {
                scene.AddObject(new Triangle(v1, v2, v3, mat_pool.back()));
            } else {
                scene.AddObject(new NormalTriangle(v1, v2, v3,
                                                   n1, n2, n3,
                                                   mat_pool.back()));
            }
            break;
        default:
            /* TODO: Implement this? */
            break;
        }

        delete sc;
    }

    uint8_t* raw = new uint8_t[scene.GetHeight() * scene.GetWidth() * 4];

    /* Run the renderer. */
    auto start = std::chrono::system_clock::now();
    std::vector<std::thread> threads;

    for (int t = 0; t < thread_count; t++) {
        threads.push_back(std::thread(render_stripe, &scene, t, thread_count, raw));
    }

    for (int t = 0; t < thread_count; t++) {
        threads[t].join();
    }

    /* Write the image to disk */
    stbi_write_png(outfile->c_str(), scene.GetWidth(), scene.GetHeight(), 4, raw, scene.GetWidth() * 4);

    /* Record the timing and clean up. */
    std::chrono::duration<double> etime = std::chrono::system_clock::now() - start;
    std::printf("\nRender time: %.2lf sec\n", etime.count());

    delete[] raw;
    delete scenefile;
    delete outfile;

    return 0;
}
