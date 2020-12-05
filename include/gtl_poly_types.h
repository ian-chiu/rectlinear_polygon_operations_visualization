#ifndef GTL_POLY_TYPES_HH
#define GTL_POLY_TYPES_HH

#include <boost/polygon/polygon.hpp>
namespace gtl = boost::polygon;

typedef gtl::polygon_90_with_holes_data<int>           Polygon_Holes;
typedef gtl::polygon_90_data<int>                      Polygon_NoHoles;
typedef gtl::rectangle_data<int>                       Rect;
typedef gtl::polygon_traits<Polygon_Holes>::point_type Point;
typedef std::vector<Polygon_Holes>                     PolygonSet;
typedef std::vector<Polygon_NoHoles>                   PolygonSet_NoHoles;

#endif