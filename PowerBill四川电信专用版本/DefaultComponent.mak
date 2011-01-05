
############# Target type (Debug/Release) ##################
############################################################
CPPCompileDebug= /Zi /Od /D "_DEBUG" /MDd  /Fd"$(TARGET_NAME)" 
CPPCompileRelease= /Ox /D"NDEBUG" /MD /Fd"$(TARGET_NAME)" 
LinkDebug=
LinkRelease=
BuildSet=Debug
SUBSYSTEM=/SUBSYSTEM:console
COM=False
RPFrameWorkDll=False
SimulinkLibName=

ConfigurationCPPCompileSwitches=   /I . /I . /I $(OMROOT)\LangCpp /I $(OMROOT)\LangCpp\oxf /nologo /W3 $(ENABLE_EH) $(CRT_FLAGS) $(CPPCompileDebug) /D "_AFXDLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" $(INST_FLAGS) $(INCLUDE_PATH) $(INST_INCLUDES) /c 

!IF "$(RPFrameWorkDll)" == "True"
ConfigurationCPPCompileSwitches=$(ConfigurationCPPCompileSwitches) /D "FRAMEWORK_DLL"
!ENDIF

!IF "$(COM)" == "True"
SUBSYSTEM=/SUBSYSTEM:windows
!ENDIF

################### Compilation flags ######################
############################################################
INCLUDE_QUALIFIER=/I
LIB_PREFIX=MS

CRT_FLAGS=
ENABLE_EH=/GX

WINMM_LIB=winmm.lib

################### Commands definition #########################
#################################################################
RMDIR = rmdir
LIB_CMD=link.exe -lib
LINK_CMD=link.exe
LIB_FLAGS=$(LinkDebug) /NOLOGO   
LINK_FLAGS=$(LinkDebug) /NOLOGO    $(SUBSYSTEM) /MACHINE:I386 

############### Generated macros #################
##################################################

FLAGSFILE=
RULESFILE=
OMROOT="D:\Program Files\IBM\Rational\Rhapsody\7.5\Share"

CPP_EXT=.cpp
H_EXT=.h
OBJ_EXT=.obj
EXE_EXT=.exe
LIB_EXT=.lib

INSTRUMENTATION=None

TIME_MODEL=RealTime

TARGET_TYPE=Executable

TARGET_NAME=DefaultComponent

all : $(TARGET_NAME)$(EXE_EXT) DefaultComponent.mak

TARGET_MAIN=MainDefaultComponent

LIBS=

INCLUDE_PATH= \
  $(INCLUDE_QUALIFIER)$(OMROOT)/LangCpp/osconfig/WIN32

ADDITIONAL_OBJS=

OBJS= \
  ibill_source\AboutFrm.obj \
  ibill_source\RegisterFrm.obj \
  ibill_source\OpenDialogFrm.obj \
  ibill_source\BillConfig.obj \
  ibill_source\BillFile.obj \
  ibill_source\BillField.obj \
  ibill_source\BillRecord.obj \
  ibill_source\FTPWatchThread.obj \
  ibill_source\FTPFile.obj \
  ibill_source\DBConfig.obj \
  ibill_source\SearchFileFrm.obj \
  ibill_source\OpenFTPDialogFrm.obj \
  ibill_source\FTPConfig.obj \
  ibill_source\MainFrm.obj \
  ibill_source\BillConfigFrm.obj \
  ibill_source\BillFormatFrm.obj \
  ibill_source\EditSearchParamFrm.obj \
  ibill_source\BillSearchThread.obj \
  ibill_source\RecordDetailFrm.obj \
  ibill_source\ViewBillFrm.obj \
  ibill_source\SearchFrm.obj \
  ibill_source\BillFileFrm.obj \
  ibill_source\ThreadConvertForDB.obj \
  ibill_source\ThreadSubmitToDB.obj \
  ibill_source\ExportToDBFrm.obj \
  ibill_source\ExportToFileThread.obj \
  ibill_source\ExportToFileFrm.obj \
  ibill_source\OpenFileThread.obj \
  ibill_source\OpeningFileFrm.obj \
  ibill_source\FieldFrm.obj \
  ibill_source\RecordFilterFrm.obj \
  ibill_source\DBConfigFrm.obj \
  ibill_source\EditDBFrm.obj \
  ibill_source\EditFilterFrm.obj \
  ibill_source\EditFTPFrm.obj \
  ibill_source\EditReplaceRuleFrm.obj \
  ibill_source\EditTransFrm.obj \
  ibill_source\ValueTransFrm.obj \
  ibill_source\FilterRptFrm.obj \
  ibill_source\FolderBrowseDialog.obj \
  ibill_source\FTPConfigFrm.obj \
  ibill_source\OptionFrm.obj \
  ibill_source\SelBillNameFrm.obj \
  ibill_source\MonitorFrm.obj \
  ibill_source\ProcessingFrm.obj \
  ibill_source\ReplaceRuleFrm.obj \
  ibill_source\ThreadExportToOra.obj \
  ibill_source\EditReadActionFrm.obj \
  ibill_source\ViewOriDataFrm.obj \
  ibill_source\public.obj \
  ibill_source\FolderBrowserDialog.obj \
  ibill_source\ibill.obj \
  ibill_source\ReplaceRultFrm.obj \
  ibill_source\TxtDataFrm.obj




OBJ_DIR=

!IF "$(OBJ_DIR)"!=""
CREATE_OBJ_DIR=if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
CLEAN_OBJ_DIR= if exist $(OBJ_DIR) $(RMDIR) $(OBJ_DIR)
!ELSE
CREATE_OBJ_DIR=
CLEAN_OBJ_DIR=
!ENDIF

######################## Predefined macros ############################
#######################################################################
$(OBJS) : $(INST_LIBS) $(OXF_LIBS)

LIB_POSTFIX=
!IF "$(BuildSet)"=="Release"
LIB_POSTFIX=R
!ENDIF

!IF "$(TARGET_TYPE)" == "Executable"
LinkDebug=$(LinkDebug) /DEBUG
LinkRelease=$(LinkRelease) /OPT:NOREF
!ELSEIF "$(TARGET_TYPE)" == "Library"
LinkDebug=$(LinkDebug) /DEBUGTYPE:CV
!ENDIF


!IF "$(INSTRUMENTATION)" == "Animation"
INST_FLAGS=/D "OMANIMATOR"
INST_INCLUDES=/I $(OMROOT)\LangCpp\aom /I $(OMROOT)\LangCpp\tom
!IF "$(RPFrameWorkDll)" == "True"
INST_LIBS= 
OXF_LIBS=$(OMROOT)\LangCpp\lib\$(LIB_PREFIX)oxfanimdll$(LIB_POSTFIX)$(LIB_EXT) 
!ELSE
INST_LIBS= $(OMROOT)\LangCpp\lib\$(LIB_PREFIX)aomanim$(LIB_POSTFIX)$(LIB_EXT)
OXF_LIBS=$(OMROOT)\LangCpp\lib\$(LIB_PREFIX)oxfinst$(LIB_POSTFIX)$(LIB_EXT) $(OMROOT)\LangCpp\lib\$(LIB_PREFIX)omComAppl$(LIB_POSTFIX)$(LIB_EXT) $(SimulinkLibName)
!ENDIF
SOCK_LIB=wsock32.lib

!ELSEIF "$(INSTRUMENTATION)" == "Tracing"
INST_FLAGS=/D "OMTRACER"
INST_INCLUDES=/I $(OMROOT)\LangCpp\aom /I $(OMROOT)\LangCpp\tom
!IF "$(RPFrameWorkDll)" == "True"
INST_LIBS=
OXF_LIBS= $(OMROOT)\LangCpp\lib\$(LIB_PREFIX)oxftracedll$(LIB_POSTFIX)$(LIB_EXT) 
!ELSE
INST_LIBS=$(OMROOT)\LangCpp\lib\$(LIB_PREFIX)tomtrace$(LIB_POSTFIX)$(LIB_EXT) $(OMROOT)\LangCpp\lib\$(LIB_PREFIX)aomtrace$(LIB_POSTFIX)$(LIB_EXT)
OXF_LIBS= $(OMROOT)\LangCpp\lib\$(LIB_PREFIX)oxfinst$(LIB_POSTFIX)$(LIB_EXT) $(OMROOT)\LangCpp\lib\$(LIB_PREFIX)omComAppl$(LIB_POSTFIX)$(LIB_EXT) $(SimulinkLibName)
!ENDIF
SOCK_LIB=wsock32.lib

!ELSEIF "$(INSTRUMENTATION)" == "None" 
INST_FLAGS=
INST_INCLUDES=
INST_LIBS=
!IF "$(RPFrameWorkDll)" == "True"
OXF_LIBS=$(OMROOT)\LangCpp\lib\$(LIB_PREFIX)oxfdll$(LIB_POSTFIX)$(LIB_EXT)
!ELSE
OXF_LIBS=$(OMROOT)\LangCpp\lib\$(LIB_PREFIX)oxf$(LIB_POSTFIX)$(LIB_EXT) $(SimulinkLibName)
!ENDIF
SOCK_LIB=

!ELSE
!ERROR An invalid Instrumentation $(INSTRUMENTATION) is specified.
!ENDIF



################## Generated dependencies ########################
##################################################################






ibill_source\AboutFrm.obj : ibill_source\AboutFrm.cpp ibill_source\AboutFrm.h    ibill_source\RegisterFrm.h ibill_source\MainFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\AboutFrm.obj" "ibill_source\AboutFrm.cpp" 



ibill_source\RegisterFrm.obj : ibill_source\RegisterFrm.cpp ibill_source\RegisterFrm.h    ibill_source\MainFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\RegisterFrm.obj" "ibill_source\RegisterFrm.cpp" 



ibill_source\OpenDialogFrm.obj : ibill_source\OpenDialogFrm.cpp ibill_source\OpenDialogFrm.h    ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\OpenDialogFrm.obj" "ibill_source\OpenDialogFrm.cpp" 



ibill_source\BillConfig.obj : ibill_source\BillConfig.cpp ibill_source\BillConfig.h    ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\BillConfig.obj" "ibill_source\BillConfig.cpp" 



ibill_source\BillFile.obj : ibill_source\BillFile.cpp ibill_source\BillFile.h    ibill_source\BillField.h ibill_source\FTPFile.h ibill_source\BillRecord.h ibill_source\BillConfig.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\BillFile.obj" "ibill_source\BillFile.cpp" 



ibill_source\BillField.obj : ibill_source\BillField.cpp ibill_source\BillField.h    ibill_source\BillFile.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\BillField.obj" "ibill_source\BillField.cpp" 



ibill_source\BillRecord.obj : ibill_source\BillRecord.cpp ibill_source\BillRecord.h    ibill_source\BillFile.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\BillRecord.obj" "ibill_source\BillRecord.cpp" 



ibill_source\FTPWatchThread.obj : ibill_source\FTPWatchThread.cpp ibill_source\FTPWatchThread.h    ibill_source\FTPFile.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\FTPWatchThread.obj" "ibill_source\FTPWatchThread.cpp" 



ibill_source\FTPFile.obj : ibill_source\FTPFile.cpp ibill_source\FTPFile.h    ibill_source\FTPWatchThread.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\FTPFile.obj" "ibill_source\FTPFile.cpp" 



ibill_source\DBConfig.obj : ibill_source\DBConfig.cpp ibill_source\DBConfig.h    ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\DBConfig.obj" "ibill_source\DBConfig.cpp" 



ibill_source\SearchFileFrm.obj : ibill_source\SearchFileFrm.cpp ibill_source\SearchFileFrm.h    
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\SearchFileFrm.obj" "ibill_source\SearchFileFrm.cpp" 



ibill_source\OpenFTPDialogFrm.obj : ibill_source\OpenFTPDialogFrm.cpp ibill_source\OpenFTPDialogFrm.h    ibill_source\SearchFileFrm.h ibill_source\MainFrm.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\OpenFTPDialogFrm.obj" "ibill_source\OpenFTPDialogFrm.cpp" 



ibill_source\FTPConfig.obj : ibill_source\FTPConfig.cpp ibill_source\FTPConfig.h    ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\FTPConfig.obj" "ibill_source\FTPConfig.cpp" 



ibill_source\MainFrm.obj : ibill_source\MainFrm.cpp ibill_source\MainFrm.h    ibill_source\OpenDialogFrm.h ibill_source\BillRecord.h ibill_source\DBConfig.h ibill_source\OpenFTPDialogFrm.h ibill_source\FTPConfig.h ibill_source\OptionFrm.h ibill_source\RegisterFrm.h ibill_source\cpuinfo.h ibill_source\DBConfigFrm.h ibill_source\BillConfigFrm.h ibill_source\BillFileFrm.h ibill_source\FTPConfigFrm.h ibill_source\AboutFrm.h ibill_source\ViewBillFrm.h ibill_source\SelBillNameFrm.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\MainFrm.obj" "ibill_source\MainFrm.cpp" 



ibill_source\BillConfigFrm.obj : ibill_source\BillConfigFrm.cpp ibill_source\BillConfigFrm.h    ibill_source\BillConfig.h ibill_source\MainFrm.h ibill_source\BillFormatFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\BillConfigFrm.obj" "ibill_source\BillConfigFrm.cpp" 



ibill_source\BillFormatFrm.obj : ibill_source\BillFormatFrm.cpp ibill_source\BillFormatFrm.h    ibill_source\BillConfig.h ibill_source\MainFrm.h ibill_source\FieldFrm.h ibill_source\RecordFilterFrm.h ibill_source\BillConfigFrm.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\BillFormatFrm.obj" "ibill_source\BillFormatFrm.cpp" 



ibill_source\EditSearchParamFrm.obj : ibill_source\EditSearchParamFrm.cpp ibill_source\EditSearchParamFrm.h    ibill_source\MainFrm.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\EditSearchParamFrm.obj" "ibill_source\EditSearchParamFrm.cpp" 



ibill_source\BillSearchThread.obj : ibill_source\BillSearchThread.cpp ibill_source\BillSearchThread.h    ibill_source\BillFile.h ibill_source\BillConfig.h ibill_source\MainFrm.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\BillSearchThread.obj" "ibill_source\BillSearchThread.cpp" 



ibill_source\RecordDetailFrm.obj : ibill_source\RecordDetailFrm.cpp ibill_source\RecordDetailFrm.h    ibill_source\BillFile.h ibill_source\MainFrm.h ibill_source\FieldFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\RecordDetailFrm.obj" "ibill_source\RecordDetailFrm.cpp" 



ibill_source\ViewBillFrm.obj : ibill_source\ViewBillFrm.cpp ibill_source\ViewBillFrm.h    ibill_source\BillFile.h ibill_source\BillConfig.h ibill_source\RecordDetailFrm.h ibill_source\MainFrm.h ibill_source\ExportToFileFrm.h ibill_source\ExportToDBFrm.h ibill_source\FilterRptFrm.h ibill_source\OpeningFileFrm.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ViewBillFrm.obj" "ibill_source\ViewBillFrm.cpp" 



ibill_source\SearchFrm.obj : ibill_source\SearchFrm.cpp ibill_source\SearchFrm.h    ibill_source\BillFile.h ibill_source\EditSearchParamFrm.h ibill_source\BillSearchThread.h ibill_source\ViewBillFrm.h ibill_source\MainFrm.h ibill_source\BillFileFrm.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\SearchFrm.obj" "ibill_source\SearchFrm.cpp" 



ibill_source\BillFileFrm.obj : ibill_source\BillFileFrm.cpp ibill_source\BillFileFrm.h    ibill_source\SearchFrm.h ibill_source\ViewBillFrm.h ibill_source\MainFrm.h ibill_source\ExportToDBFrm.h ibill_source\FTPFile.h ibill_source\ExportToFileFrm.h ibill_source\OpeningFileFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\BillFileFrm.obj" "ibill_source\BillFileFrm.cpp" 



ibill_source\ThreadConvertForDB.obj : ibill_source\ThreadConvertForDB.cpp ibill_source\ThreadConvertForDB.h    ibill_source\BillConfig.h ibill_source\BillFile.h ibill_source\BillRecord.h ibill_source\DBConfig.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ThreadConvertForDB.obj" "ibill_source\ThreadConvertForDB.cpp" 



ibill_source\ThreadSubmitToDB.obj : ibill_source\ThreadSubmitToDB.cpp ibill_source\ThreadSubmitToDB.h    ibill_source\DBConfig.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ThreadSubmitToDB.obj" "ibill_source\ThreadSubmitToDB.cpp" 



ibill_source\ExportToDBFrm.obj : ibill_source\ExportToDBFrm.cpp ibill_source\ExportToDBFrm.h    ibill_source\ThreadConvertForDB.h ibill_source\ThreadSubmitToDB.h ibill_source\MainFrm.h ibill_source\ViewBillFrm.h ibill_source\DBConfigFrm.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ExportToDBFrm.obj" "ibill_source\ExportToDBFrm.cpp" 



ibill_source\ExportToFileThread.obj : ibill_source\ExportToFileThread.cpp ibill_source\ExportToFileThread.h    ibill_source\BillFile.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ExportToFileThread.obj" "ibill_source\ExportToFileThread.cpp" 



ibill_source\ExportToFileFrm.obj : ibill_source\ExportToFileFrm.cpp ibill_source\ExportToFileFrm.h    ibill_source\ExportToFileThread.h ibill_source\MainFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ExportToFileFrm.obj" "ibill_source\ExportToFileFrm.cpp" 



ibill_source\OpenFileThread.obj : ibill_source\OpenFileThread.cpp ibill_source\OpenFileThread.h    ibill_source\BillFile.h ibill_source\MainFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\OpenFileThread.obj" "ibill_source\OpenFileThread.cpp" 



ibill_source\OpeningFileFrm.obj : ibill_source\OpeningFileFrm.cpp ibill_source\OpeningFileFrm.h    ibill_source\OpenFileThread.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\OpeningFileFrm.obj" "ibill_source\OpeningFileFrm.cpp" 



ibill_source\FieldFrm.obj : ibill_source\FieldFrm.cpp ibill_source\FieldFrm.h    ibill_source\BillConfig.h ibill_source\MainFrm.h ibill_source\BillFormatFrm.h ibill_source\ReplaceRuleFrm.h ibill_source\ValueTransFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\FieldFrm.obj" "ibill_source\FieldFrm.cpp" 



ibill_source\RecordFilterFrm.obj : ibill_source\RecordFilterFrm.cpp ibill_source\RecordFilterFrm.h    ibill_source\EditFilterFrm.h ibill_source\MainFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\RecordFilterFrm.obj" "ibill_source\RecordFilterFrm.cpp" 



ibill_source\DBConfigFrm.obj : ibill_source\DBConfigFrm.cpp ibill_source\DBConfigFrm.h    ibill_source\DBConfig.h ibill_source\MainFrm.h ibill_source\EditDBFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\DBConfigFrm.obj" "ibill_source\DBConfigFrm.cpp" 



ibill_source\EditDBFrm.obj : ibill_source\EditDBFrm.cpp ibill_source\EditDBFrm.h    ibill_source\DBConfig.h ibill_source\MainFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\EditDBFrm.obj" "ibill_source\EditDBFrm.cpp" 



ibill_source\EditFilterFrm.obj : ibill_source\EditFilterFrm.cpp ibill_source\EditFilterFrm.h    
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\EditFilterFrm.obj" "ibill_source\EditFilterFrm.cpp" 



ibill_source\EditFTPFrm.obj : ibill_source\EditFTPFrm.cpp ibill_source\EditFTPFrm.h    ibill_source\FTPConfig.h ibill_source\MainFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\EditFTPFrm.obj" "ibill_source\EditFTPFrm.cpp" 



ibill_source\EditReplaceRuleFrm.obj : ibill_source\EditReplaceRuleFrm.cpp ibill_source\EditReplaceRuleFrm.h    ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\EditReplaceRuleFrm.obj" "ibill_source\EditReplaceRuleFrm.cpp" 



ibill_source\EditTransFrm.obj : ibill_source\EditTransFrm.cpp ibill_source\EditTransFrm.h    
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\EditTransFrm.obj" "ibill_source\EditTransFrm.cpp" 



ibill_source\ValueTransFrm.obj : ibill_source\ValueTransFrm.cpp ibill_source\ValueTransFrm.h    ibill_source\MainFrm.h ibill_source\EditTransFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ValueTransFrm.obj" "ibill_source\ValueTransFrm.cpp" 



ibill_source\FilterRptFrm.obj : ibill_source\FilterRptFrm.cpp ibill_source\FilterRptFrm.h    ibill_source\MainFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\FilterRptFrm.obj" "ibill_source\FilterRptFrm.cpp" 



ibill_source\FolderBrowseDialog.obj : ibill_source\FolderBrowseDialog.cpp ibill_source\FolderBrowseDialog.h    
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\FolderBrowseDialog.obj" "ibill_source\FolderBrowseDialog.cpp" 



ibill_source\FTPConfigFrm.obj : ibill_source\FTPConfigFrm.cpp ibill_source\FTPConfigFrm.h    ibill_source\MainFrm.h ibill_source\EditFTPFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\FTPConfigFrm.obj" "ibill_source\FTPConfigFrm.cpp" 



ibill_source\OptionFrm.obj : ibill_source\OptionFrm.cpp ibill_source\OptionFrm.h    ibill_source\FolderBrowseDialog.h ibill_source\MainFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\OptionFrm.obj" "ibill_source\OptionFrm.cpp" 



ibill_source\SelBillNameFrm.obj : ibill_source\SelBillNameFrm.cpp ibill_source\SelBillNameFrm.h    ibill_source\MainFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\SelBillNameFrm.obj" "ibill_source\SelBillNameFrm.cpp" 



ibill_source\MonitorFrm.obj : ibill_source\MonitorFrm.cpp ibill_source\MonitorFrm.h    
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\MonitorFrm.obj" "ibill_source\MonitorFrm.cpp" 



ibill_source\ProcessingFrm.obj : ibill_source\ProcessingFrm.cpp ibill_source\ProcessingFrm.h    
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ProcessingFrm.obj" "ibill_source\ProcessingFrm.cpp" 



ibill_source\ReplaceRuleFrm.obj : ibill_source\ReplaceRuleFrm.cpp ibill_source\ReplaceRuleFrm.h    ibill_source\MainFrm.h ibill_source\EditReplaceRuleFrm.h ibill_source\public.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ReplaceRuleFrm.obj" "ibill_source\ReplaceRuleFrm.cpp" 



ibill_source\ThreadExportToOra.obj : ibill_source\ThreadExportToOra.cpp ibill_source\ThreadExportToOra.h    
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ThreadExportToOra.obj" "ibill_source\ThreadExportToOra.cpp" 



ibill_source\EditReadActionFrm.obj : ibill_source\EditReadActionFrm.cpp ibill_source\TxtDataFrm.h    
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\EditReadActionFrm.obj" "ibill_source\EditReadActionFrm.cpp" 



ibill_source\ViewOriDataFrm.obj : ibill_source\ViewOriDataFrm.cpp ibill_source\ViewOriDataFrm.h    ibill_source\MainFrm.h 
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ViewOriDataFrm.obj" "ibill_source\ViewOriDataFrm.cpp" 



ibill_source\public.obj : ibill_source\public.cpp ibill_source\public.h    
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\public.obj" "ibill_source\public.cpp" 



ibill_source\FolderBrowserDialog.obj : ibill_source\FolderBrowserDialog.cpp     
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\FolderBrowserDialog.obj" "ibill_source\FolderBrowserDialog.cpp" 



ibill_source\ibill.obj : ibill_source\ibill.cpp     
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ibill.obj" "ibill_source\ibill.cpp" 



ibill_source\ReplaceRultFrm.obj : ibill_source\ReplaceRultFrm.cpp     
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\ReplaceRultFrm.obj" "ibill_source\ReplaceRultFrm.cpp" 



ibill_source\TxtDataFrm.obj : ibill_source\TxtDataFrm.cpp     
	$(CREATE_OBJ_DIR)
	$(CPP) $(ConfigurationCPPCompileSwitches)  /Fo"ibill_source\TxtDataFrm.obj" "ibill_source\TxtDataFrm.cpp" 






$(TARGET_MAIN)$(OBJ_EXT) : $(TARGET_MAIN)$(CPP_EXT) $(OBJS) 
	$(CPP) $(ConfigurationCPPCompileSwitches) /Fo"$(TARGET_MAIN)$(OBJ_EXT)" $(TARGET_MAIN)$(CPP_EXT)

########################## Linking instructions ###############################
###############################################################################
$(TARGET_NAME)$(EXE_EXT): $(OBJS) $(ADDITIONAL_OBJS) $(TARGET_MAIN)$(OBJ_EXT) DefaultComponent.mak 
	@echo Linking $(TARGET_NAME)$(EXE_EXT)
	$(LINK_CMD)  $(TARGET_MAIN)$(OBJ_EXT) $(OBJS) $(ADDITIONAL_OBJS) \
	$(LIBS) \
	$(INST_LIBS) \
	$(OXF_LIBS) \
	$(SOCK_LIB) \
	$(WINMM_LIB) \
	$(LINK_FLAGS) /out:$(TARGET_NAME)$(EXE_EXT)

$(TARGET_NAME)$(LIB_EXT) : $(OBJS) $(ADDITIONAL_OBJS) DefaultComponent.mak
	@echo Building library $@
	$(LIB_CMD) $(LIB_FLAGS) /out:$(TARGET_NAME)$(LIB_EXT) $(OBJS) $(ADDITIONAL_OBJS) $(LIBS)

clean:
	@echo Cleanup
	if exist ibill_source\AboutFrm.obj erase ibill_source\AboutFrm.obj
	if exist ibill_source\RegisterFrm.obj erase ibill_source\RegisterFrm.obj
	if exist ibill_source\OpenDialogFrm.obj erase ibill_source\OpenDialogFrm.obj
	if exist ibill_source\BillConfig.obj erase ibill_source\BillConfig.obj
	if exist ibill_source\BillFile.obj erase ibill_source\BillFile.obj
	if exist ibill_source\BillField.obj erase ibill_source\BillField.obj
	if exist ibill_source\BillRecord.obj erase ibill_source\BillRecord.obj
	if exist ibill_source\FTPWatchThread.obj erase ibill_source\FTPWatchThread.obj
	if exist ibill_source\FTPFile.obj erase ibill_source\FTPFile.obj
	if exist ibill_source\DBConfig.obj erase ibill_source\DBConfig.obj
	if exist ibill_source\SearchFileFrm.obj erase ibill_source\SearchFileFrm.obj
	if exist ibill_source\OpenFTPDialogFrm.obj erase ibill_source\OpenFTPDialogFrm.obj
	if exist ibill_source\FTPConfig.obj erase ibill_source\FTPConfig.obj
	if exist ibill_source\MainFrm.obj erase ibill_source\MainFrm.obj
	if exist ibill_source\BillConfigFrm.obj erase ibill_source\BillConfigFrm.obj
	if exist ibill_source\BillFormatFrm.obj erase ibill_source\BillFormatFrm.obj
	if exist ibill_source\EditSearchParamFrm.obj erase ibill_source\EditSearchParamFrm.obj
	if exist ibill_source\BillSearchThread.obj erase ibill_source\BillSearchThread.obj
	if exist ibill_source\RecordDetailFrm.obj erase ibill_source\RecordDetailFrm.obj
	if exist ibill_source\ViewBillFrm.obj erase ibill_source\ViewBillFrm.obj
	if exist ibill_source\SearchFrm.obj erase ibill_source\SearchFrm.obj
	if exist ibill_source\BillFileFrm.obj erase ibill_source\BillFileFrm.obj
	if exist ibill_source\ThreadConvertForDB.obj erase ibill_source\ThreadConvertForDB.obj
	if exist ibill_source\ThreadSubmitToDB.obj erase ibill_source\ThreadSubmitToDB.obj
	if exist ibill_source\ExportToDBFrm.obj erase ibill_source\ExportToDBFrm.obj
	if exist ibill_source\ExportToFileThread.obj erase ibill_source\ExportToFileThread.obj
	if exist ibill_source\ExportToFileFrm.obj erase ibill_source\ExportToFileFrm.obj
	if exist ibill_source\OpenFileThread.obj erase ibill_source\OpenFileThread.obj
	if exist ibill_source\OpeningFileFrm.obj erase ibill_source\OpeningFileFrm.obj
	if exist ibill_source\FieldFrm.obj erase ibill_source\FieldFrm.obj
	if exist ibill_source\RecordFilterFrm.obj erase ibill_source\RecordFilterFrm.obj
	if exist ibill_source\DBConfigFrm.obj erase ibill_source\DBConfigFrm.obj
	if exist ibill_source\EditDBFrm.obj erase ibill_source\EditDBFrm.obj
	if exist ibill_source\EditFilterFrm.obj erase ibill_source\EditFilterFrm.obj
	if exist ibill_source\EditFTPFrm.obj erase ibill_source\EditFTPFrm.obj
	if exist ibill_source\EditReplaceRuleFrm.obj erase ibill_source\EditReplaceRuleFrm.obj
	if exist ibill_source\EditTransFrm.obj erase ibill_source\EditTransFrm.obj
	if exist ibill_source\ValueTransFrm.obj erase ibill_source\ValueTransFrm.obj
	if exist ibill_source\FilterRptFrm.obj erase ibill_source\FilterRptFrm.obj
	if exist ibill_source\FolderBrowseDialog.obj erase ibill_source\FolderBrowseDialog.obj
	if exist ibill_source\FTPConfigFrm.obj erase ibill_source\FTPConfigFrm.obj
	if exist ibill_source\OptionFrm.obj erase ibill_source\OptionFrm.obj
	if exist ibill_source\SelBillNameFrm.obj erase ibill_source\SelBillNameFrm.obj
	if exist ibill_source\MonitorFrm.obj erase ibill_source\MonitorFrm.obj
	if exist ibill_source\ProcessingFrm.obj erase ibill_source\ProcessingFrm.obj
	if exist ibill_source\ReplaceRuleFrm.obj erase ibill_source\ReplaceRuleFrm.obj
	if exist ibill_source\ThreadExportToOra.obj erase ibill_source\ThreadExportToOra.obj
	if exist ibill_source\EditReadActionFrm.obj erase ibill_source\EditReadActionFrm.obj
	if exist ibill_source\ViewOriDataFrm.obj erase ibill_source\ViewOriDataFrm.obj
	if exist ibill_source\public.obj erase ibill_source\public.obj
	if exist ibill_source\FolderBrowserDialog.obj erase ibill_source\FolderBrowserDialog.obj
	if exist ibill_source\ibill.obj erase ibill_source\ibill.obj
	if exist ibill_source\ReplaceRultFrm.obj erase ibill_source\ReplaceRultFrm.obj
	if exist ibill_source\TxtDataFrm.obj erase ibill_source\TxtDataFrm.obj
	if exist $(TARGET_MAIN)$(OBJ_EXT) erase $(TARGET_MAIN)$(OBJ_EXT)
	if exist *$(OBJ_EXT) erase *$(OBJ_EXT)
	if exist $(TARGET_NAME).pdb erase $(TARGET_NAME).pdb
	if exist $(TARGET_NAME)$(LIB_EXT) erase $(TARGET_NAME)$(LIB_EXT)
	if exist $(TARGET_NAME).ilk erase $(TARGET_NAME).ilk
	if exist $(TARGET_NAME)$(EXE_EXT) erase $(TARGET_NAME)$(EXE_EXT)
	$(CLEAN_OBJ_DIR)
