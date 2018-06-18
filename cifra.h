// ======================================================================
// Creation: 09/11/2017 - Guto Vannini
// Update  : 15/03/2018 - Guto Vannini



// ======================================================================
#ifndef CIFRA_H_
#define CIFRA_H_

// ======================================================================
// headers

// ----------------------------------------------------------------------
// For compilers that support precompilation, includes "wx/wx.h"
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/html/htmlwin.h>

// ----------------------------------------------------------------------
#define NONE -1

#define TITLE 0
#define ORIGINAL 1
#define WRITER 2
#define VERSION 3
#define ORDER 4
#define INTRO 5
#define CHORDS 6
#define STROPHE 7
#define REFRAIN 8
#define PLAINTEXT 9
#define INFO 10

#define SECTIONCOUNT 11

#define FONT 0
#define SIZE 1
#define STYLE 2
#define COLOR 3
#define ALIGN 4

#define CONFIGCOUNT 5

#if defined(__WXMSW__)
#define SEP '\\'
#else
#define SEP '/'
#endif



// ======================================================================
// cifra class
class Cifra
{
 public:
  Cifra();
  wxString      GetConfig(int section, int item);
  wxString      GetFile();
  wxString      GetHTML();
  wxString      GetProfileName();
  wxArrayString GetSectionNames();
  wxString      GetTitle();
  wxString      GetText();
  bool          Load(wxString folder);
  void          New();
  bool          ProfileLoad(wxString name);
  bool          ProfileSave(wxString name);
  bool          Save();
  bool          SavePDF();
  bool          SavePDFBook();
  void          SetConfig(int section, int item, wxString value);
  void          SetText(wxString text);
  void          SetKeyShift(int shift);

 private:
  void     Generate();
  wxString Modulation(wxString line);

  wxArrayString align_;
  wxArrayString color_;
  wxArrayString data_;
  wxArrayString font_;
  wxArrayString size_;
  wxArrayString style_;
  wxArrayString sectionname_;

  wxString      file_;
  wxString      folder_;
  wxString      html_;
  wxString      profile_;
  int           shift_;
  int           tabsize_;
  wxString      text_;
};



// ======================================================================
// application
class CifraApp : public wxApp
{
 public:
  CifraApp();
  virtual bool OnInit();
  virtual int  OnExit();
};

// ----------------------------------------------------------------------
// wxwidgets macros
DECLARE_APP(CifraApp)



// ======================================================================
// user interface
class CifraWindow : public wxFrame
{
 public:
  CifraWindow();

 protected:
  void Config(wxCommandEvent& event);
  void EventConnect(wxString pName, int pEvt,
		    void (wxObject::*pMethod)(wxEvent&));
  void Exit(wxCommandEvent& event);
  void Help(wxCommandEvent& event);
  void New(wxCommandEvent& event);
  void OnMouseEvent(wxMouseEvent& event);
  void Open(wxCommandEvent& event);
  void PDF(wxCommandEvent& event);
  void PDFBook(wxCommandEvent& event);
  void Save(wxCommandEvent& event);
  void SetTitle();
  void ShiftKeyDown(wxCommandEvent& event);
  void ShiftKeyUp(wxCommandEvent& event);
  void View(wxCommandEvent& event);

 private:
  wxHtmlWindow* html_;
  int           height_;
  wxTextCtrl*   text_;
  int           width_;
};

#endif  // CIFRA_H_
