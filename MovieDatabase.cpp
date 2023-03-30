#include "Movie.h"
#include "MovieDatabase.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

MovieDatabase::MovieDatabase() {}

MovieDatabase::~MovieDatabase() {
    for (int j = 0; j < m_movies.size(); j++) {
        delete m_movies[j];
    }
}

bool MovieDatabase::load(const string& filename)
{
    // Open input file stream
    ifstream infile(filename);

    // Read first line (the ID of the movie) and loop through each line of the file
    string tempStr;

    // get first line (the ID of the movie)
    while (getline(infile, tempStr)) {

        // Extract movie ID from the current line
        string tempID = tempStr;

        // Extract movie name from the next line
        string tempName;
        if (!getline(infile, tempName)) {
            return false;
        }

        // Extract movie release year from the next line
        string tempReleaseYear;
        if (!getline(infile, tempReleaseYear)) {
            return false;
        }

        // Extract directors data from the next line and split by commas to create a vector of director names
        string input1;
        vector<string> directorsAdjustCommas;
        if (!getline(infile, input1)) {
            return false;
        }
        stringstream directors_stream(input1);
        while (getline(directors_stream, input1, ',')) {
            directorsAdjustCommas.push_back(input1);
        }

        // Extract actors data from the next line and split by commas to create a vector of actor names
        vector<string> actorsAdjustCommas;
        if (!getline(infile, input1)) {
            return false;
        }
        stringstream actors_stream(input1);
        while (getline(actors_stream, input1, ',')) {
            actorsAdjustCommas.push_back(input1);
        }

        // Extract genres data from the next line and split by commas to create a vector of genre names
        vector<string> genresAdjustCommas;
        if (!getline(infile, input1)) {
            return false;
        }
        stringstream genres_stream(input1);
        while (getline(genres_stream, input1, ',')) {
            genresAdjustCommas.push_back(input1);
        }

        // Extract movie rating as a string from the next line and convert it to float
        string tempRating;
        if (!getline(infile, tempRating)) {
            return false;
        }
        float flo_tempRating = stof(tempRating);

        // Create a new instance of the Movie class with the extracted data
        Movie* m_movie = new Movie(tempID, tempName, tempReleaseYear, directorsAdjustCommas, actorsAdjustCommas, genresAdjustCommas, flo_tempRating);

        // Insert movies into m_movies vector to be deleted later
        m_movies.push_back(m_movie);

        // Associate the movie with its ID
        m_id_movie_map.insert(tempID, m_movie);

        int q;

        // Associate the movie with its directors in the director-movie multimap
        for (q = 0; q < directorsAdjustCommas.size(); q++) {
            m_director_movie_map.insert(directorsAdjustCommas[q], m_movie);
        }

        // Associate the movie with its actors in the actor-movie multimap
        for (q = 0; q < actorsAdjustCommas.size(); q++) {
            m_actor_movie_map.insert(actorsAdjustCommas[q], m_movie);
        }

        // Associate the movie with its genres in the genre-movie multimap
        for (q = 0; q < genresAdjustCommas.size(); q++) {
            m_genre_movie_map.insert(genresAdjustCommas[q], m_movie);
        }

        // Skip the new line
        getline(infile, tempStr);
    }
    // Successfully loaded movie data from the file
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const {
    // Find the iterator that corresponds to the given ID in the map that maps movie IDs to movie pointers.
    TreeMultimap<string, Movie*>::Iterator it = m_id_movie_map.find(id);

    // Each movie has a unique ID.

    // If the iterator is valid, then the movie with the given ID exists in the map and can be returned.
    if (it.is_valid()) {
        Movie* m_id_movie = it.get_value();
        return m_id_movie;
    }
    // If the iterator is invalid, then the movie with the given ID does not exist in the map and nullptr is returned.
    else {
        return nullptr;
    }
}

// Returns a vector of Movie pointers associated with the given director
vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const {
    // Find all the movies associated with the given director
    TreeMultimap<string, Movie*>::Iterator it = m_director_movie_map.find(director);

    // Create a vector to store all the movies associated with the director
    vector<Movie*> movies_with_director;

    // Iterate through the movies associated with the director and add them to the vector
    while (it.is_valid()) {
        movies_with_director.push_back(it.get_value());
        it.advance();
    }

    // Return the vector of movies associated with the director
    return movies_with_director;
}

// Returns a vector of Movie pointers that feature the specified actor.
vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const {
    // Find the iterator for the given actor in the actor-movie map
    TreeMultimap<string, Movie*>::Iterator it = m_actor_movie_map.find(actor);

    // Create an empty vector to store the movies with the given actor
    vector<Movie*> movies_with_actor;

    // Iterate through the iterator to add each movie with the given actor to the vector
    while (it.is_valid()) {
        movies_with_actor.push_back(it.get_value());
        it.advance();
    }

    // Return the vector of movies with the given actor
    return movies_with_actor;
}

// Returns a vector of Movie pointers associated with the given genre
vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const {
    // Find the iterator for the given genre in the genre-movie map
    TreeMultimap<string, Movie*>::Iterator it = m_genre_movie_map.find(genre);

    // Create a vector to store movies that match the given genre
    vector<Movie*> matching_movies;

    // Iterate through the iterator as long as it is valid
    while (it.is_valid()) {
        // Add the movie pointer to the matching_movies vector
        matching_movies.push_back(it.get_value());
        // Move the iterator to the next movie with the same genre
        it.advance();
    }

    // Return the vector of movies that match the given genre
    return matching_movies;
}