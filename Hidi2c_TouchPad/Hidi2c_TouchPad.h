#pragma once

#pragma once

#include <ntddk.h>
#include <ntstrsafe.h>
#include <ntintsafe.h>
#include <wdf.h>
#include <wdm.h>

#include "hidsdi.h"
#include "hidpi.h" 
#include <initguid.h>
#include <devpkey.h>


#include <hidport.h>

//
#include "spb.h"

//
#define RESHUB_USE_HELPER_ROUTINES
#include "reshub.h"

//
#include <acpiioct.h>

//
typedef BYTE* PBYTE;

#define WdfDeleteObjectSafe(o)  \
    if (o != NULL) { WdfObjectDelete(o); }

//
typedef struct _ACPI_DEVICE_SETTINGS
{
    // ACPI "_DSM" HID Descriptor Address
    USHORT  HidDescriptorAddress;

} ACPI_DEVICE_SETTINGS, * PACPI_DEVICE_SETTINGS;

#include "pshpack1.h"

#define STABLE_INTERVAL_FingerSeparated_MSEC   20   // ��ָ�ֿ�������������ȶ�ʱ����
#define STABLE_INTERVAL_FingerClosed_MSEC      100   // ��ָ��£������������ȶ�ʱ���� 

#define MouseReport_INTERVAL_MSEC         8   // ��걨����ʱ��ms����Ƶ��125hzΪ��׼
#define ButtonPointer_Interval_MSEC      150   // ��������Ҽ���ָ��������ʱ��ms��

#define Jitter_Offset         10    // ������������΢������λ����ֵ

#define SCROLL_OFFSET_THRESHOLD_X      100   // ����λ����X��ֵ 
#define SCROLL_OFFSET_THRESHOLD_Y      100   // ����λ����Y��ֵ 



#pragma pack(push)
#pragma pack(1)
typedef struct _HYBRID_REPORT {
    UCHAR       ReportID;
    UCHAR		Confidence : 1;
    UCHAR		TipSwitch : 1;
    UCHAR		Padding1 : 2;
    UCHAR		ContactID : 4;
    USHORT		X;
    USHORT		Y;
    USHORT      ScanTime;
    UCHAR       ContactCount;
    UCHAR       IsButtonClicked : 1;
    UCHAR		Padding2 : 7;
    USHORT      C5_BLOB;

} HYBRID_REPORT, * PHYBRID_REPORT;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct _PTP_CONTACT {
    UCHAR		Confidence : 1;
    UCHAR		TipSwitch : 1;
    UCHAR		ContactID : 3;
    UCHAR		Padding : 3;
    USHORT		X;
    USHORT		Y;
} PTP_CONTACT, * PPTP_CONTACT;
#pragma pack(pop)

typedef struct _PTP_REPORT {
    UCHAR       ReportID;
    PTP_CONTACT Contacts[5];
    USHORT      ScanTime;
    UCHAR       ContactCount;
    UCHAR       IsButtonClicked;
} PTP_REPORT, * PPTP_REPORT;

CHAR UnitExponent_Table[16] ={0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,1};

double MouseSensitivityTable[3] = { 0.8,1.0,1.25};


typedef struct _PTP_PARSER {

    //����׷�ٵ���ָ����
    PTP_REPORT lastFinger;
    PTP_REPORT currentFinger;

    char nMouse_Pointer_CurrentIndex; //���嵱ǰ���ָ�봥������������������ţ�-1Ϊδ����
    char nMouse_LButton_CurrentIndex; //���嵱ǰ��������������������������ţ�-1Ϊδ����
    char nMouse_RButton_CurrentIndex; //���嵱ǰ����Ҽ���������������������ţ�-1Ϊδ����
    char nMouse_MButton_CurrentIndex; //���嵱ǰ����м���������������������ţ�-1Ϊδ����
    char nMouse_Wheel_CurrentIndex; //���嵱ǰ�����ָ����ο���ָ��������������������ţ�-1Ϊδ����

    char nMouse_Pointer_LastIndex; //�����ϴ����ָ�봥������������������ţ�-1Ϊδ����
    char nMouse_LButton_LastIndex; //�����ϴ���������������������������ţ�-1Ϊδ����
    char nMouse_RButton_LastIndex; //�����ϴ�����Ҽ���������������������ţ�-1Ϊδ����
    char nMouse_MButton_LastIndex; //�����ϴ�����м���������������������ţ�-1Ϊδ����
    char nMouse_Wheel_LastIndex; //�����ϴ������ָ����ο���ָ��������������������ţ�-1Ϊδ����

    BOOLEAN bMouse_Wheel_Mode; //����������״̬��0Ϊ����δ���1Ϊ���ּ���
    BOOLEAN bMouse_Wheel_Mode_JudgeEnable;//�����Ƿ��������б�

    LARGE_INTEGER MousePointer_DefineTime;//���ָ�붨��ʱ�䣬���ڼ��㰴�����ʱ���������ж�����м�͹��ֲ���
    float TouchPad_ReportInterval;//���崥���屨����ʱ��

    LARGE_INTEGER JitterFixStartTime; // ���������㶶������ʱ���ʱ��

    float Scroll_TotalDistanceX; //��������ۼƹ�������X
    float Scroll_TotalDistanceY; //��������ۼƹ�������Y

    ULONG tick_Count;
    LARGE_INTEGER last_Ticktime; //�ϴα����ʱ
    LARGE_INTEGER current_Ticktime;//��ǰ�����ʱ
    LARGE_INTEGER ticktime_Interval;//������ʱ��

    //
    ULONG physicalMax_X;
    ULONG physicalMax_Y;
    ULONG logicalMax_X;
    ULONG logicalMax_Y;
    CHAR unitExp;
    UCHAR unit;

    DOUBLE physical_Width_mm;
    DOUBLE  physical_Height_mm;

    //������ָͷ�ߴ��С
    float thumb_Width;//��ָͷ����
    float thumb_Height;//��ָͷ�߶�
    float thumb_Scale;//��ָͷ�ߴ����ű���
    float FingerMinDistance;//������Ч��������ָ��С����
    float FingerClosedThresholdDistance;//����������ָ��£ʱ����С����
    float FingerMaxDistance;//������Ч��������ָ������
    float TouchPad_DPMM_x;//���崥����ֱ���
    float TouchPad_DPMM_y;//���崥����ֱ���
    float PointerSensitivity_x;//����ָ�������ȼ�ָ����ƶ������ű���
    float PointerSensitivity_y;//����ָ�������ȼ�ָ����ƶ������ű���

    ULONG StartY_TOP; //����󴥺���YֵΪ���봥���嶥��10mm����Y����
    ULONG StartX_LEFT; //���������XֵΪ���봥�������������Ҳ�43.2mm����X����
    ULONG StartX_RIGHT; //���������XֵΪ���봥�������������Ҳ�43.2mm����X����
    BOOLEAN bPhysicalButtonUp;//��������״̬

} PTP_PARSER, * PPTP_PARSER;

//
typedef struct _HID_DEVICE_SETTINGS
{
    // Length of the HID Descriptor
    USHORT HidDescriptorLength;

    // BCD Version
    USHORT  BcdVersion;

    // Length of the Report Descriptor
    USHORT  ReportDescriptorLength;

    // Report Descriptor Register address
    USHORT  ReportDescriptorAddress;

    // Input Register Address
    USHORT  InputRegisterAddress;

    // Input Register Maximum Length
    USHORT  InputReportMaxLength;

    // Output Register Address
    USHORT  OutputRegisterAddress;

    // Output Register Maximum Length
    USHORT  OutputReportMaxLength;

    // Command Register Address
    USHORT  CommandRegisterAddress;

    // Data Register Address
    USHORT  DataRegisterAddress;


    // I2C device metadata
    // Vendor Id
    USHORT VendorId;//HidVendorID;

    // Device Id
    USHORT ProductId;//HidProductID;

    //Revision Id
    USHORT VersionId;//HidVersionNumber;

    // Reserved Bits
    ULONG  Reserved;

} HID_DEVICE_SETTINGS, * PHID_DEVICE_SETTINGS;

#include "poppack.h"


//
// Device context
typedef struct _DEVICE_CONTEXT
{
    // Handle to the WDF device.
    WDFDEVICE                   FxDevice;//I2C_Device;

    // Locks
    WDFSPINLOCK                 InProgressLock;

    // Interrupts, DPCs and workitems
    WDFINTERRUPT                ReadInterrupt;

    // Queues 
    WDFQUEUE                    IoctlQueue;
    WDFQUEUE                    ReportQueue;
    WDFQUEUE                    IdleQueue;
    WDFQUEUE	                ResetNotificationQueue;

    // SPB specific settings
    WDFIOTARGET                 SpbIoTarget;
    LARGE_INTEGER               SpbConnectionId;
    WDFREQUEST                  SpbRequest;

    // ACPI specific settings
    ACPI_DEVICE_SETTINGS        AcpiSettings;

    // HID specific settings
    HID_DEVICE_SETTINGS         HidSettings;
    PBYTE                       pHidInputReport;
    LIST_ENTRY                  InputReportListHead;
    WDFSPINLOCK                 InputReportListLock;
    ULONG                       InputReportListCount;

    // Flags
    BOOLEAN                     HostInitiatedResetActive;

    // Windows PTP context
    PBYTE pReportDesciptorData;
    BOOLEAN  HidReportDescriptorSaved;

    UCHAR CONTACT_COUNT_MAXIMUM;
    UCHAR PAD_TYPE;
    UCHAR INPUT_MODE;
    UCHAR FUNCTION_SWITCH;

    UCHAR REPORTID_MULTITOUCH_COLLECTION;
    UCHAR REPORTID_MOUSE_COLLECTION;

    UCHAR REPORTID_DEVICE_CAPS; 
    UCHAR REPORTSIZE_DEVICE_CAPS;

    UCHAR REPORTID_PTPHQA;
    USHORT REPORTSIZE_PTPHQA;

    UCHAR REPORTID_INPUT_MODE;
    UCHAR REPORTSIZE_INPUT_MODE;

    UCHAR REPORTID_FUNCTION_SWITCH;
    UCHAR REPORTSIZE_FUNCTION_SWITCH;

    UCHAR REPORTID_LATENCY;//�ӳ�ģʽ
    UCHAR REPORTID_CONFIG_PTP_HAPTICS_ID;//���ط�������SimpleHapticsController

    HYBRID_REPORT currentFrame;
    PTP_REPORT combinedPacket;
    BYTE contactCount;

    BOOLEAN  SetFeatureReady; 
    BOOLEAN  SetInputModeOK;
    BOOLEAN  SetFunSwicthOK;
    UCHAR   GetStringStep;

    BOOLEAN PtpInputModeOn;

    //MouseLikeTouchpad Paser context
    PTP_PARSER  tp_settings;  //PTP_PARSER����
    UCHAR MouseSensitivityIndex;
    double MouseSensitivityValue;

} DEVICE_CONTEXT, * PDEVICE_CONTEXT;


//
// Request context
typedef struct _REQUEST_CONTEXT
{
    // Handle to the WDF device
    WDFDEVICE FxDevice;

    // Handle to the WDFMemory object
    WDFMEMORY FxMemory;

} REQUEST_CONTEXT, * PREQUEST_CONTEXT;


//
// Memory context
typedef struct _MEMORY_CONTEXT
{
    // linked list entry that holds an input report
    LIST_ENTRY InputReportListEntry;

} MEMORY_CONTEXT, * PMEMORY_CONTEXT;


//
// Workitem context
typedef struct _WORKITEM_CONTEXT
{
    // Handle to a WDF device object
    WDFDEVICE FxDevice;

    // Handle to a WDF request object
    WDFREQUEST FxRequest;

} WORKITEM_CONTEXT, * PWORKITEM_CONTEXT;


//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext);
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(REQUEST_CONTEXT, GetRequestContext);
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(MEMORY_CONTEXT, GetMemoryContext);
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(WORKITEM_CONTEXT, GetWorkItemContext);

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(HID_SUBMIT_IDLE_NOTIFICATION_CALLBACK_INFO, _HidGetIdleCallbackInfo);//ԭ��ȱʧ����������

typedef enum HID_COMMAND
{
    CmdMin = 0,
    CmdReservedLow = CmdMin,
    CmdReset = 1,
    CmdGetReport = 2,
    CmdSetReport = 3,
    CmdGetIdle = 4,
    CmdSetIdle = 5,
    CmdGetProtocol = 6,
    CmdSetProtocol = 7,
    CmdSetPower = 8,
    CmdReservedHigh = 9,
    CmdMax = CmdReservedHigh
} HID_COMMAND, * PHID_COMMAND;

// Report Types
typedef enum HID_REPORT_TYPE
{
    ReportTypeMin = 0,
    ReportTypeReserved = ReportTypeMin,
    ReportTypeInput = 1,
    ReportTypeOutput = 2,
    ReportTypeFeature = 3,
    ReportTypeMax = ReportTypeFeature
} HID_REPORT_TYPE, * PHID_REPORT_TYPE;

// Power States
typedef enum HID_POWER_STATE
{
    PowerStateMin = 0,
    PowerStateOn = PowerStateMin,
    PowerStateSleep = 1,
    PowerStateMax = PowerStateSleep
} HID_POWER_STATE, * PHID_POWER_STATE;


//
// HID specific defines
const USHORT HID_DESCRIPTOR_SIZE_V1 = 0x1E;
const USHORT HID_DESCRIPTOR_BCD_VERSION = 0x0100;
const USHORT HID_DESCRIPTOR_LENGTH_FIELD_SIZE = 2;
const USHORT HID_REPORT_LENGTH_FIELD_SIZE = 2;
const USHORT HID_REPORT_ID_OPTIMAL = 15;

const PWSTR MANUFACTURER_ID_STRING = L"Microsoft";
const PWSTR PRODUCT_ID_STRING = L"HIDI2C Device";
const PWSTR SERIAL_NUMBER_STRING = L"9999";

//
// Registers 
//
const USHORT REG_SIZE = 2;

//
// ACPI defines/constants

// {3CDFF6F7-4267-4555-AD05-B30A3D8938DE}
DEFINE_GUID(ACPI_DSM_GUID_HIDI2C,
    0x3CDFF6F7, 0x4267, 0x4555, 0xAD, 0x05, 0xB3, 0x0A, 0x3D, 0x89, 0x38, 0xDE);
//#define ACPI_DSM_GUID_HIDI2C  {3CDFF6F7-4267-4555-AD05-B30A3D8938DE}


#define ACPI_DSM_REVISION               1
#define ACPI_DSM_QUERY_FUNCTION         0
#define ACPI_DSM_HIDI2C_FUNCTION        1
#define ACPI_DSM_INPUT_ARGUMENTS_COUNT  4

#define EXPECTED_IOCTL_ACPI_GET_DEVICE_INFORMATION_STRING_LENGTH 100

// Default timeout for a Request sent to a IO Target 1 second
const ULONGLONG HIDI2C_REQUEST_DEFAULT_TIMEOUT = 1000;//ԭ�ȵ�λΪ��̫���Ϊms
//
// Tag used for pool allocations.
#define HIDI2C_POOL_TAG 'IdiH'

#define HIDI2C_ALLOCATE_POOL(Type, Size) \
    ExAllocatePoolWithTag((Type), (Size), HIDI2C_POOL_TAG)

#define HIDI2C_FREE_POOL(Buffer) \
    ExFreePoolWithTag((Buffer), HIDI2C_POOL_TAG)

#define MOUSEREPORT_BUFFER_SIZE  sizeof(mouse_report_t)

#define IOCTL_INTERNAL_MOUSE_REPORT CTL_CODE(FILE_DEVICE_MOUSE,0x800,METHOD_BUFFERED,FILE_ANY_ACCESS)//�Զ���ioctl

#define FUNCTION_FROM_CTL_CODE(ctrlCode) (((ULONG)((ctrlCode) & 0x3FFC)) >> 2)

ULONG runtimes_hid = 0;
ULONG runtimes_OnInterruptIsr = 0;
ULONG runtimes_OnPostInterruptsEnabled = 0;
ULONG runtimes_OnSelfManagedIoSuspend = 0;
ULONG runtimes_ioControl = 0;


VOID RegDebug(WCHAR* strValueName, PVOID dataValue, ULONG datasizeValue);

EXTERN_C DRIVER_INITIALIZE                  DriverEntry;
EXTERN_C EVT_WDF_DRIVER_DEVICE_ADD          OnDeviceAdd;
EXTERN_C EVT_WDF_DRIVER_UNLOAD              OnDriverUnload;


// WDF Device callback events
EVT_WDF_DEVICE_PREPARE_HARDWARE                     OnPrepareHardware;
EVT_WDF_DEVICE_RELEASE_HARDWARE                     OnReleaseHardware;
EVT_WDF_DEVICE_D0_ENTRY                             OnD0Entry;
EVT_WDF_DEVICE_D0_EXIT                              OnD0Exit;
EVT_WDF_DEVICE_D0_ENTRY_POST_INTERRUPTS_ENABLED     OnPostInterruptsEnabled;
NTSTATUS OnSelfManagedIoSuspend(WDFDEVICE Device);//PFN_WDF_DEVICE_SELF_MANAGED_IO_SUSPEND              OnSelfManagedIoSuspend;


// WDF Interrupt callback events
EVT_WDF_INTERRUPT_ISR           OnInterruptIsr;


VOID
PowerIdleIrpCompletion(
    _In_ PDEVICE_CONTEXT    FxDeviceContext
);

EVT_WDF_WORKITEM PowerIdleIrpWorkitem;

// WDF Queue callback events
//
EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL OnInternalDeviceIoControl;
EVT_WDF_IO_QUEUE_IO_STOP                    OnIoStop;


//
EXTERN_C
NTSTATUS
AcpiInitialize(
    _In_ PDEVICE_CONTEXT    FxDeviceContext
);

EXTERN_C
NTSTATUS
AcpiGetDeviceInformation(
    _In_ PDEVICE_CONTEXT    FxDeviceContext
);

EXTERN_C
NTSTATUS
AcpiGetDeviceMethod(
    _In_ PDEVICE_CONTEXT    FxDeviceContext
);

EXTERN_C
NTSTATUS
AcpiDsmSupported(
    _In_ PDEVICE_CONTEXT    FxDeviceContext,
    _In_ ULONG              FunctionIndex,
    _Out_ PBOOLEAN          Supported
);

EXTERN_C
VOID
AcpiPrepareInputParametersForDsm(
    _Inout_ PACPI_EVAL_INPUT_BUFFER_COMPLEX ParametersBuffer,
    _In_ ULONG ParametersBufferSize,
    _In_ ULONG FunctionIndex
);



//
// Public Functions

NTSTATUS
SpbInitialize(
    _In_ PDEVICE_CONTEXT    FxDeviceContext,
    _In_ WDFCMRESLIST       FxResourcesRaw,
    _In_ WDFCMRESLIST       FxResourcesTranslated
);

VOID
SpbDestroy(
    _In_ PDEVICE_CONTEXT  FxDeviceContext
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS
SpbWrite(
    _In_                    WDFIOTARGET FxIoTarget,
    _In_                    USHORT      RegisterAddress,
    _In_reads_(DataLength)  PBYTE       Data,
    _In_                    ULONG       DataLength,
    _In_                    ULONG       Timeout = HIDI2C_REQUEST_DEFAULT_TIMEOUT
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS
SpbWritelessRead(
    _In_                        WDFIOTARGET FxIoTarget,
    _In_                        WDFREQUEST  FxRequest,
    _Out_writes_(DataLength)    PBYTE       Data,
    _In_                        ULONG       DataLength
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS
SpbRead(
    _In_                        WDFIOTARGET FxIoTarget,
    _In_                        USHORT      RegisterAddress,
    _Out_writes_(DataLength)    PBYTE       Data,
    _In_                        ULONG       DataLength,
    _In_                        ULONG       DelayUs,
    _In_                        ULONG       Timeout = HIDI2C_REQUEST_DEFAULT_TIMEOUT
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS
SpbWriteWrite(
    _In_                            WDFIOTARGET FxIoTarget,
    _In_                            USHORT      RegisterAddressFirst,
    _In_reads_(DataLengthFirst)     PBYTE       DataFirst,
    _In_                            USHORT      DataLengthFirst,
    _In_                            USHORT      RegisterAddressSecond,
    _In_reads_(DataLengthSecond)    PBYTE       DataSecond,
    _In_                            USHORT      DataLengthSecond
);

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS
SpbWriteRead(
    _In_                            WDFIOTARGET     FxIoTarget,
    _In_                            USHORT          RegisterAddressFirst,
    _In_reads_(DataLengthFirst)     PBYTE           DataFirst,
    _In_                            USHORT          DataLengthFirst,
    _In_                            USHORT          RegisterAddressSecond,
    _Out_writes_(DataLengthSecond)  PBYTE           DataSecond,
    _In_                            USHORT          DataLengthSecond,
    _In_                            ULONG           DelayUs
);

//
// Private Functions
//

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS
SpbSequence(
    _In_                        WDFIOTARGET     FxIoTarget,
    _In_reads_(SequenceLength)  PVOID           Sequence,
    _In_                        SIZE_T          SequenceLength,
    _Out_                       PULONG          BytesReturned,
    _In_                        ULONG           Timeout = HIDI2C_REQUEST_DEFAULT_TIMEOUT
);




NTSTATUS HidInitialize(PDEVICE_CONTEXT pDevContext, WDF_POWER_DEVICE_STATE  FxPreviousState);

NTSTATUS HidPower(PDEVICE_CONTEXT pDevContext, SHORT PowerState);

NTSTATUS HidReset(PDEVICE_CONTEXT pDevContext);

NTSTATUS HidDestroy(PDEVICE_CONTEXT pDevContext, WDF_POWER_DEVICE_STATE FxTargetState);

NTSTATUS HidSendIdleNotification(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST FxRequest,
    BOOLEAN* bRequestPendingFlag);

NTSTATUS HidGetHidDescriptor(PDEVICE_CONTEXT pDevContext);

NTSTATUS
HidGetDeviceAttributes(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST Request
);

NTSTATUS
HidGetDeviceDescriptor(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST Request
);

NTSTATUS
HidGetReportDescriptor(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST Request
);

NTSTATUS
HidGetString(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST Request
);

NTSTATUS
HidWriteReport(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST Request
);

NTSTATUS
HidReadReport(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST Request,
    BOOLEAN* requestPendingFlag
);

NTSTATUS
HidGetReport(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST Request,
    HID_REPORT_TYPE ReportType
);

NTSTATUS
HidSetReport(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST Request,
    HID_REPORT_TYPE ReportType
);

//�󲿷�usage�Ѿ���hidusage.h������,hidsdi.h������hidusage.h
// HID report descriptor constants
#define HID_TYPE_BEGIN_COLLECTION 0xA1
#define HID_TYPE_END_COLLECTION 0xC0
#define HID_TYPE_USAGE_PAGE 0x05
#define HID_TYPE_USAGE_PAGE_1 0x06
#define HID_TYPE_USAGE 0x09
#define HID_TYPE_REPORT_ID 0x85
#define HID_TYPE_REPORT_SIZE 0x75
#define HID_TYPE_REPORT_COUNT 0x95
#define HID_TYPE_REPORT_COUNT_2	0x96
#define HID_TYPE_INPUT   0x81
#define HID_TYPE_FEATURE 0xB1

#define HID_TYPE_USAGE_MINIMUM 0x19
#define HID_TYPE_USAGE_MAXIMUM 0x29
#define HID_TYPE_LOGICAL_MINIMUM 0x15
#define HID_TYPE_LOGICAL_MAXIMUM 0x25
#define HID_TYPE_LOGICAL_MAXIMUM_2 0x26
#define HID_TYPE_LOGICAL_MAXIMUM_3 0x27
#define HID_TYPE_PHYSICAL_MINIMUM 0x35
#define HID_TYPE_PHYSICAL_MAXIMUM 0x45
#define HID_TYPE_PHYSICAL_MAXIMUM_2 0x46
#define HID_TYPE_PHYSICAL_MAXIMUM_3 0x47
#define HID_TYPE_UNIT_EXPONENT 0x55
#define HID_TYPE_UNIT 0x65
#define HID_TYPE_UNIT_2 0x66

#define HID_USAGE_PAGE_VENDOR_DEFINED_DEVICE_CERTIFICATION 0xC5
#define HID_USAGE_CONFIGURATION 0x0E

#define HID_USAGE_BUTTON_STATE 0x01
#define HID_USAGE_X 0x30
#define HID_USAGE_Y 0x31
#define HID_USAGE_TIP 0x42
#define HID_USAGE_CONFIDENCE 0x47
#define HID_USAGE_WIDTH 0x48
#define HID_USAGE_HEIGHT 0x49

#define HID_USAGE_INPUT_MODE 0x52
#define HID_USAGE_CONTACT_COUNT	0x54
#define HID_USAGE_CONTACT_COUNT_MAXIMUM	0x55
#define HID_USAGE_SCAN_TIME	0x56
#define HID_USAGE_BUTTON_SWITCH 0x57
#define HID_USAGE_SURFACE_SWITCH 0x58
#define HID_USAGE_PAD_TYPE	0x59

#define HID_USAGE_LATENCY_MODE 0x60
//#define HID_USAGE_HAPTIC_INTENSITY 0x23

#define HID_COLLECTION_APPLICATION 0x01
#define HID_COLLECTION_LOGICAL 0x02


//PTP Setting Values
#define PTP_MAX_CONTACT_POINTS 5
#define PTP_BUTTON_TYPE_CLICK_PAD 0
#define PTP_BUTTON_TYPE_PRESSURE_PAD 1

#define PTP_COLLECTION_MOUSE 0
#define PTP_COLLECTION_WINDOWS 3

#define PTP_FEATURE_INPUT_COLLECTION   0
#define PTP_FEATURE_SELECTIVE_REPORTING   1
#define PTP_SELECTIVE_REPORT_Button_Surface_ON 3

#define PTP_CONTACT_CONFIDENCE_BIT   1
#define PTP_CONTACT_TIPSWITCH_BIT    2


#define DEFAULT_PTP_HQA_BLOB \
	0xfc, 0x28, 0xfe, 0x84, 0x40, 0xcb, 0x9a, 0x87, \
	0x0d, 0xbe, 0x57, 0x3c, 0xb6, 0x70, 0x09, 0x88, \
	0x07, 0x97, 0x2d, 0x2b, 0xe3, 0x38, 0x34, 0xb6, \
	0x6c, 0xed, 0xb0, 0xf7, 0xe5, 0x9c, 0xf6, 0xc2, \
	0x2e, 0x84, 0x1b, 0xe8, 0xb4, 0x51, 0x78, 0x43, \
	0x1f, 0x28, 0x4b, 0x7c, 0x2d, 0x53, 0xaf, 0xfc, \
	0x47, 0x70, 0x1b, 0x59, 0x6f, 0x74, 0x43, 0xc4, \
	0xf3, 0x47, 0x18, 0x53, 0x1a, 0xa2, 0xa1, 0x71, \
	0xc7, 0x95, 0x0e, 0x31, 0x55, 0x21, 0xd3, 0xb5, \
	0x1e, 0xe9, 0x0c, 0xba, 0xec, 0xb8, 0x89, 0x19, \
	0x3e, 0xb3, 0xaf, 0x75, 0x81, 0x9d, 0x53, 0xb9, \
	0x41, 0x57, 0xf4, 0x6d, 0x39, 0x25, 0x29, 0x7c, \
	0x87, 0xd9, 0xb4, 0x98, 0x45, 0x7d, 0xa7, 0x26, \
	0x9c, 0x65, 0x3b, 0x85, 0x68, 0x89, 0xd7, 0x3b, \
	0xbd, 0xff, 0x14, 0x67, 0xf2, 0x2b, 0xf0, 0x2a, \
	0x41, 0x54, 0xf0, 0xfd, 0x2c, 0x66, 0x7c, 0xf8, \
	0xc0, 0x8f, 0x33, 0x13, 0x03, 0xf1, 0xd3, 0xc1, \
	0x0b, 0x89, 0xd9, 0x1b, 0x62, 0xcd, 0x51, 0xb7, \
	0x80, 0xb8, 0xaf, 0x3a, 0x10, 0xc1, 0x8a, 0x5b, \
	0xe8, 0x8a, 0x56, 0xf0, 0x8c, 0xaa, 0xfa, 0x35, \
	0xe9, 0x42, 0xc4, 0xd8, 0x55, 0xc3, 0x38, 0xcc, \
	0x2b, 0x53, 0x5c, 0x69, 0x52, 0xd5, 0xc8, 0x73, \
	0x02, 0x38, 0x7c, 0x73, 0xb6, 0x41, 0xe7, 0xff, \
	0x05, 0xd8, 0x2b, 0x79, 0x9a, 0xe2, 0x34, 0x60, \
	0x8f, 0xa3, 0x32, 0x1f, 0x09, 0x78, 0x62, 0xbc, \
	0x80, 0xe3, 0x0f, 0xbd, 0x65, 0x20, 0x08, 0x13, \
	0xc1, 0xe2, 0xee, 0x53, 0x2d, 0x86, 0x7e, 0xa7, \
	0x5a, 0xc5, 0xd3, 0x7d, 0x98, 0xbe, 0x31, 0x48, \
	0x1f, 0xfb, 0xda, 0xaf, 0xa2, 0xa8, 0x6a, 0x89, \
	0xd6, 0xbf, 0xf2, 0xd3, 0x32, 0x2a, 0x9a, 0xe4, \
	0xcf, 0x17, 0xb7, 0xb8, 0xf4, 0xe1, 0x33, 0x08, \
	0x24, 0x8b, 0xc4, 0x43, 0xa5, 0xe5, 0x24, 0xc2



//ReportIDΪ������Ϊ�ϲ��������ṩ�ı���id���ϲ��෢�͸��²�������report����������ʱ��Ҫ�滻��ʵ�ʵ�ReportID���ұ������ݸ�ʽҲҪ����ʵ�ʵ��²����������·�װ,��������Ҫ��ǰ��ȡhid������������ȷ����ȷ����ֵ
//ÿ��REPORTID_���벻ͬ�����ֱ�����𣬲���ֵ��1 - 255֮��
#define TEMPORARY_REPORTID_MOUSE 0x02
#define TEMPORARY_REPORTID_MULTITOUCH 0x05
#define TEMPORARY_REPORTID_DEVICE_CAPS 0x05
#define TEMPORARY_REPORTID_INPUTMODE 0x03
#define TEMPORARY_REPORTID_FUNCTION_SWITCH 0x06   
#define TEMPORARY_REPORTID_PTPHQA 0x08

//touchpad������
const unsigned char TouchpadReportDescriptor[] = {//��������ֻ��Ϊ�ϲ�ͻ�������ʹ�ã�ʵ��hid������Ӧ�����²�������ȡ������Ϊ׼
    //MOUSE TLC
            0x05, 0x01, // USAGE_PAGE(Generic Desktop)
            0x09, 0x02, //   USAGE(Mouse)
            0xA1, 0x01, //   COLLECTION(APPlication)
            0x85, TEMPORARY_REPORTID_MOUSE, //     ReportID(Mouse ReportID)  //��ʱռλ��;��ʵ��ʹ�ö�дReportʱ��Ҫ��ǰ��ȡhid������������ȷ����ȷ����ֵ
            0x09, 0x01, //   USAGE(Pointer)
            0xA1, 0x00, //     COLLECTION(Physical)
            0x05, 0x09, //     USAGE_PAGE(Button)
            0x19, 0x01, //     USAGE_MINIMUM(button 1)   Button ������ λ 0 ����� λ1 �Ҽ��� λ2 �м�
            0x29, 0x03, //     USAGE_MAXMUM(button 3)  //0x03����������갴������
            0x15, 0x00, //     LOGICAL_MINIMUM(0)
            0x25, 0x01, //     LOGICAL_MAXIMUM(1)
            0x75, 0x01, //     REPORT_SIZE(1)
            0x95, 0x03, //     REPORT_COUNT(3)  //0x03��갴������
            0x81, 0x02, //     INPUT(Data,Var,Abs)
            0x95, 0x05, //     REPORT_COUNT(5) //��Ҫ������ٸ�bitʹ�ü�����갴��������3��bitλ��1���ֽ�8bit
            0x81, 0x03, //     INPUT (Cnst,Var,Abs)////һ��pending��λ��input��Cnst����0x03
            0x05, 0x01, //     USAGE_PAGE(Generic Desktop)
            0x09, 0x30, //     USAGE(X)       X�ƶ�
            0x09, 0x31, //     USAGE(Y)       Y�ƶ�
            0x09, 0x38, //     USAGE(Wheel)   ��ֱ����
            0x15, 0x81, //     LOGICAL_MINIMUM(-127)
            0x25, 0x7F, //     LOGICAL_MAXIMUM(127)
            0x75, 0x08, //     REPORT_SIZE(8)
            0x95, 0x03, //     REPORT_COUNT(3)
            0x81, 0x06, //     INPUT(Data,Var, Rel) //X,Y,��ֱ�������������� ���ֵ

            //�±�ˮƽ����
            0x05, 0x0C, //     USAGE_PAGE (Consumer Devices)
            0x0A, 0x38, 0x02, // USAGE(AC Pan)
            0x15, 0x81, //       LOGICAL_MINIMUM(-127)
            0x25, 0x7F, //       LOGICAL_MAXIMUM(127)
            0x75, 0x08, //       REPORT_SIZE(8)
            0x95, 0x01, //       REPORT_COUNT(1)
            0x81, 0x06, //       INPUT(data,Var, Rel) //ˮƽ���֣����ֵ
            0xC0,       //       End Connection(PhySical)
            0xC0,       //     End Connection

    //TOUCH PAD input TLC
        0x05, 0x0d,                         // USAGE_PAGE (Digitizers)          
        0x09, 0x05,                         // USAGE (Touch Pad)             
        0xa1, 0x01,                         // COLLECTION (Application)         
        0x85, TEMPORARY_REPORTID_MULTITOUCH,            //   REPORT_ID (Touch pad)      //REPORTID_MULTITOUCH               
        0x09, 0x22,                         //   USAGE (Finger)                 
        0xa1, 0x02,                         //   COLLECTION (Logical)  
        0x15, 0x00,                         //       LOGICAL_MINIMUM (0)
        0x25, 0x01,                         //       LOGICAL_MAXIMUM (1)
        0x09, 0x47,                         //       USAGE (Confidence) 
        0x09, 0x42,                         //       USAGE (Tip switch)
        0x95, 0x02,                         //       REPORT_COUNT (2)
        0x75, 0x01,                         //       REPORT_SIZE (1)
        0x81, 0x02,                         //       INPUT (Data,Var,Abs)
        0x95, 0x01,                         //       REPORT_COUNT (1)
        0x75, 0x02,                         //       REPORT_SIZE (2)
        0x25, 0x02,                         //       LOGICAL_MAXIMUM (2)
        0x09, 0x51,                         //       USAGE (Contact Identifier)
        0x81, 0x02,                         //       INPUT (Data,Var,Abs)
        0x75, 0x01,                         //       REPORT_SIZE (1)
        0x95, 0x04,                         //       REPORT_COUNT (4)             
        0x81, 0x03,                         //       INPUT (Cnst,Var,Abs)
        0x05, 0x01,                         //       USAGE_PAGE (Generic Desk..
        0x15, 0x00,                         //       LOGICAL_MINIMUM (0)
        0x26, 0xff, 0x0f,                   //       LOGICAL_MAXIMUM (4095)         
        0x75, 0x10,                         //       REPORT_SIZE (16)             
        0x55, 0x0e,                         //       UNIT_EXPONENT (-2)           
        0x65, 0x13,                         //       UNIT(Inch,EngLinear)                  
        0x09, 0x30,                         //       USAGE (X)                    
        0x35, 0x00,                         //       PHYSICAL_MINIMUM (0)         
        0x46, 0x90, 0x01,                   //       PHYSICAL_MAXIMUM (400)
        0x95, 0x01,                         //       REPORT_COUNT (1)         
        0x81, 0x02,                         //       INPUT (Data,Var,Abs)         
        0x46, 0x13, 0x01,                   //       PHYSICAL_MAXIMUM (275)
        0x09, 0x31,                         //       USAGE (Y)                    
        0x81, 0x02,                         //       INPUT (Data,Var,Abs)    
        0xc0,                               //    END_COLLECTION
        0x55, 0x0C,                         //    UNIT_EXPONENT (-4)           
        0x66, 0x01, 0x10,                   //    UNIT (Seconds)        
        0x47, 0xff, 0xff, 0x00, 0x00,      //     PHYSICAL_MAXIMUM (65535)
        0x27, 0xff, 0xff, 0x00, 0x00,         //  LOGICAL_MAXIMUM (65535) 
        0x75, 0x10,                           //  REPORT_SIZE (16)             
        0x95, 0x01,                           //  REPORT_COUNT (1) 
        0x05, 0x0d,                         //    USAGE_PAGE (Digitizers)
        0x09, 0x56,                         //    USAGE (Scan Time)    
        0x81, 0x02,                           //  INPUT (Data,Var,Abs)         
        0x09, 0x54,                         //    USAGE (Contact count)
        0x25, 0x7f,                           //  LOGICAL_MAXIMUM (127) 
        0x95, 0x01,                         //    REPORT_COUNT (1)
        0x75, 0x08,                         //    REPORT_SIZE (8)    
        0x81, 0x02,                         //    INPUT (Data,Var,Abs)
        0x05, 0x09,                         //    USAGE_PAGE (Button)         
        0x09, 0x01,                         //    USAGE_(Button 1)     
        0x25, 0x01,                         //    LOGICAL_MAXIMUM (1)          
        0x75, 0x01,                         //    REPORT_SIZE (1)              
        0x95, 0x01,                         //    REPORT_COUNT (1)             
        0x81, 0x02,                         //    INPUT (Data,Var,Abs)
        0x95, 0x07,                          //   REPORT_COUNT (7)                 
        0x81, 0x03,                         //    INPUT (Cnst,Var,Abs)
        0x05, 0x0d,                         //    USAGE_PAGE (Digitizer)
        0x85, TEMPORARY_REPORTID_DEVICE_CAPS,            //   REPORT_ID (Feature)  /Ӳ��֧�ֵ���    REPORTID_DEVICE_CAPS��REPORTID_MAX_COUNT,                   
        0x09, 0x55,                         //    USAGE (Contact Count Maximum)
        0x09, 0x59,                         //    USAGE (Pad TYpe)
        0x75, 0x04,                         //    REPORT_SIZE (4) 
        0x95, 0x02,                         //    REPORT_COUNT (2)
        0x25, 0x0f,                         //    LOGICAL_MAXIMUM (15)
        0xb1, 0x02,                         //    FEATURE (Data,Var,Abs)
        0x06, 0x00, 0xff,                   //    USAGE_PAGE (Vendor Defined)
        0x85, TEMPORARY_REPORTID_PTPHQA,               //    REPORT_ID (PTPHQA)  
        0x09, 0xC5,                         //    USAGE (Vendor Usage 0xC5)    
        0x15, 0x00,                         //    LOGICAL_MINIMUM (0)          
        0x26, 0xff, 0x00,                   //    LOGICAL_MAXIMUM (0xff) 
        0x75, 0x08,                         //    REPORT_SIZE (8)             
        0x96, 0x00, 0x01,                   //    REPORT_COUNT (0x100 (256))             
        0xb1, 0x02,                         //    FEATURE (Data,Var,Abs)
        0xc0,                               // END_COLLECTION

        //CONFIG TLC
        0x05, 0x0d,                         //    USAGE_PAGE (Digitizer)
        0x09, 0x0E,                         //    USAGE (Configuration)
        0xa1, 0x01,                         //   COLLECTION (Application)
        0x85, TEMPORARY_REPORTID_INPUTMODE,             //   REPORT_ID (Feature)      REPORTID_FEATURE       
        0x09, 0x22,                         //   USAGE (Finger)              
        0xa1, 0x02,                         //   COLLECTION (logical)     
        0x09, 0x52,                         //    USAGE (Input Mode)         
        0x15, 0x00,                         //    LOGICAL_MINIMUM (0)      
        0x25, 0x0a,                         //    LOGICAL_MAXIMUM (10)
        0x75, 0x08,                         //    REPORT_SIZE (8)         
        0x95, 0x01,                         //    REPORT_COUNT (1)         
        0xb1, 0x02,                         //    FEATURE (Data,Var,Abs    
        0xc0,                               //   END_COLLECTION
        0x09, 0x22,                         //   USAGE (Finger)              
        0xa1, 0x00,                         //   COLLECTION (physical)     
        0x85, TEMPORARY_REPORTID_FUNCTION_SWITCH,     //     REPORT_ID (Feature)              
        0x09, 0x57,                         //     USAGE(Surface switch)
        0x09, 0x58,                         //     USAGE(Button switch)
        0x75, 0x01,                         //     REPORT_SIZE (1)
        0x95, 0x02,                         //     REPORT_COUNT (2)
        0x25, 0x01,                         //     LOGICAL_MAXIMUM (1)
        0xb1, 0x02,                         //     FEATURE (Data,Var,Abs)
        0x95, 0x06,                         //     REPORT_COUNT (6)             
        0xb1, 0x03,                         //     FEATURE (Cnst,Var,Abs)
        0xc0,                               //   END_COLLECTION
        0xc0,                               // END_COLLECTION

        

};
    

/////////���HID������, �������������ң��� ���֣�ˮƽ�����ʹ�ֱ������, X,Y�������ֵ
const unsigned char MouseReportDescriptor[] = {//��������ֻ��Ϊ�ϲ�ͻ�������ʹ�ã�ʵ��hid������Ӧ�����²�������ȡ������Ϊ׼
    ///
    0x05, 0x01, // USAGE_PAGE(Generic Desktop)
    0x09, 0x02, //   USAGE(Mouse)
    0xA1, 0x01, //   COLLECTION(APPlication)
    0x85, TEMPORARY_REPORTID_MOUSE, //     ReportID(Mouse ReportID)  //��ʱռλ��;��ʵ��ʹ�ö�дReportʱ��Ҫ��ǰ��ȡhid������������ȷ����ȷ����ֵ
    0x09, 0x01, //   USAGE(Pointer)
        0xA1, 0x00, //     COLLECTION(Physical)
        0x05, 0x09, //     USAGE_PAGE(Button)
        0x19, 0x01, //     USAGE_MINIMUM(button 1)   Button ������ λ 0 ����� λ1 �Ҽ��� λ2 �м�
        0x29, 0x03, //     USAGE_MAXMUM(button 3)  //0x03����������갴������
        0x15, 0x00, //     LOGICAL_MINIMUM(0)
        0x25, 0x01, //     LOGICAL_MAXIMUM(1)
        0x75, 0x01, //     REPORT_SIZE(1)
        0x95, 0x03, //     REPORT_COUNT(3)  //0x03��갴������
        0x81, 0x02, //     INPUT(Data,Var,Abs)
        0x95, 0x05, //     REPORT_COUNT(5) //��Ҫ������ٸ�bitʹ�ü�����갴��������3��bitλ��1���ֽ�8bit
        0x81, 0x03, //     INPUT (Cnst,Var,Abs)////һ��pending��λ��input��Cnst����0x03
        0x05, 0x01, //     USAGE_PAGE(Generic Desktop)
        0x09, 0x30, //     USAGE(X)       X�ƶ�
        0x09, 0x31, //     USAGE(Y)       Y�ƶ�
        0x09, 0x38, //     USAGE(Wheel)   ��ֱ����
        0x15, 0x81, //     LOGICAL_MINIMUM(-127)
        0x25, 0x7F, //     LOGICAL_MAXIMUM(127)
        0x75, 0x08, //     REPORT_SIZE(8)
        0x95, 0x03, //     REPORT_COUNT(3)
        0x81, 0x06, //     INPUT(Data,Var, Rel) //X,Y,��ֱ�������������� ���ֵ

        //�±�ˮƽ����
        0x05, 0x0C, //     USAGE_PAGE (Consumer Devices)
        0x0A, 0x38, 0x02, // USAGE(AC Pan)
        0x15, 0x81, //       LOGICAL_MINIMUM(-127)
        0x25, 0x7F, //       LOGICAL_MAXIMUM(127)
        0x75, 0x08, //       REPORT_SIZE(8)
        0x95, 0x01, //       REPORT_COUNT(1)
        0x81, 0x06, //       INPUT(data,Var, Rel) //ˮƽ���֣����ֵ
        0xC0,       //       End Connection(PhySical)
    0xC0,       //     End Connection

};


CONST HID_DESCRIPTOR DefaultHidDescriptor = {
    0x09,   // length of HID descriptor
    0x21,   // descriptor type == HID  0x21
    0x0100, // hid spec release
    0x00,   // country code == Not Specified
    0x01,   // number of HID class descriptors
    { 0x22,   // descriptor type 
    sizeof(TouchpadReportDescriptor) }  // MouseReportDescriptor//TouchpadReportDescriptor
};


///���״̬����,��Ӧ��HID���ϱߵı���
#pragma pack(1)
struct mouse_report_t
{
    BYTE    report_id;
    BYTE    button; //0 no press, 1 left, 2 right ; 3 ����ͬʱ���£�������һ�㲻����������
    CHAR    dx;
    CHAR    dy;
    CHAR    v_wheel; // ��ֱ
    CHAR    h_wheel; // ˮƽ
};
#pragma pack()


typedef struct _PTP_DEVICE_CAPS_FEATURE_REPORT {
    UCHAR ReportID;
    UCHAR MaximumContactPoints : 4;
    UCHAR ButtonType : 4;
} PTP_DEVICE_CAPS_FEATURE_REPORT, * PPTP_DEVICE_CAPS_FEATURE_REPORT;

typedef struct _PTP_DEVICE_HQA_CERTIFICATION_REPORT {
    UCHAR ReportID;
    UCHAR CertificationBlob[256];
} PTP_DEVICE_HQA_CERTIFICATION_REPORT, * PPTP_DEVICE_HQA_CERTIFICATION_REPORT;

typedef struct _PTP_DEVICE_INPUT_MODE_REPORT {
    UCHAR ReportID;
    UCHAR Mode;
} PTP_DEVICE_INPUT_MODE_REPORT, * PPTP_DEVICE_INPUT_MODE_REPORT;

#pragma pack(push)
#pragma pack(1)
typedef struct _PTP_DEVICE_SELECTIVE_REPORT_MODE_REPORT {
    UCHAR ReportID;
    UCHAR DeviceMode;
    UCHAR ButtonReport : 1;
    UCHAR SurfaceReport : 1;
    UCHAR Padding : 6;
} PTP_DEVICE_SELECTIVE_REPORT_MODE_REPORT, * PPTP_DEVICE_SELECTIVE_REPORT_MODE_REPORT;
#pragma pack(pop)


enum CONTACT_STATE {
    CONTACT_NEW = 0,
    CONTACT_CONTINUED = 1,
    CONTACT_CONFIDENCE_CANCELLED = 2,
    CONTACT_INVALID = 3
};


NTSTATUS
PtpReportFeatures(
    _In_ WDFDEVICE Device,
    _In_ WDFREQUEST Request
);

NTSTATUS
PtpSetFeature(
    PDEVICE_CONTEXT pDevContext,
    BOOLEAN SetType
);

NTSTATUS
HidGetFeature(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST Request,
    HID_REPORT_TYPE ReportType
);

NTSTATUS
HidSetFeature(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST Request,
    HID_REPORT_TYPE ReportType
);

NTSTATUS
AnalyzeHidReportDescriptor(
    PDEVICE_CONTEXT pDevContext
);

NTSTATUS
GetReportDescriptor(
    PDEVICE_CONTEXT pDevContext
);

NTSTATUS
SendOriginalReport(PDEVICE_CONTEXT pDevContext, PVOID OriginalReport, size_t outputBufferLength);

NTSTATUS
SendPtpMouseReport(PDEVICE_CONTEXT pDevContext, mouse_report_t* pMouseReport);

NTSTATUS
SendPtpMultiTouchReport(PDEVICE_CONTEXT pDevContext, PVOID MultiTouchReport, size_t outputBufferLength);

void MouseLikeTouchPad_parse_init(PDEVICE_CONTEXT pDevContext);

void MouseLikeTouchPad_parse(PDEVICE_CONTEXT pDevContext, PTP_REPORT* pPtpReport, mouse_report_t* pMouseReport);

void SetNextSensitivity(PDEVICE_CONTEXT pDevContext);

NTSTATUS SetRegisterMouseSensitivity(PDEVICE_CONTEXT pDevContext, ULONG ms_idx);

NTSTATUS GetRegisterMouseSensitivity(PDEVICE_CONTEXT pDevContext, ULONG* ms_idx);

NTSTATUS
HidSendResetNotification(
    PDEVICE_CONTEXT pDevContext,
    WDFREQUEST Request,
    BOOLEAN* requestPendingFlag_reset
);




