#include "User.h"
#include "UserDatabase.h"
#include "Movie.h"
#include "MovieDatabase.h"
#include "Recommender.h"
#include <iostream>
#include <string>
#include <chrono>
#include <cassert>
#include <list>
#include <vector>
using namespace std;

const string USER_DATAFILE = "users.txt";
const string MOVIE_DATAFILE = "movies.txt";


// This function finds movie recommendations for a given user using a Recommender object and a MovieDatabase object
// It takes in the user email, and the number of recommendations to provide
// It also times how long it takes to generate the recommendations
void findMatches(const Recommender& r,
    const MovieDatabase& md,
    const string& user_email,
    int num_recommendations) {
    // Start timing the recommendation process
    auto start_time = chrono::steady_clock::now();

    // Use the Recommender object to generate a list of recommended movies for the given user email
    vector<MovieAndRank> recommendations = r.recommend_movies(user_email, num_recommendations);

    // Stop timing the recommendation process
    auto end_time = chrono::steady_clock::now();

    // Print how long it took to generate the recommendations
    cout << "Recommendation generation took " << (chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()) << "ms" << endl;

    // If no recommendations were found, print a message to the console
    if (recommendations.empty()) {
        cout << "No recommendations found" << endl;
    }
    // Otherwise, print the list of recommended movies and their information
    else {
        for (int i = 0; i < recommendations.size(); i++) {
            // Get a pointer to the movie object from the MovieDatabase using the movie id
            Movie* movie = md.get_movie_from_id(recommendations.at(i).movie_id);

            // Print the movie title, release year, rating, and compatibility score to the console
            cout << i << ". " << movie->get_title() << " (" << movie->get_release_year() << ")\n Rating: "
                << movie->get_rating() << "\n Compatibility Score: " << recommendations.at(i).compatibility_score << "\n";
        }
    }

}

int main()
{
    // Load user database
    cout << "Loading user database..." << endl;
    auto startUser = chrono::steady_clock::now();
    UserDatabase userDb;
    if (!userDb.load(USER_DATAFILE)) {
        cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
        return 1;
    }
    auto stopUser = chrono::steady_clock::now();
    cout << "User database loaded" << endl;
    cout << "Took " << chrono::duration_cast<chrono::milliseconds>(stopUser - startUser).count() << "ms" << endl;

    // Load movie database
    cout << "Loading movie database..." << endl;
    auto startMovie = chrono::steady_clock::now();
    MovieDatabase movieDb;
    if (!movieDb.load(MOVIE_DATAFILE)) {
        cout << "Failed to load movie data file " << MOVIE_DATAFILE << "!" << endl;
        return 1;
    }
    auto stopMovie = chrono::steady_clock::now();
    cout << "Movie database loaded" << endl;
    cout << "Took " << chrono::duration_cast<chrono::milliseconds>(stopMovie - startMovie).count() << "ms" << endl;

    // User interface loop
    while (true) {
        // Display options
        cout << "1. User lookup\n2. Movie lookup\n3. Recommendation generator\n9. Exit" << endl;
        cout << "Enter a number: ";
        string choice;
        getline(cin, choice);

        // Handle user lookup option
        if (choice == "1") {
            while (true) {
                cout << "Enter user email address (or quit): ";
                string email;
                getline(cin, email);
                if (email == "quit") {
                    break;
                }

                // Perform lookup and measure duration
                auto start = chrono::steady_clock::now();
                User* user = userDb.get_user_from_email(email);
                auto stop = chrono::steady_clock::now();

                // Display result and duration
                if (user == nullptr) {
                    cout << "No user in the database has that email address." << endl;
                }
                else {
                    cout << "Found " << user->get_full_name() << endl;
                }
                cout << "Took " << chrono::duration_cast<chrono::microseconds>(stop - start).count() << "µs" << endl;
            }
        }
        // Handle movie lookup option
        else if (choice == "2") {
            while (true) {
                list<string> outputBuf;
                cout << "Enter movie attribute (or quit): ";
                string string_val;
                getline(cin, string_val);
                if (string_val == "quit") {
                    break;
                }

                // Perform lookup and measure duration
                auto start = chrono::steady_clock::now();
                Movie* movie1 = movieDb.get_movie_from_id(string_val);
                if (movie1 != nullptr) {
                    outputBuf.push_back("ID: Found " + movie1->get_title());
                }
                vector<Movie*> movie2 = movieDb.get_movies_with_actor(string_val);
                for (auto movie : movie2) {
                    outputBuf.push_back("Actor: Found " + movie->get_title());
                }
                vector<Movie*> movie3 = movieDb.get_movies_with_director(string_val);
                for (auto movie : movie3) {
                    outputBuf.push_back("Director: Found " + movie->get_title());
                }
                vector<Movie*> movie4 = movieDb.get_movies_with_genre(string_val);
                for (auto movie : movie4) {
                    outputBuf.push_back("Genre: Found " + movie->get_title());
                }
                auto stop = chrono::steady_clock::now();
                for (auto tempVal : outputBuf) {
                    cout << tempVal << endl;
                }
                cout << "Found a total of " << outputBuf.size() << " matches." << endl;
                cout << "Took (*very* roughly) " << (chrono::duration_cast<chrono::microseconds>(stop - start).count()) << "?s" << endl;
                outputBuf.clear();
            }
        }
        else if (choice == "3") {
                // Prompt user for input email and number of recommendations
                cout << "Please enter your email address: ";
                string user_email;
                getline(cin, user_email);
                cout << "How many recommendations would you like to receive? ";
                int num_recommendations;
                cin >> num_recommendations;
                cin.ignore(10000, '\n');

                // Initialize a Recommender object with the user and movie databases
                Recommender recommender(userDb, movieDb);

                // Call the findMatches function with the recommender object, movie database,
                // user email, and number of recommendations
                findMatches(recommender, movieDb, user_email, num_recommendations);
        }
        else if (choice == "9") {
            return 0;
        }
        else {
            cout << "Unknown choice" << endl;
        }
    }
}