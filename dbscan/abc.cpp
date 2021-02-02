#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>

// just for output
#include <iostream>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

int main() {
    typedef bg::model::point<double, 2, bg::cs::cartesian> point;
    typedef std::pair<point, int> value;
    typedef bgi::rtree<value, bgi::linear<16> > rtree_t;

    // create the rtree using default constructor
    rtree_t rtree;

    // create some values
    int i = 1;
    for (double f = 0; f < 10; f += 1) {
        // insert new value
        rtree.insert({point(f, f), i++});
    }

    // query point
    point pt(5, 5);

    // iterate over nearest Values
    for (rtree_t::const_query_iterator
             it = rtree.qbegin(bgi::nearest(pt, 100));
         it != rtree.qend();
         ++it) {
        double d = bg::distance(pt, it->first);

        std::cout << bg::wkt(it->first) << ", distance= " << d << std::endl;

        // break if the distance is too big
        if (d > 2) {
            std::cout << "break!" << std::endl;
            break;
        }
    }

    return 0;
}