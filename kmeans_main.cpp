#include <sstream>
#include "kmeans.hpp"
#include "read.hpp"

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

    run( k, p, c );            // run the algorithm on the data
    
    ofstream pfile( "points" );
    print_points( pfile, p );
    pfile.close();
    ofstream cfile( "centroids" );
    print_centroids( cfile, c );
    cfile.close();
    //print_centroids( c );
  }
}
