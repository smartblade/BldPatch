#define BldName "Mystery of Blade patch"
#define BldPublisher "Blade Smart"
#define BldURL "https://github.com/smartblade/BldMystery"
#define BldExeName "Bin/Blade.exe"
#define BldVersion() \
  ParseVersion( \
    "install/" + BldExeName, \
    Local[0], Local[1], Local[2], Local[3]), \
  Str(Local[0]) + "." + Str(Local[1]) + "." + Str(Local[2])
#define BackupPath "BldMystery/bak/"

[Setup]
AppendDefaultDirName=no
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{B9D8108B-B41E-4A14-89BB-B144917D973D}
AppName={#BldName}
AppVersion={#BldVersion}
AppPublisher={#BldPublisher}
AppPublisherURL={#BldURL}
AppSupportURL={#BldURL}
AppUpdatesURL={#BldURL}
DefaultDirName=C:\Games\Severence - Blade of Darkness
DefaultGroupName={#BldName}
DirExistsWarning=no
DisableProgramGroupPage=yes
InfoBeforeFile=RELEASE_NOTES.txt
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputDir=install
OutputBaseFilename=BldMysterySetup_v{#BldVersion}
UninstallFilesDir={app}/BldMystery/Uninstall
Compression=lzma
SetupIconFile=src/Blade/Icon_3.ico
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Dirs]
Name: "{app}/{#BackupPath}"

[Files]
Source: "install/Bin/Blade.exe"; DestDir: "{app}/Bin"; BeforeInstall: BeforeBladeExeInstall; Flags: ignoreversion onlyifdestfileexists uninsneveruninstall
Source: "RELEASE_NOTES.txt"; DestDir: "{app}/BldMystery"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Run]
Filename: "{app}/{#BldExeName}"; Description: "{cm:LaunchProgram,{#StringChange(BldName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Messages]
WizardInfoBefore=Welcome to the {#BldName} Setup Wizard
InfoBeforeLabel=This will install {#BldName} version {#BldVersion} on your computer.

[Code]

function NextButtonClick(CurPageID: Integer): Boolean;
var
  BladePatchMD5, DstFileMD5, BladeExePath: String;
begin
	BladePatchMD5 := 'd6af0d121e04e8482654789831501d2c';
  case CurPageID of
    wpSelectDir, wpReady:
    begin
      BladeExePath := WizardDirValue+'/bin/Blade.exe';
      if not FileExists(BladeExePath) then
      begin
        MsgBox(
          'The game is not found in ''' + WizardDirValue + ''' directory',
          mbError, MB_OK);
        Result := False;
        Exit;
      end;
      DstFileMD5 := GetMD5OfFile(BladeExePath);
      if DstFileMD5 <> BladePatchMD5 then
      begin
        MsgBox(
          'Unsupported version of game.' #13
          'Patch v1.0.1 should be installed.', mbError, MB_OK);
        Result := False;
        Exit;
      end;
    end;
  end;
  Result := True;
end;

procedure BeforeBladeExeInstall;
var
  BladeExePath, BackupPath: String;
begin
  BladeExePath := ExpandConstant(CurrentFileName);
  BackupPath := ExpandConstant('{app}/{#BackupPath}/Blade_v101.exe');
  if FileExists(BackupPath) then
    Exit;
  if not FileCopy(BladeExePath, BackupPath, True) then
    RaiseException('Failed to backup Blade.exe')
end;

procedure InitializeUninstallProgressForm();
var
  BladeExePath, BackupPath: String;
begin
  BackupPath := ExpandConstant('{app}/{#BackupPath}/Blade_v101.exe');
  BladeExePath := ExpandConstant('{app}/bin/Blade.exe');
  if not FileCopy(BackupPath, BladeExePath, False) then
    RaiseException('Failed to restore Blade.exe');
  if not DeleteFile(BackupPath) then
    RaiseException('Failed to delete Blade_v101.exe');
end;
