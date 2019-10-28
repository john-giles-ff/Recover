#ifndef AUDITSCREEN_PRESENTER_HPP
#define AUDITSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class AuditScreenView;

class AuditScreenPresenter : public Presenter, public ModelListener
{
public:
    AuditScreenPresenter(AuditScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();	

    virtual ~AuditScreenPresenter() {};

private:
    AuditScreenPresenter();

    AuditScreenView& view;
};


#endif // AUDITSCREEN_PRESENTER_HPP
