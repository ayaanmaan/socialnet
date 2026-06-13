#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <sstream>  
#include <queue>

size_t op = 0;

#define OP op++
#define VALID(username) if (!valid_user(username)) {std::cout << "NameError: Invalid username. Only alphabets, numbers 0-9 or underscore _ allowed" << std::endl; continue;}
#define DUPLICATE(user) if (user) {std::cout << "DuplicateError: User " << user->username << " already exists" << std::endl; continue;}
#define NOEXIST(user) if (!user) {std::cout << "Error 404: User not found" << std::endl; continue;}

bool valid_char(char c) {
    return (c >= 48 && c <= 57) || (c >= 95 && c <= 122 && c != 96);
}
bool valid_user(std::string username) {
    size_t size = username.size();
    for (size_t idx = 0; idx < size; idx++) {
        if (!valid_char(username[idx])) {
            return 0;
        }
    }
    return !size ? 0 : 1;
}
int string_compare(std::string s1, std::string s2) {
    size_t n1 = s1.size(), n2 = s2.size();

    bool equal = 1;
    bool compare;
    for (size_t idx = 0; idx < std::min(n1, n2); idx++) {
        char c1 = s1[idx], c2 = s2[idx];
        if (c1 == c2) {
            continue;
        }
        equal = 0;
        compare = c1 < c2;
        break;
    }
    return !equal ? compare : (n1 == n2 ? -1 : (int)(n1 < n2)); // -1 = equal, 1 = s1 smaller, 0 = s2 smaller
}
void lower(std::string& data) {
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
}

struct Post {
    Post* parent{nullptr};
    Post* left{nullptr};
    Post* right{nullptr};
    size_t height{0};

    std::string content;
    size_t operation;

    Post(std::string post_content) {
        content = post_content;
        operation = OP;
    }
};
struct PostTree {
    Post* root;

    int height(Post* post) {
        if (!post) return -1;
        return post->height;
    }

    int newHeight(Post* post) {
        return std::max(height(post->left), height(post->right)) + 1;
    }
    Post* rightRotate(Post* y) {
        Post* x = y->left;
        Post* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = newHeight(y);
        x->height = newHeight(x);
        return x;
    }

    Post* leftRotate(Post* x) {
        Post* z = x->right;
        Post* temp = z->left;
        x->right= temp;
        z->left=x;
        x->height=newHeight(x); 
        z->height=newHeight(z);
        return z;
    }
    
    int getBalance(Post* post) {
        if (!post) return 0;
        return height(post->left) - height(post->right);
    }

    Post* insertHelper(Post* node, Post* temp) {
        if (!node) return temp;
        if (temp->operation < node->operation) {
            node->left = insertHelper(node->left, temp);
        }
        else {
            node->right = insertHelper(node->right, temp);
        }
        node->height = newHeight(node);
        int balance = getBalance(node);
        if (balance > 1 && temp->operation < node->left->operation) {
            return rightRotate(node);
        }
        if (balance < -1 && temp->operation > node->right->operation) {
            return leftRotate(node);
        }
        if (balance > 1 && temp->operation > node->left->operation) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && temp->operation < node->right->operation) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }
    
    Post* minValueNode(Post* node) {
        Post* curr = node;
        while (curr->left) {
            curr = curr->left;
        }
        return curr;
    }

    void deleteTree(Post* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
    
    PostTree() {
        root = nullptr;
    }
    
    ~PostTree() {
        deleteTree(root);
    }
    
    void insert(std::string post_content) {
        root = insertHelper(root, new Post(post_content));
    }
    
    int getRootBalanceFactor() {
        if (!root) return 10;
        return getBalance(root);
    }

    void reverseInorder(Post* node, std::vector<std::string>& postList) {
        if (!node) return;
        reverseInorder(node->right, postList);
        postList.push_back(node->content);
        reverseInorder(node->left, postList);
    }
};

struct User;
typedef std::unordered_map<std::string, User*> Friends; 

struct User {
    std::string username;
    PostTree posts;   
    Friends friends; 

    User(std::string uname) : username(uname) {}
};

std::unordered_map<std::string, User*> network;

std::unordered_map<std::string, int> commands{
                                                {"EXIT", -1},
                                                {"QUIT", -1},
                                                {"ADD_USER", 1},                // <username> 
                                                {"LIST_FRIENDS", 2},           // <username> 
                                                {"ADD_FRIEND", 3},            // <username1> <username2> 
                                                {"DEGREES_OF_SEPARATION", 4},// <username1> <username2> 
                                                {"SUGGEST_FRIENDS", 5},     // <username> <N> 
                                                {"OUTPUT_POSTS", 6},       // <username> <N> 
                                                {"ADD_POST", 7}           // <username> "<post content>"
                                            };
                                            
int main() {
    while (true) {
        std::string input;
        if (!std::getline(std::cin, input)) break;
        if (input.empty()) {
            std::cout << "Warning: Input empty" << std::endl;
            continue;
        }
        std::stringstream inputline(input);

        std::string command; inputline >> command;
        int commandID = commands[command];

        if (!commandID) { // UNDEFINED
            std::cout << "CommandError: Undefined command" << std::endl;
            continue;
        }
        else if (commandID < 0) { // EXIT, QUIT
            for (const auto& user : network) {
                delete user.second;
            }
            exit(EXIT_SUCCESS);
        }

        std::string username; inputline >> username;
        lower(username);
        VALID(username);

        User* user0 = network.contains(username) ? network[username] : nullptr;
        

        if (commandID < 3) {
            if (commandID == 1) { // ADD_USER
                DUPLICATE(user0);

                network[username] = new User(username);
                std::cout << "User " << username << " added" << std::endl;
            } 
            else { // LIST_FRIENDS
                NOEXIST(user0);

                if (user0->friends.empty()) {
                    std::cout << "User has no friends" << std::endl;
                } 
                else {
                    std::vector<std::string> sortedFriends;

                    for (const auto& userfriend : user0->friends) {
                        sortedFriends.push_back(userfriend.first);
                    }
                    std::sort(sortedFriends.begin(), sortedFriends.end());

                    for (const auto& friendname : sortedFriends) {
                        std::cout << friendname << std::endl;
                    }
                }
            }
        }
        else if (commandID < 5){ // (3) ADD_FRIEND, (4) DEGREES_OF_SEPARATION
            std::string username1; inputline >> username1;
            lower(username1);
            VALID(username1);
            User* user1 = network.count(username1) ? network[username1] : nullptr;

            if (!user0 || !user1) {
                std::cout << "FindError: Atleast one user was not found" << std::endl;
                continue;
            }

            if (commandID == 3) { // ADD_FRIEND
                if (user0 == user1) {
                    std::cout << "Can't self-friend users" << std::endl;
                    continue;
                }

                if (user0->friends.contains(user1->username)) {
                    std::cout << "Users " << username << " and " << username1 << " are already friends" << std::endl;
                }

                else {
                    user0->friends[user1->username] = user1;
                    user1->friends[user0->username] = user0;
                    std::cout << "Friendship added between " << username << " and " << username1 << std::endl;
                }
            } 
            else { // DEGREES_OF_SEPARATION
                if (user0 == user1) {
                    std::cout << 0 << std::endl;
                    continue;
                }

                std::queue<std::pair<User*, int>> q;
                std::unordered_set<User*> visited;

                q.push({user0, 0});
                visited.insert(user0);
                int degrees = -1;

                while (!q.empty()) {
                    User* curr = q.front().first;
                    int dist = q.front().second;
                    q.pop();

                    if (curr == user1) {
                        degrees = dist;
                        break;
                    }
                    for (const auto& userfriend : curr->friends) {
                        if (!visited.contains(userfriend.second)) {
                            visited.insert(userfriend.second);
                            q.push({userfriend.second, dist + 1});
                        }
                    }
                }
                std::cout << degrees << std::endl;
            }
        }
        else if (commandID < 7){
            // (5) SUGGEST_FRIENDS, (6) OUTPUT_POSTS 
            NOEXIST(user0);
            std::string NUMBER; inputline >> NUMBER;
            if (NUMBER == "") {
                std::cout << "Number argument not provided" << std::endl;
                continue;
            }
            bool wrong = 0;
            for (const auto& c : NUMBER) {
                if (c < 48 || c > 57) {
                    std::cout << "TypeError: Required input was of wrong type" << std::endl;
                    wrong = 1;
                    break;
                }
            }
            if (wrong) continue;
            int N = stoi(NUMBER);

            if (commandID == 5) { // SUGGEST_FRIENDS
                if (N <= 0) continue;
                std::unordered_map<std::string, int> suggestions; 
                for (const auto& userfriend1 : user0->friends) {
                    for (const auto& userff : userfriend1.second->friends) {
                        if (userff.second != user0 && !user0->friends.contains(userff.first)) {
                            suggestions[userff.first]++;
                        }
                    }
                }
                std::vector<std::pair<std::string, int>> sortSuggestions;
                for(const auto& suggestion : suggestions) {
                    sortSuggestions.push_back(suggestion);
                }
                std::sort(sortSuggestions.begin(), sortSuggestions.end(), 
                    [](const std::pair<std::string, int>& s1, const std::pair<std::string, int>& s2) {
                    if (s1.second != s2.second) {
                        return s1.second > s2.second;
                    }
                    return s1.first < s2.first;
                });

                int count = 0;
                for (const auto& s : sortSuggestions) {
                    if (count >= N) break;
                    std::cout << s.first << " (" << s.second << " mutual friends)" << std::endl;
                    count++;
                }
                if (count == 0) {
                    std::cout << "No suggestions found" << std::endl;
                }

            } 
            
            else { // OUTPUT_POSTS
                std::vector<std::string> posts;
                user0->posts.reverseInorder(user0->posts.root, posts);

                if (posts.empty()) {
                    std::cout << "No posts by " << username << "." << std::endl;
                    continue;
                }
                
                size_t limit;
                if (N == -1) {
                    limit = posts.size(); 
                } 
                else {
                    limit = std::min((size_t)N, posts.size());
                }
                
                for (size_t i = 0; i < limit; ++i) {
                    std::cout << posts[i] << std::endl;
                }
            }
        }
        else { // ADD_POST
            NOEXIST(user0);
            std::string post_content;
            inputline >> post_content;
            user0->posts.insert(post_content);
            std::cout << "Post added by " << username << std::endl;
        }
    }
}