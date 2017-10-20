/*--------------------------------------------------------------------------*\
 |                                                                          |
 |  Copyright (C) 2017                                                      |
 |                                                                          |
 |         , __                 , __                                        |
 |        /|/  \               /|/  \                                       |
 |         | __/ _   ,_         | __/ _   ,_                                |
 |         |   \|/  /  |  |   | |   \|/  /  |  |   |                        |
 |         |(__/|__/   |_/ \_/|/|(__/|__/   |_/ \_/|/                       |
 |                           /|                   /|                        |
 |                           \|                   \|                        |
 |                                                                          |
 |      Enrico Bertolazzi                                                   |
 |      Dipartimento di Ingegneria Industriale                              |
 |      Universita` degli Studi di Trento                                   |
 |      email: enrico.bertolazzi@unitn.it                                   |
 |                                                                          |
\*--------------------------------------------------------------------------*/

#include "PolynomialRoots.hh"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>

namespace PolynomialRoots {

  using std::abs ;
  static valueType const machepsi = std::numeric_limits<valueType>::epsilon() ;
  static valueType const sqrtepsi = std::sqrt(machepsi) ;

  /*\
   *  Calculate the zeros of the quadratic a*z^2 + b*z + c.
   *  The quadratic formula, modified to avoid overflow, is used
   *  to find the larger zero if the zeros are real and both
   *  are complex. The smaller real zero is found directly from
   *  the product of the zeros c/a.
  \*/

  void
  Quadratic::findRoots() {
    r0 = r1 = 0 ;
    nrts = 0 ;
    cplx = dblx = false ;

    if ( a == 0 ) { // less than two roots b*z + c = 0
      if ( b != 0 ) { nrts = 1 ; r0 = -c/b ; }
    } else if ( c == 0 ) { // a*z^2 + b*z  = 0
      nrts = 2 ;
      dblx = b == 0 ;
      if ( !dblx ) {
        r0 = -b/a ;
        if ( r0 < 0 ) std::swap(r0,r1) ;
      }
    } else { // Compute discriminant avoiding overflow.
      valueType hb    = b/2 ; // b now b/2
      valueType abs_b = abs(hb) ;
      valueType abs_c = abs(c) ;
      valueType e, d ;
      if ( abs_b < abs_c ) {
        e = c < 0 ? -a : a ;
        e = hb*(hb/abs_c) - e ;
        d = sqrt(abs(e))*sqrt(abs_c);
      } else {
        e = 1 - (a/hb)*(c/hb) ;
        d = sqrt(abs(e))*abs_b ;
      }
      nrts = 2 ;
      cplx = e < 0 ;
      if ( cplx ) {
        // complex conjugate zeros
        r0 = -hb/a ;         // real part
        r1 = std::abs(d/a) ; // immaginary part
      } else {
        // real zeros
        dblx = d == 0 ;
        if ( dblx ) {
          r0 = r1 = -hb/a;;
        } else {
          if ( hb >= 0 ) d = -d ;
          r0 = (d-hb)/a;
          //r1 = (-d-hb)/a;
          if ( r0 != 0 ) r1 = (c/r0)/a ;
          if ( r0 > r1 ) std::swap(r0,r1) ; // order roots
        }
      }
    }
  }

  void
  Quadratic::info( std::ostream & s ) const {
    s << "\npoly a=" << a << " b=" << b << " c=" << c
      << "\nn. roots = " << nrts
      << "\ncomplex  = " << (cplx?"YES":"NO")
      << "\ndouble   = " << (dblx?"YES":"NO") ;
    if ( cplx ) {
      s << "\nx0 = (" << r0 << "," << r1 << ")"
        << "\nx1 = (" << r0 << "," << -r1 << ")" ;
    } else if ( dblx ) {
      s << "\nx0 = x1 = " << r0 ;
    } else if ( nrts == 1 ) {
      s << "\nx0 = " << r0 ;
    } else if ( nrts == 2 ) {
      s << "\nx0 = " << r0
        << "\nx1 = " << r1 ;
    }
    s << "\n" ;
  }

  bool
  Quadratic::check( std::ostream & s ) const {
    bool ok = true ;
    valueType epsi = 10*hypot(a,b)*machepsi ;
    if ( cplx ) {
      valueType z0 = std::abs(eval( root0() ));
      valueType z1 = std::abs(eval( root1() ));
      s << "|p(r0)| = " << z0
        << "\n|p(r1)| = " << z1
        << "\n" ;
      ok = z0 < epsi && z1 < epsi ;
    } else if ( nrts == 1 ) {
      valueType z0 = eval( real_root0() );
      s << "p(r0) = " << z0  << "\n" ;
      ok = std::abs(z0) < epsi ;
    } else if ( nrts == 2 ) {
      valueType z0 = eval( real_root0() );
      valueType z1 = eval( real_root1() );
      s << "p(r0) = " << z0
        << "\np(r1) = " << z1
        << "\n" ;
      ok = std::abs(z0) < epsi && std::abs(z1) < epsi  ;
    }
    return ok ;
  }

}

// EOF: PolynomialRoots-Quadratic.cc
