// $Id$
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

@file ModPlusOMCtrl.cpp
@brief Comments for file documentation.
@author Hubert Thieriot, hubert.thieriot@mines-paristech.fr
Company : CEP - ARMINES (France)
http://www-cep.ensmp.fr/english/
@version


*/
#include "ModPlusOMCtrl.h"
#include "ModModelPlus.h"
#include "ModModel.h"
#include "MOParameter.h"
#include "Project.h"
#include "LowTools.h"
#include "OpenModelicaParameters.h"

ModPlusOMCtrl::ModPlusOMCtrl(Project* project,ModModelPlus* modModelPlus,MOomc* moomc)
    :ModPlusCtrl(project,modModelPlus,moomc)
{

    _initFileTxt = _modModelPlus->modModelName()+"_init.txt";
    _initFileXml = _modModelPlus->modModelName()+"_init.xml";


#ifdef WIN32
    _exeFile = _modModelPlus->modModelName()+".exe";
#else
    _exeFile = _modModelPlus->modModelName();
#endif

    _copyAllMoOfFolder = true;

    _parameters = new MOParameters();
    OpenModelicaParameters::setDefaultParameters(_parameters);
}

ModPlusOMCtrl::~ModPlusOMCtrl(void)
{
}

ModPlusCtrl* ModPlusOMCtrl::clone()
{
    ModPlusOMCtrl* cloned = new ModPlusOMCtrl(_project,_modModelPlus,_moomc);

    cloned->_initFileTxt = _initFileTxt;
    cloned->_initFileXml = _initFileXml;
    cloned->_exeFile = _exeFile;
    cloned->_copyAllMoOfFolder = _copyAllMoOfFolder;
    cloned->_parameters = _parameters->clone();


    return cloned;
}

ModPlusCtrl::Type ModPlusOMCtrl::type() const
{
    return ModPlusCtrl::OPENMODELICA;
}

QString ModPlusOMCtrl::name()
{
    return "Open Modelica";
}

bool ModPlusOMCtrl::useMat()
{
    return _parameters->value(OpenModelicaParameters::str(OpenModelicaParameters::OUTPUT),true).toBool();
}

QString ModPlusOMCtrl::resFile()
{
    if(useMat())
        return _modModelPlus->modModelName()+"_res.mat";
    else
        return _modModelPlus->modModelName()+"_res.csv";
}

// Parameters


bool ModPlusOMCtrl::readOutputVariables(MOVector<Variable> *finalVariables,QString resFileLocal)
{
    InfoSender::instance()->send(Info("Reading final variables in "+resFileLocal,ListInfo::NORMAL2));

    if(resFileLocal.isEmpty())
        resFileLocal = resFile();

    if(useMat())
        return OpenModelica::getFinalVariablesFromMatFile(resFileLocal,finalVariables,_modModelPlus->modModelName());
    else
        return OpenModelica::getFinalVariablesFromFile(resFileLocal,finalVariables,_modModelPlus->modModelName());
}


bool ModPlusOMCtrl::readInitialVariables(MOVector<Variable> *initVariables,bool forceRecompile, QString initFile)
{
    QString initFileTxt;
    QString initFileXml;

    bool authorizeRecreate=false;
    if(initFile.isEmpty())
    {
        authorizeRecreate=true;
        initFileTxt = _modModelPlus->mmoFolder().absoluteFilePath(_initFileTxt);
        initFileXml = _modModelPlus->mmoFolder().absoluteFilePath(_initFileXml);
    }
    else
    {
        if(initFile.right(3)=="xml")
            initFileXml = initFile;

        if(initFile.right(3)=="txt")
            initFileTxt = initFile;
    }

    initVariables->clear();

    if((!QFile::exists(initFileXml)&& !QFile::exists(initFileTxt)&& authorizeRecreate)||forceRecompile)
    {
        createInitFile(_modModelPlus->moDependencies());
    }

    if(!QFile::exists(initFileXml)&& !QFile::exists(initFileTxt))
    {
        return false;
    }
    else
    {
        if(QFile::exists(initFileXml))
        {
            OpenModelica::getInputVariablesFromXmlFile(_moomc,initFileXml,_modModelPlus->modModelName(),initVariables);
            InfoSender::instance()->send(Info("Reading initial variables in "+initFileXml,ListInfo::NORMAL2));
        }
        else if(QFile::exists(initFileTxt))
        {
            OpenModelica::getInputVariablesFromTxtFile(_moomc,initFileTxt,initVariables,_modModelPlus->modModelName());
            InfoSender::instance()->send(Info("Reading initial variables in "+initFileTxt,ListInfo::NORMAL2));
        }
        return true;
    }

    return true;
}

bool ModPlusOMCtrl::compile(const QFileInfoList & moDeps)
{

    InfoSender::sendCurrentTask("OpenModelica : Compiling model "+_modModelPlus->modModelName());

    // compile
    QString logFile = _modModelPlus->mmoFolder().absoluteFilePath(_modModelPlus->modModelName()+".log");
    bool success = OpenModelica::compile(_moomc,_modModelPlus->moFilePath(),_modModelPlus->modModelName(),_modModelPlus->mmoFolder(),
                                         moDeps,_modModelPlus->neededFiles(),_modModelPlus->neededFolders());

    // Inform
    ListInfo::InfoNum iMsg;
    if(success)
        InfoSender::instance()->send(Info(ListInfo::MODELCOMPILATIONSUCCESS,_modModelPlus->modModelName(),logFile));
    else
        InfoSender::instance()->send(Info("Model "+_modModelPlus->modModelName()+" failed to compile. See OMC log tab for details.",ListInfo::ERROR2));

    InfoSender::instance()->eraseCurrentTask();

    return success;
}


bool ModPlusOMCtrl::isCompiled()
{
    bool filesExist = true;
    QStringList filesNeeded;
    filesNeeded << _exeFile;

    QString filePath;
    QFile file;

    for(int i=0;i<filesNeeded.size();i++)
    {
        filePath = _modModelPlus->mmoFolder().absoluteFilePath(filesNeeded.at(i));
        file.setFileName(filePath);
        filesExist = filesExist && file.exists();
    }
    return filesExist;
}


bool ModPlusOMCtrl::simulate(QDir tempFolder,MOVector<Variable> * inputVars,MOVector<Variable> * outputVars,QFileInfoList filesToCopy,QFileInfoList moDependencies)
{
    // Info
    InfoSender::sendCurrentTask("Open Modelica : Simulating model "+_modModelPlus->modModelName());


    // load moDependencies if not already loaded
    // storePaths in project = false
    // forceLoad = false
    //_project->loadMoFiles(moDependencies,false,false);
    // Create a segfault afterwards since problem is in a different thread than GUI :
    // _project->loadMoFiles clears moditemstree but GUI will still call it
    // (beginresetmodel seems not working in multithreading)
    // However, we should not need to load modependencies with OM since alreaedy loaded.

    // clear outputVars
    outputVars->clear();

    bool compileOk = isCompiled();
    // eventually compile model
    if(!compileOk)
        compileOk = compile(_modModelPlus->moDependencies());

    if(!compileOk)
        return false; // compilation failed, useless to pursue



    // Create tempDir
    LowTools::mkdir(tempFolder.absolutePath(),false);


    /// copy files in temp dir (\todo : optimize with a config.updateTempDir in case of several consecutive launches)
    QFileInfoList allFilesToCopy;
    QDir mmoDir = QDir(_modModelPlus->mmoFolder());
    allFilesToCopy << mmoDir.filePath(_exeFile);
    allFilesToCopy.append(filesToCopy);
    // choose init file (can take both)
    bool txt=mmoDir.exists(_initFileTxt);
    bool xml=mmoDir.exists(_initFileXml);
    if(txt)
        allFilesToCopy <<  mmoDir.filePath(_initFileTxt);
    if(xml)
        allFilesToCopy << mmoDir.filePath(_initFileXml);

    if(!txt && ! xml)
        InfoSender::instance()->sendError("Unable to find an init file for model "+_modModelPlus->modModelName());

    InfoSender::instance()->debug("Start copying in temp directory : "+tempFolder.absolutePath());
    QFileInfo fileToCopyInfo;
    //QFile fileToCopy;
    bool copyOk;
    bool removeOk;
    for(int i=0; i< allFilesToCopy.size();i++)
    {
        //fileToCopy.setFileName(allFilesToCopy.at(i));
        fileToCopyInfo = allFilesToCopy.at(i);
        removeOk = tempFolder.remove(fileToCopyInfo.fileName());
        InfoSender::instance()->debug("Removing in temp directory : "+tempFolder.filePath(fileToCopyInfo.fileName())+" : "+QVariant(removeOk).toString());
        if(!removeOk)
        {
            QFile::setPermissions(fileToCopyInfo.absoluteFilePath(),fileToCopyInfo.permissions() | QFile::WriteUser);
            removeOk = tempFolder.remove(fileToCopyInfo.fileName());
            InfoSender::instance()->debug("Removing in temp directory : "+tempFolder.filePath(fileToCopyInfo.fileName())+" : "+QVariant(removeOk).toString());
        }

        copyOk = QFile::copy(allFilesToCopy.at(i).absoluteFilePath(),tempFolder.filePath(fileToCopyInfo.fileName()));
        //= fileToCopy.copy(tempDir.filePath(fileToCopyInfo.fileName()));
        InfoSender::instance()->debug("Copying in temp directory : "+tempFolder.filePath(fileToCopyInfo.fileName())+" : "+QVariant(copyOk).toString());
        if(!copyOk)
            InfoSender::instance()->sendWarning("Unable to copy file in temp directory : "+fileToCopyInfo.fileName()/*+" ("+QFile::errorString()+")"*/);
    }


    // remove previous log files
    QStringList filesToRemove;
    filesToRemove << resFile();
    for(int i=0;i<filesToRemove.size();i++)
        tempFolder.remove(filesToRemove.at(i));

    QString tempInitFileXml = tempFolder.absoluteFilePath(_initFileXml);
    QString tempInitFileTxt = tempFolder.absoluteFilePath(_initFileTxt);
    QString tempResFile = tempFolder.absoluteFilePath(resFile());
    QString tempExeFile = tempFolder.absoluteFilePath(_exeFile);


    QFileInfo initXmlInfo(tempInitFileXml);
    QFileInfo initTxtInfo(tempInitFileTxt);

    // use xml or txt init file ?
    bool useXml = initXmlInfo.exists();

    // Specifying new Variables values in OM input file
    if(useXml)
        OpenModelica::setInputXml(tempInitFileXml,inputVars,_modModelPlus->modModelName(),parameters());
    else
        OpenModelica::setInputVariablesTxt(tempInitFileTxt,inputVars,_modModelPlus->modModelName(),parameters());

    // Launching openmodelica
    int maxNSec=_parameters->value(OpenModelicaParameters::str(OpenModelicaParameters::MAXSIMTIME),-1).toInt();

    OpenModelica::start(tempExeFile,maxNSec);

    InfoSender::eraseCurrentTask();

    //getting results
    //Checking if successed
    bool success=QFile::exists(tempResFile);

    if(!success)
        return false;

    bool readOk = readOutputVariables(outputVars,tempResFile);
    return readOk;
}
bool ModPlusOMCtrl::createInitFile(const QFileInfoList & moDeps)
{
    return compile(moDeps);
}
