#include "SessionPlot.hpp"


#include <FL/Fl_Double_Window.H>
#include <Fl/Fl_help_View.H>
#include <FL/Fl_Box.H>


SessionPlot::SessionPlot()
: frame_(0)
{
    frame_ = new Fl_Double_Window(400, 200, "PsiTrainer PC - Session Plot");
    frame_->end();
}


SessionPlot::~SessionPlot()
{
    if (frame_ != 0)
    {
        delete frame_;
        frame_ = 0;
    }
}


void SessionPlot::Show()
{
        frame_->show();
}


void SessionPlot::Hide()
{
        frame_->hide();
}
