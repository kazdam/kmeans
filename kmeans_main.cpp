#include <iostream>
#include <sstream>
#include "kmeans.hpp"
#include "read.hpp"

using std::cout;
using std::endl;
using std::ofstream;
using namespace kmeans;

int main( int intc, char** argv )
{
  points_t p;                   // data points
  centroids_t c;                // retured centroids (points)
  string filename;              // filename to read points from
  unsigned int  k;              // how many centroids to build
  if( intc == 3 ) {
    read_points( argv[1], p );
    std::stringstream is( argv[2] );
    is >> k;

    unsigned int iter = run( k, p, c );  // run the algorithm on the data
    cout << "kmeans took " << iter << " iterations" << endl;
                                                       
    // file generatin of data
    ofstream pfile( "points" );
    print_points( pfile, p );
    pfile.close();
    ofstream cfile( "centroids" );
    print_centroids( cfile, c );
    cfile.close();

    // report of points
    cout << "Points: (" << p.size() << ")" << endl;
    if( p.size() < 40 ) {
      print_points( cout, p );
    }
    cout << endl;
    
    // report of centroids
    // cout << "Centroids: (" << c.size() << ")" << endl;
    // print_centroids( cout, c );
    // cout << endl;
    
    print_summary( cout, c, p );
  }
}
