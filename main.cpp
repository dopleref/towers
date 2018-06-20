#include <stdio.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

struct tower {
    vector<double> items;
    size_t number;

    double operator[] (size_t index) {
        return items[index];
    };

    void print() const {
        std::copy(items.begin(), items.end(), std::ostream_iterator<double>(cout, " "));
        cout << endl;
    }

    void normalize() {
        while (items.size() > 1 &&
               (pow(items[items.size() - 2], items[items.size() - 1]) < 2e+9)) {
            items[items.size() - 2] = pow(items[items.size() - 2], items[items.size() - 1]);
            items.pop_back();
        }
    };

    void normalize2() {
        while (items.size() > 1 &&
               log10(items[items.size() - 2]) * items[items.size() -1] < 50) {

            items[items.size() - 2] = pow(items[items.size() - 2], items[items.size() - 1]);
            items.pop_back();
        }
    };

    void fillOnes(size_t length) {
        size_t n = length - items.size();
        for (size_t i = 0; i < n; i++) {
            items.push_back(1.0);
        }
    }

    double log2L() {
        return log10(items[0]) * items[1];
    };



    double log3L(size_t level) {
        /*
        if (items.size() - level < 3) {
            printf("log3L error");
            return 1.0;
        }
        */
        return items[level + 2]*log10(items[level + 1]) + log10(log10(items[level]));
    };

    double log3L() {
        return log3L(0);
    };

    double conv4L(size_t level) {
        if (items.size() - level < 4) {
            printf("conv4L error");
            return 1.0;
        }
        return pow(items[level], log3L(level + 1));
    };
};

void printTowers(const vector<tower>& towers) {
    for (size_t i = 0; i < towers.size(); i++) {
        towers[i].print();
    }
}

size_t maxSize(const vector<tower>& towers) {
    size_t max = 0;
    //printf("maxSize():\n");
    for (size_t i = 0; i < towers.size(); i++) {
        if (max < towers[i].items.size()) {
            max = towers[i].items.size();
        }
        //printf("max = %d\n", max);
    }
    return max;
}

bool compare(tower& t1, tower& t2) {
    size_t n = t1.items.size();

    if (n == 1) {
        return t1.items[0] < t2.items[0];
    }
    else if (n == 2) {
        return t1.log2L() - t2.log2L() < 0;
    }
    else if (n == 3) {
        return t1.log3L() - t2.log3L() < 0;
    }
    else if (n == 4) {
        return t1.conv4L(0) - t2.conv4L(0) < 0;
    }
    else {
        size_t l = t1.items.size() - 4;
        if (fabs(t1.conv4L(l) - t2.conv4L(l)) > 1e-14) {
            return t1.conv4L(l) - t2.conv4L(l);
        }
        else {
            for (size_t i = l - 1; i >= 0; i--) {
                if (t1.items[i] != t2.items[i]) {
                    return t1.items[i] < t2.items[i];
                }
            }
        }
    }

    return t1.items[0] < t2.items[0];
};

bool compare2(tower& t1, tower& t2) {
    size_t level = (t1.items.size() > t2.items.size()) ?
                    t1.items.size() : t2.items.size();
    level -= 3;
    if (level < 0) {
        level = 0;
    }
    if (t1.items.size() == t2.items.size()) {
        for (size_t i = t1.items.size() -1; i >= 0; i--) {
            if (fabs(t1.items[i] - t2.items[i]) > 1e-14) {
                if (i < level) {
                    return t1.items[i] < t2.items[i];
                }
                break;
            }
        }
    }
    return t1.log3L(level) < t2.log3L(level);
};

int main() {
    //system("chcp 65001");
    ifstream in("input.txt");
    size_t n;
    size_t m;
    double value;
    in >> n;

    vector<tower> matrix(n);
    for (size_t i = 0; i < n; i++) {
        in >> m;
        matrix[i].number = i + 1;
        for (size_t j = 0; j < m + 1; j++) {
            in >> value;
            matrix[i].items.push_back(value);
        }
        matrix[i].normalize2();
    }

    //printTowers(matrix);

    /*
    for (size_t i = 0; i < n; i++) {
        matrix[i].normalize2();
    };
    */

    //printf("\nnormalize matrix: \n");
    //printTowers(matrix);

    size_t maxsize = maxSize(matrix);
    //printf("maxsize = %d\n", maxsize);
    for (size_t i = 0; i < n; i++) {
        matrix[i].fillOnes(maxsize);
    }

    //printf("\nfill matrix: \n");
    //printTowers(matrix);

    sort(matrix.begin(), matrix.end(), compare2);

    //printf("\nsorted matrix: \n");
    //printTowers(matrix);

    /*
    printf("\nresult: \n");
    for (size_t i = 0; i < matrix.size(); i++) {
        cout << matrix[i].number << " ";
    }
    cout << endl;
    */

    /*
    sort(matrix.begin(), matrix.end(), compare);
    printf("\nsorted matrix: \n");
    printTowers(matrix);

    printf("\nresult: \n");
    for (size_t i = 0; i < matrix.size(); i++) {
        cout << matrix[i].number << " ";
    }
    cout << endl;
    */

    ofstream out("output.txt");
    for (size_t i = 0; i < matrix.size(); i++) {
        out << matrix[i].number << " ";
    }
    out << endl;

    return 0;
}
