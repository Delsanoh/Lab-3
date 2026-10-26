#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "functions.h"

using namespace std;

struct MarkerData {
    int number;
    vector<int>* arr;
    int size;
    CRITICAL_SECTION* cs;
    HANDLE hStart;
    HANDLE hContinue;
    HANDLE hCantContinue;
    HANDLE hStop;
};

DWORD WINAPI marker(LPVOID lpParam) {
    MarkerData* data = (MarkerData*)lpParam;
    int rand_number = data->number;

    WaitForSingleObject(data->hStart, INFINITE);
    srand(rand_number);

    unsigned int count = 0;

    while (1) {
        unsigned int ind = rand() % data->size;

        EnterCriticalSection(data->cs);
        bool can_mark = try_mark_element(*(data->arr), ind, rand_number);
        LeaveCriticalSection(data->cs);

        if (can_mark) {
            Sleep(5);
            ++count;
            Sleep(5);
            continue;
        }

        cout << "\n[marker " << rand_number << "] marked: " << count
            << " elements, can't mark index: " << ind << "\n";

        SetEvent(data->hCantContinue);

        HANDLE events[2] = { data->hContinue, data->hStop };
        DWORD result = WaitForMultipleObjects(2, events, FALSE, INFINITE);

        if (result == WAIT_OBJECT_0 + 1) {
            EnterCriticalSection(data->cs);
            reset_marked_elements(*(data->arr), rand_number);
            LeaveCriticalSection(data->cs);

            cout << "[marker " << rand_number << "] finished.\n";
            return 0;
        }
    }
    return 0;
}

int main() {
    setlocale(LC_ALL, "Russian");

    int arr_size;
    cout << "Enter array size: ";
    cin >> arr_size;
    if (cin.fail() || arr_size <= 0) return 1;

    vector<int> arr(arr_size, 0);

    int marker_count;
    cout << "Enter number of markers: ";
    cin >> marker_count;
    if (cin.fail() || marker_count <= 0) return 1;

    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);

    HANDLE hStart = CreateEvent(NULL, TRUE, FALSE, NULL);

    vector<HANDLE> hCantContinue(marker_count);
    vector<MarkerData> markers(marker_count);
    vector<HANDLE> hThreads(marker_count);

    for (int i = 0; i < marker_count; ++i) {
        markers[i].number = i + 1;
        markers[i].arr = &arr;
        markers[i].size = arr_size;
        markers[i].cs = &cs;
        markers[i].hStart = hStart;

        markers[i].hCantContinue = CreateEvent(NULL, TRUE, FALSE, NULL);
        markers[i].hContinue = CreateEvent(NULL, FALSE, FALSE, NULL);
        markers[i].hStop = CreateEvent(NULL, FALSE, FALSE, NULL);

        hCantContinue[i] = markers[i].hCantContinue;

        hThreads[i] = CreateThread(NULL, 0, marker, &markers[i], 0, NULL);
    }

    SetEvent(hStart);

    int finished_count = 0;
    vector<bool> finished(marker_count, false);

    while (finished_count < marker_count) {

        WaitForMultipleObjects(marker_count, hCantContinue.data(), TRUE, INFINITE);

        cout << "\nArray: ";
        for (int i = 0; i < arr_size; ++i) cout << arr[i] << ' ';
        cout << '\n';

        int num;
        while (true) {
            cout << "Enter marker number to finish (1-" << marker_count << "): ";
            if (!(cin >> num)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cerr << "Invalid input!\n";
                continue;
            }
            if (num < 1 || num > marker_count) {
                cerr << "Invalid marker number!\n";
                continue;
            }
            if (finished[num - 1]) {
                cerr << "Marker " << num << " already finished!\n";
                continue;
            }
            break;
        }

        SetEvent(markers[num - 1].hStop);
        WaitForSingleObject(hThreads[num - 1], INFINITE);
        finished[num - 1] = true;
        ++finished_count;

        cout << "\nArray after finishing marker " << num << ":\n";
        for (int i = 0; i < arr_size; ++i) cout << arr[i] << ' ';
        cout << '\n';

        for (int i = 0; i < marker_count; ++i) {
            if (!finished[i]) {
                ResetEvent(hCantContinue[i]);
                SetEvent(markers[i].hContinue);
            }
        }
    }

    cout << "\nAll markers finished.\n";

    for (int i = 0; i < marker_count; i++) {
        CloseHandle(hThreads[i]);
        CloseHandle(markers[i].hCantContinue);
        CloseHandle(markers[i].hContinue);
        CloseHandle(markers[i].hStop);
    }

    CloseHandle(hStart);
    DeleteCriticalSection(&cs);
    return 0;
}