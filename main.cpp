#include <iostream>
#include <fstream>
#include <limits>


#include "simulation.hpp"

int main() {
  double a{}, b{}, c{}, d{}; 
  double x_0{0}, y_0{0};
  while (true) {
    std::cout << "Insert the parameters of the evolution(A B C D): \n";
    std::cin >> a >> b >> c >> d;

    std::cout << "Insert the initial conditions x(0) and y(0): \n";
    std::cin >> x_0 >> y_0;

    if(std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Insert corrects values for the parameters and the initial conditions. \n" << '\n';
    } else {
      break;
    }
  }

  Parameters parameters{a, b, c, d}; 
  SpeciesState initial_state{x_0, y_0, 0.0};

  Simulation m_species_state(parameters);
  m_species_state.push_back(initial_state);

  std::vector<double> time_evolutions;

  try{ 
    std::cout << "Valid commands: \n"
              << "- compute an evolution of delta t in seconds[t val] (choose for stability a value of the order of 0.001) \n" //da implementare - ok
              << "- print on screen the evolutions [p] \n" //da implementare - ok
              << "- save the evolutions in an output file [s] \n" //da implementare -ok
              << "- draw the orbit of the evolution [d value] \n" //da implementare
              << "- draw the x, y and H trends [g val] \n" //da implementare
              << "- quit [q] \n";

    char cmd{};

    while (std::cin >> cmd) {
      double delta_t{};
      
      if (cmd == 't' && std::cin >> delta_t) {
        m_species_state.evolve(delta_t);
        m_species_state.rel_evolve();
        time_evolutions.push_back(delta_t);
      } else if (cmd == 'p') {
        std::cout << "Results are expressed as fractions with respect to the equilibrium points" << '\n'; //aggiungere i punti di equilibrio
        for (std::size_t i = 0; i < m_species_state.size() - 1; i++) {
          std::cout << "Evolution " << i + 1 << " of " << "delta t = " << time_evolutions[i] << '\n' //aggiungere che manca il delta t precedente
                    << "x = " << m_species_state.getRelSpeciesState()[i].x << '\n'
                    << "y = " << m_species_state.getRelSpeciesState()[i].y << '\n'
                    << "H = " << m_species_state.getRelSpeciesState()[i].H << '\n';
        }
      } else if (cmd == 's') {
        std::ofstream outfile ("evolution.txt");

        if (!outfile) {
          std::cerr << "File couldn't be opened." << '\n';
        } 
        for (std::size_t i = 0; i < m_species_state.size() - 1; i++) {
          outfile << "Evolution " << i + 1 << " of " << "delta t = " << time_evolutions[i] << '\n' 
                  << "x = " << m_species_state.getRelSpeciesState()[i].x << '\n'
                  << "y = " << m_species_state.getRelSpeciesState()[i].y << '\n'
                  << "H = " << m_species_state.getRelSpeciesState()[i].H << '\n';
        }

        outfile.close();

        std::cout << "Evolutions have been written in the evolution.txt file." << '\n';
      } else if(cmd == 'q') {
        return EXIT_SUCCESS;
      }     
    }
  } catch (const std::runtime_error& e) {                                   //da sistemare alla fine
    std::cerr << "Runtime error: " << e.what() << '\n';                     //da sistemare alla fine
  }
}


//gestire l'm_species_state.size() - forse fatto