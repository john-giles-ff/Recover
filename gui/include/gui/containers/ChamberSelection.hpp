#ifndef CHAMBERSELECTION_HPP
#define CHAMBERSELECTION_HPP

#include <gui_generated/containers/ChamberSelectionBase.hpp>

class ChamberSelection : public ChamberSelectionBase
{
public:
    ChamberSelection();
    virtual ~ChamberSelection() {}
protected:
	virtual void handleClickEvent(const ClickEvent& evt);
	virtual void handleDragEvent(const DragEvent& evt);

private:
	bool _state;

	void updateToState();
};

#endif // CHAMBERSELECTION_HPP
