echo off
echo �������ڡ������Ӧ�ö�����豸���и��ġ� ��ѡ���ǡ��Ի�ȡ����ԱȨ�������б���װ�ű�
pause

::��ȡ����ԱȨ��
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
::���ֵ�ǰĿ¼������
cd /d "%~dp0"

echo off
echo ��ʼ���а�װ�ű�����װ����ǰ��ȷ�����������ѹرջ��ĵ��ѱ���
pause

::��鰲װ�ļ�
if exist devcon.exe (
    echo devcon.exe�ļ�����
) else (
    echo devcon.exe����ʧ�����������������������װ��
    pause
    exit
)

::��鰲װ�ļ�
if exist Hidi2c_TouchPad.inf (
    echo Hidi2c_TouchPad.inf�ļ�����
) else (
    echo Hidi2c_TouchPad.inf�ļ���ʧ�����������������������װ��
    pause
    exit
)

::��鰲װ�ļ�
if exist Hidi2c_TouchPad.cat (
    echo Hidi2c_TouchPad.cat�ļ�����
) else (
    echo Hidi2c_TouchPad.cat�ļ���ʧ�����������������������װ��
    pause
    exit
)

::��鰲װ�ļ�
if exist Hidi2c_TouchPad.sys (
    echo Hidi2c_TouchPad.sys�ļ�����
) else (
    echo Hidi2c_TouchPad.sys�ļ���ʧ�����������������������װ��
    pause
    exit
)

::��鰲װ�ļ�
if exist EVRootCA.reg (
    echo EVRootCA.reg�ļ�����
) else (
    echo EVRootCA.reg�ļ���ʧ�����������������������װ��
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
     echo δ���ִ��ذ��豸������������Խ��Զ���װwindowsԭ�������ٴν��а�װ����������������γ���ʧ�����������ܲ����ݸñʼǱ�����
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

::��װ��ǩ��֤��Reg import EVRootCA.reg
regedit /s EVRootCA.reg
echo EVRootCA.reg��ǩ��֤�鰲װ���

::ж��touchpad���ذ�ʹ�õ�i2c����mini port�豸��ʷ����������ɨ��Ӳ��
devcon remove %hwidstr%
devcon rescan

::��װ������touchpad���ذ�ʹ�õ�i2c����mini port�豸id������ɨ��Ӳ��
devcon update Hidi2c_TouchPad.inf %hwidstr%
devcon rescan

echo Hidi2c_TouchPad�����������Ѿ���װ���
echo ����ѹһ�´��ذ尴���鿴�Ƿ������������������رձ�������ȡ������
echo ������ذ岻�����밴�������������
pause
shutdown -r -f -t 0
