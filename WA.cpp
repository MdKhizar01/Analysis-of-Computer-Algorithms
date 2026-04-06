/***********************************************************
  Group#  : 1
  Member  : Student 1 and Student 2
  Synopsis: Read from city.name and flight.txt
            Store the graph in an adjacency list and 
            answer routing queries efficiently
***********************************************************/

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <climits>

using namespace std;

map<string, int> cityToIndex;
vector<string> indexToCity;
vector<string> displayCity;
vector<vector<int>> adj;

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos || end == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Helper: Breadth-first search (returns path or empty if no path or exceeds maxConnections)
vector<int> bfs(int start, int end, int maxConnections = INT_MAX) {
    vector<int> prev(adj.size(), -1);
    vector<int> dist(adj.size(), INT_MAX);
    queue<int> q;
    q.push(start);
    dist[start] = 0;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        if (dist[node] >= maxConnections) continue;
        for (int nbr : adj[node]) {
            if (dist[nbr] == INT_MAX) {
                dist[nbr] = dist[node] + 1;
                prev[nbr] = node;
                q.push(nbr);
            }
        }
    }
    if (dist[end] == INT_MAX || dist[end] > maxConnections) return {};
    vector<int> path;
    for (int at = end; at != -1; at = prev[at]) path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}

// Utility: print a sequence of city indices
void printRoute(const vector<int>& path) {
    for (size_t i = 0; i < path.size(); ++i) {
        cout << displayCity[path[i]];
        if (i + 1 < path.size()) cout << " to ";
    }
    cout << '\n';
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: routeSearch <query_number> [arguments...]\n";
        return 1;
    }

    // Load cities
    ifstream cf("city.name");
    if (!cf) { cout << "Could not open city.name\n"; return 1; }
    vector<string> cities; string line;
    while (getline(cf, line)) if (!line.empty()) cities.push_back(trim(line));
    cf.close();
    int n = cities.size();
    indexToCity = cities; adj.assign(n, {});
    displayCity.resize(n);

    // Build map and display normalization
    for (int i = 0; i < n; ++i) {
        string orig = cities[i], ascii;
        for (size_t k = 0; k < orig.size(); ++k) {
            unsigned char c = orig[k];
            if (c == 0xC3 && k + 1 < orig.size() && (unsigned char)orig[k+1] == 0xBC) {
                ascii += 'u'; k++;
            } else ascii += orig[k];
        }
        cityToIndex[orig] = i;
        if (ascii != orig) cityToIndex[ascii] = i;
        displayCity[i] = ascii;
    }

    // Load flights
    ifstream ff("flight.txt");
    if (!ff) { cout << "Could not open flight.txt\n"; return 1; }
    int from = -1;
    while (getline(ff, line)) {
        if (line.rfind("From:", 0) == 0) {
            string c = trim(line.substr(5));
            from = cityToIndex.count(c) ? cityToIndex[c] : -1;
        } else if (line.rfind("To  :", 0) == 0 || line.rfind("       ", 0) == 0) {
            string c = trim(line.substr(6));
            if (from != -1 && cityToIndex.count(c)) {
                adj[from].push_back(cityToIndex[c]);
            }
        }
    }
    ff.close();

    int query = stoi(argv[1]);

    // Query 1: shortest within x connections
    if (query == 1 && argc == 5) {
        string A = argv[2], B = argv[3]; int x = stoi(argv[4]);
        if (!cityToIndex.count(A) || !cityToIndex.count(B)) { cout << "City not found.\n"; return 1; }
        auto path = bfs(cityToIndex[A], cityToIndex[B], x);
        if (path.empty()) cout << "No such route with " << x << " or fewer connections.\n";
        else {
            printRoute(path);
            cout << "total connection: " << (path.size() - 1) << "\n";
        }
    }

    // Query 2: through B and C to D
    else if (query == 2 && argc == 9 && string(argv[3]) == "through" && string(argv[5]) == "and" && string(argv[7]) == "to") {
        string A = argv[2], B = argv[4], C = argv[6], D = argv[8];
        if (!cityToIndex.count(A) || !cityToIndex.count(B) || !cityToIndex.count(C) || !cityToIndex.count(D)) {
            cout << "City not found.\n"; return 1;
        }
        int a = cityToIndex[A], b = cityToIndex[B], c = cityToIndex[C], d = cityToIndex[D];
        vector<int> best; int bestCost = INT_MAX;
        for (auto pr : { make_pair(b, c), make_pair(c, b) }) {
            auto p1 = bfs(a, pr.first), p2 = bfs(pr.first, pr.second), p3 = bfs(pr.second, d);
            if (p1.empty() || p2.empty() || p3.empty()) continue;
            vector<int> comb = p1;
            comb.insert(comb.end(), p2.begin() + 1, p2.end());
            comb.insert(comb.end(), p3.begin() + 1, p3.end());
            int cost = comb.size() - 1;
            if (cost < bestCost) { bestCost = cost; best = move(comb); }
        }
        if (best.empty()) cout << "No such route through " << displayCity[b] << " and " << displayCity[c] << " to " << displayCity[d] << "\n";
        else {
            printRoute(best);
            cout << "smallest number of connection: " << bestCost << "\n";
        }
    }

    // Query 4: meet at optimal city
    else if (query == 4 && argc == 5) {
        string A = argv[2], B = argv[3], C = argv[4];
        if (!cityToIndex.count(A) || !cityToIndex.count(B) || !cityToIndex.count(C)) { cout << "City not found.\n"; return 1; }
        int a = cityToIndex[A], b = cityToIndex[B], c = cityToIndex[C];
        int bestCity = -1, totalMin = INT_MAX;
        vector<int> r1, r2, r3;
        for (int i = 0; i < n; ++i) {
            if (i == a || i == b || i == c) continue;
            auto pA = bfs(a, i), pB = bfs(b, i), pC = bfs(c, i);
            if (pA.empty() || pB.empty() || pC.empty()) continue;
            int sum = pA.size() + pB.size() + pC.size() - 3;
            if (sum < totalMin) { totalMin = sum; bestCity = i; r1 = pA; r2 = pB; r3 = pC; }
        }
        if (bestCity < 0) cout << "No suitable meeting city found.\n";
        else {
            cout << "You three should meet at " << displayCity[bestCity] << "\n";
            cout << "Route for first person: "; printRoute(r1); cout << "(" << (r1.size() - 1) << " connections)\n";
            cout << "Route for second person: "; printRoute(r2); cout << "(" << (r2.size() - 1) << " connections)\n";
            cout << "Route for third person: "; printRoute(r3); cout << "(" << (r3.size() - 1) << " connections)\n";
            cout << "Total number of connection: " << totalMin << "\n";
        }
    }

    else {
        cout << "Invalid query or arguments.\n";
    }

    return 0;
}
