#ifndef VIEWDIAGNOSTICS_HPP
#define VIEWDIAGNOSTICS_HPP

#include <gui_generated/containers/ViewDiagnosticsBase.hpp>

class ViewDiagnostics : public ViewDiagnosticsBase
{
public:
    ViewDiagnostics();
    virtual ~ViewDiagnostics() {}

    virtual void initialize();

	void SetCiphers(int *ciphers, int size);
protected:
	virtual void Close();
};

#endif // VIEWDIAGNOSTICS_HPP
