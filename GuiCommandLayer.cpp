#include "GuiCommandLayer.hpp"


GuiCommandLayer::GuiCommandLayer(int x, int y, int w, int h, const char* title) 
    : Fl_Box(x, y, w, h, title)
{}


int GuiCommandLayer::handle(int event)
{
    switch(event)
    {
    case FL_PUSH:
        do_callback();
        return 1;
    case FL_KEYDOWN:
        do_callback();
        return 1;
    case FL_FOCUS:
        return 1;
    case FL_UNFOCUS:
        return 1;
    }

    return 0;
}
