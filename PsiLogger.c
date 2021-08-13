//
// © 2006 Psigenics Corporation.
//
// PsiLogger,c
//
// Version History:
// 2006.04.13  0.1.0.3  pawilber
// - creation
// - comma delimited file
//


#include <direct.h>
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <shlobj.h>


static FILE* file_ = 0;
static FILE* masterFile_ = 0;
static int psiDataLen_ = -1;
static int targetDataLen_ = -1;
static char userId_[22];
static char appName_[52];
static char appVer_[22];
static char source_[22];
static char logLine_[1024];


static double TimeNowUtc_()
{
    FILETIME fileTime;
    unsigned __int64 uint64Time;

    GetSystemTimeAsFileTime(&fileTime);

    uint64Time = fileTime.dwHighDateTime;
    uint64Time <<= 32;
    uint64Time += fileTime.dwLowDateTime;

    return ((double)((__int64)(uint64Time/10000))/1000. - 9435484800.000); // seconds from 1601 to 1900    
}


int PsiLogger_Open(char* userId, char* appName, char* appVer, char* source)
{
    char* fileName = 0;
    char homePath[MAX_PATH+1];
    char theDir[1200];
    char arcDir[1200];

    if ((strlen(userId) <= 0) || (strlen(userId) > 20))
        return -1;

    if ((strlen(appName) <= 0) || (strlen(userId) > 50))
        return -1;

    if ((strlen(appVer) <= 0) || (strlen(userId) > 20))
        return -1;

    strcpy(userId_, userId);
    strcpy(appName_, appName);
    strcpy(appVer_, appVer);
    strcpy(source_, source);

    if (file_ != 0)
        return -1;
    
    if (FAILED(SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, homePath)))
        return -1;
    sprintf(theDir, "%s\\PsiTrainer ANN Data\\", homePath);
    CreateDirectory(theDir, NULL);
    strcat(theDir, userId);
    strcpy(arcDir, theDir);
    strcat(arcDir, "\\");
    CreateDirectory(theDir, NULL);
    strcpy(theDir, arcDir);
    strcat(theDir, "Session");
    CreateDirectory(theDir, NULL);
    strcpy(theDir, arcDir);
    strcat(theDir, "Archive");
    CreateDirectory(theDir, NULL);
    strcpy(theDir, arcDir);
        
    fileName = malloc(strlen(userId) + 1000);
    sprintf(fileName, "%s\\Session\\%s_%010u.csv", theDir, userId, (unsigned long)TimeNowUtc_());
    file_ = fopen(fileName, "w+");
    sprintf(fileName, "%s\\%s_dat.csv", theDir, userId, (unsigned long)TimeNowUtc_());
    masterFile_ = fopen(fileName, "a+");
    free(fileName);

    if (masterFile_ != 0)
        fseek(masterFile_, 0, SEEK_END);
    
    return (masterFile_ != 0)? 0 : -1;
}


int PsiLogger_Close()
{
    int closeStatus = (fclose(file_) == 0)? 0 : -1;
    file_ = 0;
    psiDataLen_ = -1;
    targetDataLen_ = -1;
    return closeStatus;
}


char* PsiLogger_CreateS1Header()
{
    int topStrSize = 0;

    topStrSize = 30 + strlen(userId_) + strlen(appName_) + strlen(appVer_) +1;
    if (topStrSize > 255)
        return 0;

    sprintf(logLine_, "%010.3f,\"%s\",\"%s\",\"%s\"", TimeNowUtc_(), userId_, appName_, appVer_);

    return logLine_;
}

char* PsiLogger_CreateS3Header()
{
    int topStrSize = 0;

    topStrSize = 30 + strlen(userId_) + strlen(appName_) + strlen(appVer_) +1;
    if (topStrSize > 255)
        return 0;

    sprintf(logLine_, "%010.3f,\"%s\",\"%s\",\"%s\",\"%s\"", TimeNowUtc_(), userId_, appName_, appVer_, source_);

    return logLine_;
}

char* PsiLogger_CreateS1(char hitStatus, char targeted, char psiMode)
{
    int i= 0;

    sprintf(logLine_, "%010.3f, , ,", TimeNowUtc_());

    logLine_[15] = hitStatus+48;
    logLine_[17] = targeted+48;
    logLine_[19] = psiMode+48;
    logLine_[20] = '\0';
    
    return logLine_;
}


char* PsiLogger_CreateS2(char hitStatus, char targeted, char psiMode, double* norm7)
{
    sprintf(logLine_, "%010.3f, , , ,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f", TimeNowUtc_(), norm7[0], norm7[1], norm7[2], norm7[3], norm7[4], norm7[5], norm7[6]);

    logLine_[15] = hitStatus+48;
    logLine_[17] = targeted+48;
    logLine_[19] = psiMode+48;

    return logLine_;
}

char* PsiLogger_CreateS3(char hitStatus, char targeted, char psiMode, double* norm21)
{
    sprintf(logLine_, "%010.3f, , , ,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f,%+1.6f", TimeNowUtc_(), norm21[0], norm21[1], norm21[2], norm21[3], norm21[4], norm21[5], norm21[6], norm21[7], norm21[8], norm21[9], norm21[10], norm21[11], norm21[12], norm21[13], norm21[14], norm21[15], norm21[16], norm21[17], norm21[18], norm21[19], norm21[20]);

    logLine_[15] = hitStatus+48;
    logLine_[17] = targeted+48;
    logLine_[19] = psiMode+48;

    return logLine_;
}

static int ArchiveMaster()
{
    char homePath[MAX_PATH+1];
    char thePath[1200];
    char archivePath[1200];
    char* header;

    if (FAILED(SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, homePath)))
        return -1;

    fclose(masterFile_);
    sprintf(thePath, "%s\\PsiTrainer ANN Data\\%s\\%s_dat.csv", homePath, userId_, userId_);
    sprintf(archivePath, "%s\\PsiTrainer ANN Data\\%s\\Archive\\%s_dat_%010u.csv", homePath, userId_, userId_, (unsigned long)TimeNowUtc_());
    CopyFile(thePath, archivePath, TRUE);
    
    masterFile_ = fopen(thePath, "w+");
    header = PsiLogger_CreateS3Header();
    if (fputs(header, masterFile_) < 0)
        return -1;
    if (fputs("\n", masterFile_) < 0)
        return -1;
    if (fflush(masterFile_) != 0)
        return -1;

    return (masterFile_ != 0)? 0 : -1;
}


int PsiLogger_Log(char* entry)
{
    if ((entry == 0) || (strlen(entry) <= 0))
        return -1;

    if (fputs(entry, file_) < 0)
        return -1;
    if (fputs("\n", file_) < 0)
        return -1;
    if (fflush(file_) != 0)
        return -1;

    if ((ftell(masterFile_) == 0) && (strlen(entry) < 80))
    {
        if (fputs(entry, masterFile_) < 0)
            return -1;
        if (fputs("\n", masterFile_) < 0)
            return -1;
        if (fflush(masterFile_) != 0)
            return -1;
    }
    else if ((ftell(masterFile_) != 0) && (strlen(entry) > 80))
    {
        if (fputs(entry, masterFile_) < 0)
            return -1;
        if (fputs("\n", masterFile_) < 0)
            return -1;
        if (fflush(masterFile_) != 0)
            return -1;

        if (ftell(masterFile_) > 800000)
            ArchiveMaster();
    }

    return 0;
}
