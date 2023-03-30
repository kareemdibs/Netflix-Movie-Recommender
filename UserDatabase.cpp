#include "User.h"
#include "UserDatabase.h"
#include "treemm.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

UserDatabase::UserDatabase() {}

UserDatabase::~UserDatabase() {
    for (int i = 0; i < m_users.size(); i++) {
        delete m_users[i];
    }
}

// Load user data from a file specified by filename and construct a binary search tree of users
// Returns true if the file was successfully loaded, false otherwise
bool UserDatabase::load(const string& filename) {
    // Open the file for reading
    ifstream infile(filename);

    // If the file could not be opened, return false
    if (!infile) {
        return false;
    }

    // Read the file line by line
    string tempStr;
    while (getline(infile, tempStr)) {
        // The first line of each record contains the user's name
        string tempName = tempStr;

        // The second line contains the user's email address
        string tempEmail;
        if (!getline(infile, tempEmail)) {
            // If there is no second line, return false
            return false;
        }

        // The third line contains the number of movies watched by the user
        string num_movies_watched;
        if (!getline(infile, num_movies_watched)) {
            // If there is no third line, return false
            return false;
        }
        // Convert the string to an integer
        int int_num_movies_watched = stoi(num_movies_watched);

        // Read the IDs of the movies watched by the user
        vector<string> history_movies;
        for (int q = 0; q < int_num_movies_watched; q++) {
            string ID_mov;
            if (!getline(infile, ID_mov)) {
                // If there are not enough lines, return false
                return false;
            }
            // Add the ID to the user's list of watched movies
            history_movies.push_back(ID_mov);
        }

        // Create a new User object with the data read from the file
        User* m_user = new User(tempName, tempEmail, history_movies);

        // Insert users into m_users vector to be deleted later
        m_users.push_back(m_user);

        // Add the user to the binary search tree
        m_TMM.insert(tempEmail, m_user);

        // Skip the newline character after the current record
        getline(infile, tempStr);
    }
    // Return true to indicate that the file was successfully loaded
    return true;
}

// Find and return a User object based on their email address
// If a user with the specified email is found, return a pointer to the User object
// If a user with the specified email is not found, return a nullptr
User* UserDatabase::get_user_from_email(const string& email) const {
    // Search for the user in the binary search tree using their email address as the key
    TreeMultimap<string, User*>::Iterator it = m_TMM.find(email);

    // If the iterator is valid (i.e., the user was found), return a pointer to the User object
    if (it.is_valid()) {
        User* m_user = it.get_value();
        return m_user;
    }
    else {
        // If the iterator is not valid (i.e., the user was not found), return nullptr
        return nullptr;
    }
}