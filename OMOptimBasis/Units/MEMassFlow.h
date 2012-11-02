// $Id: MEMassFlow.h 9551 2011-07-28 16:56:59Z hubert.thieriot $
/**
 * This file is part of OpenModelica.
 *
 * Copyright (c) 1998-CurrentYear, Open Source Modelica Consortium (OSMC),
 * c/o Linköpings universitet, Department of Computer and Information Science,
 * SE-58183 Linköping, Sweden.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR 
 * THIS OSMC PUBLIC LICENSE (OSMC-PL). 
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES RECIPIENT'S ACCEPTANCE
 * OF THE OSMC PUBLIC LICENSE OR THE GPL VERSION 3, ACCORDING TO RECIPIENTS CHOICE. 
 *
 * The OpenModelica software and the Open Source Modelica
 * Consortium (OSMC) Public License (OSMC-PL) are obtained
 * from OSMC, either from the above address,
 * from the URLs: http://www.ida.liu.se/projects/OpenModelica or  
 * http://www.openmodelica.org, and in the OpenModelica distribution. 
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH
 * IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF OSMC-PL.
 *
 * See the full OSMC Public License conditions for more details.
 *
 * Main contributor 2010, Hubert Thierot, CEP - ARMINES (France)

        @file MEMassFlow.h
     @brief Comments for file documentation.
     @author Hubert Thieriot, hubert.thieriot@mines-paristech.fr
     Company : CEP - ARMINES (France)
     http://www-cep.ensmp.fr/english/
        @version

  */
#if !defined(_MEMASSFLOW_H)
#define _MEMASSFLOW_H

#include "MEDimValue.h"
#include <cmath>

class MEMassFlow : public MEDimValue
{
public:
        MEMassFlow();
        MEMassFlow(double value,int unit);
        MEMassFlow(const MEMassFlow&);
        ~MEMassFlow();

    enum Units
    {
                KG_S,
                T_H
    };

    QString unit(int iUnit)  const;
        QString unit() const;  //should'nt be (should be inherited) but error in compiler otherwise
    unsigned nbUnits() const;
    double convert(double value,int orgUnit,int dstUnit) const;

        MEMassFlow& operator+=(const MEMassFlow&);
        MEMassFlow& operator-=(const MEMassFlow&);
        MEMassFlow operator-(const MEMassFlow&) const;
        MEMassFlow operator+(const MEMassFlow&) const;
        MEMassFlow operator*(const double &) const;


};




#endif
