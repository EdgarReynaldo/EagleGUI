
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file LineEquations.hpp
 * @brief A simple class for working with general first degree line equations
 */

#ifndef LineEquations_HPP
#define LineEquations_HPP



#include "Eagle/Position.hpp"


/**! @class GLE2D
 *   @brief The basic general first degree equation of a line, represented by an object
 */

class GLE2D {
   /**! @var a @var b @var c
    *   @brief Ax + By + C = 0
    */
   double a;
   double b;
   double c;
   
   Pos2D spos;///< Start position if line segment
   Pos2D epos;///< End position if line segment
   
   void SetABC();

public :
   GLE2D(Pos2D p , double theta_rad);///< Define a line using a point and an angle
   GLE2D(Pos2D p1 , Pos2D p2);///< Define a line using two points
   
   
   double A() const {return a;}///< Get the coefficient on X
   double B() const {return b;}///< Get the coefficient on Y
   double C() const {return c;}///< Get the constant C
   
   Pos2D SPos() const {return spos;}///< Get the starting position
   Pos2D EPos() const {return epos;}///< Get the ending position
};

/**! @fn IntersectionPoint <const GLE2D& , const GLE2D& , Pos2D*>
 *   @brief Determines if there is an intersection between the two lines
 *          and stores that point in @param i
 */

bool IntersectionPoint(const GLE2D& la , const GLE2D& lb , Pos2D* i);

/**! @fn DistanceFromLine <Pos2D , GLE2D>
 *   @brief Finds the distance from point @param p to line @param l
 */

double DistanceFromLine(Pos2D p , GLE2D l);



#endif // LineEquations_HPP



