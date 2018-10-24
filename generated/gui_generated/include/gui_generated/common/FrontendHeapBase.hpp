/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef FRONTENDHEAPBASE_HPP
#define FRONTENDHEAPBASE_HPP

#include <common/Meta.hpp>
#include <common/Partition.hpp>
#include <mvp/MVPHeap.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <touchgfx/transitions/SlideTransition.hpp>

#include <gui/common/FrontendApplication.hpp>
#include <gui/model/Model.hpp>

#include <gui/idlescreen_screen/IdleScreenView.hpp>
#include <gui/idlescreen_screen/IdleScreenPresenter.hpp>
#include <gui/mainscreen_screen/MainScreenView.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>
#include <gui/processscreen_screen/ProcessScreenView.hpp>
#include <gui/processscreen_screen/ProcessScreenPresenter.hpp>
#include <gui/settingsscreen_screen/SettingsScreenView.hpp>
#include <gui/settingsscreen_screen/SettingsScreenPresenter.hpp>
#include <gui/auditscreen_screen/AuditScreenView.hpp>
#include <gui/auditscreen_screen/AuditScreenPresenter.hpp>
#include <gui/freda_screen/FREDAView.hpp>
#include <gui/freda_screen/FREDAPresenter.hpp>
#include <gui/eggscreen_screen/EggScreenView.hpp>
#include <gui/eggscreen_screen/EggScreenPresenter.hpp>


/**
 * This class provides the memory that shall be used for memory allocations
 * in the frontend. A single instance of the FrontendHeap is allocated once (in heap
 * memory), and all other frontend objects such as views, presenters and data model are
 * allocated within the scope of this FrontendHeap. As such, the RAM usage of the entire
 * user interface is sizeof(FrontendHeap).
 *
 * @note The FrontendHeap reserves memory for the most memory-consuming presenter and
 * view only. The largest of these classes are determined at compile-time using template
 * magic. As such, it is important to add all presenters, views and transitions to the
 * type lists in this class.
 *
 */
class FrontendHeapBase : public MVPHeap
{
public:
    /**
     * A list of all view types. Must end with meta::Nil.
     * @note All view types used in the application MUST be added to this list!
     */
    typedef meta::TypeList< IdleScreenView,
            meta::TypeList< MainScreenView,
            meta::TypeList< ProcessScreenView,
            meta::TypeList< SettingsScreenView,
            meta::TypeList< AuditScreenView,
            meta::TypeList< FREDAView,
            meta::TypeList< EggScreenView,
            meta::Nil > > > > > >
            > GeneratedViewTypes;

    /**
     * Determine (compile time) the View type of largest size.
     */
    typedef meta::select_type_maxsize< GeneratedViewTypes >::type MaxGeneratedViewType;

    /**
     * A list of all presenter types. Must end with meta::Nil.
     * @note All presenter types used in the application MUST be added to this list!
     */
    typedef meta::TypeList< IdleScreenPresenter,
            meta::TypeList< MainScreenPresenter,
            meta::TypeList< ProcessScreenPresenter,
            meta::TypeList< SettingsScreenPresenter,
            meta::TypeList< AuditScreenPresenter,
            meta::TypeList< FREDAPresenter,
            meta::TypeList< EggScreenPresenter,
            meta::Nil > > > > > >
            > GeneratedPresenterTypes;

    /**
     * Determine (compile time) the Presenter type of largest size.
     */
    typedef meta::select_type_maxsize< GeneratedPresenterTypes >::type MaxGeneratedPresenterType;

    /**
     * A list of all transition types. Must end with meta::Nil.
     * @note All transition types used in the application MUST be added to this list!
     */
    typedef meta::TypeList< NoTransition,
            meta::TypeList< SlideTransition<SOUTH>,
            meta::TypeList< SlideTransition<WEST>,
            meta::TypeList< SlideTransition<EAST>,
            meta::Nil > > >
            > GeneratedTransitionTypes;

    /**
     * Determine (compile time) the Transition type of largest size.
     */
    typedef meta::select_type_maxsize< GeneratedTransitionTypes >::type MaxGeneratedTransitionType;

    virtual void gotoStartScreen(FrontendApplication& app)
    {
        app.gotoIdleScreenScreenNoTransition();
    }
protected:
    FrontendHeapBase(AbstractPartition& presenters, AbstractPartition& views, AbstractPartition& transitions, FrontendApplication& app)
        : MVPHeap(presenters, views, transitions, app)
    {

    }

};

#endif // FRONTENDHEAPBASE_HPP
