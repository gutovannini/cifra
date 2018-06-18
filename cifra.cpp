// ======================================================================
// Creation: 09/11/2017 - Guto Vannini
// Update  : 15/03/2018 - Guto Vannini
// Obs     :



// ======================================================================
// headers

// ----------------------------------------------------------------------
#include "cifra.h"
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/fileconf.h>



// ======================================================================
// icon

// ----------------------------------------------------------------------
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || \
    defined(__WXMGL__) || defined(__WXX11__)
#include "icone.xpm"
#endif


// ======================================================================
// functions

// ----------------------------------------------------------------------
char* wxs2ascii(wxString pString)
{
  // ----- local variables
  wxString xStr;
  char*    xAux;

  if (pString == "")
    return NULL;

  xStr.Clear();
  xStr.Alloc(pString.Len());
  for (int i=0; i<(int)pString.Len(); i++)
    switch (pString.GetChar(i))
      {
      case 'á':
      case 'ã':
      case 'à':
      case 'â':
      case 'ä':
	xStr += "a";
	break;
      case 'Á':
      case 'Ã':
      case 'À':
      case 'Â':
      case 'Ä':
	xStr += "A";
	break;
      case 'é':
      case 'ê':
	xStr += "e";
	break;
      case 'É':
      case 'Ê':
	xStr += "E";
	break;
      case 'í':
	xStr += "i";
	break;
      case 'Í':
	xStr += "I";
	break;
      case 'ó':
      case 'ô':
      case 'õ':
	xStr += "o";
	break;
      case 'Ó':
      case 'Ô':
      case 'Õ':
	xStr += "O";
	break;
      case 'ú':
      case 'ü':
	xStr += "u";
	break;
      case 'Ú':
      case 'Ü':
	xStr += "U";
	break;
      case 'ç':
	xStr += "c";
	break;
      case 'Ç':
	xStr += "C";
	break;
      case 'ª':
	xStr += "a";
	break;
      case 'º':
      case '°':
	xStr += "o";
	break;
      default:
	if (!pString(i, 1).IsAscii())
	  xStr += "?";
	else
	  xStr += pString.Mid(i, 1);
      }

  xAux = (char*)malloc((int)xStr.Len()+2);
  strcpy(xAux, (char*)xStr.c_str());
  return xAux;
}

// ----------------------------------------------------------------------
wxString i2wxs(int pInt)
{
  wxString aux;

  aux = wxString::Format("%d", pInt);
  return aux;
}

wxString i2wxs(int pInt, const char* pFormat)
{
  wxString aux;

 aux = wxString::Format(pFormat, pInt);
 return aux;
}

// ----------------------------------------------------------------------
wxString RepeatS(wxString pRepet, short pQtd)
{
  short    i;
  wxString aux;

  aux = "";
  for (i=0; i<pQtd; i++)
    aux += pRepet;

  return aux;
}

// ----------------------------------------------------------------------
bool ConvertHtml2Pdf(wxString pFromFiles, wxString pToFile)
{
  wxArrayString xArr;
  wxArrayString xErr;
  wxString      xStr;
  wxString      xFrom;
  wxString      xApp;
  wxFileSystem  xFS;

  // ----- -----
  xFrom = "";
  if (pFromFiles.Contains("*"))
    {
      xApp = xFS.FindFirst(pFromFiles);
      while (!xApp.IsEmpty())
	{
	  xArr.Add(xApp);
	  xApp = xFS.FindNext();
	}
      xArr.Sort();
      for (int i=0; i<(int)xArr.GetCount(); i++)
	if (xArr[i].Find(" ") != wxNOT_FOUND)
	  xFrom += "\"" + xArr[i] + "\" ";
	else
	  xFrom += xArr[i] + " ";
    }
  else
    {
      if (wxFileExists(pFromFiles))
	xFrom = pFromFiles;
    }
  if (xFrom.IsEmpty())
    return false;

  // ----- -----
  xStr  = "wkhtmltopdf ";
#if defined(__WXGTK__)
  xStr += "--use-xserver ";
#endif
  xStr += "--disable-smart-shrinking ";
  //xStr += "--zoom 1.1 ";
  xStr += "-q ";                 // be less verbose
  xStr += "-s A4 ";              // set paper size to A4
  xStr += "--margin-top 5 ";     // set page top margin in mm
  xStr += "--margin-left 7 ";    // set page left margin in mm
  xStr += "--margin-right 7 ";   // set page right margin in mm
  xStr += "--margin-bottom 7 ";  // set page bottom margin in mm
  xStr += xFrom + " ";
  xStr += pToFile;

  // ----- -----
  wxRemoveFile(pToFile);
  wxExecute(xStr, xArr, xErr);
  xStr = "";
  for (int i=0; i<(int)xErr.GetCount(); ++i)
    if (!xErr[i].IsEmpty())
      xStr += xErr[i] + "\n";
  if (!xStr.IsEmpty() && !wxFileExists(pToFile))
    {
      wxMessageBox("erro wkhtmltopdf:\n" + xStr, "Atenção",
		   wxOK | wxICON_EXCLAMATION);
      return false;
    }

  // ----- -----
  if (pFromFiles.Contains("*"))
    {
      xApp = xFS.FindFirst(pFromFiles);
      while (!xApp.IsEmpty())
	{
	  wxRemoveFile(xApp);
	  xApp = xFS.FindNext();
	}
    }
  else
    wxRemoveFile(pFromFiles);

  return true;
}

// ----------------------------------------------------------------------
bool FileRead(wxString filename, wxString& filedata)
{
  // ----- -----
  wxFile         xfile;
  int            xfilesize;
  int            xreadsize;
  unsigned char* xbuffer;

  // ----- -----
  if (!xfile.Open(filename, wxFile::read))
    return false;
  xfilesize = xfile.Length();
  xbuffer   = new unsigned char[xfilesize+1];
  xreadsize = xfile.Read(xbuffer, xfilesize);
  xfile.Close();
  if (xfilesize != xreadsize)
    return false;
  filedata.Clear();
  filedata.Alloc(xfilesize);
  filedata = xbuffer;
  filedata = filedata.Left(xfilesize);

  return true;
}

// ----------------------------------------------------------------------
bool FileWrite(wxString filename, const wxString& filedata)
{
  // ----- -----
  wxFile xfile;

  // ----- -----
  if (!xfile.Open(filename, wxFile::write))
    return false;
  xfile.Write(filedata, filedata.Len());
  xfile.Close();

  return true;
}

// ----------------------------------------------------------------------
wxString OneWord(wxString phrase)
{
  // ----- local variables
  wxString temp;

  // ----- remove non-ascii characters (> 127)
  temp = wxs2ascii(phrase);

  // ----- remove wildcards and some special file name characters
  temp.Replace("?", "");
  temp.Replace("*", "");
  temp.Replace("/", "");
  temp.Replace("\\", "");
  temp.Replace(",", " ");

  // ----- lower case
  temp.MakeLower();

  // ----- remove double spaces
  while (temp.Replace("  ", " "));

  // ----- capitalize
  for (int i=0; i<(int)temp.Len(); i++)
    if (temp[i-1] == ' ' || temp[i-1] == '(' || temp[i-1] == '-')
      if (temp[i] >= 97)
	temp[i] = temp[i] - 32;
  // ----- capitalize first character
  if (temp[0] >= 97)
    temp[0] = temp[0] - 32;

  // ----- remove spaces
  temp.Replace(" ", "");

  return temp;
}



// ======================================================================
// global variables

Cifra* cifra = NULL;



// ======================================================================
// application

// ----------------------------------------------------------------------
IMPLEMENT_APP(CifraApp)

// ----------------------------------------------------------------------
CifraApp::CifraApp()
{
  // -----
  wxInitAllImageHandlers();
}

// ----------------------------------------------------------------------
bool CifraApp::OnInit()
{
  // ----- show window: linux=fullscreen_all
  CifraWindow* frame = new CifraWindow();
#if defined(__WXMSW__)
  frame->Show(true);
#else
  frame->ShowFullScreen(true, wxFULLSCREEN_ALL);
#endif

  return true;
}

// ----------------------------------------------------------------------
int CifraApp::OnExit()
{
  return 0;
}



// ======================================================================
// user interface

// ----------------------------------------------------------------------
CifraWindow::CifraWindow()
  : wxFrame(NULL, wxID_ANY, "Cifra", wxPoint(0, 0),
	    wxDefaultSize, wxBORDER_NONE)
{
  // -----
  wxDisplaySize(&width_, &height_);
  SetSize(width_, height_);
  SetIcon(wxICON(icone));
  SetBackgroundColour(*wxWHITE);

  // -----
  wxWindow* xObj;
  wxFont    xFont;
  xObj = (wxWindow*)new wxStaticText(this, wxID_ANY, "CIFRA",
				     wxPoint(5, 5), wxDefaultSize,
				     0, "lblTitle");
  xFont = xObj->GetFont();
  xFont.SetWeight(wxBOLD);
  xObj->SetFont(xFont);
  xObj = (wxWindow*)new wxButton(this, wxID_ANY, "New",
				 wxPoint(0, 25), wxSize(100, 25),
				 0, wxDefaultValidator, "btnNew");
  xObj->SetFont(xFont);
  xObj = (wxWindow*)new wxButton(this, wxID_ANY, "Open",
				 wxPoint(0, 50), wxSize(100, 25),
				 0, wxDefaultValidator, "btnOpen");
  xObj->SetFont(xFont);
  xObj = (wxWindow*)new wxButton(this, wxID_ANY, "Save",
				 wxPoint(0, 75), wxSize(100, 25),
				 0, wxDefaultValidator, "btnSave");
  xObj->SetFont(xFont);
  xObj = (wxWindow*)new wxButton(this, wxID_ANY, "Options",
				 wxPoint(0, 100), wxSize(100, 25),
				 0, wxDefaultValidator, "btnOption");
  xObj->SetFont(xFont);
  xObj = (wxWindow*)new wxButton(this, wxID_ANY, "b <<",
				 wxPoint(0, 125), wxSize(50, 25),
				 0, wxDefaultValidator, "btnDownKey");
  xObj->SetFont(xFont);
  xObj = (wxWindow*)new wxButton(this, wxID_ANY, ">> #",
				 wxPoint(50, 125), wxSize(50, 25),
				 0, wxDefaultValidator, "btnUpKey");
  xObj->SetFont(xFont);
  xObj = (wxWindow*)new wxButton(this, wxID_ANY, "View",
				 wxPoint(0, 150), wxSize(100, 25),
				 0, wxDefaultValidator, "btnView");
  xObj->SetFont(xFont);
  xObj = (wxWindow*)new wxButton(this, wxID_ANY, "PDF Sheet",
				 wxPoint(0, 175), wxSize(100, 25),
				 0, wxDefaultValidator, "btnPDF");
  xObj->SetFont(xFont);
  xObj = (wxWindow*)new wxButton(this, wxID_ANY, "PDF Book",
				 wxPoint(0, 200), wxSize(100, 25),
				 0, wxDefaultValidator, "btnBook");
  xObj->SetFont(xFont);
  xObj = (wxWindow*)new wxButton(this, wxID_ANY, "Exit",
				 wxPoint(0, 225), wxSize(100, 25),
				 0, wxDefaultValidator, "btnExit");
  xObj->SetFont(xFont);
  xObj = (wxWindow*)new wxButton(this, wxID_ANY, "Help",
				 wxPoint(0, 250), wxSize(100, 25),
				 0, wxDefaultValidator, "btnHelp");
  xObj->SetFont(xFont);
  text_ = new wxTextCtrl(this, wxID_ANY, "",
			 wxPoint(100, 25), wxSize(width_/2-100,height_-50),
			 wxTE_MULTILINE, wxDefaultValidator, "txtText");
  xFont = text_->GetFont();
  xFont.SetFaceName("Liberation Mono");
  text_->SetFont(xFont);
  html_ = new wxHtmlWindow(this, wxID_ANY, wxPoint(width_/2, 25),
			   wxSize(width_/2-100, height_-50));
  if (wxFileExists("help.html"))
    html_->LoadFile(wxFileName("help.html"));

  // -----
  Connect(GetId(), wxEVT_LEFT_DOWN, 
	  wxMouseEventHandler(CifraWindow::OnMouseEvent));
  EventConnect("btnExit", wxEVT_COMMAND_BUTTON_CLICKED, 
               wxCommandEventHandler(CifraWindow::Exit));
  EventConnect("btnNew",  wxEVT_COMMAND_BUTTON_CLICKED, 
               wxCommandEventHandler(CifraWindow::New));
  EventConnect("btnOpen", wxEVT_COMMAND_BUTTON_CLICKED, 
               wxCommandEventHandler(CifraWindow::Open));
  EventConnect("btnSave", wxEVT_COMMAND_BUTTON_CLICKED, 
               wxCommandEventHandler(CifraWindow::Save));
  EventConnect("btnDownKey", wxEVT_COMMAND_BUTTON_CLICKED, 
               wxCommandEventHandler(CifraWindow::ShiftKeyDown));
  EventConnect("btnUpKey", wxEVT_COMMAND_BUTTON_CLICKED, 
               wxCommandEventHandler(CifraWindow::ShiftKeyUp));
  EventConnect("btnView", wxEVT_COMMAND_BUTTON_CLICKED, 
               wxCommandEventHandler(CifraWindow::View));
  EventConnect("btnPDF",  wxEVT_COMMAND_BUTTON_CLICKED, 
               wxCommandEventHandler(CifraWindow::PDF));
  EventConnect("btnBook", wxEVT_COMMAND_BUTTON_CLICKED, 
               wxCommandEventHandler(CifraWindow::PDFBook));
  EventConnect("btnHelp", wxEVT_COMMAND_BUTTON_CLICKED, 
               wxCommandEventHandler(CifraWindow::Help));

  // -----
  cifra = new Cifra;
}

// ----------------------------------------------------------------------
void CifraWindow::EventConnect(wxString pName, int pEvt,
			       void (wxObject::*pMethod)(wxEvent&))
{
  wxWindow* xWin;
  xWin = FindWindow(pName);
  if (xWin)
    Connect(xWin->GetId(), pEvt, pMethod);
}

// ----------------------------------------------------------------------
void CifraWindow::Exit(wxCommandEvent& WXUNUSED(event))
{
  delete cifra;
  wxTheApp->Exit();
}

// ----------------------------------------------------------------------
void CifraWindow::Help(wxCommandEvent& WXUNUSED(event))
{
  if (wxFileExists("help.html"))
    html_->LoadFile(wxFileName("help.html"));
}

// ----------------------------------------------------------------------
void CifraWindow::New(wxCommandEvent& WXUNUSED(event))
{
  cifra->New();
  SetTitle();
}

// ----------------------------------------------------------------------
void CifraWindow::OnMouseEvent(wxMouseEvent& event)
{
  // -----
  wxClientDC dc(this);

  // ----- left click = select
  if (event.LeftDown())
    if (event.GetPosition() == wxPoint(0, 0))
      wxTheApp->Exit();
}

// ----------------------------------------------------------------------
void CifraWindow::Open(wxCommandEvent& WXUNUSED(event))
{
  // -----
  wxString folder;
  folder = wxGetCwd();
  folder = wxDirSelector("Open:", folder);
  if (folder.IsEmpty())
    return;

  // -----
  cifra->Load(folder);
  SetTitle();
}

// ----------------------------------------------------------------------
void CifraWindow::PDF(wxCommandEvent& WXUNUSED(event))
{
  wxBeginBusyCursor();
  cifra->SetText(text_->GetValue());
  cifra->SavePDF();
  wxEndBusyCursor();
}

// ----------------------------------------------------------------------
void CifraWindow::PDFBook(wxCommandEvent& WXUNUSED(event))
{
  wxBeginBusyCursor();
  cifra->SavePDFBook();
  wxEndBusyCursor();
}

// ----------------------------------------------------------------------
void CifraWindow::Save(wxCommandEvent& WXUNUSED(event))
{
  cifra->SetText(text_->GetValue());
  cifra->Save();
  SetTitle();
}

// ----------------------------------------------------------------------
void CifraWindow::SetTitle()
{
  // -----
  wxWindow* xWin;
  xWin = FindWindow("lblTitle");
  if (xWin)
    ((wxStaticText*)xWin)->SetLabel("CIFRA: " + cifra->GetTitle() +
				    " (" + cifra->GetFile() + ")");

  text_->SetValue(cifra->GetText());
  html_->SetPage(cifra->GetHTML());
}

// ----------------------------------------------------------------------
void CifraWindow::ShiftKeyDown(wxCommandEvent& WXUNUSED(event))
{
  cifra->SetText(text_->GetValue());
  cifra->SetKeyShift(-1);
  html_->SetPage(cifra->GetHTML());
}

// ----------------------------------------------------------------------
void CifraWindow::ShiftKeyUp(wxCommandEvent& WXUNUSED(event))
{
  cifra->SetText(text_->GetValue());
  cifra->SetKeyShift(1);
  html_->SetPage(cifra->GetHTML());
}

// ----------------------------------------------------------------------
void CifraWindow::View(wxCommandEvent& WXUNUSED(event))
{
  cifra->SetText(text_->GetValue());
  html_->SetPage(cifra->GetHTML());
}



// ======================================================================
// cifra class

// ----------------------------------------------------------------------
Cifra::Cifra()
{
  shift_   = 0;
  tabsize_ = 4;

  align_.Add("", SECTIONCOUNT);
  color_.Add("", SECTIONCOUNT);
  data_.Add("",  SECTIONCOUNT);
  font_.Add("",  SECTIONCOUNT);
  size_.Add("",  SECTIONCOUNT);
  style_.Add("", SECTIONCOUNT);
  sectionname_.Add("", SECTIONCOUNT);

  // ----- TITLE
  align_[TITLE]    = "center";
  font_[TITLE]     = "Liberation Mono";
  size_[TITLE]     = "16";
  style_[TITLE]    = "b";
  sectionname_[TITLE] = "Title";

  // ----- ORIGINAL
  align_[ORIGINAL] = "center";
  font_[ORIGINAL]  = "Liberation Mono";
  size_[ORIGINAL]  = "10";
  style_[ORIGINAL] = "bi";
  sectionname_[ORIGINAL] = "Original";

  // ----- WRITER
  align_[WRITER]   = "center";
  font_[WRITER]    = "Liberation Mono";
  size_[WRITER]    = "12";
  style_[WRITER]   = "b";
  sectionname_[WRITER] = "Writer";

  // ----- VERSION
  align_[VERSION]  = "center";
  font_[VERSION]   = "Liberation Mono";
  size_[VERSION]   = "10";
  style_[VERSION]  = "i";
  sectionname_[VERSION] = "Version";

  // ----- ORDER
  font_[ORDER]     = "Liberation Mono";
  size_[ORDER]     = "14";
  style_[ORDER]    = "b";
  sectionname_[ORDER] = "Order";

  // ----- INTRO
  font_[INTRO]     = "Liberation Mono";
  size_[INTRO]     = "14";
  color_[INTRO]    = "FF0000";
  style_[INTRO]    = "b";
  sectionname_[INTRO] = "Intro";

  // ----- CHORDS
  font_[CHORDS]    = "Liberation Mono";
  size_[CHORDS]    = "14";
  color_[CHORDS]   = "FF0000";
  style_[CHORDS]   = "b";
  sectionname_[CHORDS] = "Chords";

  // ----- STROPHE
  font_[STROPHE]   = "Liberation Mono";
  size_[STROPHE]   = "14";
  sectionname_[STROPHE] = "Strophe";

  // ----- REFRAIN
  font_[REFRAIN]   = "Liberation Mono";
  size_[REFRAIN]   = "14";
  style_[REFRAIN]  = "i";
  sectionname_[REFRAIN] = "Refrain";

  // ----- PLAINTEXT
  font_[PLAINTEXT] = "Liberation Mono";
  size_[PLAINTEXT] = "14";
  sectionname_[PLAINTEXT] = "Plain text";

  // ----- INFO
  font_[INFO]      = "Liberation Mono";
  size_[INFO]      = "10";
  sectionname_[INFO] = "Info";

  ProfileLoad("Default");
}

// ----------------------------------------------------------------------
void Cifra::Generate()
{
  // -----
  wxString      xstr;
  wxArrayString xtext;
  wxArrayInt    xtype;
  wxString      xline;
  int           xint;

  // -----
  if (text_.IsEmpty())
    {
      html_ = "";
      return;
    }

  // ----- break text into separate lines
  xstr = text_;
  while (!xstr.IsEmpty())
    {
      // ----- get one text line
      xline = xstr.BeforeFirst('\n');
      xstr  = xstr.AfterFirst('\n');

      // ----- convert tabs to spaces
      xint = xline.Find('\t');
      while (xint != wxNOT_FOUND)
	{
	  xint = tabsize_ - (xint % tabsize_);
	  xline.Replace("\t", RepeatS(" ", xint), false);
	  xint = xline.Find('\t');
	}

      // ----- append to array of lines/types
      xtext.Add(xline);
      xtype.Add(NONE);
    }

  /*
  xstr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  xstr = "abcdefghijklmnopqrstuvwxyz";
  xstr = "0123456789";
  xstr = "#/º°";
  //*/

  // ----- determine the type of a line
  data_.Empty();
  data_.Add("", SECTIONCOUNT);
  for (int i=0; i<(int)xtext.GetCount(); i++)
    {
      // -----
      xint = xtext[i].Find(':');
      if (xint != wxNOT_FOUND)
	{
	  xstr = xtext[i].Left(xint);
	  xstr.MakeLower();
	  xstr = wxs2ascii(xstr);
	  if (xstr == "tit" || xstr == "title")
	    {
	      xtext[i] = xtext[i].AfterFirst(':').Trim(false).Trim(true);
	      xtype[i] = TITLE;
	      data_[TITLE] = xtext[i];
	      continue;
	    }
	  else if (xstr == "comp" || xstr == "composer")
	    {
	      xtext[i] = xtext[i].AfterFirst(':').Trim(false).Trim(true);
	      xtype[i] = WRITER;
	      continue;
	    }
	  /*
	  else if (xstr == "int" || xstr == "interprete")
	    {
	    }
	  //*/
	  else if (xstr == "intro" || xstr == "introduction")
	    {
	      xtext[i] = xtext[i].Trim(false).Trim(true);
	      xtype[i] = INTRO;
	      continue;
	    }
	  else if (xstr == "ord" || xstr == "order")
	    {
	      xtext[i] = xtext[i].Trim(false).Trim(true);
	      xtype[i] = ORDER;
	      continue;
	    }
	  else if (xstr == "orig" || xstr == "original")
	    {
	      xtext[i] = xtext[i].AfterFirst(':').Trim(false).Trim(true);
	      xtype[i] = ORIGINAL;
	      continue;
	    }
	  else if (xstr == "ver" || xstr == "version")
	    {
	      xtext[i] = xtext[i].AfterFirst(':').Trim(false).Trim(true);
	      xtype[i] = VERSION;
	      continue;
	    }
	}

      // ----- STROPHE
      xstr = "acdefghijklnopqrtvwyz";
      for (int j=0; j<(int)xstr.Len(); j++)
	if (xtext[i].Find(xstr[j]) != wxNOT_FOUND)
	  {
	    xtype[i] = STROPHE;
	    break;
	  }
      xstr = "HIJKLNOPQRSTUVWXYZ";
      for (int j=0; j<(int)xstr.Len(); j++)
	if (xtext[i].Find(xstr[j]) != wxNOT_FOUND)
	  {
	    xtype[i] = STROPHE;
	    break;
	  }

      // ----- CHORDS
      if (xtype[i] == NONE)
	xtype[i] = CHORDS;
    }
  // -----
  if (data_[TITLE].IsEmpty())
    {
      xtype[0] = TITLE;
      data_[TITLE] = xtext[0];
    }

  // ----- key shift
  if (shift_ != 0)
    for (int i=0; i<(int)xtext.GetCount(); i++)
      if (xtype[i] == INTRO || xtype[i] == CHORDS)
	xtext[i] = Modulation(xtext[i]);

  // -----
  html_  = "<html>";

  // -----
  html_ += "<head>";

  // -----
  html_ += "<style type=\"text/css\">";
  html_ += "<!-- ";

  // -----
  for (int i=0; i<SECTIONCOUNT; i++)
    {
      html_ += ".f" + i2wxs(i);
      html_ += " {";
      if (!font_[i].IsEmpty())
	{
	  xstr = font_[i];
	  if (xstr.Contains(" "))
	    xstr = "\"" + xstr + "\"";
	  html_ += "font-family: " + xstr + "; ";
	}
      if (!size_[i].IsEmpty())
	{
	  html_ += "font-size: " + size_[i] + "pt; ";
	  //html_ += "line-height: " + size_[i] + "pt; ";
	}
      html_ += "padding: 0;} ";
    }
  html_ += "-->";
  html_ += "</style>";

  html_ += "</head>";

  // -----
  html_ += "<body>";

  // ----- default font face, size and color
  if (!font_[PLAINTEXT].IsEmpty()  || !size_[PLAINTEXT].IsEmpty() ||
      !color_[PLAINTEXT].IsEmpty())
    {
      html_ += "<font";
      if (!font_[PLAINTEXT].IsEmpty())
	html_ += " face='" + font_[PLAINTEXT] + "'";
      if (!size_[PLAINTEXT].IsEmpty())
	html_ += " size='" + i2wxs(5) /*size_[PLAINTEXT]*/ + "'";
      if (!color_[PLAINTEXT].IsEmpty())
	html_ += " color='#" + color_[PLAINTEXT] + "'";
      html_ += ">";
    }

  // -----
  html_ += "<table border='0' cellpadding='0' cellspacing='0' width='100%'";
  html_ += " style='";
  html_ += "table-layout: fixed; ";
  html_ += "border-collapse: collapse;";
  html_ += "'>";

  // -----
  for (int i=0; i<(int)xtext.GetCount(); i++)
    {
      // -----
      xint = xtype[i];

      // ----- begin table row and col
      html_ += "<tr";
      if (!align_[xint].IsEmpty())
	html_ += " align='" + align_[xint] + "'";
      html_ += ">";
      html_ += "<td>";

      // -----
      html_ += "<p";
      html_ += " class=f" + i2wxs(xint);
      html_ += ">";

      // ----- begin font face, size and color
      if (!color_[xint].IsEmpty())
	{
	  html_ += "<font";
	  html_ += " color='#" + color_[xint] + "'";
	  html_ += ">";
	}

      // ----- begin styles
      if (style_[xint].Find("b") != wxNOT_FOUND)
	html_ += "<b>";
      if (style_[xint].Find("i") != wxNOT_FOUND)
	html_ += "<i>";

      // -----
      if (xtext[i].Find("<a") == wxNOT_FOUND)
	xtext[i].Replace(" ", "&nbsp;");
      //xtext[i].Replace("#", "&#9839;");  // sharp
      //xtext[i].Replace("b", "&#9837;");  // flat
      if (xtext[i].IsEmpty())
	xtext[i] = "<br>";
      html_ += xtext[i];

      // ----- end styles
      if (style_[xint].Find("i") != wxNOT_FOUND)
	html_ += "</i>";
      if (style_[xint].Find("b") != wxNOT_FOUND)
	html_ += "</b>";

      // ----- end font
      if (!color_[xint].IsEmpty())
	html_ += "</font>";

      html_ += "</p>";

      // ----- end table row and col
      html_ += "</td>";
      html_ += "</tr>";
    }

  // -----
  html_ += "</table>";
  if (!font_[PLAINTEXT].IsEmpty()  || !size_[PLAINTEXT].IsEmpty() ||
      !color_[PLAINTEXT].IsEmpty())
    html_ += "</font>";
  html_ += "</body>";

  // -----
  html_ += "</html>";
}

// ----------------------------------------------------------------------
wxString Cifra::GetConfig(int section, int item)
{
  if (section < 0 || section >= SECTIONCOUNT)
    return "";
  if (item < 0 || item >= CONFIGCOUNT)
    return "";

  if (item == FONT)
    return font_[section];
  if (item == SIZE)
    return size_[section];
  if (item == STYLE)
    return style_[section];
  if (item == COLOR)
    return color_[section];
  if (item == ALIGN)
    return align_[section];

  return "";
}

// ----------------------------------------------------------------------
wxString Cifra::GetFile()
{
  if (file_.IsEmpty())
    return "New File";
  return folder_ + SEP + file_ + ".txt";
}

// ----------------------------------------------------------------------
wxString Cifra::GetHTML()
{
  return html_;
}

// ----------------------------------------------------------------------
wxString Cifra::GetProfileName()
{
  return profile_;
}

// ----------------------------------------------------------------------
wxArrayString Cifra::GetSectionNames()
{
  return sectionname_;
}

// ----------------------------------------------------------------------
wxString Cifra::GetText()
{
  return text_;
}

// ----------------------------------------------------------------------
wxString Cifra::GetTitle()
{
  if (data_[TITLE].IsEmpty())
    return "No Title";
  return data_[TITLE];
}

// ----------------------------------------------------------------------
bool Cifra::Load(wxString folder)
{
  // -----
  wxString xbuf;
  wxString temp;

  // ----- verify folder string
  if (folder.IsEmpty())
    return false;
  if (!wxDirExists(folder))
    return false;
  if (folder.Right(1) == SEP)
    folder = folder.Left(folder.Len() - 1);

  // -----
  temp = folder;
  if (temp.Find(SEP) != wxNOT_FOUND)
    temp = temp.AfterLast(SEP);
  if (!wxFileExists(folder + SEP + temp + ".txt"))
    return false;

  // ----- load file content
  if (!FileRead(folder + SEP + temp + ".txt", xbuf))
    return false;

  // ----- set member variables
  New();
  file_   = temp;
  folder_ = folder;
  text_   = xbuf;
  Generate();

  return true;
}

// ----------------------------------------------------------------------
wxString Cifra::Modulation(wxString line)
{
  // -----
  wxString scale   = "A BC D EF G ";
  wxString shift   = "A#BC#D#EF#G#";
  int      space   = 0;
  wxString chord1  = "";
  wxString chord2  = "";
  bool     sharp   = false;
  int      length1 = 0;

  for (int i=0; i<(int)line.Len(); i++)
    {
      // ----- find chord
      if (line[i] == ' ')
	continue;
      int k = scale.Find(line[i]);
      if (k == wxNOT_FOUND)
	continue;
      if (i > 0)
	if (line[i-1] != ' ' && line[i-1] != '/' &&
	    line[i-1] != '-' && line[i-1] != '(')
	  continue;

      // ----- get original chord
      chord1  = line.Mid(i).BeforeFirst(' ').BeforeFirst('/').BeforeFirst('-');
      chord2  = "";
      sharp   = false;
      length1 = chord1.Len();
      if (length1 > 1)
	{
	  if (chord1[1] == '#')
	    {
	      sharp = true;
	      k += 1;
	    }
	  if (chord1[1] == 'b')
	    {
	      sharp = true;
	      k -= 1;
	    }
	}

      // ----- modulation
      k += shift_;
      while (k < 0) k += 12;
      k = k % 12;

      // ----- new chord
      if (shift[k] == '#')
	{
	  chord2 = "#";
	  k--;
	}
      chord2 = shift[k] + chord2;

      // ----- get minor, 7th, etc., from original chord
      if (sharp && length1 > 2)
	chord2 += chord1.Mid(2);
      else if (!sharp && length1 > 1)
	chord2 += chord1.Mid(1);

      // ----- replace chord1 with chord2 
      wxString xstr;
      space += chord2.Len() - length1;
      xstr   = line.Mid(i+length1);
      while (space > 0 && xstr.Left(2) == "  ")
	{
	  xstr = xstr.Mid(1);
	  space--;
	}
      while (space < 0 && xstr.Left(1) == " ")
	{
	  xstr = " " + xstr;
	  space++;
	}
      xstr = line.Left(i) + chord2 + xstr;
      line = xstr;
    }

  return line;
}

// ----------------------------------------------------------------------
void Cifra::New()
{
  file_.Clear();
  folder_.Clear();
  html_.Clear();
  text_.Clear();

  shift_ = 0;

  data_.Empty();
  data_.Add("", SECTIONCOUNT);
}

// ----------------------------------------------------------------------
bool Cifra::ProfileLoad(wxString name)
{
  // ----- 
  wxFileConfig* profile;
  wxString      value;

  // -----
  profile = new wxFileConfig("cifra", "vannini", "cifra.cfg", "",
			     wxCONFIG_USE_LOCAL_FILE |
			     wxCONFIG_USE_RELATIVE_PATH);
  if (profile == NULL)
    return false;
  profile->SetRecordDefaults(true);

  // ----- 
  profile->SetPath("/"+name);
  for (int i=0; i<SECTIONCOUNT; i++)
    {
      // ----- default value
      value  = font_[i]  + "|";
      value += size_[i]  + "|";
      value += style_[i] + "|";
      value += color_[i] + "|";
      value += align_[i];

      // ----- get entry from config file
      value  = profile->Read(OneWord(sectionname_[i]), value);

      // ----- get values
      font_[i]  = value.BeforeFirst('|');
      value = value.AfterFirst('|');
      size_[i]  = value.BeforeFirst('|');
      value = value.AfterFirst('|');
      style_[i] = value.BeforeFirst('|');
      value = value.AfterFirst('|');
      color_[i] = value.BeforeFirst('|');
      value = value.AfterFirst('|');
      align_[i] = value.BeforeFirst('|');
    }

  profile_ = name;

  delete profile;
  return true;
}

// ----------------------------------------------------------------------
bool Cifra::ProfileSave(wxString name)
{
  // ----- 
  wxFileConfig* profile;
  wxString      value;

  // -----
  profile = new wxFileConfig("cifra", "vannini", "cifra.cfg", "",
			     wxCONFIG_USE_LOCAL_FILE |
			     wxCONFIG_USE_RELATIVE_PATH);
  if (profile == NULL)
    return false;

  // ----- 
  profile->SetPath("/"+name);
  for (int i=0; i<SECTIONCOUNT; i++)
    {
      // ----- default value
      value  = font_[i]  + "|";
      value += size_[i]  + "|";
      value += style_[i] + "|";
      value += color_[i] + "|";
      value += align_[i];

      // ----- set entry from config file
      profile->Write(OneWord(sectionname_[i]), value);
    }

  profile_ = name;

  delete profile;
  return true;
}

// ----------------------------------------------------------------------
bool Cifra::Save()
{
  // -----
  wxString tempfile   = file_;
  wxString tempfolder = folder_;
  wxString temptext;

  // -----
  if (tempfile.IsEmpty())
    {
      tempfile = data_[TITLE];
      if (tempfile.IsEmpty())
	{
	  Generate();
	  tempfile = data_[TITLE];
	}
      tempfile = OneWord(tempfile);
    }
  if (tempfile.IsEmpty())
    return false;

  // -----
  if (tempfolder.IsEmpty())
    tempfolder = tempfile;
  if (!wxDirExists(tempfolder))
    if (!wxMkdir(tempfolder))
      return false;

  // ----- convert LF to CRLF on MS Windows
  temptext = text_;
#if defined(__WXMSW__)
  temptext.Replace("\x0A", "\x0D\x0A");
#endif

  // -----
  FileWrite(tempfolder + SEP + tempfile + ".txt", temptext);

  file_   = tempfile;
  folder_ = tempfolder;

  return true;
}

// ----------------------------------------------------------------------
bool Cifra::SavePDF()
{
  if (file_.IsEmpty())
    return false;

  FileWrite(folder_ + SEP + file_ + ".html", html_);
  ConvertHtml2Pdf(folder_ + SEP + file_ + ".html",
		  folder_ + SEP + file_ + ".pdf");

  return true;
}

// ----------------------------------------------------------------------
bool Cifra::SavePDFBook()
{
  // -----
  wxString xfolder = folder_;
  wxDir    xdir(wxGetCwd());
  wxString xdirname;
  bool     xok;
  int      xint = 1;
  wxString toc;
  wxString xstr;

  // -----
  xok = xdir.GetFirst(&xdirname, "", wxDIR_DIRS);
  while (xok)
    {
      if (Load(xdirname))
	{
	  xstr = i2wxs(++xint, "%.5d");
	  toc += "<a href=\"pag" + xstr + ".html\">" +data_[TITLE] + "</a><br>";
	  FileWrite("pag" + xstr + ".html", html_);
	}
      xok = xdir.GetNext(&xdirname);
    }

  // -----
  if (xint > 0)
    {
      // ----- cover
      xstr  = "<html><head></<head><body>";
      xstr += "<font";
      xstr += " face='" + font_[TITLE] + "'";
      xstr += " size=7";
      if (!color_[TITLE].IsEmpty())
	xstr += " color='#" + color_[TITLE] + "'";
      xstr += ">";
      xstr += "<center><br>Livro<br>de<br>Músicas<br></center>";
      xstr += "</font>";
      xstr += "</body></html>";
      FileWrite("pag00000.html", xstr);

      // ----- table of contents
      toc = "<html><head></<head><body>" + toc + "</body></html>";
      FileWrite("pag00001.html", toc);

      // -----
      ConvertHtml2Pdf("pag*.html", "book.pdf");
    }

  Load(xfolder);
  return true;
}

// ----------------------------------------------------------------------
void Cifra::SetConfig(int section, int item, wxString value)
{
  if (section < 0 || section >= SECTIONCOUNT)
    return;
  if (item < 0 || item >= CONFIGCOUNT)
    return;

  if (item == FONT)
    font_[section]  = value;
  if (item == SIZE)
    size_[section]  = value;
  if (item == STYLE)
    style_[section] = value;
  if (item == COLOR)
    color_[section] = value;
  if (item == ALIGN)
    align_[section] = value;
}

// ----------------------------------------------------------------------
void Cifra::SetKeyShift(int shift)
{
  shift_ += shift;
  Generate();
}

// ----------------------------------------------------------------------
void Cifra::SetText(wxString text)
{
  if (text == text_)
    return;

  text_ = text;
  Generate();
}
