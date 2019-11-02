
#ifndef DEBUG
#define DEBUG 0
#endif


#ifndef POINTS_CPP
#define POINTS_CPP

unsigned int MAX_POINTS = 512;

#include <cstring>
#include <memory>
#include <iostream>
#include <sstream>
#include <algorithm> // for copy() and copy_n() 
#include <typeinfo>
#include <cassert> // for assert()

#ifndef INT_NaN
#define INT_NaN -1U
#endif


template <class Coord, unsigned int n_coords>
class Point {
private:
  Coord _coords [n_coords] = { 0 };
public:
  Point () {
#if DEBUG
    std::cout << "Constructor Point<" << typeid(Coord).name() << ", " 
        << n_coords << "> (void)" << std::endl;
    report();
#endif
  };
  
  template<class ...Coord2
      , typename std::enable_if<sizeof...(Coord2) == n_coords, int>::type = 0>
  Point (Coord2 ...coords) {
#if DEBUG
    std::cout << "Constructor Point<" << typeid(Coord).name() << ", " 
        << n_coords << "> (...coords)" << std::endl;
#endif
    int i = 0;
    (...,void(_coords[i++] = coords));
#if DEBUG
    report();
#endif
  };

  
  template <class Coord2, unsigned int n_coords2>
  Point (Point<Coord2, n_coords2> point) {
#if DEBUG
    std::cout << "Constructor Point<" << typeid(Coord).name() << ", " 
        << n_coords << "> (Point<" << typeid(Coord2).name() << ", " 
        << n_coords2 << "> point)" << std::endl;
#endif
    int n = n_coords < n_coords2 ? n_coords : n_coords2;
    for (int i=0; i< n; i++)
      _coords[i] = Coord(point[i]);
#if DEBUG
    report();
#endif
  };
  
  Coord& operator[] (int index) {
    //assert(index >= -(int)n_coords && index < 2 * n_coords);
    if (-(int)n_coords > index || index >= (int)n_coords) {
      std::stringstream ss;
      ss << "Point<" << typeid(Coord).name() << ", " << n_coords 
          << ">[" << index << "]: out of range, expected " 
          << -(int)n_coords << " <= index < " << n_coords;
      throw std::out_of_range(ss.str());
    }
    return _coords[index];
  };
  
  int coord_count() {
    return n_coords;
  };
  
  int length () {
    return 1;
  };
  
  void report() {
    std::cout << "This Point"
        << "<" << typeid(Coord).name() << ", " << n_coords
        << "> with a size of " << sizeof(Point)
        << "(" << length() 
        << " points x " << coord_count()
        << " coordinates x " << sizeof(Coord)
        << " bytes) => {" << _coords[0];
    for (int i=1; i<n_coords; i++)
        std::cout << ", " << _coords[i];
    std::cout << "}" << std::endl;
  };

  friend std::ostream& operator<<(std::ostream& out, const Point &list) {
    out << "Point({" << list._coords[0];
    for (int i=1; i<n_coords; i++)
      out << ", " << list._coords[i];
    out << "})";
    return out; // return std::ostream so we can chain calls to operator<<
  };
};


//TODO: make constructors for bulk data
template <class Coord, unsigned int n_coords>
class PointSequence {
private:
  unsigned int _n_points = 0;
  unsigned int _max_points = 0;
  Point<Coord, n_coords> * _p_points = NULL;
  
  Point<Coord, n_coords> * _points_allocate (unsigned int max_points) {
    return (Point<Coord, n_coords> *)
        std::malloc(sizeof(Coord) * n_coords * max_points);
  };
  
  Point<Coord, n_coords> * _points_deallocate (Point<Coord, n_coords> * p_points) {
    if (p_points != NULL) {
#if DEBUG
      std::cout << "destructing PointSequence<" << typeid(Coord).name() 
          << ", " << n_coords << ">" << std::endl;
#endif
      std::free(p_points);
    } else {
#if DEBUG
      std::cout << "ignoring second destructing PointSequence<"
          << typeid(Coord).name() << ", " << n_coords << ">" << std::endl;
#endif
    }
    return NULL;
  };
  
public:
  PointSequence (unsigned int max_points=MAX_POINTS) {
    _p_points = _points_allocate(max_points);
    _max_points = max_points;
    _n_points = 0;
  };

  PointSequence(std::initializer_list<std::initializer_list<Coord>> points) {
    int n_points = points.size();
    
    _max_points = n_points;
    _n_points = 0;
    _p_points = _points_allocate(n_points);
    
    int i, n;
    for (auto &p: points) {
      i = 0, n = p.size();
      if (n > n_coords)
          n = n_coords;
      //std::cout << "min(p.size(), n_coords) => " << n << std::endl;
      for (auto &c: p) {
        if (i >= n)
            break;
        _p_points[_n_points][i++] = c;
      }
      
      _n_points++;
    }
  };
  
  template<typename Coord2, unsigned int n_flat_array>
  PointSequence(Coord2 (&flat_array)[n_flat_array]
      , unsigned int max_points=INT_NaN, unsigned int max_coords=n_coords) {
    int n_points = n_flat_array / max_coords;
#if DEBUG
    std::cout << "n_flat_array => " << n_flat_array << std::endl;
    std::cout << "n_points => " << n_points << std::endl;
    std::cout << "sizeof(Coord2) => " << sizeof(Coord2) << std::endl;
#endif
    
    _max_points = (max_points == INT_NaN) ? n_points : max_points;
    _n_points = 0;
    _p_points = _points_allocate(n_points);
    for (int i=0; i < n_flat_array; _n_points++)
        for (int j=0; j < max_coords; j++, i++)
            _p_points[_n_points][j] = flat_array[i];
  };
  
  ~PointSequence () {
    _p_points = _points_deallocate(_p_points);
  }
  
  Point<Coord, n_coords>& operator[] (int index) {
    if (-(int)_n_points > index || index >= (int)_n_points) {
      std::stringstream ss;
      ss << "PointSequence[" << index << "]: out of range, expected " 
          << -(int)_n_points << " <= index < " << _n_points;
      throw std::out_of_range(ss.str());
    }
    if (index < 0) index += (int)_n_points;
    return _p_points[index];
  };
  
  unsigned int max_length () {
    return _max_points;
  }
  
  unsigned int length () {
    return _n_points;
  }
  
  // move to MutablePointSequence
  template <class Coord2, unsigned int n_coords2>
  PointSequence<Coord, n_coords>& append (Point<Coord2, n_coords2>& point) {
    _p_points[_n_points] = point;
    _n_points += 1;
    return *this;
  };
};

#endif //POINTS_CL_CPP
