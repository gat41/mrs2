/* 
 * Copyright (C) 2008, 2009 Raazesh Sainudiin and Jennifer Harlow
 *
 * This file is part of mrs, a C++ class library for statistical set processing.
 *
 * mrs is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*! \file: Exm_3_3.cpp
  \brief Example 3.3 from Jaulin et al, Springer, 2001, p. 61.

  Implementation of example 3.3 from Jaulin, Kieffer, Didrit and Walter, 
  Applied Interval Analysis, Springer, 2001, p. 61.
*/

#include "Exm_3_3.hpp"

// These AIASubPavings are declared as global
AIASubPaving Sc, Sc4;

// specifications of example interval boolean tests
// The boolean interval test can return BI_TRUE, BI_FALSE, or BI_INDET

AIA_BOOL_INTERVAL IBTAnnular(const ivector& x)
{
  // here we test a 2-d box for inclusion in the area between circles centred 
  // on the origin with radii 1 and 2
  interval ToInvert(1.0,2.0),Temp;
  interval Img = sqr(x[1]) + sqr(x[2]);

  if (!Intersection(Temp,Img,ToInvert)) return BI_FALSE;
  if ( Img<=ToInvert ) return BI_TRUE;

  return BI_INDET;
}

// end specification of example boolean interval tests

// specification of example interval vector function,
// ie, vector inclusion function
// as in Example 3.3 of AIA
ivector IVF_ex3_3(const ivector& x)
{
  // example in 2-d space R2
  // for f: R2 -> R2
  //f1(x1, x2) = x1*x2
  //f2(x1, x2) = x1 + x2

  ivector Img(2);

  Img[1] = x[1] * x[2];
  Img[2] = x[1] + x[2];

  return (Img);
}

// end specification of example interval vector funciions

using namespace cxsc;
using namespace std;

int main()
{
  double prec;
  clock_t start, end;

  ivector x(2);
  x[1] = interval(-3.0,3.0);
  x[2] = interval(-3.0,3.0);

  AIASubPaving A;
  A = new AIASPnode(x);

  // Using SIVIA for set inversion
  // find an AIASubPaving characterisation Sc containing the area between 
  // circles centred on the origin
  // with radii 1 and 2 (in 2 dimensional space)
  cout << "Characterization of the set Sc={(x1,x2) | 1 <= x1^2+x2^2 <= 2 }" 
       << endl;
  cout << "Enter a precision (between 1 and 0.001): ";
  cin >> prec;

  start = clock();

  // when we start we give A a box big enough to guarantee to contain the 
  // characterisation of Sc

  Sc = Sivia(IBTAnnular,A,prec);
  end = clock();

  cout << "Computing time : " 
       << ((static_cast<double>(end - start)) / CLOCKS_PER_SEC) << " s."<< endl;
  cout << "Volume: " << Volume(Sc) << endl;
  cout << "Number of leaves: " << NbLeaves(Sc) << endl;

  // To realize a file output of the AIASubPaving Sc
                    // Filename
  ofstream os("AIA3_3a.txt");
  os << 2 << endl;  // Dimension of the AIASubPaving
                    // Root box
  os << interval(-5.0,5.0) << " "
    << interval(-5.0,5.0) << " " << endl;
                    // Precision used
  os << "Precision is " << prec << endl;
  os << Sc << endl; // AIASubPaving itself
  cout << "The output AIASubPaving has been written to AIA3_3a.txt" 
       << endl << endl;

  // the AIASubPaving Sc that we have created is the regular 
  // AIASubPaving that covers the set
  // X = {(x1,x2) in R2 | sqr(x1) + sqr(x2) is in [1,2]} 
  // (remember that it contains this area rather than being this area, 
  // and that eps has determined how small we go in the AIASubPaving 
  // characterisation)

  cout << "Characterization of the set Sc4=f(Sc)" << endl
    << " with Sc from our first example and "<< endl
    << "with f1(x) = x1*x2," << endl
    << "     f2(x) = x1 + x2," << endl;
  cout << "by realizing the image of f by ImageSp" << endl;
  cout << "Enter a precision (between 1 and 0.01): ";
  cin >> prec;

  start = clock();

  // use Image SP to find a characterisation of the image of Sc using the 
  // function in IVF
  Sc4 = ImageSp(IVF_ex3_3, Sc, prec);

  end = clock();

  cout << "Computing time : " 
       << ((static_cast<double>(end - start)) / CLOCKS_PER_SEC) << " s."<< endl;
  cout << "The volume is " << Volume(Sc4) << endl;
  cout << "The number of leaves is " << NbLeaves(Sc4) << endl;

  // To realize a file output of the AIASubPaving Sc
                    // Filename
  ofstream os4("AIA3_3d.txt");
  os4 << 2 << endl; // Dimension of the AIASubPaving
                    // Domain AIASubPaving
  os4 << interval(-3.0,3.0) << " "
    << interval(-3.0,3.0) << " " << endl;
                    // Precision used
  os4 << "Precision is " << prec << endl;
                    // Image AIASubPaving itself
  os4 << Sc4 << endl;

  cout << "The output AIASubPaving has been written to AIA3_3d.txt" 
       << endl << endl;

  delete A;         // Delete all subpavings newed in dynamic memory
  delete Sc;
  delete Sc4;

  return 0;
}
