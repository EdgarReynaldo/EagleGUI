



#ifndef Interpolate_HPP
#define Interpolate_HPP


template <class Value>
inline Value Interpolate(const Value& v1 , const Value& v2 , double frac) {
   return v1 + (v2 - v1)*frac;
}



#endif // Interpolate_HPP



