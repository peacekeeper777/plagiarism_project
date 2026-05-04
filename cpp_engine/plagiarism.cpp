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
    text.erase(remove_if(text.begin(), text.end(), ::ispunct), text.end());
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

double jaccardSimilarity(const vector<string>& a, const vector<string>& b) {
    unordered_set<string> set1(a.begin(), a.end());
    unordered_set<string> set2(b.begin(), b.end());
    int intersectionCount = 0;
    for (const string& word : set1) {
        if (set2.count(word)) {
            intersectionCount++;
        }
    }
    int unionCount = set1.size() + set2.size() - intersectionCount;
    if (unionCount == 0) return 0.0;
    return (double)intersectionCount / unionCount;
}

int computeLCSLength(const vector<string>& seq1, const vector<string>& seq2) {
    int m = seq1.size();
    int n = seq2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (seq1[i - 1] == seq2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

vector<string> extractNGrams(const vector<string>& tokens, int n) {
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

vector<string> findCommonPhrases(const vector<string>& a, const vector<string>& b) {
    unordered_set<string> set1(a.begin(), a.end());
    unordered_set<string> set2(b.begin(), b.end());
    vector<string> common;
    for (const string& phrase : set1) {
        if (set2.count(phrase)) {
            common.push_back(phrase);
        }
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
    if (t1.empty() || t2.empty()) {
        cout << "{\"similarity\":0.0,\"patterns\":[]}" << endl;
        return 0;
    }
    double jaccardSim = jaccardSimilarity(t1, t2);
    int lcsLength = computeLCSLength(t1, t2);
    double lcsSim = (double)(2.0 * lcsLength) / (t1.size() + t2.size());
    double finalSim = (0.5 * jaccardSim + 0.5 * lcsSim) * 100.0;
    vector<string> n1 = extractNGrams(t1, 3);
    vector<string> n2 = extractNGrams(t2, 3);
    vector<string> patterns = findCommonPhrases(n1, n2);
    string lcsSummary = "LCS Match: " + to_string(lcsLength) + " sequential words found in common";
    patterns.insert(patterns.begin(), lcsSummary);
    cout << "{";
    cout << "\"similarity\":" << finalSim << ",";
    cout << "\"patterns\":[";
    for (size_t i = 0; i < patterns.size(); i++) {
        cout << "\"" << patterns[i] << "\"";
        if (i != patterns.size() - 1) cout << ",";
    }
    cout << "]}";
    return 0;
}