#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <random>
#include <vector>
#include <iomanip>
#include <exception>
using namespace std;


class UserNotFoundException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: User not found!";
    }
};

class FriendAlreadyExistsException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: Friend already exists!";
    }
};


// ADTS implemented
class Node{
public:
    int data;
    Node *next;
    Node(int val){
        data = val;
        next = NULL;
    }
};

class Stack {
public:
    Node *top;
    Stack()
    {
        top = nullptr;
    }
    void push(int value){
        Node *newNode = new Node(value);
        if (top == NULL){
            top = newNode;
            return;
        }
        newNode->next = top;
        top = newNode;
    }

    void pop(){
        if (empty()){
            cout << "Stack underflow! No element: \n";
            return;
        }
        Node *temp = top;
        top = top->next;
        temp->next = NULL;
        delete temp;
        temp = NULL;
    }
    bool empty(){
        return top == NULL;
    }
    int peek(){
        if (empty())
            return -1;
        return top->data;
    }
};

class Queue {
private:
    Node* front;
    Node* rear;

public:
    Queue() {
        front = nullptr;
        rear = nullptr;
    }
    bool isEmpty() {
        return front == nullptr;
    }
    void enqueue(int val) {
        Node* newNode = new Node(val);
        if (rear == nullptr) { 
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        rear = newNode;
    }
    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty. Cannot dequeue.\n";
            return;
        }
        Node* temp = front;
        front = front->next;
       
        if (front == nullptr) rear = nullptr;
        delete temp;
    }

    int peek() {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return -1;
        }
        return front->data;
    }

    void display() {
        Node* temp = front;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

class User {
private:
    string name;
    bool status;    // user status
    int age;
    vector<string> friends;
    vector<pair<string,string>> inbox;     //{sender, message}
public:

    User () {
        name = "";
        age = 0;
        status = false;
    }

    User (string name, int age) {
        this->name = name;
        this->age = age;
        status = false;
    }
    // getters
    string getName () const {
        return name;
    }

    int getAge() const {
        return age;
    }

    bool getStatus() const {
        return status;
    }

    void setStatus (bool status) {
        this->status = status;
    }

    int partition(vector<string>& arr, int low, int high) {
        string pivot = arr[high];   
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) { 
                i++;
                swap(arr[i], arr[j]);
            }
        }

        swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    // quick sort algorithm
    void quickSort(vector<string>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    void sortfriends () {
        if (friends.size() > 1)
            quickSort(friends,0,(int)friends.size() - 1);
    }

    bool findFriend (string name) {
        sortfriends ();
        int st = 0, end = (int)friends.size() - 1;

        while (st <= end)
        {
            int mid = st + (end - st)/2;
            if (friends[mid] == name) {
                return true;
            }
            else if (friends[mid] < name) {
                st = mid + 1;
            }
            else {
                end = mid - 1;
            }
        }
        return false;
    }

    void addfriend (string friendname) {
        if (friendname.empty()) {
            cout << "\nInput is empty ";
            return ;
        }
        // checking if freind already exist.
        if (find(friends.begin(), friends.end(), friendname) == friends.end()) {
            friends.push_back(friendname);
            cout << "Friend " << friendname << " added.\n";
        } 
        else {
            cout << "\nAlready a friend ";
        }
    }

    void removefriend (string friendname) {
        auto it = find (friends.begin(),friends.end(),friendname);

        // checking if friend exist 
        if (it != friends.end()) {
            friends.erase (it);
        }
        // edge case handle
        else {
            cout << "friend does not found \n";
        }
    }

    void showfriends () const {
        // checking if a person has no freinds.
        if (friends.empty()) {
            cout << "No friends yet.";
            return;
        }     
        cout << "Friends of " << name << ": ";

        for(const string &f: friends) {
            cout << f << " ";
        }
        cout << "\n";
    }

    void sendMessage (User &receiver, string message) {
       receiver.inbox.push_back({this->name, message});
       cout << "Message sent from " << this->name << " to " << receiver.getName() << endl;
    }

    void receiveMessage (User &sender, string message) {
        inbox.push_back ({sender.getName(),message});
        cout << "Message receive from " << sender.getName() << endl;
    }

    void showInbox () const {
        cout << "\nInbox of " << name << ":\n";
        // check if inbox is empty
        if (inbox.empty()) 
            cout << "No messages yet.\n";
        else {
            for(const auto &it : inbox) {
                cout << it.first << " => " << it.second << endl;
            }
        }
    }

    const vector<string>& getFriends() const {
        return friends;
    }

    const vector<pair<string,string>>& getInbox() const {
        return inbox;
    }

    void pushInbox(const pair<string,string>& msg) {
        inbox.push_back(msg);
    }
};

class SocialNetwork {
private:
    unordered_map <string,User> users;     // {name,connections}

public:

    SocialNetwork() {}

    bool UserExists (const string &name) {                // unodered maps provide O(1) searching
        return users.find (name) != users.end();
    }

    string escapeCommas(const string &s) {
        return s;
    }

    User& getUser (string name) {
        return users[name];
    }

    void addUser (string name, int age) {
        if (UserExists(name)) {
            cout << "User already exists.\n";
            return;
        }
        users[name] = User(name, age);
        cout << "User " << name << " added successfully.\n";
        saveToFile();
    }

    void addConnection (string u, string v) {
        if (!UserExists(u) || !UserExists(v)) {
            throw UserNotFoundException();
            return;
        }
        users[u].addfriend (v);
        users[v].addfriend (u);

        cout << "\nConnection added between users: " << u << " and " << v << "\n";
        saveToFile();
    }

    void showfreinds (string name) {
        if(!UserExists (name)) {
            cout << "\nUser does not exist:  ";
            return;
        }
        else {
            users[name].showfriends();
        }
    }

    bool areDirectFriends(string u, string v) {
        for (auto &fr : users[u].getFriends()) {
            if (fr == v) return true;
        }
        return false;
    }

    // BFS with timing print
    bool isConnected (string u, string v) {
        if (!UserExists(u) || !UserExists(v)) {
            return false;
        }

        queue<string> q;
        unordered_map<string,bool> visited;   // {name,true}
        visited[u] = true;
        q.push(u);

        while (!q.empty())
        {
            string current = q.front();
            q.pop();

            if(current == v) {
                return true;
            }

            for (auto &it: users[current].getFriends()) {
                if (!visited[it]) {
                    visited[it] = true;
                    q.push(it);
                }
            }
        }
        return false;
    }

    // DFS using recursion
    void DFS_helper (string u , unordered_map<string,bool> &visited) {
        cout << u << " ";
        visited[u] = true;

        for (auto &v: users[u].getFriends()) {
            if (!visited [v]) {
                DFS_helper (v, visited);
            }
        }
    }
    // find communities
    void find_communities() {
        int group = 0;

        unordered_map<string,bool> visited;
        cout << "\nCommunities:\n";
        for (auto &it: users)
        {
            string name = it.first;
            if (!visited[name]) {
                group++;
                cout << "Group " << group << ": ";
                DFS_helper (name,visited);
            }   
            cout << endl;
        }
        cout << endl;
    }

    void sendMessage (string sender, string receiver, string message) {
      if (!UserExists(sender) || !UserExists(receiver)) {
        cout << "\nUser does not exist.\n";
        return;
      }
      users[sender].sendMessage(users[receiver], message);
      saveToFile();
    }

    void showInbox(string username) {
        if (!UserExists(username)) {
            cout << "User not found.\n";
            return;
        }
        users[username].showInbox();
    }

    void showAllUsers() {
        cout << "\nRegistered Users:\n";
        for (auto &p : users) {
            cout << "- " << p.first << " (" << p.second.getAge() << ")\n";
        }       
    }

    bool detectCycleUtil(const string &u, unordered_map<string,bool> &visited, const string &parent) {
        visited[u] = true;

        for (auto &v : users[u].getFriends()) {
            if (!visited[v]) {
                if (detectCycleUtil(v, visited, u)) {
                    return true;
                }
            }
            else if (v != parent) {
                return true;
            }
        }
        return false;
    }

    bool detectCycle() {
        unordered_map<string,bool> visited;

        for (auto &it : users) {
            if (!visited[it.first]) {
                if (detectCycleUtil(it.first, visited, ""))
                    return true;
                }
        }
        return false;
    }

    void shortestPath(string source, string destination) {
        if (!UserExists(source) || !UserExists(destination)) {
            cout << "One or both users not found.\n";
            return;
        }

        unordered_map<string, bool> visited;
        unordered_map<string, string> parent; // to reconstruct path
        queue<string> q;

        visited[source] = true;
        q.push(source);

        bool found = false;

        while (!q.empty()) {
            string current = q.front();
            q.pop();

            if (current == destination) {
                found = true;
                break;
            }

            for (auto &friendName : users[current].getFriends()) {
                if (!visited[friendName]) {
                    visited[friendName] = true;
                    parent[friendName] = current;
                    q.push(friendName);
                }
            }
        }
    
        if (!found) {
            cout << "No path exists between " << source << " and " << destination << ".\n";
            return;
        }

        vector<string> path;    // to store path
        string current = destination;
        while (current != source) {
            path.push_back(current);
            current = parent[current];
        }
        path.push_back(source);

        reverse(path.begin(), path.end());       // re-stored path

        cout << "Shortest path (" << path.size() - 1 << " connections): ";
        for (auto &user : path) {
            cout << user;
            if (user != destination) cout << " -> ";
        }
        cout << endl;
    }

    vector<string> suggestFriends(string username, int maxSuggestions) {
        vector<string> suggestions;

        if (!UserExists(username)) {
            cout << "User not found.\n";
            return suggestions;
        }

        const auto &fr = users[username].getFriends();
        unordered_set<string> directFriends(fr.begin(), fr.end());

        unordered_map<string, int> mutualCount;

        for (auto &friendName : directFriends) {
            const auto &fofList = users[friendName].getFriends();
            for (auto &fof : fofList) {
                if (fof != username && directFriends.count(fof) == 0) {
                    mutualCount[fof]++;
                }
            }
        }

        vector<pair<int, string>> sortedSuggestions;
        for (auto &p : mutualCount) {
            sortedSuggestions.push_back({p.second, p.first});
        }

        sort(sortedSuggestions.begin(), sortedSuggestions.end(), greater<>());

        for (int i = 0; i < min(maxSuggestions, (int)sortedSuggestions.size()); i++) {
            suggestions.push_back(sortedSuggestions[i].second);
        }

        return suggestions;
    }

    vector<string> commonFriends(string user1, string user2) {
        vector<string> commons;

        if (!UserExists(user1) || !UserExists(user2)) {
            cout << "One or both users not found.\n";
            return commons;
        }

        // Get friends of both users
        const auto &friends1 = users[user1].getFriends();
        unordered_set<string> directFriends(friends1.begin(), friends1.end());

        unordered_set<string> friends2(users[user2].getFriends().begin(), users[user2].getFriends().end());

        // Find intersection
        for (const string &f : directFriends) {
            if (friends2.find(f) != friends2.end()) {
                commons.push_back(f);
            }
        }
        return commons;
    }

    void autoGenerate(int N) {
        users.clear();
        for (int i = 0; i < N; ++i) {
            string name = "U" + to_string(i);
            users[name] = User(name, 18 + (i % 10));
        }
        if (N <= 1) return;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, N-1);
       
        int E = 2 * N;
        for (int i = 0; i < E; ++i) {
            int a = dist(gen), b = dist(gen);
            if (a == b) continue;
            string A = "U" + to_string(a), B = "U" + to_string(b);
            // add both directions
            users[A].addfriend(B);
            users[B].addfriend(A);
        }
    }

    void saveToFile(const string &filename = "social_data.txt") {
        ofstream out(filename);
        if (!out) {
            cout << "Error opening file for writing!\n";
            return;
        }

        for (auto &p : users) {
            const string &username = p.first;
            out << "USER," << username << "," << p.second.getAge();
            const auto &fr = p.second.getFriends();
            for (const auto &f : fr) {
                out << "," << f;
            }
            out << "\n";
        }
        for (auto &p : users) {
            const string &username = p.first;
            const auto &inb = p.second.getInbox();
            for (const auto &m : inb) {
                out << "MSG," << m.first << "," << username << "," << m.second << "\n";
            }
        }

        out.close();
    }

    void loadFromFile(const string &filename = "social_data.txt") {
    ifstream in(filename);
    if (!in) {
        cout << "No saved file found. Starting fresh.\n";
        return;
    }

    users.clear();
    string line;
    vector<pair<string, vector<string>>> pendingFriends;
    vector<tuple<string,string,string>> pendingMessages; // sender, receiver, message

    while (getline(in, line)) {
        if (line.empty()) continue;

        vector<string> tokens;
        string token;

        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] == ',') {
                tokens.push_back(token);
                token.clear();
            } else {
                token.push_back(line[i]);
            }
        }
        tokens.push_back(token);

        if (tokens.size() == 0) continue;

        if (tokens[0] == "USER") {
            if (tokens.size() >= 3) {
                string username = tokens[1];
                int age = 0;
                try { age = stoi(tokens[2]); } catch (...) { age = 0; }

                users[username] = User(username, age);

                vector<string> friendsList;
                for (size_t k = 3; k < tokens.size(); ++k) {
                    if (!tokens[k].empty())
                        friendsList.push_back(tokens[k]);
                }

                if (!friendsList.empty())
                    pendingFriends.push_back({username, friendsList});
            }
        }
        else if (tokens[0] == "MSG") {
            if (tokens.size() >= 4) {
                string sender = tokens[1];
                string receiver = tokens[2];

                string msg;
                for (size_t k = 3; k < tokens.size(); ++k) {
                    if (k > 3) msg.push_back(',');
                    msg += tokens[k];
                }

                pendingMessages.push_back({sender, receiver, msg});
            }
        }
    }
    for (auto &p : pendingFriends) {
        const string &username = p.first;
        for (const auto &f : p.second) {
            if (UserExists(f) && UserExists(username)) {
                users[username].addfriend(f);
            }
        }
    }

    for (auto &p : users) {
        string u = p.first;

        for (auto &v : p.second.getFriends()) {
            if (UserExists(v)) {
                const auto &friendsV = users[v].getFriends();

                if (find(friendsV.begin(), friendsV.end(), u) == friendsV.end()) {
                    users[v].addfriend(u);
                }
            }
        }
    }

    for (auto &t : pendingMessages) {
        string sender = get<0>(t);
        string receiver = get<1>(t);
        string msg = get<2>(t);

        if (UserExists(sender) && UserExists(receiver)) {
            users[receiver].receiveMessage(users[sender], msg);
        }
    }

    in.close();
    cout << "Data loaded successfully.\n";
}

};



template<typename Func>
long long measureMicroseconds(Func f) {
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}



bool isValidUsername(const string &name) {
    return !name.empty() && name.length() <= 20;
}

int getSafeInt() {
    int x;
    while (true) {
        cin >> x;
        if (!cin.fail()) return x;

        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please enter a number: ";
    }
}

string getValidUsername() {
    string name;

    while (true) {
        getline(cin, name);
        if (isValidUsername(name))
            return name;

        cout << "Invalid username! Must be 1-20 characters. Try again: ";
    }
}

int getValidAge() {
    cout << "Enter age (0-120): ";
    int age = getSafeInt();

    while (age < 0 || age > 120) {
        cout << "Invalid age! Enter a value between 0 and 120: ";
        age = getSafeInt();
    }
    return age;
}

int main()
{
    SocialNetwork sn;
    sn.loadFromFile();

    int choice;
    string u, v, msg;
    int age;

    while (true) {
        cout << "\n==== Social Network Simulator ====\n";
        cout << "1. Add User";
        cout << "\n2. Add Connection ";
        cout << "\n3. Show Friends ";
        cout << "\n4. Check Connection ";
        cout << "\n5. Shortest Path ";
        cout << "\n6. Find Communities ";
        cout << "\n7. Send Message ";
        cout << "\n8. Show Inbox ";
        cout << "\n9. Show All Users ";
        cout << "\n10. Detect Cycle ";
        cout << "\n11. Suggest friends ";
        cout << "\n12. Common freinds ";
        cout << "\n13. Show Time";
        cout << "\n14. Exit\n";
       
        cout << "Enter your choice: ";
        choice = getSafeInt();
        cin.ignore();

        if (choice == 1) {
            cout << "Enter username, age: ";
            u = getValidUsername();
            age = getValidAge();
            sn.addUser(u, age);
        }
        else if (choice == 2) {
            cout << "Enter first username: ";
            u = getValidUsername(); 
            cout << "Enter second username: ";
            v = getValidUsername(); 
            try {
                sn.addConnection(u, v);
            }
            catch (FriendAlreadyExistsException &e) {
                cout << e.what() << endl;
            }
            catch (UserNotFoundException &e) {
                cout << e.what() << endl;
            }
        }

        else if (choice == 3) {
            cout << "Enter username: ";
            u = getValidUsername();
            sn.showfreinds(u);
        }
        else if (choice == 4) {
            cout << "Enter first username: ";
            u = getValidUsername();
            cout << "Enter second username: ";
            v = getValidUsername();
            if  (sn.isConnected(u, v)) {
                cout << "Connected\n";
            }
            else {
                cout << "Not Connected\n";
            }
        }
        else if (choice == 5) {
            cout << "Enter source: ";
            u = getValidUsername();
            cout << "Enter destination ";
            v = getValidUsername();
            sn.shortestPath(u, v);
        }
        else if (choice == 6) {
            sn.find_communities();
        }
        else if (choice == 7) {
            cout << "Enter sender name: ";
            u = getValidUsername();
            cout << "Enter receiver name: ";
            v  = getValidUsername();
            cout << "\nEnter message you want to send: ";
            getline(cin, msg);

            sn.sendMessage(u, v, msg);
        }
        else if (choice == 8) {
            cout << "Enter username: ";
            u = getValidUsername();
            sn.showInbox(u);
        }
        else if (choice == 9) {
            sn.showAllUsers();
        }
        else if (choice == 10) {
            if (sn.detectCycle()) {
                cout << "\nCycle exist in network \n"; 
            }
            else {
                cout << "\nCycle does not exist \n";
            }
        }
        else if (choice == 11) {
            cout << "Enter username: ";
            u = getValidUsername();

            int maxSuggestions;
            cout << "How many suggestions do you want? ";
            maxSuggestions = getSafeInt();
            cin.ignore();

            vector<string> sugg = sn.suggestFriends(u, maxSuggestions);

            if (sugg.empty()) {
                cout << "No friend suggestions available.\n";
            } 
            else {
                cout << "Suggested Friends for " << u << ": ";
                for (auto &name : sugg) {
                    cout << name << " ";
                }
                cout << endl;
            }
        }

        else if (choice == 12) {
            cout << "Enter first username: ";
            u = getValidUsername();
            cout << "Enter second username: ";
            v = getValidUsername();

            vector<string> commons = sn.commonFriends(u, v);

            if (commons.empty()) {
                cout << "No common friends.\n";
            } 
            else {
                cout << "Common Friends between " << u << " and " << v << ": ";
                for (auto &f : commons) {
                    cout << f << " ";
                }
            cout << endl;
        }
        }
       

#include <iomanip> 

else if (choice == 13) {
    cout << "\n========================================================\n";
    cout << "         PERFORMANCE TESTING - TIME COMPLEXITY          \n";
    cout << "            (Time measured in microseconds)             \n";
    cout << "========================================================\n\n";
    
    vector<int> testSizes = {10, 50, 1000, 5000, 10000};
    
    // Table header
    cout << "+---------+-----------+-----------+-----------+-----------+------------+------------+-------------+\n";
    cout << "| Users(N)|    BFS    | BFS Ratio |    DFS    | DFS Ratio |  SP Path   |  Suggest   |   Common    |\n";
    cout << "+---------+-----------+-----------+-----------+-----------+------------+------------+-------------+\n";
    
    long long prevBfsTime = 0;
    long long prevDfsTime = 0;

    for (int N : testSizes) {
        sn.autoGenerate(N);
        string A = "U0";
        string B = "U" + to_string(max(0, N-1));
        
        long long bfsTime = measureMicroseconds([&](){ sn.isConnected(A, B); });
        long long dfsTime = measureMicroseconds([&](){ sn.find_communities(); });
        long long spTime  = measureMicroseconds([&](){ sn.shortestPath(A, B); });
        long long sugTime = measureMicroseconds([&](){ sn.suggestFriends(A, 10); });
        long long comTime = measureMicroseconds([&](){ sn.commonFriends(A, B); });
        
        double bfsRatio = prevBfsTime == 0 ? 0 : (double)bfsTime / prevBfsTime;
        double dfsRatio = prevDfsTime == 0 ? 0 : (double)dfsTime / prevDfsTime;
        prevBfsTime = bfsTime;
        prevDfsTime = dfsTime;
        
        // Print row
        cout << "| " << setw(7) << N << " | ";
        cout << setw(9) << bfsTime << " | ";
        if (bfsRatio == 0) cout << setw(9) << "-";
        else cout << setw(9) << fixed << setprecision(2) << bfsRatio;
        cout << " | ";
        cout << setw(9) << dfsTime << " | ";
        if (dfsRatio == 0) cout << setw(9) << "-";
        else cout << setw(9) << fixed << setprecision(2) << dfsRatio;
        cout << " | ";
        cout << setw(10) << spTime << " | ";
        cout << setw(10) << sugTime << " | ";
        cout << setw(11) << comTime << " |\n";
    }
    
    cout << "+---------+-----------+-----------+-----------+-----------+------------+------------+-------------+\n";
    cout << "\nNOTE: V = Vertices (users), E = Edges (friendships)\n";
    cout << "      Times may vary based on system load and graph structure\n";
    cout << "\n========================================================\n";
    cout << "           PERFORMANCE TEST COMPLETED                   \n";
    cout << "========================================================\n";
}

        else if (choice == 14) {
            cout << "Exiting...\n";
            break;
        }
        
        else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
