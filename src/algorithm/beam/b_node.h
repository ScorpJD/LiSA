#ifndef _B_NODE_H_
#define _B_NODE_H_

#include"../../basics/global.hpp"
#include"../../lisa_dt/schedule/os_sched.hpp"
#include"../../basics/order.hpp"

#include"../../lisa_dt/ctrlpara.hpp"
#include"../../lisa_dt/schedule.hpp"
#include"../../lisa_dt/ptype.hpp"
#include"../../lisa_dt/lvalues.hpp"
#include <fstream.h>
#include"../../basics/except.hpp"

enum CostFunc {
  CMax,
  CLast
};

extern CostFunc costFunc;
extern TIMETYP objective;


/**  B_Node a modified Lisa_OsSchedule
     @author Jan Tusch & Andre Herms
     @see Lisa_OsSchedule
     @see KList
*/ 
//@{

class B_Node : public Lisa_OsSchedule {
  
  friend ostream & operator<<(ostream&, B_Node&); 

 protected:
  B_Node *parent;
  TIMETYP cost;
  
  int lastAddedRow, lastAddedCol;

 public:
  ///construct a schedule from another
  B_Node(B_Node *parent);
  ///construct a schedule that hands over the problem to its children
  B_Node(Lisa_OsProblem *);
  ///get costs of the (partial) schedule
  virtual TIMETYP getCosts();
  ///for comparision
  operator TIMETYP (void){return getCosts();}
  ///insert an operation in the partial schedule and remember this operation until insert is invoked again
  ///@see Lisa_OsSchedule
  int insert(int,int,int,int);
  ///the problem
  Lisa_OsProblem * problem;
};


/** represents a container with capacity k and special functionality regarding intsertion
    
    @author Jan Tusch & Andre Herms
    @see B_Node
*/

class KList {

public:
  B_Node **list;
  ///index of next free insertion position
  int in_list;
  ///position of the element with the maximum costValue overall elements
  int worst_in_list;
  int k;
  ///constructs a KList with capicity k
  KList(int k);
  ~KList();
  ///add a B_Node to the list if either the list is not full or there is a B_Node with larger costs in the list
  void add(B_Node *);
  ///add a B_Node to the list at position p, if there is no B_Node or a B_Node with larger costs at position p
  void add(B_Node *, int p);
  ///returns true if the capacity is exhausted
  bool full() { return in_list >= k;} 
};

//@}
#endif
