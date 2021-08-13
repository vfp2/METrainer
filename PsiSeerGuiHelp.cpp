#include "PsiSeerGuiHelp.hpp"


#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>


PsiSeerGuiHelp::PsiSeerGuiHelp()
: isVisible_(false)
, helpWindow_(0)
{
    Fl::lock();

    helpWindow_ = new Fl_Double_Window(430, 520, "PsiTrainer Help");
    helpWindow_->end();
    helpWindow_->show();

    Fl::unlock();
}


PsiSeerGuiHelp::~PsiSeerGuiHelp()
{
    if (helpWindow_ != 0)
    {
        delete helpWindow_;
        helpWindow_ = 0;
    }
}


void PsiSeerGuiHelp::Show()
{
    if (isVisible_)
        return;

    Fl::lock();
    helpWindow_->show();
    Fl::unlock();

    isVisible_ = true;
}


void PsiSeerGuiHelp::Hide()
{
    Fl::lock();
    helpWindow_->hide();
    Fl::unlock();

    isVisible_ = false;
}
