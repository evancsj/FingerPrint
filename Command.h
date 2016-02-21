#include "UART.h"

void TurnOnLED(int fd);
void TurnOffLED(int fd);
void Identify(int fd);
void Verify(int fd);
void Enroll(int fd);
void EnrollOneTime(int fd);
void ClearTemplate(int fd);
void ClearAllTemplate(int fd);
void GetEmptyID(int fd);
void GetTemplateStatus(int fd);
void GetBrokenTemplate(int fd);
void ReadTemplate(int fd);
void WriteTemplate(int fd); //not finish
void SetSecurityLevel(int fd);
void GetSecurityLevel(int fd);
void SetFingerTimeOut(int fd);
void GetFingerTimeOut(int fd);
void SetDeviceID(int fd);
void GetDeviceID(int fd);
void GetFWVersion(int fd);
void FingerDetect(int fd); //some mistakes
void SetBaudrate(int fd);
void SetDuplicationCheck(int fd);
void GetDuplicationCheck(int fd);
void EnterStandbyMode(int fd);
void EnrollAndStoreinRAM(int fd);
void GetEnrollData(int fd);
void GetFeatureDataofCapturedFP(int fd);
void VerifyDownloadFeatureWithCapturedFP(int fd);//not finish
void IdentifyDownloadFeatureWithCaptureFP(int fd);//not finish
void GetDeviceName(int fd);
void IdentifyFree(int fd);//not finish
void SetDevicePassword(int fd);
void VerifyDevicePassword(int fd);
void GetEnrollCount(int fd);
void ChangeTemplate(int fd);
void SetOperationMode(int fd);
void GetOperationMode(int fd);
void FPCancel(int fd);//not finish
void TestConnect(int fd);

