; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=Area_cmpt
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "OGLEvent.h"
LastPage=0

ClassCount=13
Class1=COGLEventApp
Class2=COGLEventDoc
Class3=COGLEventView
Class4=CMainFrame

ResourceCount=9
Resource1=IDD_VIEW_MAPPING
Class5=CAboutDlg
Resource2=IDD_AREA_CMPT
Class6=CFormCommandView1
Class7=CTexture
Resource3=IDR_MAINFRAME
Class8=ViewMap
Resource4=IDD_OPERATION
Class9=Txt_map
Resource5=IDD_ABOUTBOX
Class10=Operation
Resource6=IDD_txt_map
Class11=Data_input
Resource7=IDD_VLMN_CMPT
Class12=Area_cmpt
Resource8=IDD_DATA_INPUT
Class13=Vlmn_cmpt
Resource9=IDD_FORM_COMMAND (English (U.S.))

[CLS:COGLEventApp]
Type=0
HeaderFile=OGLEvent.h
ImplementationFile=OGLEvent.cpp
Filter=N
LastObject=COGLEventApp
BaseClass=CWinApp
VirtualFilter=AC

[CLS:COGLEventDoc]
Type=0
HeaderFile=OGLEventDoc.h
ImplementationFile=OGLEventDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=COGLEventDoc

[CLS:COGLEventView]
Type=0
HeaderFile=OGLEventView.h
ImplementationFile=OGLEventView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=COGLEventView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_AREA_CMPT
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=OGLEvent.cpp
ImplementationFile=OGLEvent.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_SAVE
Command2=ID_SAVE_DATA
Command3=ID_FRONT_VIEW
Command4=ID_RIGHT_VIEW
Command5=ID_LEFT_VIEW
Command6=ID_TXT_MAPPING
Command7=ID_FILE_PRINT
Command8=ID_FILE_PRINT_PREVIEW
Command9=ID_FILE_PRINT_SETUP
Command10=ID_APP_EXIT
Command11=ID_DATA_ALALYSIS
Command12=ID_READ_BKGD
Command13=ID_READ_FGD
Command14=ID_IMG_DIFF
Command15=ID_AREA_CMPT
Command16=ID_VLMN_CMPT
Command17=ID_VIEW_TOOLBAR
Command18=ID_VIEW_STATUS_BAR
Command19=ID_APP_ABOUT
CommandCount=19

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_FORM_COMMAND (English (U.S.))]
Type=1
Class=CFormCommandView1
ControlCount=50
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_CHECK_ANTIALIAS,button,1342242819
Control4=IDC_CHECK_SMOOTH,button,1342242819
Control5=IDC_CHECK_LIGHTING,button,1342242819
Control6=IDC_RADIO_MODEL_WIREFRAME,button,1342177289
Control7=IDC_RADIO_MODEL_FACE,button,1342177289
Control8=IDC_STATIC,button,1342177287
Control9=IDC_CHECK_VROTATION,button,1342242819
Control10=IDC_STATIC,button,1342177287
Control11=IDC_SLIDERSCALEX,msctls_trackbar32,1342242840
Control12=IDC_FRAME_COLOR_BACK,static,1342181383
Control13=IDC_STATIC,static,1342308608
Control14=IDC_STATIC,static,1342308352
Control15=IDC_FRAME_COLOR_LIGHT_SPECULAR,static,1342181383
Control16=IDC_FRAME_COLOR_LIGHT_DIFFUSE,static,1342181383
Control17=IDC_FRAME_COLOR_LIGHT_AMBIENT,static,1342181383
Control18=IDC_RADIO_MODEL_VERTEX,button,1342177289
Control19=IDC_FACE1,static,1342308608
Control20=IDC_FRAME_COLOR_FACE1,static,1342181383
Control21=IDC_CHECK_Rot_X,button,1342242819
Control22=IDC_CHECK_Rot_Y,button,1342242819
Control23=IDC_STATIC,button,1342177287
Control24=IDC_CHECK_Rot_Z,button,1342242819
Control25=IDC_STATIC,button,1342177287
Control26=IDC_RADIO_PROJ,button,1342177289
Control27=IDC_RADIO_ORTH,button,1342177289
Control28=IDC_CHECK_AXIS,button,1342242819
Control29=IDC_STATIC,button,1342177287
Control30=IDC_RADIO_PHOTO,button,1342177289
Control31=IDC_RADIO_COLOR,button,1342177289
Control32=IDC_CHECK_TXTMAP,button,1342242819
Control33=IDC_CHECK_MESHPROJ,button,1342242819
Control34=IDC_INVISIBLE,button,1342242816
Control35=IDC_PROJ_MAP,button,1342242816
Control36=IDC_NEXTVIEW,button,1342242816
Control37=IDC_SAVEIMAGE,button,1342242816
Control38=IDC_M11,static,1342308352
Control39=IDC_M13,static,1342308352
Control40=IDC_M14,static,1342308352
Control41=IDC_M12,static,1342308352
Control42=IDC_M21,static,1342308352
Control43=IDC_M23,static,1342308352
Control44=IDC_M24,static,1342308352
Control45=IDC_M22,static,1342308352
Control46=IDC_M31,static,1342308352
Control47=IDC_M33,static,1342308352
Control48=IDC_M34,static,1342308352
Control49=IDC_M32,static,1342308352
Control50=IDC_ERROR_SUM,static,1342308352

[CLS:CFormCommandView1]
Type=0
HeaderFile=FormCommandView1.h
ImplementationFile=FormCommandView1.cpp
BaseClass=CFormView
Filter=D
LastObject=IDC_ERROR_SUM
VirtualFilter=VWC

[CLS:CTexture]
Type=0
HeaderFile=Texture.h
ImplementationFile=Texture.cpp
BaseClass=CView
Filter=C
LastObject=CTexture
VirtualFilter=VWC

[DLG:IDD_VIEW_MAPPING]
Type=1
Class=ViewMap
ControlCount=3
Control1=IDOK,button,1342242817
Control2=ID_POSE_ADJ,button,1342242817
Control3=ID_PROJ_MAPPING,button,1342242817

[CLS:ViewMap]
Type=0
HeaderFile=ViewMap.h
ImplementationFile=ViewMap.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_POSE_ADJ
VirtualFilter=dWC

[DLG:IDD_txt_map]
Type=1
Class=Txt_map
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_WIDTH,static,1342308352
Control4=IDC_W,edit,1350631552

[CLS:Txt_map]
Type=0
HeaderFile=Txt_map.h
ImplementationFile=Txt_map.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_APP_ABOUT
VirtualFilter=dWC

[DLG:IDD_OPERATION]
Type=1
Class=Operation
ControlCount=21
Control1=IDC_STATIC,static,1342308352
Control2=IDC_READ_MODEL,button,1342242816
Control3=IDC_READ_IMAGE,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_PT_LOC_1,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_MTX_CPT,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_PT_LOC_2,button,1342242816
Control11=IDC_STATIC,static,1342308352
Control12=IDC_PRMT_OPT,button,1342242816
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342177294
Control15=IDC_STATIC,static,1342177294
Control16=IDC_MAP,button,1342242816
Control17=IDC_PXL_COMP,button,1342242816
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_ITERATIVE,button,1342242816
Control21=IDC_TXT_BLEND,button,1342242816

[CLS:Operation]
Type=0
HeaderFile=Operation.h
ImplementationFile=Operation.cpp
BaseClass=CFormView
Filter=D
LastObject=IDC_PXL_COMP

[DLG:IDD_DATA_INPUT]
Type=1
Class=Data_input
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352

[CLS:Data_input]
Type=0
HeaderFile=Data_input.h
ImplementationFile=Data_input.cpp
BaseClass=CDialog
Filter=D
LastObject=Data_input

[DLG:IDD_AREA_CMPT]
Type=1
Class=Area_cmpt
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_AREA,static,1342308352
Control3=IDC_AREA_VALUE,static,1342308352

[CLS:Area_cmpt]
Type=0
HeaderFile=Area_cmpt.h
ImplementationFile=Area_cmpt.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_VLMN_CMPT]
Type=1
Class=Vlmn_cmpt
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_VLMN,static,1342308352
Control3=IDC_VLMN_VALUE,static,1342308352

[CLS:Vlmn_cmpt]
Type=0
HeaderFile=Vlmn_cmpt.h
ImplementationFile=Vlmn_cmpt.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_VALUE_1

