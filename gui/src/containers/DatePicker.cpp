#include <gui/containers/DatePicker.hpp>

DatePicker::DatePicker()
{	
}

void DatePicker::initialize()
{
    DatePickerBase::initialize();

	time = LFT::Information.GetCurrentTime();
	Update();
}

void DatePicker::Confirm()
{	
	STime out(time.getRaw());


	LFT::Information.SetRTC(
		out.GetYear(),
		out.GetMonth() + 1,
		out.GetDay(),
		out.GetHour(),
		out.GetMinute(),
		out.GetSecond());

	invalidate();
}

void DatePicker::YearUp()
{		
	time.incYear(+1);
	Update();
}

void DatePicker::YearDown()
{	
	time.incYear(-1);
	Update();
}

void DatePicker::MonthUp()
{
	time.incMonth(+1);
	Update();
}

void DatePicker::MonthDown()
{
	STime out(time.getRaw());
	if (out.GetMonth() == 0)
	{
		//Bug where doing just -1 month causes year to freak out 
		time.incMonth(11);
		time.incYear(-1);
	}
	else
	{
		time.incMonth(-1);
	}	
	Update();
}

void DatePicker::DayUp()
{
	time.incDay(+1);
	Update();
}

void DatePicker::DayDown()
{
	time.incDay(-1);
	Update();
}

void DatePicker::HourUp()
{
	time.incHour(+1);
	Update();
}

void DatePicker::HourDown()
{
	time.incHour(-1);
	Update();
}

void DatePicker::MinuteUp()
{
	time.incMinute(+1);
	Update();
}

void DatePicker::MinuteDown()
{
	time.incMinute(-1);
	Update();
}

void DatePicker::Update()
{
	STime out(time.getRaw());
	

	//Show time, month gets a +1 since it counts 0-11 not 1-12
	Unicode::snprintf(TxtYearBuffer,	TXTYEAR_SIZE,	"%04d", out.GetYear());
	Unicode::snprintf(TxtMonthBuffer,	TXTMONTH_SIZE,	"%02d", out.GetMonth() + 1);
	Unicode::snprintf(TxtDayBuffer,		TXTDAY_SIZE,	"%02d", out.GetDay());
	Unicode::snprintf(TxtHourBuffer,	TXTHOUR_SIZE,	"%02d", out.GetHour());
	Unicode::snprintf(TxtMinuteBuffer,	TXTMINUTE_SIZE, "%02d", out.GetMinute());
	
				

	TxtYear.invalidate();
	TxtMonth.invalidate();
	TxtDay.invalidate();
	TxtHour.invalidate();
	TxtMinute.invalidate();
}


