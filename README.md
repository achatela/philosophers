# Dining Philosophers Simulation

## Overview

The Dining Philosophers problem is a classic synchronization problem dealing with managing resources (forks) among processes (philosophers) in a concurrent programming environment. This project simulates the behavior of philosophers who alternately eat, think, and sleep around a circular table with a single fork between each pair of philosophers.

## Problem Statement

- **Philosophers** sit around a round table with a large bowl of spaghetti.
- Each philosopher thinks, eats, and sleeps. When eating, they are not thinking or sleeping and vice versa.
- A philosopher needs two forks to eat and must pick up the left and right fork adjacent to them.
- The simulation ends if a philosopher dies of starvation, which happens if they don't begin eating within a certain time after their last meal or the start of the simulation.

## Program Functionality

- The program handles multiple philosophers (threads) each requiring two forks (mutexes) to eat without causing deadlock or resource starvation.
- Philosophers’ actions are logged with precise timestamps, ensuring clarity on their actions and the sequence of events leading up to their deaths, if any.
- The simulation can be parameterized by the number of philosophers, the time to die, eat, and sleep, and optionally, the minimum number of meals each philosopher should have.

## Usage

To compile the program, run:

```bash
make all
```

To start the simulation, execute:
```bash
./dining_philosophers <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_meals]
```

Example:
```bash
./philo 5 800 200 200
```