#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "species.hpp"
#include <vector>

class Simulation {
  std::vector<SpeciesState> m_species_state;
  std::vector<SpeciesState> m_rel_species_state;
  Parameters const& evolution_parameters;
  
  public:
    Simulation(Parameters const& params);

    SpeciesState back(); //forse aggiungere const

    std::size_t size() const;

    const std::vector<SpeciesState>& getRelSpeciesState() const;

    SpeciesState solve(Parameters const &parameters, SpeciesState const &state, double const delta_t) const;

    void push_back (SpeciesState const &state);

    void evolve(double delta_t);

    void rel_evolve();
};

#endif

//modificare tutti i size_t con i nel codice

//controllare tutte le reference/const