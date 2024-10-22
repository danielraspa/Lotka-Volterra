#ifndef SPECIES_HPP
#define SPECIES_HPP

struct Parameters {
 double A{};
 double B{};
 double C{};
 double D{};
};

struct SpeciesState {
  double x{};
  double y{};
  double H{};
};

#endif