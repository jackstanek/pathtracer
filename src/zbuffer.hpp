#ifndef ZBUFFER_H_
#define ZBUFFER_H_

#include <queue>

struct SceneObjectIntersection;

typedef std::priority_queue<SceneObjectIntersection,
                            std::vector<SceneObjectIntersection>,
                            std::greater<SceneObjectIntersection> > ZBuffer;

#endif
