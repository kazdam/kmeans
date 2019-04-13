#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <limits>
#include <string>
#include <vector>
#include <fstream>

using std::vector;
using std::string;
using std::ofstream;

namespace kmeans {

  struct base_point {
    string name;
    long x;
    long y;
  };

  struct point : public base_point {
    string index;                 // name of the closest centroid
    float  distance;              // distance to the closest centroid

    point( string& _name, long _x, long _y ) : base_point{ _name, _x, _y } {
      index = "";
      distance = std::numeric_limits<float>::max();
    }
    point( string _name, long _x, long _y, string& _index, float _distance ) : base_point{ _name, _x, _y }, index(_index), distance(_distance) {}
  };

  struct centroid : public base_point {
    centroid( string _name, long _x, long _y ) : base_point{ _name, _x, _y } {}
  };

  typedef vector< point > points_t;
  typedef vector< centroid > centroids_t;

  void sort_centroids    ( centroids_t& c );
  void copy_centroids    ( centroids_t& b, centroids_t& c );
  bool compare_centroids ( centroids_t& a, centroids_t& b );

  void print_centroids   ( centroids_t& c );
  void print_points      ( points_t& p ) ;
  void print_centroids   ( ofstream& o, centroids_t& c );
  void print_points      ( ofstream& o, points_t& p ) ;

  void initialize ( points_t& p, centroids_t& c, unsigned int k );
  void assignment ( points_t& p, centroids_t& c );
  void update     ( points_t& p, centroids_t& c );

  void run ( unsigned int k, points_t& p, centroids_t& c );
}

#endif
