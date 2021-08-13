#ifndef INCLUDE_HPP_PSISEERGUIHELP_
#define INCLUDE_HPP_PSISEERGUIHELP_


class Fl_Double_Window;
//class Fl_Help_View;


class PsiSeerGuiHelp
{
public:
    PsiSeerGuiHelp();
    ~PsiSeerGuiHelp();

public:
    void Show();
    void Hide();

private:
    bool isVisible_;

    Fl_Double_Window* helpWindow_;
};


#endif // INCLUDE_HPP_PSISEERGUIHELP_
