/*
 * ************** os_sched.cpp *******************************************
 * 
 * inherit for a schedule for the open shop problem
 *
 * Owner: Thomas Tautenhahn
 *
 * 11.12.1998
*/

#include"os_sched.hpp"
#include"../basics/order.hpp"
#define debug


Lisa_OsProblem::Lisa_OsProblem(Lisa_Values * Pin) : Lisa_ShpProblem( Pin ) 
  {
  };

// ############ Lisa_OsSchedule #################################################

// returns an empty feasible schedule to *Pin with MIJ=emptyset 
Lisa_OsSchedule::Lisa_OsSchedule(Lisa_OsProblem* Pin)
     { int  i,j;
       
       ComputeTails=FALSE;
       ComputeHeads=TRUE;
       P = OSP = Pin;
       if ( !(MOsucc=new Lisa_Matrix<int>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       if ( !(MOpred=new Lisa_Matrix<int>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       if ( !(JOsucc=new Lisa_Matrix<int>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       if ( !(JOpred=new Lisa_Matrix<int>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       if ( !(head  =new Lisa_Matrix<TIMETYP>(P->n+1,P->m+1)) )
	 cerr << "\nError: no more memory";
       tail=NULL; 
       for (i=0; i<=(P->n); i++)
          for (j=0; j<=(P->m); j++)
	     { 
               (*MOsucc)[i][j]=(*JOsucc)[i][j]=SINK;
	       (*MOpred)[i][j]=(*JOpred)[i][j]=SOURCE;
               (*head)[i][j]=(*P->ri)[i];
	     }
       value=(TIMETYP) 0;
       ScheduleOK=TRUE;
     }

Lisa_OsSchedule::~Lisa_OsSchedule()
     { 
       delete head;
       delete MOsucc;
       delete MOpred;
       delete JOsucc;
       delete JOpred;
       if (tail)
          delete tail;
     }

 
void Lisa_OsSchedule::operator=(Lisa_OsSchedule &other)
     { 
       if (other.P != P)
         {
           cout << "Error: can only copy schedules of same problem!\n";
           return;
         }
       (*MOsucc)=*other.MOsucc;
       (*JOsucc)=*other.JOsucc;
       (*MOpred)=*other.MOpred;
       (*JOpred)=*other.JOpred;
       (*head)=*other.head;
       if (other.tail)
         {
           if (!tail)
             tail =new Lisa_Matrix<TIMETYP>(P->n+1,P->m+1);
           (*tail)=*other.tail;
         }        
       else if (tail) delete tail;
       ScheduleOK=other.ScheduleOK;
       ComputeTails=other.ComputeTails;
       ComputeHeads=other.ComputeHeads;
       lastinsi=other.lastinsi; lastinsj=other.lastinsj;  
       value=other.value;
     }

int Lisa_OsSchedule::insert(int i, int j, int woi, int woj)
    { 
      int h, xyz;  
      TIMETYP oldmosucc, oldjosucc;

      if (!ScheduleOK) { cerr << "\nSchedule not feasible --> nothig done \n";
		     return(ERROR);
		   }
#ifdef debug
      if (exists(i,j))
	{ 
          cerr <<"\nError: operation ("<<i<<","<<j<<") is already inserted!";
	  return(ERROR);
	}
      if (!(exists(woi,j) && exists (i, woj)))
	{ 
          cerr << "\nError: insertion position does not exist!";
	  return(ERROR);
	}
#endif
      // remember where it starts:  
      sti=i; stj=j;

      // modify lists:
      (*JOsucc)[i][j]=(*JOsucc)[woi][j]; (*JOsucc)[woi][j]=i;
      (*JOpred)[i][j]=woi;   (*JOpred)[(*JOsucc)[i][j]][j]=i;
      (*MOsucc)[i][j]=(*MOsucc)[i][woj]; (*MOsucc)[i][woj]=j;
      (*MOpred)[i][j]=woj;   (*MOpred)[i][(*MOsucc)[i][j]]=j;

      // recompute heads / tails
      xyz=OK;
      if (ComputeHeads)
	 { 
           started=FALSE;
	   if ((h=(*JOsucc)[i][j])) oldjosucc=(*head)[h][j];
           if ((h=(*MOsucc)[i][j])) oldmosucc=(*head)[i][h];  
	   (*head)[i][j]=-MAXTIME;
           if (pullhead(i,j)==CYCLE) return CYCLE;
	   if ((h=(*JOsucc)[i][j]) && (oldjosucc==(*head)[h][j]))
              if ((h=(*MOsucc)[i][j]) && (oldmosucc==(*head)[i][h]))
                 xyz=PERFEKT;
	 }
      if (ComputeTails)
	 { 
           started=FALSE;
           (*tail)[i][j]=-MAXTIME;
  	   if (pulltail(i,j)==CYCLE) return CYCLE;
  	 }
      return(xyz); 
    }

void Lisa_OsSchedule::exclude(int i, int j)
    { 
      int pi,si, pj, sj;
    
      if (!ScheduleOK) 
	{ 
	  cerr << "\nSchedule not feasible -> nothing done!";
	  return;
	}
#ifdef debug
      if (!exists(i,j))
	{ 
          cerr<<"\nError: operation ("<<i<<","<<j<<") is not inserted!";
	  return;
	}
#endif

      // modify lists
      pi=(*JOpred)[i][j]; si=(*JOsucc)[i][j];
      pj=(*MOpred)[i][j]; sj=(*MOsucc)[i][j];
      (*JOsucc)[pi][j]=si;  
      (*JOpred)[si][j]=pi;
      (*MOsucc)[i][pj]=sj;  
      (*MOpred)[i][sj]=pj; 
      
      // reset operation (i,j)

      (*head)[i][j]=(*P->ri)[i]; 
      if (tail) (*tail)[i][j]=0;
      (*MOsucc)[i][j]=(*MOpred)[i][j]=(*JOsucc)[i][j]=(*JOpred)[i][j]=SOURCE;  
 
      // recompute heads and tails
      if (ComputeHeads)
        {
          if (si) { started=FALSE;
                    sti=si; stj=j;
                    pullhead(si,j);
                  }  
          else (*head)[SINK][j]=(*head)[pi][j]+(*P->time)[pi][j]; 
          if (sj) { started=FALSE;
                    sti=i; stj=sj;
                    pullhead(i,sj);
                  }  
          else (*head)[i][SINK]=(*head)[i][pj]+(*P->time)[i][pj];
        }  
      if (ComputeTails)
        {
          if (pi) { started=FALSE;
                    sti=pi; stj=j;
                    pulltail(pi,j);
                  }           
          if (pj) { started=FALSE;
                    sti=i; stj=pj;
                    pulltail(i,pj);
                  }  
        }  
    }

int Lisa_OsSchedule::read_LR(Lisa_Matrix<int> * lr)
 {
   int i, j, r;
   for (i=1; i<=P->n; i++)
      for (j=1; j<=P->m; j++)
         if (exists(i,j)) exclude(i,j);
   for (r=P->n*P->m; r; r--)
      for (i=1; i<=P->n; i++)
         for (j=1; j<=P->m; j++)
            if ((*lr)[i-1][j-1]==r)
              insert(i,j,SOURCE,SOURCE);
   ScheduleOK=TRUE;
   return OK;
 }

int Lisa_OsSchedule::read_Cij(Lisa_Matrix<TIMETYP> * cij)
 {
   int i,j,k;
   Lisa_Order * order = new Lisa_Order(P->n, P->m);
   
   for (i=1; i<=P->n; i++)
      for (j=1; j<=P->m; j++)
         order->read_one_key(i-1, j-1, MAXTIME-(*P->time)[i][j]);
   order->sort();
   order->read(cij);
   order->sort();
   
   // construct sequence:
   for (k=0; k<(P->n * P->m); k++)
      {
        i=order->row(k)+1; j=order->col(k)+1;
        if (exists(i,j)) exclude(i,j);
        if ((*cij)[i-1][j-1]) 
	  insert(i,j,(*JOpred)[SINK][j],(*MOpred)[i][SINK]);
      }
    
   // shift operations to the right time slots:
   sti=stj=0;                      // to disable cycle test
   for (k=0; k<(P->n * P->m); k++)
      {
        i=order->row(k)+1; j=order->col(k)+1;
        if ((*cij)[i-1][j-1])  
          pushhead(i,j, (*cij)[i-1][j-1]- (*(P->time))[i][j]);
      }    
   ComputeHeadsTails(TRUE,TRUE);
   TailsFromHeads();
   delete order;

   
   // now test wether input was correct:
   ScheduleOK=TRUE;
   for (i=P->n; i;  i--)
       for (j=P->m; j; j--)
           if (MAXTIME-(*tail)[i][j] != (*cij)[i-1][j-1] ) return ERROR;
   return OK;
  }




 