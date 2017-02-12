;
; Inno Setup can be found at: http://www.jrsoftware.org/
;

[Setup]
AppName=Dorgem
AppVerName=Dorgem 2.1.0
VersionInfoVersion=2.1.0.0
AppPublisher=Frank Fesevur
AppPublisherURL=http://dorgem.sourceforge.net
DefaultDirName={pf}\Dorgem
DefaultGroupName=Dorgem
AllowNoIcons=yes
UninstallDisplayIcon={app}\Dorgem.exe
SetupIconFile=..\Res\Dorgem.ico
OutputBaseFilename=dorgem210
OutputDir="..\Releases\version 2.1.0"
WizardImageBackColor=clWhite
WizardImageFile=Dorgem.bmp
WizardSmallImageFile=DorgemSmall.bmp

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Files]
Source: "..\Release\Dorgem.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\HtmlHelp\Dorgem.chm"; DestDir: "{app}"; Flags: ignoreversion

[INI]
Filename: "{app}\Dorgem.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://dorgem.sourceforge.net"

[Icons]
Name: "{group}\Dorgem"; Filename: "{app}\Dorgem.exe"
Name: "{group}\Dorgem on the Web"; Filename: "{app}\Dorgem.url"
Name: "{group}\Dorgem Help"; Filename: "{app}\Dorgem.chm"
Name: "{group}\Uninstall Dorgem"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Dorgem"; Filename: "{app}\Dorgem.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\Dorgem.exe"; Description: "Launch Dorgem"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\Dorgem.url"

[Registry]
Root: HKCU; Subkey: "Software\Dorgem"; Flags: uninsdeletekey
