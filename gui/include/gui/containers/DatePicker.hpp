#ifndef DATEPICKER_HPP
#define DATEPICKER_HPP

#include <gui_generated/containers/DatePickerBase.hpp>
#include <gui/LFT/LFT.hpp>

class DatePicker : public DatePickerBase
{
public:
    DatePicker();
    virtual ~DatePicker() {}

    virtual void initialize();

	virtual void Confirm();	
protected:
	virtual void YearUp();
	virtual void YearDown();

	virtual void MonthUp();
	virtual void MonthDown();

	virtual void DayUp();
	virtual void DayDown();

	virtual void HourUp();
	virtual void HourDown();

	virtual void MinuteUp();
	virtual void MinuteDown();

private:
	void Update();	

	DateTime time;	
};

#endif // DATEPICKER_HPP
