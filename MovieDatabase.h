#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include "treemm.h"

class Movie;

class MovieDatabase
{
public:
    MovieDatabase();
    ~MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

private:
    TreeMultimap<std::string, Movie*> m_id_movie_map;
    TreeMultimap<std::string, Movie*> m_director_movie_map;
    TreeMultimap<std::string, Movie*> m_actor_movie_map;
    TreeMultimap<std::string, Movie*> m_genre_movie_map;
    std::vector<Movie*> m_movies;
};

#endif // MOVIEDATABASE_INCLUDED