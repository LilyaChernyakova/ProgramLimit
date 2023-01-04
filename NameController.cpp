#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <winnt.h>
#include <fstream>
#include <string>
#include <strsafe.h>
#include <time.h> 

using namespace std;

int main() {
    time_t start, end;
    _TCHAR NameControllerPath[] = _T("Software\\NameController\\");
    HKEY hKey;

    // Если программа открыта впервые
    if (RegOpenKeyEx(HKEY_CURRENT_USER, NameControllerPath, 0, KEY_READ, &hKey) == ERROR_FILE_NOT_FOUND) {
        cout << "Congrats! It's your first login\n";
        if (RegCreateKeyEx(HKEY_CURRENT_USER, NameControllerPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
            cout << "RegCreateKeyEx error\n";
            return 1;
        }
        DWORD time_first_value = 0;
        DWORD limit_first_value = 0;
        DWORD version_first_value = 0;

        if (RegSetValueEx(hKey, _T("Time Limit"), 0, REG_DWORD, (const BYTE*)&time_first_value, sizeof(time_first_value)) != ERROR_SUCCESS) {
            cout << "RegSetValueEx error\n";
            return 2;
        }
        if (RegSetValueEx(hKey, _T("Starts Limit"), 0, REG_DWORD, (const BYTE*)&limit_first_value, sizeof(limit_first_value)) != ERROR_SUCCESS) {
            cout << "RegSetValueEx error\n";
            return 2;
        }
        if (RegSetValueEx(hKey, _T("Full Version"), 0, REG_DWORD, (const BYTE*)&version_first_value, sizeof(version_first_value)) != ERROR_SUCCESS) {
            cout << "RegSetValueEx error\n";
            return 2;
        }
        if (RegCloseKey(hKey) != ERROR_SUCCESS) {
            cout << "RegCloseKey error\n";
            return 3;
        }
    }

    // Если программа запускается не впервые, работаем
    if (RegOpenKeyEx(HKEY_CURRENT_USER, NameControllerPath, 0, KEY_READ | KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        DWORD starts_val;
        DWORD starts_val_dataSize = sizeof(starts_val);
        DWORD version_val;
        DWORD version_val_dataSize = sizeof(version_val);
        DWORD time_val;
        DWORD time_val_dataSize = sizeof(version_val);

        // Проверяем лимит и версию
        if (RegGetValueA(HKEY_CURRENT_USER, "Software\\NameController\\", "Time Limit", RRF_RT_DWORD, nullptr, &time_val, &time_val_dataSize) == ERROR_SUCCESS) {
            if (RegGetValueA(HKEY_CURRENT_USER, "Software\\NameController\\", "Starts Limit", RRF_RT_DWORD, nullptr, &starts_val, &starts_val_dataSize) == ERROR_SUCCESS) {
                if (RegGetValueA(HKEY_CURRENT_USER, "Software\\NameController\\", "Full Version", RRF_RT_DWORD, nullptr, &version_val, &version_val_dataSize) == ERROR_SUCCESS) {
                    // Если лимит исчерпан
                    if ((starts_val >= 3 || time_val >= 60) && version_val == 0) {
                        cout << "Sorry, program limit reached. Install full version [type 1]\n";
                        int inp;
                        cin >> inp;
                        if (inp == 1) {
                            version_val = 1;
                            version_val_dataSize = sizeof(version_val);
                            if (RegSetValueEx(hKey, _T("Full Version"), 0, REG_DWORD, (const BYTE*)&version_val, version_val_dataSize) != ERROR_SUCCESS) {
                                cout << "RegSetValueEx error\n";
                                return 2;
                            }
                            cout << "Full version installed\n" << endl;
                        }
                    }
                    // Если лимит не исчерпан
                    else {
                        // Ввод имени и все, что этого касается
                        cout << "Input your name\n";
                        time(&start);
                        string inp;
                        cin >> inp;

                        string file_str;
                        ifstream data_file;
                        data_file.open("C:\\Users\\Lenovo\\Desktop\\Папка студента\\3 курс\\ТиМП\\лаба2\\data.txt", ios::in);
                        int isHave = 0;
                        while (getline(data_file, file_str)) {
                            if (file_str.compare(inp) == 0) {
                                cout << "Your name exists already\n";
                                isHave = 1;
                                break;
                            };
                        }
                        data_file.close();

                        if (isHave == 0) {
                            ofstream data_file;
                            data_file.open("C:\\Users\\Lenovo\\Desktop\\Папка студента\\3 курс\\ТиМП\\лаба2\\data.txt", ios::app);
                            data_file << inp << "\n";
                            data_file.close();
                        }
                        // Запись новых лимитов
                        time(&end);
                        double seconds = difftime(end, start);
                        time_val += (int)seconds;
                        starts_val += 1;
                        if (version_val == 0) {
                            printf("Program used %i out of 3\n", starts_val);
                            printf("Program used %d out of 60 sec\n", time_val);

                        }
                        else {
                            printf("Program used %i out of oo\n", starts_val);
                            printf("Program used %d out of oo sec\n", time_val);
                        }
                        if (RegSetValueEx(hKey, _T("Starts Limit"), 0, REG_DWORD, (const BYTE*)&starts_val, sizeof(starts_val)) != ERROR_SUCCESS) {
                            cout << "RegSetValueEx error\n";
                            return 2;
                        }
                        if (RegSetValueEx(hKey, _T("Time Limit"), 0, REG_DWORD, (const BYTE*)&time_val, sizeof(time_val)) != ERROR_SUCCESS) {
                            cout << "RegSetValueEx error\n";
                            return 2;
                        }
                    }
                }
                else {
                    cout << "RegGetValueA error\n";
                    return 4;
                }
            }
            else {
                cout << "RegGetValueA error\n";
                return 4;
            }
        }
        else {
            cout << "RegGetValueA error\n";
            return 4;
        }


        if (RegCloseKey(hKey) != ERROR_SUCCESS) {
            cout << "RegCloseKey error\n" << endl;
            return 3;
        }
    }
    else {
        cout << "RegOpenKeyEx error\n";
        return -1;
    }

    system("pause");
    return 0;
}