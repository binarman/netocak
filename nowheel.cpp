#include <array>
#include <cmath>
#include <numbers>
#include <vector>

using Vec3 = std::array<float, 3>;

struct Node {
  Vec3 pos;
  Vec3 speed;
  float mass;
};

struct ElasticRelation {
  int nodes[2];
  float l;
  float fading;
  float stiffness;
};

float distance(Vec3 p1, Vec3 p2) {
  float dist = 0;
  for (int i = 0; i < 3; ++i)
    dist += p1[i] * p2[i];
  return sqrt(dist);
}

class Terrain {
public:
  Vec3 getForce(const Vec3 &pos) {
    float level = 1.0;
    // terrain pushing up
    if (pos[2] < level)
      return {0.0, 0.0, 1.0f - pos[3]};
    // gravity pushing down
    return {0.0, 0.0, -0.1};
  }
};

struct Vehicle {
  std::vector<Node> nodes;
  std::vector<ElasticRelation> e;
};

Vehicle createVehicle() {
  Vehicle v;
  const int segments = 10;
  // two circles + 2 axis nodes
  // do not initialize coordinates properly,
  // let's see if solver can self assembly structure
  v.nodes.resize(segments * 2 + 2);
  for (int i = 0; i < v.nodes.size(); ++i) {
    v.nodes[i].pos[0] = i / 10.0;
    v.nodes[i].pos[1] = i / 10.0;
    v.nodes[i].pos[2] = 1.0 + i / 100.0;
  }
  // axis length
  v.e.push_back(ElasticRelation{{0, 1}, 1.0, 0.9, 1.0});
  // wheel spokes
  for (int i = 0; i < segments; ++i) {
    v.e.push_back(ElasticRelation{{0, i + 2}, 1.0, 0.9, 1.0});
    v.e.push_back(ElasticRelation{{1, i + 2 + segments}, 1.0, 0.9, 1.0});
  }
  return v;
}

void solve(Vehicle &v, Terrain &t, float dtime) {
  // TODO
}

bool verifyRelations(const Vehicle &v, float abs_tolerance, float rel_tolerance) {
  for (const auto &e: v.e) {
    float dist = distance(v.nodes[e.nodes[0]].pos, v.nodes[e.nodes[1]].pos);
    float abs_error = fabs(dist - e.l);
    float rel_error = abs_error/e.l;
    if (abs_error > abs_tolerance && rel_error > rel_tolerance)
      return false;
  }
  return true;
}

int main(){
  auto v = createVehicle();
  Terrain t;
  for (int i = 0; i < 1000; ++i) {
    solve(v, t, 0.001);
  }
  verifyRelations(v, 0.01, 0.01);
  return 0;
}

