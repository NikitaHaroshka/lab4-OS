#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <Windows.h>
using namespace std;

volatile int Base[10][10];
HANDLE mutex[10][10];  // Массив мьютексов для каждой ячейки
HANDLE hSemaphore;
CRITICAL_SECTION cs;

DWORD WINAPI thread1(LPVOID) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(1, 10);

    for (int i = 0; i < 10; i++) {
        int randomCell1X1 = distr(gen);
        int randomCell1Y1 = distr(gen);
        EnterCriticalSection(&cs);
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm tm_utc = {};
        gmtime_s(&tm_utc, &now_time);
        cout << put_time(&tm_utc, "%H:%M:%S") << " Mexica " << endl;
        LeaveCriticalSection(&cs);

        WaitForSingleObject(mutex[randomCell1X1][randomCell1Y1], INFINITE);

        EnterCriticalSection(&cs);
        Base[randomCell1X1][randomCell1Y1] += 100;
        cout << "Thread 1 changed cell (" << randomCell1X1 << "," << randomCell1Y1 << ") to " << Base[randomCell1X1][randomCell1Y1] << endl;
        
        LeaveCriticalSection(&cs);

        // Освобождение мьютекса
        ReleaseMutex(mutex[randomCell1X1][randomCell1Y1]);
        ReleaseSemaphore(hSemaphore, 1, NULL);
        
    }
    return 0;
}

DWORD WINAPI thread2(LPVOID) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(1, 10);
    for (int i = 0; i < 10; i++) {
        int randomCell1X2 = distr(gen);
        int randomCell1Y2 = distr(gen);
        EnterCriticalSection(&cs);
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm tm_utc = {};
        gmtime_s(&tm_utc, &now_time);
        cout << put_time(&tm_utc, "%H:%M:%S") << " America " << endl;
        LeaveCriticalSection(&cs);
        WaitForSingleObject(mutex[randomCell1X2][randomCell1Y2], INFINITE);

        EnterCriticalSection(&cs);
        Base[randomCell1X2][randomCell1Y2] += 100;
        cout << "Thread 2 changed cell (" << randomCell1X2 << "," << randomCell1Y2 << ") to " << Base[randomCell1X2][randomCell1Y2] << endl;
        
        LeaveCriticalSection(&cs);

        // Освобождение мьютекса
        ReleaseMutex(mutex[randomCell1X2][randomCell1Y2]);
        ReleaseSemaphore(hSemaphore, 1, NULL);
        
    }
    return 0;
}

DWORD WINAPI thread3(LPVOID) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(1, 10);

    for (int i = 0; i < 10; i++) {
        int randomCell1X3 = distr(gen);
        int randomCell1Y3 = distr(gen) ;
        EnterCriticalSection(&cs);
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm tm_utc = {};
        gmtime_s(&tm_utc, &now_time);
        cout << put_time(&tm_utc, "%H:%M:%S") << " Canada " << endl;
        LeaveCriticalSection(&cs);
        WaitForSingleObject(mutex[randomCell1X3][randomCell1Y3], INFINITE);

        EnterCriticalSection(&cs);
        Base[randomCell1X3][randomCell1Y3] += 100;
        cout << "Thread 3 changed cell (" << randomCell1X3 << "," << randomCell1Y3 << ") to " << Base[randomCell1X3][randomCell1Y3] << endl;
        
        LeaveCriticalSection(&cs);

        // Освобождение мьютекса
        ReleaseMutex(mutex[randomCell1X3][randomCell1Y3]);
        ReleaseSemaphore(hSemaphore, 1, NULL);
        
    }
    return 0;
}


int main()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(1, 10);
    InitializeCriticalSection(&cs);

    for (int i = 0; i < 10; i++) {
        cout << "|";
        for (int j = 0; j < 10; j++) {
            Base[i][j] = distr(gen);
            cout << setw(3) << Base[i][j] << " |";
        }
        cout << endl;
    }
    

    // Инициализация мьютексов для каждой ячейки матрицы
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            mutex[i][j] = CreateMutex(NULL, FALSE, NULL);  // Создаем мьютекс для каждой ячейки
        }
    }

    HANDLE hThread1, hThread2, hThread3;
    DWORD IDThread1, IDThread2, IDThread3;

    hSemaphore = CreateSemaphore(NULL, 0, 100, NULL);
    if (hSemaphore == NULL) return GetLastError();
    
    hThread1 = CreateThread(NULL, 0, thread1, NULL, 0, &IDThread1);
    hThread2 = CreateThread(NULL, 0, thread2, NULL, 0, &IDThread2);
    hThread3 = CreateThread(NULL, 0, thread3, NULL, 0, &IDThread3);

    if (hThread1 == NULL || hThread2 == NULL || hThread3 == NULL) {
        return GetLastError();
    }

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);
    WaitForSingleObject(hThread3, INFINITE);

    
    while (true) {
        int X, Y;
        EnterCriticalSection(&cs);

        cout << "Choose cell: ";
        cin >> X >> Y;
        LeaveCriticalSection(&cs);

        if (X == 107 && Y == 42) {
            EnterCriticalSection(&cs);
            cout << "Current data: " << endl;
            for (int i = 0; i < 10; i++) {
                cout << "|";
                
                for (int j = 0; j < 10; j++) {
                    cout << setw(3) << Base[i][j] << " |";
                    
                }
                cout << endl;
            }
            LeaveCriticalSection(&cs);
           
            return 0;
        }
        WaitForSingleObject(hSemaphore, INFINITE);
        EnterCriticalSection(&cs);
        cout << "Data in this cell " << Base[X][Y] << ' '<<endl;
        LeaveCriticalSection(&cs);
    }

    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hThread3);
    CloseHandle(hSemaphore);
    DeleteCriticalSection(&cs);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            CloseHandle(mutex[i][j]);
        }
    }

    return 0;
}
