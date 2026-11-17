#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Statistics {
    long long comparisons = 0;
    long long swaps = 0;
    long long calls = 0;
};

struct ArrayInfo {
    int count = 0;
    long long sum = 0;
    long long squareSum = 0;
};

bool tracing = false;
const int TRACE_LIMIT = 30;

void printArray(const vector<int>& a) {
    cout << "[ ";

    for (int x : a) {
        cout << x << " ";
    }

    cout << "]" << endl;
}

void printPart(const vector<int>& a, int low, int high) {
    cout << "[ ";

    for (int i = low; i <= high; i++) {
        cout << a[i] << " ";
    }

    cout << "]";
}

void exchange(int& a, int& b, Statistics& s) {
    if (&a == &b) return;

    int temp = a;
    a = b;
    b = temp;

    s.swaps++;
}

ArrayInfo getArrayInfo(const vector<int>& a) {
    ArrayInfo info;

    info.count = (int)a.size();

    for (int x : a) {
        info.sum += x;
        info.squareSum += 1LL * x * x;
    }

    return info;
}

bool checkSorted(const vector<int>& a) {
    for (int i = 1; i < (int)a.size(); i++) {
        if (a[i] < a[i - 1]) {
            return false;
        }
    }

    return true;
}

bool checkComposition(const vector<int>& original,
    const vector<int>& sorted) {

    ArrayInfo first = getArrayInfo(original);
    ArrayInfo second = getArrayInfo(sorted);

    if (first.count != second.count) return false;
    if (first.sum != second.sum) return false;
    if (first.squareSum != second.squareSum) return false;

    vector<int> a = original;
    vector<int> b = sorted;

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    return a == b;
}

bool checkResult(const vector<int>& original,
    const vector<int>& sorted) {

    return checkSorted(sorted) &&
        checkComposition(original, sorted);
}

int lomutoPartition(vector<int>& a,
    int low,
    int high,
    Statistics& s) {

    int pivot = a[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {

        s.comparisons++;

        if (a[j] <= pivot) {
            i++;

            exchange(a[i], a[j], s);
        }
    }

    exchange(a[i + 1], a[high], s);

    return i + 1;
}

int hoarePartition(vector<int>& a,
    int low,
    int high,
    Statistics& s) {

    int pivot = a[low + (high - low) / 2];

    int i = low - 1;
    int j = high + 1;

    while (true) {

        do {
            i++;
            s.comparisons++;
        } while (a[i] < pivot);

        do {
            j--;
            s.comparisons++;
        } while (a[j] > pivot);

        if (i >= j) {
            return j;
        }

        exchange(a[i], a[j], s);
    }
}

void quickSortLomuto(vector<int>& a,
    int low,
    int high,
    Statistics& s,
    int depth = 0) {

    if (low >= high) return;

    s.calls++;

    int callNumber = s.calls;

    int pivot = a[high];

    if (tracing) {
        cout << "\n[Lomuto] Call #" << callNumber << endl;
        cout << "Depth: " << depth << endl;
        cout << "Range: " << low << " - " << high << endl;
        cout << "Pivot: " << pivot << endl;
        cout << "Before partition: ";

        printPart(a, low, high);
        cout << endl;
    }

    int p = lomutoPartition(a, low, high, s);

    if (tracing) {
        cout << "Pivot position: " << p << endl;

        cout << "After partition: ";
        printPart(a, low, high);
        cout << endl;

        cout << "Left range: "
            << low << " - " << p - 1 << endl;

        cout << "Right range: "
            << p + 1 << " - " << high << endl;
    }

    quickSortLomuto(a, low, p - 1, s, depth + 1);

    quickSortLomuto(a, p + 1, high, s, depth + 1);
}

void quickSortHoare(vector<int>& a,
    int low,
    int high,
    Statistics& s,
    int depth = 0) {

    if (low >= high) return;

    s.calls++;

    int callNumber = s.calls;

    int pivot = a[low + (high - low) / 2];

    if (tracing) {
        cout << "\n[Hoare] Call #" << callNumber << endl;
        cout << "Depth: " << depth << endl;
        cout << "Range: " << low << " - " << high << endl;
        cout << "Pivot: " << pivot << endl;

        cout << "Before partition: ";
        printPart(a, low, high);
        cout << endl;
    }

    int p = hoarePartition(a, low, high, s);

    if (tracing) {
        cout << "Returned boundary: " << p << endl;

        cout << "After partition: ";
        printPart(a, low, high);
        cout << endl;

        cout << "Left range: "
            << low << " - " << p << endl;

        cout << "Right range: "
            << p + 1 << " - " << high << endl;
    }

    quickSortHoare(a, low, p, s, depth + 1);

    quickSortHoare(a, p + 1, high, s, depth + 1);
}

void printStatistics(const Statistics& s) {

    cout << "Comparisons: " << s.comparisons << endl;
    cout << "Swaps: " << s.swaps << endl;
    cout << "Recursive calls: " << s.calls << endl;
}

void resetStatistics(Statistics& s) {
    s.comparisons = 0;
    s.swaps = 0;
    s.calls = 0;
}

void testLomutoPartition() {

    cout << "\n========== LOMUTO PARTITION TESTS =========="
        << endl;

    vector<vector<int>> tests = {
        {5, 8, 9, 2, 1},
        {1, 3, 4, 5, 9},
        {8, 2, 5, 3, 6},
        {4, 2, 4, 1, 4}
    };

    for (int k = 0; k < (int)tests.size(); k++) {

        vector<int> a = tests[k];

        Statistics s;

        cout << "\nTest #" << k + 1 << endl;

        cout << "Original array: ";
        printArray(a);

        int pivot = a[a.size() - 1];

        int p = lomutoPartition(
            a, 0, (int)a.size() - 1, s
        );

        cout << "Pivot value: " << pivot << endl;
        cout << "Pivot position: " << p << endl;

        cout << "Array after partition: ";
        printArray(a);

        cout << "Comparisons: " << s.comparisons << endl;
        cout << "Swaps: " << s.swaps << endl;
    }
}

void testHoarePartition() {

    cout << "\n========== HOARE PARTITION TESTS =========="
        << endl;

    vector<vector<int>> tests = {
        {1, 3, 5, 7, 9},
        {9, 7, 5, 3, 1},
        {8, 2, 5, 3, 6},
        {4, 4, 4, 4, 4}
    };

    for (int k = 0; k < (int)tests.size(); k++) {

        vector<int> a = tests[k];

        Statistics s;

        cout << "\nTest #" << k + 1 << endl;

        cout << "Original array: ";
        printArray(a);

        int pivot = a[a.size() / 2];

        cout << "Pivot value: " << pivot << endl;

        int p = hoarePartition(
            a, 0, (int)a.size() - 1, s
        );

        cout << "Returned boundary: " << p << endl;

        cout << "Array after partition: ";
        printArray(a);

        cout << "Left index boundary: " << p << endl;
        cout << "Right index boundary: " << p + 1 << endl;

        cout << "Comparisons: " << s.comparisons << endl;
        cout << "Swaps: " << s.swaps << endl;
    }
}

bool runAutomaticTests() {

    cout << "\n========== AUTOMATIC TESTS =========="
        << endl;

    vector<vector<int>> tests = {
        {5, 2, 9, 1, 7, 3},
        {1, 2, 3, 4, 5, 6},
        {6, 5, 4, 3, 2, 1},
        {4, 4, 4, 4, 4},
        {8, 2, 8, 1, 5, 2, 9},
        {-5, 0, 10, -2, 7},
        {42},
        {2, 1}
    };

    bool allPassed = true;

    for (int i = 0; i < (int)tests.size(); i++) {

        vector<int> original = tests[i];

        vector<int> lomuto = original;
        vector<int> hoare = original;

        Statistics sLomuto;
        Statistics sHoare;

        tracing = false;

        if (!lomuto.empty()) {
            quickSortLomuto(
                lomuto, 0,
                (int)lomuto.size() - 1,
                sLomuto
            );

            quickSortHoare(
                hoare, 0,
                (int)hoare.size() - 1,
                sHoare
            );
        }

        bool resultLomuto =
            checkResult(original, lomuto);

        bool resultHoare =
            checkResult(original, hoare);

        cout << "\nTest #" << i + 1 << endl;

        cout << "Lomuto: "
            << (resultLomuto ? "PASSED" : "FAILED")
            << endl;

        cout << "Hoare: "
            << (resultHoare ? "PASSED" : "FAILED")
            << endl;

        if (!resultLomuto || !resultHoare) {
            allPassed = false;
        }
    }

    cout << "\nAutomatic tests result: "
        << (allPassed ? "ALL PASSED" : "FAILED")
        << endl;

    return allPassed;
}

void runExperiment(const vector<int>& original) {

    vector<int> a = original;
    vector<int> b = original;

    Statistics lomutoStats;
    Statistics hoareStats;

    tracing = original.size() <= TRACE_LIMIT;

    cout << "\n========== LOMUTO QUICKSORT =========="
        << endl;

    cout << "Original array: ";
    printArray(original);

    if (tracing) {
        cout << "\nTracing enabled." << endl;
    }
    else {
        cout << "\nTracing disabled (array > 30 elements)."
            << endl;
    }

    if (!a.empty()) {
        quickSortLomuto(
            a, 0, (int)a.size() - 1,
            lomutoStats
        );
    }

    cout << "\nSorted array: ";
    printArray(a);

    cout << "\nLomuto statistics:" << endl;
    printStatistics(lomutoStats);

    bool lomutoOK = checkResult(original, a);

    cout << "Lomuto verification: "
        << (lomutoOK ? "PASSED" : "FAILED")
        << endl;

    cout << "\n========== HOARE QUICKSORT =========="
        << endl;

    if (!b.empty()) {
        quickSortHoare(
            b, 0, (int)b.size() - 1,
            hoareStats
        );
    }

    cout << "\nSorted array: ";
    printArray(b);

    cout << "\nHoare statistics:" << endl;
    printStatistics(hoareStats);

    bool hoareOK = checkResult(original, b);

    cout << "Hoare verification: "
        << (hoareOK ? "PASSED" : "FAILED")
        << endl;

    cout << "\n========== COMPARISON =========="
        << endl;

    cout << "Lomuto comparisons: "
        << lomutoStats.comparisons << endl;

    cout << "Hoare comparisons: "
        << hoareStats.comparisons << endl;

    cout << "Lomuto swaps: "
        << lomutoStats.swaps << endl;

    cout << "Hoare swaps: "
        << hoareStats.swaps << endl;

    cout << "\nFinal result: "
        << (lomutoOK && hoareOK ? "BOTH PASSED" : "ERROR")
        << endl;
}

int main() {

    cout << "======================================" << endl;
    cout << "QUICKSORT ALGORITHM COMPARISON" << endl;
    cout << "Lomuto vs Hoare partition schemes" << endl;
    cout << "======================================" << endl;

    testLomutoPartition();

    testHoarePartition();

    bool testsPassed = runAutomaticTests();

    if (!testsPassed) {
        cout << "\nExperiments stopped: tests failed."
            << endl;
        return 1;
    }

    cout << "\n========== ARRAY INPUT =========="
        << endl;

    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    if (n < 1) {
        cout << "Invalid array size." << endl;
        return 1;
    }

    vector<int> original(n);

    cout << "Enter array elements:" << endl;

    for (int i = 0; i < n; i++) {
        cin >> original[i];
    }

    cout << "\nOriginal array: ";
    printArray(original);

    runExperiment(original);

    cout << "\nProgram finished successfully." << endl;

    return 0;
}
