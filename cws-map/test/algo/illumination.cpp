#include <algorithm>
#include <iomanip>
#include <iostream>
#include <list>
#include <vector>

struct illum_params {
  double absorption;
  int cur_illumination;
};

struct illum_source {
  int brightness;
};

struct plain_subject {
  struct illum_params params;
};

struct source_subject : plain_subject {
  struct illum_source source;
};

struct cell {
  std::vector<plain_subject *> subjects;
};

/*
 * Illimination absorption is calculated like: sort elements from max to min by
 * absorption and calculated subtracted multiplication
 *
 * ex: sub1 = 0.5, sub2 = 0.3, sub3 = 0.6
 *
 * sorted => 0.6, 0.5, 0.3
 *
 * absorption:
 *  (1 -   1   * 0.6) = 0.4
 *  (0.4 - 0.4 * 0.5) = 0.2
 *  (0.2 - 0.2 * 0.3) = 0.14
 *
 *  result: 1 - 0.14 = 0.86
 *
 *  so to calculate:
 *  1) find count of elements in cell
 *  2) get array of subject absorption
 *  3) sort this array
 *  4) calculate
 */
double cell_absorption(const std::list<double> & subjects) {
  double res = 1;

  std::vector<double> absorptions;
  for (const auto & subject : subjects) {
    absorptions.push_back(subject);
  }

  std::sort(absorptions.begin(), absorptions.end(),
            [](const double lsv, const double rsv) -> bool { return lsv > rsv; });

  for (const auto & ab : absorptions) {
    res = res - res * ab;
  }

  return 1 - res;
}

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

int get_brightness(int n_brightness, double n_absorption) {
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
      get_brightness(n_brightness, absorptions[p.first][p.second]);
}

std::vector<std::vector<int>>
calc_illumination_src(const std::vector<std::vector<double>> & absorptions,
                      const illum_source_xy & src) {
  auto dim = get_dimension(absorptions);
  auto res = std::vector<std::vector<int>>(dim.first, std::vector<int>(dim.second, -1));

  res[src.x][src.y] = get_brightness(src.brightness, absorptions[src.x][src.y]);

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

int main() {
  // std::list<double> subjects{0.6, 0.5, 0.3};

  // std::cout << cell_absorption(subjects) << std::endl;

  std::vector<std::vector<double>> abs_layer{
      {0.2, 0.2, 0.4, 0.1},
      {0.2, 0.2, 0.0, 0.1},
      {0.2, 0.2, 0.0, 0.1},
      {0.2, 0.0, 0.4, 0.5},
  };

  std::vector<illum_source_xy> srcs;
  // srcs.push_back((illum_source_xy){.x = 0, .y = 0, .brightness = 350});
  srcs.push_back((illum_source_xy){.x = 1, .y = 1, .brightness = 200});
  // srcs.push_back((illum_source_xy){.x = 2, .y = 2, .brightness = 200});
  // srcs.push_back((illum_source_xy){.x = 3, .y = 3, .brightness = 400});

  auto res = calc_illumination(abs_layer, srcs);

  for (const auto & line : res) {
    for (const auto & e : line) {
      std::cout << std::setw(6) << e;
    }
    std::cout << std::endl;
  }
}
