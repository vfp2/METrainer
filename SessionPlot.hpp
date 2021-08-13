#ifndef INCLUDE_HPP_SESSIONPLOT_
#define INCLUDE_HPP_SESSIONPLOT_


class Fl_Double_Window;


class SessionPlot
{
public:
    SessionPlot();
    ~SessionPlot();


public:
    void Show();
    void Hide();


private:
    Fl_Double_Window* frame_;
};



#endif // INCLUDE_HPP_SESSIONPLOT_
