/* ************************************************************************* */
/*                              Filename: Heur.h                             */
/*                                                                           */
/*    Export: Procedure Heuristic_Schedule:                                  */
/*                      Berechnung einer heuristischen Loesung fuer den      */
/*                      Suchbaumknoten SonNode.                              */
/* ************************************************************************* */

#ifndef _HEUR_H
#define _HEUR_H



/* ************************************************************************* */
/*                         Procedure  Heuristic_Schedule()                   */
/*                                                                           */
/*  INPUT:  -/-                                                              */
/*                                                                           */
/*  FUNCTION: Berechnung einer heuristischen Loesung. Der kritische Pfad ist */
/*            in CriticalPath gespeichert. Verbessert die berechnete Fertig- */
/*	      stellungszeit die aktuelle upper bound UB, so wird UB auf-     */
/*            datiert. Die Loesung wird in der Datei output.dat gespeichert. */
/* ************************************************************************* */

void Heuristic_Schedule_MATCHING();

void Heuristic_Schedule_BOTTLE_MATCHING();

void Heuristic_Schedule_BOTTLE_MATCHING_VAR();

void Heuristic_Schedule_LB_PREC_RULE();
void Heuristic_Schedule_LB_PREC_RULE_VAR();

#endif
