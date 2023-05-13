#include "cws/map_layer/illumination.hpp"
#include <algorithm>
#include <list>
#include <vector>

struct illum_source_xy {
  int x, y;
  int brightness;
};

std::pair<int, int> vector(std::pair<int, int> p1, std::pair<int, int> p2) {
  return std::make_pair(p2.first - p1.first, p2.second - p1.second);
}

int scalar_multiplication(std::pair<int, int> v1, std::pair<int, int> v2) {
  return v1.first * v2.first + v1.second * v2.second;
}

std::list<std::pair<int, int>> get_neighbours(std::pair<int, int> dim,
                                              std::pair<int, int> p) {

  std::list<std::pair<int, int>> n;

  for (int x = p.first - 1; x <= p.first + 1; ++x) {
    for (int y = p.second - 1; y <= p.second + 1; ++y) {
      if (x < 0 || x >= dim.first || y < 0 || y >= dim.second ||
          (x == p.first && y == p.second)) {
        continue;
      }
      n.emplace_back(x, y);
    }
  }

  return n;
}

int get_distance_square(std::pair<int, int> v) {
  return v.first * v.first + v.second * v.second;
}

template<typename T>
std::pair<int, int> get_dimension(const std::vector<std::vector<T>> & map) {
  return std::make_pair(map.size(), map.empty() ? 0 : map[0].size());
}

int get_next_brightness(int n_brightness, double n_absorption) {
  return static_cast<double>(n_brightness) * (1 - n_absorption);
}

void calc_cell_illum(const std::pair<int, int> & dim,
                     const std::vector<std::vector<double>> & absorptions,
                     std::vector<std::vector<int>> & layer,
                     const illum_source_xy & src_s, std::pair<int, int> p) {

  // if already counted
  if (layer[p.first][p.second] != -1) {
    return;
  }

  auto src = std::make_pair(src_s.x, src_s.y);

  auto neighs = get_neighbours(dim, p);

  std::pair<int, int> max_n;
  auto max_sc_pn = -1;
  auto max_d_pn = 0;

  auto v_psrc = vector(p, src);
  // get closest element to the src or src
  for (const auto & n : neighs) {
    auto v_pn = vector(p, n);
    auto sc_pn = scalar_multiplication(v_psrc, v_pn);
    auto d_pn = get_distance_square(v_pn);
    // find closest point if choosing between diagonal and adjacent
    if (sc_pn > max_sc_pn || (sc_pn == max_sc_pn && d_pn < max_d_pn)) {
      max_n = n;
      max_sc_pn = sc_pn;
      max_d_pn = d_pn;
    }
  }

  auto & n_brightness = layer[max_n.first][max_n.second];

  if (n_brightness == -1) {
    calc_cell_illum(dim, absorptions, layer, src_s, max_n);
  }

  layer[p.first][p.second] =
      get_next_brightness(n_brightness, absorptions[max_n.first][max_n.second]);
}

std::vector<std::vector<int>>
calc_illumination_src(const std::vector<std::vector<double>> & absorptions,
                      const illum_source_xy & src) {
  auto dim = get_dimension(absorptions);
  auto res = std::vector<std::vector<int>>(dim.first, std::vector<int>(dim.second, -1));

  res[src.x][src.y] = src.brightness;

  for (int x = 0; x < dim.first; ++x) {
    for (int y = 0; y < dim.second; ++y) {
      calc_cell_illum(dim, absorptions, res, src, std::make_pair(x, y));
    }
  }

  return res;
}

std::vector<std::vector<int>>
calc_illumination(const std::vector<std::vector<double>> & absorptions,
                  const std::vector<illum_source_xy> & sources) {

  auto dim = get_dimension(absorptions);
  auto res = std::vector<std::vector<int>>(dim.first, std::vector<int>(dim.second, 0));

  for (const auto & src : sources) {
    auto res_src = calc_illumination_src(absorptions, src);
    for (int x = 0; x < dim.first; ++x) {
      for (int y = 0; y < dim.second; ++y) {
        res[x][y] += res_src[x][y];
      }
    }
  }

  return res;
}

void MapLayerIllumination::setState(const MapLayerAbsorption & absorptionLayer,
                                    const MapLayerSubject & subjectLayer) {

}
