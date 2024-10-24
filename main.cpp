#include <iostream>
#include <fstream>
#include <limits>
#include <SFML/Graphics.hpp>


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
              << "- draw the orbit of the evolution [d val] \n" //da implementare
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
      } else if (cmd == 'g' && std::cin >> delta_t) {
        sf::RenderWindow window(sf::VideoMode(800, 600), "Simulation Graph"); //creazione finestra 

        sf::VertexArray axes (sf::Lines, 4);
        sf::VertexArray prey_graph(sf::LineStrip); //LineStrip -> List of connected lines, a point uses the previous point to form a line.
        sf::VertexArray predator_graph(sf::LineStrip);
        sf::VertexArray const_of_motion_graph(sf::LineStrip);

        axes[0] = sf::Vertex(sf::Vector2f(50, 550), sf::Color::Black); //begin of x-axis
        axes[1] = sf::Vertex(sf::Vector2f(750, 550), sf::Color::Black); //end of x-axis
        axes[2] = sf::Vertex(sf::Vector2f(50, 550), sf::Color::Black); //begin of y-axis
        axes[3] = sf::Vertex(sf::Vector2f(50, 50), sf::Color::Black); //end of y-axis

        float t{0}; //Vector2f needs float
        float x_axis_offset{50}; //to fix the x offset in SFML's window
        float y_axis_offset{550}; //to fix the y offset in SFML's window

        while (window.isOpen())
        {
          sf::Event event; //define a system event and its parameter

          while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
            window.close();
            }
          }

          m_species_state.evolve(delta_t);

          prey_graph.append(sf::Vertex(sf::Vector2f((t + static_cast<float>(delta_t)) + x_axis_offset, - (static_cast<float>(m_species_state.back().x) - y_axis_offset)), sf::Color::Blue)); //- to flip the y-axis
          predator_graph.append(sf::Vertex(sf::Vector2f((t + static_cast<float>(delta_t)) + x_axis_offset, - (static_cast<float>(m_species_state.back().y) - y_axis_offset)), sf::Color::Red));
          const_of_motion_graph.append(sf::Vertex(sf::Vector2f((t + static_cast<float>(delta_t)) + x_axis_offset, - (static_cast<float>(m_species_state.back().H) - y_axis_offset)), sf::Color::Magenta));

          t += static_cast<float>(delta_t);

          window.clear(sf::Color::White);

          window.draw(axes);
          window.draw(prey_graph);
          window.draw(predator_graph);
          window.draw(const_of_motion_graph);

          window.display();
        }
        
      } else if(cmd == 'q') {
        return EXIT_SUCCESS;
      }     
    }
  } catch (const std::runtime_error& e) {                                   //da sistemare alla fine
    std::cerr << "Runtime error: " << e.what() << '\n';                     //da sistemare alla fine
  }
}


//gestire l'm_species_state.size() - forse fatto