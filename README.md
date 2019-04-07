********************************************************************************************
QuickInfo: 
I spend some time to get a Windows Service with a FTD232R Dongle to work.
- new Driver 1.4.8 in Makefile
- fixed a fatal compile error -> unknown type name LPCTSTR
- Analyzed SIGSEGV 11 CoreDump to FT_SetEventNotification and removed shim function. (may someone know a bit more about mutex)
- Fixed and extended Trace Commands - did not get anything out on WINEDEBUG=ftd2xx.
- tested on pure i386 xubuntu with wine 3.0 on Atom HW - NOMULTILIBTESTdone

Command to build:  
sudo apt-get install wine32 git build-essential wine32-tools cabextract winbind  
git clone https://github.com/steininet/wineftd2xx  
cd wineftd2xx  
sudo make install ARCH=i386 WINEDLLPATH=/usr/lib/i386-linux-gnu/wine  

Command to Install the Driver on a i386  
sudo cp libftd2xx-i386-1.4.8/build/libftd2xx.so.1.4.8 /usr/local/lib  
sudo cp libftd2xx-i386-1.4.8/build/libftd2xx.a /usr/local/lib  
sudo ln -sf /usr/local/lib/libftd2xx.so.1.4.8 /usr/lib/libftd2xx.so  
sudo ln -sf /usr/local/lib/libftd2xx.so.1.4.8 /usr/local/lib/libftd2xx.so  
sudo chmod 0755 /usr/local/lib/libftd2xx.so.1.4.8  
sudo lconfig -v  

<implement /etc/udev/rules.d with the right VID/PID to get userspace access rights>

Command to Start  
WINEPREFIX=~/.aprefix WINEARCH="win32" FTDID=dc48 WINEDEBUG=ftd2xx wine <some.exe>  

this is based on https://github.com/brentr/wineftd2xx  
********************************************************************************************

This is a Linux Wine dll.so that substitutes for FTDI's D2XX drivers.
It is effectively a shim layer between the FTDI's Linux D2XX drivers and 
Microsoft Windows applicatons running under Wine.

Almost all calls are emulated, except those that have no meaning under Unix.
To date, however, it has been tested only with the Intronix LogicPort running 
under 32-bit Intel Linux.  64-bit Intel *should* also work.

Note that wine version >= 1.7.27 is recommended.

To install, after cloning this repository locally:

1)  confirm that you have a functioning wine version >= 1.7.27
    You must it such that the desired wine executable and tools are 
    in your $PATH.

2)  at the top level directory of the repository, type:

  $  make 
  
    This will fetch the required libraries from FTDI's website and build 
    ftd2xx.dll.so and libftd2xx.def
    
3)  To add these files in your wine installation:

  $  sudo   make  install
  
 
4)  Ensure that your users have permission to access to the USB FTDI devices.
    If you use udev, this will likely involve adding rules in /etc/udev/rules.d
    
    For example, the Intronix LogicPort might be made accessible to all users
    in the plugdev group by:
    
    # sudo  cp  etc/udev/rules.d/81-logicport.rules  /etc/udev/rules.d

    Use 81-logicport.rules as a template if you have a different device.

   
5)  ftd2xx.dll.so requires that the FTDID variable be set to indicate the 
    USB Vendor and Product ID of the FTD2xx device your Windows application
    controls.  The syntax of the FTDID variable is either:
    
    FTDID=<USB Product ID>
    
    or:
    
    FTDID=<USB Vendor ID>:<USB Product ID>
    
    Product and Vendor ID are in hexidecimal.
    If omitted, the Vendor is assumed to be 0403 (hex).
    
    For example, 
      to start the windows application controlling Intronix LogicPort:
    
    $ FTDID=DC48 wine ~/.wine/drive_c/Program\ Files/LogicPort/LogicPort.exe 

The only issue I see with LogicPort.exe running under Wine is that the
command shortcut buttons do not render until one hovers the cursor over them.
Thus, at most one is visible at any time.
This is not really much of an issue, as the buttons do work when clicked and
all their functions are also available on dropdown menus.
