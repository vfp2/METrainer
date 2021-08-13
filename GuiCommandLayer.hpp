#ifndef INCLUDE_HPP_GUICOMMANDLAYER_
#define INCLUDE_HPP_GUICOMMANDLAYER_


#include <FL/Fl_Box.H>


class GuiCommandLayer : public Fl_Box
{
public:
    GuiCommandLayer(int x, int y, int w, int h, const char* title=0);

public:
    virtual int handle(int event);
};


#endif // INCLUDE_HPP_GUICOMMANDLAYER_
