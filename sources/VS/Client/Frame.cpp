// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Frame.h"
#include <wx/statline.h>


enum
{
    TOOL_VIEW_BRIEF,        // ����������� ��� �����������
    TOOL_VIEW_FULL          // ������ ��� �����������
};


Frame::Frame(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title)
{
    SetIcon(wxICON(sample));

    wxMenu *menuFile = new wxMenu;

    menuFile->Append(wxID_ABOUT);
    menuFile->Append(wxID_EXIT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _("����"));

    wxMenu *menuSettings = new wxMenu;
    menuBar->Append(menuSettings, _("���������"));

    wxMenu *menuHelp = new wxMenu;
    menuBar->Append(menuHelp, _("������"));

    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Frame::OnQuit, this, wxID_EXIT);

    Bind(wxEVT_MENU, &Frame::OnViewBrief, this, TOOL_VIEW_BRIEF);
}


void Frame::OnViewBrief(wxCommandEvent &)
{

}


void Frame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close(true);
}

void Frame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxBoxSizer *topsizer;
    wxDialog dlg(this, wxID_ANY, wxString(_("About")));

    topsizer = new wxBoxSizer(wxVERTICAL);

#if wxUSE_STATLINE
    topsizer->Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
#endif // wxUSE_STATLINE

    wxButton *bu1 = new wxButton(&dlg, wxID_OK, _("OK"));
    bu1->SetDefault();

    topsizer->Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);

    dlg.SetSizer(topsizer);
    topsizer->Fit(&dlg);

    dlg.ShowModal();
}
