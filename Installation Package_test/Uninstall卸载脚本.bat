echo off
echo �������ڡ������Ӧ�ö�����豸���и��ġ� ��ѡ���ǡ��Ի�ȡ����ԱȨ�������б�ж�ؽű�
pause

::��ȡ����ԱȨ��
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
::���ֵ�ǰĿ¼������
cd /d "%~dp0"

echo off
echo ��ʼ����ж�ؽű���ж������ǰ��ȷ�����������ѹرջ��ĵ��ѱ���
pause

::��鰲װ�ļ�
if exist devcon.exe (
    echo devcon.exe�ļ�����
) else (
    echo devcon.exe����ʧ�����������������������װ��
    pause
    exit
)

::����ɨ��Ӳ��
devcon rescan

::ɾ���ɰ���������
dir /b C:\Windows\System32\DriverStore\FileRepository\hidi2c_touchpad.inf* >dir_del_list.tmp
for /f "delims=" %%i in (dir_del_list.tmp) do (
    rd/s /q C:\Windows\System32\DriverStore\FileRepository\%%i
    echo �ɰ������ļ�%%i��ɾ��
)
del/f /q dir_del_list.tmp

::����touchpad���ذ��豸device
:seek
devcon hwids *HID_DEVICE_UP:000D_U:0005* >hwid0.tmp

::����Ƿ��ҵ�touchpad���ذ��豸device
find/i "HID" hwid0.tmp && (
    echo �ҵ����ذ��豸
) || (
     echo δ���ִ��ذ��豸������������Խ��Զ���װwindowsԭ�������ٴν���ж�ر���������������γ���ʧ�����������ܲ����ݸñʼǱ�����
     echo ���߹رձ������˳���װ��
     del/f /q hwid*.tmp
     pause
     devcon update hidi2c.inf ACPI\PNP0C50
     devcon rescan
     goto seek
)

::������&COL�ַ����в������к�
find/i /n "&COL" hwid0.tmp >hwid1.tmp
::���������б�������
find/i "[1]" hwid1.tmp >hwid2.tmp

::�����ӳٱ�����չ
setlocal enabledelayedexpansion

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

::ɾ����ʱ�ļ�
del/f /q hwid*.tmp

::��ѯtouchpad���ذ�ʹ�õ�i2c����mini port�豸��ǰ����inf�ļ�
reg query "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Hidi2c_TouchPad" /v "DisplayName" >oemInfName.tmp
for /f "delims=@, tokens=2"  %%i in (oemInfName.tmp) do (
    set "oemInfName=%%i"
)
echo oem.inf��װ�ļ�Ϊ%oemInfName%
del/f /q oemInfName*.tmp

::ж��touchpad���ذ�ʹ�õ�i2c����mini port�豸��ʷ����������ɨ��Ӳ��
devcon disable %hwidstr%
devcon remove %hwidstr%
del/f /q C:\Windows\System32\Drivers\Hidi2c_TouchPad.sys
del/f /q C:\Windows\System32\Drivers\%oemInfName%
devcon dp_delete %oemInfName%
devcon rescan

::ж����ǩ��֤��Reg delete EVRootCA.reg
reg delete "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\SystemCertificates\ROOT\Certificates\E403A1DFC8F377E0F4AA43A83EE9EA079A1F55F2" /f
echo EVRootCA.reg��ǩ��֤��ж�����

echo Hidi2c_TouchPad�����������Ѿ�ж�����

::���β���touchpad���ذ��豸device
:seek2
devcon hwids *HID_DEVICE_UP:000D_U:0005* >hwid0.tmp

::���μ���Ƿ��ҵ�touchpad���ذ��豸device
find/i "HID" hwid0.tmp && (
    echo �ҵ����ذ��豸
    echo ������ذ岻�����밴�������������
    echo ������ذ�����������رձ�������ȡ������
    del/f /q hwid*.tmp
    pause
) || (
     echo ����δ���ִ��ذ��豸������������Խ��Զ���װwindowsԭ����������������γ���ʧ����windowsԭ���������ܲ����ݸñʼǱ�����
     echo ���߹رձ������˳�
     del/f /q hwid*.tmp
     pause
     devcon update hidi2c.inf ACPI\PNP0C50
     devcon rescan
     goto seek2
)

pause
shutdown -r -f -t 0
