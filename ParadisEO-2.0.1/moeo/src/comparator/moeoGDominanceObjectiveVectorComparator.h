/*
* <moeoGDominanceObjectiveVectorComparator.h>
* Copyright (C) DOLPHIN Project-Team, INRIA Futurs, 2006-2007
* (C) OPAC Team, LIFL, 2002-2007
*
* Arnaud Liefooghe
*
* This software is governed by the CeCILL license under French law and
* abiding by the rules of distribution of free software.  You can  use,
* modify and/ or redistribute the software under the terms of the CeCILL
* license as circulated by CEA, CNRS and INRIA at the following URL
* "http://www.cecill.info".
*
* As a counterpart to the access to the source code and  rights to copy,
* modify and redistribute granted by the license, users are provided only
* with a limited warranty  and the software's author,  the holder of the
* economic rights,  and the successive licensors  have only  limited liability.
*
* In this respect, the user's attention is drawn to the risks associated
* with loading,  using,  modifying and/or developing or reproducing the
* software by the user in light of its specific status of free software,
* that may mean  that it is complicated to manipulate,  and  that  also
* therefore means  that it is reserved for developers  and  experienced
* professionals having in-depth computer knowledge. Users are therefore
* encouraged to load and test the software's suitability as regards their
* requirements in conditions enabling the security of their systems and/or
* data to be ensured and,  more generally, to use and operate it in the
* same conditions as regards security.
* The fact that you are presently reading this means that you have had
* knowledge of the CeCILL license and that you accept its terms.
*
* ParadisEO WebSite : http://paradiseo.gforge.inria.fr
* Contact: paradiseo-help@lists.gforge.inria.fr
*
*/
//-----------------------------------------------------------------------------

#ifndef MOEOGDOMINANCEOBJECTIVEVECTORCOMPARATOR_H_
#define MOEOGDOMINANCEOBJECTIVEVECTORCOMPARATOR_H_

#include <comparator/moeoObjectiveVectorComparator.h>

/**
 * This functor class allows to compare 2 objective vectors according to g-dominance.
 * The concept of g-dominance as been introduced in:
 * J. Molina, L. V. Santana, A. G. Hernandez-Diaz, C. A. Coello Coello, R. Caballero,
 * "g-dominance: Reference point based dominance" (2007)
 */
template < class ObjectiveVector >
class moeoGDominanceObjectiveVectorComparator : public moeoObjectiveVectorComparator < ObjectiveVector >
  {
  public:

    /**
     * Ctor.
     * @param _ref the reference point
     */
    moeoGDominanceObjectiveVectorComparator(ObjectiveVector & _ref) : ref(_ref)
    {}


    /**
     * Returns true if _objectiveVector1 is g-dominated by _objectiveVector2.
     * @param _objectiveVector1 the first objective vector
     * @param _objectiveVector2 the second objective vector
     */
    const bool operator()(const ObjectiveVector & _objectiveVector1, const ObjectiveVector & _objectiveVector2)
    {
      unsigned int flag1 = flag(_objectiveVector1);
      unsigned int flag2 = flag(_objectiveVector2);
      if (flag2==0)
        {
          // cannot dominate
          return false;
        }
      else if ( (flag2==1) && (flag1==0) )
        {
          // is dominated
          return true;
        }
      else // (flag1==1) && (flag2==1)
        {
          // both are on the good region, so let's use the classical Pareto dominance
          return paretoComparator(_objectiveVector1, _objectiveVector2);
        }
    }


  private:

    /** the reference point */
    ObjectiveVector & ref;
    /** Pareto comparator */
    moeoParetoObjectiveVectorComparator < ObjectiveVector > paretoComparator;


    /**
     * Returns the flag of _objectiveVector according to the reference point
     * @param _objectiveVector the first objective vector
     */
    unsigned int flag(const ObjectiveVector & _objectiveVector)
    {
      unsigned int result=1;
      for (unsigned int i=0; i<ref.nObjectives(); i++)
        {
          if (_objectiveVector[i] > ref[i])
            {
              result=0;
            }
        }
      if (result==0)
        {
          result=1;
          for (unsigned int i=0; i<ref.nObjectives(); i++)
            {
              if (_objectiveVector[i] < ref[i])
                {
                  result=0;
                }
            }
        }
      return result;
    }

  };

#endif /*MOEOGDOMINANCEOBJECTIVEVECTORCOMPARATOR_H_*/
