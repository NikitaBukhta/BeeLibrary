#define MyAppName "BeeLibrary"
#define MyAppVersion "0.1"
#define MyAppExeName "BeeLibrary.exe"

[Setup]
AppId={{B33L1BR4-RY00-0000-0000-000000000001}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
OutputDir=..\dist
OutputBaseFilename=BeeLibrary_setup
Compression=lzma2
SolidCompression=yes
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
SetupIconFile=..\assets\icon.ico
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
; Main executable
Source: "..\staging\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion

; Qt configuration (plugin/QML paths relative to exe)
Source: "..\staging\qt.conf"; DestDir: "{app}"; Flags: ignoreversion

; Qt and runtime DLLs
Source: "..\staging\*.dll"; DestDir: "{app}"; Flags: ignoreversion

; Qt plugins
Source: "..\staging\plugins\*"; DestDir: "{app}\plugins"; Flags: ignoreversion recursesubdirs

; QML modules
Source: "..\staging\qml\*"; DestDir: "{app}\qml"; Flags: ignoreversion recursesubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\Uninstall {#MyAppName}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "Launch {#MyAppName}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: filesandordirs; Name: "{app}"
