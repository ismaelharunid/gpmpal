

#ifndef DEBUG
#define DEBUG 0
#endif

#include "points.cpp"


void test_point_constructors() {
  Point<int, 1> p0;
  Point<float, 2> p1{};
  Point<double, 3> p2 = {};
  Point<long double, 4> p3{0.1,1.2,2.3,3.4};
  Point<int, 4> p4 = {0.1,1.2,2.3,3.4};
  Point<float, 4> p5 = p0;
  Point<double, 4> p6 = p1;
  Point<long double, 4> p7 = Point<int, 4>();
  Point<float, 4> p8 = Point<double, 4>(p4);
  Point<float, 4> p9 = Point<float, 4>(1.2,2.3,3.4,4.5);
  Point<float, 4> p10 = Point<float, 2>({3.4,4.5});
};

void test_point_sequence_constructors() {
  PointSequence<float, 2> ps0;
  std::cout << "ps0.length() => " << ps0.length() << std::endl;
  std::cout << "ps0.max_length() => " << ps0.max_length() << std::endl;
  
  Point<float, 2> p0{0.1,2.3}, p1{3.4,5.6}, p2{7.8,9.1}, p3{10.1,12.3};
  ps0.append(p0).append(p1).append(p2).append(p3);
  try{
    std::cout << "ps0[" << ps0.length() << "]" << std::endl;
    for (int i=-2; i<5; i++) {
      std::cout << "ps0[" << i << "] => " << ps0[i] << std::endl;
    }
  } catch (const std::out_of_range& e) {
    std::cout << e.what() << std::endl;
  };
  
  PointSequence<float, 2> ps1{ {10,20}, {30}, {40,50,60}, {} };
  try{
    std::cout << "ps1[" << ps1.length() << "]" << std::endl;
    for (int i=0, n=ps1.length(); i<n; i++) {
      std::cout << "ps1[" << i << "] => " << ps1[i] << std::endl;
    }
  } catch (const std::out_of_range& e) {
    std::cout << e.what() << std::endl;
  };
  
  float flat_array[] = {10,20, 30,40, 50,60};
  PointSequence<float, 2> ps2 = flat_array;
  try{
    std::cout << "ps2[" << ps1.length() << "]" << std::endl;
    for (int i=0, n=ps2.length(); i<n; i++) {
      std::cout << "ps2[" << i << "] => " << ps2[i] << std::endl;
    }
  } catch (const std::out_of_range& e) {
    std::cout << e.what() << std::endl;
  };
};

int main() {
  test_point_constructors();
  test_point_sequence_constructors();
  
};
