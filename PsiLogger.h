//
// © 2006 Psigenics Corporation.
//
// PsiLogger,h
//
// Version History:
// 2006.04.13  0.1.0.3  pawilber
// - creation


#ifdef __cplusplus
extern "C" {
#endif


int PsiLogger_Open(char* userId, char* appName, char* appVer, char* source);
int PsiLogger_Close();
int PsiLogger_Log(char* entry);

char* PsiLogger_CreateS1Header();
char* PsiLogger_CreateS3Header();
char* PsiLogger_CreateS1(char hitStatus, char targeted, char psiMode);
char* PsiLogger_CreateS2(char hitStatus, char targeted, char psiMode, double* norm7);
char* PsiLogger_CreateS3(char hitStatus, char targeted, char psiMode, double* norm21);


#ifdef __cplusplus
}
#endif
