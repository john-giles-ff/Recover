#include <gui/containers/ViewDiagnostics.hpp>

ViewDiagnostics::ViewDiagnostics()
{

}

void ViewDiagnostics::initialize()
{
    ViewDiagnosticsBase::initialize();
}

void ViewDiagnostics::SetCiphers(int* ciphers, int size)
{
	bool anyVisible = false;
	if (size > 0 && ciphers[0] > 0)
	{
		Unicode::snprintf(TxtCipher1Buffer, TXTCIPHER1_SIZE, "%X", ciphers[0]);
		anyVisible = true;
	}
	if (size > 1 && ciphers[1] > 0)
	{
		Unicode::snprintf(TxtCipher2Buffer, TXTCIPHER2_SIZE, "%X", ciphers[1]);
		anyVisible = true;
	}
	if (size > 2 && ciphers[2] > 0)
	{
		Unicode::snprintf(TxtCipher3Buffer, TXTCIPHER3_SIZE, "%X", ciphers[2]);
		anyVisible = true;
	}
	if (size > 3 && ciphers[3] > 0)
	{
		Unicode::snprintf(TxtCipher4Buffer, TXTCIPHER4_SIZE, "%X", ciphers[3]);
		anyVisible = true;
	}
	if (size > 4 && ciphers[4] > 0)
	{
		Unicode::snprintf(TxtCipher5Buffer, TXTCIPHER5_SIZE, "%X", ciphers[4]);
		anyVisible = true;
	}

	TxtNoDiagnostics.setVisible(!anyVisible);

}

void ViewDiagnostics::Close()
{
	setVisible(false);
	invalidate();
}

