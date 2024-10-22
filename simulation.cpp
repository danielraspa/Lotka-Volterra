#include "simulation.hpp"

#include <cmath>
#include <stdexcept>

Simulation::Simulation(Parameters const& params) : evolution_parameters(params) {}

SpeciesState Simulation::back() {
  return m_species_state.back();
}

std::size_t Simulation::size() const {
  return m_species_state.size();
}

const std::vector<SpeciesState>& Simulation::getRelSpeciesState() const {
  return m_rel_species_state;
}

SpeciesState Simulation::solve(Parameters const &parameters, SpeciesState const &state, double const delta_t) const {
  auto const x =  state.x + (parameters.A - parameters.B * state.y) * state.x * delta_t;
  auto const y =  state.y + (parameters.C * state.x - parameters.D) * state.y * delta_t;
  
  if (x == 0 && y == 0){
    return SpeciesState{0, 0, 0};
  }

  auto const H = - parameters.D * std::log(x) + parameters.C * x + parameters.B * y - parameters.A * std::log(y);
  
  return SpeciesState{x, y, H};
}

void Simulation::push_back (SpeciesState const &state) {
  m_species_state.push_back(state);
}

void Simulation::evolve(double delta_t) {
  if (m_species_state.empty()) {
    throw std::runtime_error{
      "There isn't a initial state."
    };
  }

  //if (m_species_state.back().x == 0 || m_species_state.back().y == 0) {
  //  throw std::runtime_error{                                                         //GESTIRLO MEGLIO
  //    "Initial number of preys and predators must be greater than 0."
  //  };
  //}

  auto state = m_species_state.back();

  auto next_state = solve(evolution_parameters, state, delta_t);

  Simulation::push_back(next_state);

  if (m_species_state.back().x < 0 || m_species_state.back().y < 0) {
    throw std::runtime_error{
      "The system cannot evolve in a negative number of preys and predator, try with a better choice of the evolution parameters, like (1.4, 0.3, 0.1, 2)"
    };
  } 
}

void Simulation::rel_evolve() {
  if (m_species_state.empty()) {
    throw std::runtime_error{
      "There isn't a initial state."
    };
  }

  double x_eq = evolution_parameters.D / evolution_parameters.C;
  double y_eq = evolution_parameters.A / evolution_parameters.B;

  auto last_state = m_species_state.back();

  SpeciesState rel_state;

  rel_state.x = last_state.x / x_eq;
  rel_state.y = last_state.y / y_eq;
  rel_state.H = last_state.H;
  
  m_rel_species_state.push_back(rel_state);

  if (m_species_state.back().x < 0 || m_species_state.back().y < 0) {
    throw std::runtime_error{
      "The system cannot evolve in a negative number of preys and predator" //forse da togliere
    };
  } 
}