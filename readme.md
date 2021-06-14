[[_TOC_]]

#Key Facts
##Project Description
The RECOVER is a machine that performs a chemical process on metalic samples in order to reveal latent fingerprint marks. It does this by achieving a vacuum in
a chamber, then heating the chemical so that the chemical is evenly dispersed over the sample(s).

If a vacuum is not reached before chemical dispersal the chemical is likely to ignite, therefor there are safety interlocks to ensure that this is not possible

##Hardware Configuration
It is worth noting that there are 2 MCU's on the system. One that controls the graphical interface, and one that controls the rest of the hardware. This source code 
is for the graphical MCU. The secondary source code is elsewhere.

#Getting Started
You will need some tools in order to start developing on the RECOVER, they are as follows:
- TouchGFX
- ST-Link Utility
- Visual Studio

##Touchgfx Simulator vs Target
TouchGFX automatically includes a simulator that can be used for testing. Sales and the like also like the simulator since it can be used to show the screen larger 
than the actual screen. This simulator however has none of the target board code so any call to actual hardware must be surrounded by a `#ifndef SIMULATOR \n #endif`

##TouchGFX Limitations
TouchGFX's ideology is to sacrifice memory over cpu intensity this means that according to TouchGFX in order to have a button of a certain style, you should create an 
image of that button, and an image of what that button looks like when it is pressed, and it will then use that to render instead of having to spend the CPU power in 
order to draw the button a new each time.  This is a little confusing coming from a desktop perspective, and depending on how much memory is remaining you may have to 
write code that allows you to contradict this ideology.

##Software / Firmware. 
Technically the graphical MCU has firmware and the PCB MCU has firmware, however to differentiate from these two, the graphical MCU firmware is referred to as software 
since it has been developed by the software department and the PCB MCU firmware is reffered to as firmware.


#Projects
There are multiple projects in this git repository
##RECOVER
Embedded application that allows the control of the RECOVER unit 
##RECOVER_Companion
Windows application that allows the user to extract the logs and update the RECOVER software
##RecoverControl
Library allows you to control the RECOVER
##UserDecode
This decodes the Cyphers created by the application
##PresureCurveLinearizing
Developer tool that is used to create a ramp so that the pressure being achieved is seen to be done in a linear fashion instead of lograthmically
##LogInspector
Production tool that evaluates multiple units soak test runs so look for any outliers
##LFTFileCreator
Developer tool that creates a .LFT file (used to update software) from the software + firmware.
##BCTestStrap
Developer tool that allows for a command line interface with the RECOVER Unit, it can also attempt to recover bricked RECOVER units depending on their current state
##AUTOFlash
Production tool that automatically installs software provided to it


#Dependencies
##Third Party Libraries/Code
This includes the following third party libaries/code:

###RECOVER
- FreeRTOS
- ST Libraries
###RECOVER_Companion
- AsyncIO
- IntelHexFormatReader
- Ookii.Dialogs.Wpf
- PDFSharp-Migradoc-wpf
###RecoverControl
- IntelHexFormatReader
###UserDecode
- None
###PressureCurveLinearizing
- LiveCharts
- LiveCharts.Wpf
- Ookii.Dialogs.Wpf
###LogInspector
- InteractiveDataDisplay.WPF
- LiveCharts
- LiveCharts.WPF
###LFTFileCreator
- None
###BCTestStrap
- IntelHexFormatReader
###AUTOFlash
- None

##Library Licenses
- FreeRTOS - [MIT](https://www.freertos.org/a00114.html)
- ST Libraries - [Open Source](https://www.st.com/content/ccc/resource/legal/legal_agreement/license_agreement/group0/eb/8e/f9/c1/9e/64/49/95/SLA0048/files/SLA0048.txt/jcr:content/translations/en.SLA0048.txt)
- AsyncIO - [MPL-2.0](https://github.com/somdoron/AsyncIO/blob/master/LICENSE.md)
- IntelHexFormatReader - [MIT](https://github.com/twinearthsoftware/IntelHexFormatReader/blob/master/LICENSE.md)
- Ookii.Dialogs.Wpf - [BSD](https://github.com/ookii-dialogs/ookii-dialogs-wpf/blob/master/LICENSE)
- PDFSharp-Migradoc-wpf - [BSD](https://github.com/ookii-dialogs/ookii-dialogs-wpf/blob/master/LICENSE)
- LiveCharts - [MIT](https://github.com/Live-Charts/Live-Charts/blob/master/LICENSE.TXT)
- LiveCharts.Wpf - [MIT](https://github.com/Live-Charts/Live-Charts/blob/master/LICENSE.TXT)
- InteractiveDataDisplay.WPF - [MIT](https://github.com/microsoft/InteractiveDataDisplay.WPF/blob/master/LICENSE)

