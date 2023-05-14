#include "cws/map_layer/illumination.hpp"
#include <algorithm>
#include <list>
#include <vector>

static Illumination ILLUMINATION_DEFAULT = {.value = -1};

using PII = std::pair<int, int>;

PII getVector(Coordinates p1, Coordinates p2) {
  return std::make_pair(p2.x - p1.x, p2.y - p1.y);
}

int getScalarMultiplication(PII v1, PII v2) {
  return v1.first * v2.first + v1.second * v2.second;
}

int getDistanceSquare(PII v) { return v.first * v.first + v.second * v.second; }

std::list<Coordinates> getNeighbours(Dimension dim, Coordinates p) {

  std::list<Coordinates> n;

  for (int x = p.x - 1; x <= p.x + 1; ++x) {
    for (int y = p.y - 1; y <= p.y + 1; ++y) {
      if (x < 0 || x >= dim.width || y < 0 || y >= dim.height ||
          (x == p.x && y == p.x)) {
        continue;
      }
      n.emplace_back(x, y);
    }
  }
  return n;
}

void calcForCell(MapLayerIllumination &layer,
                 const MapLayerAbsorption &absorptions, Coordinates &src,
                 Coordinates p) {

  // already calculated
  if (layer.getIllumination(p) != ILLUMINATION_DEFAULT) {
    return;
  }

  const auto dim = layer.getDimension();

  auto neighs = getNeighbours(dim, p);

  Coordinates best_n;
  auto best_sm_pn = -1;
  auto best_d_pn = 0;

  auto v_psrc = getVector(p, src);
  // get closest element to the src or src
  for (const auto &n : neighs) {
    auto v_pn = getVector(p, n);
    auto sc_pn = getScalarMultiplication(v_psrc, v_pn);
    auto d_pn = getDistanceSquare(v_pn);
    // find closest point if choosing between diagonal and adjacent
    if (sc_pn > best_sm_pn || (sc_pn == best_sm_pn && d_pn < best_d_pn)) {
      best_n = n;
      best_sm_pn = sc_pn;
      best_d_pn = d_pn;
    }
  }

  Illumination max_n_illum;
  {
    max_n_illum = layer.getIllumination(best_n);

    if (max_n_illum == ILLUMINATION_DEFAULT) {
      calcForCell(layer, absorptions, src, best_n);
    }

    max_n_illum = layer.getIllumination(best_n);
  }

  layer.setIllumination(
      p, max_n_illum.getActualIllumination(absorptions.getAbsorption(p)));
}

MapLayerIllumination
calcForLightSrc(Dimension dim, const MapLayerAbsorption &absorptionLayer,
                std::pair<Coordinates, const Subject::LightSourceAlt *> src) {

  // to check whether calculated for this cell
  MapLayerIllumination res(dim, ILLUMINATION_DEFAULT);

  {
    auto abs = absorptionLayer.getAbsorption(src.first);
    auto illum = src.second->getCurLightParams()
                     .rawIllumination.getActualIllumination(abs);
    res.setIllumination(src.first, illum);
  }

  Coordinates p;
  for (p.x = 0; p.x < dim.width; ++p.x) {
    for (p.y = 0; p.y < dim.height; ++p.x) {
      calcForCell(res, absorptionLayer, src.first, p);
    }
  }

  return res;
}

void MapLayerIllumination::update(const MapLayerAbsorption &absorptionLayer,
                                  const MapLayerSubject &subjectLayer) {

  const auto &srcs = subjectLayer.getActiveLightSources();
  auto dim = getDimension();

  for (const auto &src : srcs) {
    auto res = calcForLightSrc(dim, absorptionLayer, src);

    // illumination for each source is managed like simple addition
    Coordinates c;
    for (c.x = 0; c.x < dim.width; ++c.x) {
      for (c.y = 0; c.y < dim.height; ++c.y) {
        setIllumination(c, getIllumination(c) + res.getIllumination(c));
      }
    }
  }
}
