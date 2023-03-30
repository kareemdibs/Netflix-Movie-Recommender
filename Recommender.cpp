#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
using namespace std;

// Define the constructor for the Recommender class which takes in two parameters:
// 1) a reference to a constant UserDatabase object called "user_database"
// 2) a reference to a constant MovieDatabase object called "movie_database"
Recommender::Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database) {
    // Use const_cast to remove the const qualifier from the input references
    // and assign the resulting pointer to the member variable "m_user_database"
    m_user_database = const_cast<UserDatabase*>(&user_database);

    // Use const_cast to remove the const qualifier from the input references
    // and assign the resulting pointer to the member variable "m_movie_database"
    m_movie_database = const_cast<MovieDatabase*>(&movie_database);
}

// Returns true if movie1 should be sorted before movie2 based on their scores, ratings, and names.
bool Recommender::customCompare(const AuxiliaryMovieAndRank& movie1, const AuxiliaryMovieAndRank& movie2) {
    if (movie1.m_movie_score > movie2.m_movie_score) {
        // If movie1 has a higher score than movie2, then it should be sorted before movie2
        return true;
    }
    else if (movie1.m_movie_score < movie2.m_movie_score) {
        // If movie1 has a lower score than movie2, then it should be sorted after movie2
        return false;
    }
    else { // Scores are equal
        if (movie1.m_movie_rating > movie2.m_movie_rating) {
            // If movie1 has a higher rating than movie2, then it should be sorted before movie2
            return true;
        }
        else if (movie1.m_movie_rating < movie2.m_movie_rating) {
            // If movie1 has a lower rating than movie2, then it should be sorted after movie2
            return false;
        }
        else { // Scores and ratings are equal
            // Sort movies alphabetically by their names
            return movie1.m_movie_name < movie2.m_movie_name;
        }
    }
}

// This function takes in a user's email and the number of recommended movies to output
// It uses a compatibility score to recommend movies that are related to movies the user has watched before
vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const {
    // If movie_count is not a positive integer, return an empty vector
    if (movie_count <= 0) {
        vector<MovieAndRank> empty_vector_recs;
        return empty_vector_recs;
    }

    // Get the user from the user database using their email
    User* m_user = m_user_database->get_user_from_email(user_email);

    // Get a vector of movie IDs that the user has watched
    vector<string> movies_watched_ids = m_user->get_watch_history();

    // Create a vector of movie objects for each watched movie ID
    vector<Movie*> movies_watched_vector;
    for (int a = 0; a < movies_watched_ids.size(); a++) {
        Movie* tempMovie = m_movie_database->get_movie_from_id(movies_watched_ids[a]);
        movies_watched_vector.push_back(tempMovie);
    }
    
    // Create an unordered map to keep track of the compatibility score of movies
    // Key: movie object pointer
    // Value: compatibility score
    unordered_map<Movie*, int> compatibility_map;

    // For each movie the user has watched
    for (int i = 0; i < movies_watched_vector.size(); i++) {

        // Find all directors of the movie
        vector<string> movie_directors_vector = movies_watched_vector[i]->get_directors();

        // For every director of the movie
        for (int q = 0; q < movie_directors_vector.size(); q++) {
            // Find all movies with that particular director
            vector<Movie*> movies_with_director = m_movie_database->get_movies_with_director(movie_directors_vector[q]);

            // Add 20 points to their compatibility score and insert into the unordered map
            for (int v = 0; v < movies_with_director.size(); v++) {
                compatibility_map[movies_with_director[v]] += 20;
            }
        }

        // Find all actors of the movie
        vector<string> movie_actors_vector = movies_watched_vector[i]->get_actors();
        
        // For every actor of the movie
        for (int q = 0; q < movie_actors_vector.size(); q++) {
            // Find all movies with that actor
            vector<Movie*> movies_with_actor = m_movie_database->get_movies_with_actor(movie_actors_vector[q]);

            // Add 30 points to their compatibility score and insert into the unordered map if not already in
            for (int v = 0; v < movies_with_actor.size(); v++) {
                compatibility_map[movies_with_actor[v]] += 30;
            }
        }

        // Find all genres of the movie
        vector<string> movie_genres_vector = movies_watched_vector[i]->get_genres();
        
        // For each genre of the movie
        for (int q = 0; q < movie_genres_vector.size(); q++) {

            // find all movies with that genre
            vector<Movie*> movies_with_genre = m_movie_database->get_movies_with_genre(movie_genres_vector[q]);

            // add 1 point to their compatibility score and insert into the unordered map if not already in
            for (int v = 0; v < movies_with_genre.size(); v++) {
                compatibility_map[movies_with_genre[v]] += 1;
            }
        }
    }

    // Remove movies that the user has already watched from the compatibility map
    for (unordered_map<Movie*, int>::iterator it = compatibility_map.begin(); it != compatibility_map.end(); ) {
        if (find(movies_watched_vector.begin(), movies_watched_vector.end(), it->first) != movies_watched_vector.end()) {
            it = compatibility_map.erase(it);
        }
        else {
            ++it;
        }
    }

    // Convert the filtered compatibility map into a vector of auxiliary movie-and-rank objects
    vector<AuxiliaryMovieAndRank> auxiliary_vector;
    for (unordered_map<Movie*, int>::iterator m = compatibility_map.begin(); m != compatibility_map.end(); m++) {
        
        string func_movie_id = (m->first)->get_id();
        
        float func_movie_rating = (m->first)->get_rating();
        
        string func_movie_name = (m->first)->get_title();
        
        int func_compatibility_score = m->second;

        AuxiliaryMovieAndRank funcMovieRank(func_movie_id, func_compatibility_score, func_movie_rating, func_movie_name);
        auxiliary_vector.push_back(funcMovieRank);
    }

    // Sort the auxiliary vector by decreasing compatibility score and increasing movie ID
    sort(auxiliary_vector.begin(), auxiliary_vector.end(), &customCompare);

    // Create a vector of movie-and-rank objects with at most movie_count recommendations
    vector<MovieAndRank> recommendations_vector;
    for (int c = 0; c < movie_count; c++) {
        string movieID = auxiliary_vector[c].m_movie_id;
        int compatibilityScore = auxiliary_vector[c].m_movie_score;

        MovieAndRank temp(movieID, compatibilityScore);
        recommendations_vector.push_back(temp);
    }

    return recommendations_vector;
}