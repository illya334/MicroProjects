#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <winioctl.h>
#include <cmath>

using namespace std;

#define reg register
#define uint unsigned int

unsigned int strlendz(char *arr){ // String Len Double Zero
    for(register unsigned int i=0; ; i++)
        if(arr[i] == '\0' && arr[i+1] == '\0')
            return i;
}

int main() {
    char *logicalDriveStrings = (char*)LocalAlloc(LPTR, MAX_PATH);
    char *DriveName =           (char*)LocalAlloc(LPTR, MAX_PATH);
    char *filePath =            (char*)LocalAlloc(LPTR, MAX_PATH);
    uint diskNum = 0;

    if (!GetLogicalDriveStrings(MAX_PATH, logicalDriveStrings)){ // Отримати список дисків
        cout << "Error in 'GetLogicalDriveStrings'. Code: " << GetLastError();
        system("pause");
        return 1;
    }

    // видалення слеш, тому що QueryDosDevice почне кричати
    uint sizeLogicalDriveStrings = strlendz(logicalDriveStrings);
    for(reg uint i=0; i < sizeLogicalDriveStrings; i++){
        if(logicalDriveStrings[i] == '\\')
            for(reg uint j=i; j < sizeLogicalDriveStrings; j++)
                logicalDriveStrings[j] = logicalDriveStrings[j+1];
    }

    char *tmplogicalDriveStrings = logicalDriveStrings;
    while(tmplogicalDriveStrings[0] != '\0') {
        if (!QueryDosDevice(tmplogicalDriveStrings, DriveName, 1024)){ // Отримання фізичне ім'я диску
            cout << "Error in 'QueryDosDevice'. Code: " << GetLastError();
            system("pause");
            return 1;
        }

        cout << diskNum << ") Drive '" << tmplogicalDriveStrings << "' is '" << DriveName << "'\n";

        tmplogicalDriveStrings += strlen(tmplogicalDriveStrings) + 1;
        diskNum++;
    }

    reg uint tmpDiskNum = diskNum;
    cout << "select disk: ";
    cin >> diskNum;

    if(diskNum > tmpDiskNum || diskNum < 0){
        cout << "Error invalid disk" << endl;
        system("pause");
        return 1;
    }

    char drivePath[20] = "\\\\.\\";
    // Отримуємо ім'я диску (\\.\A:)
    while(1){
        if(diskNum > 0){
            diskNum--;
            logicalDriveStrings += strlen(logicalDriveStrings) + 1;
        } else {
            break;
        }
    }
    strcat(drivePath, logicalDriveStrings);

    cout << "\nAbout:\n";

    DWORD cnt;
    DISK_GEOMETRY dg;

    HANDLE hDevice = CreateFileA(drivePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDevice == INVALID_HANDLE_VALUE){
        cout << "Error in 'CreateFileA'. Code: " << GetLastError() << endl;
        cout << "Try running the program with administrator rights." << endl;
        system("pause");
        return 1;
    }

    if (!DeviceIoControl(hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &dg, sizeof(DISK_GEOMETRY), &cnt, NULL)){
        cout << "Error in 'DeviceIoControl'. Code: " << GetLastError() << endl;
        system("pause");
        return 1;
    }

    cout << "Cylinders: " << dg.Cylinders.QuadPart << endl;
    cout << "Tracks per cylinder: " << dg.TracksPerCylinder << endl;
    cout << "Sectors per track: " << dg.SectorsPerTrack << endl;
    cout << "Bytes per sector: " << dg.BytesPerSector << endl;
    uint diskSize = dg.Cylinders.QuadPart * dg.TracksPerCylinder * dg.SectorsPerTrack * dg.BytesPerSector;
    uint sectorsOnDrive = ceil( (float)diskSize / dg.BytesPerSector );
    cout << "Disk size: " << diskSize << " bytes (" << (float)(diskSize)/1024/1024 << " MB)" << endl;

    cout << "\nPath to the file to be written to disk: ";
    cin >> filePath;

    HANDLE hFile = CreateFileA(filePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        cout << "Invalid path or file not found. Error code 'CreateFileA': " << GetLastError() << endl;
        cout << "\nRecommendation: Perhaps the reason is that the program does not support Unicode strings, try moving the file so that the path and file name contain only English characters. For example: \"C:\\boot.bin\"." << endl;
        cout << "\nOr there are double quotes." << endl;
        system("pause");
        return 1;
    }
    uint fileSize = GetFileSize(hFile, NULL);
    uint fileSizeSectors = ceil( (float)fileSize / dg.BytesPerSector );

    cout << "\nFile size: " << fileSize << " bytes\n";

    char tmpChar = 0;
    uint tmpUint = 0;

    BOOL writeOtherSectors = FALSE;
    if(fileSize >= dg.BytesPerSector){
        writeOnOtherSectorsQuestion:
        cout << "\nYour file has exceeded the disk sector size.\nWhat to do?\n1) Write the file to other sectors.\n2) Write the file only to the selected sector, the rest of the file data is ignored.\n";
        cin >> tmpUint;
        if(tmpUint == 1 || tmpUint == 2){
            if(tmpUint == 1) writeOtherSectors = TRUE;
        } else goto writeOnOtherSectorsQuestion;
    }

    BOOL bootSector = FALSE;
    bootableQuestion:
    cout <<"\nMake the first sector bootable? (y/n) ";
    cin >> tmpChar;

    if(tmpChar == 'y' || tmpChar == 'n' || tmpChar == 'Y' || tmpChar == 'N'){
        if(tmpChar == 'y' || tmpChar == 'Y') bootSector = TRUE;
    } else goto bootableQuestion;

    uint sectorToWrite = 0;
    sectorNumberQuestion:
    cout << "Specify the sector number where to write the file: ";
    cin >> sectorToWrite;

    if( sectorToWrite > sectorsOnDrive ){
        cout << "ERROR: too big number - no such sector." << endl;
        goto sectorNumberQuestion;
    }

    system("cls");

    cout << "About disk:" << endl;
    cout << "Cylinders: " << dg.Cylinders.QuadPart << endl;
    cout << "Tracks per cylinder: " << dg.TracksPerCylinder << endl;
    cout << "Sectors per track: " << dg.SectorsPerTrack << endl;
    cout << "Bytes per sector: " << dg.BytesPerSector << endl;
    cout << "Disk size: " << diskSize << " bytes (" << (float)(diskSize)/1024/1024 << " MB)" << endl;
    cout << endl;

    cout << "Address:" << endl;
    cout << "\tSector: " << sectorToWrite << endl;

    cout << "\nBoot sector: " << (bootSector ? "TRUE" : "FALSE") << endl;

    cout << "\nFile size: " << fileSize << " bytes (" << fileSizeSectors << "/" << sectorsOnDrive << " sectors)" << endl;

    if(fileSizeSectors > sectorsOnDrive) cout << "Disk overflow - file will be truncated." << endl;

    cout << "\nIf the data is incorrect, try restarting the program." << endl;

    system("pause");

    if(SetFilePointer(hDevice, sectorToWrite * dg.BytesPerSector, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER){
        cout << "ERROR: Cant set offset. Code: "<< GetLastError() << endl;
        system("pause");
        return 1;
    }

    cout << "Reading file...\t";

    char *fileData = (char*)LocalAlloc(LPTR, fileSizeSectors * dg.BytesPerSector);
    if(!ReadFile(hFile, fileData, fileSize, &cnt, NULL)){
        cout << "\nERROR: Cant read the file. Code: "<< GetLastError() << endl;
        system("pause");
        return 1;
    }
    cout << "OK" << endl;

    cout << "Writing...\t";

    if(SetFilePointer(hDevice, sectorToWrite * dg.BytesPerSector, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER){
        cout << "\nERROR: Cant set offset. Code: "<< GetLastError() << endl;
        system("pause");
        return 1;
    }

    if(bootSector){
        fileData[510] = '\x55';
        fileData[511] = '\xAA';
    }

    if(!writeOtherSectors) fileSizeSectors = 1;

    for(uint i=0; i < fileSizeSectors && i < sectorsOnDrive; i++){
        if(!WriteFile(hDevice, fileData, dg.BytesPerSector, &cnt, NULL)){
            cout << "\nERROR: Cant write file on drive. Code: " << GetLastError() << endl;
            system("pause");
            return 1;
        }

        fileData += dg.BytesPerSector;
    }

    cout << "OK" << endl;

    system("pause");

    return 0;
}
