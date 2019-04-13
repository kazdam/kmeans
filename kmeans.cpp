#include <limits>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <sstream>
#include <iostream>
#include "kmeans.hpp"

using std::vector;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;

namespace kmeans {

  void copy_centroids ( centroids_t& b, centroids_t& c )
  {
    c.clear();
    for( auto& i : b ) {
      c.push_back( centroid{ i.name, i.x, i.y } );
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

  void print_points ( ofstream& o, points_t&p ) {
    for( auto& i : p  ) {
      o << i.name << " " << i.x << " " << i.y << " " << " " << i.index << endl;
    }
    cout << endl;
  }

  void sort_centroids ( centroids_t& c )
  {
    sort( c.begin(), c.end(), []( const centroid& a, const centroid& b ) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
      } );
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

  void print_centroids ( ofstream& o, centroids_t& c ) {
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
    cout << "xl=" << xl << " xh=" << xh <<
      " yl=" << yl << " yh=" << yh << endl;
    std::uniform_int_distribution<> xdis( xl, xh );
    std::uniform_int_distribution<> ydis( yl, yh );
    for( unsigned int i = 0; i < k; ++i ) {
      stringstream name;
      name << i;
      c.push_back( { name.str(), xdis( gen ), ydis( gen ) } );
    }
  }

  void assignment ( points_t& p, centroids_t& c )
  {
    sort_centroids( c );
    for( auto& i : p ) {
      vector< float > dist;
      for( auto &j : c ) {
        float distance =
          std::sqrt( std::pow( i.x - j.x, 2 ) +
                     std::pow( i.y - j.y, 2 ) );
        dist.push_back( distance );
      }
      float m = std::numeric_limits<float>::max();
      long   l = std::numeric_limits<long>::max();
      for( unsigned long k = 0; k < c.size(); ++k ) {
        if( dist[k] < m ) {
          m = dist[k];
          l = k;
        }
      }
      // l is the index pointing to the nearest centroid for a given point
      i.index = c[l].name;
      // m is the distance to the nearest centroid for a given point
      i.distance = m;
    }
  }

  void update ( points_t& p, centroids_t& c )
  {
    for( unsigned long i = 0; i < c.size(); ++i ) {
      long xs = 0;
      long ys = 0;
      int n =  0;
      for( auto &j : p ) {
        if( j.index == c[i].name ) {
          xs += j.x;
          ys += j.y;
          n++;
        }
      }
      //cout << "centroid: ( " << c[i].x << ", " << c[i].y << " ) --> ";
      if( n != 0 ) {
        long x = (long) (xs / (float) n);
        long y = (long) (ys / (float) n);
        //cout << "( " << x << ", " << y << " )" << endl;
        c[i].x = x; c[i].y = y;
      } else {
        //cout << "( " << c[i].x << ", " << c[i].y << " )" << endl;
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

  void run ( unsigned int k, points_t& p, centroids_t& c )
  {
    int max_iter = 100;
  
    centroids_t temp;
    initialize( p, c, k );
    //print_centroids( c ) ;
    assignment( p, c );
    int iter = 0;
    while( iter < max_iter ) {
      cout << "iter=" << iter << endl;
      copy_centroids( c, temp );
      update( p, c );
      assignment( p, c );
      iter++;
      if( compare_centroids( c, temp ) )
        break;
    }
    cout << "iter=" << iter << endl;
  }
}
