echo off
echo �������ڡ������Ӧ�ö�����豸���и��ġ� ��ѡ���ǡ��Ի�ȡ����ԱȨ�������б���װ�ű�
pause
echo.

::��ȡ����ԱȨ��
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
::���ֵ�ǰĿ¼������
cd /d "%~dp0"

echo off
echo ��ʼ���а�װ�ű�����װ����ǰ��ȷ�����������ѹرջ��ĵ��ѱ���
pause
echo.

echo ��ʼ��鰲װ�ļ�
echo.
if exist devcon.exe (
    echo devcon.exe�ļ�����
) else (
    echo devcon.exe����ʧ�����������������������װ��
    pause
    exit
)

if exist Hidi2c_TouchPad.inf (
    echo Hidi2c_TouchPad.inf�ļ�����
) else (
    echo Hidi2c_TouchPad.inf�ļ���ʧ�����������������������װ��
    pause
    exit
)

if exist Hidi2c_TouchPad.cat (
    echo Hidi2c_TouchPad.cat�ļ�����
) else (
    echo Hidi2c_TouchPad.cat�ļ���ʧ�����������������������װ��
    pause
    exit
)

if exist Hidi2c_TouchPad.sys (
    echo Hidi2c_TouchPad.sys�ļ�����
) else (
    echo Hidi2c_TouchPad.sys�ļ���ʧ�����������������������װ��
    pause
    exit
)

if exist EVRootCA.reg (
    echo EVRootCA.reg�ļ�����
) else (
    echo EVRootCA.reg�ļ���ʧ�����������������������װ��
    pause
    exit
)
echo.


echo ��ʼɾ�������ļ�����
dir /b C:\Windows\System32\DriverStore\FileRepository\hidi2c_touchpad.inf* >dir_del_list.tmp
for /f "delims=" %%i in (dir_del_list.tmp) do (
    rd/s /q C:\Windows\System32\DriverStore\FileRepository\%%i
    echo �ɰ������ļ�����%%i��ɾ��
    echo.
)
del/f /q dir_del_list.tmp
echo.

echo ����ɨ��Ӳ��
devcon rescan
echo.

::�����ӳٱ�����չ
setlocal enabledelayedexpansion

:seek
echo ��ʼ��װwindowsԭ�津�ذ�����
devcon update C:\Windows\INF\hidi2c.inf ACPI\PNP0C50
devcon rescan
devcon update hidi2c.inf ACPI\PNP0C50
devcon rescan
echo.

echo ��ʼ����touchpad���ذ��豸device
devcon hwids *HID_DEVICE_UP:000D_U:0005* >hwid0.tmp
echo.

::����Ƿ��ҵ�touchpad���ذ��豸device
find/i "HID" hwid0.tmp && (
    echo.
    echo �ҵ����ذ��豸
    echo.
) || (
     echo δ���ִ��ذ��豸������������Զ���װWindowsԭ�����������߹رձ������ֶ���װԭ������
     echo.
     del/f /q hwid*.tmp
     pause
     goto seek
)
echo.


echo ��ʼ����I2C�豸id
::������&COL�ַ����в������к�
find/i /n "&COL" hwid0.tmp >hwid1.tmp
::���������б�������
find/i "[1]" hwid1.tmp >hwid2.tmp

::�滻&COL�������ַ�����ָɾ���кţ��滻���ذ��豸�ſ�ͷ
for /f "delims=" %%i in (hwid2.tmp) do (
    set "str=%%i"
    set "str=!str:&COL=^!"
    set "str=!str:[1]=!"
    set "str=!str:HID=ACPI!"
    echo !str!>hwid3.tmp
)

::��^�ָ��ַ�����ȡ��ͷ��������touchpad��ʹ�õ�i2c����mini port�豸id
for /f "delims=^" %%i in (hwid3.tmp) do (
    set "hwidstr=%%i"
    echo !hwidstr!>hwid.txt
)
echo ���ذ�touchpad�豸��ʹ�õ�i2c����mini port�豸idΪ%hwidstr%
echo.

::ɾ����ʱ�ļ�
del/f /q hwid*.tmp


echo ��ʼ��ѯ�Ƿ�װ�˵���������Hidi2c_TouchPad�Լ�oem*.inf�ļ�����
::ע���ض�������ļ�����Ҫ����oem��������ע���δ�ҵ�ʱ��������ļ�����������ļ������и��ź����ж�
for /f "delims=@, tokens=2"  %%i in ('reg query "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Hidi2c_TouchPad" /v "DisplayName"') do (
    set "infFileName=%%i" 
    echo !infFileName!>infResult.txt
)
echo.
    
find/i "oem" infResult.txt && (
    echo.
    echo �ҵ����ذ��豸����������Hidi2c_TouchPad
    echo.
    for /f "delims=" %%j in (infResult.txt) do (
    set "infFileName=%%j"
    )
    echo oem��װ�ļ�Ϊ%infFileName%
    echo.
) || (
     echo δ���ֵ���������Hidi2c_TouchPad
     echo.
     goto inst
)


:uninst
echo ��ʼж�ؾɰ�touchpad���ذ�����
devcon -f dp_delete %infFileName% && (
    echo.
    echo �ɰ津�ذ�����ж�سɹ�
) || (
     echo �ɰ津�ذ�����ж��ʧ��
)
echo.

echo ��ʼɾ�������������ļ�
del/f /q C:\Windows\INF\%infFileName%
del/f /q C:\Windows\System32\Drivers\Hidi2c_TouchPad.sys
echo.

echo ��ʼ�ٴ�ɾ�������ļ�����
dir /b C:\Windows\System32\DriverStore\FileRepository\hidi2c_touchpad.inf* >dir_del_list.tmp
for /f "delims=" %%i in (dir_del_list.tmp) do (
    rd/s /q C:\Windows\System32\DriverStore\FileRepository\%%i
    echo �����������ļ�����%%i��ɾ��
    echo.
)
del/f /q dir_del_list.tmp
echo.

echo ��ʼɾ��������ע�����Ϣ
echo.
reg delete "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Hidi2c_TouchPad" /f && (
    echo ����ע�����Ϣ��ɾ��
) || (
     echo ������ע�����Ϣ�����ڻ���ע����������
)
echo.

echo �ɰ�Hidi2c_TouchPad�����������Ѿ�ж�����
echo.


:inst
echo ��ʼ��װ��ǩ��֤��Reg import EVRootCA.reg
regedit /s EVRootCA.reg && (
    echo EVRootCA.reg��ǩ��֤�鰲װ���
) || (
     echo EVRootCA.regע���������󣬰�װʧ��
)
echo.

echo ��ʼ��װ������
devcon update Hidi2c_TouchPad.inf %hwidstr%  && (
    echo.
    devcon rescan
    echo Hidi2c_TouchPad�����������Ѿ���װ���
    echo.
    echo ����ѹһ�´��ذ尴���鿴�Ƿ������������������رձ�������ȡ������
    echo ������ذ岻�����밴�������������
    pause
    shutdown -r -f -t 0
) || (
     echo Hidi2c_TouchPad������������װʧ�ܣ���Ҫ�������Ժ��ٴ����б���װ���򼴿ɳɹ���װ
     echo ��������ָ�ԭ���������˳�
     echo.
     pause
     devcon update C:\Windows\INF\hidi2c.inf %hwidstr%
     devcon rescan
     devcon update hidi2c.inf %hwidstr%
     devcon rescan
     exit    
)
echo.


