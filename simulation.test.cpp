#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "simulation.hpp"

#include "doctest.h"

TEST_CASE("Test equilibrium points") {
  Parameters parameters{1.6, 3.6, 5.9, 4.9};

  SUBCASE ("Species extintion") {
    Simulation m_species_state(parameters);
    SpeciesState initial_state{0.0, 0.0, 0.0};
    m_species_state.push_back(initial_state);
   
    m_species_state.evolve(0.001);

    CHECK(m_species_state.back().x == 0);
    CHECK(m_species_state.back().y == 0);
    CHECK(m_species_state.back().H == 0);   

    m_species_state.rel_evolve();

    CHECK(m_species_state.getRelSpeciesState().back().x == 0);
    CHECK(m_species_state.getRelSpeciesState().back().y == 0);
    CHECK(m_species_state.getRelSpeciesState().back().H == 0);
  }

  SUBCASE ("Predators eat all the newborn prays") {
    Simulation m_species_state(parameters);
    SpeciesState initial_state{0.8305084746, 0.4444444444, 8.70750236};
    m_species_state.push_back(initial_state);

    m_species_state.evolve(0.001);

    CHECK(m_species_state.back().x == doctest::Approx(0.8305084746));
    CHECK(m_species_state.back().y == doctest::Approx(0.4444444444));
    CHECK(m_species_state.back().H == doctest::Approx(8.70750236));  

    m_species_state.rel_evolve();
    CHECK(m_species_state.getRelSpeciesState().back().x == doctest::Approx(1));
    //add y test
  }
}

TEST_CASE("Check the sign of x and y") {
  SUBCASE("Check positive  value"){
    Parameters parameters{1.6, 3.6, 5.9, 4.9};
    Simulation m_species_state(parameters);
    SpeciesState initial_state{240., 120, 1813.484882};
    m_species_state.push_back(initial_state);

    m_species_state.evolve(0.001);

    CHECK(m_species_state.back().x > 0);
    CHECK(m_species_state.back().y > 0);
    CHECK(m_species_state.back().H == doctest::Approx(1813.).epsilon(0.1));  

    //??????????

    initial_state = {1300., 900., 10863.98258};

    m_species_state.evolve(0.0001);

    CHECK(m_species_state.back().x > 0);
    CHECK(m_species_state.back().y > 0);
    CHECK(m_species_state.back().H == doctest::Approx(1813.484882).epsilon(0.1));    
  }

  SUBCASE("Check negative value"){
    Parameters parameters{1.6, 500., 5.9, 4.9};
    Simulation m_species_state(parameters);
    SpeciesState initial_state{20., 37., 1813.484882};
    m_species_state.push_back(initial_state);

    CHECK_THROWS(m_species_state.evolve(0.0001));    
  }
}

TEST_CASE("Check whith random values") {
  Parameters parameters{1.4, 0.3, 0.1, 2};
  Simulation m_species_state(parameters);
  SpeciesState initial_state{340., 450., 148.620981};
  m_species_state.push_back(initial_state);
   
  m_species_state.evolve(0.001);

  CHECK(m_species_state.back().x == doctest::Approx(294.576));
  CHECK(m_species_state.back().y == doctest::Approx(464.4));
  CHECK(m_species_state.back().H == doctest::Approx(148.620981).epsilon(0.1));

  m_species_state.rel_evolve();
  CHECK(m_species_state.getRelSpeciesState().back().x == doctest::Approx(14.7288));
  CHECK(m_species_state.getRelSpeciesState().back().y == doctest::Approx(99.51428571));  
}

TEST_CASE("Check after multiple time evolutions") {
  Parameters parameters{1.4, 0.3, 0.1, 2};
  Simulation m_species_state(parameters);
  SpeciesState initial_state{340., 450., 148.7891621};
  m_species_state.push_back(initial_state);

  for (int i = 1; i < 4; i++){ 
    m_species_state.evolve(0.001);
    m_species_state.rel_evolve();
  }  

  CHECK(m_species_state.back().x == doctest::Approx(217.952108));
  CHECK(m_species_state.back().y == doctest::Approx(488.3141726));
  CHECK(m_species_state.back().H == doctest::Approx(148.8535693).epsilon(0.1));

  CHECK(m_species_state.getRelSpeciesState().back().x == doctest::Approx(10.8976054));
  CHECK(m_species_state.getRelSpeciesState().back().y == doctest::Approx(104.6387513));    
}

TEST_CASE("Test of the size() method") {
  Parameters parameters{1.4, 0.3, 0.1, 2};
  Simulation m_species_state(parameters);

  CHECK(m_species_state.size() == 0);
  CHECK(m_species_state.getRelSpeciesState().size() == 0);

  SpeciesState initial_state{340., 450., 148.7891621};
  m_species_state.push_back(initial_state);

  CHECK(m_species_state.size() == 1);
  CHECK(m_species_state.getRelSpeciesState().size() == 0);

  m_species_state.evolve(0.001);
  m_species_state.rel_evolve();

  CHECK(m_species_state.size() == 2);
  CHECK(m_species_state.getRelSpeciesState().size() == 1);

  for (std::size_t i = 0; i < 20; i++) {
    m_species_state.evolve(0.001);
    m_species_state.rel_evolve();
  }

  CHECK(m_species_state.size() == 22);
  CHECK(m_species_state.getRelSpeciesState().size() == 21);
}