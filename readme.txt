usbmonitor allows to start new processes automatically when usb devices are plugged in. Any time one is plugged in, the event is logged in the file log.txt. You can pick the device's ID from there and append the following line to the file target.txt:
id=$id, exe=$path_to_exe {, event=plugged/unplugged, admin=true/false, wdir=[default]/[usbmonitor]/[executable]/$working_directory, args=$arguments}

This program only works on Windows because it uses platform specific APIs.

ISSUES
* It is not unicode friendly.
* if you put the path to usbmonitor.exe in target.txt bad things may happen...
* You shouldn't be able to open multiple instances of usbmonitor.
* fix log target in case of [default] wdir
* files should be opened in executable directory (and not in working directory) or on my documents/appdata
* make it appdata friendly, log.txt cannot be written in some folders
* it is not cross platform
* maybe use semicolon to split lines in target.txt and trimming should erase newlines.
* message USB PLUGGED at startup should be different
