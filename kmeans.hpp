#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <limits>
#include <string>
#include <vector>
#include <set>
#include <fstream>

using std::vector;
using std::set;
using std::string;
using std::ostream;

namespace kmeans {

  struct base_point {
    string name;
    long x;
    long y;
  };

  struct point : base_point {
    string index;                 // name of the closest centroid
    float  distance;              // distance to the closest centroid

    point( string& _name, long _x, long _y ) : base_point{ _name, _x, _y } {
      index = "";
      distance = std::numeric_limits<float>::max();
    }
    point( string _name, long _x, long _y, string& _index, float _distance ) : base_point{ _name, _x, _y }, index(_index), distance(_distance) {}
    point( const point& p ) : base_point{ p.name, p.x, p.y }, index(p.index), distance(p.distance) {}
  };

  typedef vector< point > points_t;

  struct centroid : base_point {
    points_t points;
    
    centroid( string _name, long _x, long _y ) : base_point{ _name, _x, _y } {}
    centroid( const centroid& c ) : base_point{ c.name, c.x, c.y } {}
    centroid& operator= ( const centroid& other ) = default;
    // bool operator() ( const centroid& lhs, const centroid& rhs ) const {
    //   return( lhs.name.compare( rhs.name ) == 0 );
    // }
    int operator< ( const centroid& rhs ) const {
      return name.compare( rhs.name ) < 0;
      // return std::lexicographical_compare( name.begin(), name.end(),
      //                                      rhs.name.begin(), rhs.name.end() );
    }
  };

  struct centroid_hash_function {
    size_t operator() ( const centroid& c ) const {
      return c.name.length();
    }
  };

  struct centroid_compare_function {
    bool operator() ( const centroid& lhs, const centroid& rhs ) const {
      int l = std::atoi( lhs.name.c_str() );
      int r = std::atoi( rhs.name.c_str() );
      return l < r;
      //return lhs.name.compare( rhs.name ) < 0;
    }
  };
  
//  typedef set< centroid > centroids_t;
  typedef vector< centroid > centroids_t;

  void sort_centroids    ( centroids_t& c );
  void copy_centroids    ( centroids_t& b, centroids_t& c );
  bool compare_centroids ( centroids_t& a, centroids_t& b );

  void print_centroids   ( centroids_t& c );
  void print_points      ( points_t& p ) ;
  void print_centroids   ( ostream& o, centroids_t& c );
  void print_points      ( ostream& o, points_t& p ) ;
  void print_summary     ( ostream& o, centroids_t& s, points_t&p );

  void initialize ( points_t& p, centroids_t& c, unsigned int k );
  void assignment ( points_t& p, centroids_t& c );
  void update     ( points_t& p, centroids_t& c );

  unsigned int run ( unsigned int k, points_t& p, centroids_t& c );
}

#endif
