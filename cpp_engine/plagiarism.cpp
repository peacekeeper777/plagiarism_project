#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
using namespace std;
string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return "";
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
void preprocess(string& text) {
    transform(text.begin(), text.end(), text.begin(), ::tolower);
}
vector<string> tokenize(const string& text) {
    vector<string> tokens;
    istringstream stream(text);
    string word;
    while (stream >> word) {
        tokens.push_back(word);
    }
    return tokens;
}
double jaccard(const vector<string>& a, const vector<string>& b) {
    unordered_set<string> s1(a.begin(), a.end());
    unordered_set<string> s2(b.begin(), b.end());
    int intersection = 0;
    for (const auto& w : s1) {
        if (s2.count(w)) intersection++;
    }
    int uni = s1.size() + s2.size() - intersection;
    return (uni == 0) ? 0.0 : (double)intersection / uni;
}
vector<string> ngrams(const vector<string>& tokens, int n) {
    vector<string> result;
    if (tokens.size() < n) return result;
    for (size_t i = 0; i <= tokens.size() - n; i++) {
        string gram = tokens[i];
        for (int j = 1; j < n; j++) {
            gram += " " + tokens[i + j];
        }
        result.push_back(gram);
    }
    return result;
}
vector<string> commonPatterns(const vector<string>& a, const vector<string>& b) {
    unordered_set<string> s1(a.begin(), a.end());
    unordered_set<string> s2(b.begin(), b.end());
    vector<string> common;
    for (const auto& w : s1) {
        if (s2.count(w)) common.push_back(w);
    }
    return common;
}
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "{\"error\":\"Provide two files\"}";
        return 1;
    }
    string text1 = readFile(argv[1]);
    string text2 = readFile(argv[2]);
    preprocess(text1);
    preprocess(text2);
    vector<string> t1 = tokenize(text1);
    vector<string> t2 = tokenize(text2);
    double sim = jaccard(t1, t2) * 100.0;
    vector<string> n1 = ngrams(t1, 2);
    vector<string> n2 = ngrams(t2, 2);
    vector<string> patterns = commonPatterns(n1, n2);
    cout << "{";
    cout << "\"similarity\":" << sim << ",";
    cout << "\"patterns\":[";
    for (size_t i = 0; i < patterns.size(); i++) {
        cout << "\"" << patterns[i] << "\"";
        if (i != patterns.size() - 1) cout << ",";
    }
    cout << "]}";
    return 0;
}