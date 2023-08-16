:- module(main, [is_vote_wasted/2, is_candidate_elected/2, candidate_count_from_city/3, all_parties/1, all_candidates_from_party/2, all_elected_from_party/2, election_rate/2, council_percentage/2, alternative_debate_setups/2]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE

% PART A %

is_vote_wasted(City, PoliticalParty) :- 
    not((elected(City, PoliticalParty, X) , X > 0)).

is_candidate_elected(Name, PoliticalParty) :- 
    candidate(Name, PoliticalParty, City, Row),
    elected(City, PoliticalParty, X),
    X >= Row.

candidate_count_from_city([], _, Count) :- Count is 0.
candidate_count_from_city([H|T], GivenCity, Count) :-
    candidate(H, _, GivenCity, _),
    candidate_count_from_city(T, GivenCity, Count1),
    Count is Count1 + 1.
candidate_count_from_city([H|T], GivenCity, Count) :-
    not(candidate(H, _, GivenCity, _)),
    candidate_count_from_city(T, GivenCity, Count).

% PART B %

all_parties(ListOfPoliticalParties) :-
    findall(X, party(X,_), ListOfPoliticalParties).

all_candidates_from_party(PoliticalParty, ListOfCandidates) :-
    findall(X, candidate(X, PoliticalParty, _, _), ListOfCandidates).

all_elected_from_party(PoliticalParty, ListOfCandidates) :-
    findall(X, is_candidate_elected(X, PoliticalParty), ListOfCandidates).

election_rate(PoliticalParty, Rate) :-
    all_candidates_from_party(PoliticalParty, ListOfCandidates),
    all_elected_from_party(PoliticalParty, ListOfElected),
    length(ListOfCandidates, CandidateCount),
    length(ListOfElected, ElectedCount),
    Rate is ElectedCount / CandidateCount.

council_percentage(PoliticalParty, Rate) :-
    all_elected_from_party(PoliticalParty, ListOfElected),
    length(ListOfElected, ElectedCount),
    to_elect(CountOfTotal),
    Rate is ElectedCount / CountOfTotal.

% PART C %

ads_helper([], []).
ads_helper([Hi|Ti], [Hc|Tc]) :-
    party(Party, Hi),
    candidate(Hc, Party, _, _),
    ads_helper(Ti, Tc),
    not(member(Hc, Tc)).

alternative_debate_setups(DescriptionString, OrderedListOfCandidates) :-
    string_chars(DescriptionString, DescriptionList),
    ads_helper(DescriptionList, OrderedListOfCandidates).