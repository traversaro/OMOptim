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

 	@file WidgetTableEIGroups.cpp
 	@brief Comments for file documentation.
 	@author Hubert Thieriot, hubert.thieriot@mines-paristech.fr
 	Company : CEP - ARMINES (France)
 	http://www-cep.ensmp.fr/english/
 	@version 0.9 
*/

#include "WidgetTableEIGroups.h"
#include "ui_WidgetTableEIGroups.h"
#include <QtGui/QErrorMessage>



WidgetTableEIGroups::WidgetTableEIGroups(EIItem* _rootEI,bool _editable,EIReader* _eiReader,QWidget *parent) :

    QWidget(parent),
    ui(new Ui::WidgetTableEIGroupsClass)
{
    ui->setupUi(this);
	
	rootEI = _rootEI;
	editable = _editable;
	eiReader = _eiReader;

	//table model
	tableEIGroups = new TableEIItems(rootEI,editable,eiReader,EI::GROUP,true);
	tableView=new MOTableView(this);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->setModel(tableEIGroups);
	ui->layoutTable->addWidget(tableView);
	//GuiTools::minimizeTableSize(tableView);

	connect(ui->pushAdd,SIGNAL(clicked()),this,SLOT(onPushedAdd()));
	connect(ui->pushRemove,SIGNAL(clicked()),this,SLOT(onPushedRemove()));


}

WidgetTableEIGroups::~WidgetTableEIGroups()
{
    delete ui;
}



void WidgetTableEIGroups::onPushedAdd()
{
	//eiStreams->addItem(new EIStream());
}

void WidgetTableEIGroups::onPushedRemove()
{
	/*QModelIndexList indexList = tableView->selectionModel()->selectedRows();
	QModelIndex curSourceIndex;

	QList<int> rows;
	for(int i=0;i<indexList.size();i++)
	{
		curSourceIndex = streamsProxyModel->mapToSource(indexList.at(i));
		rows.push_back(curSourceIndex.row());
	}
	eiStreams->removeRows(rows);*/
}