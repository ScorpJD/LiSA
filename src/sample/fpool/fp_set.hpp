
#ifndef _FP_Set_h
#define _FP_Set_h

#include "../../basics/matrix.hpp"
#include "fp_vect.hpp"

/// vector set for football pool problem
/** @author        Thomas Tautenhahn
 *  @version 3.0
 */
class FP_Set: public Lisa_GenericObject{
private:
  //@{
  /// private variable,needs documentation
  int slack, sigma,n;
  Lisa_Vector<int> * covered;
  FP_Vector ** vectors;
  //@}
  
public:
  /// constructor 
  /** create FP_Set with given vector length and number of vectors */
  FP_Set(int, int);
  
  ///destructor
  ~FP_Set();
  
  /// assignment operator
  const FP_Set& operator=(FP_Set& other);
  
  //@{
  ///member function, needs documentation
  FP_Vector * replace_vector(int , FP_Vector *);
  inline int get_slack() { return slack; }
  inline int get_cover( int i) { return (*covered)[i]; }
  inline FP_Vector& get_vector(int i) { return (*vectors[i]); }
  void spread();
  void random(long *);
  void random2(long *);
  void write(std::ostream& strm = std::cout) const;
  //@}
};

//**************************************************************************

#endif

