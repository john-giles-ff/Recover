/******************************************************************************
 *
 * @author    EDT <http://www.edtc.com>
 *
 *****************************************************************************/
#ifndef ETEM070001ZDH6TOUCHCONTROLLER_HPP
#define ETEM070001ZDH6TOUCHCONTROLLER_HPP

#include <platform/driver/touch/TouchController.hpp>

namespace touchgfx
{
class ETEM070001ZDH6TouchController: public TouchController
{
public:
    ETEM070001ZDH6TouchController() : isInitialized(false) {}
    virtual void init();
    virtual bool sampleTouch(int32_t& x, int32_t& y);
protected:
    bool isInitialized;
};

}

#endif // ETEM070001ZDH6TOUCHCONTROLLER_HPP
