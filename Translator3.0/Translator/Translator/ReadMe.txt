================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : Translator Project Overview
===============================================================================

The application wizard has created this Translator application for
you.  This application not only demonstrates the basics of using the Microsoft
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your Translator application.

Translator.vcxproj
    This is the main project file for VC++ projects generated using an application wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    application wizard.

Translator.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

Translator.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CTranslatorApp application class.

Translator.cpp
    This is the main application source file that contains the application
    class CTranslatorApp.

Translator.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

res\Translator.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file Translator.rc.

res\Translator.rc2
    This file contains resources that are not edited by Microsoft
    Visual C++. You should place all resources not editable by
    the resource editor in this file.


/////////////////////////////////////////////////////////////////////////////

The application wizard creates one dialog class:

TranslatorDlg.h, TranslatorDlg.cpp - the dialog
    These files contain your CTranslatorDlg class.  This class defines
    the behavior of your application's main dialog.  The dialog's template is
    in Translator.rc, which can be edited in Microsoft Visual C++.

/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named Translator.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, you will need
to redistribute the MFC DLLs. If your application is in a language
other than the operating system's locale, you will also have to
redistribute the corresponding localized resources mfc110XXX.DLL.
For more information on both of these topics, please see the section on
redistributing Visual C++ applications in MSDN documentation.

/////////////////////////////////////////////////////////////////////////////


	schedule:
1) Trans.h Trans.cpp
2) TransDlg.h TransDlg.cpp
3) CtrlFun.h CtrlFun.cpp


	What is difficult for me:
1) I need to write Trans.cpp which is contain google translate API, but google translate have a key to verify whether this request come from explorer, and different word at the different time with different key algorithm. so I need some time to analyse it(Maybe I need one day).
2) I'll use the new IDE which is called Visual Studio, It will take me some time to know that.
3) I must use Google to find what I need, It means I must use english(It'll take me much time)
4) others ...... 


	recording
1) the first day is familiar with the visaul studio, find data in google, translate english words.. using too much time.
