﻿// $Id$
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
 * Main contributor 2010, Hubert Thierot, CEP - ARMINES (France)

 	@file WidgetCalculateMooPoints.cpp
 	@brief Comments for file documentation.
 	@author Hubert Thieriot, hubert.thieriot@mines-paristech.fr
 	Company : CEP - ARMINES (France)
 	http://www-cep.ensmp.fr/english/
 	@version 0.9 
*/

#include "WidgetCalculateMooPoints.h"
#include "ui_WidgetCalculateMooPoints.h"
#include <QtGui/QErrorMessage>
#include "QtGui/QListWidget"


WidgetCalculateMooPoints::WidgetCalculateMooPoints(OptimResult* _result,WidgetMooPlot* _widgetMooPlot, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WidgetCalculateMooPointsClass)
{
    ui->setupUi(this);
	

	result = _result;
	widgetMooPlot = _widgetMooPlot;

	connect(ui->pushCalcSelected,SIGNAL(clicked()),
		this,SLOT(recomputeSelectedPoints()));

	connect(ui->pushExport,SIGNAL(clicked()),
		this,SLOT(exportSelectedPoints()));
}

WidgetCalculateMooPoints::~WidgetCalculateMooPoints()
{
    delete ui;
}





void WidgetCalculateMooPoints::recomputeSelectedPoints()
{
	std::vector<int> pointsList;

	pointsList = widgetMooPlot->_listPoints->getSelectedIndexes().toVector().toStdVector();

	bool forceRecompute = ui->checkForceRecompute->isChecked();
	result->recomputePoints(pointsList,forceRecompute);

}

void WidgetCalculateMooPoints::exportSelectedPoints()
{
	
	// get file name
	QString csvPath = QFileDialog::getSaveFileName(
		this,
		"MO - Export optimum points",
		QString::null,
		"CSV file (*.csv)" );
	
	if(!csvPath.isNull())
	{
		QList<int> listPoints = widgetMooPlot->_listPoints->getSelectedIndexes();
		QString csvText = result->buildAllVarsFrontCSV(listPoints);
		
		QFile frontFile(csvPath);
		if(frontFile.exists())
			frontFile.remove();

		frontFile.open(QIODevice::WriteOnly);
		QTextStream tsfront( &frontFile );
		tsfront << csvText;
		frontFile.close();
	}
}