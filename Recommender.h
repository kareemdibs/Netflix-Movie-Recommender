#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
        : movie_id(id), compatibility_score(score) {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
public:
    Recommender(const UserDatabase& user_database,
        const MovieDatabase& movie_database);
    //~Recommender();
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
        int movie_count) const;

private:
    UserDatabase* m_user_database;
    MovieDatabase* m_movie_database;

    struct AuxiliaryMovieAndRank 
    {
        AuxiliaryMovieAndRank(std::string movie_id, int movie_score, float movie_rating, const std::string movie_name) : m_movie_id(movie_id), m_movie_score(movie_score), m_movie_name(movie_name), m_movie_rating(movie_rating) {}

        std::string m_movie_id;
        int m_movie_score;
        float m_movie_rating;
        std::string m_movie_name;
    };

    static bool customCompare(const AuxiliaryMovieAndRank& Movie1, const AuxiliaryMovieAndRank& Movie2);
};

#endif // RECOMMENDER_INCLUDED