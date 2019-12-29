#include <limits>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <sstream>
#include <iostream>
#include "kmeans.hpp"

using std::vector;
using std::map;
using std::unordered_map;
using std::string;
using std::stringstream;
using std::ostream;
using std::setw;
using std::cout;
using std::endl;
using std::sqrt;
using std::pow;

namespace kmeans {

  void copy_centroids ( centroids_t& b, centroids_t& c )
  {
    c.clear();
    for( auto& i : b ) {
      c.emplace_back( i.name, i.x, i.y );
    }
  }

  void print_points ( points_t& p ) {
    for( auto& i : p  ) {
      cout << "(" << i.name << ": " << i.x << ", " << i.y << ", " <<
        "i=" << i.index << ", " <<
        "d=" << i.distance << 
        "), ";
    }
    cout << endl;
  }

  void print_summary ( ostream& o, centroids_t& s, points_t&p ) {
    map< centroid, points_t, centroid_compare_function > m;
    //unordered_map< centroid, points_t, centroid_hash_function > m;
    for( auto& i : p ) {
      // int index;
      // stringstream name( i.index );
      // name >> index;
      int index = std::atoi( i.index.c_str() );
      centroid& t = s[ index ];
      m[ t ].push_back( i );
      // m[ t ].emplace_back( i );
    }
    o << "Centroid Summary:" << endl;
    for( auto& j : m ) {
      const centroid& c = j.first;
      const points_t& p = j.second;
      o << setw(2) << c.name << ": (" << setw(9) << c.x << ", "
        << setw(9) << c.y << ") has " << setw(5) << p.size() << " members" << endl;
    }
    o << endl;
    // for( auto& c : s ) {
    //   o << setw(2) << c.name << ": (" << setw(9) << c.x << ", "
    //     << setw(9) << c.y << ") has " << setw(5) << c.points.size() << " members" << endl;
    // }
    // o << endl;
  }

  void print_points ( ostream& o, points_t&p ) {
    for( auto& i : p  ) {
      o << i.name << " " << i.x << " " << i.y << " "
        << i.index << " " << i.distance << endl;
    }
  }

  void sort_centroids ( centroids_t& c )
  {
    sort( c.begin(), c.end(), []( const centroid& a, const centroid& b ) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
      } );
  }

  void clear_centroid_points ( centroids_t& c )
  {
    for( auto& i : c ) {
      i.points.clear();
    }
  }

  void print_centroids ( centroids_t& c ) {
    cout << "Centroids: ";
    for( auto& i : c  ) {
      cout << "(" ;
      if( !i.name.empty() )
        cout << i.name << ": ";
      cout << i.x << ", " << i.y << "), ";
    }
    cout << endl;
  }

  void print_centroids ( ostream& o, centroids_t& c ) {
    for( auto& i : c  ) {
      o << i.name << " " << i.x << " " << i.y << endl;
    }
  }

  void initialize ( points_t& p, centroids_t& c, unsigned int k )
  {
    std::random_device rd;
    std::mt19937 gen( rd() );

    long
      xl = std::numeric_limits<long>::max(),
      xh = std::numeric_limits<long>::min(),
      yl = std::numeric_limits<long>::max(),
      yh = std::numeric_limits<long>::min();
    for( auto& i : p ) {
      xl = std::min( i.x, xl );
      xh = std::max( i.x, xh );
      yl = std::min( i.y, yl );
      yh = std::max( i.y, yh );
    }
    std::uniform_int_distribution<> xdis( xl, xh );
    std::uniform_int_distribution<> ydis( yl, yh );
    for( unsigned int i = 0; i < k; ++i ) {
      // stringstream name;
      // name << i;
      // c.push_back( { name.str(), xdis( gen ), ydis( gen ) } );
      string name = std::to_string( i );
      c.push_back( { name, xdis( gen ), ydis( gen ) } );
    }
  }

  void assignment ( points_t& p, centroids_t& c )
  {
    clear_centroid_points( c );
    for( auto& i : p ) {
      vector< float > dist;
      for( auto &j : c ) {
        float distance = sqrt( pow( i.x - j.x, 2 ) + pow( i.y - j.y, 2 ) );
        dist.push_back( distance );
      }
      float m = std::numeric_limits<float>::max();
      long  l = std::numeric_limits<long>::max();
      for( unsigned long k = 0; k < c.size(); ++k ) {
        if( dist[k] < m ) {
          m = dist[k];
          l = k;
        }
      }
      if( l != std::numeric_limits<long>::max() ) {
        // l is the index pointing to the nearest centroid for a given point
        i.index = c[l].name;
        // m is the distance to the nearest centroid for a given point
        i.distance = m;
        // add this point to the centroid list of points
        c[l].points.push_back( i );
      }
    }
  }

  void update ( centroids_t& c, unsigned int limit )
  {
    for( auto& i : c ) {
      long xs = 0;
      long ys = 0;
      for( auto& j : i.points ) {
        xs += j.x;
        ys += j.y;
      }
      if( i.points.size() > 0 ) {
        long x = (long) (xs / (float) i.points.size() );
        long y = (long) (ys / (float) i.points.size() );
        i.x = x; i.y = y;       // update centroid location with average
      }
    }
  }

  bool compare_centroids ( centroids_t& a, centroids_t& b )
  {
    bool compare = true;
    for( unsigned long i = 0; i < a.size(); ++i )
      if( a[i].x != b[i].x || a[i].y != b[i].y )
        compare = false;
    return compare;
  }

  unsigned int run ( unsigned int k, points_t& p, centroids_t& c )
  {
    unsigned int max_iter = 100;
    unsigned int limit = 4096;
  
    centroids_t t;
    initialize( p, c, k );
    //print_centroids( c ) ;
    //sort_centroids( c );
    assignment( p, c );
    unsigned int iter = 0;
    while( iter < max_iter ) {
      copy_centroids( c, t );   // copy c to t
      update( c, limit );
      assignment( p, c );
      iter++;
      // if centroids have stopped moving (compares to saved t)
      if( compare_centroids( c, t ) )
        break;
    }
    //cout << "iter=" << iter << endl;
    return iter;
  }
}
