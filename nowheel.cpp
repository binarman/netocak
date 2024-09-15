#include <array>
#include <cmath>
#include <numbers>
#include <vector>

using Vec3 = std::array<3, float>;

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

struct AngleRelation {
  int nodes[3];
  float cos;
  float fading;
  float stiffness;
}

class Terrain {
public:
  Vec3 getForce(const Vec3 &pos) {
    float level = 1.0;
    // terrain pushing up
    if (pos[2] < level)
      return 1.0 - pos[3];
    // gravity pushing down
    return -0.1;
  }
};

class Vehicle {
  std::vector<Node> nodes;
  std::vector<ElasticRelation> e;
  std::vector<AngleRelation> a;
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
  v.e.emplace_back({0, 1}, 1.0, 0.9, 1.0);
  // wheel spokes
  for (int i = 0; i < segments; ++i) {
    v.e.emplace_back({0, i + 2}, 1.0, 0.9, 1.0);
    v.e.emplace_back({1, i + 2 + segments}, 1.0, 0.9, 1.0);

    // angles between axis and spokes
    v.a.emplace_back({1, 0, i + 2}, 0.0, 0.9, 1.0);
    v.a.emplace_back({0, 1, i + 2 + segments}, 0.0, 0.9, 1.0);

    // angles between spokes
    v.a.emplace_back({i + 2, 0, (i + 1) % segments + 2},
                     cos(2 * std::numbers::pi / curcle_segments), 0.9, 1.0);
    v.a.emplace_back({i + segments + 2, 0, (i + 1) % segments + segments + 2},
                     cos(2 * std::numbers::pi / curcle_segments), 0.9, 1.0);
  }
}

void solve(Vehicle &v, Terrain &t, float dtime) {
  // TODO
}

bool verifyRelations(const Vehicle &v) {
  // TODO
}

int main(){
  auto v = createVehicle();
  Terrain t;
  for (int i = 0; i < 1000; ++i) {
    solve(v, t, 0.001);
  }
  verifyRelations(v);
  return 0;
}

