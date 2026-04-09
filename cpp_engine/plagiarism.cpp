#include <bits/stdc++.h>
using namespace std;

// Read file
string readFile(string filename) {
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Convert to lowercase
string preprocess(string text) {
    transform(text.begin(), text.end(), text.begin(), ::tolower);
    return text;
}

// Tokenize words
vector<string> tokenize(string text) {
    vector<string> tokens;
    string word = "";

    for(char c : text) {
        if(isspace(c)) {
            if(word != "") tokens.push_back(word);
            word = "";
        } else {
            word += c;
        }
    }
    if(word != "") tokens.push_back(word);
    return tokens;
}

// Jaccard similarity
double jaccard(vector<string> a, vector<string> b) {
    set<string> s1(a.begin(), a.end());
    set<string> s2(b.begin(), b.end());

    int intersection = 0;
    for(auto &w : s1) {
        if(s2.count(w)) intersection++;
    }

    int uni = s1.size() + s2.size() - intersection;
    return (double)intersection / uni;
}

// Generate n-grams
vector<string> ngrams(vector<string> tokens, int n) {
    vector<string> result;

    for(int i = 0; i <= tokens.size() - n; i++) {
        string gram = "";
        for(int j = 0; j < n; j++) {
            gram += tokens[i + j] + " ";
        }
        result.push_back(gram);
    }
    return result;
}

// Find common patterns
vector<string> commonPatterns(vector<string> a, vector<string> b) {
    set<string> s1(a.begin(), a.end());
    set<string> s2(b.begin(), b.end());

    vector<string> common;

    for(auto &w : s1) {
        if(s2.count(w)) common.push_back(w);
    }
    return common;
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        cout << "{\"error\":\"Provide two files\"}";
        return 0;
    }

    string text1 = preprocess(readFile(argv[1]));
    string text2 = preprocess(readFile(argv[2]));

    vector<string> t1 = tokenize(text1);
    vector<string> t2 = tokenize(text2);

    double sim = jaccard(t1, t2) * 100;

    vector<string> n1 = ngrams(t1, 2);
    vector<string> n2 = ngrams(t2, 2);

    vector<string> patterns = commonPatterns(n1, n2);

    // Output JSON
    cout << "{";
    cout << "\"similarity\":" << sim << ",";
    cout << "\"patterns\":[";

    for(int i = 0; i < patterns.size(); i++) {
        cout << "\"" << patterns[i] << "\"";
        if(i != patterns.size()-1) cout << ",";
    }

    cout << "]}";

    return 0;
}