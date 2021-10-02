#define BldName "Mystery of Blade patch"
#define BldPublisher "Blade Smart"
#define BldURL "https://github.com/smartblade/BldMystery"
#define BldExeName "Bin/Blade.exe"
#define BldMajor
#define BldMinor
#define BldRelease
#define BldBuild
#define BldVersion() \
  ParseVersion( \
    "install/" + BldExeName, \
    BldMajor, BldMinor, BldRelease, BldBuild), \
  Str(BldMajor) + "." + Str(BldMinor) + "." + Str(BldRelease)
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
AlwaysShowDirOnReadyPage=yes
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
Source: "install/Bin/Raster/rOpenGL.dll"; DestDir: "{app}/Bin/Raster"; BeforeInstall: BeforeROpenGLDllInstall; Flags: ignoreversion uninsneveruninstall
Source: "RELEASE_NOTES.txt"; DestDir: "{app}/BldMystery"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Run]
Filename: "{app}/{#BldExeName}"; Description: "{cm:LaunchProgram,{#StringChange(BldName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Messages]
WizardInfoBefore=Welcome to the {#BldName} Setup Wizard
InfoBeforeLabel=This will install {#BldName} version {#BldVersion} on your computer.

[Code]

function BladeVersion(Major, Minor, Release: Cardinal): Cardinal;
begin
  Result := Release + Minor * 100 + Major * 10000;
end;

function GetBladeExeVersionNumbers(
  ExePath: String; var Major, Minor, Release, Build: Cardinal): boolean;
var
  MS, LS: Cardinal;
begin
  Result := GetVersionNumbers(ExePath, MS, LS);
  if not Result then
  begin
    Exit;
  end;
  Major := MS shr 16;
  Minor := MS and $FFFF;
  Release := LS shr 16;
  Build := LS and $FFFF;
end;

function GetBladeExeVersion(ExePath: String): Cardinal;
var
  Major, Minor, Release, Build: Cardinal;
begin
  if not GetBladeExeVersionNumbers(ExePath, Major, Minor, Release, Build) Then
  begin
    Result := 0;
    Exit;
  end;
  Result := BladeVersion(Major, Minor, Release);
end;

function IsSupportedBinary(ExePath: String; var ErrorMessage: String): Boolean;
var
  BladePatchMD5, DstFileMD5: String;
  Major, Minor, Release, Build: Cardinal;
begin
	BladePatchMD5 := 'd6af0d121e04e8482654789831501d2c';
  DstFileMD5 := GetMD5OfFile(ExePath);
  if DstFileMD5 = BladePatchMD5 then
  begin
    Result := True;
    Exit;
  end;
  if GetBladeExeVersion(ExePath) >= BladeVersion(1, 20, 0) then
  begin
    if
      GetBladeExeVersion(ExePath) <=
      BladeVersion({#BldMajor}, {#BldMinor}, {#BldRelease}) then
    begin
      // Found previous version, we can update it.
      Result := True;
      Exit;
    end;
    GetBladeExeVersionNumbers(ExePath, Major, Minor, Release, Build);
    ErrorMessage := Format(
      'Newer version of the game was found: %d.%d.%d', [Major, Minor, Release]);
    Result := False;
    Exit;
  end;
  ErrorMessage :=
    'Unsupported version of game.' #13
    'Patch v1.0.1 should be installed.';
  Result := False;
end;

function NextButtonClick(CurPageID: Integer): Boolean;
var
  BladeExePath, ErrorMessage: String;
begin
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
      if not IsSupportedBinary(BladeExePath, ErrorMessage) then
      begin
        MsgBox(ErrorMessage, mbError, MB_OK);
        Result := False;
        Exit;
      end;
    end;
  end;
  Result := True;
end;

procedure StoreBackupFile(FilePath, BackupName: String);
var
  BackupPath: String;
begin
  FilePath := ExpandConstant(FilePath);
  BackupPath := ExpandConstant('{app}/{#BackupPath}/' + BackupName);
  if not FileExists(FilePath) then
    Exit;
  if FileExists(BackupPath) then
    Exit;
  if not FileCopy(FilePath, BackupPath, True) then
    RaiseException('Failed to backup ' + FilePath);
end;

procedure BeforeBladeExeInstall;
begin
  StoreBackupFile(CurrentFileName, 'Blade_v101.exe');
end;

procedure BeforeROpenGLDllInstall;
begin
  StoreBackupFile(CurrentFileName, 'rOpenGL.dll');
end;

procedure RestoreFile(FileName, BackupName: String);
var
  FilePath, BackupPath: String;
begin
  BackupPath := ExpandConstant('{app}/{#BackupPath}/' + BackupName);
  FilePath := ExpandConstant('{app}/' + FileName);
  if not FileExists(BackupPath) then
    Exit;
  if not FileCopy(BackupPath, FilePath, False) then
    RaiseException('Failed to restore ' + FileName);
  if not DeleteFile(BackupPath) then
    RaiseException('Failed to delete ' + BackupName);
end;

procedure InitializeUninstallProgressForm();
begin
  RestoreFile('bin/Blade.exe', 'Blade_v101.exe')
  RestoreFile('bin/Raster/rOpenGL.dll', 'rOpenGL.dll')
end;
