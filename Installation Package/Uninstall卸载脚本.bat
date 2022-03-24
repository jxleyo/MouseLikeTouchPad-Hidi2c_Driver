echo off
echo 弹出窗口“允许此应用对你的设备进行更改“ 请选择“是”以获取管理员权限来运行本卸载脚本
pause
echo.

::获取管理员权限
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
::保持当前目录下运行
cd /d "%~dp0"

echo off
echo 开始运行卸载脚本，卸载驱动前请确保其他程序已关闭或文档已保存
pause
echo.

echo 开始检查安装文件
echo.
if exist devcon.exe (
    echo devcon.exe文件正常
) else (
    echo devcon.exe程序丢失，请检查或者重新下载驱动安装包
    pause
    exit
)
echo.


echo 开始删除驱动文件备份
dir /b C:\Windows\System32\DriverStore\FileRepository\hidi2c_touchpad.inf* >dir_del_list.tmp
for /f "delims=" %%i in (dir_del_list.tmp) do (
    rd/s /q C:\Windows\System32\DriverStore\FileRepository\%%i
    echo 旧版驱动文件备份%%i已删除
    echo.
)
del/f /q dir_del_list.tmp
echo.

echo 重新扫描硬件
devcon rescan
echo.

::开启延迟变量扩展
setlocal enabledelayedexpansion

:seek
echo 开始安装windows原版触控板驱动
devcon update C:\Windows\INF\hidi2c.inf ACPI\PNP0C50
devcon rescan
devcon update hidi2c.inf ACPI\PNP0C50
devcon rescan
echo.
     
echo 开始查找touchpad触控板设备device
devcon hwids *HID_DEVICE_UP:000D_U:0005* >hwid0.tmp
echo.

::检查是否找到touchpad触控板设备device
find/i "HID" hwid0.tmp && (
    echo.
    echo 找到触控板设备
    echo.
) || (
     echo 未发现触控板设备，按任意键将自动安装Windows原版驱动后再次尝试查找触控板设备
     echo 多次尝试都是 未发现触控板设备 则本驱动不兼容该笔记本电脑硬件
     echo.
     del/f /q hwid*.tmp
     pause
     goto seek
)
echo.


echo 开始查找I2C设备id
::查找有&COL字符的行并加入行号
find/i /n "&COL" hwid0.tmp >hwid1.tmp
::过滤其他行保留首行
find/i "[1]" hwid1.tmp >hwid2.tmp

::替换&COL成特殊字符方便分割，删除行号，替换触控板设备号开头
for /f "delims=" %%i in (hwid2.tmp) do (
    set "str=%%i"
    set "str=!str:&COL=^!"
    set "str=!str:[1]=!"
    set "str=!str:HID=ACPI!"
    echo !str!>hwid3.tmp
)

::以^分割字符串获取开头即可生成touchpad所使用的i2c总线mini port设备id
for /f "delims=^" %%i in (hwid3.tmp) do (
    set "hwidstr=%%i"
    echo !hwidstr!>hwid.txt
)
echo 触控板touchpad设备所使用的i2c总线mini port设备id为%hwidstr%
echo.

::删除临时文件
del/f /q hwid*.tmp


echo 开始查询是否安装了第三方驱动Hidi2c_TouchPad以及oem*.inf文件名称
::注意重定向输出文件名不要保护oem字样否则注册表未找到时会输出该文件名到保存的文件内容中干扰后续判断
for /f "delims=@, tokens=2"  %%i in ('reg query "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Hidi2c_TouchPad" /v "DisplayName"') do (
    set "infFileName=%%i" 
    echo !infFileName!>infResult.txt
)
echo.
    
find/i "oem" infResult.txt && (
    echo.
    echo 找到触控板设备第三方驱动Hidi2c_TouchPad
    echo.
    for /f "delims=" %%j in (infResult.txt) do (
    set "infFileName=%%j"
    )
    echo oem安装文件为%infFileName%
    echo.
) || (
     echo 未发现第三方驱动Hidi2c_TouchPad，系统未安装本驱动，按任意键退出
     echo.
     pause
     exit
)
echo.


:uninst
echo 开始卸载touchpad触控板驱动
devcon -f dp_delete %infFileName% && (
    echo.
    echo 第三方触控板驱动Hidi2c_TouchPad卸载成功
) || (
     echo 第三方触控板驱动Hidi2c_TouchPad卸载失败
)
echo.

echo 开始删除遗留的驱动文件
del/f /q C:\Windows\INF\%infFileName%
del/f /q C:\Windows\System32\Drivers\Hidi2c_TouchPad.sys
echo.

echo 开始再次删除驱动文件备份
dir /b C:\Windows\System32\DriverStore\FileRepository\hidi2c_touchpad.inf* >dir_del_list.tmp
for /f "delims=" %%i in (dir_del_list.tmp) do (
    rd/s /q C:\Windows\System32\DriverStore\FileRepository\%%i
    echo 遗留的驱动文件备份%%i已删除
    echo.
)
del/f /q dir_del_list.tmp
echo.

echo 开始卸载自签名证书Reg delete EVRootCA.reg
echo.
reg delete "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\SystemCertificates\ROOT\Certificates\E403A1DFC8F377E0F4AA43A83EE9EA079A1F55F2" /f && (
    echo EVRootCA.reg自签名证书卸载完成
) || (
     echo EVRootCA.reg自签名证书不存在或者注册表操作错误
)
echo.

echo 开始删除驱动的注册表信息
echo.
reg delete "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Hidi2c_TouchPad" /f && (
    echo 驱动注册表信息已删除
) || (
     echo 驱动的注册表信息不存在或者reg delete注册表操作错误
)
echo.

echo Hidi2c_TouchPad第三方驱动已经卸载完成
echo.

echo 如果触控板不工作请按任意键重启电脑
echo 如果触控板运行正常则关闭本窗口以取消重启
echo.

pause
shutdown -r -f -t 0
