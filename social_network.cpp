#include <iostream>
#include <unordered_map>
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

class User {
private:
    string name;
    string password;
    bool status;    // user status (logged in)
    int age;
    vector<string> friends;
    vector<pair<string,string>> inbox;     //{sender, message}
public:
    User () {
        name = "";
        age = 0;
        status = false;
        password = "";
    }

    User (string name, int age) {
        this->name = name;
        this->age = age;
        status = false;
        password = "";
    }

    string getName () {
        return name;
    }

    int getAge() {
        return age;
    }

    bool getStatus() {
        return status;
    }

    void setStatus (bool status) {
        this->status = status;
    }

    string getPassword() {
        return password;
    }

    void setPassword(const string &p) {
        password = p;
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

    void quickSort(vector<string>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    void sortfriends () {
        if (friends.empty()) return;
        quickSort(friends, 0, (int)friends.size() - 1);
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
        // checking if friend already exist
        if (find(friends.begin(), friends.end(), friendname) == friends.end()) {
            friends.push_back(friendname);
            cout << "Friend " << friendname << " added.\n";
        } 
        else {
            cout << "Already a friend.\n";
        }
    }

    void removefriend (string friendname) {
        auto it = find (friends.begin(),friends.end(),friendname);

        // checking if friend exist
        if (it != friends.end()) {
            friends.erase (it);
        }
        else {
            cout << "friend not found \n";
        }
    }

    void showfriends () {
        // checking if a person has no friends.
        if (friends.empty()) {
            cout << "No friends yet.";
            return;
        }     
        cout << "Friends of " << name << ": ";

        for(string &f: friends) {
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
        cout << "Message received from " << sender.getName() << endl;
    }

    void showInbox () {
        cout << "\nInbox of " << name << ":\n";
        // inbox is empty
        if (inbox.empty()) 
            cout << "No messages yet.\n";
        else {
            for(auto &it : inbox) {
                cout << it.first << " => " << it.second << endl;
            }
        }
    }

    vector<string> getFriends()  {
        return friends;
    }

    // helpers for file I/O
    void setFriends(const vector<string> &f) {
        friends = f;
    }
    void setInbox(const vector<pair<string,string>> &ib) {
        inbox = ib;
    }
    vector<pair<string,string>> getInbox() {
        return inbox;
    }
};

class SocialNetwork {
private:
    unordered_map <string,User> users; 
public:

    bool UserExists (string name) {                // unordered maps provide O(1) searching
        if (users.find (name) != users.end()) {
            return true;
        }
        else {
            return false;
        }
    }

    User& getUser (string name) {
        return users[name];
    }

    void addUser (string name, int age) {
        // checking if user already
        if (UserExists(name)) {
            cout << "User already exists.\n";
            return;
        }
        users[name] = User(name, age);
        cout << "User " << name << " added successfully.\n";
    }

    void addUser (string name, int age, const string &password) {
        if (UserExists(name)) {
            cout << "User already exists.\n";
            return;
        }
        users[name] = User(name, age);
        users[name].setPassword(password);
        cout << "User " << name << " added successfully.\n";
    }

    void addConnection (string u, string v) {
        if (!UserExists(u) || !UserExists(v)) {
            cout << "One or both users not found.\n";
            return;
        }
        users[u].addfriend (v);
        users[v].addfriend (u);

        cout << "\nConnection added between users: " << u << " and " << v << endl;
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

    bool isConnected (string u, string v) {
        if (!UserExists(u) || !UserExists(v)) {
            return false;
        }
        // check using BFS traversal
        queue<string> q;

        unordered_map<string,bool> visited;
        visited[u] = true;
        q.push(u);

        while (!q.empty())
        {
            string current = q.front();
            q.pop();

            if(current == v) return true;

            for (auto &it: users[current].getFriends()) {
                if (!visited[it]) {
                    visited[it] = true;
                    q.push(it);
                }
            }
        }
        return false;
    }

    void DFS_helper (string u , unordered_map<string,bool> &visited) {
        cout << u << " ";
        visited[u] = true;

        for (auto &v: users[u].getFriends()) {
            if (!visited [v]) {
                DFS_helper (v, visited);
            }
        }
    }

    void find_communities() {
        int group = 0;

        unordered_map<string,bool> visited ;
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
      users[receiver].receiveMessage(users[sender], message);
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
                if (detectCycleUtil(v, visited, u))
                    return true;
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

        vector<string> path;
        string current = destination;
        while (current != source) {
            path.push_back(current);
            current = parent[current];
        }
        path.push_back(source);

        reverse(path.begin(), path.end());

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

        unordered_set<string> directFriends(users[username].getFriends().begin(),
                                            users[username].getFriends().end());
        unordered_map<string, int> mutualCount; // count mutual friends

        // Check friends of friends
        for (auto &friendName : directFriends) {
            for (auto &fof : users[friendName].getFriends()) {
                // ignore self and existing direct friends
                if (fof != username && directFriends.find(fof) == directFriends.end()) {
                    mutualCount[fof]++;
                }
            }
        }

        // Create a vector of suggestions sorted by mutual friends count
        vector<pair<int, string>> sortedSuggestions;
        for (auto &p : mutualCount) {
            sortedSuggestions.push_back({p.second, p.first});
        }

        // Sort in descending order of mutual friends
        sort(sortedSuggestions.begin(), sortedSuggestions.end(), greater<>());

        // Pick top maxSuggestions
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
        unordered_set<string> friends1(users[user1].getFriends().begin(), users[user1].getFriends().end());
        unordered_set<string> friends2(users[user2].getFriends().begin(), users[user2].getFriends().end());

        // Find intersection
        for (const string &f : friends1) {
            if (friends2.find(f) != friends2.end()) {
                commons.push_back(f);
            }
        }

        return commons;
    }
    void saveToFile(const string &filename = "users.txt") {
        ofstream out(filename);
        if (!out) {
            cout << "Failed to open file for saving: " << filename << endl;
            return;
        }

        for (auto &p : users) {
            string uname = p.first;
            User &u = p.second;
            out << "USER " << uname << "\n";
            out << "AGE " << u.getAge() << "\n";
            out << "PASS " << u.getPassword() << "\n";

            // friends
            vector<string> f = u.getFriends();
            out << "FRIENDS ";
            for (size_t i = 0; i < f.size(); ++i) {
                out << f[i];
                if (i + 1 < f.size()) out << ",";
            }
            out << "\n";

            // inbox
            out << "INBOX\n";
            auto inbox = u.getInbox();
            for (auto &msg : inbox) {
                // replace any newline characters in message with a space to keep file format safe
                string safeMsg = msg.second;
                for (char &c : safeMsg) if (c == '\n') c = ' ';
                out << msg.first << "|" << safeMsg << "\n";
            }
            out << "ENDINBOX\n";
            out << "ENDUSER\n";
        }

        out.close();
        cout << "Saved users to file.\n";
    }

    void loadFromFile(const string &filename = "users.txt") {
        ifstream in(filename);
        if (!in) {
           
            // cout << "No save file found (" << filename << "). Starting fresh.\n";
            return;
        }

        string line;
        string curUser;
        int curAge = 0;
        string curPass;
        vector<string> curFriends;
        vector<pair<string,string>> curInbox;

        while (getline(in, line)) {
            if (line.rfind("USER ", 0) == 0) {
               
                if (!curUser.empty()) {
                    users[curUser] = User(curUser, curAge);
                    users[curUser].setPassword(curPass);
                    users[curUser].setFriends(curFriends);
                    users[curUser].setInbox(curInbox);
                    // reset
                    curUser.clear(); curFriends.clear(); curInbox.clear(); curPass.clear(); curAge = 0;
                }
                curUser = line.substr(5);
            }
            else if (line.rfind("AGE ", 0) == 0) {
                curAge = stoi(line.substr(4));
            }
            else if (line.rfind("PASS ", 0) == 0) {
                curPass = line.substr(5);
            }
            else if (line.rfind("FRIENDS ", 0) == 0) {
                curFriends.clear();
                string rest = line.substr(8);
                if (!rest.empty()) {
                    stringstream ss(rest);
                    string token;
                    while (getline(ss, token, ',')) {
                        if (!token.empty())
                            curFriends.push_back(token);
                    }
                }
            }
            else if (line == "INBOX") {
                // read lines until ENDINBOX
                while (getline(in, line)) {
                    if (line == "ENDINBOX") break;
                    // parse sender|message
                    size_t pos = line.find('|');
                    if (pos != string::npos) {
                        string sender = line.substr(0, pos);
                        string message = line.substr(pos + 1);
                        curInbox.push_back({sender, message});
                    }
                }
            }
            else if (line == "ENDUSER") {
                if (!curUser.empty()) {
                    users[curUser] = User(curUser, curAge);
                    users[curUser].setPassword(curPass);
                    users[curUser].setFriends(curFriends);
                    users[curUser].setInbox(curInbox);
                    // reset for next block
                    curUser.clear(); curFriends.clear(); curInbox.clear(); curPass.clear(); curAge = 0;
                }
            }
        }

        if (!curUser.empty()) {
            users[curUser] = User(curUser, curAge);
            users[curUser].setPassword(curPass);
            users[curUser].setFriends(curFriends);
            users[curUser].setInbox(curInbox);
        }

        in.close();
        cout << "Loaded users from file.\n";
    }

    bool login(const string &username, const string &password) {
        if (!UserExists(username)) return false;
        if (users[username].getPassword() == password) {
            users[username].setStatus(true);
            return true;
        }
        return false;
    }

    void logout(const string &username) {
        if (UserExists(username)) {
            users[username].setStatus(false);
        }
    }
};

// funcion to check integer
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
    string currentUser = ""; // track logged-in user

    while (true) {
        cout << "\n==== Social Network Simulator ====\n";
        cout << "1.Add User";
        cout << "\n2. Add Connection ";
        cout << "\n3. Show Friends ";
        cout << "\n4. Check Connection ";
        cout << "\n5. Shortest Path ";
        cout << "\n6. Find Communities ";
        cout << "\n7. Send Message ";
        cout << "\n8. Show Inbox ";
        cout << "\n9. Show All Users ";
        cout << "\n10. Exit";
        cout << "\n11. Register (with password)";
        cout << "\n12. Login";
        cout << "\n13. Logout\n";

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
            sn.addConnection(u, v);
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
            cout << (sn.isConnected(u, v) ? "Connected\n" : "Not Connected\n");
        }
        else if (choice == 5) {
            cout << "Enter source: ";
            u = getValidUsername();
            cout << "Enter destination: ";
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
            cout << "Exiting...\n";
            sn.saveToFile();
            break;
        }
        else if (choice == 11) { // Register with password
            cout << "Register - Enter username: ";
            u = getValidUsername();
            cout << "Enter age: ";
            age = getValidAge();
            cout << "Enter password: ";
            string pass;
            getline(cin, pass);
            sn.addUser(u, age, pass);
        }
        else if (choice == 12) { // Login
            cout << "Login - Enter username: ";
            u = getValidUsername();
            cout << "Enter password: ";
            string pass;
            getline(cin, pass);

            if (sn.login(u, pass)) {
                currentUser = u;
                cout << "Login successful. Current user: " << currentUser << endl;
            } else {
                cout << "Login failed. Incorrect username or password.\n";
            }
        }
        else if (choice == 13) { // Logout
            if (!currentUser.empty()) {
                sn.logout(currentUser);
                cout << "Logged out: " << currentUser << endl;
                currentUser.clear();
            } else {
                cout << "No user currently logged in.\n";
            }
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
