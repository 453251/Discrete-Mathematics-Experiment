#include<iostream>
#include<format>
#include<set>
#include<vector>
#include<unordered_map>
#include<algorithm>
using namespace std;

using Matrix = vector<vector<int>>;

set<char> U;
set<char> A;
set<char> B;
Matrix R;

vector<char> convert_A;

char temp;
int temp_int;
set<char> temp_set;
int i, j;

class DisjointSet {
private:
    unordered_map<int, int> parent;  // 存储每个元素的父节点
    unordered_map<int, int> rank;    // 存储每个集合的秩（用于优化）

public:
    // 初始化并查集，每个元素都是一个独立的集合
    void makeSet(const vector<int>& elements);

    // 查找元素所属的集合（根节点）
    int find(int element);

    // 合并两个集合
    void unionSets(int element1, int element2);
};

void DisjointSet::makeSet(const vector<int>& elements) {
    for (int element : elements) {
        parent[element] = element;
        rank[element] = 0;
    }
}

int DisjointSet::find(int element) { // NOLINT(*-no-recursion)
    if (parent[element] != element) {
        parent[element] = find(parent[element]);  // 路径压缩
    }
    return parent[element];
}

void DisjointSet::unionSets(int element1, int element2) {
    int root1 = find(element1);
    int root2 = find(element2);

    if (root1 != root2) {
        if (rank[root1] < rank[root2]) {
            parent[root1] = root2;
        }
        else if (rank[root1] > rank[root2]) {
            parent[root2] = root1;
        }
        else {
            parent[root1] = root2;
            rank[root2]++;
        }
    }
}

vector<vector<int>> determinePartition(const vector<vector<int>>& R1, const vector<int>& elements) {
    int n = (int)(R1.size());

    // 初始化并查集
    DisjointSet ds;
    ds.makeSet(elements);

    // 构建并查集
    for (int i1 = 0; i1 < n; ++i1) {
        for (int j1 = i1 + 1; j1 < n; ++j1) {
            if (R1[i1][j1] == 1) {
                ds.unionSets(elements[i1], elements[j1]);
            }
        }
    }

    // 构建划分
    unordered_map<int, vector<int>> partition;
    for (int element : elements) {
        int root = ds.find(element);
        partition[root].push_back(element);
    }

    // 转换为二维数组形式
    vector<vector<int>> result;
    for (const auto& pair : partition) {
        result.push_back(pair.second);
    }

    return result;
}

vector<vector<int>> pow(vector<vector<int>>& target, int times) {
    vector<vector<int>> result = target;
    while (--times) {
        vector<vector<int>> med = vector<vector<int>>(A.size(), vector<int>(A.size(), 0));
        for (int i2 = 0; i2 < target.size(); i2++)
            for (int j2 = 0; j2 < target.size(); j2++)
                for (int k = 0; k < target.size(); k++)
                {
                    med[i2][j2] += (result[i2][k] * target[k][j2]);
                    med[i2][j2] = (med[i2][j2] > 0);
                }
        result = med;
    }

    return result;
}

vector<vector<int>> transitiveClosure(const Matrix& target) {
    int n = (int)(target.size());

    // 创建传递闭包矩阵并初始化为关系矩阵 R
    Matrix closure = target;

    // 使用 Warshall 算法计算传递闭包
    for (int k = 0; k < n; ++k) {
        for (int i3 = 0; i3 < n; ++i3) {
            for (int j3 = 0; j3 < n; ++j3) {
                closure[i3][j3] = closure[i3][j3] || (closure[i3][k] && closure[k][j3]);
            }
        }
    }

    return closure;
}

void print(vector<vector<int>> PR) {
    int stop = 0;
    for (i = 0; i < A.size(); i++) {
        for (j = 0; j < A.size(); j++) {
            if (PR[i][j]) {
                if (stop == 0) {
                    stop = 1;
                }
                else {
                    cout << ',';
                }
                cout << format("<{0},{1}>", convert_A[i], convert_A[j]);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);

    {
        cout << "请输入全集U:" << endl;
        while ((temp = (char)(getchar())) != '\n') {
            if (temp != ' ')
                U.insert(temp);
        }

        cout << "请输入集合A:" << endl;
        while ((temp = (char)(getchar())) != '\n') {
            if (temp != ' ')
                A.insert(temp);
        }

        cout << "请输入集合B:" << endl;
        while ((temp = (char)(getchar())) != '\n') {
            if (temp != ' ')
                B.insert(temp);
        }

        cout << "请输入A上的关系矩阵R:" << endl;
        R = Matrix(A.size(), vector<int>(A.size(), 0));

        cout << "  ";
        for (auto str : A) {
            cout << str << ' ';
        }
        cout << endl;

        int k = 0;
        for (auto str : A) {
            cout << str << "|";
            for (j = 0; j < A.size(); j++) {
                cin >> temp_int;
                R[k][j] = temp_int;
            }
            k++;
        }
    }//输入部分

    cout << "全集U:{";
    i = 0;
    for (auto& elem : U) {
        cout << elem;
        if (i != U.size() - 1)
            cout << ',';
        i++;
    }
    cout << "}" << endl;

    cout << "集合A:{";
    i = 0;
    for (auto& elem : A) {
        cout << elem;
        if (i != A.size() - 1)
            cout << ',';
        i++;
    }
    cout << "} ";

    cout << "集合B:{";
    i = 0;
    for (auto& elem : B) {
        cout << elem;
        if (i != B.size() - 1)
            cout << ',';
        i++;
    }
    cout << "}\n" << endl;



    temp_set.clear();
    cout << "A∪B: ";
    set_union(A.begin(), A.end(), B.begin(), B.end(), inserter(temp_set, temp_set.begin()));
    cout << "{";
    i = 0;
    for (auto& elem : temp_set) {
        cout << elem;
        if (i != temp_set.size() - 1)
            cout << ',';
        i++;
    }
    cout << "}";

    temp_set.clear();
    cout << endl << "A∩B: ";
    set_intersection(A.begin(), A.end(), B.begin(), B.end(), inserter(temp_set, temp_set.begin()));
    cout << "{";
    i = 0;
    for (auto& elem : temp_set) {
        cout << elem;
        if (i != temp_set.size() - 1)
            cout << ',';
        i++;
    }
    cout << "}";

    temp_set.clear();
    cout << endl << "A-B: ";
    set_difference(A.begin(), A.end(), B.begin(), B.end(), inserter(temp_set, temp_set.begin()));
    cout << "{";
    i = 0;
    for (auto& elem : temp_set) {
        cout << elem;
        if (i != temp_set.size() - 1)
            cout << ',';
        i++;
    }
    cout << "}";

    temp_set.clear();
    cout << endl << "~ A: ";
    set_difference(U.begin(), U.end(), A.begin(), A.end(), inserter(temp_set, temp_set.begin()));
    cout << "{";
    i = 0;
    for (auto& elem : temp_set) {
        cout << elem;
        if (i != temp_set.size() - 1)
            cout << ',';
        i++;
    }
    cout << "}";

    temp_set.clear();
    cout << endl << "A⊕B: ";
    set_symmetric_difference(A.begin(), A.end(), B.begin(), B.end(), inserter(temp_set, temp_set.begin()));
    cout << "{";
    i = 0;
    for (auto& elem : temp_set) {
        cout << elem;
        if (i != temp_set.size() - 1)
            cout << ',';
        i++;
    }
    cout << "}\n" << endl;
    //question 1


    for (auto& elem : A) convert_A.push_back(elem);

    cout << "R  :{";
    print(R);
    cout << '}' << endl;

    vector<vector<int>> r2 = pow(R, 2);
    cout << "R^2:{";
    print(r2);
    cout << '}' << endl;

    vector<vector<int>> r3 = pow(R, 3);
    cout << "R^3:{";
    print(r3);
    cout << '}' << endl;
    //question 2


    //question 3

    vector<vector<int>> rR = R;
    for (i = 0; i < A.size(); i++)
        rR[i][i] = 1;
    cout << "r(R):{";
    print(rR);
    cout << '}' << endl;

    vector<vector<int>> sR{ A.size(), vector<int>(A.size(), 0) };
    for (i = 0; i < A.size(); i++)
        for (j = 0; j < A.size(); j++) {
            sR[i][j] = ((R[i][j] + R[j][i]) > 0);
        }

    cout << "s(R):{";
    print(sR);
    cout << '}' << endl;

    vector<vector<int>> tR = transitiveClosure(R);
    cout << "t(R):{";
    print(tR);
    cout << '}' << endl;




    //question 4
    int reverse = -1; //自反标记
    int symmetric = -1; //对称标记
    int transitive = 0; //传递标记
    //自反
    int flag = 0;

    int count = 0;
    for (i = 0; i < A.size(); i++) {
        if (R[i][i] == 1) count++;
    }
    if (count == 0) {
        cout << "R具有反自反性" << endl;
        reverse = 0;
        flag = 1;
    }
    else if (count == A.size()) {
        cout << "R具有自反性" << endl;
        reverse = 1;
        flag = 1;
    }

    //对称
    int total;
    total = count = 0;
    for (i = 0; i < A.size(); i++) {
        for (int j3 = i + 1; j3 < A.size(); j3++) {
            if (R[i][j3] == 1 && R[j3][i] == 1) {
                count++;
                total++;
            }
            else if (R[i][j3] == 1 || R[j3][i] == 1) {
                total++;
            }

        }
    }
    if (total == count) {
        cout << "R具有对称性" << endl;
        symmetric = 1;
        flag = 1;
    }
    else if (count == 0) {
        cout << "R具有反对称性" << endl;
        symmetric = 0;
        flag = 1;
    }

    //传递性
    for (i = 0; i < A.size(); i++) {
        for (j = 0; j < A.size(); j++) {
            //传递闭包判断
            if (R[i][j] != tR[i][j]) {
                goto out;
            }
        }
    }
    cout << "R具有传递性" << endl;
    transitive = 1;
    flag = 1;
out:
    if (!flag)
        cout << "R不具有性质" << endl;


    //question 5
    if (reverse == 1 && symmetric == 1 && transitive == 1) {
        cout << "R为等价关系" << endl;

        //question 6
        // 求等价关系确定的划分

        vector<int> target;
        for (auto& elem : convert_A)
            target.push_back(elem);
        vector<vector<int>> partition = determinePartition(R, target);

        // 输出划分
        cout << "划分:" << endl;

        for (const auto& subset : partition) {
            cout << '{';

            for (i = 0; i < subset.size(); i++) {
                cout << static_cast<char>(subset[i]);
                if (i != subset.size() - 1)
                    cout << ',';
            }
            cout << '}' << endl;
        }

    }
    else if (reverse == 1 && symmetric == 1 && transitive == 0) //IDE不能检测goto 实际上条件可变
        cout << "R为相容关系" << endl;
    else if (reverse == 1 && symmetric == 0 && transitive == 1)
        cout << "R为偏序关系" << endl;
    else
        cout << "R没有特定关系" << endl;
    //输出部分
    system("pause");
    return 0;
}