echo off
echo �������ڡ������Ӧ�ö�����豸���и��ġ� ��ѡ���ǡ��Ի�ȡ����ԱȨ�������б���װ�ű�
echo Pop up window "allow this app to make changes to your device" please select "yes" to obtain administrator rights to run this installation script
echo.

::��ȡ����ԱȨ��
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
::���ֵ�ǰĿ¼������
cd /d "%~dp0"

echo off
echo ��ʼ���а�װ�ű�����װ����ǰ��ȷ�����������ѹرջ��ĵ��ѱ���
echo Start running the installation script. Before installing the driver, make sure that other programs have been closed or the document has been saved.
echo.


::�����ӳٱ�����չ
setlocal enabledelayedexpansion
echo.

 ::ɾ����ʷ�����ļ�
 if exist LogFIle\hid_dev.txt (
    del/f /q LogFIle\hid_dev.txt
)
 if exist LogFIle\i2c_dev.txt (
    del/f /q LogFIle\i2c_dev.txt
)
del/f /q LogFIle\dev*.tmp
del/f /q LogFIle\drv*.tmp
echo.

::ɾ����ʷ��¼�ļ�
if exist LogFIle\Return_UninstDrv.txt (
    del/f /q LogFIle\Return_UninstDrv.txt
)
if exist LogFIle\UninstDrvSucceeded.txt (
    del/f /q LogFIle\UninstDrvSucceeded.txt
)
echo.


::���Ŀ¼
if not exist LogFIle (
    md LogFIle
    echo.
)


pnputil /scan-devices
echo scan-devicesɨ���豸ok
echo.

echo ��ʼ�������е�HID�豸device��ע���/connected��ʾ�Ѿ�����
pnputil /enum-devices /connected /class {745a17a0-74d3-11d0-b6fe-00a0c90f57da}  /ids /relations >LogFIle\hid_dev.txt
echo enum-devicesö���豸ok
echo.

::����Ƿ��ҵ�touchpad���ذ��豸device
find/i "HID_DEVICE_UP:000D_U:0005" LogFIle\hid_dev.txt || (
     echo δ���ִ��ذ��豸������ж������
     echo No TouchPad device found, no need to unload the driver.
     echo.
     echo UNDRV_OK >LogFIle\Return_UninstDrv.txt
     echo UNDRV_OK >LogFIle\UninstDrvSucceeded.txt
     echo.
     exit
)

echo �ҵ�touchpad���ذ��豸
echo TouchPad device found.
echo.


echo ��ʼ����touchpad���ذ��Ӧ�ĸ���I2C�豸ʵ��InstanceID
echo.
 
 ::�滻�س����з�Ϊ���ŷ���ָע�����NUL����Ϊ׷��>>д��
for /f "delims=" %%i in (LogFIle\hid_dev.txt) do (
   set /p="%%i,"<nul>>LogFIle\dev0.tmp
 )

::�滻HID_DEVICE_UP:000D_U:0005Ϊ#����ָע��set /p��Ҫ�Ӷ���
for /f "delims=, tokens=*" %%i in (LogFIle\dev0.tmp) do (
    set "str=%%i"
    set "str=!str:HID_DEVICE_UP:000D_U:0005=#!"
    set /p="!str!,"<nul>>LogFIle\dev1.tmp
)

  ::��ȡ#�ָ���������ı���ע��set /p��Ҫ�Ӷ���
 for /f "delims=# tokens=2,*" %%i in (LogFIle\dev1.tmp) do (
   set /p="%%i,"<nul>>LogFIle\dev2.tmp
 )

  ::��ȡ:�ָ���������ı���ע��set /p��Ҫ�Ӷ���
 for /f "delims=: tokens=2" %%i in (LogFIle\dev2.tmp) do (
   set /p="%%i,"<nul>>LogFIle\dev3.tmp
 )

   ::��ȡ,�ָ���ǰ����ı�
 for /f "delims=, tokens=1" %%i in (LogFIle\dev3.tmp) do (
  set /p="%%i"<nul>>LogFIle\dev4.tmp
 )

    ::ɾ���ո�
 for /f "delims= " %%i in (LogFIle\dev4.tmp) do (
   set "str=%%i"
   echo !str!>LogFIle\TouchPad_I2C_devInstanceID.txt
 )
echo.

del/f /q LogFIle\hid_dev.txt
del/f /q LogFIle\dev*.tmp
echo.
 

 ::��֤InstanceID
  for /f "delims=" %%i in (LogFIle\TouchPad_I2C_devInstanceID.txt) do (
   set "i2c_dev_InstanceID=%%i"
   echo i2c_dev_InstanceID="!i2c_dev_InstanceID!"
   echo.
 )

 ::ע���/connected��ʾ�Ѿ�����
 pnputil /enum-devices /connected /instanceid "%i2c_dev_InstanceID%" /ids /relations /drivers >LogFIle\i2c_dev.txt
 echo.
 
::����Ƿ�װMouseLikeTouchPad_I2C����
find/i "MouseLikeTouchPad_I2C" LogFIle\i2c_dev.txt || (
     echo δ����MouseLikeTouchPad_I2C����������ж������
     echo No MouseLikeTouchPad_I2C driver found, no need to unload the driver.
     echo.
     del/f /q LogFIle\i2c_dev.txt
     del/f /q LogFIle\TouchPad_I2C_devInstanceID.txt
     echo UNDRV_OK >LogFIle\Return_UninstDrv.txt
     echo UNDRV_OK >LogFIle\UninstDrvSucceeded.txt
     exit
)

echo �ҵ�MouseLikeTouchPad_I2C����
echo.

echo ��ʼ����MouseLikeTouchPad_I2C����oem�ļ���
echo.

 ::ɾ����ʷ�����ļ�
del/f /q LogFIle\drv*.tmp
 echo.
 
 ::�滻�س����з�Ϊ���ŷ���ָע�����NUL����Ϊ׷��>>д��
for /f "delims=" %%i in (LogFIle\i2c_dev.txt) do (
   set /p="%%i,"<nul>>LogFIle\drv0.tmp
 )

::�滻mouseliketouchpad_i2c.infΪ#����ָע��set /p��Ҫ�Ӷ���
for /f "delims=, tokens=*" %%i in (LogFIle\drv0.tmp) do (
    set "str=%%i"
    set "str=!str:mouseliketouchpad_i2c.inf=#!"
    set /p="!str!,"<nul>>LogFIle\drv1.tmp
)

  ::��ȡ#�ָ���ǰ����ı�
 for /f "delims=# tokens=1" %%i in (LogFIle\drv1.tmp) do (
   set /p="%%i"<nul>>LogFIle\drv2.tmp
 )

::�滻oemΪ[����ָע��set /p��Ҫ�Ӷ���
for /f "delims=, tokens=*" %%i in (LogFIle\drv2.tmp) do (
    set "str=%%i"
    set "str=!str:oem=[!"
    set /p="!str!,"<nul>>LogFIle\drv3.tmp
)

  ::��ȡ���һ��[�ָ���������ı���ע��tokensҪѡ2������������в���nul���治��׷�Ӷ���>
 for /f "delims=[ tokens=2,*" %%i in (LogFIle\drv3.tmp) do (
   set /p="%%i,"<nul>LogFIle\drv4.tmp
 )

   ::��ȡ,�ָ���ǰ����ı�
 for /f "delims=, tokens=1" %%i in (LogFIle\drv4.tmp) do (
  set /p="oem%%i"<nul>LogFIle\oemfilename.txt
 )
echo.

 ::ɾ����ʷ�����ļ�
del/f /q LogFIle\drv*.tmp
echo.


  ::��ȡoemfilename
  for /f "delims=" %%i in (LogFIle\oemfilename.txt) do (
   set "oemfilename=%%i"
   echo oemfilename="!oemfilename!"
   echo.
 )

 :ж��oem������������
 pnputil /delete-driver "%oemfilename%" /uninstall /force
 echo delete-driverж������ok
echo.

pnputil /scan-devices
echo scan-devicesɨ���豸ok
echo.


::��֤�Ƿ�ж�سɹ���ע���/connected��ʾ�Ѿ�����
 pnputil /enum-devices /connected /instanceid "%i2c_dev_InstanceID%" /ids /relations /drivers >LogFIle\i2c_dev.txt
 echo.
 
find/i "MouseLikeTouchPad_I2C" LogFIle\i2c_dev.txt && (
     echo ж������ʧ�ܣ�����������
     echo Failed to unload the driver. Please try again.
     echo.
     del/f /q LogFIle\i2c_dev.txt
     echo UNDRV_FAILED >LogFIle\Return_UninstDrv.txt
     exit
)

del/f /q LogFIle\i2c_dev.txt
del/f /q LogFIle\TouchPad_I2C_devInstanceID.txt
echo ж�������ɹ�
echo Unload driver succeeded.
echo.
echo UNDRV_OK >LogFIle\Return_UninstDrv.txt
echo UNDRV_OK >LogFIle\UninstDrvSucceeded.txt
echo.