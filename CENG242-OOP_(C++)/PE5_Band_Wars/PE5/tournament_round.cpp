#include "tournament_round.h"

// TournamentRound functions goes here

TournamentRound::TournamentRound()
{
    bands = std::list<MusicBand*>();
}
TournamentRound::TournamentRound(std::list<MusicBand*>_bands) 
{
    for (auto it = _bands.begin(); it != _bands.end(); ++it)
    {
        bands.push_back(*it);
    }
}
TournamentRound::TournamentRound(std::vector<MusicBand*>_bands) 
{
    for (auto it = _bands.begin(); it != _bands.end(); ++it)
    {
        bands.push_back(*it);
    }
}

std::size_t TournamentRound::size() { return bands.size(); }
/*
TournamentRound::TournamentRound(TournamentRound& other)
{
    for (auto it = other.bands.begin(); it != other.bands.end(); ++it)
    {
        bands.push_back(*it);
    }
}
TournamentRound::TournamentRound(TournamentRound&& other)
{
    bands = std::move(other.bands);
    other.bands = std::list<MusicBand*>();
}
*/
TournamentRound& TournamentRound::operator=(TournamentRound&& other)
{
    bands.clear();
    bands = std::move(other.bands);
    other.bands = std::list<MusicBand*>();
    return *this;
}
TournamentRound& TournamentRound::get_next_round()
{
    std::vector<MusicBand*> winners;
    int size = bands.size();
    int repeat = (size+1)/2;
    auto fromFront = bands.begin();
    auto fromBack = bands.rend();
    fromBack++;
    int scoreFront, scoreBack, fanChange;
    for (int i = 0; i < repeat; ++i)
    {
        MusicBand *frontBand = *fromFront, *backBand = *fromBack;
        if (frontBand == backBand)
        {
            winners.push_back(frontBand);
            break;
        }
        if (frontBand->get_energy() <= 0)
        {
            winners.push_back(backBand);
            fromFront++;
            fromBack++;
            continue;
        }
        if (backBand->get_energy() <= 0)
        {
            winners.push_back(frontBand);
            fromFront++;
            fromBack++;
            continue;
        }
        scoreFront = frontBand->play(backBand);
        scoreBack = backBand->play(frontBand);
        if (scoreFront > scoreBack)
        {
            fanChange = abs(scoreFront - scoreBack);
            if (fanChange > backBand->get_fan_count()) fanChange = backBand->get_fan_count();
            frontBand->set_fan_count(frontBand->get_fan_count() + fanChange);
            backBand->set_fan_count(backBand->get_fan_count() - fanChange);
            winners.push_back(frontBand);
        }
        else if (scoreFront == scoreBack)
        {
            winners.push_back(frontBand);
        }
        else if (scoreFront < scoreBack)
        {
            fanChange = abs(scoreFront - scoreBack);
            if (fanChange > frontBand->get_fan_count()) fanChange = frontBand->get_fan_count();
            frontBand->set_fan_count(frontBand->get_fan_count() - fanChange);
            backBand->set_fan_count(backBand->get_fan_count() + fanChange);
            winners.push_back(backBand);
        }
        fromFront++;
        fromBack++;
    }
    return *(new TournamentRound(winners));
}

std::ostream& operator<< (std::ostream &os, TournamentRound &other)
{
    int size = other.size();
    auto it = other.bands.begin();
    for (int i = 0; i < size; ++i)
    {
        os << (*it)->get_name();
        if (i != size - 1) os << "\t";
        ++it;
    }
    return os;
}