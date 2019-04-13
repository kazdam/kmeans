#include <iostream>
#include <fstream>
#include <sstream>
#include "kmeans.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using std::getline;
using std::stringstream;

namespace kmeans {
  int read_points( string filename, points_t& p )
  {
    ifstream infile( filename.c_str() );
    if( !infile.is_open() ) {
      cout << "ERROR: failed to open file " << filename << endl;
      return -1;
    }

    int n = 0;
    string line;
    while( getline( infile, line ) ) {
      stringstream is( line );
      string name;
      long x, y;
      is >> name;
      is >> x;
      is >> y;
      p.push_back( point( name, x, y ) );
      ++n;
    }
    infile.close();

    return n;
  }
}
