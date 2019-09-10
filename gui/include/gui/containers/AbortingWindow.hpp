#ifndef ABORTINGWINDOW_HPP
#define ABORTINGWINDOW_HPP

#include <gui_generated/containers/AbortingWindowBase.hpp>

class AbortingWindow : public AbortingWindowBase
{
public:
    AbortingWindow();
    virtual ~AbortingWindow() {}

    virtual void initialize();
protected:
};

#endif // ABORTINGWINDOW_HPP
