#ifndef READ_H
#define READ_H

#include <string>
#include "kmeans.hpp"

namespace kmeans {
  int read_points( string filename, points_t& p );
}

#endif
