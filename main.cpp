#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

struct tower {
    double items[10];
    size_t height;
    size_t number;

    tower() {
        memset(items, 1.0, 10);
        height = 0;
    }

    void add(double value) {
        items[height] = value;
        height++;
    };

    void print() const {
        //std::copy(items, items + height, std::ostream_iterator<double>(cout, " "));
        for (size_t i = 0; i < height; i++) {
            cout << items[i] << " ";
        }
        cout << endl;
    };

    void normalize() {
        while (height > 1 && pow(items[height - 2], items[height - 1]) < 1e50) {
            items[height - 2] = pow(items[height - 2], items[height - 1]);
            height--;
        }
    };

    double conv2() {
        return items[1] * log10(items[0]);
    };

    double conv3(size_t lvl) {
        return items[lvl + 2] * log10(items[lvl + 1]) + log10(log10(items[lvl]));
    };
};

void printTowers(const vector<tower>& towers) {
    for (size_t i = 0; i < towers.size(); i++) {
        towers[i].print();
    }
}

size_t max(const tower& t1, const tower& t2) {
    if (t1.height > t2.height) {
        return t1.height;
    }
    else {
        return t2.height;
    }
};

bool compare(tower& t1, tower& t2) {
    size_t maxt = max(t1, t2);

    if (maxt == 1) {
        return t1.items[0] < t2.items[0];
    }
    else if (maxt == 2) {
        return t1.conv2() < t2.conv2();
    }
    else if (maxt == 3) {
        return t1.conv3(0) < t2.conv3(0);
    }
    else if (maxt > 3) {
        size_t lvl = maxt - 3;
        if (fabs(t1.conv3(lvl) - fabs(t2.conv3(lvl))) > 1e-14) {
            return t1.conv3(lvl) < t2.conv3(lvl);
        }
        else {
            while (lvl >= 0) {
                if (t1.items[lvl] != t2.items[lvl]) {
                    return t1.items[lvl] < t2.items[lvl];
                }
                lvl--;
            }
        }
    }

    return t1.items[0] < t2.items[0];

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
            matrix[i].add(value);
        }
        matrix[i].normalize();
    }


    printTowers(matrix);

    //printf("sorting:\n");
    sort(matrix.begin(), matrix.end(), compare);

    printf("\nsorted matrix:\n");
    printTowers(matrix);
    printf("\n");

    ofstream out("output.txt");
    for (size_t i = 0; i < matrix.size(); i++) {
        out << matrix[i].number << " ";
        cout << matrix[i].number << " ";
    }
    out << endl;
    cout << endl;

    out.close();

    return 0;
}
